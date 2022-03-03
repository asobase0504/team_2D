//============================
//
// �}�b�v�`�b�v�ݒ�
// Author:hamada ryuuga
//
//============================
#include "map.h"
#include "input.h"
#include "enemy.h"
#include <stdio.h>
#include "common.h"

// �}�N����`
#define NUM_MAP	    (1024)		//�w�i����
#define X_MAP		(4)			//�}�b�v�`�b�v�T�C�YX
#define Y_MAP		(2)			//�}�b�v�`�b�v�T�C�YY
#define MAP_SIZEX	(10)		//�}�b�v�T�C�YX
#define MAP_SIZEY	(100)		//�}�b�v�T�C�YY
#define MAPYLENGTH	(10)		//�}�b�v�̒���
#define BLOCKSIZEX	(((float)SCREEN_WIDTH /MAP_SIZEX))
#define BLOCKSIZEY	(((float)SCREEN_HEIGHT*MAPYLENGTH / MAP_SIZEY))

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTextureMap = NULL; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffMap = NULL; //���_�o�b�t�@�̐ݒ�
static Map s_aMap[NUM_MAP];
static char s_aMapFile[10][255];
static D3DXVECTOR3 s_Move(0.0f, 5.0f, 0.0f);
static D3DXVECTOR3 s_PosOffset;
static float s_fMapScale;
static int stage = 0;

//================
//����������
//================
void InitMap(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9  pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mapchip_field2.jpg",
		&s_pTextureMap);

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, //�������_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuffMap,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);
	for (int i = 0; i < NUM_MAP; i++, pVtx += 4)
	{
		s_aMap[i].bUse = false;

		s_aMap[i].tex = 0;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffMap->Unlock();

	PasSetMap("data\\hoge4.txt");
	s_PosOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_fMapScale = 1.0f;
}

//�j��
void UninitMap(void)
{
	stage = 0;
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

//==================
//�X�V����
//==================
void UpdateMap(void)
{
	s_PosOffset += s_Move;

	if (s_PosOffset.y >= (BLOCKSIZEY * MAP_SIZEY) - SCREEN_HEIGHT - BLOCKSIZEY)
	{//�������[�v����
		s_PosOffset.y = 0.0f;
		stage++;
		stage %= 5;
		// �}�b�v�̐ݒ�B
		InitMapSet(&s_aMapFile[stage][0]);
		SetEnemyLynk(stage);
	}

	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;
	//���_�o�b�t�@�����b�N
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos;

	for (int i = 0; i < NUM_MAP; i++, pVtx += 4)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		pos = (s_aMap[i].pos + s_PosOffset)* s_fMapScale;

		SetNormalpos2d(pVtx, pos.x, pos.x + BLOCKSIZEX * s_fMapScale, pos.y, pos.y + BLOCKSIZEY * s_fMapScale);

		if (pos.y >= SCREEN_HEIGHT)
		{
			s_aMap[i].bUse = false;
		}
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffMap->Unlock();
}

//==================
//�`�揈��
//==================
void DrawMap(void)
{
	for (int i = 0; i < NUM_MAP; i++)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		//�f�o�C�X�̃|�C���^
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffMap, 0, sizeof(VERTEX_2D));
		
		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureMap);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);
	}
}

