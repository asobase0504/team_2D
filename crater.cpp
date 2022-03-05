//==================================================
// 
// 3Dゲーム制作 ( crater.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "crater.h"
#include "map.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_CRATER		(256)		// クレーターの最大数
#define CRATER_SIZE		(30.0f)		// クレーターのサイズ

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	bool			bUse;		// 使用しているかどうか
}Crater;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファへのポインタ
static Crater						s_Crater[MAX_CRATER];		// クレーターの情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitCrater(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/crater.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_CRATER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_Crater, 0, sizeof(s_Crater));

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_CRATER; i++)
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
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitCrater(void)
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
void UpdateCrater(void)
{
	for (int i = 0; i < MAX_CRATER; i++)
	{
		if (!s_Crater[i].bUse)
		{// 使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		s_Crater[i].pos.y += MAP_SPEAD;		// Mapから速度をGetして欲しいです。よろしく！

		if (s_Crater[i].pos.y >= (SCREEN_HEIGHT + CRATER_SIZE))
		{// 画面外に出た
			s_Crater[i].bUse = false;
		}

		VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// 指定の位置までポインタを進める

		// 頂点座標の設定
		pVtx[0].pos = s_Crater[i].pos + D3DXVECTOR3(-CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[1].pos = s_Crater[i].pos + D3DXVECTOR3( CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[2].pos = s_Crater[i].pos + D3DXVECTOR3(-CRATER_SIZE,  CRATER_SIZE, 0.0f);
		pVtx[3].pos = s_Crater[i].pos + D3DXVECTOR3( CRATER_SIZE,  CRATER_SIZE, 0.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawCrater(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_CRATER; i++)
	{
		if (!s_Crater[i].bUse)
		{// 使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetCrater(D3DXVECTOR3 pos)
{
	for (int i = 0; i < MAX_CRATER; i++)
	{
		if (s_Crater[i].bUse)
		{// 使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		s_Crater[i].pos = pos;
		s_Crater[i].bUse = true;

		VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// 指定の位置までポインタを進める

		// 頂点座標の設定
		pVtx[0].pos = pos + D3DXVECTOR3(-CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3( CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-CRATER_SIZE,  CRATER_SIZE, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3( CRATER_SIZE,  CRATER_SIZE, 0.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}