//==================================================
// 
// �G����
// Author : Isoe Jukia
// Author : ����]��
// 
//==================================================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "Calculation.h"
#include "common.h"
#include "collision.h"
#include "score.h"
#include "player.h"
#include "Boss.h"
#include "target.h"
#include "crater.h"
#include "sound.h"
#include "life.h"
#include <stdio.h>

//----------------------------
// �}�N��
//----------------------------
#define MOVE_SPEED   (2.5f)
#define COLLISION	(25)
#define RADIUS		(5.0f)
#define SPEED		(2.0f)
#define JUDGEMENT	(100)	// ������G�̔���T�C�Y
#define UP_ESCAPE	(200)	// ��ɓ�����G�̔���T�C�Y
#define ABILTIY		(0.005f)
#define MAXSTAGE	(5)
#define JUGE		(80)

//�V�I�i�C�g
#define SHEO_DIST_START_ROTATION	(200.0f)							// ���n�߂鋗��
#define SHEO_RADIUS_ROTATION		(SHEO_DIST_START_ROTATION - 20.0f)	// ��]���a
#define SHEO_SPEED_ROTATION			(D3DXToRadian(6.0f))				// ��]���x
#define SHEO_FRAME_GOODBY			(300)								// ���A��ɂȂ鎞��
#define SHEO_SPEED_GOODBY			(10.0f)								// ���A��ɂȂ鑬�x

//**************************************************
// �X�^�e�B�b�N�ϐ�
//**************************************************
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static Enemy s_aEnemy[MAX_ENEMY];					// �G�̏��
static Enemy s_aTypeEnemy[ENEMYTYPE_MAX];
static char EnemyLink[MAXSTAGE][256];
static int count = 0;

//**************************************************
// �v���g�^�C�v�錾
//**************************************************
static void TrackingMove(Enemy* pEnemy);	// �ǔ�����
static void UpdateSky1(Enemy* pEnemy);		// ��̓G1�̍X�V
static void UpdateSky2(Enemy* pEnemy);		// ��̓G2�̍X�V
static void UpdateBuckSky(Enemy* pEnemy);	// �A���̓G�̍X�V
static void MoveBakyura(Enemy* pEnemy);		// �o�L�����̈ړ�����
static void MoveSheonite(Enemy* pEnemy);	// �V�I�i�C�g�̈ړ�����
static void UpdateZakato1(Enemy* pEnemy);	// TOGETOGE
static void UpdateZakato2(Enemy* pEnemy);	// �U�J�[�g�̏���2(�ǔ��i�V�A���̊Ԋu�Œe�𔭎˂��ď���)
static void UpdateZakato3(Enemy* pEnemy);	// �U�J�[�g�̏���3(�ǔ��A���A���Ԍo�߂Œe�𔭎˂��ď���)
static void UpdateZakato4(Enemy* pEnemy);	// �U�J�[�g�̏���4(�ǔ��i�V�A���Ԍo�߂Œe�𔭎˂��ď���)
static void ZakatoFinish(Enemy* pEnemy);	// �U�J�[�g���I������ۂ̏���
static void ReflectMove(Enemy* pEnemy);
static void Updateflag(Enemy* pEnemy);		// �t���b�N�̏���

//**************************************************
// �O���[�o���֐�
//**************************************************
//--------------------------------------------------
// ������
// Auther�FIsoe Jukia
//--------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	count = 0;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fSize, pEnemy->pos.y - pEnemy->fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fSize, pEnemy->pos.y - pEnemy->fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fSize, pEnemy->pos.y + pEnemy->fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fSize, pEnemy->pos.y + pEnemy->fSize, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

		// �e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	// ���ƂŃt�@�C���œ��͂ł���悤�ɂ������ȁ[�[�[�[
	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		s_aTypeEnemy[nCnt].nLife = 1;
	}

	//�e�N�X�`���f�[�^�ǂݍ���
	LoadSetFile("data\\txt\\enemy.txt");
}

