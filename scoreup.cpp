//==================================================
// 
// 3D�Q�[������ ( scoreup.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "scoreup.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_SCORE			(64)			// �X�R�A�̍ő吔
#define MAX_DIGITS			(8)				// ���̍ő吔��
#define MAX_MOVE			(-1.0f)			// �ړ��ʂ̍ő�l
#define INTERVAL_WIDTH		(30.0f)			// ���̊Ԋu�̕�
#define SCORE_WIDTH			(25.0f)			// �X�R�A�̕�
#define SCORE_HEIGHT		(35.0f)			// �X�R�A�̍���
#define OPERATOR_WIDTH		(20.0f)			// ���Z�q�̕�
#define OPERATOR_HEIGHT		(25.0f)			// ���Z�q�̍���
#define START_POS_X			(240.0f)		// X�̏����ʒu
#define START_POS_Y			(77.5f)			// Y�̏����ʒu

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXCOLOR		col;			// �F
	int				nDigits;		// ����
	int				nAlphaTime;		// ���l�ύX�p�̎���
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}ScoreUp;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------

/*�� �X�R�A�֘A ��*/

static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
static ScoreUp						s_ScoreUp[MAX_SCORE];		// �X�R�A�̏��

/*�� ���Z�q�֘A ��*/

static LPDIRECT3DTEXTURE9			s_pTexturePlus = NULL;			// �v���X�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMinus = NULL;			// �}�C�i�X�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffOperator = NULL;		// ���Z�q�̒��_�o�b�t�@�ւ̃|�C���^
static bool							s_bOperator[MAX_SCORE];			// ���Z�q���ǂ��炩 [ true  : �v���X  false  : �}�C�i�X ]

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitScoreUp(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Number_0To9.png",
		&s_pTexture);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/plus.png",
		&s_pTexturePlus);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/minus.png",
		&s_pTextureMinus);

	// �������̃N���A
	memset(s_ScoreUp, 0, sizeof(s_ScoreUp));

	// �������̃N���A
	memset(s_bOperator, 0, sizeof(s_bOperator));

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	/*�� �X�R�A�֘A ��*/

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_DIGITS * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		for (int j = 0; j < MAX_DIGITS; j++)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
			
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			
			pVtx += 4;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	/*�� ���Z�q�֘A ��*/

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffOperator,
		NULL);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffOperator->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffOperator->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitScoreUp(void)
{
	/*�� �X�R�A�֘A ��*/

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

	/*�� ���Z�q�֘A ��*/

	if (s_pTexturePlus != NULL)
	{// �e�N�X�`���̉��
		s_pTexturePlus->Release();
		s_pTexturePlus = NULL;
	}

	if (s_pTextureMinus != NULL)
	{// �e�N�X�`���̉��
		s_pTextureMinus->Release();
		s_pTextureMinus = NULL;
	}

	if (s_pVtxBuffOperator != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuffOperator->Release();
		s_pVtxBuffOperator = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateScoreUp(void)
{
	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (!pScoreUp->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		pScoreUp->pos.y += MAX_MOVE;
		pScoreUp->col.a = cosf((pScoreUp->nAlphaTime * 0.01f) * (D3DX_PI * 2.0f));
		pScoreUp->nAlphaTime++;

		if (pScoreUp->col.a <= 0.0f)
		{// �w��̒l�ȉ��ɂȂ���
			pScoreUp->bUse = false;
		}

		VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

		/*�� �X�R�A�֘A ��*/

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += MAX_DIGITS * i * 4;		// �w��̈ʒu�܂Ń|�C���^��i�߂�

		for (int j = 0; j < pScoreUp->nDigits; j++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(pScoreUp->pos.x + (-INTERVAL_WIDTH * j) + (-INTERVAL_WIDTH * 0.5f), pScoreUp->pos.y, 0.0f);

			// ���_���W�̐ݒ�
			pVtx[0].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pScoreUp->col;
			pVtx[1].col = pScoreUp->col;
			pVtx[2].col = pScoreUp->col;
			pVtx[3].col = pScoreUp->col;

			pVtx += 4;		// �|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		/*�� ���Z�q�֘A ��*/

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffOperator->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// �w��̈ʒu�܂Ń|�C���^��i�߂�

		D3DXVECTOR3 pos = pScoreUp->pos;
		pos.x += (-INTERVAL_WIDTH * pScoreUp->nDigits) + (-INTERVAL_WIDTH * 0.5f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pScoreUp->col;
		pVtx[1].col = pScoreUp->col;
		pVtx[2].col = pScoreUp->col;
		pVtx[3].col = pScoreUp->col;

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffOperator->Unlock();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawScoreUp(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*�� �X�R�A�֘A ��*/

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (!pScoreUp->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		for (int j = 0; j < pScoreUp->nDigits; j++)
		{
			int nIdx = (MAX_DIGITS * i * 4) + (j * 4);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				nIdx,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);							// �v���~�e�B�u(�|���S��)��
		}
	}

	/*�� ���Z�q�֘A ��*/

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffOperator, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (!pScoreUp->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		// �e�N�X�`���̐ݒ�
		if (s_bOperator[i])
		{// �v���X
			pDevice->SetTexture(0, s_pTexturePlus);
		}
		else
		{// �}�C�i�X
			pDevice->SetTexture(0, s_pTextureMinus);
		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);							// �v���~�e�B�u(�|���S��)��
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetScoreUp(int nScore)
{
	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (pScoreUp->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		pScoreUp->pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pScoreUp->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pScoreUp->nDigits = 0;
		pScoreUp->nAlphaTime = 0;
		pScoreUp->bUse = true;

		s_bOperator[i] = true;

		if (nScore < 0)
		{// �}�C�i�X
			nScore *= -1;
			s_bOperator[i] = false;
		}

		int nNumber = nScore;

		while (1)
		{// �������[�v
			if (nNumber >= 10)
			{// 2���ȏ�
				nNumber /= 10;
				pScoreUp->nDigits++;
			}
			else
			{// 1��
				pScoreUp->nDigits++;
				break;
			}
		}

		VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

		/*�� �X�R�A�֘A ��*/

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += MAX_DIGITS * i * 4;		// �w��̈ʒu�܂Ń|�C���^��i�߂�

		int aNumber[MAX_DIGITS];

		for (int j = 0; j < pScoreUp->nDigits; j++)
		{
			aNumber[j] = nScore % 10;
			nScore /= 10;

			D3DXVECTOR3 pos = pScoreUp->pos;
			pos.x += (-INTERVAL_WIDTH * j) + (-INTERVAL_WIDTH * 0.5f);

			// ���_���W�̐ݒ�
			pVtx[0].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pScoreUp->col;
			pVtx[1].col = pScoreUp->col;
			pVtx[2].col = pScoreUp->col;
			pVtx[3].col = pScoreUp->col;

			float fLeft = 0.1f * aNumber[j];
			float fRight = (0.1f * aNumber[j]) + 0.1f;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(fLeft, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fRight, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(fLeft, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(fRight, 1.0f);

			pVtx += 4;		// �|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		/*�� ���Z�q�֘A ��*/

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffOperator->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// �w��̈ʒu�܂Ń|�C���^��i�߂�

		D3DXVECTOR3 pos = pScoreUp->pos;
		pos.x += (-INTERVAL_WIDTH * pScoreUp->nDigits) + (-INTERVAL_WIDTH * 0.5f);

		// ���_���W�̐ݒ�
		pVtx[0].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pScoreUp->col;
		pVtx[1].col = pScoreUp->col;
		pVtx[2].col = pScoreUp->col;
		pVtx[3].col = pScoreUp->col;

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffOperator->Unlock();

		break;
	}
}