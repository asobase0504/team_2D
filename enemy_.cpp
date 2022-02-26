//=========================================
// 
// G
// Author : Isoe Jukia
// Author : ûüì]
// 
//=========================================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "Calculation.h"
#include "player.h"

//----------------------------
// }N
//----------------------------
#define COLLISION	(25)
#define JUDGEMENT	(100)	//ð¯éGÌ»èTCY
#define UP_ESCAPE	(200)	//ãÉ¦°éGÌ»èTCY

//----------------------------
// static Ï
//----------------------------
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};	//eNX`ÖÌ|C^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//¸_obt@ÖÌ|C^
static Enemy s_aEnemy[MAX_ENEMY];
static int s_nCounterAnim;
static int s_nPatternAnim;

//----------------------------
// vg^Cvé¾
//----------------------------
static void UpdateSky1(Enemy* pEnemy);
static void UpdateSky2(Enemy* pEnemy);
static void UpdateBuckSky(Enemy* pEnemy);
static void UpdateZakato1(Enemy* pEnemy);	//UJ[gÌ1(ÇöAAêèÌÔuÅeð­ËµÄÁÅ)
static void UpdateZakato2(Enemy* pEnemy);	//UJ[gÌ2(ÇöiVAêèÌÔuÅeð­ËµÄÁÅ)
static void UpdateZakato3(Enemy* pEnemy);	//UJ[gÌ3(ÇöAAÔoßÅeð­ËµÄÁÅ)
static void UpdateZakato4(Enemy* pEnemy);	//UJ[gÌ4(ÇöiVAÔoßÅeð­ËµÄÁÅ)
static void ZakatoFinish(Enemy* pEnemy);	//UJ[gªI¹·éÛÌ

