//**************************************************************************************************
//
// �v�Z����
// Author�FYudaKaito
//
//**************************************************************************************************
#ifndef _COMMON_H_		// ���̃}�N����`������ĂȂ�������
#define _COMMON_H_		// ��d�C���N�\���[�h�h�~�̃}�N����`

#include"main.h"

// �v���g�^�C�v�錾
void SetVtxAnimTex(VERTEX_2D* pVtx, int* nCnt, int nMaxCnt, int* nPatternm, int nMaxPatternm);
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col);
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight);

#endif