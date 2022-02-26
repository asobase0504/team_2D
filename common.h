//**************************************************************************************************
//
// 計算処理
// Author：YudaKaito
//
//**************************************************************************************************
#ifndef _COMMON_H_		// このマクロ定義がされてなかったら
#define _COMMON_H_		// 二重インク―ルード防止のマクロ定義

#include"main.h"

// プロトタイプ宣言
void SetVtxAnimTex(VERTEX_2D* pVtx, int* nCnt, int nMaxCnt, int* nPatternm, int nMaxPatternm);
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col);
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight);

#endif