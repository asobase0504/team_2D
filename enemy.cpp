//==================================================
// 
// G
// Author : Isoe Jukia
// Author : ûüì]
// 
//==================================================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "Calculation.h"
#include "common.h"
#include "collision.h"
#include "score.h"
#include "player.h"
#include "Boss.h"
#include "target.h"
#include "crater.h"
#include "sound.h"
#include "life.h"
#include <stdio.h>

//----------------------------
// }N
//----------------------------
#define MOVE_SPEED			(2.5f)
#define COLLISION			(25)
#define JUDGEMENT			(100)		// ð¯éGÌ»èTCY
#define UP_ESCAPE			(200)		// ãÉ¦°éGÌ»èTCY
#define ABILTIY				(0.005f)
#define MAXSTAGE			(5)			// Xe[W
#define BULLET_INTERVAL		(180)		// UÔu

//VIiCg
#define SHEO_DIST_START_ROTATION	(200.0f)							// ñènßé£
#define SHEO_RADIUS_ROTATION		(SHEO_DIST_START_ROTATION - 20.0f)	// ñ]¼a
#define SHEO_SPEED_ROTATION			(D3DXToRadian(6.0f))				// ñ]¬x
#define SHEO_FRAME_GOODBY			(300)								// ¨AèÉÈéÔ
#define SHEO_SPEED_GOODBY			(10.0f)								// ¨AèÉÈé¬x

//**************************************************
// X^eBbNÏ
//**************************************************
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};	//eNX`ÖÌ|C^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//¸_obt@ÖÌ|C^
static Enemy s_aEnemy[MAX_ENEMY];					// GÌîñ
static Enemy s_aTypeEnemy[ENEMYTYPE_MAX];
static char EnemyLink[MAXSTAGE][256];
static int count;

//**************************************************
// vg^Cvé¾
//**************************************************
static void TrackingMove(Enemy* pEnemy);	// Çö
static void ReflectMove(Enemy* pEnemy);
static void UpdateSky1(Enemy* pEnemy);		// óÌG1ÌXV
static void UpdateSky2(Enemy* pEnemy);		// óÌG2ÌXV
static void UpdateBuckSky(Enemy* pEnemy);	// AéóÌGÌXV
static void MoveBakyura(Enemy* pEnemy);		// oLÌÚ®
static void MoveSheonite(Enemy* pEnemy);	// VIiCgÌÚ®
static void UpdateZakato1(Enemy* pEnemy);	// TOGETOGE
static void UpdateZakato2(Enemy* pEnemy);	// UJ[gÌ2(ÇöiVAêèÌÔuÅeð­ËµÄÁÅ)
static void UpdateZakato3(Enemy* pEnemy);	// UJ[gÌ3(ÇöAAÔoßÅeð­ËµÄÁÅ)
static void UpdateZakato4(Enemy* pEnemy);	// UJ[gÌ4(ÇöiVAÔoßÅeð­ËµÄÁÅ)
static void Updateflag(Enemy* pEnemy);		// tbNÌ
static void ZakatoFinish(Enemy* pEnemy);	// UJ[gªI¹·éÛÌ
static void ShotBullet(Enemy* pEnemy, int Interval);		// eð¿¾·

