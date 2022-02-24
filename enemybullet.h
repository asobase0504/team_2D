#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

#include "main.h"
//====================================
//�e�\���̂̒�`
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	int nLife;			//����
	bool bUse;			//�g�p���Ă��邩�ǂ���
	bool bChase;		//�ǂ����ǂ���
}EnemyBullet;
//====================================
//�v���g�^�C�v�錾
//====================================
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos);

#endif