#include "main.h"
#include "enemy.h"
#include "Calculation.h"
#include "enemybullet.h"
#include "player.h"

//----------------------------
//マクロ
#define COLLISION	(25)
#define JUDGEMENT	(100)	//避ける敵の判定サイズ
#define UP_ESCAPE	(200)	//上に逃げる敵の判定サイズ
#define MAX_ENEMY	(255)

//----------------------------
//static 変数
static LPDIRECT3DTEXTURE9 s_pTex[TYPEENEMY_MAX] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//頂点バッファへのポインタ
static enemy s_aEnemy[MAX_ENEMY];
static int s_nCounterAnim;
static int s_nPatternAnim;

static D3DXCOLOR g_col;

//初期化
void InitEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 目玉
		&s_pTex[TYPEENEMY_SKY_1]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 車
		&s_pTex[TYPEENEMY_SKY_2]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 
		&s_pTex[TYPEENEMY_SKY_3]);

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// 敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		s_aEnemy[nCntEnemy].nLife = 1;
		s_aEnemy[nCntEnemy].flg = true;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// 頂点フォーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定	   u      v
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / 12.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 12.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点をアンロックする
	s_pVtxBuff->Unlock();

	InitEnemyBullet();
}

// 終了
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

	// 頂点バッファ破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	UninitEnemyBullet();
}

void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		enemy* pEnemy = &s_aEnemy[nCntEnemy];

		if (!pEnemy->bUse)
		{
			continue;	//処理を１回飛ばす　[1]
		}

		Player* pPlayer = GetPlayer();

		if (pEnemy->flg)	//追尾
		{//true の時
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
		{//弾座標重なり

		 //HitPlayer(2);			//ダメージを受ける

		}

		if (pEnemy->nLife > 0)
		{
			//-------------------------------------------------------------
			//		試作

			switch (pEnemy->nType)
			{
			case TYPEENEMY_SKY_1:
				if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
					&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
					&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
					&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
				{//敵が逃げる処理

					pEnemy->flg = false;
					//s_aEnemy[nCntEnemy].move.y = 0;		//コメントアウトを消すと直角に曲がる
					pEnemy->move.x = 3;

				}
				break;
			case TYPEENEMY_SKY_2:

				if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
					&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
					&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
					&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
				{// 敵が逃げる処理
					pEnemy->flg = false;
					pEnemy->move.y = 0;		// コメントアウトを消すと直角に曲がる
					pEnemy->move.x = 3;
				}
				break;
			case TYPEENEMY_SKY_3:		//引き返し敵

				if (pEnemy->Buk)		//戻る処理
				{//true の時
					pEnemy->move.y += easeInOutBack(-0.25f);
				}

				if (pEnemy->pos.x + UP_ESCAPE >= pPlayer->pos.x - UP_ESCAPE
					&& pEnemy->pos.x - UP_ESCAPE <= pPlayer->pos.x + UP_ESCAPE
					&& pEnemy->pos.y + UP_ESCAPE >= pPlayer->pos.y - UP_ESCAPE
					&& pEnemy->pos.y - UP_ESCAPE <= pPlayer->pos.y + UP_ESCAPE)
				{// 敵が逃げる処理
					pEnemy->flg = false;		//追尾
					s_aEnemy[nCntEnemy].Buk = true;		//戻る処理
				}
				break;
			case TYPEENEMY_GROUND_1:		//直進敵

				break;

			case TYPEENEMY_GROUND_2:		//動かない敵

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
					SetEnemyBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f));
					pEnemy->nCntBullet = 0;
				}
			}
		}
		else
		{// 敵が死んだときの処理をここに撃ち込んでください


		}

		// 位置を更新
		pEnemy->pos += pEnemy->move;

		// 画面端設定			左壁									右壁						下壁						上壁
		if (pEnemy->pos.x <= -100.0f || pEnemy->pos.x >= SCREEN_WIDTH + 100.0f || pEnemy->pos.y >= SCREEN_HEIGHT + 100.0f || pEnemy->pos.y <= -100.0f)
		{
			pEnemy->bUse = false;
			pEnemy->pos.x = SCREEN_WIDTH;
		}

		VERTEX_2D* pVtx;

		// 頂点バッファをロックし、頂点データへのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;	// 頂点座標データのポインタを４つ分進める		

		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y + ENEMY_HEIGHT, 0.0f);

		s_nCounterAnim++;								// アニメーション
		if (s_nCounterAnim >= 10)
		{
			s_nCounterAnim = 0;
			s_nPatternAnim++;

			if (s_nPatternAnim >= 12)
			{
				s_nPatternAnim = 0;
			}

			pVtx[0].tex = D3DXVECTOR2((1.0f / 12.0f) * s_nPatternAnim, 0.0f);			//--------------------------敵が増えたとき増やす
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

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		// 頂点をアンロックする
		s_pVtxBuff->Unlock();
	}

	UpdateEnemyBullet();
}

// 描画
void DrawEnemy(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, s_pTex[s_aEnemy[nCntEnemy].nType]);

		if (!s_aEnemy[nCntEnemy].bUse)
		{
			continue;
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntEnemy * 4,
			2);
	}
	DrawEnemyBullet();
}

void SetEnemy(D3DXVECTOR3 pos, TYPEENEMY nType)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		enemy* pEnemy = &s_aEnemy[nCntEnemy];

		if (pEnemy->bUse)
		{// 敵が使用されている場合
			continue;
		}

		pEnemy->pos = pos;
		pEnemy->nLife = 1;
		pEnemy->bUse = true;	// 使用している状態
		pEnemy->flg = true;
		//pEnemy->nCounter = 0;	// インクリメント
		pEnemy->nType = nType;

		VERTEX_2D*pVtx;
		// 頂点バッファをロックし、頂点データへのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * 4;	// 頂点座標データのポインタを４つ分進める

		// 頂点座標の設定
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

		// 頂点をアンロックする
		s_pVtxBuff->Unlock();
		break;
	}
}

// 当たり処理
void HitEnemy(int nCntEnemy, int nDamage)
{
	enemy* pEnemy = &s_aEnemy[nCntEnemy];

	pEnemy->nLife -= nDamage;

	//
	// 衝突時のSE
	//

	VERTEX_2D*pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntEnemy;

	//PauseIN *pPauseIN = GetPausein();

	if (pEnemy->nLife < 0)	// 敵が死んだとき
	{
		Player* pPlayer = GetPlayer();

		float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;				// 自分と敵のｘを計算
		float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;				// 同様ｙ計算
		float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);

		pEnemy->move.x = -(moveEnemyX / moveEnemyR) * 30.0f;	// 死んだとき飛んでく
		pEnemy->move.y = -(moveEnemyY / moveEnemyR) * 30.0f;

		//
		// スコアの加算
		//

		// 頂点カラーの設定
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

	// 頂点をアンロックする
	s_pVtxBuff->Unlock();
}

// 取得処理
enemy *GetEnemy(void)
{
	return s_aEnemy;
}
