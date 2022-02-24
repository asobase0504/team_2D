//=========================================
// 背景設定
// author hamada ryuuga
// Author 冨所知生
//=========================================
#include"map.h"
#include <stdio.h>

#define NUM_MAP	    (12800)						// 背景枚数
#define X_MAP		(4)							// マップチップサイズX
#define Y_MAP		(2)							// マップチップサイズY
#define MAPSIZX		(10)						// マップサイズX
#define MAPSIZY		(50)						// マップサイズY
#define MAPYLENGTH  (10)						// マップの長さ
#define BLOCKSIZX   ((SCREEN_WIDTH / MAPSIZX))
#define BLOCKSIZY	((SCREEN_HEIGHT * MAPYLENGTH / MAPSIZY))

// スタティック変数
static LPDIRECT3DTEXTURE9 s_pTextureMap = {};			// テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffMap = NULL;	// 頂点バッファの設定
static Map s_aMap[NUM_MAP];
static float s_fMapSizX, s_fMapSizY;

//=========================================
// マップチップの初期化処理
//=========================================
void InitMap(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/mapchip_field2.jpg",
		&s_pTextureMap);

	// 頂点バッファ
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMap,
		NULL);

	VERTEX_2D*pVtx; // 頂点へのポインタ
	
	// 頂点バッファをアンロック
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_MAP; i++)
	{
		s_aMap[i].bUse = false;
		s_aMap[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aMap[i].move = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		
		// 座標の設定
		SetNorotpos2d(pVtx,
			0.0f,
			BLOCKSIZX,
			0.0f,
			BLOCKSIZY);

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

		// テクスチャの座標設定
		Settex2d(pVtx, (1.0f / X_MAP) * 0, (1.0f / X_MAP) * 0 + 1, (1.0f / Y_MAP) * 0, (1.0f / Y_MAP) * 0 + 1);

		pVtx += 4;
	}
	// 頂点バッファをアンロック
	s_pVtxBuffMap->Unlock();
}

//=========================================
// マップの破棄する処理
//=========================================
void UninitMap(void)
{
	//テクスチャの破棄
	if (s_pTextureMap != NULL)
	{
		s_pTextureMap->Release();
		s_pTextureMap = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuffMap != NULL)
	{
		s_pVtxBuffMap->Release();
		s_pVtxBuffMap = NULL;
	}
}
//=========================================
// マップの更新処理
//=========================================
void UpdateMap(void)
{
	for (int i = 0; i < NUM_MAP; i++)
	{
		Map* pMap = &s_aMap[i];

		if (!pMap->bUse)
		{// 対応した箇所が使用されている時
			continue;
		}

		pMap->pos += pMap->move;

		VERTEX_2D*pVtx; // 頂点へのポインタ

		// 頂点バッファをアンロック
		s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		// 座標を設定する
		SetNorotpos2d(pVtx,
			pMap->pos.x,
			pMap->pos.x + BLOCKSIZX,
			pMap->pos.y,
			pMap->pos.y + BLOCKSIZY);

		// 頂点バッファをアンロック
		s_pVtxBuffMap->Unlock();
	}
}

//=========================================
// マップの描画処理
//=========================================
void DrawMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスのポインタ

	for (int i = 0; i < NUM_MAP; i++)
	{
		Map* pMap = &s_aMap[i];

		if (!pMap->bUse)
		{
			continue;
		}

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffMap, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTextureMap);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			4 * i,
			2);
	}
}

//=========================================
// マップデータの設定
//=========================================
void SetMap(D3DXVECTOR3 pos, int nType, int tex)
{
	for (int nCntMap = 0; nCntMap < NUM_MAP; nCntMap++)
	{
		Map* pMap = &s_aMap[nCntMap];

		if (pMap->bUse)
		{// 敵が使用されてない場合
			continue;
		}

		pMap->pos = pos;

		pMap->nType = nType;
		pMap->bUse = true;

		VERTEX_2D*pVtx; // 頂点へのポインタ

		s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nCntMap * 4;

		// 座標を設定する
		SetNorotpos2d(pVtx,
			pMap->pos.x,
			pMap->pos.x + BLOCKSIZX,
			pMap->pos.y,
			pMap->pos.y + BLOCKSIZY);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float X = (float)(tex % 100 / 10);
		float Y = (float)(tex % 10 / 1);

		// テクスチャの座標設定
		Settex2d(pVtx, (1.0f / X_MAP) * X, (1.0f / X_MAP) * X + 1.0f / X_MAP, (1.0f / Y_MAP) * Y, ((1.0f / Y_MAP) * Y) + (1.0f / Y_MAP));

		//頂点バッファをアンロック
		s_pVtxBuffMap->Unlock();
		break;
	}
}

//=========================================
// マップチップの読み込み
//=========================================
void InitMapSet(char *Filename)
{
	int s_aMap[MAPSIZY][MAPSIZX];
	s_fMapSizX = (SCREEN_WIDTH) / MAPSIZX;
	s_fMapSizY = (SCREEN_HEIGHT * 3.0f) / MAPSIZY;
	FILE *pFile = NULL;	// ファイルポインタを宣言

	int nCntX, nCntY;

	// ファイルを開く
	pFile = fopen(&Filename[0], "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		for (nCntY = 0; nCntY < MAPSIZY; nCntY++)
		{
			for (nCntX = 0; nCntX < MAPSIZX; nCntX++)
			{// Mapの書き込み
				fscanf(pFile, "%d", &s_aMap[nCntY][nCntX]);
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
	}

	// ブロック設定
	for (nCntY = 0; nCntY < MAPSIZY; nCntY++)
	{
		for (nCntX = 0; nCntX < MAPSIZX; nCntX++)
		{// Mapの書き込み
			SetMap(D3DXVECTOR3(s_fMapSizX * nCntX, -(s_fMapSizY * nCntY) + SCREEN_HEIGHT, 0.0f), 1, s_aMap[nCntY][nCntX]);
		}
	}
	for (nCntY = 0; nCntY < MAPSIZY; nCntY++)
	{
		for (nCntX = 0; nCntX < MAPSIZX; nCntX++)
		{// Mapの書き込み
			s_aMap[nCntY][nCntX] = 0;
		}
	}
}

//=========================================
// 座標設定
//=========================================
void SetNorotpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}
//=========================================
// テクスチャの座標設定
//=========================================
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	// テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);
}