//========================================================
//
// �w�i�̏���
// Author : �g�~�h�R���@�g���L
//
//========================================================
#include "main.h"
#include "bg.h"

//�}�N����`
#define NUM_BG (1)			//�w�i�̐�
#define BG_WIDTH (1.0f)		//�w�i�̉���
#define BG_HEIGHT (0.01f)	//�w�i�̏c��
#define MAX_HEIGHT (1.0f)	//�w�i�̍ő�l

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = { NULL };			//�e�N�X�`��(�R�����j�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;				//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR2 g_posTexV;										//�e�N�X�`�����W�̊J�n�ʒu(V�l)
D3DXVECTOR2 g_BGmovePlayer;									//�ړ���

static float HeightSize;
static float WidthSize;
//========================================================
//�w�i�̏���������
//========================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntBG;

	WidthSize = 0.0f;
	HeightSize = 0.0f;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bg000.png",
		&g_pTextureBG[0]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	g_BGmovePlayer = D3DXVECTOR2(0.0f, 0.01f);			//�ړ��ʂ�������

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//���_���W�̎w��
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(BG_WIDTH, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BG_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2(BG_WIDTH, BG_HEIGHT);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}
//========================================================
//�w�i�̏I������
//========================================================
void UninitBG(void)
{
	//�e�N�X�`���̔j��
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_pTextureBG[nCntBG] != NULL)
		{
			g_pTextureBG[nCntBG]->Release();
			g_pTextureBG[nCntBG] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}
//========================================================
//�w�i�̍X�V����
//========================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	HeightSize++;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	g_posTexV.x = 0.0f;
	g_posTexV.y = HeightSize * 0.0001f;

	pVtx[0].tex = D3DXVECTOR2(g_posTexV.x, MAX_HEIGHT - (g_posTexV.y + BG_HEIGHT));
	pVtx[1].tex = D3DXVECTOR2(BG_WIDTH + g_posTexV.x, MAX_HEIGHT - (g_posTexV.y + BG_HEIGHT));
	pVtx[2].tex = D3DXVECTOR2(g_posTexV.x, MAX_HEIGHT - g_posTexV.y);
	pVtx[3].tex = D3DXVECTOR2(BG_WIDTH + g_posTexV.x, MAX_HEIGHT - g_posTexV.y);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}
//====================================================
//�w�i�̕`�揈��
//====================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntBG;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̕`��
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		//�|���S���̕`�� �l�p
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntBG * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}