//--------------------------------------------------
// �I��
// Auther�FIsoe Jukia
//--------------------------------------------------
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		if (s_pTex[nCntEnemy] != NULL)
		{// �e�N�X�`���̔j��
			s_pTex[nCntEnemy]->Release();
			s_pTex[nCntEnemy] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//
// �X�V
//
// Auther�FIsoe Jukia
// Auther�FTakano Keisuke
// Auther�FYuda Kaito
// Auther�FTomidokoro Tomoki
// Auther�FBuriya Kouta
//
//--------------------------------------------------
void UpdateEnemy(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	Enemy* pEnemy = s_aEnemy;
	Player* pPlayer = GetPlayer();
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (!pEnemy->bUse)
		{
			continue;
		}

		// �ʒu���X�V
		pEnemy->pos.y += pEnemy->fSpeed;

		if (pEnemy->pos.y >= 0.0f - pEnemy->fSize)
		{
			pEnemy->pos += pEnemy->move;

			// �G�l�~�[�̃^�C�v���Ƃ̏���
			switch (pEnemy->nType)
			{
			case ENEMYTYPE_SKY_1:
				UpdateSky1(pEnemy);
				break;
			case ENEMYTYPE_SKY_2:
				UpdateSky2(pEnemy);
				break;
			case ENEMYTYPE_SKY_3:		// �����Ԃ��G
				UpdateBuckSky(pEnemy);
				break;
			case ENEMYTYPE_BAKYURA:		// ���G��
				MoveBakyura(pEnemy);
				break;
			case ENEMYTYPE_SHEONITE:	// ��̓G
				MoveSheonite(pEnemy);
				break;
			case ENEMYTYPE_GROUND_1:	// ���i�G
				pEnemy->nCntBullet++;

				if (pEnemy->nCntBullet >= 60)
				{
					// �G�l�~�[����v���C���[�܂ł̋����̎Z�o
					D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

					//�Ίp���̊p�x���Z�o
					Direction.z = atan2f(Direction.x, Direction.y);
					Direction.x = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����
					Direction.y = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����

					SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
					pEnemy->nCntBullet = 0;
				}
				break;
			case ENEMYTYPE_GROUND_2:	// �����Ȃ��G

				break;
			case ENEMYTYPE_WARP_1:		//�ǔ��A���A���̋����Œe�𔭎˂��A����	
				UpdateZakato1(pEnemy);
				break;
			case ENEMYTYPE_WARP_2:
				UpdateZakato2(pEnemy);
				break;
			case ENEMYTYPE_WARP_3:		//�ǔ��A���A���Ԍo�߂Œe�𔭎˂��ď���
				UpdateZakato3(pEnemy);
				break;
			case ENEMYTYPE_WARP_4:		//�ǔ��i�V�A���Ԍo�߂Œe�𔭎˂��ď���
				UpdateZakato4(pEnemy);
				break;
			case  FLAG_STATE:			//�t���O
				Updateflag(pEnemy);
				if (pEnemy->FlagOn)
				{
					// ���_�J���[�̐ݒ�
					SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
				}
				else
				{
					// ���_�J���[�̐ݒ�
					SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));
				}
				break;
			default:
				break;
			}

			// �e������������
			Bullet* pBullet = GetBullet();
			for (int i = 0; i < MAX_BULLET; i++, pBullet++)
			{
				if (!pBullet->bUse || pBullet->BulletType == BULLETTYPE_ENEMY)
				{
					continue;
				}
				if (CollisionCircle(pEnemy->pos, COLLISION, pBullet->pos, pBullet->size.x))
				{
					AddScore(255);
					HitEnemy(pEnemy, 1);
					pBullet->bUse = false;
				}
			}

			if (pEnemy->pos.y > 720.0f + pEnemy ->fSize+200.0f)
			{
				pEnemy->bTracking = false;
			}
			if (pEnemy->pos.y < 0.0f&&pEnemy->bBack)
			{
				pEnemy->bUse = false;
			}
			// �G�l�~�[�̃��C�t��0�ɂȂ����Ƃ�
			if (pEnemy->nLife <= 0)
			{
				pEnemy->bUse = false;
			}
		}
		// ���_���W�̐ݒ�
		SetVtxPos(pVtx, &pEnemy->pos, pEnemy->fSize, pEnemy->fSize);

		// �e�N�X�`���̐ݒ�
		SetVtxAnimTex(pVtx, &pEnemy->AnimTex);
		SetTex2d(pVtx, (1.0f / pEnemy->AnimTex.nDivisionX)*pEnemy->AnimTex.nPatternX
			, (1.0f / pEnemy->AnimTex.nDivisionX)* (pEnemy->AnimTex.nPatternX + 1)
			, 1.0f / pEnemy->AnimTex.nDivisionY*pEnemy->AnimTex.nPatternY
			, 1.0f / pEnemy->AnimTex.nDivisionY*(pEnemy->AnimTex.nPatternY + 1));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �X�J�C1�̓G�̍X�V����
