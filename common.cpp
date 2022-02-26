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
void SetVtxAnimTex(VERTEX_2D* pVtx, int* nCnt, int nMaxCnt, int* nPatternm, int nMaxPatternm)
{
	*nCnt++;	// �A�j���[�V����
	if (*nCnt >= nMaxCnt)
	{
		*nCnt = 0;
		*nPatternm++;

		if (*nPatternm >= nMaxPatternm)
		{
			*nPatternm = 0;
		}

		pVtx[0].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * *nPatternm, 0.0f);			//--------------------------�G���������Ƃ����₷
		pVtx[1].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * (*nPatternm + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * *nPatternm, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * (*nPatternm + 1), 1.0f);
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
