//==================================================
// 
// 3Dゲーム制作 ( bestscore.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "bestscore.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define BESTSCORE_WIDTH 		(240.0f)		// ベストスコアの幅
#define BESTSCORE_HEIGHT		(30.0f)			// ベストスコアの高さ
#define START_POS_X				(245.0f)		// 始まりのXの位置
#define START_POS_Y				(80.0f)			// 始まりのYの位置
#define MAX_ALPHA_TIME			(120)			// α値変更用の時間の最大値
#define BLINK_SPEED				(0.1f)			// 点滅速度
#define MIN_ALPHA				(0.6f)			// α値の最小値

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static bool							s_bUpdate;				// ベストスコアが更新されたかどうか
static int							s_nAlphaTime;			// α値変更用の時間

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitBestScore(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_bUpdate = false;
	s_nAlphaTime = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/BEST_SCORE.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(           0.0f, -BESTSCORE_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(BESTSCORE_WIDTH, -BESTSCORE_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(           0.0f,  BESTSCORE_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(BESTSCORE_WIDTH,  BESTSCORE_HEIGHT * 0.5f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

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
void UninitBestScore(void)
{
	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateBestScore(void)
{
	if (s_bUpdate)
	{// ベストスコアが更新された
		if (s_nAlphaTime <= MAX_ALPHA_TIME)
		{// 指定の値以下
			s_nAlphaTime++;

			VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

			// 頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			float fAlpha = (sinf((s_nAlphaTime * BLINK_SPEED) * (D3DX_PI * 2.0f)) * (1.0f - MIN_ALPHA)) + MIN_ALPHA;

			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, fAlpha);

			// 頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			// 頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
		}
		else
		{
			VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

			// 頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			// 頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawBestScore(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

//--------------------------------------------------
// 点滅
//--------------------------------------------------
void BlinkBestScore(void)
{
	s_bUpdate = true;
}