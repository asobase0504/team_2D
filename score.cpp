//============================================================================================================
//
//�`�[������[score.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "score.h"

#include <stdio.h>

//============================================================================================================
//�}�N����`
//============================================================================================================
#define MAX_SCORE			(8)			//�X�R�A�̍ő包��
#define NOWSCORE_POS_X		(0.0f)		//���݂̃X�R�A�̈ʒu(X)
#define NOWSCORE_POS_Y		(10.0f)		//���݂̃X�R�A�̈ʒu(Y)
#define BESTSCORE_POS_X		(245.0f)	//�x�X�g�X�R�A�̈ʒu(X)
#define BESTSCORE_POS_Y		(10.0f)		//�x�X�g�X�R�A�̈ʒu(Y)
#define SCORE_WIDTH			(30)		//�X�R�A�̕�
#define SCORE_HEIGHT		(45)		//�X�R�A�̍���
#define BESTSCORE_FILE		("data/txt/BestScore.txt")	// �x�X�g�X�R�A�̊Ǘ�����t�@�C��

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  s_pVtxBuffNowScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  s_pVtxBuffBestScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static D3DXVECTOR3				s_posNowScore;					//���݂̃X�R�A�̈ʒu
static D3DXVECTOR3				s_posBestScore;					//�x�X�g�X�R�A�̈ʒu
static int						s_nNowScore;					//���݂̃X�R�A�̒l
static int						s_nBestScore;					//�x�X�g�X�R�A�̒l

//============================================================================================================
//�v���g�^�C�v�錾
//============================================================================================================
static void SetNowScore(void);
static void SetBestScore(void);

//============================================================================================================
//�X�R�A�̏���������
//============================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/number000.png",
								&s_pTexture);

	//�ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		s_posNowScore	= D3DXVECTOR3(NOWSCORE_POS_X, NOWSCORE_POS_Y, 0.0f);	//���݂̃X�R�A�̈ʒu
		s_posBestScore	= D3DXVECTOR3(BESTSCORE_POS_X, BESTSCORE_POS_Y, 0.0f);	//�x�X�g�X�R�A�̈ʒu
		s_nNowScore		= 0;													//�X�R�A�̒l
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffNowScore,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffBestScore,
								NULL);

	/********************* ���݂̃X�R�A�̕� *********************/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//���_���W�̐ݒ�
		float fLeft		= (s_posNowScore.x + (nCnt * SCORE_WIDTH));
		float fRight	= (s_posNowScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH);
		float fUp		= (s_posNowScore.y);
		float fDown		= (s_posNowScore.y + SCORE_HEIGHT);

		pVtx[0].pos = D3DXVECTOR3(fLeft,	fUp,	0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight,	fUp,	0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,	fDown,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight,	fDown,	0.0f);

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

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffNowScore->Unlock();

	/********************* �x�X�g�X�R�A�̕� *********************/

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBestScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//���_���W�̐ݒ�
		float fLeft		= (s_posBestScore.x + (nCnt * SCORE_WIDTH));
		float fRight	= (s_posBestScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH);
		float fUp		= (s_posBestScore.y);
		float fDown		= (s_posBestScore.y + SCORE_HEIGHT);

		pVtx[0].pos = D3DXVECTOR3(fLeft,	fUp,	0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight,	fUp,	0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,	fDown,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight,	fDown,	0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBestScore->Unlock();

	//���݂̃X�R�A�ƃx�X�g�X�R�A���Z�b�g
	SetNowScore();
	SetBestScore();
}

//============================================================================================================
//�X�R�A�̏I������
//============================================================================================================
void UninitScore(void)
{
	///�ۑ�����^�C�~���O�͂����R�ɂǂ���
	//�x�X�g�X�R�A�̕ۑ�
	SaveBestScore();

	/*********** �e�N�X�`���̔j�� ***********/
	
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	/*********** ���_�o�b�t�@�̔j�� ***********/
	
	if (s_pVtxBuffNowScore != NULL)
	{//���݂̃X�R�A
		s_pVtxBuffNowScore->Release();
		s_pVtxBuffNowScore = NULL;
	}

	if (s_pVtxBuffBestScore != NULL)
	{//�x�X�g�X�R�A
		s_pVtxBuffBestScore->Release();
		s_pVtxBuffBestScore = NULL;
	}
}

//============================================================================================================
//�X�R�A�̍X�V����
//============================================================================================================
void UpdateScore(void)
{
}

//============================================================================================================
//�X�R�A�̕`�揈��
//============================================================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	/********************* ���݂̃X�R�A�̕� *********************/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffNowScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		//�X�R�A�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
								2 * MAX_SCORE);			//�`�悷��v���~�e�B�u��
	}

	/********************* �x�X�g�X�R�A�̕� *********************/

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBestScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		//�X�R�A�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
								nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
								2 * MAX_SCORE);			//�`�悷��v���~�e�B�u��
	}
}

//============================================================================================================
//���݂̃X�R�A�̐ݒ菈��
//============================================================================================================
void SetNowScore(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);

	//���݂̃X�R�A��0�ɐݒ肷��
	s_nNowScore = 0;

	//�X�R�A�̒l�ɍ��킹�ăe�N�X�`����ύX
	aPosTexU[0] = s_nNowScore % 100000000 / 10000000;
	aPosTexU[1] = s_nNowScore % 10000000 / 1000000;
	aPosTexU[2] = s_nNowScore % 1000000 / 100000;
	aPosTexU[3] = s_nNowScore % 100000 / 10000;
	aPosTexU[4] = s_nNowScore % 10000 / 1000;
	aPosTexU[5] = s_nNowScore % 1000 / 100;
	aPosTexU[6] = s_nNowScore % 100 / 10;
	aPosTexU[7] = s_nNowScore % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffNowScore->Unlock();
}

