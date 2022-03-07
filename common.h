//**************************************************************************************************
//
// 計算処理
// Author：YudaKaito
//
//**************************************************************************************************
#ifndef _COMMON_H_		// このマクロ定義がされてなかったら
#define _COMMON_H_		// 二重インク―ルード防止のマクロ定義

#include"main.h"

#define		VTX		(4)		// 頂点数

typedef struct
{
	int nCnt;
	int nPatternX;
	int nPatternY;
	int nDivisionX;
	int nDivisionY;
	int nSpeed;
}ANIM_TEX;

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	SETPOS_MIDDLE = 0,		// 真ん中
	SETPOS_LEFT,			// 左
	SETPOS_RIGHT,			// 右
	SETPOS_TOP,				// 上
	SETPOS_BOTTOM,			// 下
	SETPOS_MAX
}SETPOS;
// プロトタイプ宣言
void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex);
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col);
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight);


//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------

/*↓ 頂点情報 [2D] ↓*/

void Setpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, SETPOS setpos);
void Setcol(VERTEX_2D *pVtx, D3DXCOLOR col);
void Settex(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom);

void Initpos(VERTEX_2D *pVtx);
void Initrhw(VERTEX_2D *pVtx);
void Initcol(VERTEX_2D *pVtx);
void Inittex(VERTEX_2D *pVtx);
void InitAll(VERTEX_2D *pVtx);

void NormalizeRot(float *pRot);
void Specified(int *pNumber, int nMax, int nMin);
void Specified(float *pNumber, float fMax, float fMin);
void VtxSmallBig(float *pfMin, float *pfMax, float fPos);

/*↓ 外積・内積 ↓*/

float Vec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);
float Vec2Dot(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);

#endif