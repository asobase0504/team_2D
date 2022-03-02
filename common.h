//**************************************************************************************************
//
// 計算処理
// Author：YudaKaito
//
//**************************************************************************************************
#ifndef _COMMON_H_		// このマクロ定義がされてなかったら
#define _COMMON_H_		// 二重インク―ルード防止のマクロ定義

#include"main.h"

typedef struct
{
	int nCnt;
	int nPatternX;
	int nPatternY;
	int nDivisionX;
	int nDivisionY;
	int nSpeed;
}ANIM_TEX;

// プロトタイプ宣言
void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex);
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col);
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);
void SetNorotpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight);

#endif