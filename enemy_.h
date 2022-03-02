#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

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
	ENEMYTYPE_SHEONITE,		// ��̓G
	ENEMYTYPE_BAKYURA,		// ���G��
	ENEMYTYPE_GROUND_1,		// �n��̓G1
	ENEMYTYPE_GROUND_2,		// �n��̓G2
	BOSS_PARTS_1,
	BOSS_PARTS_2,
	BOSS_PARTS_3,
	BOSS_PARTS_4,
	BOSS_PARTS_5,
	ENEMYTYPE_MAX
} ENEMYTYPE;

// �G�̍\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	ENEMYSTATE state;		// ���
	ENEMYTYPE nType;		// ���
	float Size;				// �傫��		
	bool bUse;				// �g�p���Ă邩�ǂ���	
	int nLife;				// �̗�
	int nCntState;			// ��ԊǗ��J�E���g
	int nCntBullet;			// �e�̏o�鎞��
	int nCntAnim;			// ���[�V�����̃J�E���g
	int nPatternAnim;		// ���[�V�����̃J�E���g
	// �����Ǘ�
	bool bBack;				// �߂�V�X�e��
	bool bTracking;			// �ǔ��g�p
	// �V�I�i�C�g�p
	float fSheoRot;			// �V�I�i�C�g��p�p�x
	int nSheoCnt;
}Enemy;

// �v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, float Size, ENEMYTYPE nType);
void HitEnemy(Enemy* pEnemy, int nDamage);
Enemy* GetEnemy(void);

#endif
