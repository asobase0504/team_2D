#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

// �}�N����`
#define MAX_Boss	256			// �G�̍ő吔
#define Boss_WIDTH  (25.0f)		// X	�G�T�C�Y
#define Boss_HEIGHT (25.0f)		// Y	�G�T�C�Y

typedef enum
{
	BossSTATE_NORMAL = 0,	// �ʏ���
	BossSTATE_DAMAGE,		// �_���[�W����
	BossSTATE_MAX
}BossSTATE;

typedef enum
{
	TYPEENEMY_SKY_1,		// ��̓G�P
	TYPEENEMY_SKY_2,		// ��̓G�Q
	TYPEENEMY_SKY_3,		// ��̓G�R
	TYPEENEMY_GROUND_1,		// �n��̓G1
	TYPEENEMY_GROUND_2,		// �n��̓G2
	TYPEENEMY_MAX
} TYPEENEMY;

// �G�̍\���̒�`
typedef struct
{
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 pos;		// �ʒu
	BossSTATE state;		// ���
	int nCounerState;		// ��ԊǗ��J�E���^�[
	int nLife;				// �̗�
	int nCntBullet;			// �e�̏o�鎞��
	TYPEENEMY nType;		// ���
	bool bUse;				// �g�p���Ă邩�ǂ���	
	int	nCounter;			// �J�E���g�_�E��
	int SizeX;				// X���̑傫��		
	int SizeY;				// Y���̑傫��

	//---------------
	//ENEMY�@MOVE
	bool Buk;		//�߂�V�X�e��
	bool flg;		//�ǔ��g�p

}enemy;

// �v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, TYPEENEMY nType );
void HitEnemy(int nCntBoss, int nDamage);
enemy* GetEnemy(void);


#endif
