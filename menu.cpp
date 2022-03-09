//==================================================
// 
// FPG制作 ( menu.cpp )
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
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
}Option;

/*↓ メニュー ↓*/

typedef struct
{
	D3DXVECTOR3				pos;					// 位置
	LPDIRECT3DTEXTURE9		pTexture;				// テクスチャ
	Option					Option[MAX_OPTION];		// 選択肢の情報
	int						nNumUse;				// 使用数
	float					fWidth;					// 幅
	float					fHeight;				// 高さ
	float					fInterval;				// 選択肢の間隔
	float					fBlinkSpeed;			// 点滅速度
	bool					bFrame;					// 枠がいるかどうか [ true : いる false : いらない ]
	bool					bUse;					// 使用しているかどうか
}Menu;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMenu = NULL;			// メニューの頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffOption = NULL;		// 選択肢の頂点バッファへのポインタ
static Menu							s_aMenu[MAX_MENU];				// メニューの情報
static int							s_nIdxMenu;						// 選ばれているメニューの番号
static int							s_nIdxOption;					// 選ばれている選択肢の番号
static int							s_nAlphaTime;					// α値変更用の時間

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void ChangeColor(Menu *pMenu);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitMenu(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;

	// メモリのクリア
	memset(s_aMenu, 0, sizeof(s_aMenu));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMenu,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_MENU * MAX_OPTION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffOption,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

								// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_MENU; i++)
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

	// 頂点バッファをアンロックする
	s_pVtxBuffMenu->Unlock();

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

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
	s_pVtxBuffOption->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMenu(void)
{
	if (s_pVtxBuffMenu != NULL)
	{// 頂点バッファの解放
		s_pVtxBuffMenu->Release();
		s_pVtxBuffMenu = NULL;
	}

	if (s_pVtxBuffOption != NULL)
	{// 頂点バッファの解放
		s_pVtxBuffOption->Release();
		s_pVtxBuffOption = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMenu(void)
{
	Menu *pMenu = &s_aMenu[s_nIdxMenu];

	if (!pMenu->bUse)
	{// 使用していない
		return;
	}

	// 色の変更
	ChangeColor(pMenu);
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMenu(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (!pMenu->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		if (pMenu->bFrame)
		{// 枠がいる
		 // 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, s_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			if (pMenu->pTexture != NULL)
			{// テクスチャがある
				pDevice->SetTexture(0, pMenu->pTexture);
			}
			else
			{// テクスチャがない
				pDevice->SetTexture(0, NULL);
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				i * 4,						// 描画する最初の頂点インデックス
				2);							// プリミティブ(ポリゴン)数
		}

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffOption, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

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
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame)
{
	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	int nIdx = 0;
	Menu *pMenu = NULL;

	for (nIdx = 0; nIdx < MAX_MENU; nIdx++)
	{
		pMenu = &s_aMenu[nIdx];

		if (pMenu->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);
		float fPosY = menu.fTop + ((menu.fBottom - menu.fTop) * 0.5f);

		pMenu->pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
		pMenu->nNumUse = menu.nNumUse;

		pMenu->fWidth = menu.fRight - menu.fLeft;
		pMenu->fHeight = menu.fBottom - menu.fTop;

		if (menu.bSort)
		{// 縦
			pMenu->fInterval = pMenu->fHeight / (menu.nNumUse + 1);
		}
		else
		{// 横
			pMenu->fInterval = pMenu->fWidth / (menu.nNumUse + 1);
		}

		pMenu->fBlinkSpeed = NORMAL_BLINK_SPEED;

		pMenu->bFrame = Frame.bUse;

		pMenu->bUse = true;

		s_nIdxMenu = nIdx;
		s_nIdxOption = 0;

		if (Frame.pTexture != NULL)
		{// テクスチャがある
			pMenu->pTexture = *Frame.pTexture;
		}
		else
		{// テクスチャがない
			pMenu->pTexture = NULL;
		}

		if (Frame.bUse)
		{// 枠がいる
		 // 頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nIdx * 4;		// 指定の位置までポインタを進める

			float fWidth = (menu.fRight - menu.fLeft) * 0.5f;
			float fHeight = (menu.fBottom - menu.fTop) * 0.5f;
			D3DXVECTOR3 pos = D3DXVECTOR3(menu.fLeft + fWidth, menu.fTop + fHeight, 0.0f);

			// 頂点座標の設定
			pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = Frame.col;
			pVtx[1].col = Frame.col;
			pVtx[2].col = Frame.col;
			pVtx[3].col = Frame.col;

			// 頂点バッファをアンロックする
			s_pVtxBuffMenu->Unlock();
		}

		break;
	}

	for (int j = 0; j < pMenu->nNumUse; j++)
	{
		Option *pOption = &pMenu->Option[j];

		if (menu.bSort)
		{// 縦
			pOption->pos = D3DXVECTOR3(pMenu->pos.x, menu.fTop + (pMenu->fInterval * (j + 1)), 0.0f);
		}
		else
		{// 横
			pOption->pos = D3DXVECTOR3(menu.fLeft + (pMenu->fInterval * (j + 1)), pMenu->pos.y, 0.0f);
		}

		pOption->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pOption->fWidth = menu.fWidth;
		pOption->fHeight = menu.fHeight;
		pOption->pTexture = *menu.pTexture[j];

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdx * MAX_MENU) + (j * 4);		// 指定の位置までポインタを進める

		float fWidth = menu.fWidth * 0.5f;
		float fHeight = menu.fHeight * 0.5f;

		// 頂点座標の設定
		pVtx[0].pos = pOption->pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
		pVtx[1].pos = pOption->pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
		pVtx[2].pos = pOption->pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
		pVtx[3].pos = pOption->pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = pOption->col;
		pVtx[1].col = pOption->col;
		pVtx[2].col = pOption->col;
		pVtx[3].col = pOption->col;

		// 頂点バッファをアンロックする
		s_pVtxBuffOption->Unlock();
	}

	return nIdx;
}

//--------------------------------------------------
// 選択肢の色の初期化
//--------------------------------------------------
void InitColorOption(void)
{
	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	// 指定の位置までポインタを進める
	pVtx += (s_nIdxMenu * MAX_MENU) + (s_nIdxOption * 4);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuffOption->Unlock();
}

//--------------------------------------------------
// 選択肢の変更
//--------------------------------------------------
void ChangeOption(int Idx)
{
	s_nIdxOption = Idx;
}

//--------------------------------------------------
// 選択肢の決定
//--------------------------------------------------
void DecisionOption(void)
{
	s_aMenu[s_nIdxMenu].fBlinkSpeed = DECISION_BLINK_SPEED;
}

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetMenu(int nIdx)
{
	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;
	s_aMenu[nIdx].bUse = false;
}

//--------------------------------------------------
// 色の変更
//--------------------------------------------------
static void ChangeColor(Menu *pMenu)
{
	s_nAlphaTime++;

	Option *pOption = &pMenu->Option[s_nIdxOption];

	float fCurve = cosf((s_nAlphaTime * pMenu->fBlinkSpeed) * (D3DX_PI * 2.0f));
	float fAlpha = (fCurve * (1.0f - MIN_ALPHA)) + MIN_ALPHA;

	pOption->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

								// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_nIdxMenu * MAX_MENU) + (s_nIdxOption * 4);		// 指定の位置までポインタを進める

	// 頂点カラーの設定
	pVtx[0].col = pOption->col;
	pVtx[1].col = pOption->col;
	pVtx[2].col = pOption->col;
	pVtx[3].col = pOption->col;

	// 頂点バッファをアンロックする
	s_pVtxBuffOption->Unlock();
}