//--------------------------------------------------
// ú»
//--------------------------------------------------
void InitEnemy(void)
{
	//foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// eNX`ÇÝÝ	GeNX`
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// ÚÊ
		&s_pTex[ENEMYTYPE_SKY_1]);

	// eNX`ÇÝÝ	GeNX`
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// Ô
		&s_pTex[ENEMYTYPE_SKY_2]);

	// eNX`ÇÝÝ	GeNX`
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 
		&s_pTex[ENEMYTYPE_SKY_3]);

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// GÌîñÌú»
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		s_aEnemy[nCntEnemy].nLife = 1;
		s_aEnemy[nCntEnemy].flg = true;
	}

	// ¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,	// mÛ·éobt@TCY
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// ¸_tH[}bg
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// ¸_îñÖÌ|C^

	// ¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// ¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);

		// rhwÌÝè
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ¸_J[ÌÝè
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// eNX`ÀWÌÝè	   u      v
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / 12.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 12.0f, 1.0f);

		pVtx += 4;
	}

	// ¸_ðAbN·é
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// I¹
//--------------------------------------------------
void UninitEnemy(void)
{
	for (int Count = 0; Count < ENEMYTYPE_MAX; Count++)
	{
		if (s_pTex[Count] != NULL)
		{
			s_pTex[Count]->Release();
			s_pTex[Count] = NULL;
		}
	}

	// ¸_obt@jü
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// XV
//--------------------------------------------------
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		if (!pEnemy->bUse)
		{
			continue;	//ðPñòÎ·@[1]
		}

		// vC[îñÌæ¾
		Player* pPlayer = GetPlayer();

		if (pEnemy->flg)	//Çö
		{//true Ì
			float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;
			float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;
			float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);
			pEnemy->move.x = (moveEnemyX / moveEnemyR) * 3.0f;
			pEnemy->move.y = (moveEnemyY / moveEnemyR) * 3.0f;
		}

		if (pEnemy->nLife > 0)
		{
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

				pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				break;
			case ENEMYTYPE_BAKYURA:		// ³GÇ
				break;
			case ENEMYTYPE_SHEONITE:	// óÌG
				break;

			case TYPEENEMY_WARP_1:		//ÇöAAêèÌ£Åeð­ËµAÁÅ	
				UpdateZakato1(pEnemy);
				break;

			case TYPEENEMY_WARP_2:
				UpdateZakato2(pEnemy);
				break;

			case TYPEENEMY_WARP_3:		//ÇöAAÔoßÅeð­ËµÄÁÅ
				UpdateZakato3(pEnemy);
				break;

			case TYPEENEMY_WARP_4:		//ÇöiVAÔoßÅeð­ËµÄÁÅ
				UpdateZakato4(pEnemy);
				break;
			default:
				break;
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

					SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
					pEnemy->nCntBullet = 0;
				}
			}
		}
		else
		{// Gªñ¾Æ«Ìð±±É¿ñÅ­¾³¢


		}

		// ÊuðXV
		pEnemy->pos += pEnemy->move;

		// æÊ[Ýè
		if (pEnemy->pos.x <= -100.0f || pEnemy->pos.x >= SCREEN_WIDTH + 100.0f || pEnemy->pos.y >= SCREEN_HEIGHT + 100.0f || pEnemy->pos.y <= -100.0f)
		{
			pEnemy->bUse = false;
			pEnemy->pos.x = SCREEN_WIDTH;
		}

		VERTEX_2D* pVtx;
		// ¸_obt@ðbNµA¸_f[^ÖÌ|C^ðæ¾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;	// ¸_ÀWf[^Ì|C^ðSÂªißé		

		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);

		s_nCounterAnim++;	// Aj[V
		if (s_nCounterAnim >= 10)
		{
			s_nCounterAnim = 0;
			s_nPatternAnim++;

			if (s_nPatternAnim >= 12)
			{
				s_nPatternAnim = 0;
			}

			pVtx[0].tex = D3DXVECTOR2((1.0f / 12.0f) * s_nPatternAnim, 0.0f);			//--------------------------Gª¦½Æ«â·
			pVtx[1].tex = D3DXVECTOR2((1.0f / 12.0f) * (s_nPatternAnim + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / 12.0f) * s_nPatternAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / 12.0f) * (s_nPatternAnim + 1), 1.0f);

		}

		switch (pEnemy->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;
		case  ENEMYSTATE_DAMAGE:
			pEnemy->nCounterState--;
			if (pEnemy->nCounterState <= 0)
			{
				pEnemy->nCounterState = ENEMYSTATE_NORMAL;

				// ¸_J[ÌÝè
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		// ¸_ðAbN·é
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// `æ
//--------------------------------------------------
void DrawEnemy(void)
{
	// foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntEnemy * 4,
			2);
	}
}

//--------------------------------------------------
// Ýè
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse)
		{// Gªgp³êÄ¢éê
			continue;
		}

		pEnemy->pos = pos;
		pEnemy->nLife = 1;
		pEnemy->bUse = true;	// gpµÄ¢éóÔ
		pEnemy->flg = true;
		pEnemy->nType = nType;

		VERTEX_2D*pVtx;

		// ¸_obt@ðbNµA¸_f[^ÖÌ|C^ðæ¾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * 4;	// ¸_ÀWf[^Ì|C^ðSÂªißé

		// ¸_ÀWÌÝè
		pVtx[0].pos.x = pEnemy->pos.x - ENEMY_WIDTH;
		pVtx[0].pos.y = pEnemy->pos.y - ENEMY_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEnemy->pos.x + ENEMY_WIDTH;
		pVtx[1].pos.y = pEnemy->pos.y - ENEMY_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEnemy->pos.x - ENEMY_WIDTH;
		pVtx[2].pos.y = pEnemy->pos.y + ENEMY_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEnemy->pos.x + ENEMY_WIDTH;
		pVtx[3].pos.y = pEnemy->pos.y + ENEMY_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// ¸_ðAbN·é
		s_pVtxBuff->Unlock();
		break;
	}
	return pEnemy;
}

