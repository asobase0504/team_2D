//**************************************************************************************************
//
// �e�̐ݒ菈��
// Auther�F�������l
//
//**************************************************************************************************
#ifndef _BULLET_H_		//���̃}�N����`������ĂȂ�������
#define _BULLET_H_		//��d�C���N�\���[�h�h�~�̃}�N����`

#include"main.h"

//*************************************************************
// �}�N����`
//*************************************************************
#define	MAX_BULLET			(1280)

//*************************************************************
// �e�̎�ނ̗񋓌^
//*************************************************************
typedef enum
{
	BULLETTYPE_SKY = 0,
	BULLETTYPE_GROUND,
	MAX_BULLETTYPE,
}BulletType;

//************************************************************
// �|���S���e�N�X�`���̖��O���`
//************************************************************
typedef struct
{
	char	aName[128];
}BulletTextureName;

//************************************************************
// �|���S���̍\���̂��`
//************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// ���S�_
	D3DXVECTOR3			rot;				// ����
	D3DXVECTOR3			size;				// �傫��
	D3DXVECTOR3			move;				// �ړ��x�N�g��
	D3DXCOLOR			col;				// �J���[
	BulletType			BulletType;			// �e�̎��
	float				fLength;			// �Ίp���̒���
	float				fAngele;			// �Ίp���̊p�x
	float				fRotMove;			// �ړ������̊p�x
	float				fSpeed;				// �ړ����x
	int					nLife;				// ����
	bool				bUse;				// �g�p���Ă邩�ǂ���
}Bullet;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
void SetBulletVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele);

#endif