//==================
//�}�b�v�f�[�^�̐ݒ�
//==================
void SetMap(D3DXVECTOR3 pos, int nType, int tex)
{
	VERTEX_2D *pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	Map* pMap = s_aMap;

	for (int i = 0; i < NUM_MAP; i++, pVtx += 4, pMap++)
	{
		if (pMap->bUse)
		{
			continue;
		}

		pMap->bUse = true;

		//�}�b�v���g�p����ĂȂ��ꍇ
		pMap->pos = pos;
		pMap->nType = nType;
		pMap->tex = tex;

		int X = (pMap->tex % X_MAP);
		int Y = (pMap->tex / X_MAP);

		//�e�N�X�`���̍��W�ݒ�
		SetTex2d(pVtx, (1.0f / X_MAP) * X, (1.0f / X_MAP) * (X + 1.0f), (1.0f / Y_MAP) * Y, (1.0f / Y_MAP) * (Y + 1.0f));

		break;
	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffMap->Unlock();
}

//==================
//�t�@�C���̓��͏���
//==================
void InitMapSet(char *Filename)
{
	int aMap[MAP_SIZEY][MAP_SIZEX];

	for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
		{//Map�̏�������
			aMap[nCntY][nCntX] = 0;
		}
	}

	//�t�@�C�����J��
	FILE *pFile = fopen(&Filename[0], "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
			{//Map�̏�������
				fscanf(pFile, "%d", &aMap[nCntY][nCntX]);
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	//�u���b�N�ݒ�
	for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
		{//Map�̏�������
			SetMap(D3DXVECTOR3(BLOCKSIZEX*nCntX, (-(BLOCKSIZEY*nCntY) + SCREEN_HEIGHT), 0.0f), 1, aMap[nCntY][nCntX]);
		}
	}
}

//==================
//�t�@�C���̏o�͏���
//==================
void OutputMap(char *Filename)
{
	//�t�@�C���J��
	FILE *pFile = fopen(Filename, "w");
	int nCnt = 0;
	//�u���b�N�ݒ�
	for (int nCntY = 0; nCntY < MAP_SIZEY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
		{//Map�̏�������
			fprintf(pFile, "%d\t", s_aMap[nCnt].tex);
			nCnt++;
		}
		fprintf(pFile, "\n");
	}

	fclose(pFile);
}

//==================
//�}�b�v�`�b�v�ύX����
//==================
void ConversionMap(D3DXVECTOR3 pos, int tex)
{
	D3DXVECTOR3 mapPos;

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_MAP; i++, pVtx += 4)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		mapPos = (s_aMap[i].pos + s_PosOffset)* s_fMapScale;

		if (((mapPos.x < pos.x) && (mapPos.x + BLOCKSIZEX * s_fMapScale > pos.x)) &&
			((mapPos.y < pos.y) && (mapPos.y + BLOCKSIZEY * s_fMapScale > pos.y)))
		{
			s_aMap[i].tex = tex;

			int X = (s_aMap[i].tex % X_MAP);
			int Y = (s_aMap[i].tex / X_MAP);

			//�e�N�X�`���̍��W�ݒ�
			SetTex2d(pVtx, (1.0f / X_MAP) * X, (1.0f / X_MAP) * (X + 1.0f), (1.0f / Y_MAP) * Y, (1.0f / Y_MAP) * (Y + 1.0f));
		}
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffMap->Unlock();
}
//==================
//�}�b�v�̓����蔻��
//==================
bool CollisionMap(D3DXVECTOR3 pos)
{
	bool Hit = false;//�`�b�v�ƃ}�E�X�̓����蔻�肪�������Ă鎞
	D3DXVECTOR3 mapPos;

	for (int i = 0; i < NUM_MAP; i++)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		mapPos = (s_aMap[i].pos + s_PosOffset)* s_fMapScale;

		if (((mapPos.x < pos.x) && (mapPos.x + BLOCKSIZEX * s_fMapScale > pos.x)) &&
			((mapPos.y < pos.y) && (mapPos.y + BLOCKSIZEY * s_fMapScale > pos.y)))
		{

			Hit = true;
		}
	}
	return Hit;
}
//==================
//�T�C�Y�ύX
//==================
void SizeMap(float SIZ)
{
	s_fMapScale = 1.0f / SIZ;
}

//==================
//�}�b�v�̎擾
//==================
Map *GetMap(void)
{
	return s_aMap;
}

//==================
//�R���e����
//==================
void ConteSet(void)
{
	s_PosOffset.y = 0.0f;
	// �}�b�v�̐ݒ�B
	InitMapSet(&s_aMapFile[stage][0]);
	SetEnemyLynk(stage);

}

//==================
//file�p�X�ǂݍ���
//==================
void PasSetMap(char *Filename)
{
	//�t�@�C�����J��
	Enemy *Enemy = GetEnemy();
	FILE *pFile = fopen(&Filename[0], "r");
	char	s_aString[256];//�t�@�C���̕��������
	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		int number = 0;
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���
			fscanf(pFile, "%s", &s_aString[0]);

			if (strcmp(&s_aString[0], "MAPLYNK") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%s", &s_aMapFile[number][0]);
				number++;
			}
			if (strcmp(&s_aString[0], "ENEMYLYNK") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%s", &s_aString[0]);
				EnemyLynk(&s_aString[0]);
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}