//--------------------------------------------------
// ú»
// AutherFIsoe Jukia
//--------------------------------------------------
void InitEnemy(void)
{
	// ú»
	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));
	count = 0;

	// ¸_obt@Ì¶¬
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx;			// ¸_îñÖÌ|C^
	Enemy* pEnemy = s_aEnemy;	// Gl~[ð|C^»

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++, pVtx += 4)
	{
		// ¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fSize, pEnemy->pos.y - pEnemy->fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fSize, pEnemy->pos.y - pEnemy->fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fSize, pEnemy->pos.y + pEnemy->fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fSize, pEnemy->pos.y + pEnemy->fSize, 0.0f);

		// rhwÌÝè
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ¸_J[ÌÝè
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

		// eNX`ÌÀWÝè
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ¸_obt@ðAbN·é
	s_pVtxBuff->Unlock();

	//eNX`f[^ÇÝÝ
	LoadSetFile("data\\txt\\enemy.txt");
}

//--------------------------------------------------
// I¹
// AutherFIsoe Jukia
//--------------------------------------------------
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		if (s_pTex[nCntEnemy] != NULL)
		{// eNX`Ìjü
			s_pTex[nCntEnemy]->Release();
			s_pTex[nCntEnemy] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{// ¸_obt@Ìjü
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//
// XV
//
// AutherFIsoe Jukia
// AutherFTakano Keisuke
// AutherFYuda Kaito
// AutherFTomidokoro Tomoki
// AutherFBuriya Kouta
//
//--------------------------------------------------
void UpdateEnemy(void)
{
	VERTEX_2D *pVtx;			//¸_îñÖÌ|C^
	Enemy* pEnemy = s_aEnemy;

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (!pEnemy->bUse)
		{
			continue;
		}

		// ÊuðXV
		pEnemy->pos.y += pEnemy->fSpeed;

		if (pEnemy->pos.y >= 0.0f - pEnemy->fSize)
		{
			pEnemy->pos += pEnemy->move;

			// Gl~[Ì^Cv²ÆÌ
			switch (pEnemy->nType)
			{
			case ENEMYTYPE_SKY_1:
				UpdateSky1(pEnemy);
				break;
			case ENEMYTYPE_SKY_2:
				UpdateSky2(pEnemy);
				break;
			case ENEMYTYPE_SKY_3:		// ø«ÔµG
				UpdateBuckSky(pEnemy);
				break;
			case ENEMYTYPE_BAKYURA:		// ³GÇ
				MoveBakyura(pEnemy);
				break;
			case ENEMYTYPE_SHEONITE:	// óÌG
				MoveSheonite(pEnemy);
				break;
			case ENEMYTYPE_GROUND_1:	// ¼iG
				pEnemy->nCntBullet++;

				ShotBullet(pEnemy, BULLET_INTERVAL);
				break;
			case ENEMYTYPE_GROUND_2:	// ®©È¢G

				break;
			case ENEMYTYPE_WARP_1:		//ÇöAAêèÌ£Åeð­ËµAÁÅ	
				UpdateZakato1(pEnemy);
				break;
			case ENEMYTYPE_WARP_2:
				UpdateZakato2(pEnemy);
				break;
			case ENEMYTYPE_WARP_3:		//ÇöAAÔoßÅeð­ËµÄÁÅ
				UpdateZakato3(pEnemy);
				break;
			case ENEMYTYPE_WARP_4:		//ÇöiVAÔoßÅeð­ËµÄÁÅ
				UpdateZakato4(pEnemy);
				break;
			case  FLAG_STATE:			//tO
				Updateflag(pEnemy);
				if (pEnemy->FlagOn)
				{
					// ¸_J[ÌÝè
					SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
				}
				else
				{
					// ¸_J[ÌÝè
					SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));
				}
				break;
			default:
				break;
			}

			// eª½Á½
			Bullet* pBullet = GetBullet();
			for (int i = 0; i < MAX_BULLET; i++, pBullet++)
			{
				if (!pBullet->bUse || pBullet->BulletType == BULLETTYPE_ENEMY)
				{
					continue;
				}

				if ((pBullet->BulletType == BULLETTYPE_PLAYER_GROUND && !pEnemy->bIsSkyType)
					|| (pBullet->BulletType == BULLETTYPE_PLAYER_SKY && pEnemy->bIsSkyType))
				{// eÌíÞÆGÌíÞªêvµÄ¢é
					if (CollisionCircle(pEnemy->pos, COLLISION, pBullet->pos, pBullet->size.x))
					{
						AddScore(255);
						HitEnemy(pEnemy, 1);
						pBullet->bUse = false;
					}
				}
			}

			//  éöxißÎ©®ÅÇöðØé
			if (pEnemy->pos.y > 720.0f + pEnemy->fSize + 200.0f)
			{
				pEnemy->bTracking = false;
			}

			// ãÉAéGðÁ·
			if (pEnemy->pos.y < 0.0f - pEnemy->fSize && pEnemy->bBack)
			{
				pEnemy->bUse = false;
			}

			// Gl~[ÌCtª0ÉÈÁ½Æ«
			if (pEnemy->nLife <= 0)
			{
				pEnemy->bUse = false;
			}
		}

		// ¸_ÀWÌÝè
		SetVtxPos(pVtx, &pEnemy->pos, pEnemy->fSize, pEnemy->fSize);

		// eNX`ÌÝè
		SetVtxAnimTex(pVtx, &pEnemy->AnimTex);
		SetTex2d(pVtx, (1.0f / pEnemy->AnimTex.nDivisionX)*pEnemy->AnimTex.nPatternX
			, (1.0f / pEnemy->AnimTex.nDivisionX)* (pEnemy->AnimTex.nPatternX + 1)
			, 1.0f / pEnemy->AnimTex.nDivisionY*pEnemy->AnimTex.nPatternY
			, 1.0f / pEnemy->AnimTex.nDivisionY*(pEnemy->AnimTex.nPatternY + 1));
	}
	// ¸_obt@ðAbN·é
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// XJC1ÌGÌXV
// AutherFIsoe Jukia
// AutherFYuda Kaito
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	// ÇöÌ
	TrackingMove(pEnemy);

	// ½ËÌ
	ReflectMove(pEnemy);

	// eðo·
	ShotBullet(pEnemy, 120);
}

