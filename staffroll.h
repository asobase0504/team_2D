//================================
//
// �^�C�g����ʂ̃w�b�_�[
// Author:Tomoki Tomidokoro
//
//================================
#ifndef _STAFFROLL_H_		//���̃}�N������`����ĂȂ�������
#define _STAFFROLL_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	D3DXCOLOR col;		//�F
	int		nCntStaff;	//�傫�������߂邽�߂̃J�E���g
	int		nType;		//�g�p���Ă���摜�̃^�C�v
	bool	bUse;		//�g�p���Ă��邩���Ă��Ȃ���
}STAFFROLL;

//�v���g�^�C�v�錾
void InitStaffroll(void);
void UninitStaffroll(void);
void UpdateStaffroll(void);
void DrawStaffroll(void);
void SetStaffroll(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);

#endif