// Auther�FIsoe Jukia
// Auther�FYuda Kaito
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();
	// �ǔ��̏���
	TrackingMove(pEnemy);

	//pEnemy->move.y = pEnemy->fSpeed;
	ReflectMove(pEnemy);

	// �e���o������
	if (pEnemy->pos.y > 0.0f)
	{	// ��ʓ��Ɏ��܂��Ă���ꍇ
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 60)
		{
			// �G�l�~�[����v���C���[�܂ł̋����̎Z�o
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//�Ίp���̊p�x���Z�o
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����
			Direction.y = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// �X�J�C2�̓G�̍X�V����
// Auther�FIsoe Jukia
// Auther�FYuda Kaito
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();
	// �ǔ��̏���
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{// �G�������鏈��
		pEnemy->bTracking = false;
		pEnemy->move.y = 0;		// �R�����g�A�E�g�������ƒ��p�ɋȂ���
		pEnemy->move.x = 3;
	}

	// �e���o������
	if (pEnemy->pos.y > 0.0f)
	{	// ��ʓ��Ɏ��܂��Ă���ꍇ
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 60)
		{
			// �G�l�~�[����v���C���[�܂ł̋����̎Z�o
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//�Ίp���̊p�x���Z�o
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����
			Direction.y = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// �X�J�C3�̓G�̍X�V����
// Auther�FIsoe Jukia
// Auther�FYuda Kaito
//--------------------------------------------------
void UpdateBuckSky(Enemy* pEnemy)
{
	//�߂鏈��
	if (pEnemy->bBack)
	{
		pEnemy->move.y += easeInOutBack(-0.25f);
	}
	// �ǔ��̏���
	TrackingMove(pEnemy);

	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	if (CollisionCircle(pEnemy->pos, 100, pPlayer->pos, pPlayer->size.x))
	{// �G�������鏈��
		pEnemy->bTracking = false;		//�ǔ�
		pEnemy->bBack = true;		//�߂鏈��
	}

	// �e���o������
	if (pEnemy->pos.y > 0.0f)
	{	// ��ʓ��Ɏ��܂��Ă���ꍇ
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 60)
		{
			// �G�l�~�[����v���C���[�܂ł̋����̎Z�o
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//�Ίp���̊p�x���Z�o
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����
			Direction.y = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// �o�L�����̓���
// Author: Buriya Kota
//--------------------------------------------------
void MoveBakyura(Enemy* pEnemy)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (!s_aEnemy[nCnt].bUse)
		{
			continue;
		}

		if (s_aEnemy[nCnt].pos.y > SCREEN_HEIGHT)
		{
			//s_aEnemy[nCnt].bUse = false;
		}
	}
}

//--------------------------------------------------
// �V�I�i�C�g�̓���
// Author: Buriya Kota
//--------------------------------------------------
void MoveSheonite(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();
	// �ǔ��̏���
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, 0.0f, pPlayer->pos, SHEO_DIST_START_ROTATION))
	{	// �͈͂ɓ���������
		pEnemy->bTracking = false;

		if (pEnemy->fSheoRot < 0.0f)
		{	// �����̊p�x
			D3DXVECTOR3 vecEnemy = pEnemy->pos - pPlayer->pos;
			pEnemy->fSheoRot = atan2f(vecEnemy.x, vecEnemy.y);
		}
		// ��������
		pEnemy->fSheoRot += SHEO_SPEED_ROTATION;
		float distance = SHEO_RADIUS_ROTATION;

		pEnemy->nSheoCnt++;
		if (pEnemy->nSheoCnt > SHEO_FRAME_GOODBY)
		{	// �V�I�i�C�g�̋A��
			distance = SHEO_DIST_START_ROTATION;
			pEnemy->move.x = sinf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
			pEnemy->move.y = cosf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
		}
		// �v���C���[�̈ʒu����V�I�i�C�g�̈ʒu���Z�o
		pEnemy->pos.x = pPlayer->pos.x + distance * sinf(pEnemy->fSheoRot);
		pEnemy->pos.y = pPlayer->pos.y + distance * cosf(pEnemy->fSheoRot);
	}
}

