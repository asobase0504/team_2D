//================================
//
// �^�C�g����ʂ̃w�b�_�[
// Author:Teruto Sato
//
//================================
#ifndef _TITLE_H_		//���̃}�N������`����ĂȂ�������
#define _TITLE_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h" 

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;		//��
	float fHeight;		//����
	D3DXCOLOR col;		//�F
}TITLE;

//�v���g�^�C�v�錾
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif