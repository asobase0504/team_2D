//**************************************************************************************************
//
// �v�Z����
// Author�FYudaKaito
//
//**************************************************************************************************
#include "common.h"

//--------------------------------------------------
// �e�N�X�`���A�j���[�V����
// Author�FHamada Ryuga
//--------------------------------------------------
void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex)
{
	pAnimTex->nCnt++;
	pAnimTex->nSpeed = 12;
	if ((pAnimTex->nCnt % pAnimTex->nSpeed) == 0)//�����ő��x����
	{

		pAnimTex->nPatternX++;

		if (pAnimTex->nPatternX >= pAnimTex->nDivisionX)
		{
			pAnimTex->nPatternX = 0;
			pAnimTex->nPatternY++;
		}
		if (pAnimTex->nPatternY >= pAnimTex->nDivisionY)
		{
			pAnimTex->nPatternY = 0;
		}

		SetTex2d(pVtx,
			1.0f / pAnimTex->nDivisionX * pAnimTex->nPatternX
			, 1.0f / pAnimTex->nDivisionX * pAnimTex->nPatternX + (1.0f / pAnimTex->nDivisionX)
			, 1.0f / pAnimTex->nDivisionY * pAnimTex->nPatternY
			, 1.0f / pAnimTex->nDivisionY * pAnimTex->nPatternY + 1.0f / pAnimTex->nDivisionY);
	}
}

//--------------------------------------------------
// ���_�J���[�̐ݒ�
// Author�FYuda Kaito
//--------------------------------------------------
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col)
{
	pVtx[0].col = *col;
	pVtx[1].col = *col;
	pVtx[2].col = *col;
	pVtx[3].col = *col;
}

//--------------------------------------------------
// ���_���W�̐ݒ�
// Author�FYuda Kaito
//--------------------------------------------------
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight)
{
	pVtx[0].pos = D3DXVECTOR3(pos->x - fWidth, pos->y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos->x + fWidth, pos->y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos->x - fWidth, pos->y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos->x + fWidth, pos->y + fHeight, 0.0f);
}

//---------------------------------------
//�Z�b�g�e�N�X�`��(2d)
//Auther�Fhamada ryuuga
//---------------------------------------
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);

}
//=========================================
// ���W�ݒ�
//Auther�Fhamada ryuuga
//=========================================
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}
