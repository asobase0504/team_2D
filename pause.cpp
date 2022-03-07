//==================================================
//
// ゲーム制作 ( pause.cpp )
// Author  : katsuki mizuki
//
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "input.h"
#include "fade.h"
#include "menu.h"
#include "game.h"

#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MENU_WIDTH		(500.0f)								// メニューの幅
#define MENU_HEIGHT		(150.0f)								// メニューの高さ
#define MENU_LEFT		(SCREEN_WIDTH * 0.25f)					// メニューの左端
#define MENU_RIGHT		(SCREEN_WIDTH * 0.75f)					// メニューの右端
#define MENU_TOP		(SCREEN_HEIGHT * 0.1f)					// メニューの上端
#define MENU_BOTTOM		(SCREEN_HEIGHT * 0.9f)					// メニューの下端
#define BG_COLOR		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f))		// 背景の色
#define FRAME_COLOR		(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))		// フレームの色

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	MENU_GAME = 0,		// ゲーム
	MENU_RETRY,			// リトライ
	MENU_TITLE,			// タイトル
	MENU_MAX
}MENU;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			// 背景の頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// 頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		// メニューのテクスチャへのポインタ
static int							s_nSelectMenu;					// 選ばれているメニュー
static int							s_nIdxMenu;						// 使っているメニューの番号

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Input(void);

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitPause(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nSelectMenu = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Back_To_Game.png",
		&s_pTextureMenu[MENU_GAME]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Restart.png",
		&s_pTextureMenu[MENU_RETRY]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Back_To_Title.png",
		&s_pTextureMenu[MENU_TITLE]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT* 0.5f;

	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = BG_COLOR;
	pVtx[1].col = BG_COLOR;
	pVtx[2].col = BG_COLOR;
	pVtx[3].col = BG_COLOR;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuffBG->Unlock();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	fWidth = (MENU_RIGHT - MENU_LEFT) * 0.5f;
	fHeight = (MENU_BOTTOM - MENU_TOP) * 0.5f;

	pos = D3DXVECTOR3(MENU_LEFT + fWidth, MENU_TOP + fHeight, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = FRAME_COLOR;
	pVtx[1].col = FRAME_COLOR;
	pVtx[2].col = FRAME_COLOR;
	pVtx[3].col = FRAME_COLOR;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPause(void)
{
	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{// テクスチャの解放
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePause(void)
{
	// 入力
	Input();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawPause(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// 描画するプリミティブ数

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// 描画するプリミティブ数
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetPause(void)
{
	ResetMenu(s_nIdxMenu);

	MenuArgument menu;
	menu.nNumUse = MENU_MAX;
	menu.fLeft = MENU_LEFT;
	menu.fRight = MENU_RIGHT;
	menu.fTop = MENU_TOP;
	menu.fBottom = MENU_BOTTOM;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;

	for (int i = 0; i < MENU_MAX; i++)
	{
		menu.pTexture[i] = &s_pTextureMenu[i];
	}

	// メニューの設定
	s_nIdxMenu = SetMenu(menu);
}

//--------------------------------------------------
// 入力
//--------------------------------------------------
static void Input(void)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// Wキーが押されたかどうか
		s_nSelectMenu = ((s_nSelectMenu - 1) + MENU_MAX) % MENU_MAX;

	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// Sキーが押されたかどうか
		s_nSelectMenu = ((s_nSelectMenu + 1) + MENU_MAX) % MENU_MAX;
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
	{//決定キー(ENTERキー)が押されたかどうか
		switch (s_nSelectMenu)
		{
		case MENU_GAME:			// ゲーム
			SetEnablePause(false);
			ResetMenu(s_nIdxMenu);
			break;

		case MENU_RETRY:		// リトライ
			ChangeMode(MODE_GAME);
			break;

		case MENU_TITLE:		// タイトル
			ChangeMode(MODE_TITLE);
			break;

		default:
			assert(false);
			break;
		}
	}
}