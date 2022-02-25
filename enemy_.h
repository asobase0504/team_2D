#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

// �}�N����`
#define ENEMY_WIDTH  (25.0f)		// X	�G�T�C�Y
#define ENEMY_HEIGHT (25.0f)		// Y	�G�T�C�Y
#define MAX_ENEMY	(255)	//�G�̐�

typedef enum
{
	EnemySTATE_NORMAL = 0,	// �ʏ���
	EnemySTATE_DAMAGE,		// �_���[�W����
	EnemySTATE_MAX
}ENEMYSTATE;

typedef enum
{
	TYPEENEMY_SKY_1,		// ��̓G�P
	TYPEENEMY_SKY_2,		// ��̓G�Q
	TYPEENEMY_SKY_3,		// ��̓G�R

	//--------------------
	//�{�X
	BOSS_PARTS_1,
	BOSS_PARTS_2,
	BOSS_PARTS_3,
	BOSS_PARTS_4,
	BOSS_PARTS_5,

	TYPEENEMY_MAX
} TYPEENEMY;

// �G�̍\���̒�`
typedef struct
{
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 pos;		// �ʒu
	ENEMYSTATE state;		// ���
	int nCounerState;		// ��ԊǗ��J�E���^�[
	int nLife;				// �̗�
	TYPEENEMY nType;		// ���
	bool bUse;				// �g�p���Ă邩�ǂ���	
	int	nCounter;			// �J�E���g�_�E��
	int SizeX;				// X���̑傫��		
	int SizeY;				// Y���̑傫��

	//---------------
	//ENEMY�@MOVE
	bool Buk;		//�߂�V�X�e��
	bool flg;		//�ǔ��g�p

}Enemy;

// �v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, TYPEENEMY nType );
void HitEnemy(int nCntBoss, int nDamage);
Enemy* GetEnemy(void);


#endif
