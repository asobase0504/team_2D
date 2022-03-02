//================================
//
// �^�C�g�����
// Author:Teruto Sato
//
//================================
#include "title.h"
#include "input.h"
#include "map.h"
#include "fade.h"

//�}�N����`
#define NUM_TITLE	(3)					//�e�N�X�`���̍ő吔
#define BLINK_TIME	(200)				//�_�łɂ����鎞��
#define BLINK_HALF	(BLINK_TIME / 2)	//�_�ł��؂�ւ�鎞��
#define MAP_SPEED	(0.0005f)			//��ʃX�N���[���̑���

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9			s_pTexture[NUM_TITLE] = {};		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^;
static TITLE s_Title[NUM_TITLE];	//�^�C�g���̏��̎擾
static int s_nTime = 0;			//�_�ł̎���

//============================
// �^�C�g���̏���������
//============================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//------------------------------
	//	�e�N�X�`���̓ǂݍ���
	//------------------------------
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\map.png", &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rogo.png", &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press.png", &s_pTexture[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE,	//�m�ۂ���o�b�t�@�̃T�C�Y
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//------------------------------
	//	�\���̂̏�����
	//------------------------------
	//�ʒu�̏�����
	s_Title[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_Title[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f);
	s_Title[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f);

	//���̏�����
	s_Title[0].fWidth = SCREEN_WIDTH / 2;
	s_Title[1].fWidth = 300.0f;
	s_Title[2].fWidth = 450.0f;

	//�����̏�����
	s_Title[0].fHeight = SCREEN_HEIGHT / 2;
	s_Title[1].fHeight = 80.0f;
	s_Title[2].fHeight = 50.0f;

	//�F�̏�����
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		s_Title[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------
	//	���_���̐ݒ�
	//------------------------------
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		TITLE *title = s_Title + nCnt;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(title->pos.x - title->fWidth, title->pos.y - title->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(title->pos.x + title->fWidth, title->pos.y - title->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(title->pos.x - title->fWidth, title->pos.y + title->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(title->pos.x + title->fWidth, title->pos.y + title->fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = title->col;
		pVtx[1].col = title->col;
		pVtx[2].col = title->col;
		pVtx[3].col = title->col;

		//�e�N�X�`�����W�̐ݒ�
		if (nCnt == 0)
		{//�w�i�̃e�N�X�`���Ȃ�
			pVtx[0].tex = D3DXVECTOR2(0.2f, 0.7f);
			pVtx[1].tex = D3DXVECTOR2(0.4f, 0.7f);
			pVtx[2].tex = D3DXVECTOR2(0.2f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);
		}
		else
		{//����ȊO�̃e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	InitMap();		// �}�b�v
	// �}�b�v�̐ݒ�B
	InitMapSet(MAP_FILE0);
}

//============================
// �^�C�g���̏I������
//============================
void UninitTitle(void)
{
	UninitMap();	// �}�b�v
					
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================
// �^�C�g���̍X�V����
//============================
void UpdateTitle(void)
{
	UpdateMap();	// �}�b�v�f�[�^

	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nTime++;				//�^�C���̉��Z
	s_nTime %= BLINK_TIME;	//�^�C���̏�����

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		TITLE *title = s_Title + nCnt;

		//------------------------
		//	�e�N�X�`���̓_��
		//------------------------
		if (s_nTime >= BLINK_HALF)
		{//�^�C����40�ȏ�Ȃ�
			s_Title[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�\������
		}
		else
		{//�^�C����40�����Ȃ�
			s_Title[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	//�\�����Ȃ�
		}

		//------------------------
		//	�w�i�̃X�N���[��
		//------------------------
		if (nCnt == 0)
		{//�w�i�̃e�N�X�`���Ȃ�
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
			pVtx[1].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
			pVtx[2].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
			pVtx[3].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = title->col;
		pVtx[1].col = title->col;
		pVtx[2].col = title->col;
		pVtx[3].col = title->col;

		pVtx += 4;
	}

	//----------------
	//	��ʑJ��
	//----------------
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A,0))
	{

		SetFade(MODE_GAME);
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//============================
// �^�C�g���̕`�揈��
//============================
void DrawTitle(void)
{
	DrawMap();		// �}�b�v�f�[�^

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 1; nCnt < NUM_TITLE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[nCnt]);

		//�^�C�g���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
							   nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
							   2);						//�`�悷��v���~�e�B�u��
	}
}