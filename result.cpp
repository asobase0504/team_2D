//================================
//
// ���U���g���
// Author:Teruto Sato
//
//================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include <time.h>

//�}�N����`
#define NUM_RESULT	(4)					//�e�N�X�`���̍ő吔
#define BLINK_TIME	(300)				//�_�łɂ����鎞��
#define BLINK_HALF	(BLINK_TIME / 2)	//�_�ł��؂�ւ�鎞��
#define MAP_SPEED	(0.0001f)			//��ʃX�N���[���̑���

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9			s_pTexture[NUM_RESULT] = {};	//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^;
static RESULT s_Result[NUM_RESULT];	//���U���g�̏��̎擾
static int s_nTime;			//�_�ł̎���
static int s_nMapTime;

//============================
// ���U���g�̏���������
//============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	//SCORE *pScore = GetScore();	//�X�R�A�̎擾

	//------------------------------
	//	�e�N�X�`���̓ǂݍ���
	//------------------------------
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Enemy\\sky001.jpg",
							  &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Congratulation.png",
							  &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\flag.png",
							  &s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
							  "",
							  &s_pTexture[3]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RESULT,	//�m�ۂ���o�b�t�@�̃T�C�Y
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//------------------------------
	//	�\���̂̏�����
	//------------------------------
	//�ʒu�̏�����
	s_Result[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_Result[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 20.0f, 200.0f, 0.0f);
	s_Result[2].pos = D3DXVECTOR3(750.0f, 450.0f, 0.0f);
	s_Result[3].pos = D3DXVECTOR3(350.0f, 450.0f, 0.0f);

	//���̏�����
	s_Result[0].fWidth = SCREEN_WIDTH / 2;
	s_Result[1].fWidth = 500.0f;
	s_Result[2].fWidth = 300.0f;
	s_Result[3].fWidth = 100.0f;

	//�����̏�����
	s_Result[0].fHeight = SCREEN_HEIGHT / 2;
	s_Result[1].fHeight = 100.0f;
	s_Result[2].fHeight = 50.0f;
	s_Result[3].fHeight = 100.0f;

	//�F�̏�����
	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		s_Result[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------
	//	���_���̐ݒ�
	//------------------------------
	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		RESULT *result = s_Result + nCnt;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y + result->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y + result->fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = result->col;
		pVtx[1].col = result->col;
		pVtx[2].col = result->col;
		pVtx[3].col = result->col;

		//�e�N�X�`�����W�̐ݒ�
		if (nCnt == 0)
		{//�w�i�̃e�N�X�`���Ȃ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
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

	//�X�R�A�̏���������
	InitScore();

	//�X�R�A�̐ݒ菈��
	//SetScore(pScore->nScore);
}

//============================
// ���U���g�̏I������
//============================
void UninitResult(void)
{
	//�X�R�A�̏I������
	UninitScore();

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
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
// ���U���g�̍X�V����
//============================
void UpdateResult(void)
{
	VERTEX_2D*pVtx;				//���_���ւ̃|�C���^
	//SCORE *pScore = GetScore();	//�X�R�A�̎擾

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nTime++;				//�^�C���̉��Z
	s_nTime %= BLINK_TIME;	//�^�C���̏�����

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		RESULT *result = s_Result + nCnt;

		//------------------------
		//	�e�N�X�`���̏㉺
		//------------------------
		if (s_nTime >= BLINK_HALF)
		{//�^�C����150�ȏ�Ȃ�
			s_Result[2].pos.y -= 0.12f;	//�グ��
			s_Result[3].pos.y -= 0.12f;
		}
		else
		{//�^�C����150�����Ȃ�
			s_Result[2].pos.y += 0.12f;	//������
			s_Result[3].pos.y += 0.12f;
		}

		//------------------------
		//	�w�i�̃X�N���[��
		//------------------------
		if (nCnt == 0)
		{//�w�i�̃e�N�X�`���Ȃ�
		 //�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
			pVtx[1].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
			pVtx[2].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
			pVtx[3].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y + result->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y + result->fHeight, 0.0f);

		pVtx += 4;
	}

	//----------------
	//	��ʑJ��
	//----------------
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A,0))
	{
		SetFade(MODE_TITLE);
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//�X�R�A�̍X�V����
	UpdateScore();
}

//============================
// ���U���g�̕`�揈��
//============================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[nCnt]);

		//���U���g�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);						//�`�悷��v���~�e�B�u��
	}

	//�X�R�A�̕`�揈��
	DrawScore();
}