//--------------------------------------------------
// XJC2ÌGÌXV
// AutherFIsoe Jukia
// AutherFYuda Kaito
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
	// ÇöÌ
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, GetPlayer()->pos, JUDGEMENT))
	{// Gª¦°é
		pEnemy->bTracking = false;
		pEnemy->move.y = 0;		// RgAEgðÁ·Æ¼pÉÈªé
		pEnemy->move.x = 3;
	}

	// eðo·
	ShotBullet(pEnemy, 120);
}

//--------------------------------------------------
// XJC3ÌGÌXV
// AutherFIsoe Jukia
// AutherFYuda Kaito
//--------------------------------------------------
void UpdateBuckSky(Enemy* pEnemy)
{
	//ßé
	if (pEnemy->bBack)
	{
		pEnemy->move.y += easeInOutBack(-0.25f);
	}
	// ÇöÌ
	TrackingMove(pEnemy);

	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	if (CollisionCircle(pEnemy->pos, 100, pPlayer->pos, pPlayer->size.x))
	{// Gª¦°é
		pEnemy->bTracking = false;		//Çö
		pEnemy->bBack = true;		//ßé
	}

	// eðo·
	ShotBullet(pEnemy, 120);
}

//--------------------------------------------------
// oLÌ®«
// Author: Buriya Kota
//--------------------------------------------------
void MoveBakyura(Enemy* pEnemy)
{
	// ½©ð­Èç±¿çÉ
}

//--------------------------------------------------
// VIiCgÌ®«
// Author: Buriya Kota
//--------------------------------------------------
void MoveSheonite(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();
	// ÇöÌ
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, 0.0f, pPlayer->pos, SHEO_DIST_START_ROTATION))
	{	// ÍÍÉüÁ½çñé
		pEnemy->bTracking = false;

		if (pEnemy->fSheoRot < 0.0f)
		{	// úÌpx
			D3DXVECTOR3 vecEnemy = pEnemy->pos - pPlayer->pos;
			pEnemy->fSheoRot = atan2f(vecEnemy.x, vecEnemy.y);
		}
		// ­µ¸Âñ·
		pEnemy->fSheoRot += SHEO_SPEED_ROTATION;
		float distance = SHEO_RADIUS_ROTATION;

		pEnemy->nSheoCnt++;
		if (pEnemy->nSheoCnt > SHEO_FRAME_GOODBY)
		{	// VIiCgÌAÒ
			distance = SHEO_DIST_START_ROTATION;
			pEnemy->move.x = sinf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
			pEnemy->move.y = cosf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
		}
		// vC[ÌÊu©çVIiCgÌÊuðZo
		pEnemy->pos.x = pPlayer->pos.x + distance * sinf(pEnemy->fSheoRot);
		pEnemy->pos.y = pPlayer->pos.y + distance * cosf(pEnemy->fSheoRot);
	}
}

