//=========================================
// �w�i�ݒ�
// author hamada ryuuga
// Author �y���m��
//=========================================
#include"map.h"
#include <stdio.h>

#define NUM_MAP	    (12800)						// �w�i����
#define X_MAP		(4)							// �}�b�v�`�b�v�T�C�YX
#define Y_MAP		(2)							// �}�b�v�`�b�v�T�C�YY
#define MAPSIZX		(10)						// �}�b�v�T�C�YX
#define MAPSIZY		(50)						// �}�b�v�T�C�YY
#define MAPYLENGTH  (10)						// �}�b�v�̒���
#define BLOCKSIZX   ((SCREEN_WIDTH / MAPSIZX))
#define BLOCKSIZY	((SCREEN_HEIGHT * MAPYLENGTH / MAPSIZY))

// �X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTextureMap = {};			// �e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffMap = NULL;	// ���_�o�b�t�@�̐ݒ�
static Map s_aMap[NUM_MAP];
static float s_fMapSizX, s_fMapSizY;

//=========================================
// �}�b�v�`�b�v�̏���������
//=========================================
void InitMap(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/mapchip_field2.jpg",
		&s_pTextureMap);

	// ���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMap,
		NULL);

	VERTEX_2D*pVtx; // ���_�ւ̃|�C���^
	
	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_MAP; i++)
	{
		s_aMap[i].bUse = false;
		s_aMap[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aMap[i].move = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		
		// ���W�̐ݒ�
		SetNorotpos2d(pVtx,
			0.0f,
			BLOCKSIZX,
			0.0f,
			BLOCKSIZY);

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

		// �e�N�X�`���̍��W�ݒ�
		Settex2d(pVtx, (1.0f / X_MAP) * 0, (1.0f / X_MAP) * 0 + 1, (1.0f / Y_MAP) * 0, (1.0f / Y_MAP) * 0 + 1);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuffMap->Unlock();
}

//=========================================
// �}�b�v�̔j�����鏈��
//=========================================
void UninitMap(void)
{
	//�e�N�X�`���̔j��
	if (s_pTextureMap != NULL)
	{
		s_pTextureMap->Release();
		s_pTextureMap = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffMap != NULL)
	{
		s_pVtxBuffMap->Release();
		s_pVtxBuffMap = NULL;
	}
}
//=========================================
// �}�b�v�̍X�V����
//=========================================
void UpdateMap(void)
{
	for (int i = 0; i < NUM_MAP; i++)
	{
		Map* pMap = &s_aMap[i];

		if (!pMap->bUse)
		{// �Ή������ӏ����g�p����Ă��鎞
			continue;
		}

		pMap->pos += pMap->move;

		VERTEX_2D*pVtx; // ���_�ւ̃|�C���^

		// ���_�o�b�t�@���A�����b�N
		s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		// ���W��ݒ肷��
		SetNorotpos2d(pVtx,
			pMap->pos.x,
			pMap->pos.x + BLOCKSIZX,
			pMap->pos.y,
			pMap->pos.y + BLOCKSIZY);

		// ���_�o�b�t�@���A�����b�N
		s_pVtxBuffMap->Unlock();
	}
}

//=========================================
// �}�b�v�̕`�揈��
//=========================================
void DrawMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̃|�C���^

	for (int i = 0; i < NUM_MAP; i++)
	{
		Map* pMap = &s_aMap[i];

		if (!pMap->bUse)
		{
			continue;
		}

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffMap, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureMap);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			4 * i,
			2);
	}
}

//=========================================
// �}�b�v�f�[�^�̐ݒ�
//=========================================
void SetMap(D3DXVECTOR3 pos, int nType, int tex)
{
	for (int nCntMap = 0; nCntMap < NUM_MAP; nCntMap++)
	{
		Map* pMap = &s_aMap[nCntMap];

		if (pMap->bUse)
		{// �G���g�p����ĂȂ��ꍇ
			continue;
		}

		pMap->pos = pos;

		pMap->nType = nType;
		pMap->bUse = true;

		VERTEX_2D*pVtx; // ���_�ւ̃|�C���^

		s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nCntMap * 4;

		// ���W��ݒ肷��
		SetNorotpos2d(pVtx,
			pMap->pos.x,
			pMap->pos.x + BLOCKSIZX,
			pMap->pos.y,
			pMap->pos.y + BLOCKSIZY);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		float X = (float)(tex % 100 / 10);
		float Y = (float)(tex % 10 / 1);

		// �e�N�X�`���̍��W�ݒ�
		Settex2d(pVtx, (1.0f / X_MAP) * X, (1.0f / X_MAP) * X + 1.0f / X_MAP, (1.0f / Y_MAP) * Y, ((1.0f / Y_MAP) * Y) + (1.0f / Y_MAP));

		//���_�o�b�t�@���A�����b�N
		s_pVtxBuffMap->Unlock();
		break;
	}
}

//=========================================
// �}�b�v�`�b�v�̓ǂݍ���
//=========================================
void InitMapSet(char *Filename)
{
	int s_aMap[MAPSIZY][MAPSIZX];
	s_fMapSizX = (SCREEN_WIDTH) / MAPSIZX;
	s_fMapSizY = (SCREEN_HEIGHT * 3.0f) / MAPSIZY;
	FILE *pFile = NULL;	// �t�@�C���|�C���^��錾

	int nCntX, nCntY;

	// �t�@�C�����J��
	pFile = fopen(&Filename[0], "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		for (nCntY = 0; nCntY < MAPSIZY; nCntY++)
		{
			for (nCntX = 0; nCntX < MAPSIZX; nCntX++)
			{// Map�̏�������
				fscanf(pFile, "%d", &s_aMap[nCntY][nCntX]);
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
	}

	// �u���b�N�ݒ�
	for (nCntY = 0; nCntY < MAPSIZY; nCntY++)
	{
		for (nCntX = 0; nCntX < MAPSIZX; nCntX++)
		{// Map�̏�������
			SetMap(D3DXVECTOR3(s_fMapSizX * nCntX, -(s_fMapSizY * nCntY) + SCREEN_HEIGHT, 0.0f), 1, s_aMap[nCntY][nCntX]);
		}
	}
	for (nCntY = 0; nCntY < MAPSIZY; nCntY++)
	{
		for (nCntX = 0; nCntX < MAPSIZX; nCntX++)
		{// Map�̏�������
			s_aMap[nCntY][nCntX] = 0;
		}
	}
}

//=========================================
// ���W�ݒ�
//=========================================
void SetNorotpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}
//=========================================
// �e�N�X�`���̍��W�ݒ�
//=========================================
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	// �e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);
}