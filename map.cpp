//============================
//
// �}�b�v�`�b�v�ݒ�
// Author:hamada ryuuga
//
//============================
#include "map.h"
#include "input.h"
#include "enemy.h"
#include "common.h"
#include <stdio.h>

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTextureMap = NULL; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffMap = NULL; //���_�o�b�t�@�̐ݒ�
static Map s_aMap[NUM_MAP];
static D3DXVECTOR3 s_Move(0.0f, MAPMOVE, 0.0f);
static D3DXVECTOR3 s_PosOffset;
static float s_fMapScale;
static int s_stage = 0;

//================
//����������
//================
void InitMap(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/mapchip_field2.png",
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
	PasSetMap("Data\\hoge4.txt");
	s_PosOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_fMapScale = 1.0f;
	s_stage = 0;
}

//================
///�j��
//================
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

//==================
//�X�V����
//==================
void UpdateMap(void)
{
	{
		s_PosOffset += s_Move;

		if (s_PosOffset.y >= (BLOCKSIZEY*MAP_SIZEY) - (SCREEN_HEIGHT)-BLOCKSIZEY)
		{//�������[�v����
			falseSetEnemy();
			falseSetMap();
			s_PosOffset.y = 0.0f;
			s_stage++;
			s_stage %= 5;
			// �}�b�v�̐ݒ�B
			ConteSet(s_stage);
		}

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

		SetNormalpos2d(pVtx,
			pos.x,
			pos.x + BLOCKSIZEX * s_fMapScale,
			pos.y,
			pos.y + BLOCKSIZEY * s_fMapScale);
		//if (!IsDebug())
		{
			if (pos.y >= SCREEN_HEIGHT)
			{
				s_aMap[i].bUse = false;
			}
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
	
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);//�������̊g��
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);//�傫���̏k��
		
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			4 * i,
			2);
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);//�������̊g��
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);//�傫���̏k��

	}
}

//==================
//�}�b�v�f�[�^�̐ݒ�(�������W�^�C�v�e�N�X�`���̉��Ԗڂ��g����)
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
		float a = BLOCKSIZEY * 10.3f;
		for (int nCntX = 0; nCntX < MAP_SIZEX; nCntX++)
		{//Map�̏�������
			SetMap(D3DXVECTOR3(BLOCKSIZEX*nCntX, (-(BLOCKSIZEY*nCntY) + a), 0.0f), 1, aMap[nCntY][nCntX]);
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
void ConteSet(int stage)
{
	s_PosOffset.y = 0.0f;
	// �}�b�v�̐ݒ�B
	falseSetEnemy();
	falseSetMap();
	InitMapSet(&s_aMap[stage].filename[0]);
	SetEnemyLynk(stage);

}
//==================
//STAGE�ԍ��̏��擾
//==================
int GetStage(void)
{
	return s_stage;
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
				fscanf(pFile, "%s", &s_aMap[number].filename[0]);
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
int sopiteMap(D3DXVECTOR3 pos)
{
	int Hit = 0;//�`�b�v�ƃ}�E�X�̓����蔻�肪�������Ă鎞
	D3DXVECTOR3 mapPos;

	for (int i = 0; i < NUM_MAP; i++)
	{
		if (!s_aMap[i].bUse)
		{
			continue;
		}

		mapPos = (s_aMap[i].pos + s_PosOffset)* s_fMapScale;

		if (((mapPos.x < pos.x) && (mapPos.x + MAP_SIZEY * s_fMapScale > pos.x)) &&
			((mapPos.y < pos.y) && (mapPos.y + MAP_SIZEY * s_fMapScale > pos.y)))
		{

			Hit = s_aMap[i].tex;
		}
	}
	return Hit;
}
void falseSetMap(void)
{
	for (int i = 0; i < NUM_MAP; i++)
	{
		s_aMap[i].bUse = false;
	}
}