//================================
//
// �X�^�b�t���[�����
// Author:�y���m��
//
//================================
#include "staffroll.h"
#include "input.h"
#include "enemy.h"
#include "map.h"
#include "fade.h"

//�}�N����`
#define NUM_STAFFROLL	(100)					//�e�N�X�`���̍ő吔
#define MAP_SPEED	(0.0005f)					//��ʃX�N���[���̑���
#define MAX_MEMBER	(2)							//�G���h���[���ɂ̂���ő吔

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9			s_pTexture[NUM_STAFFROLL] = {};		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;					//���_�o�b�t�@�ւ̃|�C���^;
static D3DXVECTOR3 StaffPos[NUM_STAFFROLL];		//�X�^�b�t���[���̏ꏊ
static float StaffWidth[NUM_STAFFROLL];			//�X�^�b�t���[���̕�
static float StaffHeight[NUM_STAFFROLL];		//�X�^�b�t���[���̍���
static STAFFROLL s_Staffroll[NUM_STAFFROLL];	//�X�^�b�t���[���̏��̎擾
static bool MODE_ROLL;							//�G���h���[���̌o�ߏ�
static int RollNumber;							//���݂̃G���h���[���̔ԍ�
static int FinishRoll;							//�G���h���[�����I������܂ł̎���

//============================
// �X�^�b�t���[���̏���������
//============================
void InitStaffroll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

//------------------------------
//	�e�N�X�`���̓ǂݍ���
//------------------------------
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\jobilogo.png", &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\buriya.png", &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\hamada.png", &s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press.png", &s_pTexture[3]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press.png", &s_pTexture[4]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_STAFFROLL,	//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//------------------------------
	//	�\���̂̐ݒ�
	//------------------------------
	//�ʒu�̐ݒ�
	StaffPos[0] = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	StaffPos[1] = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 100.0f, 0.0f);

	//���̐ݒ�
	StaffWidth[0] = SCREEN_WIDTH / 2;
	StaffWidth[1] = 450.0f;

	//�����̐ݒ�
	StaffHeight[0]= SCREEN_HEIGHT / 2;
	StaffHeight[1]= 100.0f;

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++)
	{
		STAFFROLL *staff = s_Staffroll + nCnt;
		staff->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		staff->bUse = false;
		staff->nCntStaff = 0;
	}

	FinishRoll = 0;
	RollNumber = 0;
	MODE_ROLL = false;

	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	SetStaffroll(StaffPos[0], StaffWidth[0], StaffHeight[0], 0);
}

//============================
// �X�^�b�t���[���̏I������
//============================
void UninitStaffroll(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++)
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
// �X�^�b�t���[���̍X�V����
//============================
void UpdateStaffroll(void)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^
	
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	STAFFROLL *staff = s_Staffroll;

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++, staff++)
	{
		if (staff->bUse)
		{
			staff->nCntStaff++;

			if (MODE_ROLL)
			{//���_���W�̐ݒ�

				staff->pos.y -= 1.0f;

				pVtx[0].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth + staff->nCntStaff / 2, staff->pos.y - staff->fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth - staff->nCntStaff / 2, staff->pos.y - staff->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth + staff->nCntStaff / 2, staff->pos.y + staff->fHeight - staff->nCntStaff / 3, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth - staff->nCntStaff / 2, staff->pos.y + staff->fHeight - staff->nCntStaff / 3, 0.0f);

				//rhw�̐ݒ�
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;

				//���_�J���[�̐ݒ�
				pVtx[0].col = staff->col;
				pVtx[1].col = staff->col;
				pVtx[2].col = staff->col;
				pVtx[3].col = staff->col;

				if (RollNumber < MAX_MEMBER - 1)
				{
					if (staff->nCntStaff == 240)
					{
						RollNumber++;

						SetStaffroll(StaffPos[1], StaffWidth[1], StaffHeight[1], 1 + RollNumber);
					}
				}
				if(RollNumber == MAX_MEMBER - 1)
				{
					FinishRoll++;

					if (FinishRoll == 600 * MAX_MEMBER)
					{
						SetFade(MODE_TITLE);
					}
				}
			}
			else if (!MODE_ROLL)
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth + staff->nCntStaff / 2, staff->pos.y - staff->fHeight + staff->nCntStaff / 3, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth - staff->nCntStaff / 2, staff->pos.y - staff->fHeight + staff->nCntStaff / 3, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth + staff->nCntStaff / 2, staff->pos.y + staff->fHeight - staff->nCntStaff / 3, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth - staff->nCntStaff / 2, staff->pos.y + staff->fHeight - staff->nCntStaff / 3, 0.0f);

				//rhw�̐ݒ�
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;

				//���_�J���[�̐ݒ�
				pVtx[0].col = staff->col;
				pVtx[1].col = staff->col;
				pVtx[2].col = staff->col;
				pVtx[3].col = staff->col;

				if (staff->pos.x < staff->nCntStaff / 2)
				{
					staff->nCntStaff = 0;
					staff->bUse = false;
					MODE_ROLL = true;
					SetStaffroll(StaffPos[1], StaffWidth[1], StaffHeight[1], 1);
				}
			}
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//----------------
	//	��ʑJ��
	//----------------
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A, 0))
	{

		SetFade(MODE_TITLE);
	}
}

//============================
// �X�^�b�t���[���̕`�揈��
//============================
void DrawStaffroll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++)
	{
		STAFFROLL *staff = s_Staffroll + nCnt;

		if (staff->bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture[staff->nType]);

			//�X�^�b�t���[���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCnt * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);						//�`�悷��v���~�e�B�u��
		}
	}
}

//=========================================
//�X�^�b�t���[����ݒu���鏈��
//=========================================
void SetStaffroll(D3DXVECTOR3 pos, float fWidth , float fHeight, int nType)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++)
	{
		STAFFROLL *staff = s_Staffroll + nCnt;

		if (!staff->bUse)
		{
			staff->pos = pos;
			staff->nType = nType;
			staff->bUse = true;
			staff->fWidth = fWidth;
			staff->fHeight = fHeight;
			staff->nCntStaff = 0;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth, staff->pos.y - staff->fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth, staff->pos.y - staff->fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth, staff->pos.y + staff->fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth, staff->pos.y + staff->fHeight, 0.0f);
			
			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = staff->col;
			pVtx[1].col = staff->col;
			pVtx[2].col = staff->col;
			pVtx[3].col = staff->col;

			//�e�N�X�`���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}
