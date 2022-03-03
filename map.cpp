//============================
//
// マップチップ設定
// Author:hamada ryuuga
//
//============================
#include "map.h"
#include "input.h"
#include "enemy.h"
#include <stdio.h>
#include "common.h"

// マクロ定義
#define NUM_MAP	    (1024)		//背景枚数
#define X_MAP		(4)			//マップチップサイズX
#define Y_MAP		(2)			//マップチップサイズY
#define MAP_SIZEX	(10)		//マップサイズX
#define MAP_SIZEY	(100)		//マップサイズY
#define MAPYLENGTH	(10)		//マップの長さ
#define BLOCKSIZEX	(((float)SCREEN_WIDTH /MAP_SIZEX))
#define BLOCKSIZEY	(((float)SCREEN_HEIGHT*MAPYLENGTH / MAP_SIZEY))

//スタティック変数
static LPDIRECT3DTEXTURE9 s_pTextureMap = NULL; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffMap = NULL; //頂点バッファの設定
static Map s_aMap[NUM_MAP];
static char s_aMapFile[10][255];
static D3DXVECTOR3 s_Move(0.0f, 5.0f, 0.0f);
static D3DXVECTOR3 s_PosOffset;
static float s_fMapScale;
static int stage = 0;

//================
//初期化処理
//================
void InitMap(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9  pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mapchip_field2.jpg",
		&s_pTextureMap);

	//頂点バッファ
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, //ここ頂点フォーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuffMap,
		NULL);

	VERTEX_2D*pVtx; //頂点へのポインタ

	//頂点バッファをアンロック
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);
	for (int i = 0; i < NUM_MAP; i++, pVtx += 4)
	{
		s_aMap[i].bUse = false;

		s_aMap[i].tex = 0;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//頂点バッファをアンロック
	s_pVtxBuffMap->Unlock();

	PasSetMap("data\\hoge4.txt");
	s_PosOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_fMapScale = 1.0f;
}

//破棄
void UninitMap(void)
{
	stage = 0;
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

//==================
//更新処理
//==================
void UpdateMap(void)
{
	s_PosOffset += s_Move;

	if (s_PosOffset.y >= (BLOCKSIZEY * MAP_SIZEY) - SCREEN_HEIGHT - BLOCKSIZEY)
	{//無限ループ処理
		s_PosOffset.y = 0.0f;
		stage++;
		stage %= 5;
		// マップの設定。
		InitMapSet(&s_aMapFile[stage][0]);
		SetEnemyLynk(stage);
	}

	//頂点へのポインタ
	VERTEX_2D*pVtx;
	//頂点バッファをロック
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos;

	for (int i = 0; i < NUM_MAP; i++, pVtx += 4)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		pos = (s_aMap[i].pos + s_PosOffset)* s_fMapScale;

		SetNormalpos2d(pVtx, pos.x, pos.x + BLOCKSIZEX * s_fMapScale, pos.y, pos.y + BLOCKSIZEY * s_fMapScale);

		if (pos.y >= SCREEN_HEIGHT)
		{
			s_aMap[i].bUse = false;
		}
	}

	//頂点バッファをアンロック
	s_pVtxBuffMap->Unlock();
}

//==================
//描画処理
//==================
void DrawMap(void)
{
	for (int i = 0; i < NUM_MAP; i++)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		//デバイスのポインタ
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffMap, 0, sizeof(VERTEX_2D));
		
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTextureMap);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);
	}
}

//==================
//マップデータの設定
//==================
void SetMap(D3DXVECTOR3 pos, int nType, int tex)
{
	VERTEX_2D *pVtx; //頂点へのポインタ
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	Map* pMap = s_aMap;

	for (int i = 0; i < NUM_MAP; i++, pVtx += 4, pMap++)
	{
		if (pMap->bUse)
		{
			continue;
		}

		pMap->bUse = true;

		//マップが使用されてない場合
		pMap->pos = pos;
		pMap->nType = nType;
		pMap->tex = tex;

		int X = (pMap->tex % X_MAP);
		int Y = (pMap->tex / X_MAP);

		//テクスチャの座標設定
		SetTex2d(pVtx, (1.0f / X_MAP) * X, (1.0f / X_MAP) * (X + 1.0f), (1.0f / Y_MAP) * Y, (1.0f / Y_MAP) * (Y + 1.0f));

		break;
	}
	//頂点バッファをアンロック
	s_pVtxBuffMap->Unlock();
}

