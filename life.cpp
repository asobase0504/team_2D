//------------------------------------------------------------------------------
//
//���C�t
//Author;takano
//
//------------------------------------------------------------------------------
#include"life.h"

//---------------
//�}�N����`
//---------------
#define MAX_LIFE			(256)								//�ő僉�C�t

//---------------------
//�X�^�e�B�b�N�ϐ�
//---------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;					//�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
static bool bUseLife[MAX_LIFE];
static int nCountNumber;

//-------------------------
//���C�t��ʂ̏���������
//-------------------------
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//�f�o�C�X�̃|�C���^

													//�f���@�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff, NULL);

	D3DXCreateTextureFromFile(pDevice, "Data//TEXTURE//Life.png", &s_pTexture);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{//������
		bUseLife[nCnt] = false;
	}

	nCountNumber = 0;

	VERTEX_2D*pVtx;						//���_���ւ̃|�C���^

										//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		//���C�t�̈ʒu�̌v�Z
		D3DXVECTOR3 pos = D3DXVECTOR3(20.0f + (40.0f * nCnt), 690.0f, 0.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - LIFE_SIZE, pos.y - LIFE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + LIFE_SIZE, pos.y - LIFE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - LIFE_SIZE, pos.y + LIFE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + LIFE_SIZE, pos.y + LIFE_SIZE, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�L�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				//�f�f�f�[�^��4���i��
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//-------------------------
//���C�t��ʂ̏I������
//-------------------------
void UninitLife(void)
{
	//�e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//-------------------------
//���C�t��ʂ̍X�V����
//-------------------------
void UpdateLife(void)
{

}

//-------------------------
//���C�t��ʂ̕`�揈��
//-------------------------
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//�f�o�C�X�̃|�C���^

													//�f���@�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_LIFE; i++)
	{
		if (bUseLife[i])
		{//�g�p����Ă��鎞

		 //�|���S���̕`��
			pDevice->DrawPrimitive
			(D3DPT_TRIANGLESTRIP,
				i * 4,
				2);
		}
	}
}

//-----------------------------------
//���C�t�̔z�u
//-----------------------------------
void SetLife(void)
{
	if (nCountNumber < MAX_LIFE)
	{//MAXLIFE�ȏ����̂�h��

		bUseLife[nCountNumber] = true;		//�g�p����

		nCountNumber++;						//���C�t�̃J�E���^�[�𑝂₷
	}
}

//-----------------------------------
//���C�t�̃q�b�g����
//-----------------------------------
void HitLife(void)
{
	if (nCountNumber > 0)
	{//0�ȉ�������̂�h��

		nCountNumber--;						//���C�t�̃J�E���^�[�����炷

		bUseLife[nCountNumber] = false;		//�g�p���Ȃ���Ԃɂ���
	}

}