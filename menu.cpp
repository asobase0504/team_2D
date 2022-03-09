//==================================================
// 
// FPG���� ( menu.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include "main.h"
#include "input.h"
#include "menu.h"
#include "fade.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_MENU					(16)		// ���j���[�̍ő吔
#define NORMAL_BLINK_SPEED			(0.01f)		// �ʏ펞�̓_�ő��x
#define DECISION_BLINK_SPEED		(0.1f)		// ���莞�̓_�ő��x
#define MIN_ALPHA					(0.6f)		// ���l�̍ŏ��l

//--------------------------------------------------
// �\����
//--------------------------------------------------

/*�� �I���� ��*/

typedef struct
{
	D3DXVECTOR3				pos;			// �ʒu
	D3DXCOLOR				col;			// �F
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
	float					fWidth;			// ��
	float					fHeight;		// ����
}Option;

/*�� ���j���[ ��*/

typedef struct
{
	D3DXVECTOR3				pos;					// �ʒu
	LPDIRECT3DTEXTURE9		pTexture;				// �e�N�X�`��
	Option					Option[MAX_OPTION];		// �I�����̏��
	int						nNumUse;				// �g�p��
	float					fWidth;					// ��
	float					fHeight;				// ����
	float					fInterval;				// �I�����̊Ԋu
	float					fBlinkSpeed;			// �_�ő��x
	bool					bFrame;					// �g�����邩�ǂ��� [ true : ���� false : ����Ȃ� ]
	bool					bUse;					// �g�p���Ă��邩�ǂ���
}Menu;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMenu = NULL;			// ���j���[�̒��_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffOption = NULL;		// �I�����̒��_�o�b�t�@�ւ̃|�C���^
static Menu							s_aMenu[MAX_MENU];				// ���j���[�̏��
static int							s_nIdxMenu;						// �I�΂�Ă��郁�j���[�̔ԍ�
static int							s_nIdxOption;					// �I�΂�Ă���I�����̔ԍ�
static int							s_nAlphaTime;					// ���l�ύX�p�̎���

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void ChangeColor(Menu *pMenu);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMenu(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;

	// �������̃N���A
	memset(s_aMenu, 0, sizeof(s_aMenu));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMenu,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_MENU * MAX_OPTION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffOption,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

								// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_MENU; i++)
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
	s_pVtxBuffMenu->Unlock();

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_MENU; i++)
	{
		for (int j = 0; j < MAX_OPTION; j++)
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
	s_pVtxBuffOption->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMenu(void)
{
	if (s_pVtxBuffMenu != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuffMenu->Release();
		s_pVtxBuffMenu = NULL;
	}

	if (s_pVtxBuffOption != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuffOption->Release();
		s_pVtxBuffOption = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMenu(void)
{
	Menu *pMenu = &s_aMenu[s_nIdxMenu];

	if (!pMenu->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	// �F�̕ύX
	ChangeColor(pMenu);
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMenu(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (!pMenu->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

		if (pMenu->bFrame)
		{// �g������
		 // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, s_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			if (pMenu->pTexture != NULL)
			{// �e�N�X�`��������
				pDevice->SetTexture(0, pMenu->pTexture);
			}
			else
			{// �e�N�X�`�����Ȃ�
				pDevice->SetTexture(0, NULL);
			}

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				i * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);							// �v���~�e�B�u(�|���S��)��
		}

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffOption, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pMenu->Option[j].pTexture);

			int nIdx = (i * MAX_MENU) + (j * 4);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				nIdx,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);							// �v���~�e�B�u(�|���S��)��
		}
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame)
{
	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	int nIdx = 0;
	Menu *pMenu = NULL;

	for (nIdx = 0; nIdx < MAX_MENU; nIdx++)
	{
		pMenu = &s_aMenu[nIdx];

		if (pMenu->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);
		float fPosY = menu.fTop + ((menu.fBottom - menu.fTop) * 0.5f);

		pMenu->pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);
		pMenu->nNumUse = menu.nNumUse;

		pMenu->fWidth = menu.fRight - menu.fLeft;
		pMenu->fHeight = menu.fBottom - menu.fTop;

		if (menu.bSort)
		{// �c
			pMenu->fInterval = pMenu->fHeight / (menu.nNumUse + 1);
		}
		else
		{// ��
			pMenu->fInterval = pMenu->fWidth / (menu.nNumUse + 1);
		}

		pMenu->fBlinkSpeed = NORMAL_BLINK_SPEED;

		pMenu->bFrame = Frame.bUse;

		pMenu->bUse = true;

		s_nIdxMenu = nIdx;
		s_nIdxOption = 0;

		if (Frame.pTexture != NULL)
		{// �e�N�X�`��������
			pMenu->pTexture = *Frame.pTexture;
		}
		else
		{// �e�N�X�`�����Ȃ�
			pMenu->pTexture = NULL;
		}

		if (Frame.bUse)
		{// �g������
		 // ���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nIdx * 4;		// �w��̈ʒu�܂Ń|�C���^��i�߂�

			float fWidth = (menu.fRight - menu.fLeft) * 0.5f;
			float fHeight = (menu.fBottom - menu.fTop) * 0.5f;
			D3DXVECTOR3 pos = D3DXVECTOR3(menu.fLeft + fWidth, menu.fTop + fHeight, 0.0f);

			// ���_���W�̐ݒ�
			pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = Frame.col;
			pVtx[1].col = Frame.col;
			pVtx[2].col = Frame.col;
			pVtx[3].col = Frame.col;

			// ���_�o�b�t�@���A�����b�N����
			s_pVtxBuffMenu->Unlock();
		}

		break;
	}

	for (int j = 0; j < pMenu->nNumUse; j++)
	{
		Option *pOption = &pMenu->Option[j];

		if (menu.bSort)
		{// �c
			pOption->pos = D3DXVECTOR3(pMenu->pos.x, menu.fTop + (pMenu->fInterval * (j + 1)), 0.0f);
		}
		else
		{// ��
			pOption->pos = D3DXVECTOR3(menu.fLeft + (pMenu->fInterval * (j + 1)), pMenu->pos.y, 0.0f);
		}

		pOption->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pOption->fWidth = menu.fWidth;
		pOption->fHeight = menu.fHeight;
		pOption->pTexture = *menu.pTexture[j];

		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdx * MAX_MENU) + (j * 4);		// �w��̈ʒu�܂Ń|�C���^��i�߂�

		float fWidth = menu.fWidth * 0.5f;
		float fHeight = menu.fHeight * 0.5f;

		// ���_���W�̐ݒ�
		pVtx[0].pos = pOption->pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
		pVtx[1].pos = pOption->pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
		pVtx[2].pos = pOption->pos + D3DXVECTOR3(-fWidth, fHeight, 0.0f);
		pVtx[3].pos = pOption->pos + D3DXVECTOR3(fWidth, fHeight, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pOption->col;
		pVtx[1].col = pOption->col;
		pVtx[2].col = pOption->col;
		pVtx[3].col = pOption->col;

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffOption->Unlock();
	}

	return nIdx;
}

//--------------------------------------------------
// �I�����̐F�̏�����
//--------------------------------------------------
void InitColorOption(void)
{
	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	// �w��̈ʒu�܂Ń|�C���^��i�߂�
	pVtx += (s_nIdxMenu * MAX_MENU) + (s_nIdxOption * 4);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffOption->Unlock();
}

//--------------------------------------------------
// �I�����̕ύX
//--------------------------------------------------
void ChangeOption(int Idx)
{
	s_nIdxOption = Idx;
}

//--------------------------------------------------
// �I�����̌���
//--------------------------------------------------
void DecisionOption(void)
{
	s_aMenu[s_nIdxMenu].fBlinkSpeed = DECISION_BLINK_SPEED;
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
void ResetMenu(int nIdx)
{
	s_nIdxMenu = 0;
	s_nIdxOption = 0;
	s_nAlphaTime = 0;
	s_aMenu[nIdx].bUse = false;
}

//--------------------------------------------------
// �F�̕ύX
//--------------------------------------------------
static void ChangeColor(Menu *pMenu)
{
	s_nAlphaTime++;

	Option *pOption = &pMenu->Option[s_nIdxOption];

	float fCurve = cosf((s_nAlphaTime * pMenu->fBlinkSpeed) * (D3DX_PI * 2.0f));
	float fAlpha = (fCurve * (1.0f - MIN_ALPHA)) + MIN_ALPHA;

	pOption->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

								// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_nIdxMenu * MAX_MENU) + (s_nIdxOption * 4);		// �w��̈ʒu�܂Ń|�C���^��i�߂�

	// ���_�J���[�̐ݒ�
	pVtx[0].col = pOption->col;
	pVtx[1].col = pOption->col;
	pVtx[2].col = pOption->col;
	pVtx[3].col = pOption->col;

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffOption->Unlock();
}
