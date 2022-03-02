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

//**************************************************
// }N
//**************************************************
#define COLLISION	(25)
#define RADIUS		(5.0f)
#define SPEED		(2.0f)
#define SHEO_DIST_START_ROTATION		(200.0f)		// VIiCgªñènßé£
#define SHEO_RADIUS_ROTATION		(SHEO_DIST_START_ROTATION - 20.0f)	// VIiCgÌñ]¼a
#define SHEO_SPEED_ROTATION		(D3DXToRadian(6.0f))
#define SHEO_FRAME_GOODBY	(300)	// VIiCgª¨AèÉÈéÔ
#define SHEO_SPEED_GOODBY	(10.0f)	// VIiCgª¨AèÉÈé¬x
#define JUDGEMENT	(100)	// ð¯éGÌ»èTCY
#define UP_ESCAPE	(200)	// ãÉ¦°éGÌ»èTCY

//**************************************************
// X^eBbNÏ
//**************************************************
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;
static Enemy s_aEnemy[MAX_ENEMY];					// GÌîñ
static Enemy s_aTypeEnemy[ENEMYTYPE_MAX];

//**************************************************
// vg^Cvé¾
//**************************************************
static void TrackingMove(Enemy* pEnemy);	// Çö
static void UpdateSky1(Enemy* pEnemy);		// óÌG1ÌXV
static void UpdateSky2(Enemy* pEnemy);		// óÌG2ÌXV
static void UpdateBuckSky(Enemy* pEnemy);	// AéóÌGÌXV
static void MoveBakyura(Enemy* pEnemy);
static void MoveSheonite(Enemy* pEnemy);

//**************************************************
// O[oÖ
//**************************************************
//--------------------------------------------------
// ú»
// AutherFIsoe Jukia
//--------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//foCXÌ|C^

	// eNX`ÌÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy1.png",
		&s_pTex[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BirdEnemy.png",
		&s_pTex[1]);

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// ¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;			// ¸_îñÖÌ|C^

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// ¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);

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

		pVtx += 4;		// ¸_f[^Ì|C^ð4Âªißé
	}

	// ¸_obt@ðAbN·é
	s_pVtxBuff->Unlock();

	//  ÆÅt@CÅüÍÅ«éæ¤Éµ½¢È[[[[
	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		s_aTypeEnemy[nCnt].nLife = 1;
	}
	s_aTypeEnemy[ENEMYTYPE_SHEONITE].nLife = 30;
	s_aTypeEnemy[ENEMYTYPE_BAKYURA].nLife = 255;
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
		case ENEMYTYPE_GROUND_1:	// ¼iG

			break;
		case ENEMYTYPE_GROUND_2:	// ®©È¢G

			break;
		case ENEMYTYPE_BAKYURA:		// ³GÇ
			MoveBakyura(pEnemy);
			break;
		case ENEMYTYPE_SHEONITE:	// óÌG
			MoveSheonite(pEnemy);
			break;
		default:
			break;
		}

		// Gl~[ª_[WðHçÁ½Æ«
		switch (pEnemy->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;
		case  ENEMYSTATE_DAMAGE:
			pEnemy->nCntState--;
			if (pEnemy->nCntState <= 0)
			{
				pEnemy->nCntState = ENEMYSTATE_NORMAL;

				// ¸_J[ÌÝè
				SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			}
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
			if (CollisionCircle(pEnemy->pos, COLLISION, pBullet->pos, pBullet->size.x))
			{
				AddScore(255);
				HitEnemy(pEnemy, 1);
			}
		}

		// Gl~[ÌCtª0ÉÈÁ½Æ«
		if (pEnemy->nLife < 0)
		{
			pEnemy->bUse = false;
		}

		// ¸_ÀWÌXV
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
	}
	// ¸_obt@ðAbN·é
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// oLÌ®«
// Author: Buriya Kota
//--------------------------------------------------
void MoveBakyura(Enemy* pEnemy)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (!s_aEnemy[nCnt].bUse)
		{
			continue;
		}

		if (s_aEnemy[nCnt].pos.y > SCREEN_HEIGHT)
		{
			s_aEnemy[nCnt].bUse = false;
		}
	}
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
		{	// VIiCgª AèÉÈçêÜ·
			distance = SHEO_DIST_START_ROTATION;
			pEnemy->move.x = sinf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
			pEnemy->move.y = cosf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
		}
		// vC[ÌÊu©çVIiCgÌÊuðZo
		pEnemy->pos.x = pPlayer->pos.x + distance * sinf(pEnemy->fSheoRot);
		pEnemy->pos.y = pPlayer->pos.y + distance * cosf(pEnemy->fSheoRot);
	}
}

