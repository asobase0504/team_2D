#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

// �}�N����`
#define MAX_Boss			(256)	// �G�̍ő吔
#define ENEMY_WIDTH			(25.0f)	// X	�G�T�C�Y
#define ENEMY_HEIGHT		(25.0f)	// Y	�G�T�C�Y
#define MAX_ENEMY			(255)
#define NUM_ENEMY			(2)		// �G�̎��

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
	ENEMYTYPE_WARP_1,		// ���[�v����G1
	ENEMYTYPE_WARP_2,		// ���[�v����G2
	ENEMYTYPE_WARP_3,		// ���[�v����G3
	ENEMYTYPE_WARP_4,		// ���[�v����G4
	BOSS_PARTS_1,			// BOSS_PARTS1
	BOSS_PARTS_2,			// BOSS_PARTS2
	BOSS_PARTS_3,			// BOSS_PARTS3
	BOSS_PARTS_4,			// BOSS_PARTS4
	BOSS_PARTS_5,			// BOSS_PARTS5
	ENEMYTYPE_MAX
} ENEMYTYPE;


// �G�̍\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	ENEMYSTATE state;		// ���
	ENEMYTYPE nType;		// ���
	bool bUse;				// �g�p���Ă邩�ǂ���	
	int nCounterState;		// ��ԊǗ��J�E���^�[
	int nLife;				// �̗�
	int nCntBullet;			// �e�̏o�鎞��
	int	nCounter;			// �J�E���g�_�E��
	float SizeX;			// X���̑傫��		
	float SizeY;			// Y���̑傫��
	int nCounterAnim;		// ���[�V�����̃J�E���g
	int nPatternAnim;		// ���[�V�����̃J�E���g

	// �G�l�~�[�̑���
	bool Buk;		// �߂�V�X�e��
	bool flg;		// �ǔ��g�p
	bool bTP;		// TP�@�\
}Enemy;

// �v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nType );
void HitEnemy(Enemy* pEnemy, int nDamage);
Enemy* GetEnemy(void);

#endif
