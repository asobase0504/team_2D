//**************************************************************************************************
//
// �v���C���[�̐ݒ菈��
// Author�F�������l
//
//**************************************************************************************************
#include "main.h"
#include "input.h"
#include "life.h"
#include "common.h"
#include "player.h"
#include "collision.h"
#include "target.h"
#include "map.h"

//*******************************************************************************
// �萔
//*******************************************************************************
#define MAX_SPEED				(10.0f)			// �ړ����x�̍ő�l
#define MIN_SPEED				(0.0f)			// �ړ����x�̍ŏ��l
#define MOVE_SPEED				(50.0f)			// �ݒ莞�̈ړ���
#define MOVE_FRICTION			(0.5f)			// �����C�W��
#define MAX_CNT_SKY_SHOT		(0.25f * 60)	// �󒆒e���ˊԊu(�b�� * �t���[����)
#define MAX_CNT_GRAND_SHOT		(0.7f * 60)		// �n��e���ˊԊu(�b�� * �t���[����)
#define TARGET_DISTANCE			(300.0f)		// �^�[�Q�b�g�̊Ԋu
#define START_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f))	// �X�^�[�g���̈ʒu

//*******************************************************************************
// �O���[�o���ϐ�
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
static Player s_Player;										// �v���C���[��

//---------------------------------------------------------------------------
// �v���C���[������
//---------------------------------------------------------------------------
void InitPlayer(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v���C���[�ɒ���t����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,					// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,													// ���_�t�@�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// �v���C���[���̏�����
	memset(&s_Player, 0, sizeof(s_Player));

	// ���_���ւ̃|�C���^�𐶐�						
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �Ίp���̒������Z�o����
	s_Player.fLength = sqrtf(((s_Player.size.x * s_Player.size.x) + (s_Player.size.y * s_Player.size.y)) * 0.5f);

	// �Ίp���̊p�x���Z�o
	s_Player.fAngele = atan2f(s_Player.size.x, s_Player.size.y);

	// ���_���W�̐ݒ�
	SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();

	// �v���C���[�̃Z�b�g
	SetPlayer(START_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//---------------------------------------------------------------------------
// �v���C���[�I��
//---------------------------------------------------------------------------
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��	  
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// ���_�o�b�t�@��j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//---------------------------------------------------------------------------
// �v���C���[�X�V
//---------------------------------------------------------------------------
void UpdatePlayer(void)
{
	// �ړ��x�N�g���̍X�V
	s_Player.move = MovePlayer();

	// �ʒu�̍X�V
	s_Player.pos += s_Player.move;

	// �e�̔���
	ShotPlayer();

	if (s_Player.nIdxTarge != -1)
	{
		// �^�[�Q�b�g�̈ړ�
		SetPositionTarget(s_Player.nIdxTarge, D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f));
	}
	
	// ���_���ւ̃|�C���^�𐶐�						
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();

	// �e������������
	Bullet* pBullet = GetBullet();
	for (int i = 0; i < MAX_BULLET; i++, pBullet++)
	{
		if (!pBullet->bUse || pBullet->BulletType != BULLETTYPE_ENEMY)
		{
			continue;
		}
		if (CollisionCircle(s_Player.pos, s_Player.size.x, pBullet->pos, pBullet->size.x))
		{
			s_Player.pos = START_POS;
			s_Player.nLife--;
			ConteSet();	
			pBullet->bUse = false;
		}
	}
}

//---------------------------------------------------------------------------
// �v���C���[�`��
//---------------------------------------------------------------------------
void DrawPlayer(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (s_Player.bUse)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		//�v���C���[�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//---------------------------------------------------------------------------
// �v���C���[�ݒ�
//---------------------------------------------------------------------------
void SetPlayer(D3DXVECTOR3	pos, D3DXVECTOR3 rot)
{
	//���_���ւ̃|�C���^�𐶐�						
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!s_Player.bUse)
	{//�g�p����ĂȂ��ꍇ
		// �v���C���[���̐ݒ�
		s_Player.pos = pos;									// ���S�_
		s_Player.rot = rot;									// ����
		s_Player.size = D3DXVECTOR3(50.0f,50.0f,0.0f);		// �T�C�Y
		s_Player.col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);		// �J���[
		s_Player.BulletType = (BulletType)(0);				// �e�̎��
		s_Player.nLife = 5;									// �̗�
		s_Player.nCntShot = 1;								// �e���˂܂ł̃J�E���g
		s_Player.fSpeed = MOVE_SPEED;						// ���x
		s_Player.nIdxTarge = -1;							// �^�[�Q�b�g
		s_Player.nCntShotUse = 0;							// �e�̔��˂��ł���܂ł̃J�E���g
		s_Player.bTriggerShot = false;						// �g���K�[�e���˂̉s��
		s_Player.bPressShot = false;						// �v���X�e���˂̉s��
		s_Player.bUse = true;								// �g�p���Ă�

		//�Ίp���̒������Z�o����
		s_Player.fLength = sqrtf((s_Player.size.x * s_Player.size.x) + (s_Player.size.y * s_Player.size.y))* 0.5f;

		//�Ίp���̊p�x���Z�o
		s_Player.fAngele = atan2f(s_Player.size.x, s_Player.size.y);

		//���_���W�̐ݒ�
		SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

		// �^�[�Q�b�g
		s_Player.nIdxTarge = SetTarget(D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE,0.0f), s_Player.rot);

		for (int i = 0; i < s_Player.nLife; i++)
		{
			SetLife(D3DXVECTOR3(25.0f + 42.5f * i, SCREEN_HEIGHT - 25.0f, 0.0f));
		}
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// �v���C���[�̒��_�֌W�̐ݒ�
//---------------------------------------------------------------------------
void SetPlayerVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele)
{
	//���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z + (D3DX_PI + fAngele)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z + (D3DX_PI + fAngele)) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngele)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngele)) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - (0 + fAngele)) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - (0 + fAngele)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z - (0 - fAngele)) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z - (0 - fAngele)) * fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	SetVtxColor(pVtx,&col);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//---------------------------------------------------------------------------
// �v���C���[�ړ�
//---------------------------------------------------------------------------
D3DXVECTOR3 MovePlayer(void)
{
	// �ϐ��錾
	D3DXVECTOR3 move = s_Player.move;

	// �ϐ��錾
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ�����

	if (GetKeyboardPress(DIK_W))
	{// [W]�L�[�������ꂽ��
		moveDir.y += -1.0f;
	}
	if (GetKeyboardPress(DIK_A))
	{// [A]�L�[�������ꂽ��
		moveDir.x += -1.0f;
	}
	if (GetKeyboardPress(DIK_D))
	{// [D]�L�[�������ꂽ��
		moveDir.x += 1.0f;
	}
	if (GetKeyboardPress(DIK_S))
	{// [S]�L�[�������ꂽ��
		moveDir.y += 1.0f;
	}

	if (moveDir.x != 0.0f || moveDir.y != 0.0f)
	{
		// �ړ������x�N�g���̑傫����1.0f�ɂ���
		D3DXVec3Normalize(&moveDir, &moveDir);

		// ���x�̎Z�o
		move += moveDir * s_Player.fSpeed;

		// ���x���~�b�^�[
		float speed = D3DXVec3Length(&move);
		if (speed > MAX_SPEED)
		{
			move = move / speed * MAX_SPEED;
		}
	}


	// �����C�W����������
	move *= (1.0f - MOVE_FRICTION);

	// ���x���~�b�^�[
	if (D3DXVec3Length(&move) < MIN_SPEED)
	{
		move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return move;
}

//---------------------------------------------------------------------------
// �e�̔���
//---------------------------------------------------------------------------
void ShotPlayer()
{
	// �ϐ��錾
	int nCntMaxShot;

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// �E�{�^���������ꂽ
		s_Player.BulletType = (BulletType)((int)(s_Player.BulletType) + 1);

		if (s_Player.BulletType >= MAX_BULLETTYPE - 1)
		{// �^�C�v���𒴂����Ƃ�
			s_Player.BulletType = (BulletType)(0);
		}
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// ���{�^���������ꂽ
		s_Player.BulletType = (BulletType)((int)(s_Player.BulletType) - 1);

		if (s_Player.BulletType <= -1)
		{// �^�C�v���𒴂����Ƃ�
			s_Player.BulletType = (BulletType)((int)(MAX_BULLETTYPE)-2);
		}
	}

	switch (s_Player.BulletType)
	{
	case BULLETTYPE_PLAYER_SKY:
		nCntMaxShot = (int)(MAX_CNT_SKY_SHOT);
		break;

	case BULLETTYPE_PLAYER_GROUND:
		nCntMaxShot = (int)(MAX_CNT_GRAND_SHOT);
		break;
	}

	if (GetKeyboardTrigger(DIK_SPACE)
		&& !s_Player.bTriggerShot
		&& !s_Player.bPressShot)
	{
		// �g���K�[�e���g�p���Ă���
		s_Player.bTriggerShot = true;
		s_Player.bPressShot = true;

		s_Player.nCntShot = nCntMaxShot;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{// SPACE�������ꂽ
		s_Player.nCntShot++;
	}
	if (GetKeyboardRelease(DIK_SPACE)
		&& s_Player.bPressShot
		&& s_Player.bTriggerShot)
	{
		s_Player.nCntShotUse = s_Player.nCntShot;
		s_Player.nCntShot = 0;
		s_Player.bPressShot = false;
	}

	if (s_Player.bTriggerShot
		&& !s_Player.bPressShot)
	{
		// �J�E���g�C���N�������g
		s_Player.nCntShotUse++;

		if (s_Player.nCntShotUse >= nCntMaxShot)
		{
			// �J�E���g��������
			s_Player.nCntShotUse = 0;
			s_Player.bTriggerShot = false;
		}
	}

	if (s_Player.nCntShot >= nCntMaxShot)
	{// �e�̔���
		switch (s_Player.BulletType)
		{
		case BULLETTYPE_PLAYER_SKY:
			SetBullet(D3DXVECTOR3(s_Player.pos.x - (s_Player.size.x / 2.0f), s_Player.pos.y - s_Player.size.y, 0.0f), s_Player.rot, s_Player.BulletType, -1, true);
			SetBullet(D3DXVECTOR3(s_Player.pos.x + (s_Player.size.x / 2.0f), s_Player.pos.y - s_Player.size.y, 0.0f), s_Player.rot, s_Player.BulletType, -1, true);
			break;

		case BULLETTYPE_PLAYER_GROUND:
			LockOnTarget(s_Player.nIdxTarge);
			// �^�[�Q�b�g
			s_Player.nIdxTarge = SetTarget(D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f), s_Player.rot);
			break;
		}
		
		s_Player.nCntShot = 0;
	}
}

Player *GetPlayer(void)
{
	return &s_Player;
}