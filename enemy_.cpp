#include "main.h"
#include "enemy.h"
#include "Calculation.h"
#include "player.h"

//----------------------------
//�}�N��
#define COLLISION	(25)
#define JUDGEMENT	(100)	//������G�̔���T�C�Y
#define UP_ESCAPE	(200)	//��ɓ�����G�̔���T�C�Y
#define MAX_ENEMY	(255)

//----------------------------
//static �ϐ�
static LPDIRECT3DTEXTURE9 s_Texture[TYPEENEMY_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_PvtxBuff = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static enemy g_aEnemy[MAX_ENEMY];
static int g_nCounterAnimEnemy;
static int g_nPatternanimEnemy;

static D3DXCOLOR g_col;

//������
void InitEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// �ڋ�
		&s_Texture[TYPEENEMY_SKY_1]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// ��
		&s_Texture[TYPEENEMY_SKY_2]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 
		&s_Texture[TYPEENEMY_SKY_3]);

	ZeroMemory(g_aEnemy, sizeof(g_aEnemy[0]));

	// �G�̏��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].nLife = 1;
		g_aEnemy[nCntEnemy].flg = true;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_PvtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_PvtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
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
	s_PvtxBuff->Unlock();
}

// �I��
void UninitEnemy(void)
{
	for (int Count = 0; Count < TYPEENEMY_MAX; Count++)
	{
		if (s_Texture[Count] != NULL)
		{
			s_Texture[Count]->Release();
			s_Texture[Count] = NULL;
		}
	}

	// ���_�o�b�t�@�j��
	if (s_PvtxBuff != NULL)
	{
		s_PvtxBuff->Release();
		s_PvtxBuff = NULL;
	}
}

void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		enemy* pEnemy = &g_aEnemy[nCntEnemy];

		if (!pEnemy->bUse)
		{
			continue;	//�������P���΂��@[1]
		}

		Player* pPlayer = GetPlayer();

		if (pEnemy->flg)	//�ǔ�
		{//true �̎�
			float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;
			float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;
			float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);
			pEnemy->move.x = (moveEnemyX / moveEnemyR) * 3.0f;
			pEnemy->move.y = (moveEnemyY / moveEnemyR) * 3.0f;
		}

		if (pEnemy->pos.x + COLLISION >= pPlayer->pos.x - COLLISION
			&& pEnemy->pos.x - COLLISION <= pPlayer->pos.x + COLLISION
			&& pEnemy->pos.y + COLLISION >= pPlayer->pos.y - COLLISION
			&& pEnemy->pos.y - COLLISION <= pPlayer->pos.y + COLLISION)
		{//�e���W�d�Ȃ�

		 //HitPlayer(2);			//�_���[�W���󂯂�

		}

		if (pEnemy->nLife > 0)
		{
			//-------------------------------------------------------------
			//		����

			switch (pEnemy->nType)
			{
			case TYPEENEMY_SKY_1:
				if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
					&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
					&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
					&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
				{//�G�������鏈��

					pEnemy->flg = false;
					//g_aEnemy[nCntEnemy].move.y = 0;		//�R�����g�A�E�g�������ƒ��p�ɋȂ���
					pEnemy->move.x = 3;

				break;
			case TYPEENEMY_SKY_2:

				if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
					&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
					&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
					&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
				{// �G�������鏈��
					pEnemy->flg = false;
					pEnemy->move.y = 0;		// �R�����g�A�E�g�������ƒ��p�ɋȂ���
					pEnemy->move.x = 3;
				}
				break;
			case TYPEENEMY_SKY_3:		//�����Ԃ��G

				if (pEnemy->Buk)		//�߂鏈��
				{//true �̎�
					pEnemy->move.y += easeInOutBack(-0.25f);
				}

				if (pEnemy->pos.x + UP_ESCAPE >= pPlayer->pos.x - UP_ESCAPE
					&& pEnemy->pos.x - UP_ESCAPE <= pPlayer->pos.x + UP_ESCAPE
					&& pEnemy->pos.y + UP_ESCAPE >= pPlayer->pos.y - UP_ESCAPE
					&& pEnemy->pos.y - UP_ESCAPE <= pPlayer->pos.y + UP_ESCAPE)
				{// �G�������鏈��
					pEnemy->flg = false;		//�ǔ�
					g_aEnemy[nCntEnemy].Buk = true;		//�߂鏈��
				}
				break;
			case TYPEENEMY_GROUND_1:		//���i�G

				break;

			case TYPEENEMY_GROUND_2:		//�����Ȃ��G

				pBoss->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				break;
			default:
				break;
			}
			if (pBoss->pos.y > 0)
			{
				pBoss->nCntBullet++;
				if (pBoss->nCntBullet >= 120)
				{
					SetEnemyBullet(D3DXVECTOR3(pBoss->pos.x, pBoss->pos.y - pBoss->SizeY, 0.0f));
					pBoss->nCntBullet = 0;
				}
			}
		}
		else
		{// �G�����񂾂Ƃ��̏����������Ɍ�������ł�������


		}

		// �ʒu���X�V
		pBoss->pos += pBoss->move;

		// ��ʒ[�ݒ�			����									�E��						����						���
		if (pBoss->pos.x <= -100.0f || pBoss->pos.x >= SCREEN_WIDTH + 100.0f || pBoss->pos.y >= SCREEN_HEIGHT + 100.0f || pBoss->pos.y <= -100.0f)
		{
			pBoss->bUse = false;
			pBoss->pos.x = SCREEN_WIDTH;
		}

		VERTEX_2D* pVtx;

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		s_PvtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntBoss;	// ���_���W�f�[�^�̃|�C���^���S���i�߂�		

		pVtx[0].pos = D3DXVECTOR3(pBoss->pos.x - Boss_WIDTH, pBoss->pos.y - Boss_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBoss->pos.x + Boss_WIDTH, pBoss->pos.y - Boss_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBoss->pos.x - Boss_WIDTH, pBoss->pos.y + Boss_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBoss->pos.x + Boss_WIDTH, pBoss->pos.y + Boss_HEIGHT, 0.0f);

		g_nCounterAnimBoss++;								// �A�j���[�V����
		if (g_nCounterAnimBoss >= 10)
		{
			g_nCounterAnimBoss = 0;
			g_nPatternanimBoss++;

			if (g_nPatternanimBoss >= 12)
			{
				g_nPatternanimBoss = 0;
			}

			pVtx[0].tex = D3DXVECTOR2((1.0f / 12.0f) * g_nPatternanimBoss, 0.0f);			//--------------------------�G���������Ƃ����₷
			pVtx[1].tex = D3DXVECTOR2((1.0f / 12.0f) * (g_nPatternanimBoss + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / 12.0f) * g_nPatternanimBoss, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / 12.0f) * (g_nPatternanimBoss + 1), 1.0f);

		}

		switch (pBoss->state)
		{
		case  BossSTATE_NORMAL:
			break;

		case  BossSTATE_DAMAGE:
			pBoss->nCounerState--;
			if (pBoss->nCounerState <= 0)
			{
				pBoss->nCounerState = BossSTATE_NORMAL;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		// ���_���A�����b�N����
		s_PvtxBuffBoss->Unlock();
	}
}

