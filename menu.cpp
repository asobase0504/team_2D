//==================================================
// 
// 3Dゲーム制作 ( menu.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "input.h"
#include "menu.h"
#include "fade.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_MENU					(16)		// メニューの最大数
#define NORMAL_BLINK_SPEED			(0.01f)		// 通常時の点滅速度
#define DECISION_BLINK_SPEED		(0.1f)		// 決定時の点滅速度
#define MIN_ALPHA					(0.6f)		// α値の最小値

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ 選択肢 ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXCOLOR				col;			// 色
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
}Option;

/*↓ メニュー ↓*/

typedef struct
{
	D3DXVECTOR3		pos;					// 位置
	Option			Option[MAX_OPTION];		// 選択肢の情報
	int				nNumUse;				// 使用数
	float			fWidth;					// 幅
	float			fHeight;				// 高さ
	float			fInterval;				// 選択肢の間隔
	float			fBlinkSpeed;			// 点滅速度
	bool			bUse;					// 使用しているかどうか
}Menu;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// 頂点バッファへのポインタ
static Menu							s_aMenu[MAX_MENU];				// メニューの情報
static int							s_nSelectMenu;					// 選ばれているメニュー
static int							s_nSelectOption;				// 選ばれている選択肢
static int							s_nAlphaTime;					// α値変更用の時間

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void SetColor(const Option &option);
static void Input(Menu *pMenu, Option *pOption);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMenu(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nSelectMenu = 0;
	s_nSelectOption = 0;
	s_nAlphaTime = 0;

	// メモリのクリア
	memset(s_aMenu, 0, sizeof(s_aMenu));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_MENU * MAX_OPTION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_MENU; i++)
	{
		for (int j = 0; j < MAX_OPTION; j++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;
		}
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMenu(void)
{
	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMenu(void)
{
	Menu *pMenu = &s_aMenu[s_nSelectMenu];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}

	s_nAlphaTime++;

	Option *pOption = &pMenu->Option[s_nSelectOption];

	pOption->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 色の設定
	SetColor(*pOption);

	// 入力
	Input(pMenu, pOption);

	float fCurve = cosf((s_nAlphaTime * pMenu->fBlinkSpeed) * (D3DX_PI * 2.0f));

	pOption->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, (fCurve * (1.0f - MIN_ALPHA)) + MIN_ALPHA);
	
	// 色の設定
	SetColor(*pOption);
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMenu(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (!pMenu->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, pMenu->Option[j].pTexture);

			int nIdx = (i * MAX_MENU) + (j * 4);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				nIdx,						// 描画する最初の頂点インデックス
				2);							// プリミティブ(ポリゴン)数
		}
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetMenu(const MenuArgument &menu)
{
	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	int nIdx = 0;

	for (int nIdx = 0; nIdx < MAX_MENU; nIdx++)
	{
		Menu *pMenu = &s_aMenu[nIdx];

		if (pMenu->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);

		pMenu->pos = D3DXVECTOR3(fPosX, menu.fTop, 0.0f);
		pMenu->nNumUse = menu.nNumUse;

		pMenu->fWidth = menu.fRight - menu.fLeft;
		pMenu->fHeight = menu.fBottom - menu.fTop;
		pMenu->fInterval = pMenu->fHeight / (menu.nNumUse + 1);
		pMenu->fBlinkSpeed = NORMAL_BLINK_SPEED;

		pMenu->bUse = true;

		s_nSelectMenu = nIdx;
		s_nSelectOption = 0;

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			Option *pOption = &pMenu->Option[j];

			pOption->pos = D3DXVECTOR3(pMenu->pos.x, menu.fTop + (pMenu->fInterval * (j + 1)), 0.0f);
			pOption->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pOption->fWidth = menu.fWidth;
			pOption->fHeight = menu.fHeight;
			pOption->pTexture = *menu.pTexture[j];

			// 頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdx * MAX_MENU) + (j * 4);		// 指定の位置までポインタを進める

			float fWidth = menu.fWidth * 0.5f;
			float fHeight = menu.fHeight * 0.5f;

			// 頂点座標の設定
			pVtx[0].pos = pOption->pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
			pVtx[1].pos = pOption->pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
			pVtx[2].pos = pOption->pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
			pVtx[3].pos = pOption->pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = pOption->col;
			pVtx[1].col = pOption->col;
			pVtx[2].col = pOption->col;
			pVtx[3].col = pOption->col;

			// 頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
		}

		break;
	}

	return nIdx;
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetMenu(int nIdx)
{
	s_nSelectMenu = 0;
	s_nSelectOption = 0;
	s_nAlphaTime = 0;
	s_aMenu[nIdx].bUse = false;
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
static void SetColor(const Option &option)
{
	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_nSelectMenu * MAX_MENU) + (s_nSelectOption * 4);		// 指定の位置までポインタを進める

	// 頂点カラーの設定
	pVtx[0].col = option.col;
	pVtx[1].col = option.col;
	pVtx[2].col = option.col;
	pVtx[3].col = option.col;

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void Input(Menu *pMenu, Option *pOption)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetDirectJoypadTrigger(JOYKEY_CROSS_UP) /*|| GetDirectJoypadStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP)*/)
	{// Wキーが押されたかどうか
		s_nSelectOption = ((s_nSelectOption - 1) + pMenu->nNumUse) % pMenu->nNumUse;
		pOption = &pMenu->Option[s_nSelectOption];
		s_nAlphaTime = 0;
	}
	else if (GetKeyboardTrigger(DIK_S) || GetDirectJoypadTrigger(JOYKEY_CROSS_DOWN) /*|| GetDirectJoypadStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN)*/)
	{// Sキーが押されたかどうか
		s_nSelectOption = ((s_nSelectOption + 1) + pMenu->nNumUse) % pMenu->nNumUse;
		pOption = &pMenu->Option[s_nSelectOption];
		s_nAlphaTime = 0;
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetDirectJoypadTrigger(JOYKEY_DIRECT_1_BUTTON))
	{//決定キー(ENTERキー)が押されたかどうか
		pMenu->fBlinkSpeed = DECISION_BLINK_SPEED;
	}
}
