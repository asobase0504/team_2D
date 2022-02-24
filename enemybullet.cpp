//========================================================
//
//�e����
//Author:����]��
//
//========================================================
#include "enemybullet.h"
#include "enemy.h"
#include "player.h"
//====================================
//�}�N����`
//====================================
#define MAX_ENEMYBULLET		128		//�e�̍ő吔
#define SIZE_BULLET			15.0f	//�e�̑傫��
//====================================
//�O���[�o���ϐ�
//====================================
static LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static EnemyBullet g_aBullet[MAX_ENEMYBULLET];		//�e�̏��
//====================================
//���C���֐�
//====================================

//====================================
//�e�̏���������
//====================================
void InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet002.png",
		&g_pTextureBullet);

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
		g_aBullet[nCntBullet].bChase = true;	//�g�p���Ă����Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMYBULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}
//====================================
//�e�̏I������
//====================================
void UninitEnemyBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//====================================
//�e�̍X�V����
//====================================
void UpdateEnemyBullet(void)
{
	int nCntBullet;
	Player * pPlayer = GetPlayer();
	enemy * pEnemy = GetEnemy();
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//�e���g�p����Ă���
		 //�e�̈ʒu�X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			//���_���W�̍X�V
			VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntBullet * 4);		//�|�C���^��4���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffBullet->Unlock();

			if (g_aBullet[nCntBullet].pos.x <= -SIZE_BULLET || g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH + SIZE_BULLET || g_aBullet[nCntBullet].pos.y <= -SIZE_BULLET || g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT + SIZE_BULLET)
			{//�e����ʊO�ɏo��
				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
	}
}
//====================================
//�e�̕`�揈��
//====================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
		 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);	//�v���~�e�B�u(�|���S��)��
		}
	}
}
//====================================
//�e�̐ݒ菈��
//====================================
void SetEnemyBullet(D3DXVECTOR3 pos)
{
	Player * pPlayer = GetPlayer();

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;
			pVtx += (nCntBullet * 4);		//�Y���̈ʒu�܂Ői�߂�
			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			g_aBullet[nCntBullet].nLife = 0;
			g_aBullet[nCntBullet].bChase = true;		//�g�p���Ă�����
			g_aBullet[nCntBullet].bUse = true;		//�g�p���Ă�����
			if (g_aBullet[nCntBullet].bChase)
			{
				//�v���C���[�Ɍ������Ĕ��
				float moveBulletX = pPlayer->pos.x - g_aBullet[nCntBullet].pos.x;
				float moveBulletY = pPlayer->pos.y - g_aBullet[nCntBullet].pos.y;
				float moveBulletR = sqrtf(moveBulletX * moveBulletX + moveBulletY * moveBulletY);
				g_aBullet[nCntBullet].move.x = (moveBulletX / moveBulletR) * 3.0f;
				g_aBullet[nCntBullet].move.y = (moveBulletY / moveBulletR) * 3.0f;
				g_aBullet[nCntBullet].bChase = false;
			}
			break;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}