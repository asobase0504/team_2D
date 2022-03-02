//**************************************************************************************************
//
// �^�[�Q�b�g�̐ݒ菈��
// Auther�F�������l
//
//**************************************************************************************************
#ifndef _TARGET_H_		//���̃}�N����`������ĂȂ�������
#define _TARGET_H_		//��d�C���N�\���[�h�h�~�̃}�N����`

#include "main.h"
#include "enemy.h"

//*************************************************************
// �}�N����`
//*************************************************************
#define	MAX_TARGET			(1280)

//*************************************************************
// �^�[�Q�b�g�̎�ނ̗񋓌^
//*************************************************************
typedef enum
{
	TARGETSTATE_LOCK_OFF = 0,
	TARGETSTATE_LOCK_ON,
	MAX_TARGETTYPE,
}TargetState;

//************************************************************
// �^�[�Q�b�g�e�N�X�`���̖��O���`
//************************************************************
typedef struct
{
	char	aName[128];
}TargetTextureName;

//************************************************************
// �^�[�Q�b�g�̍\���̂��`
//************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// ���S�_
	D3DXVECTOR3			rot;				// ����
	D3DXVECTOR3			size;				// �傫��
	D3DXCOLOR			col;				// �J���[
	TargetState			TargetType;			// �^�[�Q�b�g�̎��
	float				fLength;			// �Ίp���̒���
	float				fAngele;			// �Ίp���̊p�x
	int					nIdxBullet;			// �e�̎��ʔԍ�
	bool				bLockOnEnemy;		// �^�[�Q�b�g���ɓG������
	bool				bUse;				// �g�p���Ă邩�ǂ���
}Target;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitTarget(void);
void UninitTarget(void);
void UpdateTarget(void);
void DrawTarget(void);
int SetTarget(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetPositionTarget(int nIdxShadow, D3DXVECTOR3 pos);
void LockOnTarget(int nIdxShadow);
void SetTargetVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele);
void CollisionTarget(D3DXVECTOR3 pos, float fLength, ENEMYTYPE TypeEnemy);
Target *GetTarget(void);

#endif


