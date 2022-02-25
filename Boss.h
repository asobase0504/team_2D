#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "enemy.h"

#define COLLISION_B	(25)	//�G�T�C�Y����
#define JUDGEMENT_B	(25)	//������G�̔���T�C�Y

typedef enum
{
	BossSTATE_NORMAL = 0,	// �ʏ���
	BossSTATE_DAMAGE,		// �_���[�W����
	BossSTATE_MAX
}BOSSSTATE;

// �G�̍\���̒�`
typedef struct
{
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 pos;		// �ʒu
	int nCounerState;		// ��ԊǗ��J�E���^�[
	ENEMYTYPE nType;		// ���
	bool bUse;				// �g�p���Ă邩�ǂ���	
	int	nCounter;			// �J�E���g�_�E��
	int nLife;				// �̗�
	BOSSSTATE state;		// ���
	Enemy* Enemy[5];
}BOSS;

// �v���g�^�C�v�錾
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 pos/*, TYPEENEMY nType*/);

#endif






