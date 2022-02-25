//----------------------------------------------
//
//Enemy.cpp
//��]�W���L�A
//
//----------------------------------------------

#include "main.h"
#include "enemy.h"
#include "Boss.h"
#include "player.h"

#define MAX_BOSS	(255)	//�G�̐�

static LPDIRECT3DTEXTURE9 s_TextureBOSS[ENEMYTYPE_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_PvtxBuffBOSS = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static BOSS s_aBoss[MAX_BOSS];
static Player s_Player;

// �v���g�^�C�v�錾
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data/image/�X���C��A_�ړ�000.png",				// �撣���ĉ摜�\��t���Ă˂�
		&s_TextureBOSS[BOSS_PARTS_1]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data/image/�X���C��B_�ړ�000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_2]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data/image/�X���C��C_�ړ�000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_3]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data/image/�X���C��D_�ړ�000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_4]);

	// �e�N�X�`���ǂݍ���	�G�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data/image/�ǂ���A_�U��000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_5]);

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{
		s_aBoss[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aBoss[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aBoss[nCount].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_PvtxBuffBOSS,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_PvtxBuffBOSS->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x - ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x + ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x - ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x + ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y + ENEMY_HEIGHT, 0.0f);

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
	s_PvtxBuffBOSS->Unlock();

}
void UninitBoss(void)
{
	for (int Count = 0; Count < ENEMYTYPE_MAX; Count++)
	{
		if (s_TextureBOSS[Count] != NULL)
		{
			s_TextureBOSS[Count]->Release();
			s_TextureBOSS[Count] = NULL;
		}
	}

	// ���_�o�b�t�@�j��
	if (s_PvtxBuffBOSS != NULL)
	{
		s_PvtxBuffBOSS->Release();
		s_PvtxBuffBOSS = NULL;
	}
}
void UpdateBoss(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!s_aBoss->bUse)
		{
			continue;	//�������P���΂��@[1]
		}
		if (s_aBoss->pos.x + COLLISION_B >= s_Player.pos.x - COLLISION_B
			&& s_aBoss->pos.x - COLLISION_B <= s_Player.pos.x + COLLISION_B
			&& s_aBoss->pos.y + COLLISION_B >= s_Player.pos.y - COLLISION_B
			&& s_aBoss->pos.y - COLLISION_B <= s_Player.pos.y + COLLISION_B)
		{//�e���W�d�Ȃ�

		 //HitPlayer(2);			//�_���[�W���󂯂�

		}

		if (s_aBoss->nLife > 0)
		{
			switch (s_aBoss->nType)
			{
			case BOSS_PARTS_1:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//�G�������鏈��
					

				}
				break;
			case BOSS_PARTS_2:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//�G�������鏈��
				 

				}
				break;
			case BOSS_PARTS_3:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//�G�������鏈��
				
				
				}
				break;
			case BOSS_PARTS_4:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//�G�������鏈��
				 

				}
				break;
			case BOSS_PARTS_5:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//�G�������鏈��
				

				}
				break;
			default:
				break;
			}
		}
		else
		{// �G�����񂾂Ƃ��̏����������Ɍ�������ł�������


		}

		// �ʒu���X�V
		s_aBoss->pos += s_aBoss->move;

		VERTEX_2D* pVtx;

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		s_PvtxBuffBOSS->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;	// ���_���W�f�[�^�̃|�C���^���S���i�߂�		

		pVtx[0].pos = D3DXVECTOR3(s_aBoss->pos.x - ENEMY_WIDTH, s_aBoss->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aBoss->pos.x + ENEMY_WIDTH, s_aBoss->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aBoss->pos.x - ENEMY_WIDTH, s_aBoss->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aBoss->pos.x + ENEMY_WIDTH, s_aBoss->pos.y + ENEMY_HEIGHT, 0.0f);

		switch (s_aBoss->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;

		case  ENEMYSTATE_DAMAGE:
			s_aBoss->nCounerState--;
			if (s_aBoss->nCounerState <= 0)
			{
				s_aBoss->nCounerState = ENEMYSTATE_NORMAL;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		// ���_���A�����b�N����
		s_PvtxBuffBOSS->Unlock();
	}
}
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_PvtxBuffBOSS, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_TextureBOSS[s_aBoss[nCntEnemy].nType]);

		if (!s_aBoss[nCntEnemy].bUse)
		{
			continue;
		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntEnemy * 4,
			2);
	}
}
void SetBoss(D3DXVECTOR3 pos/*, TYPEENEMY nType*/)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (s_aBoss->bUse)
		{// �G���g�p����Ă���ꍇ
			continue;
		}

		SetEnemy(D3DXVECTOR3(pos.x-50.0f, pos.y+50.0f,0.0f), BOSS_PARTS_1);
		SetEnemy(D3DXVECTOR3(pos.x+50.0f, pos.y + 50.0f, 0.0f), BOSS_PARTS_2);
		SetEnemy(D3DXVECTOR3(pos.x -50.0f, pos.y-50.0f, 0.0f), BOSS_PARTS_3);
		SetEnemy(D3DXVECTOR3(pos.x+50.0f, pos.y -50.0f, 0.0f), BOSS_PARTS_4);
		SetEnemy(D3DXVECTOR3(pos.x+0.0f, pos.y + 0.0f, 0.0f), BOSS_PARTS_5);

		s_aBoss->pos = pos;
		//s_aBoss->nLife = 1;
		s_aBoss->bUse = true;	// �g�p���Ă�����	
		//s_aBoss->nCounter = 0;	// �C���N�������g
		//s_aBoss->nType = nType;

		VERTEX_2D*pVtx;
		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		s_PvtxBuffBOSS->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * 4;	// ���_���W�f�[�^�̃|�C���^���S���i�߂�

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = s_aBoss->pos.x - ENEMY_WIDTH;
		pVtx[0].pos.y = s_aBoss->pos.y - ENEMY_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = s_aBoss->pos.x + ENEMY_WIDTH;
		pVtx[1].pos.y = s_aBoss->pos.y - ENEMY_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = s_aBoss->pos.x - ENEMY_WIDTH;
		pVtx[2].pos.y = s_aBoss->pos.y + ENEMY_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = s_aBoss->pos.x + ENEMY_WIDTH;
		pVtx[3].pos.y = s_aBoss->pos.y + ENEMY_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// ���_���A�����b�N����
		s_PvtxBuffBOSS->Unlock();
		break;
	}
}


//Set�@�ł����Ă���
//�v���O�����̊m�F
//�����̏���
