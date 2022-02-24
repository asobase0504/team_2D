//========================================================
//
//e
//Author:ûüì]
//
//========================================================
#include "enemybullet.h"
#include "enemy.h"
#include "player.h"
//====================================
//}Nè`
//====================================
#define MAX_ENEMYBULLET		128		//eÌÅå
#define SIZE_BULLET			15.0f	//eÌå«³
//====================================
//O[oÏ
//====================================
static LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//eNX`ÖÌ|C^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//¸_obt@ÖÌ|C^
static EnemyBullet g_aBullet[MAX_ENEMYBULLET];		//eÌîñ
//====================================
//CÖ
//====================================

//====================================
//eÌú»
//====================================
void InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//foCXÌæ¾
	pDevice = GetDevice();

	//eNX`ÌÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet002.png",
		&g_pTextureBullet);

	//eÌîñÌú»
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;		//gpµÄ¢È¢óÔÉ·é
		g_aBullet[nCntBullet].bChase = true;	//gpµÄ¢éóÔÉ·é
	}

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMYBULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D * pVtx;		//¸_îñÖÌ|C^
	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		//¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
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
	g_pVtxBuffBullet->Unlock();
}
//====================================
//eÌI¹
//====================================
void UninitEnemyBullet(void)
{
	//eNX`Ìjü
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//¸_obt@Ìjü
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//====================================
//eÌXV
//====================================
void UpdateEnemyBullet(void)
{
	int nCntBullet;
	Player * pPlayer = GetPlayer();
	enemy * pEnemy = GetEnemy();
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//eªgp³êÄ¢é
		 //eÌÊuXV
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			//¸_ÀWÌXV
			VERTEX_2D * pVtx;		//¸_îñÖÌ|C^

			//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntBullet * 4);		//|C^ð4Âªißé
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			//¸_obt@ðAbN·é
			g_pVtxBuffBullet->Unlock();

			if (g_aBullet[nCntBullet].pos.x <= -SIZE_BULLET || g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH + SIZE_BULLET || g_aBullet[nCntBullet].pos.y <= -SIZE_BULLET || g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT + SIZE_BULLET)
			{//eªæÊOÉo½
				g_aBullet[nCntBullet].bUse = false;	//gpµÄ¢È¢óÔÉ·é
			}
		}
	}
}
//====================================
//eÌ`æ
//====================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//foCXÖÌ|C^
	int nCntBullet;

	//foCXÌæ¾
	pDevice = GetDevice();
	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));
	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);
	//eNX`ÌÝè
	pDevice->SetTexture(0, g_pTextureBullet);
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//eªgp³êÄ¢é
		 //|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);	//v~eBu(|S)
		}
	}
}
//====================================
//eÌÝè
//====================================
void SetEnemyBullet(D3DXVECTOR3 pos)
{
	Player * pPlayer = GetPlayer();

	VERTEX_2D * pVtx;		//¸_îñÖÌ|C^
	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//eªgp³êÄ¢È¢
			g_aBullet[nCntBullet].pos = pos;
			pVtx += (nCntBullet * 4);		//YÌÊuÜÅißé
			//¸_ÀWÌXV
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			g_aBullet[nCntBullet].nLife = 0;
			g_aBullet[nCntBullet].bChase = true;		//gpµÄ¢éóÔ
			g_aBullet[nCntBullet].bUse = true;		//gpµÄ¢éóÔ
			if (g_aBullet[nCntBullet].bChase)
			{
				//vC[Éü©ÁÄòÔ
				float moveBulletX = pPlayer->pos.x - g_aBullet[nCntBullet].pos.x;
				float moveBulletY = pPlayer->pos.y - g_aBullet[nCntBullet].pos.y;
				float moveBulletR = sqrtf(moveBulletX * moveBulletX + moveBulletY * moveBulletY);
				g_aBullet[nCntBullet].move.x = (moveBulletX / moveBulletR) * 3.0f;
				g_aBullet[nCntBullet].move.y = (moveBulletY / moveBulletR) * 3.0f;
				g_aBullet[nCntBullet].bChase = false;
			}
			break;
		}
	}
	//¸_obt@ðAbN·é
	g_pVtxBuffBullet->Unlock();
}