//-------------------------------------------------
//	�U�J�[�g1�̓G�̍X�V����
// Author�FTomidokoro Tomoki
//-------------------------------------------------
void UpdateZakato1(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();


	float fRotMove, fRotDest, fRotDiff;

	fRotMove = atan2f(pEnemy->move.x, pEnemy->move.y);			//���݂̈ړ�����

	fRotDest = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);			//�ړI�̈ړ�����

	fRotDiff = fRotDest - fRotMove; 	//�ړI�̈ړ������܂ł̍���

	//�p�x�̒l���C������
	if (fRotDiff > D3DX_PI)
	{//�ړI�̈ړ������ւ̍�����3.14����̏ꍇ
		fRotDiff -= D3DX_PI * 2;
	}

	else if (fRotDiff < -D3DX_PI)
	{//�ړI�̈ړ������ւ̍�����3.14��艺�̏ꍇ
		fRotDiff += D3DX_PI * 2;
	}

	fRotMove += fRotDiff * ABILTIY;				//�ړ������̕␳

	//move �C��
	if (fRotMove > D3DX_PI)
	{//���݂̈ړ������̊p�x��3.14�𒴂����ꍇ
		fRotMove -= D3DX_PI * 2;
	}

	else if (fRotMove < -D3DX_PI)
	{//���݂̈ړ������̊p�x��3.14��艺�̏ꍇ
		fRotMove += D3DX_PI * 2;
	}

	pEnemy->move.x = sinf(fRotMove) * MOVE_SPEED;
	pEnemy->move.y = cosf(fRotMove) * MOVE_SPEED;

	pEnemy->bTracking = false;

	// �e���o������
	if (pEnemy->pos.y > 0.0f)
	{	// ��ʓ��Ɏ��܂��Ă���ꍇ

		pEnemy->nCntBullet++;

		if (pEnemy->nCntBullet >= 120)
		{
			// �G�l�~�[����v���C���[�܂ł̋����̎Z�o
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//�Ίp���̊p�x���Z�o
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����
			Direction.y = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
			pEnemy->nCntBullet = 0;
		}
	}
}

//-------------------------------------------------
//	�U�J�[�g2�̓G�̍X�V����
//-------------------------------------------------
void UpdateZakato2(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	//�ǔ��i�V�A���̋����Œe�𔭎˂��A����
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2, 0.0f);
		pEnemy->bTracking = false;		//�ǔ�
		pEnemy->bTP = true;
	}

	if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
		&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
		&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
		&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
	{//�G���I�����鏈��
		ZakatoFinish(pEnemy);
	}
}

//-------------------------------------------------
//	�U�J�[�g3�̓G�̍X�V����
//-------------------------------------------------
void UpdateZakato3(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	//�ǔ��A���A���̋����Œe�𔭎˂��A����
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
		pEnemy->bTP = true;
	}
	if (pEnemy->nCntBullet >= 110)
	{// �G�������鏈��
		ZakatoFinish(pEnemy);
	}
}

//-------------------------------------------------
//	�U�J�[�g4�̓G�̍X�V����
//-------------------------------------------------
void UpdateZakato4(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	//�ǔ��i�V�A���Ԍo�߂Œe�𔭎˂��ď���
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
		pEnemy->bTracking = false;		//�ǔ�
		pEnemy->bTP = true;
	}
	if (pEnemy->nCntBullet >= 110)
	{// �G�������鏈��
		ZakatoFinish(pEnemy);
	}
}

//=============================================
// �U�J�[�g(�e���|�[�g����G)�̏I������ۂ̏���
//=============================================
void ZakatoFinish(Enemy* pEnemy)
{
	Player* pPlayer = GetPlayer();

	// �G�l�~�[����v���C���[�܂ł̋����̎Z�o
	D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

	//�Ίp���̊p�x���Z�o
	Direction.z = atan2f(Direction.x, Direction.y);
	Direction.x = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����
	Direction.y = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����

	//�e�̔���
	SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
	pEnemy->nCntBullet = 0;
	pEnemy->bTracking = false;		//�ǔ�
	pEnemy->bTP = false;
	pEnemy->bUse = false;		//�G�l�~�[�𖳌�������
}

//--------------------------------------------------
// �`��
// Author�FIsoe Jukia
//--------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTex[s_aEnemy[nCntEnemy].nType]);

		if (!s_aEnemy[nCntEnemy].bUse)
		{
			continue;
		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntEnemy * 4,2);
	}
}

