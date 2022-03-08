//==================================================
// 
// 3D�Q�[������ ( menu.cpp )
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
	float					fWidth;			// ��
	float					fHeight;		// ����
	LPDIRECT3DTEXTURE9		pTexture;		// �e�N�X�`��
}Option;

/*�� ���j���[ ��*/

typedef struct
{
	D3DXVECTOR3		pos;					// �ʒu
	Option			Option[MAX_OPTION];		// �I�����̏��
	int				nNumUse;				// �g�p��
	float			fWidth;					// ��
	float			fHeight;				// ����
	float			fInterval;				// �I�����̊Ԋu
	float			fBlinkSpeed;			// �_�ő��x
	bool			bUse;					// �g�p���Ă��邩�ǂ���
}Menu;

//--------------------------------------------------
// �X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
static Menu							s_aMenu[MAX_MENU];				// ���j���[�̏��
static int							s_nSelectMenu;					// �I�΂�Ă��郁�j���[
static int							s_nSelectOption;				// �I�΂�Ă���I����
static int							s_nAlphaTime;					// ���l�ύX�p�̎���

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void SetColor(const Option &option);
static void Input(Menu *pMenu, Option *pOption);

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMenu(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_nSelectMenu = 0;
	s_nSelectOption = 0;
	s_nAlphaTime = 0;

	// �������̃N���A
	memset(s_aMenu, 0, sizeof(s_aMenu));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_MENU * MAX_OPTION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMenu(void)
{
	if (s_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMenu(void)
{
	Menu *pMenu = &s_aMenu[s_nSelectMenu];

	if (!pMenu->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	s_nAlphaTime++;

	Option *pOption = &pMenu->Option[s_nSelectOption];

	pOption->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �F�̐ݒ�
	SetColor(*pOption);

	// ����
	Input(pMenu, pOption);

	float fCurve = cosf((s_nAlphaTime * pMenu->fBlinkSpeed) * (D3DX_PI * 2.0f));

	pOption->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, (fCurve * (1.0f - MIN_ALPHA)) + MIN_ALPHA);
	
	// �F�̐ݒ�
	SetColor(*pOption);
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMenu(void)
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_MENU; i++)
	{
		Menu *pMenu = &s_aMenu[i];

		if (!pMenu->bUse)
		{// �g�p���Ă��Ȃ�
			continue;
		}

		/*�� �g�p���Ă��� ��*/

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
int SetMenu(const MenuArgument &menu)
{
	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	int nIdx = 0;

	for (int nIdx = 0; nIdx < MAX_MENU; nIdx++)
	{
		Menu *pMenu = &s_aMenu[nIdx];

		if (pMenu->bUse)
		{// �g�p���Ă���
			continue;
		}

		/*�� �g�p���Ă��Ȃ� ��*/

		float fPosX = menu.fLeft + ((menu.fRight - menu.fLeft) * 0.5f);

		pMenu->pos = D3DXVECTOR3(fPosX, menu.fTop, 0.0f);
		pMenu->nNumUse = menu.nNumUse;

		pMenu->fWidth = menu.fRight - menu.fLeft;
		pMenu->fHeight = menu.fBottom - menu.fTop;
		pMenu->fInterval = pMenu->fHeight / (menu.nNumUse + 1);
		pMenu->fBlinkSpeed = NORMAL_BLINK_SPEED;

		pMenu->bUse = true;

		s_nSelectMenu = nIdx;
		s_nSelectOption = 0;

		for (int j = 0; j < pMenu->nNumUse; j++)
		{
			Option *pOption = &pMenu->Option[j];

			pOption->pos = D3DXVECTOR3(pMenu->pos.x, menu.fTop + (pMenu->fInterval * (j + 1)), 0.0f);
			pOption->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pOption->fWidth = menu.fWidth;
			pOption->fHeight = menu.fHeight;
			pOption->pTexture = *menu.pTexture[j];

			// ���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdx * MAX_MENU) + (j * 4);		// �w��̈ʒu�܂Ń|�C���^��i�߂�

			float fWidth = menu.fWidth * 0.5f;
			float fHeight = menu.fHeight * 0.5f;

			// ���_���W�̐ݒ�
			pVtx[0].pos = pOption->pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
			pVtx[1].pos = pOption->pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
			pVtx[2].pos = pOption->pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
			pVtx[3].pos = pOption->pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pOption->col;
			pVtx[1].col = pOption->col;
			pVtx[2].col = pOption->col;
			pVtx[3].col = pOption->col;

			// ���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();
		}

		break;
	}

	return nIdx;
}

//--------------------------------------------------
// ���Z�b�g
//--------------------------------------------------
void ResetMenu(int nIdx)
{
	s_nSelectMenu = 0;
	s_nSelectOption = 0;
	s_nAlphaTime = 0;
	s_aMenu[nIdx].bUse = false;
}

//--------------------------------------------------
// �F�̐ݒ�
//--------------------------------------------------
static void SetColor(const Option &option)
{
	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_nSelectMenu * MAX_MENU) + (s_nSelectOption * 4);		// �w��̈ʒu�܂Ń|�C���^��i�߂�

	// ���_�J���[�̐ݒ�
	pVtx[0].col = option.col;
	pVtx[1].col = option.col;
	pVtx[2].col = option.col;
	pVtx[3].col = option.col;

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// ����
//--------------------------------------------------
static void Input(Menu *pMenu, Option *pOption)
{
	if (GetFade() != FADE_NONE)
	{// �t�F�[�h���Ă���
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetDirectJoypadTrigger(JOYKEY_CROSS_UP) /*|| GetDirectJoypadStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP)*/)
	{// W�L�[�������ꂽ���ǂ���
		s_nSelectOption = ((s_nSelectOption - 1) + pMenu->nNumUse) % pMenu->nNumUse;
		pOption = &pMenu->Option[s_nSelectOption];
		s_nAlphaTime = 0;
	}
	else if (GetKeyboardTrigger(DIK_S) || GetDirectJoypadTrigger(JOYKEY_CROSS_DOWN) /*|| GetDirectJoypadStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN)*/)
	{// S�L�[�������ꂽ���ǂ���
		s_nSelectOption = ((s_nSelectOption + 1) + pMenu->nNumUse) % pMenu->nNumUse;
		pOption = &pMenu->Option[s_nSelectOption];
		s_nAlphaTime = 0;
	}

	if (GetKeyboardTrigger(DIK_RETURN) || GetDirectJoypadTrigger(JOYKEY_DIRECT_1_BUTTON))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		pMenu->fBlinkSpeed = DECISION_BLINK_SPEED;
	}
}
