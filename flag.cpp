//========================================================
//
//SP�t���b�O
//Author:����]��
//
//========================================================
#include "flag.h"
#include "player.h"
#include "bullet.h"
//====================================
//�}�N����`
//====================================
#define MAX_FLAG		(128)		//SP�t���b�O�̍ő吔
#define NUM_FLAG		(2)			//SP�t���b�O�̎��
//====================================
//�O���[�o���ϐ�
//====================================
LPDIRECT3DTEXTURE9 g_pTextureFlag[NUM_FLAG] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFlag = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Flag g_aFlag[MAX_FLAG];								//SP�t���b�O�̏��
//====================================
//SP�t���b�O�̏���������
//====================================
void InitFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntFlag;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag001.png",
		&g_pTextureFlag[0]);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag000.png",
		&g_pTextureFlag[1]);
	//SP�t���b�O�̏��̏�����
	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		g_aFlag[nCntFlag].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].state = FLAG_STATE_OFF;
		g_aFlag[nCntFlag].fHeight = 0.0f;
		g_aFlag[nCntFlag].fWidth = 0.0f;
		g_aFlag[nCntFlag].fAngle = 0.0f;
		g_aFlag[nCntFlag].fLength = 0.0f;
		g_aFlag[nCntFlag].nType = 0;
		g_aFlag[nCntFlag].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FLAG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFlag,
		NULL);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
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
	g_pVtxBuffFlag->Unlock();
}
//====================================
//SP�t���b�O�̏I������
//====================================
void UninitFlag(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureFlag[NUM_FLAG] != NULL)
	{
		g_pTextureFlag[NUM_FLAG]->Release();
		g_pTextureFlag[NUM_FLAG] = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFlag != NULL)
	{
		g_pVtxBuffFlag->Release();
		g_pVtxBuffFlag = NULL;
	}
}
//====================================
//SP�t���b�O�̍X�V����
//====================================
void UpdateFlag(void)
{

		VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
		{
			if (g_aFlag[nCntFlag].nType == 0)
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

				pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
			}
			else
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
			}
		}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffFlag->Unlock();
}
//====================================
//SP�t���b�O�̕`�揈��
//====================================
void DrawFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntFlag;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFlag, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == true)
		{//SP�t���b�O���g�p����Ă���
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureFlag[g_aFlag[nCntFlag].nType]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFlag * 4, 2);	//�v���~�e�B�u(�|���S��)��
		}
	}
}
//====================================
//SP�t���b�O�̍폜����
//====================================
void DeleteFlag(int nCntFlag)
{
	//SP�t���b�O�̔�\��
	g_aFlag[nCntFlag].bUse = false;
}
//====================================
//SP�t���b�O�̐ݒ菈��
//====================================
void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	int nCntFlag;
	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == false)
		{//SP�t���b�O���g�p����Ă��Ȃ�
			g_aFlag[nCntFlag].pos = pos;
			g_aFlag[nCntFlag].fWidth = fWidth;
			g_aFlag[nCntFlag].fHeight = fHeight;
			g_aFlag[nCntFlag].nType = nType;
			g_aFlag[nCntFlag].state = FLAG_STATE_OFF;
			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);

			g_aFlag[nCntFlag].bUse = true;		//�g�p���Ă�����
			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFlag->Unlock();
}
//====================================
//SP�t���b�O���̎擾
//====================================
Flag * GetFlag(void)
{
	return &g_aFlag[0];
}
//====================================
//SP�t���b�O�̓����蔻��(�e�p)
//====================================
void CollisionFlagBullet(D3DXVECTOR3 pPos, float fWidth, float fHeight)
{
	Bullet *pBullet = GetBullet();
	Flag *pFlag = GetFlag();
	float RightPlayer = pPos.x + fWidth / 2;
	float LeftPlayer = pPos.x - fWidth / 2;
	float BottomPlayer = pPos.y;
	float TopPlayer = pPos.y - fHeight;
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		float  RightFlag = g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2;
		float  LeftFlag = g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2;
		float  BottomFlag = g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2;
		float  TopFlag = g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2;
		if (g_aFlag[nCntFlag].bUse == true)
		{
			if (RightFlag >= LeftPlayer&& LeftFlag <= RightPlayer && BottomFlag >= TopPlayer && TopFlag <= BottomPlayer)
			{
				if (g_aFlag[nCntFlag].nType == 0)
				{//�e�Ƃ̓����蔻��
					g_aFlag[nCntFlag].nType = 1;
					pBullet->bUse = false;
				}
			}
		}
		pFlag++;
	}
}
//====================================
//SP�t���b�O�̓����蔻��(�v���C���[)
//====================================
void CollisionFlagPlayer(D3DXVECTOR3 pPos, float fWidth, float fHeight)
{
	Player *pPlayer = GetPlayer();
	Flag * pFlag = GetFlag();
	float RightPlayer = pPos.x + fWidth / 2;
	float LeftPlayer = pPos.x - fWidth / 2;
	float BottomPlayer = pPos.y;
	float TopPlayer = pPos.y - fHeight;
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		float  RightFlag = g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2;
		float  LeftFlag = g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2;
		float  BottomFlag = g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2;
		float  TopFlag = g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2;
		if (g_aFlag[nCntFlag].bUse == true)
		{
			if (RightFlag >= LeftPlayer&& LeftFlag <= RightPlayer && BottomFlag >= TopPlayer && TopFlag <= BottomPlayer)
			{
				if (g_aFlag[nCntFlag].nType == 1)
				{//�v���C���[�Ƃ̓����蔻��
					pPlayer->nLife++;
					DeleteFlag(nCntFlag);
				}
			}
		}
		pFlag++;
	}
}