//-------------------------------------------------
//	UJ[g1ÌGÌXV
// AuthorFTomidokoro Tomoki
//-------------------------------------------------
void UpdateZakato1(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();


	float fRotMove, fRotDest, fRotDiff;

	fRotMove = atan2f(pEnemy->move.x, pEnemy->move.y);			//»ÝÌÚ®ûü

	fRotDest = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);			//ÚIÌÚ®ûü

	fRotDiff = fRotDest - fRotMove; 	//ÚIÌÚ®ûüÜÅÌ·ª

	//pxÌlðC³·é
	if (fRotDiff > D3DX_PI)
	{//ÚIÌÚ®ûüÖÌ·ªª3.14æèãÌê
		fRotDiff -= D3DX_PI * 2;
	}

	else if (fRotDiff < -D3DX_PI)
	{//ÚIÌÚ®ûüÖÌ·ªª3.14æèºÌê
		fRotDiff += D3DX_PI * 2;
	}

	fRotMove += fRotDiff * ABILTIY;				//Ú®ûüÌâ³

	//move C³
	if (fRotMove > D3DX_PI)
	{//»ÝÌÚ®ûüÌpxª3.14ð´¦½ê
		fRotMove -= D3DX_PI * 2;
	}

	else if (fRotMove < -D3DX_PI)
	{//»ÝÌÚ®ûüÌpxª3.14æèºÌê
		fRotMove += D3DX_PI * 2;
	}

	pEnemy->move.x = sinf(fRotMove) * MOVE_SPEED;
	pEnemy->move.y = cosf(fRotMove) * MOVE_SPEED;

	pEnemy->bTracking = false;

	// eðo·
	ShotBullet(pEnemy, 120);
}

//-------------------------------------------------
//	UJ[g2ÌGÌXV
//-------------------------------------------------
void UpdateZakato2(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	//ÇöiVAêèÌ£Åeð­ËµAÁÅ
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2, 0.0f);
		pEnemy->bTracking = false;		//Çö
		pEnemy->bTP = true;
	}

	if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
		&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
		&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
		&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
	{//GªI¹·é
		ZakatoFinish(pEnemy);
	}
}

//-------------------------------------------------
//	UJ[g3ÌGÌXV
//-------------------------------------------------
void UpdateZakato3(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	//ÇöAAêèÌ£Åeð­ËµAÁÅ
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
		pEnemy->bTP = true;
	}
	if (pEnemy->nCntBullet >= 110)
	{// GªÁ¦é
		ZakatoFinish(pEnemy);
	}
}

//-------------------------------------------------
//	UJ[g4ÌGÌXV
//-------------------------------------------------
void UpdateZakato4(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	//ÇöiVAÔoßÅeð­ËµÄÁÅ
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
		pEnemy->bTracking = false;		//Çö
		pEnemy->bTP = true;
	}
	if (pEnemy->nCntBullet >= 110)
	{// GªÁ¦é
		ZakatoFinish(pEnemy);
	}
}

//=============================================
// UJ[g(e|[g·éG)ÌI¹·éÛÌ
//=============================================
void ZakatoFinish(Enemy* pEnemy)
{
	// Gl~[©çvC[ÜÅÌ£ÌZo
	D3DXVECTOR3 Direction = pEnemy->pos - GetPlayer()->pos;

	//ÎpüÌpxðZo
	Direction.z = atan2f(Direction.x, Direction.y);
	Direction.x = 0.0f;		// gíÈ¢îñÈÌÅú»
	Direction.y = 0.0f;		// gíÈ¢îñÈÌÅú»

	//eÌ­Ë
	SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
	pEnemy->nCntBullet = 0;
	pEnemy->bTracking = false;		//Çö
	pEnemy->bTP = false;
	pEnemy->bUse = false;			//Gl~[ð³ø»·é
}

