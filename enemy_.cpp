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

//**************************************************
// �}�N��
//**************************************************
#define COLLISION	(25)
#define RADIUS		(5.0f)
#define SPEED		(2.0f)
#define SHEO_DIST_START_ROTATION		(200.0f)		// �V�I�i�C�g�����n�߂鋗��
#define SHEO_RADIUS_ROTATION		(SHEO_DIST_START_ROTATION - 20.0f)	// �V�I�i�C�g�̉�]���a
#define SHEO_SPEED_ROTATION		(D3DXToRadian(6.0f))
#define SHEO_FRAME_GOODBY	(300)	// �V�I�i�C�g�����A��ɂȂ鎞��
#define SHEO_SPEED_GOODBY	(10.0f)	// �V�I�i�C�g�����A��ɂȂ鑬�x
#define JUDGEMENT	(100)	// ������G�̔���T�C�Y
#define UP_ESCAPE	(200)	// ��ɓ�����G�̔���T�C�Y

//**************************************************
// �X�^�e�B�b�N�ϐ�
//**************************************************
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;
static Enemy s_aEnemy[MAX_ENEMY];					// �G�̏��
static Enemy s_aTypeEnemy[ENEMYTYPE_MAX];

//**************************************************
// �v���g�^�C�v�錾
//**************************************************
static void TrackingMove(Enemy* pEnemy);	// �ǔ�����
static void UpdateSky1(Enemy* pEnemy);		// ��̓G1�̍X�V
static void UpdateSky2(Enemy* pEnemy);		// ��̓G2�̍X�V
static void UpdateBuckSky(Enemy* pEnemy);	// �A���̓G�̍X�V
static void MoveBakyura(Enemy* pEnemy);
static void MoveSheonite(Enemy* pEnemy);

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

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy1.png",
		&s_pTex[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BirdEnemy.png",
		&s_pTex[1]);

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);

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
	s_aTypeEnemy[ENEMYTYPE_SHEONITE].nLife = 30;
	s_aTypeEnemy[ENEMYTYPE_BAKYURA].nLife = 255;
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

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (!pEnemy->bUse)
		{
			continue;
		}

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
		case ENEMYTYPE_GROUND_1:	// ���i�G

			break;
		case ENEMYTYPE_GROUND_2:	// �����Ȃ��G

			break;
		case ENEMYTYPE_BAKYURA:		// ���G��
			MoveBakyura(pEnemy);
			break;
		case ENEMYTYPE_SHEONITE:	// ��̓G
			MoveSheonite(pEnemy);
			break;
		default:
			break;
		}

		// �G�l�~�[���_���[�W��H������Ƃ�
		switch (pEnemy->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;
		case  ENEMYSTATE_DAMAGE:
			pEnemy->nCntState--;
			if (pEnemy->nCntState <= 0)
			{
				pEnemy->nCntState = ENEMYSTATE_NORMAL;

				// ���_�J���[�̐ݒ�
				SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			}
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
			}
		}

		// �G�l�~�[�̃��C�t��0�ɂȂ����Ƃ�
		if (pEnemy->nLife < 0)
		{
			pEnemy->bUse = false;
		}

		// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
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
			s_aEnemy[nCnt].bUse = false;
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
		{	// �V�I�i�C�g�����A��ɂȂ��܂�
			distance = SHEO_DIST_START_ROTATION;
			pEnemy->move.x = sinf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
			pEnemy->move.y = cosf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
		}
		// �v���C���[�̈ʒu����V�I�i�C�g�̈ʒu���Z�o
		pEnemy->pos.x = pPlayer->pos.x + distance * sinf(pEnemy->fSheoRot);
		pEnemy->pos.y = pPlayer->pos.y + distance * cosf(pEnemy->fSheoRot);
	}
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

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{//�G�������鏈��
		pEnemy->bTracking = false;
		pEnemy->move.x = 3.0f;
	}

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

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
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
		if (pEnemy->nCntBullet >= 120)
		{
			// �G�l�~�[����v���C���[�܂ł̋����̎Z�o
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//�Ίp���̊p�x���Z�o
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����
			Direction.y = 0.0f;		// �g��Ȃ����Ȃ̂ŏ�����

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
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
	{//true �̎�
		pEnemy->move.y += easeInOutBack(-0.25f);
	}
	// �ǔ��̏���
	TrackingMove(pEnemy);

	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{// �G�������鏈��
		pEnemy->bTracking = false;		//�ǔ�
		pEnemy->bBack = true;		//�߂鏈��
	}

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

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// �`�揈��
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
		if (s_aEnemy[nCntEnemy].bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTex[s_aEnemy[nCntEnemy].nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntEnemy * 4,
				2);
		}
	}
}

//--------------------------------------------------
// �Z�b�g
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, float Size, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse)
		{
			continue;
		}

		// �G�l�~�[�ɏ�����������
		*pEnemy = s_aTypeEnemy[nType];

		// Use��true�ɂ���
		pEnemy->bUse = true;
		// �����̐ݒ�
		pEnemy->pos = pos;
		pEnemy->Size = Size;
		pEnemy->nType = nType;
		//pEnemy->nLife = 1; //	InitEnemy�ŉ��ݒ肵���̂ŁA�����ł͂��Ȃ�

		// ���_���W�̍X�V
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;
	}
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
		pEnemy->move.x = (moveEnemyX / moveEnemyR) * 3.0f;
		pEnemy->move.y = (moveEnemyY / moveEnemyR) * 3.0f;
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
