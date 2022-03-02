//**************************************************************************************************
//
// �v�Z����
// Author�FYudaKaito
//
//**************************************************************************************************
#ifndef _COMMON_H_		// ���̃}�N����`������ĂȂ�������
#define _COMMON_H_		// ��d�C���N�\���[�h�h�~�̃}�N����`

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

// �v���g�^�C�v�錾
void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex);
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col);
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);
void SetNorotpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight);

#endif