//--------------------------------------------------
// �ݒ�
// Author�FIsoe Jukia
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, float fSize, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse)
		{
			continue;
		}

		// �G�l�~�[�ɏ�����������
		*pEnemy = s_aTypeEnemy[nType];

		pEnemy->bUse = true;	// �g�p���Ă����ԂɈڍs

		// �����̐ݒ�
		pEnemy->bTracking = true;
		pEnemy->bTracking = s_aTypeEnemy[nType].bTracking;
		pEnemy->pos = pos;
		pEnemy->fSize = fSize;
		pEnemy->nType = nType;
		pEnemy->fSpeed = s_aTypeEnemy[nType].fSpeed;
		//pEnemy->move.y = saTypeEnemy[nType].fSpeed;
		pEnemy->FlagOn = true;
		switch (pEnemy->nType)
		{
		case ENEMYTYPE_SHEONITE:
			pEnemy->fSheoRot = -1.0f;
			pEnemy->nSheoCnt = 0;
			pEnemy->nLife = 30;
			break;
		case ENEMYTYPE_SKY_1:
			pEnemy->bReflect = true;
			break;
		case FLAG_STATE:
			pEnemy->FlagOn = false;
		default:
			break;
		}

		// ���_���W�̐ݒ�
		SetVtxPos(pVtx, &pEnemy->pos, pEnemy->fSize, pEnemy->fSize);
		// ���_�J���[�̐ݒ�
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		break;
	}

	// ���_���A�����b�N����
	s_pVtxBuff->Unlock();

	return pEnemy;
}

//--------------------------------------------------
// �����菈��
// Auther�FIsoe Jukia
//--------------------------------------------------
void HitEnemy(Enemy* pEnemy, int nDamage)
{
	pEnemy->nLife -= nDamage;

	//
	// �_���[�W����SE
	//
}

//--------------------------------------------------
// �ǔ�����
// Auther�FIsoe Jukia
// Auther�FYuda Kaito
//--------------------------------------------------
void TrackingMove(Enemy * pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	if (pEnemy->bTracking)
	{
		float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;
		float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;
		float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);
		pEnemy->move.x = (moveEnemyX / moveEnemyR) * pEnemy->fSpeed;
		pEnemy->move.y = (moveEnemyY / moveEnemyR) * pEnemy->fSpeed;
		if (pEnemy->pos.y >= pPlayer->pos.y)
		{
			pEnemy->bTracking = false;
		}
	}
}

//--------------------------------------------------
// �擾����
// Auther�FIsoe Jukia
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return s_aEnemy;
}

