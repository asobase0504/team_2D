#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"
#include "common.h"

// �}�N����`
#define MAX_ENEMY			(255)

typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// �ʏ���
	ENEMYSTATE_DAMAGE,		// �_���[�W����
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{
	ENEMYTYPE_SKY_1,		// ��̓G1
	ENEMYTYPE_SKY_2,		// ��̓G2
	ENEMYTYPE_SKY_3,		// ����������G
	ENEMYTYPE_SHEONITE,		// �����Ȃ���		
	ENEMYTYPE_BAKYURA,		// ���G��
	ENEMYTYPE_GROUND_1,		// �n��̓G1
	ENEMYTYPE_GROUND_2,		// �n��̓G2
	ENEMYTYPE_WARP_1,		// ���[�v����G1	(�����s�R)
	ENEMYTYPE_WARP_2,		// ���[�v����G2	(�����s�R)
	ENEMYTYPE_WARP_3,		// ���[�v����G3	(�����s�R)
	ENEMYTYPE_WARP_4,		// ���[�v����G4	(�����s�R)
	ENEMYTYPE_BOSS,			// �{�X
	BOSS_PARTS_1,			// BOSS_PARTS1
	BOSS_PARTS_2,			// BOSS_PARTS2
	BOSS_PARTS_3,			// BOSS_PARTS3
	BOSS_PARTS_4,			// BOSS_PARTS4
	BOSS_PARTS_5,			// BOSS_PARTS5
	FLAG_STATE,				// ��
	ENEMYTYPE_MAX
} ENEMYTYPE;

// �G�̍\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	ENEMYSTATE state;		// ���
	ENEMYTYPE nType;		// ���
	ANIM_TEX AnimTex;		// �A�j���[�V�����J�E���g
	float fSpeed;			// �ړ���
	float fSize;			// �傫��		
	bool bUse;				// �g�p���Ă邩�ǂ���	
	int nLife;				// �̗�
	int nCntState;			// ��ԊǗ��J�E���g
	int nCntBullet;			// �e�̏o�鎞��
	// �����Ǘ�
	bool bBack;				// �߂�V�X�e��
	bool bTracking;			// �ǔ��g�p
	bool bTP;
	// �V�I�i�C�g�p
	float fSheoRot;			// �V�I�i�C�g��p�p�x
	int nSheoCnt;			// �V�I�i�C�g�J�E���g
	bool bReflect;			// ����
	bool FlagOn;			//�t���O�p
}Enemy;

// �v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, float Size, ENEMYTYPE nType);
void HitEnemy(Enemy* pEnemy, int nDamage);
void LoadSetFile(char *Filename);
void EnemyLynk(char *Filename);		// 
void SetEnemyLynk(int number);		// 
Enemy* GetEnemy(void);
void falseSetEnemy(void);

#endif

