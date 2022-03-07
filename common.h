//**************************************************************************************************
//
// �v�Z����
// Author�FYudaKaito
//
//**************************************************************************************************
#ifndef _COMMON_H_		// ���̃}�N����`������ĂȂ�������
#define _COMMON_H_		// ��d�C���N�\���[�h�h�~�̃}�N����`

#include"main.h"

#define		VTX		(4)		// ���_��

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
// �񋓌^
//--------------------------------------------------
typedef enum
{
	SETPOS_MIDDLE = 0,		// �^��
	SETPOS_LEFT,			// ��
	SETPOS_RIGHT,			// �E
	SETPOS_TOP,				// ��
	SETPOS_BOTTOM,			// ��
	SETPOS_MAX
}SETPOS;
// �v���g�^�C�v�錾
void SetVtxAnimTex(VERTEX_2D* pVtx, ANIM_TEX* pAnimTex);
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col);
void SetTex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight);


//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------

/*�� ���_��� [2D] ��*/

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

/*�� �O�ρE���� ��*/

float Vec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);
float Vec2Dot(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);

#endif