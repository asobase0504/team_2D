//==================================================
// 
// 敵処理
// Author : Isoe Jukia
// Author : 髙野馨將
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
// マクロ
//**************************************************
#define COLLISION	(25)
#define RADIUS		(5.0f)
#define SPEED		(2.0f)
#define SHEO_DIST_START_ROTATION		(200.0f)		// シオナイトが回り始める距離
#define SHEO_RADIUS_ROTATION		(SHEO_DIST_START_ROTATION - 20.0f)	// シオナイトの回転半径
#define SHEO_SPEED_ROTATION		(D3DXToRadian(6.0f))
#define SHEO_FRAME_GOODBY	(300)	// シオナイトがお帰りになる時間
#define SHEO_SPEED_GOODBY	(10.0f)	// シオナイトがお帰りになる速度
#define JUDGEMENT	(100)	// 避ける敵の判定サイズ
#define UP_ESCAPE	(200)	// 上に逃げる敵の判定サイズ

//**************************************************
// スタティック変数
//**************************************************
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;
static Enemy s_aEnemy[MAX_ENEMY];					// 敵の情報
static Enemy s_aTypeEnemy[ENEMYTYPE_MAX];

//**************************************************
// プロトタイプ宣言
//**************************************************
static void TrackingMove(Enemy* pEnemy);	// 追尾処理
static void UpdateSky1(Enemy* pEnemy);		// 空の敵1の更新
static void UpdateSky2(Enemy* pEnemy);		// 空の敵2の更新
static void UpdateBuckSky(Enemy* pEnemy);	// 帰る空の敵の更新
static void MoveBakyura(Enemy* pEnemy);
static void MoveSheonite(Enemy* pEnemy);

//**************************************************
// グローバル関数
//**************************************************
//--------------------------------------------------
// 初期化
// Auther：Isoe Jukia
//--------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy1.png",
		&s_pTex[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BirdEnemy.png",
		&s_pTex[1]);

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

		// テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// あとでファイルで入力できるようにしたいなーーーー
	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		s_aTypeEnemy[nCnt].nLife = 1;
	}
	s_aTypeEnemy[ENEMYTYPE_SHEONITE].nLife = 30;
	s_aTypeEnemy[ENEMYTYPE_BAKYURA].nLife = 255;
}

//--------------------------------------------------
// 終了
// Auther：Isoe Jukia
//--------------------------------------------------
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		if (s_pTex[nCntEnemy] != NULL)
		{// テクスチャの破棄
			s_pTex[nCntEnemy]->Release();
			s_pTex[nCntEnemy] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//
// 更新
//
// Auther：Isoe Jukia
// Auther：Takano Keisuke
// Auther：Yuda Kaito
// Auther：Tomidokoro Tomoki
// Auther：Buriya Kouta
//
//--------------------------------------------------
void UpdateEnemy(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	Enemy* pEnemy = s_aEnemy;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (!pEnemy->bUse)
		{
			continue;
		}

		pEnemy->pos += pEnemy->move;

		// エネミーのタイプごとの処理
		switch (pEnemy->nType)
		{
		case ENEMYTYPE_SKY_1:
			UpdateSky1(pEnemy);
			break;
		case ENEMYTYPE_SKY_2:
			UpdateSky2(pEnemy);
			break;
		case ENEMYTYPE_SKY_3:		// 引き返し敵
			UpdateBuckSky(pEnemy);
			break;
		case ENEMYTYPE_GROUND_1:	// 直進敵

			break;
		case ENEMYTYPE_GROUND_2:	// 動かない敵

			break;
		case ENEMYTYPE_BAKYURA:		// 無敵壁
			MoveBakyura(pEnemy);
			break;
		case ENEMYTYPE_SHEONITE:	// 空の敵
			MoveSheonite(pEnemy);
			break;
		default:
			break;
		}

		// エネミーがダメージを食らったとき
		switch (pEnemy->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;
		case  ENEMYSTATE_DAMAGE:
			pEnemy->nCntState--;
			if (pEnemy->nCntState <= 0)
			{
				pEnemy->nCntState = ENEMYSTATE_NORMAL;

				// 頂点カラーの設定
				SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			}
			break;
		}

		// 弾が当たった時
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

		// エネミーのライフが0になったとき
		if (pEnemy->nLife < 0)
		{
			pEnemy->bUse = false;
		}

		// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// バキュラの動き
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
// シオナイトの動き
// Author: Buriya Kota
//--------------------------------------------------
void MoveSheonite(Enemy* pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();
	// 追尾の処理
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, 0.0f, pPlayer->pos, SHEO_DIST_START_ROTATION))
	{	// 範囲に入ったら回る
		pEnemy->bTracking = false;
		
		if (pEnemy->fSheoRot < 0.0f)
		{	// 初期の角度
			D3DXVECTOR3 vecEnemy = pEnemy->pos - pPlayer->pos;
			pEnemy->fSheoRot = atan2f(vecEnemy.x, vecEnemy.y);
		}
		// 少しずつ回す
		pEnemy->fSheoRot += SHEO_SPEED_ROTATION;
		float distance = SHEO_RADIUS_ROTATION;
		pEnemy->nSheoCnt++;
		if (pEnemy->nSheoCnt > SHEO_FRAME_GOODBY)
		{	// シオナイトがあ帰りになられます
			distance = SHEO_DIST_START_ROTATION;
			pEnemy->move.x = sinf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
			pEnemy->move.y = cosf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
		}
		// プレイヤーの位置からシオナイトの位置を算出
		pEnemy->pos.x = pPlayer->pos.x + distance * sinf(pEnemy->fSheoRot);
		pEnemy->pos.y = pPlayer->pos.y + distance * cosf(pEnemy->fSheoRot);
	}
}

