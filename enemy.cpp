//=========================================
// 
// �G����
// Author : Isoe Jukia
// Author : ����]��
// 
//=========================================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "Calculation.h"
#include "common.h"
#include "collision.h"
#include "score.h"
#include "player.h"
#include <stdio.h>

//----------------------------
// �}�N��
//----------------------------
#define COLLISION	(25)
#define JUDGEMENT	(100)	//������G�̔���T�C�Y
#define UP_ESCAPE	(200)	//��ɓ�����G�̔���T�C�Y

//----------------------------
// static �ϐ�
//----------------------------
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static Enemy s_aEnemy[MAX_ENEMY];

//----------------------------
// �v���g�^�C�v�錾
//----------------------------
static void TrackingMove(Enemy* pEnemy);	// �ǔ�����
static void UpdateSky1(Enemy* pEnemy);		// ��̓G1�̍X�V
static void UpdateSky2(Enemy* pEnemy);		// ��̓G2�̍X�V
static void UpdateBuckSky(Enemy* pEnemy);	// �A���̓G�̍X�V
static void UpdateZakato1(Enemy* pEnemy);	//�U�J�[�g�̏���1(�ǔ��A���A���̊Ԋu�Œe�𔭎˂��ď���)
static void UpdateZakato2(Enemy* pEnemy);	//�U�J�[�g�̏���2(�ǔ��i�V�A���̊Ԋu�Œe�𔭎˂��ď���)
static void UpdateZakato3(Enemy* pEnemy);	//�U�J�[�g�̏���3(�ǔ��A���A���Ԍo�߂Œe�𔭎˂��ď���)
static void UpdateZakato4(Enemy* pEnemy);	//�U�J�[�g�̏���4(�ǔ��i�V�A���Ԍo�߂Œe�𔭎˂��ď���)
static void ZakatoFinish(Enemy* pEnemy);	//�U�J�[�g���I������ۂ̏���

//--------------------------------------------------
// ������
// Author�FIsoe Jukia
//--------------------------------------------------
void InitEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// �ڋ�
		&s_pTex[ENEMYTYPE_SKY_1]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// ��
		&s_pTex[ENEMYTYPE_SKY_2]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 
		&s_pTex[ENEMYTYPE_SKY_3]);

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// �G�̏��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		s_aEnemy[nCntEnemy].nLife = 1;
		s_aEnemy[nCntEnemy].flg = true;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

		// �e�N�X�`�����W�̐ݒ�	   u      v
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / 12.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 12.0f, 1.0f);

		pVtx += 4;
	}

	// ���_���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
// Author�FIsoe Jukia
//--------------------------------------------------
void UninitEnemy(void)
{
	for (int Count = 0; Count < ENEMYTYPE_MAX; Count++)
	{
		if (s_pTex[Count] != NULL)
		{
			s_pTex[Count]->Release();
			s_pTex[Count] = NULL;
		}
	}

	// ���_�o�b�t�@�j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//
// �X�V
//
// Author�FIsoe Jukia
// Author�FTakano Keisuke
// Author�FYuda Kaito
// Author�FTomidokoro Tomoki
// Author�FBuriya Kouta
//
//--------------------------------------------------
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		if (!pEnemy->bUse)
		{
			continue;	//�������P���΂��@[1]
		}

		TrackingMove(pEnemy);	// �ǔ�����

		// ��ނ��Ƃ̍X�V����
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

			pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;
		case ENEMYTYPE_BAKYURA:		// ���G��

			break;
		case ENEMYTYPE_SHEONITE:	// ��̓G

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
		default:
			break;
		}

		// �ʒu���X�V
		pEnemy->pos += pEnemy->move;

		// ��ʒ[�ݒ�
		if (pEnemy->pos.x <= -100.0f || pEnemy->pos.x >= SCREEN_WIDTH + 100.0f || pEnemy->pos.y >= SCREEN_HEIGHT + 100.0f || pEnemy->pos.y <= -100.0f)
		{
			pEnemy->bUse = false;
			pEnemy->pos.x = SCREEN_WIDTH;
		}

		// �e�Ƃ̓����蔻��
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

		VERTEX_2D* pVtx;
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;	// ���_���W�f�[�^�̃|�C���^���S���i�߂�		

		// ���_���W�̐ݒ�
		SetVtxPos(pVtx, &pEnemy->pos, ENEMY_WIDTH, ENEMY_HEIGHT);

		// �e�N�X�`���̐ݒ�
		SetVtxAnimTex(pVtx, &pEnemy->nCounterAnim, 10, &pEnemy->nPatternAnim, 12);

		switch (pEnemy->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;
		case  ENEMYSTATE_DAMAGE:
			pEnemy->nCounterState--;
			if (pEnemy->nCounterState <= 0)
			{
				pEnemy->nCounterState = ENEMYSTATE_NORMAL;

				// ���_�J���[�̐ݒ�
				SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			}
			break;
		}

		// ���_���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// �`��
