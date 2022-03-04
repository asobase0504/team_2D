//========================================================
//
//SP�t���b�O
//Author:����]��
//
//========================================================
#ifndef _FLAG_H_
#define _FLAG_H_

#include "main.h"
//====================================
//SP�t���b�O�̏��
//====================================
typedef enum
{
	FLAG_STATE_ON = 0,	//��������
	FLAG_STATE_OFF,		//�B�ꂽ���
	FLAG_STATE_MAX
}FLAG_STATE;
//====================================
//SP�t���b�O�\���̂̒�`
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posOld;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	FLAG_STATE state;	//���
	float fWidth;		//��
	float fHeight;		//����
	float fAngle;		//�p�x
	float fLength;		//����
	int nType;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Flag;
//====================================
//�v���g�^�C�v�錾
//====================================
void InitFlag(void);
void UninitFlag(void);
void UpdateFlag(void);
void DrawFlag(void);
void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
Flag * GetFlag(void);
void CollisionFlagBullet(D3DXVECTOR3 pPos, float fWidth, float fHeight);
void CollisionFlagPlayer(D3DXVECTOR3 pPos, float fWidth, float fHeight);
#endif