//--------------------------------------------------
// eð¿¾·
// AuthorFYuda Kaito
//
// ø
// Enemy* pEnemy	lðÏX·éGl~[
// int Interval		UÌÔu
//--------------------------------------------------
void ShotBullet(Enemy* pEnemy , int Interval)
{
	if (pEnemy->pos.y < SCREEN_HEIGHT || pEnemy->pos.x < 0.0f || pEnemy->pos.x > SCREEN_WIDTH)
	{	// æÊàÉûÜÁÄ¢éê

		pEnemy->nCntBullet++;

		if (pEnemy->nCntBullet >= Interval)
		{
			// Gl~[©çvC[ÜÅÌ£ÌZo
			D3DXVECTOR3 Direction = pEnemy->pos - GetPlayer()->pos;

			//ÎpüÌpxðZo
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// gíÈ¢îñÈÌÅú»
			Direction.y = 0.0f;		// gíÈ¢îñÈÌÅú»

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// `æ
// AuthorFIsoe Jukia
//--------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// foCXÌ|C^

	// ¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// eNX`ÌÝè
		pDevice->SetTexture(0, s_pTex[s_aEnemy[nCntEnemy].nType]);

		if (!s_aEnemy[nCntEnemy].bUse)
		{
			continue;
		}

		// |SÌ`æ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntEnemy * 4,2);
	}
}

//--------------------------------------------------
// Ýè
// AuthorFIsoe Jukia
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, float fSize, ENEMYTYPE nType)
{
	VERTEX_2D *pVtx;			// ¸_îñÖÌ|C^
	Enemy* pEnemy = s_aEnemy;

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse)
		{
			continue;
		}

		// Gl~[Éîñð½¹é
		*pEnemy = s_aTypeEnemy[nType];

		pEnemy->bUse = true;	// gpµÄ¢éóÔÉÚs

		// øÌÝè
		pEnemy->bTracking = true;
		pEnemy->bTracking = s_aTypeEnemy[nType].bTracking;
		pEnemy->pos = pos;
		pEnemy->fSize = fSize;
		pEnemy->nType = nType;
		pEnemy->fSpeed = s_aTypeEnemy[nType].fSpeed;
		//pEnemy->move.y = saTypeEnemy[nType].fSpeed;
		pEnemy->FlagOn = true;

		switch (pEnemy->nType)
		{
		case ENEMYTYPE_SHEONITE:
			pEnemy->fSheoRot = -1.0f;
			pEnemy->nSheoCnt = 0;
			pEnemy->nLife = 30;
			break;
		case ENEMYTYPE_SKY_1:
			pEnemy->bReflect = true;
			break;
		case FLAG_STATE:
			pEnemy->FlagOn = false;
		default:
			break;
		}

		switch (pEnemy->nType)
		{
		case ENEMYTYPE_SKY_1:
		case ENEMYTYPE_SKY_2:
		case ENEMYTYPE_SKY_3:
		case ENEMYTYPE_SHEONITE:
		case ENEMYTYPE_BAKYURA:
		case ENEMYTYPE_WARP_1:
		case ENEMYTYPE_WARP_2:
		case ENEMYTYPE_WARP_3:
		case ENEMYTYPE_WARP_4:
		case ENEMYTYPE_BOSS:
			pEnemy->bIsSkyType = true;
			break;
		case ENEMYTYPE_GROUND_1:
		case ENEMYTYPE_GROUND_2:
		case FLAG_STATE:
			pEnemy->bIsSkyType = false;
			break;
		default:
			break;
		}

		// ¸_ÀWÌÝè
		SetVtxPos(pVtx, &pEnemy->pos, pEnemy->fSize, pEnemy->fSize);
		// ¸_J[ÌÝè
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		break;
	}

	// ¸_ðAbN·é
	s_pVtxBuff->Unlock();

	return pEnemy;
}