//--------------------------------------------------
// ½è
//--------------------------------------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	Enemy* pEnemy = &s_aEnemy[nCntEnemy];

	pEnemy->nLife -= nDamage;

	//
	// ÕËÌSE
	//

	VERTEX_2D*pVtx;

	// ¸_obt@ðbNµA¸_f[^ÖÌ|C^ðæ¾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntEnemy;

	//PauseIN *pPauseIN = GetPausein();

	if (pEnemy->nLife < 0)	// Gªñ¾Æ«
	{
		Player* pPlayer = GetPlayer();

		float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;				// ©ªÆGÌðvZ
		float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;				// ¯lvZ
		float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);

		pEnemy->move.x = -(moveEnemyX / moveEnemyR) * 30.0f;	// ñ¾Æ«òñÅ­
		pEnemy->move.y = -(moveEnemyY / moveEnemyR) * 30.0f;

		//
		// XRAÌÁZ
		//

		// ¸_J[ÌÝè
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		pEnemy->state = ENEMYSTATE_DAMAGE;
		pEnemy->nCounterState = 10;
		pVtx[0].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
	}

	// ¸_ðAbN·é
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// æ¾
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return s_aEnemy;
}

//--------------------------------------------------
// XJC1ÌGÌXV
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
		&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
		&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
		&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
	{//Gª¦°é

		pEnemy->flg = false;
		//s_aEnemy[nCntEnemy].move.y = 0;		//RgAEgðÁ·Æ¼pÉÈªé
		pEnemy->move.x = 3;

	}
}

//--------------------------------------------------
// XJC2ÌGÌXV
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
		&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
		&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
		&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
	{// Gª¦°é
		pEnemy->flg = false;
		pEnemy->move.y = 0;		// RgAEgðÁ·Æ¼pÉÈªé
		pEnemy->move.x = 3;
	}
}

//--------------------------------------------------
// XJC3ÌGÌXV
//--------------------------------------------------
void UpdateBuckSky(Enemy* pEnemy)
{
	if (pEnemy->Buk)	//ßé
	{//true Ì
		pEnemy->move.y += easeInOutBack(-0.25f);
	}

	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	if (pEnemy->pos.x + UP_ESCAPE >= pPlayer->pos.x - UP_ESCAPE
		&& pEnemy->pos.x - UP_ESCAPE <= pPlayer->pos.x + UP_ESCAPE
		&& pEnemy->pos.y + UP_ESCAPE >= pPlayer->pos.y - UP_ESCAPE
		&& pEnemy->pos.y - UP_ESCAPE <= pPlayer->pos.y + UP_ESCAPE)
	{// Gª¦°é
		pEnemy->flg = false;		//Çö
		pEnemy->Buk = true;		//ßé
	}
}

//-------------------------------------------------
//	UJ[g1ÌGÌXV
//-------------------------------------------------
void UpdateZakato1(Enemy* pEnemy)
{
	// vC[îñÌæ¾
	Player* pPlayer = GetPlayer();

	//ÇöAAêèÌ£Åeð­ËµAÁÅ
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2, 0.0f);
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
		pEnemy->flg = false;		//Çö
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
		pEnemy->flg = false;		//Çö
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
	Player* pPlayer = GetPlayer();
	
	// Gl~[©çvC[ÜÅÌ£ÌZo
	D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

	//ÎpüÌpxðZo
	Direction.z = atan2f(Direction.x, Direction.y);
	Direction.x = 0.0f;		// gíÈ¢îñÈÌÅú»
	Direction.y = 0.0f;		// gíÈ¢îñÈÌÅú»

	//eÌ­Ë
	SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
	pEnemy->nCntBullet = 0;
	pEnemy->flg = false;		//Çö
	pEnemy->bTP = false;
	pEnemy->bUse = false;		//Gl~[ð³ø»·é
}