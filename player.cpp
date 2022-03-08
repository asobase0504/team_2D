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
#include "key_config.h"
#include "map.h"
#include "fade.h"

//*******************************************************************************
// �萔
//*******************************************************************************
#define MAX_SPEED				(20.0f)																		// �ړ����x�̍ő�l
#define MIN_SPEED				(0.0f)																		// �ړ����x�̍ŏ��l
#define MOVE_SPEED				(MAX_SPEED)																		// �ݒ莞�̈ړ���
#define MOVE_FRICTION			(0.5f)																		// �����C�W��
#define MAX_CNT_SKY_SHOT		(0.25f * 60)																// �󒆒e���ˊԊu(�b�� * �t���[����)
#define MAX_CNT_GRAND_SHOT		(0.7f * 60)																	// �n��e���ˊԊu(�b�� * �t���[����)
#define TARGET_DISTANCE			(300.0f)																	// �^�[�Q�b�g�̊Ԋu
#define PLAYER_RADIUS			(50.0f)																		// ���a
#define	START_LIFE				(5)																			// �ŏ��̗̑�
#define RESPAWN_PLAYER			(60)																		// ���X�|�[���ҋ@����
#define START_POS_Y				(SCREEN_HEIGHT * 0.8f)														// �X�^�[�g���̈ʒu(Y)
#define SPAWN_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT + PLAYER_RADIUS, 0.0f))		// �X�|�[�����̈ʒu

//******************************************************************************
// �ÓI�ϐ�
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
static Player s_Player;										// �v���C���[��

