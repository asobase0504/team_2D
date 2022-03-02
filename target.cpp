//**************************************************************************************************
//
// �^�[�Q�b�g�̐ݒ菈��
// Auther�F�������l
// Author : ����]��
//
//**************************************************************************************************
#include "main.h"
#include "input.h"
		 
#include "target.h"
#include "bullet.h"
#include "player.h"

//*******************************************************************************
// �}�N����`
//*******************************************************************************
#define Target_SPEED_SKY	(15.0f)		// �󒆂̃^�[�Q�b�g���x
#define Target_SPEED_GROUND	(20.0f)		// �n��̃^�[�Q�b�g���x
#define SIZE_TARGET			(30.0f)		// �^�[�Q�b�g�̑傫��

//*******************************************************************************
// �O���[�o���ϐ�
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_TARGET] = {};	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
static Target s_aTarget[MAX_TARGET];					// �^�[�Q�b�g��
static char s_texName[MAX_TARGET][128];					// �^�[�Q�b�g�e�N�X�`���̖��O�����擾

//---------------------------------------------------------------------------
// �^�[�Q�b�g������
//---------------------------------------------------------------------------
void InitTarget(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �^�[�Q�b�g�e�N�X�`���̖��O���̏�����
	memset(&s_texName[0], 0, sizeof(s_texName));

	for (int i = 0; i < MAX_TARGET; i++)
	{// �^�[�Q�b�g�ɒ���t����e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			&s_texName[i][0],
			&s_pTexture[i]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TARGET,		// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,													// ���_�t�@�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_���ւ̃|�C���^�𐶐�						
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �^�[�Q�b�g���̏�����
	memset(&s_aTarget[0], 0, sizeof(s_aTarget));

	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		// �Ίp���̒������Z�o����
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		// �Ίp���̊p�x���Z�o
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		//���_���W�̐ݒ�
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g�I��
//---------------------------------------------------------------------------
void UninitTarget(void)
{
	for (int i = 0; i < MAX_TARGET; i++)
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
// �^�[�Q�b�g�X�V
//---------------------------------------------------------------------------
void UpdateTarget(void)
{
	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		if (!pTarget->bUse)
		{
			continue;
		}

		if (pTarget->bLockOnEnemy)
		{
			pTarget->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// �J���[
		}
		else if (!pTarget->bLockOnEnemy)
		{
			switch (pTarget->TargetType)
			{
			case TARGETSTATE_LOCK_OFF:
				pTarget->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);		// �J���[
				break;

			case TARGETSTATE_LOCK_ON:
				pTarget->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f);		// �J���[
				break;
			}
		}

		//////////////////////////////////////
		//// �X�N���[��
		//pTarget->pos += �X�N���[�����l;
		//////////////////////////////////////

		if (pTarget->nIdxBullet != -1)
		{
			Bullet *pBullet = GetBullet();

			pBullet += pTarget->nIdxBullet;

			if (!pBullet->bUse)
			{
				pTarget->bUse = false;
			}
		}

		//�Ίp���̒������Z�o����
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		//�Ίp���̊p�x���Z�o
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		//���_���ւ̃|�C���^�𐶐�						
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		//���_���W�̐ݒ�
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		//���_�o�b�t�@���A�����b�N
		s_pVtxBuff->Unlock();
	}
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g�`��
//---------------------------------------------------------------------------
void DrawTarget(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		if (!pTarget->bUse)
		{
			continue;
		}

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[(int)(pTarget->TargetType)]);

		//�^�[�Q�b�g�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g�ݒ�
//---------------------------------------------------------------------------
int SetTarget(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ϐ��錾
	int nCntTarget;

	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		Target* pTarget = &s_aTarget[nCntTarget];

		if (pTarget->bUse)
		{//�g�p����ĂȂ��ꍇ
			continue;
		}

		//�^�C�g�����S���̐ݒ�
		pTarget->pos = pos;										// ���S�_
		pTarget->rot = rot;										// ����
		pTarget->size = D3DXVECTOR3(SIZE_TARGET, SIZE_TARGET, 0.0f);		// �傫��
		pTarget->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);		// �J���[
		pTarget->TargetType = TARGETSTATE_LOCK_OFF;				// ���
		pTarget->nIdxBullet = -1;								// �Ή�����e�̔ԍ�
		pTarget->bLockOnEnemy = false;							// �G������
		pTarget->bUse = true;									// �g�p���Ă�

		//�Ίp���̒������Z�o����
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		//�Ίp���̊p�x���Z�o
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		//���_���ւ̃|�C���^�𐶐�						
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntTarget * 4;

		//���_���W�̐ݒ�
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		//���_�o�b�t�@���A�����b�N
		s_pVtxBuff->Unlock();

		break;
	}

	return nCntTarget;
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g�̈ړ�
//---------------------------------------------------------------------------
void SetPositionTarget(int nIdxShadow, D3DXVECTOR3 pos)
{
	Target* pTarget = &s_aTarget[nIdxShadow];

	if (pTarget->TargetType == TARGETSTATE_LOCK_OFF)
	{
		// �^�[�Q�b�g���̐ݒ�
		pTarget->pos = pos;										// ���S�_

		// �Ίp���̒������Z�o����
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		// �Ίp���̊p�x���Z�o
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		// ���_���ւ̃|�C���^�𐶐�						
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nIdxShadow * 4;

		// ���_���W�̐ݒ�
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		// ���_�o�b�t�@���A�����b�N
		s_pVtxBuff->Unlock();
	}
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g�̃��b�N�I��
//---------------------------------------------------------------------------
void LockOnTarget(int nIdxShadow)
{
	Target* pTarget = &s_aTarget[nIdxShadow];
	Player *pPlayer = GetPlayer();

	if (pTarget->TargetType == TARGETSTATE_LOCK_OFF)
	{// ���b�N�I�t�̎�
		pTarget->TargetType = TARGETSTATE_LOCK_ON;
		pTarget->nIdxBullet = SetBullet(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - pPlayer->size.y, 0.0f), pPlayer->rot, pPlayer->BulletType, nIdxShadow, pTarget->bLockOnEnemy);
	}
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g�̓����蔻��
//---------------------------------------------------------------------------
void CollisionTarget(D3DXVECTOR3 pos, float fLength, ENEMYTYPE TypeEnemy)
{
	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		if (!pTarget->bUse)
		{
			continue;
		}

		pTarget->bLockOnEnemy = false;

		// �ϐ��錾
		float fTotalLength = pTarget->fLength + fLength;		// ���a�̍��v
		D3DXVECTOR3 DiffPos = pos - pTarget->pos;				// �ʒu�̍�

		// �ʒu�̍�
		float DiffLength = sqrtf((DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y));

		if (fTotalLength >= DiffLength
			&& pTarget->bLockOnEnemy == false
			&& TypeEnemy == ENEMYTYPE_GROUND_1
			&& TypeEnemy == ENEMYTYPE_GROUND_2)
		{
			pTarget->bLockOnEnemy = true;
		}
	}
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g���̎擾
//---------------------------------------------------------------------------
Target *GetTarget(void)
{
	return &s_aTarget[0];
}

//---------------------------------------------------------------------------
// �^�[�Q�b�g�̒��_�֌W�̐ݒ�
//---------------------------------------------------------------------------
void SetTargetVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele)
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