// �`��
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_PvtxBuffBoss, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBoss = 0; nCntBoss < MAX_ENEMY; nCntBoss++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_TextureBoss[g_aBoss[nCntBoss].nType]);

		if (!g_aBoss[nCntBoss].bUse)
		{
			continue;
		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntBoss * 4,
			2);
	}
}

void SetEnemy(D3DXVECTOR3 pos, TYPEENEMY nType)
{
	for (int nCntBoss = 0; nCntBoss < MAX_ENEMY; nCntBoss++)
	{
		enemy* pEnemy = &g_aBoss[nCntBoss];

		if (pEnemy->bUse)
		{// �G���g�p����Ă���ꍇ
			continue;
		}

		pEnemy->nCntBullet = 0;
		pEnemy->pos = pos;
		pEnemy->nLife = 1;
		pEnemy->bUse = true;	// �g�p���Ă�����	
		//pEnemy->nCounter = 0;	// �C���N�������g
		pEnemy->nType = nType;

		VERTEX_2D*pVtx;
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		s_PvtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBoss * 4;	// ���_���W�f�[�^�̃|�C���^���S���i�߂�

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = pEnemy->pos.x - Boss_WIDTH;
		pVtx[0].pos.y = pEnemy->pos.y - Boss_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEnemy->pos.x + Boss_WIDTH;
		pVtx[1].pos.y = pEnemy->pos.y - Boss_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEnemy->pos.x - Boss_WIDTH;
		pVtx[2].pos.y = pEnemy->pos.y + Boss_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEnemy->pos.x + Boss_WIDTH;
		pVtx[3].pos.y = pEnemy->pos.y + Boss_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// ���_���A�����b�N����
		s_PvtxBuffBoss->Unlock();
		break;
	}
}

// �����菈��
void HitEnemy(int nCntBoss, int nDamage)
{
	enemy* pEnemy = &g_aBoss[nCntBoss];

	pEnemy->nLife -= nDamage;

	//
	// �Փˎ���SE
	//

	VERTEX_2D*pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_PvtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntBoss;

	//PauseIN *pPauseIN = GetPausein();

	if (pEnemy->nLife < 0)	// �G�����񂾂Ƃ�
	{
		float moveBossX = Player.x - pEnemy->pos.x;				// �����ƓG�̂����v�Z
		float moveBossY = Player.y - pEnemy->pos.y;				// ���l���v�Z
		float moveBossR = sqrtf(moveBossX * moveBossX + moveBossY * moveBossY);

		pEnemy->move.x = -(moveBossX / moveBossR) * 30.0f;	// ���񂾂Ƃ����ł�
		pEnemy->move.y = -(moveBossY / moveBossR) * 30.0f;

		//
		// �X�R�A�̉��Z
		//

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_col;
		pVtx[1].col = g_col;
		pVtx[2].col = g_col;
		pVtx[3].col = g_col;
	}
	else
	{
		pEnemy->state = BossSTATE_DAMAGE;
		pEnemy->nCounerState = 10;
		pVtx[0].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
	}

	// ���_���A�����b�N����
	s_PvtxBuffBoss->Unlock();
}

// �擾����
enemy *GetEnemy(void)
{
	return g_aBoss;
}
