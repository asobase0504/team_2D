//============================
//
// マップチップ設定ヘッター
// Author:hamada ryuuga
//
//============================

#ifndef _MAP_H_
#define _MAP_H_

#include"main.h"
#define MAP_FILE0	"Data/txt/map.txt"
#define MAP_FILE1	"Data/txt/map1.txt"
#define MAP_FILE2	"Data/txt/map2.txt"
#define MAP_FILE3	"Data/txt/map3.txt"
#define MAP_FILE4	"Data/txt/map4.txt"
#define MAP_FILE5	"Data/txt/map5.txt"
#define MAP_FILE6	"Data/txt/map6.txt"
// マクロ定義
#define NUM_MAP	    (2048)		//背景枚数
#define X_MAP		(22)		//マップチップサイズX
#define Y_MAP		(2)			//マップチップサイズY
#define MAP_SIZEX	(20)		//マップサイズX
#define MAP_SIZEY	(100)		//マップサイズY
#define MAPYLENGTH	(10)		//マップの長さ
#define BLOCKSIZEX	(((float)SCREEN_WIDTH /MAP_SIZEX))
#define BLOCKSIZEY	(((float)SCREEN_HEIGHT*MAPYLENGTH / MAP_SIZEY))
#define MAPMOVE (2.5f)

//-------------
//構造体
//-------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//カラー
	int nType;			//テクスチャ
	int tex;			//テクスチャタイプ
	bool bUse;			//
	char filename[256];
}Map;

//-----------------
//プロトタイプ宣言
//-----------------
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos, int nType, int tex);	//マップデータの設定(引数座標タイプテクスチャの何番目を使うか)
void InitMapSet(char *Filename);
void ConversionMap(D3DXVECTOR3 pos, int tex);		//マップデータのテクスチャの変更(引数座標,タイプ,テクスチャの何番目にするか)
void OutputMap(char *Filename);						//マップデータのテクスチャの変更(引数座標、テクスチャの何番目にするか)
void SizeMap(float SIZ);							//マップデータのテクスチャの変更(引数　変更後のサイズ)
Map *GetMap(void);
int GetStage(void);
bool CollisionMap(D3DXVECTOR3 pos);					//当たり判定
void ConteSet(int stage);							//コンテ
void PasSetMap(char *Filename);						//マップパスをセットする
int sopiteMap(D3DXVECTOR3 pos);						//マップデータのテクスチャの変更(引数　座標)
void falseSetMap(void);								//すべてのマップチップを削除する
#endif // !_Map_H_
