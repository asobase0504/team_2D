//=========================================
//
//	マップチップのヘッダーファイル
//	Author : 浜田琉雅
//	Author : 冨所知生
//
//=========================================
#ifndef _MAP_H_
#define _MAP_H_

#include"main.h"

//=========================================
//構造体の宣言
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//カラー
	D3DXVECTOR3 siz;	//サイズ
	int nType;			//テクスチャ
	bool bUse;
}Map;

//=========================================
//プロトタイプ宣言
//=========================================
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos, int nType, int tex);
void InitMapSet(char *Filename);
Map *GetMap(void);
void SetNorotpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);

#endif // !_Map_H_
#pragma once
