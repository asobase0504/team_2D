//============================
//
// ���U���g��ʂ̃w�b�_�[
// Author:Teruto Sato
//
//============================
#ifndef _RESULT_H_		//���̃}�N������`����ĂȂ�������
#define _RESULT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	D3DXCOLOR col;		//�F
}RESULT;

//�v���g�^�C�v�錾
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif