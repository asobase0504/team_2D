//========================================================
//
//SPtbO
//Author:ûüì]
//
//========================================================
#include "flag.h"
#include "player.h"
#include "bullet.h"
//====================================
//}Nè`
//====================================
#define MAX_FLAG		(128)		//SPtbOÌÅå
#define NUM_FLAG		(2)			//SPtbOÌíÞ
//====================================
//O[oÏ
//====================================
LPDIRECT3DTEXTURE9 g_pTextureFlag[NUM_FLAG] = {};	//eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFlag = NULL;		//¸_obt@ÖÌ|C^
Flag g_aFlag[MAX_FLAG];								//SPtbOÌîñ
//====================================
//SPtbOÌú»
//====================================
void InitFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntFlag;

	//foCXÌæ¾
	pDevice = GetDevice();
	//eNX`ÌÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag001.png",
		&g_pTextureFlag[0]);
	//eNX`ÌÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag000.png",
		&g_pTextureFlag[1]);
	//SPtbOÌîñÌú»
	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		g_aFlag[nCntFlag].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].state = FLAG_STATE_OFF;
		g_aFlag[nCntFlag].fHeight = 0.0f;
		g_aFlag[nCntFlag].fWidth = 0.0f;
		g_aFlag[nCntFlag].fAngle = 0.0f;
		g_aFlag[nCntFlag].fLength = 0.0f;
		g_aFlag[nCntFlag].nType = 0;
		g_aFlag[nCntFlag].bUse = false;	//gpµÄ¢È¢óÔÉ·é
	}

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FLAG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFlag,
		NULL);

	VERTEX_2D * pVtx;		//¸_îñÖÌ|C^
							//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		//¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		//rhwÌÝè
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//¸_J[ÌÝè
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//eNX`ÀWÌÝè
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//¸_f[^Ì|C^ð4Âªißé
	}
	//¸_obt@ðAbN·é
	g_pVtxBuffFlag->Unlock();
}
//====================================
//SPtbOÌI¹
//====================================
void UninitFlag(void)
{
	//eNX`Ìjü
	if (g_pTextureFlag[NUM_FLAG] != NULL)
	{
		g_pTextureFlag[NUM_FLAG]->Release();
		g_pTextureFlag[NUM_FLAG] = NULL;
	}
	//¸_obt@Ìjü
	if (g_pVtxBuffFlag != NULL)
	{
		g_pVtxBuffFlag->Release();
		g_pVtxBuffFlag = NULL;
	}
}
//====================================
//SPtbOÌXV
//====================================
void UpdateFlag(void)
{

		VERTEX_2D * pVtx;		//¸_îñÖÌ|C^
		//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
		g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
		{
			if (g_aFlag[nCntFlag].nType == 0)
			{
				//¸_J[ÌÝè
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

				pVtx += 4;	//¸_f[^Ì|C^ð4Âªißé
			}
			else
			{
				//¸_J[ÌÝè
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx += 4;	//¸_f[^Ì|C^ð4Âªißé
			}
		}
		//¸_obt@ðAbN·é
		g_pVtxBuffFlag->Unlock();
}
//====================================
//SPtbOÌ`æ
//====================================
void DrawFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//foCXÖÌ|C^
	int nCntFlag;

	//foCXÌæ¾
	pDevice = GetDevice();
	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, g_pVtxBuffFlag, 0, sizeof(VERTEX_2D));
	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == true)
		{//SPtbOªgp³êÄ¢é
		 //eNX`ÌÝè
			pDevice->SetTexture(0, g_pTextureFlag[g_aFlag[nCntFlag].nType]);
			//|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFlag * 4, 2);	//v~eBu(|S)
		}
	}
}
//====================================
//SPtbOÌí
//====================================
void DeleteFlag(int nCntFlag)
{
	//SPtbOÌñ\¦
	g_aFlag[nCntFlag].bUse = false;
}
//====================================
//SPtbOÌÝè
//====================================
void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	int nCntFlag;
	VERTEX_2D * pVtx;		//¸_îñÖÌ|C^
							//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == false)
		{//SPtbOªgp³êÄ¢È¢
			g_aFlag[nCntFlag].pos = pos;
			g_aFlag[nCntFlag].fWidth = fWidth;
			g_aFlag[nCntFlag].fHeight = fHeight;
			g_aFlag[nCntFlag].nType = nType;
			g_aFlag[nCntFlag].state = FLAG_STATE_OFF;
			//¸_ÀWÌXV
			pVtx[0].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);

			g_aFlag[nCntFlag].bUse = true;		//gpµÄ¢éóÔ
			break;
		}
		pVtx += 4;
	}
	//¸_obt@ðAbN·é
	g_pVtxBuffFlag->Unlock();
}
//====================================
//SPtbOîñÌæ¾
//====================================
Flag * GetFlag(void)
{
	return &g_aFlag[0];
}
//====================================
//SPtbOÌ½è»è(ep)
//====================================
void CollisionFlagBullet(D3DXVECTOR3 pPos, float fWidth, float fHeight)
{
	Bullet *pBullet = GetBullet();
	Flag *pFlag = GetFlag();
	float RightPlayer = pPos.x + fWidth / 2;
	float LeftPlayer = pPos.x - fWidth / 2;
	float BottomPlayer = pPos.y;
	float TopPlayer = pPos.y - fHeight;
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		float  RightFlag = g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2;
		float  LeftFlag = g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2;
		float  BottomFlag = g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2;
		float  TopFlag = g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2;
		if (g_aFlag[nCntFlag].bUse == true)
		{
			if (RightFlag >= LeftPlayer&& LeftFlag <= RightPlayer && BottomFlag >= TopPlayer && TopFlag <= BottomPlayer)
			{
				if (g_aFlag[nCntFlag].nType == 0)
				{//eÆÌ½è»è
					g_aFlag[nCntFlag].nType = 1;
					pBullet->bUse = false;
				}
			}
		}
		pFlag++;
	}
}
//====================================
//SPtbOÌ½è»è(vC[)
//====================================
void CollisionFlagPlayer(D3DXVECTOR3 pPos, float fWidth, float fHeight)
{
	Player *pPlayer = GetPlayer();
	Flag * pFlag = GetFlag();
	float RightPlayer = pPos.x + fWidth / 2;
	float LeftPlayer = pPos.x - fWidth / 2;
	float BottomPlayer = pPos.y;
	float TopPlayer = pPos.y - fHeight;
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		float  RightFlag = g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2;
		float  LeftFlag = g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2;
		float  BottomFlag = g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2;
		float  TopFlag = g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2;
		if (g_aFlag[nCntFlag].bUse == true)
		{
			if (RightFlag >= LeftPlayer&& LeftFlag <= RightPlayer && BottomFlag >= TopPlayer && TopFlag <= BottomPlayer)
			{
				if (g_aFlag[nCntFlag].nType == 1)
				{//vC[ÆÌ½è»è
					pPlayer->nLife++;
					DeleteFlag(nCntFlag);
				}
			}
		}
		pFlag++;
	}
}