//--------------------------------------------------
// スカイ1の敵の更新処理
// Auther：Isoe Jukia
// Auther：Yuda Kaito
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();
	// 追尾の処理
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{//敵が逃げる処理
		pEnemy->bTracking = false;
		pEnemy->move.x = 3.0f;
	}

	// 弾を出す処理
	if (pEnemy->pos.y > 0.0f)
	{	// 画面内に収まっている場合
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 120)
		{
			// エネミーからプレイヤーまでの距離の算出
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//対角線の角度を算出
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// 使わない情報なので初期化
			Direction.y = 0.0f;		// 使わない情報なので初期化

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// スカイ2の敵の更新処理
// Auther：Isoe Jukia
// Auther：Yuda Kaito
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();
	// 追尾の処理
	TrackingMove(pEnemy);

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{// 敵が逃げる処理
		pEnemy->bTracking = false;
		pEnemy->move.y = 0;		// コメントアウトを消すと直角に曲がる
		pEnemy->move.x = 3;
	}

	// 弾を出す処理
	if (pEnemy->pos.y > 0.0f)
	{	// 画面内に収まっている場合
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 120)
		{
			// エネミーからプレイヤーまでの距離の算出
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//対角線の角度を算出
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// 使わない情報なので初期化
			Direction.y = 0.0f;		// 使わない情報なので初期化

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// スカイ3の敵の更新処理
// Auther：Isoe Jukia
// Auther：Yuda Kaito
//--------------------------------------------------
void UpdateBuckSky(Enemy* pEnemy)
{
	//戻る処理
	if (pEnemy->bBack)	
	{//true の時
		pEnemy->move.y += easeInOutBack(-0.25f);
	}
	// 追尾の処理
	TrackingMove(pEnemy);

	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	if (CollisionCircle(pEnemy->pos, JUDGEMENT, pPlayer->pos, JUDGEMENT))
	{// 敵が逃げる処理
		pEnemy->bTracking = false;		//追尾
		pEnemy->bBack = true;		//戻る処理
	}

	// 弾を出す処理
	if (pEnemy->pos.y > 0.0f)
	{	// 画面内に収まっている場合
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 120)
		{
			// エネミーからプレイヤーまでの距離の算出
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//対角線の角度を算出
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// 使わない情報なので初期化
			Direction.y = 0.0f;		// 使わない情報なので初期化

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->Size, 0.0f), Direction, BULLETTYPE_ENEMY);
			pEnemy->nCntBullet = 0;
		}
	}
}

//--------------------------------------------------
// 描画処理
//--------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (s_aEnemy[nCntEnemy].bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, s_pTex[s_aEnemy[nCntEnemy].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntEnemy * 4,
				2);
		}
	}
}

//--------------------------------------------------
// セット
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, float Size, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse)
		{
			continue;
		}

		// エネミーに情報を持たせる
		*pEnemy = s_aTypeEnemy[nType];

		// Useをtrueにする
		pEnemy->bUse = true;
		// 引数の設定
		pEnemy->pos = pos;
		pEnemy->Size = Size;
		pEnemy->nType = nType;
		//pEnemy->nLife = 1; //	InitEnemyで仮設定したので、ここではやらない

		// 頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y - pEnemy->Size, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->Size, pEnemy->pos.y + pEnemy->Size, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;
	}
	return pEnemy;
}

//--------------------------------------------------
// 当たり処理
// Auther：Isoe Jukia
//--------------------------------------------------
void HitEnemy(Enemy* pEnemy, int nDamage)
{
	pEnemy->nLife -= nDamage;

	//
	// ダメージ時のSE
	//
}

//--------------------------------------------------
// 追尾処理
// Auther：Isoe Jukia
// Auther：Yuda Kaito
//--------------------------------------------------
void TrackingMove(Enemy * pEnemy)
{
	// プレイヤー情報の取得
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
// 取得処理
// Auther：Isoe Jukia
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return s_aEnemy;
}
