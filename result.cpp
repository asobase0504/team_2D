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
#define NUM_RESULT	(6)					//�e�N�X�`���̍ő吔
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

	//------------------------------
	//	�e�N�X�`���̓ǂݍ���
	//------------------------------
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Result_BG.png",
							  &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\ScoreLine.png",
							  &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\BestScoreLine.png",
							  &s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\RESULT.png",
							  &s_pTexture[3]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\player000.png",
							  &s_pTexture[4]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\ENEMY\\sky_enemy_001.png",
							  &s_pTexture[5]);

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
	//���U���g�̔w�i
	s_Result[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_Result[0].fWidth = SCREEN_WIDTH / 2;
	s_Result[0].fHeight = SCREEN_HEIGHT / 2;

	//�X�R�A�̐�
	s_Result[1].pos = D3DXVECTOR3(500.0f, 280.0f, 0.0f);
	s_Result[1].fWidth = 350.0f;
	s_Result[1].fHeight = 50.0f;

	//�x�X�g�X�R�A�̐�
	s_Result[2].pos = D3DXVECTOR3(500.0f, 450.0f, 0.0f);
	s_Result[2].fWidth = 350.0f;
	s_Result[2].fHeight = 50.0f;

	//�v���C���[�̃e�N�X�`��
	s_Result[3].pos = D3DXVECTOR3(300.0f, 100.0f, 0.0f);
	s_Result[3].fWidth = 250.0f;
	s_Result[3].fHeight = 50.0f;

	//�G�̃e�N�X�`��
	s_Result[4].pos = D3DXVECTOR3(1050.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
	s_Result[4].fWidth = 80.0f;
	s_Result[4].fHeight = 80.0f;

	s_Result[5].pos = D3DXVECTOR3(100.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
	s_Result[5].fWidth = 80.0f;
	s_Result[5].fHeight = 80.0f;

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
		if (nCnt == 0)
		{//�w�i�̃e�N�X�`���Ȃ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		}
		else
		{//����ȊO�̃e�N�X�`��
			pVtx[0].col = result->col;
			pVtx[1].col = result->col;
			pVtx[2].col = result->col;
			pVtx[3].col = result->col;
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//�X�R�A�̏���������
	InitScore();

	SetScorePos(D3DXVECTOR3(520.0f, 240.0f, 0.0f));
	SetBestScorePos(D3DXVECTOR3(520.0f, 410.0f, 0.0f));
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

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nTime++;				//�^�C���̉��Z
	s_nTime %= BLINK_TIME;	//�^�C���̏�����

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		RESULT *result = s_Result + nCnt;

		if (s_nTime >= BLINK_HALF)
		{
			s_Result[4].pos.y += 0.6f;
			s_Result[5].pos.x -= 0.8f;
		}
		else
		{
			s_Result[4].pos.y -= 0.6f;
			s_Result[5].pos.x += 0.8f;
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
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A, 0))
	{
		ChangeMode(MODE_RANKING);
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