//==================
//ファイルの入力処理
//==================
void InitMapSet(char *Filename)
{
	int aMap[MAP_SIZEY][MAP_SIZEX];

	for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
		{//Mapの書き込み
			aMap[nCntY][nCntX] = 0;
		}
	}

	//ファイルを開く
	FILE *pFile = fopen(&Filename[0], "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
			{//Mapの書き込み
				fscanf(pFile, "%d", &aMap[nCntY][nCntX]);
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	//ブロック設定
	for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
		{//Mapの書き込み
			SetMap(D3DXVECTOR3(BLOCKSIZEX*nCntX, (-(BLOCKSIZEY*nCntY) + SCREEN_HEIGHT), 0.0f), 1, aMap[nCntY][nCntX]);
		}
	}
}

//==================
//ファイルの出力処理
//==================
void OutputMap(char *Filename)
{
	//ファイル開け
	FILE *pFile = fopen(Filename, "w");
	int nCnt = 0;
	//ブロック設定
	for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
		{//Mapの書き込み
			fprintf(pFile, "%d\t", s_aMap[nCnt].tex);
			nCnt++;
		}
		fprintf(pFile, "\n");
	}

	fclose(pFile);
}

//==================
//マップチップ変更処理
//==================
void ConversionMap(D3DXVECTOR3 pos, int tex)
{
	D3DXVECTOR3 mapPos;

	VERTEX_2D*pVtx; //頂点へのポインタ
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_MAP; i++, pVtx += 4)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		mapPos = (s_aMap[i].pos + s_PosOffset)* s_fMapScale;

		if (((mapPos.x < pos.x) && (mapPos.x + BLOCKSIZEX * s_fMapScale > pos.x)) &&
			((mapPos.y < pos.y) && (mapPos.y + BLOCKSIZEY * s_fMapScale > pos.y)))
		{
			s_aMap[i].tex = tex;

			int X = (s_aMap[i].tex % X_MAP);
			int Y = (s_aMap[i].tex / X_MAP);

			//テクスチャの座標設定
			SetTex2d(pVtx, (1.0f / X_MAP) * X, (1.0f / X_MAP) * (X + 1.0f), (1.0f / Y_MAP) * Y, (1.0f / Y_MAP) * (Y + 1.0f));
		}
	}

	//頂点バッファをアンロック
	s_pVtxBuffMap->Unlock();
}
//==================
//マップの当たり判定
//==================
bool CollisionMap(D3DXVECTOR3 pos)
{
	bool Hit = false;//チップとマウスの当たり判定が当たってる時
	D3DXVECTOR3 mapPos;

	for (int i = 0; i < NUM_MAP; i++)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		mapPos = (s_aMap[i].pos + s_PosOffset)* s_fMapScale;

		if (((mapPos.x < pos.x) && (mapPos.x + BLOCKSIZEX * s_fMapScale > pos.x)) &&
			((mapPos.y < pos.y) && (mapPos.y + BLOCKSIZEY * s_fMapScale > pos.y)))
		{

			Hit = true;
		}
	}
	return Hit;
}
//==================
//サイズ変更
//==================
void SizeMap(float SIZ)
{
	s_fMapScale = 1.0f / SIZ;
}

//==================
//マップの取得
//==================
Map *GetMap(void)
{
	return s_aMap;
}

//==================
//コンテ処理
//==================
void ConteSet(void)
{
	s_PosOffset.y = 0.0f;
	// マップの設定。
	InitMapSet(&s_aMapFile[stage][0]);
	SetEnemyLynk(stage);

}

//==================
//fileパス読み込み
//==================
void PasSetMap(char *Filename)
{
	//ファイルを開く
	Enemy *Enemy = GetEnemy();
	FILE *pFile = fopen(&Filename[0], "r");
	char	s_aString[256];//ファイルの文字入れる
	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		int number = 0;
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み
			fscanf(pFile, "%s", &s_aString[0]);

			if (strcmp(&s_aString[0], "MAPLYNK") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%s", &s_aMapFile[number][0]);
				number++;
			}
			if (strcmp(&s_aString[0], "ENEMYLYNK") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%s", &s_aString[0]);
				EnemyLynk(&s_aString[0]);
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}