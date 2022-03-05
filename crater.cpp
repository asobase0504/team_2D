//==================================================
// 
// 3D�Q�[������ ( crater.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "crater.h"
#include "map.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_CRATER		(256)		// �N���[�^�[�̍ő吔
#define CRATER_SIZE		(30.0f)		// �N���[�^�[�̃T�C�Y

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	bool			bUse;		// �g�p���Ă��邩�ǂ���
}Crater;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
static Crater						s_Crater[MAX_CRATER];		// �N���[�^�[�̏��

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitCrater(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/crater.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_CRATER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//�������̃N���A
	memset(s_Crater, 0, sizeof(s_Crater));

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_CRATER; i++)
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
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitCrater(void)
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
void UpdateCrater(void)
{
	for (int i = 0; i < MAX_CRATER; i++)
	{
		if (!s_Crater[i].bUse)
		{// �g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

		s_Crater[i].pos.y += MAP_SPEAD;		// Map���瑬�x��Get���ė~�����ł��B��낵���I

		if (s_Crater[i].pos.y >= (SCREEN_HEIGHT + CRATER_SIZE))
		{// ��ʊO�ɏo��
			s_Crater[i].bUse = false;
		}

		VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// �w��̈ʒu�܂Ń|�C���^��i�߂�

		// ���_���W�̐ݒ�
		pVtx[0].pos = s_Crater[i].pos + D3DXVECTOR3(-CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[1].pos = s_Crater[i].pos + D3DXVECTOR3( CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[2].pos = s_Crater[i].pos + D3DXVECTOR3(-CRATER_SIZE,  CRATER_SIZE, 0.0f);
		pVtx[3].pos = s_Crater[i].pos + D3DXVECTOR3( CRATER_SIZE,  CRATER_SIZE, 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawCrater(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_CRATER; i++)
	{
		if (!s_Crater[i].bUse)
		{// �g�p����Ă��Ȃ�
			continue;
		}

		/*�� �g�p����Ă��� ��*/

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
void SetCrater(D3DXVECTOR3 pos)
{
	for (int i = 0; i < MAX_CRATER; i++)
	{
		if (s_Crater[i].bUse)
		{// �g�p����Ă���
			continue;
		}

		/*�� �g�p����Ă��Ȃ� ��*/

		s_Crater[i].pos = pos;
		s_Crater[i].bUse = true;

		VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// �w��̈ʒu�܂Ń|�C���^��i�߂�

		// ���_���W�̐ݒ�
		pVtx[0].pos = pos + D3DXVECTOR3(-CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3( CRATER_SIZE, -CRATER_SIZE, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-CRATER_SIZE,  CRATER_SIZE, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3( CRATER_SIZE,  CRATER_SIZE, 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}