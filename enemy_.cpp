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
#include "player.h"

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
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static Enemy s_aEnemy[MAX_ENEMY];
static int s_nCounterAnim;
static int s_nPatternAnim;

//----------------------------
// �v���g�^�C�v�錾
//----------------------------
static void UpdateSky1(Enemy* pEnemy);
static void UpdateSky2(Enemy* pEnemy);
static void UpdateBuckSky(Enemy* pEnemy);
static void UpdateZakato1(Enemy* pEnemy);	//�U�J�[�g�̏���1(�ǔ��A���A���̊Ԋu�Œe�𔭎˂��ď���)
static void UpdateZakato2(Enemy* pEnemy);	//�U�J�[�g�̏���2(�ǔ��i�V�A���̊Ԋu�Œe�𔭎˂��ď���)
static void UpdateZakato3(Enemy* pEnemy);	//�U�J�[�g�̏���3(�ǔ��A���A���Ԍo�߂Œe�𔭎˂��ď���)
static void UpdateZakato4(Enemy* pEnemy);	//�U�J�[�g�̏���4(�ǔ��i�V�A���Ԍo�߂Œe�𔭎˂��ď���)
static void ZakatoFinish(Enemy* pEnemy);	//�U�J�[�g���I������ۂ̏���

//--------------------------------------------------
// ������
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
// �X�V
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

		if (pEnemy->nLife > 0)
		{
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

			case TYPEENEMY_WARP_1:		//�ǔ��A���A���̋����Œe�𔭎˂��A����	
				UpdateZakato1(pEnemy);
				break;

			case TYPEENEMY_WARP_2:
				UpdateZakato2(pEnemy);
				break;

			case TYPEENEMY_WARP_3:		//�ǔ��A���A���Ԍo�߂Œe�𔭎˂��ď���
				UpdateZakato3(pEnemy);
				break;

			case TYPEENEMY_WARP_4:		//�ǔ��i�V�A���Ԍo�߂Œe�𔭎˂��ď���
				UpdateZakato4(pEnemy);
				break;
			default:
				break;
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
		{// �G�����񂾂Ƃ��̏����������Ɍ�������ł�������


		}

		// �ʒu���X�V
		pEnemy->pos += pEnemy->move;

		// ��ʒ[�ݒ�
		if (pEnemy->pos.x <= -100.0f || pEnemy->pos.x >= SCREEN_WIDTH + 100.0f || pEnemy->pos.y >= SCREEN_HEIGHT + 100.0f || pEnemy->pos.y <= -100.0f)
		{
			pEnemy->bUse = false;
			pEnemy->pos.x = SCREEN_WIDTH;
		}

		VERTEX_2D* pVtx;
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;	// ���_���W�f�[�^�̃|�C���^���S���i�߂�		

		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);

		s_nCounterAnim++;	// �A�j���[�V����
		if (s_nCounterAnim >= 10)
		{
			s_nCounterAnim = 0;
			s_nPatternAnim++;

			if (s_nPatternAnim >= 12)
			{
				s_nPatternAnim = 0;
			}

			pVtx[0].tex = D3DXVECTOR2((1.0f / 12.0f) * s_nPatternAnim, 0.0f);			//--------------------------�G���������Ƃ����₷
			pVtx[1].tex = D3DXVECTOR2((1.0f / 12.0f) * (s_nPatternAnim + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / 12.0f) * s_nPatternAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / 12.0f) * (s_nPatternAnim + 1), 1.0f);

		}

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
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		// ���_���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// �`��
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
//--------------------------------------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	Enemy* pEnemy = &s_aEnemy[nCntEnemy];

	pEnemy->nLife -= nDamage;

	//
	// �Փˎ���SE
	//

	VERTEX_2D*pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntEnemy;

	//PauseIN *pPauseIN = GetPausein();

	if (pEnemy->nLife < 0)	// �G�����񂾂Ƃ�
	{
		Player* pPlayer = GetPlayer();

		float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;				// �����ƓG�̂����v�Z
		float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;				// ���l���v�Z
		float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);

		pEnemy->move.x = -(moveEnemyX / moveEnemyR) * 30.0f;	// ���񂾂Ƃ����ł�
		pEnemy->move.y = -(moveEnemyY / moveEnemyR) * 30.0f;

		//
		// �X�R�A�̉��Z
		//

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		pEnemy->state = ENEMYSTATE_DAMAGE;
		pEnemy->nCounterState = 10;
		pVtx[0].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
	}

	// ���_���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �擾����
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return s_aEnemy;
}

//--------------------------------------------------
// �X�J�C1�̓G�̍X�V����
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
		&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
		&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
		&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
	{//�G�������鏈��

		pEnemy->flg = false;
		//s_aEnemy[nCntEnemy].move.y = 0;		//�R�����g�A�E�g�������ƒ��p�ɋȂ���
		pEnemy->move.x = 3;

	}
}

//--------------------------------------------------
// �X�J�C2�̓G�̍X�V����
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
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
}

//--------------------------------------------------
// �X�J�C3�̓G�̍X�V����
//--------------------------------------------------
void UpdateBuckSky(Enemy* pEnemy)
{
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
}

//-------------------------------------------------
//	�U�J�[�g1�̓G�̍X�V����
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