//--------------------------------------------------
// ½è
// AutherFIsoe Jukia
//--------------------------------------------------
void HitEnemy(Enemy* pEnemy, int nDamage)
{
	pEnemy->nLife -= nDamage;

	//
	// _[WÌSE
	//
}

//--------------------------------------------------
// Çö
// AutherFIsoe Jukia
// AutherFYuda Kaito
//--------------------------------------------------
void TrackingMove(Enemy * pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	if (pEnemy->bTracking)
	{
		float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;
		float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;
		float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);
		pEnemy->move.x = (moveEnemyX / moveEnemyR) * pEnemy->fSpeed;
		pEnemy->move.y = (moveEnemyY / moveEnemyR) * pEnemy->fSpeed;
		if (pEnemy->pos.y >= pPlayer->pos.y)
		{
			pEnemy->bTracking = false;
		}
	}
}

//--------------------------------------------------
// æ¾
// AutherFIsoe Jukia
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return s_aEnemy;
}

//----------------------------
//t@CÌüÍ}bvîñ
// AuthorFHamada Ryuga
//----------------------------
void LoadSetFile(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;
	int     MoveSet = 0;

	// t@C|C^Ìé¾
	FILE* pFile;

	//t@CðJ­
	pFile = fopen(Filename, "r");
	int nCntEnemy = 0;

	if (pFile != NULL)
	{//t@CªJ¢½ê
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//X^[géÜÅóÇÝÞ
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ¶ñÌú»ÆÇÝÝ// ¶ñÌú»ÆÇÝÝ

			fscanf(pFile, "%s", &s_aString[0]);

			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//±êÌ ÆRg
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "NUM_TEXTURE") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ

				fscanf(pFile, "%d", &Num_Tex);

			}
			if (strcmp(&s_aString[0], "DIVISION") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionX);
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionY);
			}
			if (strcmp(&s_aString[0], "ANIMESPEED") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nSpeed);
			}
			if (strcmp(&s_aString[0], "MOVESPEED") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
				fscanf(pFile, "%f", &s_aTypeEnemy[nCntEnemy].fSpeed);
			}
			if (strcmp(&s_aString[0], "TRACKING") == 0)
			{
				int checker = 0;
				fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
				fscanf(pFile, "%d", &checker);
				if (checker == 1)
				{
					s_aTypeEnemy[nCntEnemy].bTracking = true;
				}
				else
				{
					s_aTypeEnemy[nCntEnemy].bTracking = false;
				}
			}
			if (strcmp(&s_aString[0], "LIFE") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].nLife);
			}
			if (strcmp(&s_aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);	//ÇÝÞâÂ
				fscanf(pFile, "%s", &s_aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//foCXÌæ¾
				pDevice = GetDevice();
				//eNX`ÌÇÝÝ
				D3DXCreateTextureFromFile(pDevice,
					&s_aString[0],
					&s_pTex[(ENEMYTYPE)nCntEnemy]);
			}
			if (strcmp(&s_aString[0], "END_TEX") == 0)
			{
				nCntEnemy++;

			}

			if (strcmp(&s_aString[0], "TYPESTATE") == 0)
			{// ¶ñªêvµ½ê
				while (strncmp(&s_aString[0], "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ

					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						//fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
						//fscanf(pFile, "%f", &s_aTypeEnemy[MoveSet].fSpeed);
					}
					if (strcmp(&s_aString[0], "BACK") == 0)
					{
						int checker = 0;
						fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
						fscanf(pFile, "%d", &checker);
						if (checker == 1)
						{
							s_aTypeEnemy[MoveSet].bBack = true;
						}
						else
						{
							s_aTypeEnemy[MoveSet].bBack = false;
						}

					}
					if (strcmp(&s_aString[0], "TRACKING") == 0)
					{
						int checker = 0;
						fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
						fscanf(pFile, "%d", &checker);
						if (checker == 1)
						{
							s_aTypeEnemy[MoveSet].bTracking = true;
						}
						else
						{
							s_aTypeEnemy[MoveSet].bTracking = false;
						}
					}
					if (strcmp(&s_aString[0], "LIFE") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
						fscanf(pFile, "%d", &s_aTypeEnemy[MoveSet].nLife);
					}
					if (strcmp(&s_aString[0], "ENDSTATE") == 0)
					{
						MoveSet++;
					}
				}

			}
			if (strcmp(&s_aString[0], "SET") == 0)
			{// ¶ñªêvµ½ê
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int  nType = 0;
				float  fSize = 0.0f;
				nCntEnemy = 0;
				while (1)
				{//188 2900
					// ¶ñÌú»ÆÇÝÝ
					s_aString[0] = {};
					fscanf(pFile, "%s", &s_aString[0]);

					if (strncmp(&s_aString[0], "#", 1) == 0)
					{//±êÌ ÆRg
						fgets(&s_aString[0], sizeof(s_aString), pFile);
						continue;
					}

					if (strcmp(&s_aString[0], "POS") == 0)
					{// ¶ñªêvµ½ê

						fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&s_aString[0], "TYPE") == 0)
					{// ¶ñªêvµ½ê
						fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
						fscanf(pFile, "%d", &nType);
						if (nType >= (int)ENEMYTYPE_MAX)
						{//ñ^Ìæèå«©Á½çðú»·é
							nType = 0;
						}
					}
					if (strcmp(&s_aString[0], "SIZE") == 0)
					{// ¶ñªêvµ½ê
						fscanf(pFile, "%s", &s_aString[0]);//ÇÝÞâÂ
						fscanf(pFile, "%f", &fSize);
					}
					if (strcmp(&s_aString[0], "END_SET") == 0)
					{//Zbg
						SetEnemy(D3DXVECTOR3(s_modelpos),fSize,(ENEMYTYPE)nType);
						break;
					}
					if (strcmp(&s_aString[0], "END_SCRIPT") == 0)
					{// ¶ñªêvµ½ê
						break;
					}
				}
			}
		}
		//t@CðÂ¶é
		fclose(pFile);
	}
}

