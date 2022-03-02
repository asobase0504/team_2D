//========================================================
//
// 背景の処理
// Author : トミドコロ　トモキ
//
//========================================================
#include "main.h"
#include "bg.h"

//マクロ定義
#define NUM_BG (1)			//背景の数
#define BG_WIDTH (1.0f)		//背景の横幅
#define BG_HEIGHT (0.01f)	//背景の縦幅
#define MAX_HEIGHT (1.0f)	//背景の最大値

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = { NULL };			//テクスチャ(３枚文）へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;				//頂点バッファへのポインタ
D3DXVECTOR2 g_posTexV;										//テクスチャ座標の開始位置(V値)
D3DXVECTOR2 g_BGmovePlayer;									//移動量

static float HeightSize;
static float WidthSize;
//========================================================
//背景の初期化処理
//========================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntBG;

	WidthSize = 0.0f;
	HeightSize = 0.0f;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bg000.png",
		&g_pTextureBG[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	g_BGmovePlayer = D3DXVECTOR2(0.0f, 0.01f);			//移動量を初期化

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//頂点座標の指定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(BG_WIDTH, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BG_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2(BG_WIDTH, BG_HEIGHT);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}
//========================================================
//背景の終了処理
//========================================================
void UninitBG(void)
{
	//テクスチャの破棄
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_pTextureBG[nCntBG] != NULL)
		{
			g_pTextureBG[nCntBG]->Release();
			g_pTextureBG[nCntBG] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}
//========================================================
//背景の更新処理
//========================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	HeightSize++;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	g_posTexV.x = 0.0f;
	g_posTexV.y = HeightSize * 0.0001f;

	pVtx[0].tex = D3DXVECTOR2(g_posTexV.x, MAX_HEIGHT - (g_posTexV.y + BG_HEIGHT));
	pVtx[1].tex = D3DXVECTOR2(BG_WIDTH + g_posTexV.x, MAX_HEIGHT - (g_posTexV.y + BG_HEIGHT));
	pVtx[2].tex = D3DXVECTOR2(g_posTexV.x, MAX_HEIGHT - g_posTexV.y);
	pVtx[3].tex = D3DXVECTOR2(BG_WIDTH + g_posTexV.x, MAX_HEIGHT - g_posTexV.y);

	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}
//====================================================
//背景の描画処理
//====================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntBG;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの描画
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		//ポリゴンの描画 四角
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntBG * 4,			//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}