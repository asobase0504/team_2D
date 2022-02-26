//**************************************************************************************************
//
// �v���C���[�̐ݒ菈��
// Author�F�������l
//
//**************************************************************************************************
#ifndef _PLAYER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		//��d�C���N�\���[�h�h�~�̃}�N����`

#include"main.h"

#include"bullet.h"

//************************************************************
// �v���C���[�̍\���̂��`
//************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// ���S�_
	D3DXVECTOR3			rot;				// ����
	D3DXVECTOR3			move;				// �ړ���
	D3DXVECTOR3			size;				// �傫��
	D3DXCOLOR			col;				// �J���[
	BulletType			BulletType;			// �e�̎��
	float				fLength;			// �Ίp���̒���
	float				fAngele;			// �Ίp���̊p�x
	float				fSpeed;				// ���x
	int					nLife;				// �̗�
	int					nCntShot;			// �e�̔��˂܂ł̃J�E���g
	bool				bUse;				// �g�p���Ă邩�ǂ���
}Player;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetPlayerVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele);

D3DXVECTOR3 MovePlayer(void);
void ShotPlayer();
Player *GetPlayer(void);

#endif