// Author�FIsoe Jukia
//--------------------------------------------------
void DrawEnemy(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntEnemy * 4,
			2);
	}
}

//--------------------------------------------------
// �ݒ�
// Author�FIsoe Jukia
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse)
		{// �G���g�p����Ă���ꍇ
			continue;
		}

		pEnemy->pos = pos;
		pEnemy->nLife = 1;
		pEnemy->bUse = true;	// �g�p���Ă�����
		pEnemy->flg = true;
		pEnemy->nType = nType;

		VERTEX_2D*pVtx;

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * 4;	// ���_���W�f�[�^�̃|�C���^���S���i�߂�

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = pEnemy->pos.x - ENEMY_WIDTH;
		pVtx[0].pos.y = pEnemy->pos.y - ENEMY_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEnemy->pos.x + ENEMY_WIDTH;
		pVtx[1].pos.y = pEnemy->pos.y - ENEMY_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEnemy->pos.x - ENEMY_WIDTH;
		pVtx[2].pos.y = pEnemy->pos.y + ENEMY_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEnemy->pos.x + ENEMY_WIDTH;
		pVtx[3].pos.y = pEnemy->pos.y + ENEMY_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// ���_���A�����b�N����
		s_pVtxBuff->Unlock();
		break;
	}
	return pEnemy;
}

//--------------------------------------------------
// �����菈��
// Author�FIsoe Jukia
//--------------------------------------------------
void HitEnemy(Enemy* pEnemy, int nDamage)
{
	pEnemy->nLife -= nDamage;

	//
	// �_���[�W����SE
	//
}

//--------------------------------------------------
// �擾����
// Author�FIsoe Jukia
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return s_aEnemy;
}

//--------------------------------------------------
// �ǔ�����
// Author�FIsoe Jukia
// Author�FYuda Kaito
//--------------------------------------------------
void TrackingMove(Enemy * pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	if (pEnemy->flg)	//�ǔ�
	{//true �̎�
		float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;
		float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;
		float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);
		pEnemy->move.x = (moveEnemyX / moveEnemyR) * 3.0f;
		pEnemy->move.y = (moveEnemyY / moveEnemyR) * 3.0f;
	}
}

