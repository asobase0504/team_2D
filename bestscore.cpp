//==================================================
// 
// 3D�Q�[������ ( bestscore.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "bestscore.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define BESTSCORE_WIDTH 		(240.0f)		// �x�X�g�X�R�A�̕�
#define BESTSCORE_HEIGHT		(30.0f)			// �x�X�g�X�R�A�̍���
#define START_POS_X				(245.0f)		// �n�܂��X�̈ʒu
#define START_POS_Y				(80.0f)			// �n�܂��Y�̈ʒu
#define MAX_ALPHA_TIME			(120)			// ���l�ύX�p�̎��Ԃ̍ő�l
#define BLINK_SPEED				(0.1f)			// �_�ő��x
#define MIN_ALPHA				(0.6f)			// ���l�̍ŏ��l

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static bool							s_bUpdate;				// �x�X�g�X�R�A���X�V���ꂽ���ǂ���
static int							s_nAlphaTime;			// ���l�ύX�p�̎���

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitBestScore(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_bUpdate = false;
	s_nAlphaTime = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/BEST_SCORE.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos + D3DXVECTOR3(           0.0f, -BESTSCORE_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(BESTSCORE_WIDTH, -BESTSCORE_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(           0.0f,  BESTSCORE_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(BESTSCORE_WIDTH,  BESTSCORE_HEIGHT * 0.5f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitBestScore(void)
{
	if (s_pTexture != NULL)
	{// �e�N�X�`���̉��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateBestScore(void)
{
	if (s_bUpdate)
	{// �x�X�g�X�R�A���X�V���ꂽ
		if (s_nAlphaTime <= MAX_ALPHA_TIME)
		{// �w��̒l�ȉ�
			s_nAlphaTime++;

			VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

			// ���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			float fAlpha = (sinf((s_nAlphaTime * BLINK_SPEED) * (D3DX_PI * 2.0f)) * (1.0f - MIN_ALPHA)) + MIN_ALPHA;

			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, fAlpha);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			// ���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();
		}
		else
		{
			VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

			// ���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			// ���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();
		}
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawBestScore(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);							// �v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
// �_��
//--------------------------------------------------
void BlinkBestScore(void)
{
	s_bUpdate = true;
}