//----------------------------
//�t�@�C���̓��̓}�b�v���
// Author�FHamada Ryuga
//----------------------------
void LoadSetFile(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;
	int     MoveSet = 0;

	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(Filename, "r");
	int nCntEnemy = 0;

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", &s_aString[0]);

			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//����̂��ƃR�����g
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "NUM_TEXTURE") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��

				fscanf(pFile, "%d", &Num_Tex);

			}
			if (strcmp(&s_aString[0], "DIVISION") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionX);
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionY);
			}
			if (strcmp(&s_aString[0], "ANIMESPEED") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nSpeed);
			}
			if (strcmp(&s_aString[0], "MOVESPEED") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%f", &s_aTypeEnemy[nCntEnemy].fSpeed);
			}
			if (strcmp(&s_aString[0], "TRACKING") == 0)
			{
				int checker = 0;
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%d", &checker);
				if (checker == 1)
				{
					s_aTypeEnemy[nCntEnemy].bTracking = true;
				}
				else
				{
					s_aTypeEnemy[nCntEnemy].bTracking = false;
				}
			}
			if (strcmp(&s_aString[0], "LIFE") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].nLife);
			}
			if (strcmp(&s_aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);	//���ǂݍ��ނ��
				fscanf(pFile, "%s", &s_aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//�f�o�C�X�̎擾
				pDevice = GetDevice();
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					&s_aString[0],
					&s_pTex[(ENEMYTYPE)nCntEnemy]);
			}
			if (strcmp(&s_aString[0], "END_TEX") == 0)
			{
				nCntEnemy++;

			}

			if (strcmp(&s_aString[0], "TYPESTATE") == 0)
			{// �����񂪈�v�����ꍇ
				while (strncmp(&s_aString[0], "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��

					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						//fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						//fscanf(pFile, "%f", &s_aTypeEnemy[MoveSet].fSpeed);
					}
					if (strcmp(&s_aString[0], "BACK") == 0)
					{
						int checker = 0;
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &checker);
						if (checker == 1)
						{
							s_aTypeEnemy[MoveSet].bBack = true;
						}
						else
						{
							s_aTypeEnemy[MoveSet].bBack = false;
						}

					}
					if (strcmp(&s_aString[0], "TRACKING") == 0)
					{
						int checker = 0;
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &checker);
						if (checker == 1)
						{
							s_aTypeEnemy[MoveSet].bTracking = true;
						}
						else
						{
							s_aTypeEnemy[MoveSet].bTracking = false;
						}
					}
					if (strcmp(&s_aString[0], "LIFE") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &s_aTypeEnemy[MoveSet].nLife);
					}
					if (strcmp(&s_aString[0], "ENDSTATE") == 0)
					{
						MoveSet++;
					}
				}

			}
			if (strcmp(&s_aString[0], "SET") == 0)
			{// �����񂪈�v�����ꍇ
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int  nType = 0;
				float  fSize = 0.0f;
				nCntEnemy = 0;
				while (1)
				{//188 2900
					// ������̏������Ɠǂݍ���
					s_aString[0] = {};
					fscanf(pFile, "%s", &s_aString[0]);

					if (strncmp(&s_aString[0], "#", 1) == 0)
					{//����̂��ƃR�����g
						fgets(&s_aString[0], sizeof(s_aString), pFile);
						continue;
					}

					if (strcmp(&s_aString[0], "POS") == 0)
					{// �����񂪈�v�����ꍇ

						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&s_aString[0], "TYPE") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nType);
						if (nType >= (int)ENEMYTYPE_MAX)
						{//�񋓌^�̐����傫�������琔��������������
							nType = 0;
						}
					}
					if (strcmp(&s_aString[0], "SIZE") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &fSize);
					}
					if (strcmp(&s_aString[0], "END_SET") == 0)
					{//�Z�b�g
						SetEnemy(D3DXVECTOR3(s_modelpos),fSize,(ENEMYTYPE)nType);
						break;
					}
					if (strcmp(&s_aString[0], "END_SCRIPT") == 0)
					{// �����񂪈�v�����ꍇ
						break;
					}
				}
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
}

//--------------------
//�w�肵��file�p�X����݂���
//--------------------
void EnemyLynk(char *Filename)
{
	if (EnemyLink[count][0] == '\0')
	{
		strcat(&EnemyLink[count][0], &Filename[0]);//�����@aFile��-�����ɓ����
		count++;
	}
}

//--------------------
//�w�肵��file�p�X���Ăяo��
//--------------------
void SetEnemyLynk(int number)
{
	LoadSetFile(&EnemyLink[number][0]);
}

//--------------------
// �����蔻��
//--------------------
void ReflectMove(Enemy* pEnemy)
{
	Player* pPlayer = GetPlayer();

	if (pEnemy->bReflect)
	{
		if (pEnemy->pos.x >= pPlayer->pos.x - pPlayer->size.x
			&& pEnemy->pos.x <= pPlayer->pos.x + pPlayer->size.x)
		{//�G���I�����鏈��			
			pEnemy->bReflect = false;
			pEnemy->bTracking = false;
			pEnemy->move.x += -pEnemy->move.x * 3;
			//			pEnemy->move.y += -pEnemy->move.y * 3;
		}
	}
}

//--------------------
//�G����
// Author�FHamada Ryuga
//--------------------
void falseSetEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!s_aEnemy[nCntEnemy].bUse)
		{
			continue;
		}
		s_aEnemy[nCntEnemy].bUse = false;
	}
}
//--------------------
//��
// Author : ����]��
// Author�FHamada Ryuga
//--------------------
void Updateflag(Enemy* pEnemy)	// 
{
	// �e������������
	Bullet* pBullet = GetBullet();
	if (CollisionCircle(pEnemy->pos, COLLISION, pBullet->pos, pBullet->size.x))
	{
		//�����Ńt���b�N�̉�

		pEnemy->FlagOn = true;
	}

	Player* pPlayer = GetPlayer();
	if ((CollisionCircle(pEnemy->pos, COLLISION, pPlayer->pos, pPlayer->size.x)) && pEnemy->FlagOn)
	{
		pEnemy->bUse = false;
		SetLife();
	}


}