//******************************************************************************
// �v���g�^�C�v�錾
//*******************************************************************************
static D3DXVECTOR3 MovePlayer(void);	// �ړ�
static void LockMovePlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 size);	// �ړ�����

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
	SetPlayer(SPAWN_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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
	// �v���C���[�̏��
	switch (s_Player.state)
	{
	case PLAYER_STATE_NORMAL:
	{
		// �ړ��x�N�g���̍X�V
		s_Player.move = MovePlayer();

		// �ʒu�̍X�V
		s_Player.pos += s_Player.move;

		// �e�̔���
		ShotPlayer();

		// �v���C���[�̈ړ�����
		LockMovePlayer(&s_Player.pos, s_Player.size);

		if (s_Player.nIdxTarge != -1)
		{
			// �^�[�Q�b�g�̈ړ�
			SetPositionTarget(s_Player.nIdxTarge, D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f));
		}

		// �e������������
		Bullet* pBullet = GetBullet();
		for (int i = 0; i < MAX_BULLET; i++, pBullet++)
		{
			if (!pBullet->bUse || pBullet->BulletType != BULLETTYPE_ENEMY)
			{// �e���g�p����Ă��Ȃ����A�G�̒e�ȊO�̎�
				continue;
			}

			if (CollisionCircle(s_Player.pos, s_Player.size.x, pBullet->pos, pBullet->size.x))
			{// �e�ɓ�������//�ʂ������ɓ񔭂����������P��������悤�ɂ���by�l�c����
				Target *pTarget = GetTarget();
				StartFadeOut();//�����ꂪ�t�F�[�h
				s_Player.nLife--;
				DeleteBullet();
				int Map = GetStage();
				ConteSet(Map);//MAP�ݒ�
				s_Player.state = PLAYER_STATE_DAMAGE;
				s_Player.nCntState = RESPAWN_PLAYER;
				s_Player.pos = SPAWN_POS;
				pTarget += s_Player.nIdxTarge;
				pTarget->bUse = false;
				pBullet->bUse = false;
				HitLife();
				if (s_Player.nLife <= 0)
				{	// ���U���g�Ɉڍs
					ChangeMode(MODE_RESULT);
				}
				break;
			}
		}

		// �F�����肷��
		s_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
	case PLAYER_STATE_DAMAGE:
		s_Player.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_Player.nCntState--;

		if (s_Player.nCntState <= 0)
		{// �J�E���g��0�ȉ��̎�
			s_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			s_Player.state = PLAYER_STATE_START;
			s_Player.nCntState = 0;
		}
		break;

	case PLAYER_STATE_START:
		// �ʒu�̍X�V
		if (s_Player.nLife == START_LIFE)
		{
			s_Player.pos.y += -1.0f;
		}
		else
		{
			s_Player.pos.y += -5.0f;
		}

		if (s_Player.pos.y <= START_POS_Y)
		{// �X�^�[�g�ʒu�Ɏ�������
			s_Player.pos.y = START_POS_Y;
			s_Player.state = PLAYER_STATE_NORMAL;

			// �^�[�Q�b�g
			s_Player.nIdxTarge = SetTarget(D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f), s_Player.rot);
		}

		break;
	}
	
	// ���_���ւ̃|�C���^�𐶐�						
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// �v���C���[�`��
//---------------------------------------------------------------------------
void DrawPlayer(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//���_���ւ̃|�C���^�𐶐�						
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!s_Player.bUse)
	{//�g�p����ĂȂ��ꍇ
		// �v���C���[���̐ݒ�
		s_Player.pos = pos;													// ���S�_
		s_Player.rot = rot;													// ����
		s_Player.size = D3DXVECTOR3(PLAYER_RADIUS, PLAYER_RADIUS, 0.0f);		// �T�C�Y
		s_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						// �J���[
		s_Player.BulletType = (BulletType)(0);								// �e�̎��
		s_Player.state = PLAYER_STATE_START;								// �ʏ���
		s_Player.nLife = START_LIFE;										// �̗�
		s_Player.fSpeed = MOVE_SPEED;										// ���x
		s_Player.nIdxTarge = -1;											// �^�[�Q�b�g
		s_Player.nCntState = 0;												// ��ԕύX�܂ł̃J�E���g
		s_Player.bUse = true;												// �g�p���Ă�

		for (int i = 0; i < 2; i++)
		{
			s_Player.BulletData[i].nCntShot = 1;			// �e���˂܂ł̃J�E���g
			s_Player.BulletData[i].nCntShotUse = 0;			// �e�̔��˂��ł���܂ł̃J�E���g
			s_Player.BulletData[i].bTriggerShot = false;	// �g���K�[�e���˂̉s��
			s_Player.BulletData[i].bPressShot = false;		// �v���X�e���˂̉s��
		}

		//�Ίp���̒������Z�o����
		s_Player.fLength = sqrtf((s_Player.size.x * s_Player.size.x) + (s_Player.size.y * s_Player.size.y))* 0.5f;

		//�Ίp���̊p�x���Z�o
		s_Player.fAngele = atan2f(s_Player.size.x, s_Player.size.y);

		//���_���W�̐ݒ�
		SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

		for (int i = 0; i < s_Player.nLife; i++)
		{
			SetLife();
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
static D3DXVECTOR3 MovePlayer(void)
{
	// �ϐ��錾
	D3DXVECTOR3 move = s_Player.move;

	// �ϐ��錾
	D3DXVECTOR3 moveDir = MoveKey();		// �ړ�����

	if (moveDir.x != 0.0f || moveDir.y != 0.0f)
	{
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
	// �󒆒e
	{
		BulletData* pBulletData = &s_Player.BulletData[BULLETTYPE_PLAYER_SKY];

		if (GetKeyboardPress(DIK_SPACE) || GetDirectJoypadPress(JOYKEY_DIRECT_2_BUTTON))
		{
			pBulletData->nCntShot++;
		}

		if ((GetKeyboardTrigger(DIK_SPACE) ||  GetDirectJoypadTrigger(JOYKEY_DIRECT_2_BUTTON))
			&& !pBulletData->bTriggerShot
			&& !pBulletData->bPressShot)
		{
			// �g���K�[�e�ƃv���X�e���g�p���Ă���
			pBulletData->bTriggerShot = true;
			pBulletData->bPressShot = true;

			// �J�E���g���ő�l�ɂ���
			pBulletData->nCntShot = MAX_CNT_SKY_SHOT;
		}

		if ((GetKeyboardRelease(DIK_SPACE) || GetDirectJoypadRelease(JOYKEY_DIRECT_2_BUTTON))
			&& pBulletData->bPressShot
			&& pBulletData->bTriggerShot)
		{// �L�[�������ꂽ��
			pBulletData->nCntShotUse = pBulletData->nCntShot;
			pBulletData->nCntShot = 0;
			pBulletData->bPressShot = false;
		}

		if (pBulletData->bTriggerShot
			&& !pBulletData->bPressShot)
		{
			// �J�E���g�C���N�������g
			pBulletData->nCntShotUse++;

			if (pBulletData->nCntShotUse >= MAX_CNT_SKY_SHOT)
			{
				// �J�E���g��������
				pBulletData->nCntShotUse = 0;
				pBulletData->bTriggerShot = false;
			}
		}

		if (pBulletData->nCntShot >= MAX_CNT_SKY_SHOT)
		{// �e�̔���
			SetBullet(D3DXVECTOR3(s_Player.pos.x - (s_Player.size.x / 2.0f), s_Player.pos.y - s_Player.size.y, 0.0f), s_Player.rot, BULLETTYPE_PLAYER_SKY, -1, true);
			SetBullet(D3DXVECTOR3(s_Player.pos.x + (s_Player.size.x / 2.0f), s_Player.pos.y - s_Player.size.y, 0.0f), s_Player.rot, BULLETTYPE_PLAYER_SKY, -1, true);
			pBulletData->nCntShot = 0;
		}
	}

	// �n��e
	{
		BulletData* pBulletData = &s_Player.BulletData[BULLETTYPE_PLAYER_GROUND];

		if (GetKeyboardPress(DIK_RETURN) || GetDirectJoypadPress(JOYKEY_DIRECT_0_BUTTON))
		{
			pBulletData->nCntShot++;
		}

		if ((GetKeyboardTrigger(DIK_RETURN) || GetDirectJoypadTrigger(JOYKEY_DIRECT_0_BUTTON))
			&& !pBulletData->bTriggerShot
			&& !pBulletData->bPressShot)
		{
			// �g���K�[�e�ƃv���X�e���g�p���Ă���
			pBulletData->bTriggerShot = true;
			pBulletData->bPressShot = true;

			// �J�E���g���ő�l�ɂ���
			pBulletData->nCntShot = MAX_CNT_GRAND_SHOT;
		}

		if ((GetKeyboardRelease(DIK_RETURN) || GetDirectJoypadRelease(JOYKEY_DIRECT_0_BUTTON))
			&& pBulletData->bPressShot
			&& pBulletData->bTriggerShot)
		{// �L�[�������ꂽ��
			pBulletData->nCntShotUse = pBulletData->nCntShot;
			pBulletData->nCntShot = 0;
			pBulletData->bPressShot = false;
		}

		if (pBulletData->bTriggerShot
			&& !pBulletData->bPressShot)
		{
			// �J�E���g�C���N�������g
			pBulletData->nCntShotUse++;

			if (pBulletData->nCntShotUse >= MAX_CNT_GRAND_SHOT)
			{
				// �J�E���g��������
				pBulletData->nCntShotUse = 0;
				pBulletData->bTriggerShot = false;
			}
		}

		if (pBulletData->nCntShot >= MAX_CNT_GRAND_SHOT)
		{// �e�̔���
			LockOnTarget(s_Player.nIdxTarge);
			// �^�[�Q�b�g
			s_Player.nIdxTarge = SetTarget(D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f), s_Player.rot);
			pBulletData->nCntShot = 0;
		}
	}
}

//---------------------------------------------------------------------------
// �v���C���[�̈ړ�����
//---------------------------------------------------------------------------
static void LockMovePlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 size)
{
	if (0.0f + (size.x / 2.0f) >= pos->x)
	{
		pos->x = 0.0f + (size.x / 2.0f);
	}
	if (0.0f + (size.y / 2.0f) >= pos->y)
	{
		pos->y = 0.0f + (size.y / 2.0f);
	}
	if (SCREEN_WIDTH - (size.x / 2.0f) <= pos->x)
	{
		pos->x = SCREEN_WIDTH - (size.x / 2.0f);
	}
	if (SCREEN_HEIGHT - (size.y / 2.0f) <= pos->y)
	{
		pos->y = SCREEN_HEIGHT - (size.y / 2.0f);
	}
}

//---------------------------------------------------------------------------
// �v���C���[�̎擾
//---------------------------------------------------------------------------
Player *GetPlayer(void)
{
	return &s_Player;
}