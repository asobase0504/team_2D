//**************************************************************************************************
//
// �v�Z����
// Author�FYudaKaito
//
//**************************************************************************************************
#include "common.h"

#include <assert.h>
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


//--------------------------------------------------
// ���_�J���[�̐ݒ� [2D]
//--------------------------------------------------
void Setcol(VERTEX_2D *pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
}

//--------------------------------------------------
// �e�N�X�`���̐ݒ� [2D]
//--------------------------------------------------
void Settex(VERTEX_2D *pVtx, float fULeft, float fURight, float fVTop, float fVBottom)
{
	pVtx[0].tex = D3DXVECTOR2(fULeft, fVTop);
	pVtx[1].tex = D3DXVECTOR2(fURight, fVTop);
	pVtx[2].tex = D3DXVECTOR2(fULeft, fVBottom);
	pVtx[3].tex = D3DXVECTOR2(fURight, fVBottom);
}

//--------------------------------------------------
// ���_���W�̏����� [2D]
//--------------------------------------------------
void Initpos(VERTEX_2D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// rhw�̏����� [2D]
//--------------------------------------------------
void Initrhw(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//--------------------------------------------------
// ���_�J���[�̏����� [2D]
//--------------------------------------------------
void Initcol(VERTEX_2D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
// �e�N�X�`���̏����� [2D]
//--------------------------------------------------
void Inittex(VERTEX_2D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
// �S�Ă̏����� [2D]
//--------------------------------------------------
void InitAll(VERTEX_2D *pVtx)
{
	// ���_���W�̏����� [2D]
	Initpos(pVtx);

	// rhw�̏����� [2D]
	Initrhw(pVtx);

	// ���_�J���[�̏����� [2D]
	Initcol(pVtx);

	// �e�N�X�`���̏����� [2D]
	Inittex(pVtx);
}
//--------------------------------------------------
// �p�x�̐��K��
//--------------------------------------------------
void NormalizeRot(float *pRot)
{
	if (*pRot >= D3DX_PI)
	{// 3.14���傫��
		*pRot -= D3DX_PI * 2.0f;
	}
	else if (*pRot <= -D3DX_PI)
	{// -3.14��菬����
		*pRot += D3DX_PI * 2.0f;
	}
}

//--------------------------------------------------
// �w��̒l�ȏ�E�ȉ� [int]
//--------------------------------------------------
void Specified(int *pNumber, int nMax, int nMin)
{
	if (*pNumber >= nMax)
	{// �w��̒l�ȏ�
		*pNumber = nMax;
	}
	else if (*pNumber <= nMin)
	{// �w��̒l�ȉ�
		*pNumber = nMin;
	}
}

//--------------------------------------------------
// �w��̒l�ȏ�E�ȉ� [float]
//--------------------------------------------------
void Specified(float *pNumber, float fMax, float fMin)
{
	if (*pNumber >= fMax)
	{// �w��̒l�ȏ�
		*pNumber = fMax;
	}
	else if (*pNumber <= fMin)
	{// �w��̒l�ȉ�
		*pNumber = fMin;
	}
}

//--------------------------------------------------
// �o�[�e�b�N�X�̏������E�傫��
//--------------------------------------------------
void VtxSmallBig(float *pfMin, float *pfMax, float fPos)
{
	if (fPos < *pfMin)
	{// ������
		*pfMin = fPos;
	}

	if (fPos > *pfMax)
	{// �傫��
		*pfMax = fPos;
	}
}

//--------------------------------------------------
// 2D�x�N�g���̊O��
//--------------------------------------------------
float Vec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return (v1->x * v2->z) - (v1->z * v2->x);
}

//--------------------------------------------------
// 2D�x�N�g���̓���
//--------------------------------------------------
float Vec2Dot(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return (v1->x * v2->x) + (v1->z * v2->z);
}

/*�� 2D ��*/

static void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetLeftpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetToppos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);


void Setpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight, SETPOS setpos)
{
	switch (setpos)
	{
	case SETPOS_MIDDLE:		// �^��

		// ���_���W�̐ݒ� [2D] (�^�񒆂ɒ��S)
		SetMiddlepos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_LEFT:		// ��

		// ���_���W�̐ݒ� [2D] (���ɒ��S)
		SetLeftpos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_RIGHT:		// �E

		// ���_���W�̐ݒ� [2D] (�E�ɒ��S)
		SetRightpos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_TOP:		// ��

		// ���_���W�̐ݒ� [2D] (��ɒ��S)
		SetToppos(pVtx, pos, fWidth, fHeight);

		break;

	case SETPOS_BOTTOM:		// ��

		// ���_���W�̐ݒ� [2D] (���ɒ��S)
		SetBottompos(pVtx, pos, fWidth, fHeight);

		break;

	default:
		assert(false);
		break;
	}
}
//--------------------------------------------------
// ���_���W�̐ݒ� [2D] (�^�񒆂ɒ��S)
//--------------------------------------------------
static void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
// ���_���W�̐ݒ� [2D] (���ɒ��S)
//--------------------------------------------------
static void SetLeftpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(0.0f, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
// ���_���W�̐ݒ� [2D] (�E�ɒ��S)
//--------------------------------------------------
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(0.0f, fHeight, 0.0f);
}

//--------------------------------------------------
// ���_���W�̐ݒ� [2D] (��ɒ��S)
//--------------------------------------------------
static void SetToppos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, 0.0f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);
}

//--------------------------------------------------
// ���_���W�̐ݒ� [2D] (���ɒ��S)
//--------------------------------------------------
static void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth, 0.0f, 0.0f);
}