//============================================================================================================
//�x�X�g�X�R�A�̐ݒ菈��
//============================================================================================================
void SetBestScore(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBestScore->Lock(0, 0, (void**)&pVtx, 0);

	//�ǂݍ��񂾃x�X�g�X�R�A��ݒ肷��
	s_nBestScore = LoadBestScore();

	//�X�R�A�̒l�ɍ��킹�ăe�N�X�`����ύX
	aPosTexU[0] = s_nBestScore % 100000000 / 10000000;
	aPosTexU[1] = s_nBestScore % 10000000 / 1000000;
	aPosTexU[2] = s_nBestScore % 1000000 / 100000;
	aPosTexU[3] = s_nBestScore % 100000 / 10000;
	aPosTexU[4] = s_nBestScore % 10000 / 1000;
	aPosTexU[5] = s_nBestScore % 1000 / 100;
	aPosTexU[6] = s_nBestScore % 100 / 10;
	aPosTexU[7] = s_nBestScore % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBestScore->Unlock();
}

//============================================================================================================
//�X�R�A�̉��Z����
//============================================================================================================
void AddScore(int nValue)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);

	//�X�R�A�̒l�ɔC�ӂ̒l�����Z����
	s_nNowScore += nValue;

	//�X�R�A�̒l�ɍ��킹�ăe�N�X�`����ύX
	aPosTexU[0]	= s_nNowScore % 100000000 / 10000000;
	aPosTexU[1] = s_nNowScore % 10000000 / 1000000;
	aPosTexU[2] = s_nNowScore % 1000000 / 100000;
	aPosTexU[3] = s_nNowScore % 100000 / 10000;
	aPosTexU[4] = s_nNowScore % 10000 / 1000;
	aPosTexU[5] = s_nNowScore % 1000 / 100;
	aPosTexU[6] = s_nNowScore % 100 / 10;
	aPosTexU[7] = s_nNowScore % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffNowScore->Unlock();

	if (s_nNowScore >= s_nBestScore)
	{//���݂̃X�R�A���x�X�g�X�R�A���X�V�����Ƃ�
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffBestScore->Lock(0, 0, (void**)&pVtx, 0);

		//�uBest�v�ƁuNow�v��A�����ĉ��Z����
		s_nBestScore = s_nNowScore;

		//�X�R�A�̒l�ɍ��킹�ăe�N�X�`����ύX
		aPosTexU[0] = s_nBestScore % 100000000 / 10000000;
		aPosTexU[1] = s_nBestScore % 10000000 / 1000000;
		aPosTexU[2] = s_nBestScore % 1000000 / 100000;
		aPosTexU[3] = s_nBestScore % 100000 / 10000;
		aPosTexU[4] = s_nBestScore % 10000 / 1000;
		aPosTexU[5] = s_nBestScore % 1000 / 100;
		aPosTexU[6] = s_nBestScore % 100 / 10;
		aPosTexU[7] = s_nBestScore % 10 / 1;

		//�e�N�X�`�����W�̐ݒ�
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

			pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
		}

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffBestScore->Unlock();
	}
}

//============================================================================================================
//�X�R�A�̌��Z����
//============================================================================================================
void SubScore(int nValue)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int aPosTexU[MAX_SCORE];	//�e���̐������i�[

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);
	
	//�X�R�A�̒l����C�ӂ̒l�����Z����
	s_nNowScore -= nValue;

	if (s_nNowScore <= 0)
	{//0�ȉ���������
		s_nNowScore = 0;
	}

	//�X�R�A�̒l�ɍ��킹�ăe�N�X�`����ύX
	aPosTexU[0] = s_nNowScore % 100000000 / 10000000;
	aPosTexU[1] = s_nNowScore % 10000000 / 1000000;
	aPosTexU[2] = s_nNowScore % 1000000 / 100000;
	aPosTexU[3] = s_nNowScore % 100000 / 10000;
	aPosTexU[4] = s_nNowScore % 10000 / 1000;
	aPosTexU[5] = s_nNowScore % 1000 / 100;
	aPosTexU[6] = s_nNowScore % 100 / 10;
	aPosTexU[7] = s_nNowScore % 10 / 1;

	//�e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffNowScore->Unlock();
}

//============================================================================================================
//�x�X�g�X�R�A�̕ۑ�
//============================================================================================================
void SaveBestScore(void)
{
	FILE *pFile = fopen(BESTSCORE_FILE, "w");		//�t�@�C�����J��

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		//�t�@�C���Ƀx�X�g�X�R�A���Z�[�u����
		fprintf(pFile, "%d\n", s_nBestScore);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf(" *** �t�@�C�����J���Ȃ��c���Ɓc *** \n");
	}
}

//============================================================================================================
//�x�X�g�X�R�A�̓ǂݍ���
//============================================================================================================
int LoadBestScore(void)
{
	FILE *pFile = fopen(BESTSCORE_FILE, "r");		//�t�@�C�����J��
	int nBestScore;									//�x�X�g�X�R�A�ǂݍ��ޗp

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		//�t�@�C������X�R�A�����[�h����
		fscanf(pFile, "%d\n", &nBestScore);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf(" *** �t�@�C�����J���Ȃ��c���Ɓc *** \n");
	}

	return nBestScore;		//�ǂݍ��񂾃x�X�g�X�R�A��Ԃ�
}

//============================================================================================================
//�X�R�A�̎擾
//============================================================================================================
int GetScore(void)
{
	return s_nNowScore;
}