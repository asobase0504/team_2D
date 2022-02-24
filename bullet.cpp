//**************************************************************************************************
//
// �e�̐ݒ菈��
// Auther�F�������l
// Author : ����]��
//
//**************************************************************************************************
#include"main.h"
#include"input.h"

#include"bullet.h"

//*******************************************************************************
// �}�N����`
//*******************************************************************************
#define BULLET_SPEED_SKY	(15.0f);	// �󒆂̒e���x
#define BULLET_SPEED_GROUND	(10.0f);	// �n��̒e���x
#define SIZE_BULLET			(15.0f)		//�e�̑傫��

//*******************************************************************************
// �O���[�o���ϐ�
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_BULLET] = {};	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static Bullet s_aBullet[MAX_BULLET];					// �e��
static char s_texName[MAX_BULLET][128];					// �e�e�N�X�`���̖��O�����擾

//---------------------------------------------------------------------------
// �e������
//---------------------------------------------------------------------------
void InitBullet(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�e�N�X�`���̖��O���̏�����
	memset(&s_texName[0], 0, sizeof(s_texName));

	for (int i = 0; i < MAX_BULLET; i++)
	{// �e�ɒ���t����e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			&s_texName[i][0],
			&s_pTexture[i]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,		// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,													// ���_�t�@�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_���ւ̃|�C���^�𐶐�						
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e���̏�����
	memset(&s_aBullet[0], 0, sizeof(s_aBullet));

	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];
		
		// �Ίp���̒������Z�o����
		pBullet->fLength = sqrtf(((pBullet->size.x * pBullet->size.x) + (pBullet->size.y * pBullet->size.y)) * 0.5f);

		// �Ίp���̊p�x���Z�o
		pBullet->fAngele = atan2f(pBullet->size.x, pBullet->size.y);

		//���_���W�̐ݒ�
		SetBulletVtx(pVtx, pBullet->pos, pBullet->rot, pBullet->col, pBullet->fLength, pBullet->fAngele);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// �e�I��
//---------------------------------------------------------------------------
void UninitBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{// �e�N�X�`���̔j��	  
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();

			s_pTexture[i] = NULL;
		}
	}

	// ���_�o�b�t�@��j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();

		s_pVtxBuff = NULL;
	}
}

//---------------------------------------------------------------------------
// �e�X�V
//---------------------------------------------------------------------------
void UpdateBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];

		if (!pBullet->bUse)
		{
			continue;
		}

		// ����������
		pBullet->nLife--;

		if (pBullet->nLife <= 0)
		{// ������0�ȉ��̎�
			pBullet->bUse = false;
		}

		// �ړ�
		pBullet->pos += pBullet->move;

		//�Ίp���̒������Z�o����
		pBullet->fLength = sqrtf(((pBullet->size.x * pBullet->size.x) + (pBullet->size.y * pBullet->size.y)) * 0.5f);

		//�Ίp���̊p�x���Z�o
		pBullet->fAngele = atan2f(pBullet->size.x, pBullet->size.y);

		//���_���ւ̃|�C���^�𐶐�						
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		//���_���W�̐ݒ�
		SetBulletVtx(pVtx, pBullet->pos, pBullet->rot, pBullet->col, pBullet->fLength, pBullet->fAngele);

		//���_�o�b�t�@���A�����b�N
		s_pVtxBuff->Unlock();

		if (pBullet->pos.x <= -SIZE_BULLET || pBullet->pos.x >= SCREEN_WIDTH + SIZE_BULLET || pBullet->pos.y <= -SIZE_BULLET || pBullet->pos.y >= SCREEN_HEIGHT + SIZE_BULLET)
		{//�e����ʊO�ɏo��
			pBullet->bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
		}

	}
}

//---------------------------------------------------------------------------
// �e�`��
//---------------------------------------------------------------------------
void DrawBullet(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];
		
		if (!pBullet->bUse)
		{
			continue;
		}

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[(int)(pBullet->BulletType)]);

		//�e�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

//---------------------------------------------------------------------------
// �e�ݒ�
//---------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3	pos, D3DXVECTOR3 rot, int nType)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];

		if (pBullet->bUse)
		{//�g�p����ĂȂ��ꍇ
			continue;
		}

		//�^�C�g�����S���̐ݒ�
		pBullet->pos = pos;									// ���S�_
		pBullet->rot = rot;									// ����
		pBullet->BulletType = (BulletType)(nType);			// �^�C�v 
		pBullet->bUse = true;								// �g�p���Ă�

		pBullet->fRotMove = pBullet->rot.z - D3DX_PI;		// �ړ�����

		if (pBullet->fRotMove >= D3DX_PI)
		{// �ړ������̐��K��
			pBullet->fRotMove -= D3DX_PI * 2;
		}
		else if (pBullet->fRotMove <= -D3DX_PI)
		{// �ړ������̐��K��
			pBullet->fRotMove += D3DX_PI * 2;
		}

		switch (pBullet->BulletType)
		{
		case BULLETTYPE_PLAYER_SKY:
			pBullet->size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);		// �傫��
			pBullet->col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);		// �J���[
			pBullet->fSpeed = BULLET_SPEED_SKY;						// ���x
			pBullet->nLife = 180;									// ����
			break;

		case BULLETTYPE_PLAYER_GROUND:
			pBullet->size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);		// �傫��
			pBullet->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		// �J���[
			pBullet->fSpeed = BULLET_SPEED_GROUND;					// ���x
			pBullet->nLife = 180;									// ����
			break;
		case BULLETTYPE_ENEMY:
			pBullet->size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);		// �傫��
			pBullet->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// �J���[
			pBullet->fSpeed = 3.0f;					// ���x
			pBullet->nLife = 180;										// ����
		}

		// �ړ��x�N�g���̎Z�o
		pBullet->move.x = sinf(pBullet->fRotMove);
		pBullet->move.y = cosf(pBullet->fRotMove);
		pBullet->move *= pBullet->fSpeed;

		//�Ίp���̒������Z�o����
		pBullet->fLength = sqrtf(((pBullet->size.x * pBullet->size.x) + (pBullet->size.y * pBullet->size.y)) / 2.0f);

		//�Ίp���̊p�x���Z�o
		pBullet->fAngele = atan2f(pBullet->size.x, pBullet->size.y);

		//���_���ւ̃|�C���^�𐶐�						
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		//���_���W�̐ݒ�
		SetBulletVtx(pVtx, pBullet->pos, pBullet->rot, pBullet->col, pBullet->fLength, pBullet->fAngele);

		//���_�o�b�t�@���A�����b�N
		s_pVtxBuff->Unlock();

		break;
	}
}

//---------------------------------------------------------------------------
// �e�̒��_�֌W�̐ݒ�
//---------------------------------------------------------------------------
void SetBulletVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele)
{
	//���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z + (D3DX_PI + fAngele)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z + (D3DX_PI + fAngele)) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngele)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngele)) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - (0 + fAngele)) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - (0 + fAngele)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z - (0 - fAngele)) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z - (0 - fAngele)) * fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}