//--------------------
//wèµ½filepXðæÝ±Þ
//--------------------
void EnemyLynk(char *Filename)
{
	if (EnemyLink[count][0] == '\0')
	{
		strcat(&EnemyLink[count][0], &Filename[0]);//¬@aFile-±¢ÂÉüêé
		count++;
	}
}

//--------------------
//wèµ½filepXðÄÑo·
//--------------------
void SetEnemyLynk(int number)
{
	LoadSetFile(&EnemyLink[number][0]);
}

//--------------------
// ½è»è
//--------------------
void ReflectMove(Enemy* pEnemy)
{
	Player* pPlayer = GetPlayer();

	if (pEnemy->bReflect)
	{
		if (pEnemy->pos.x >= pPlayer->pos.x - pPlayer->size.x
			&& pEnemy->pos.x <= pPlayer->pos.x + pPlayer->size.x)
		{//GªI¹·é			
			pEnemy->bReflect = false;
			pEnemy->bTracking = false;
			pEnemy->move.x += -pEnemy->move.x * 3;
			//			pEnemy->move.y += -pEnemy->move.y * 3;
		}
	}
}

//--------------------
//GÁ·
// AuthorFHamada Ryuga
//--------------------
void falseSetEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!s_aEnemy[nCntEnemy].bUse)
		{
			continue;
		}
		s_aEnemy[nCntEnemy].bUse = false;
	}
}

//--------------------
//ø
// Author : ûüì]
// AuthorFHamada Ryuga
//--------------------
void Updateflag(Enemy* pEnemy)	// 
{
	// eª½Á½
	Bullet* pBullet = GetBullet();
	if (CollisionCircle(pEnemy->pos, COLLISION, pBullet->pos, pBullet->size.x))
	{
		//±±ÅtbNÌ¹

		pEnemy->FlagOn = true;
	}

	Player* pPlayer = GetPlayer();
	if ((CollisionCircle(pEnemy->pos, COLLISION, pPlayer->pos, pPlayer->size.x)) && pEnemy->FlagOn)
	{
		pEnemy->bUse = false;
		SetLife();
	}


}