//===================
//�p�[�e�B�N��
//autho hamadaryuuga
//===================
#include"particle.h"
#include"common.h"
#include"player.h"

#define NUR_PARTICLE (4)

//�X�^�e�B�b�N
static LPDIRECT3DTEXTURE9 s_pTextureParticle[NUR_PARTICLE] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffParticle = NULL; //���_�o�b�t�@�̐ݒ�
static Particle s_aParticle[MAX_PARTICLE];

//==================================
//������
//==================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	int nCntParticle;
	
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/effect000.jpg",
		&s_pTextureParticle[0]);


	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//�������Y��Ȃ�
		s_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		s_aParticle[nCntParticle].nLife = 0;
		s_aParticle[nCntParticle].bUse = false;
		s_aParticle[nCntParticle].fRadeius = 0;
		s_aParticle[nCntParticle].nPatternAnim = 0;
		s_aParticle[nCntParticle].nCounterAnim = 0;
	}

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuffParticle,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		////���_���W


		SetNormalpos2d(pVtx, s_aParticle[nCntParticle].pos.x - s_aParticle[nCntParticle].fRadeius,
				s_aParticle[nCntParticle].pos.x + s_aParticle[nCntParticle].fRadeius,
				s_aParticle[nCntParticle].pos.y - s_aParticle[nCntParticle].fRadeius,
				s_aParticle[nCntParticle].pos.y + s_aParticle[nCntParticle].fRadeius);

		//RHW�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = s_aParticle[nCntParticle].col;
		pVtx[1].col = s_aParticle[nCntParticle].col;
		pVtx[2].col = s_aParticle[nCntParticle].col;
		pVtx[3].col = s_aParticle[nCntParticle].col;

		//�e�N�X�`���̍��W�ݒ�
		Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffParticle->Unlock();
}
void UninitParticle(void)
{
	for (int cut = 0; cut < NUR_PARTICLE; cut++)
	{
		if (s_pTextureParticle[cut] != NULL)
		{					  
			s_pTextureParticle[cut]->Release();
			s_pTextureParticle[cut] = NULL;
		}
	}
	

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffParticle != NULL)
	{
		s_pVtxBuffParticle->Release();
		s_pVtxBuffParticle = NULL;
	}
}
void UpdateParticle(void)
{
	int nCntParticle;
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
	s_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (s_aParticle[nCntParticle].bUse)
		{
			Player* pPlayer = GetPlayer();

			
			s_aParticle[nCntParticle].pos += s_aParticle[nCntParticle].move;
			s_aParticle[nCntParticle].pos.x += pPlayer->move.x/2;
			if (pPlayer->pos.y > s_aParticle[nCntParticle].pos.y)
			{
				s_aParticle[nCntParticle].bUse = false;
			}
		
			SetNormalpos2d(pVtx, s_aParticle[nCntParticle].pos.x - s_aParticle[nCntParticle].fRadeius,
						s_aParticle[nCntParticle].pos.x + s_aParticle[nCntParticle].fRadeius,
						s_aParticle[nCntParticle].pos.y - s_aParticle[nCntParticle].fRadeius,
						s_aParticle[nCntParticle].pos.y + s_aParticle[nCntParticle].fRadeius);

			s_aParticle[nCntParticle].col.a -= 1.0f / s_aParticle[nCntParticle].nMaxLife;
			//���_�J���[�̐ݒ�
			pVtx[0].col = s_aParticle[nCntParticle].col;
			pVtx[1].col = s_aParticle[nCntParticle].col;
			pVtx[2].col = s_aParticle[nCntParticle].col;
			pVtx[3].col = s_aParticle[nCntParticle].col;

		
			
		
		}
		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffParticle->Unlock();
}
void DrawParticle(void)
{
	int nCntParticle;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^

	pDevice = GetDevice(); //���

	 //���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	//�e�N�X�`���̕`��
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (s_aParticle[nCntParticle].bUse)
		{
		
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTextureParticle[0]);
			
			//�|���S���̕`�� �l�p
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCntParticle * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);
		}//�v���~�e�B�u(�|���S��)��
	}
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadeius, int nLife , int nType)
{
	int nCntParticle;
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!s_aParticle[nCntParticle].bUse)
		{
			//�G���g�p����ĂȂ��ꍇ
			s_aParticle[nCntParticle].fRadeius = fRadeius;
			s_aParticle[nCntParticle].pos.x = pos.x;
			s_aParticle[nCntParticle].pos.y = pos.y;
			s_aParticle[nCntParticle].pos.z = pos.z;

			s_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
			s_aParticle[nCntParticle].move.y = 3.0f;//sinf((float)(rand() % 629 - 314) / 100.0f + 100.0f);

			//���_���W
			SetNormalpos2d(pVtx, s_aParticle[nCntParticle].pos.x - fRadeius,
				s_aParticle[nCntParticle].pos.x + fRadeius, 
				s_aParticle[nCntParticle].pos.y - fRadeius,
				s_aParticle[nCntParticle].pos.y + fRadeius);
			
			s_aParticle[nCntParticle].nType = nType;
			s_aParticle[nCntParticle].nLife = nLife;
			s_aParticle[nCntParticle].bUse = true;
			s_aParticle[nCntParticle].col = col;
			s_aParticle[nCntParticle].nMaxLife = nLife;
			
			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffParticle->Unlock();

}