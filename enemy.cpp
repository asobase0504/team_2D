//=========================================
// 
// G
// Author : Isoe Jukia
// Author : ûüì]
// 
//=========================================
#include "main.h"
#include "enemy.h"
#include "Calculation.h"
#include "player.h"

//----------------------------
//}N
//----------------------------
#define COLLISION	(25)
#define JUDGEMENT	(100)	//ð¯éGÌ»èTCY
#define UP_ESCAPE	(200)	//ãÉ¦°éGÌ»èTCY
#define MAX_ENEMY	(255)

//----------------------------
//static Ï
//----------------------------
static LPDIRECT3DTEXTURE9 s_pTex[TYPEENEMY_MAX] = {};	//eNX`ÖÌ|C^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//¸_obt@ÖÌ|C^
static enemy s_aEnemy[MAX_ENEMY];
static int s_nCounterAnim;
static int s_nPatternAnim;

static D3DXCOLOR g_col;

//ú»
void InitEnemy(void)
{
	//foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// eNX`ÇÝÝ	GeNX`
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// ÚÊ
		&s_pTex[TYPEENEMY_SKY_1]);

	// eNX`ÇÝÝ	GeNX`
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// Ô
		&s_pTex[TYPEENEMY_SKY_2]);

	// eNX`ÇÝÝ	GeNX`
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 
		&s_pTex[TYPEENEMY_SKY_3]);

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
		enemy* pEnemy = &s_aEnemy[nCntEnemy];

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

// I¹
void UninitEnemy(void)
{
	for (int Count = 0; Count < TYPEENEMY_MAX; Count++)
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

void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		enemy* pEnemy = &s_aEnemy[nCntEnemy];

		if (!pEnemy->bUse)
		{
			continue;	//ðPñòÎ·@[1]
		}

		// vC[îñæ¾
		Player* pPlayer = GetPlayer();

		if (pEnemy->flg)	//Çö
		{//true Ì
			float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;
			float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;
			float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);
			pEnemy->move.x = (moveEnemyX / moveEnemyR) * 3.0f;
			pEnemy->move.y = (moveEnemyY / moveEnemyR) * 3.0f;
		}

		if (pEnemy->pos.x + COLLISION >= pPlayer->pos.x - COLLISION
			&& pEnemy->pos.x - COLLISION <= pPlayer->pos.x + COLLISION
			&& pEnemy->pos.y + COLLISION >= pPlayer->pos.y - COLLISION
			&& pEnemy->pos.y - COLLISION <= pPlayer->pos.y + COLLISION)
		{//eÀWdÈè

		 //HitPlayer(2);			//_[Wðó¯é

		}

		if (pEnemy->nLife > 0)
		{
			//-------------------------------------------------------------
			//		ì

			switch (pEnemy->nType)
			{
			case TYPEENEMY_SKY_1:
				if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
					&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
					&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
					&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
				{//Gª¦°é

					pEnemy->flg = false;
					//s_aEnemy[nCntEnemy].move.y = 0;		//RgAEgðÁ·Æ¼pÉÈªé
					pEnemy->move.x = 3;

				}
				break;
			case TYPEENEMY_SKY_2:

				if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
					&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
					&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
					&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
				{// Gª¦°é
					pEnemy->flg = false;
					pEnemy->move.y = 0;		// RgAEgðÁ·Æ¼pÉÈªé
					pEnemy->move.x = 3;
				}
				break;
			case TYPEENEMY_SKY_3:		//ø«ÔµG

				if (pEnemy->Buk)		//ßé
				{//true Ì
					pEnemy->move.y += easeInOutBack(-0.25f);
				}

				if (pEnemy->pos.x + UP_ESCAPE >= pPlayer->pos.x - UP_ESCAPE
					&& pEnemy->pos.x - UP_ESCAPE <= pPlayer->pos.x + UP_ESCAPE
					&& pEnemy->pos.y + UP_ESCAPE >= pPlayer->pos.y - UP_ESCAPE
					&& pEnemy->pos.y - UP_ESCAPE <= pPlayer->pos.y + UP_ESCAPE)
				{// Gª¦°é
					pEnemy->flg = false;		//Çö
					s_aEnemy[nCntEnemy].Buk = true;		//ßé
				}
				break;
			case TYPEENEMY_GROUND_1:		//¼iG

				break;

			case TYPEENEMY_GROUND_2:		//®©È¢G

				pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				break;
			default:
				break;
			}
			if (pEnemy->pos.y > 0)
			{
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

		// æÊ[Ýè			¶Ç									EÇ						ºÇ						ãÇ
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

		s_nCounterAnim++;								// Aj[V
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
			pEnemy->nCounerState--;
			if (pEnemy->nCounerState <= 0)
			{
				pEnemy->nCounerState = ENEMYSTATE_NORMAL;

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

// `æ
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

void SetEnemy(D3DXVECTOR3 pos, TYPEENEMY nType)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		enemy* pEnemy = &s_aEnemy[nCntEnemy];

		if (pEnemy->bUse)
		{// Gªgp³êÄ¢éê
			continue;
		}

		pEnemy->pos = pos;
		pEnemy->nLife = 1;
		pEnemy->bUse = true;	// gpµÄ¢éóÔ
		pEnemy->flg = true;
		//pEnemy->nCounter = 0;	// CNg
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
}

// ½è
void HitEnemy(int nCntEnemy, int nDamage)
{
	enemy* pEnemy = &s_aEnemy[nCntEnemy];

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
		pVtx[0].col = g_col;
		pVtx[1].col = g_col;
		pVtx[2].col = g_col;
		pVtx[3].col = g_col;
	}
	else
	{
		pEnemy->state = ENEMYSTATE_DAMAGE;
		pEnemy->nCounerState = 10;
		pVtx[0].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0, 0, 1.0f);
	}

	// ¸_ðAbN·é
	s_pVtxBuff->Unlock();
}

// æ¾
enemy *GetEnemy(void)
{
	return s_aEnemy;
}