//--------------------------------------------------
// XJC1ÌGÌXV
// AutherFIsoe Jukia
// AutherFYuda Kaito
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();
	// ÇöÌ
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{//Gª¦°é
		pEnemy->bTracking = false;
		pEnemy->move.x = 3.0f;
	}

	// eðo·
	if (pEnemy->pos.y > 0.0f)
	{	// æÊàÉûÜÁÄ¢éê
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 120)
		{
			// Gl~[©çvC[ÜÅÌ£ÌZo
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//ÎpüÌpxðZo
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// gíÈ¢îñÈÌÅú»
			Direction.y = 0.0f;		// gíÈ¢îñÈÌÅú»

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// XJC2ÌGÌXV
// AutherFIsoe Jukia
// AutherFYuda Kaito
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();
	// ÇöÌ
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{// Gª¦°é
		pEnemy->bTracking = false;
		pEnemy->move.y = 0;		// RgAEgðÁ·Æ¼pÉÈªé
		pEnemy->move.x = 3;
	}

	// eðo·
	if (pEnemy->pos.y > 0.0f)
	{	// æÊàÉûÜÁÄ¢éê
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 120)
		{
			// Gl~[©çvC[ÜÅÌ£ÌZo
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//ÎpüÌpxðZo
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// gíÈ¢îñÈÌÅú»
			Direction.y = 0.0f;		// gíÈ¢îñÈÌÅú»

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
			pEnemy->nCntBullet = 0;
		}
	}
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
	{//true Ì
		pEnemy->move.y += easeInOutBack(-0.25f);
	}
	// ÇöÌ
	TrackingMove(pEnemy);

	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{// Gª¦°é
		pEnemy->bTracking = false;		//Çö
		pEnemy->bBack = true;		//ßé
	}

	// eðo·
	if (pEnemy->pos.y > 0.0f)
	{	// æÊàÉûÜÁÄ¢éê
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 120)
		{
			// Gl~[©çvC[ÜÅÌ£ÌZo
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//ÎpüÌpxðZo
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// gíÈ¢îñÈÌÅú»
			Direction.y = 0.0f;		// gíÈ¢îñÈÌÅú»

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// `æ
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
		if (s_aEnemy[nCntEnemy].bUse)
		{
			// eNX`ÌÝè
			pDevice->SetTexture(0, s_pTex[s_aEnemy[nCntEnemy].nType]);

			// |SÌ`æ
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntEnemy * 4,
				2);
		}
	}
}

//--------------------------------------------------
// Zbg
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, float Size, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	VERTEX_2D *pVtx;			// ¸_îñÖÌ|C^

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse)
		{
			continue;
		}

		// Gl~[Éîñð½¹é
		*pEnemy = s_aTypeEnemy[nType];

		// UseðtrueÉ·é
		pEnemy->bUse = true;
		// øÌÝè
		pEnemy->pos = pos;
		pEnemy->Size = Size;
		pEnemy->nType = nType;
		//pEnemy->nLife = 1; //	InitEnemyÅ¼Ýèµ½ÌÅA±±ÅÍâçÈ¢

		// ¸_ÀWÌXV
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);

		// ¸_J[ÌÝè
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;
	}
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
		pEnemy->move.x = (moveEnemyX / moveEnemyR) * 3.0f;
		pEnemy->move.y = (moveEnemyY / moveEnemyR) * 3.0f;
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