//----------------------------
//�t�@�C���̓��̓}�b�v���
// Author�FHamada Ryuga
//----------------------------
void LoadSetFile(char *Filename)
{
	// �t�@�C���|�C���^�̐錾
	FILE* pFile;
	char aString[255];
	int Num_Tex;

	//�t�@�C�����J��
	pFile = fopen(&Filename[0], "r");
	int nCntEnemy = 0;
	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			aString[0] = {};
			fscanf(pFile, "%s", &aString[0]);
		}
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM_TEXTURE") == 0)
			{
				fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%d", &Num_Tex);

			}

			if (strcmp(&aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%s", &aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//�f�o�C�X�̎擾
				pDevice = GetDevice();
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					&aString[0],
					&s_pTex[nCntEnemy]);
				nCntEnemy++;
			}

			if (strcmp(&aString[0], "SET") == 0)
			{// �����񂪈�v�����ꍇ
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int  nType = 0;
				float  fSize = 0.0f;
				while (1)
				{
					// ������̏������Ɠǂݍ���
					aString[0] = {};
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{//����̂��ƃR�����g
						fgets(&aString[0], sizeof(aString), pFile);
						continue;
					}

					if (strcmp(&aString[0], "POS") == 0)
					{// �����񂪈�v�����ꍇ

						fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&aString[0], "TYPE") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&aString[0], "SIZE") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &fSize);
					}
					if (strcmp(&aString[0], "END_SET") == 0)
					{//�A�C�e���̏ꍇ
						SetEnemy(D3DXVECTOR3(s_modelpos), (ENEMYTYPE)nType);
						break;
					}
					if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{// �����񂪈�v�����ꍇ
						break;
					}
				}
			}
		}
	}

	//�t�@�C�������
	fclose(pFile);
}

//--------------------------------------------------
// �X�J�C1�̓G�̍X�V����
// Author�FIsoe Jukia
// Author�FYuda Kaito
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	if (pEnemy->nLife > 0)
	{	// �����Ă���ꍇ
		// �v���C���[���̎擾
		Player* pPlayer = GetPlayer();

		if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
			&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
			&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
			&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
		{//�G�������鏈��

			pEnemy->flg = false;
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

				SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
				pEnemy->nCntBullet = 0;
			}
		}
	}
	else
	{
		pEnemy->bUse = false;
	}
}

//--------------------------------------------------
// �X�J�C2�̓G�̍X�V����
// Author�FIsoe Jukia
// Author�FYuda Kaito
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
	if (pEnemy->nLife > 0)
	{	// �����Ă���ꍇ
		// �v���C���[���̎擾
		Player* pPlayer = GetPlayer();

		if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
			&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
			&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
			&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
		{// �G�������鏈��
			pEnemy->flg = false;
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

				SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
				pEnemy->nCntBullet = 0;
			}
		}
	}
	else
	{
		pEnemy->bUse = false;
	}
}

//--------------------------------------------------
// �X�J�C3�̓G�̍X�V����
// Author�FIsoe Jukia
// Author�FYuda Kaito
// Author�FTomidokoro Tomoki
//--------------------------------------------------
void UpdateBuckSky(Enemy* pEnemy)
{
	if (pEnemy->nLife > 0)
	{	// �����Ă���ꍇ
		if (pEnemy->Buk)	//�߂鏈��
		{//true �̎�
			pEnemy->move.y += easeInOutBack(-0.25f);
		}

		// �v���C���[���̎擾
		Player* pPlayer = GetPlayer();

		if (pEnemy->pos.x + UP_ESCAPE >= pPlayer->pos.x - UP_ESCAPE
			&& pEnemy->pos.x - UP_ESCAPE <= pPlayer->pos.x + UP_ESCAPE
			&& pEnemy->pos.y + UP_ESCAPE >= pPlayer->pos.y - UP_ESCAPE
			&& pEnemy->pos.y - UP_ESCAPE <= pPlayer->pos.y + UP_ESCAPE)
		{// �G�������鏈��
			pEnemy->flg = false;		//�ǔ�
			pEnemy->Buk = true;		//�߂鏈��
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

				SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
				pEnemy->nCntBullet = 0;
			}
		}
	}
	else
	{
		pEnemy->bUse = false;
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

	//�ǔ��A���A���̋����Œe�𔭎˂��A����
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2, 0.0f);
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
		pEnemy->flg = false;		//�ǔ�
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
		pEnemy->flg = false;		//�ǔ�
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
	SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
	pEnemy->nCntBullet = 0;
	pEnemy->flg = false;		//�ǔ�
	pEnemy->bTP = false;
	pEnemy->bUse = false;		//�G�l�~�[�𖳌�������
}