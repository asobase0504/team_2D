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
#include "Boss.h"
#include "target.h"
#include "crater.h"
#include "sound.h"
#include "life.h"
#include <stdio.h>

//----------------------------
// マクロ
//----------------------------
#define MOVE_SPEED   (2.5f)
#define COLLISION	(25)
#define RADIUS		(5.0f)
#define SPEED		(2.0f)
#define JUDGEMENT	(100)	// 避ける敵の判定サイズ
#define UP_ESCAPE	(200)	// 上に逃げる敵の判定サイズ
#define ABILTIY		(0.005f)
#define MAXSTAGE	(5)
#define JUGE		(80)

//シオナイト
#define SHEO_DIST_START_ROTATION	(200.0f)							// 回り始める距離
#define SHEO_RADIUS_ROTATION		(SHEO_DIST_START_ROTATION - 20.0f)	// 回転半径
#define SHEO_SPEED_ROTATION			(D3DXToRadian(6.0f))				// 回転速度
#define SHEO_FRAME_GOODBY			(300)								// お帰りになる時間
#define SHEO_SPEED_GOODBY			(10.0f)								// お帰りになる速度

//**************************************************
// スタティック変数
//**************************************************
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//頂点バッファへのポインタ
static Enemy s_aEnemy[MAX_ENEMY];					// 敵の情報
static Enemy s_aTypeEnemy[ENEMYTYPE_MAX];
static char EnemyLink[MAXSTAGE][256];
static int count = 0;

//**************************************************
// プロトタイプ宣言
//**************************************************
static void TrackingMove(Enemy* pEnemy);	// 追尾処理
static void UpdateSky1(Enemy* pEnemy);		// 空の敵1の更新
static void UpdateSky2(Enemy* pEnemy);		// 空の敵2の更新
static void UpdateBuckSky(Enemy* pEnemy);	// 帰る空の敵の更新
static void MoveBakyura(Enemy* pEnemy);		// バキュラの移動処理
static void MoveSheonite(Enemy* pEnemy);	// シオナイトの移動処理
static void UpdateZakato1(Enemy* pEnemy);	// TOGETOGE
static void UpdateZakato2(Enemy* pEnemy);	// ザカートの処理2(追尾ナシ、一定の間隔で弾を発射して消滅)
static void UpdateZakato3(Enemy* pEnemy);	// ザカートの処理3(追尾アリ、時間経過で弾を発射して消滅)
static void UpdateZakato4(Enemy* pEnemy);	// ザカートの処理4(追尾ナシ、時間経過で弾を発射して消滅)
static void ZakatoFinish(Enemy* pEnemy);	// ザカートが終了する際の処理
static void ReflectMove(Enemy* pEnemy);
static void Updateflag(Enemy* pEnemy);		// フラックの処理

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

	ZeroMemory(s_aEnemy, sizeof(s_aEnemy[0]));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	count = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fSize, pEnemy->pos.y - pEnemy->fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fSize, pEnemy->pos.y - pEnemy->fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fSize, pEnemy->pos.y + pEnemy->fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fSize, pEnemy->pos.y + pEnemy->fSize, 0.0f);

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

	//テクスチャデータ読み込み
	LoadSetFile("data\\txt\\enemy.txt");
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
	Player* pPlayer = GetPlayer();
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (!pEnemy->bUse)
		{
			continue;
		}

		// 位置を更新
		pEnemy->pos.y += pEnemy->fSpeed;

		if (pEnemy->pos.y >= 0.0f - pEnemy->fSize)
		{
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
			case ENEMYTYPE_BAKYURA:		// 無敵壁
				MoveBakyura(pEnemy);
				break;
			case ENEMYTYPE_SHEONITE:	// 空の敵
				MoveSheonite(pEnemy);
				break;
			case ENEMYTYPE_GROUND_1:	// 直進敵
				pEnemy->nCntBullet++;

				if (pEnemy->nCntBullet >= 60)
				{
					// エネミーからプレイヤーまでの距離の算出
					D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

					//対角線の角度を算出
					Direction.z = atan2f(Direction.x, Direction.y);
					Direction.x = 0.0f;		// 使わない情報なので初期化
					Direction.y = 0.0f;		// 使わない情報なので初期化

					SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
					pEnemy->nCntBullet = 0;
				}
				break;
			case ENEMYTYPE_GROUND_2:	// 動かない敵

				break;
			case ENEMYTYPE_WARP_1:		//追尾アリ、一定の距離で弾を発射し、消滅	
				UpdateZakato1(pEnemy);
				break;
			case ENEMYTYPE_WARP_2:
				UpdateZakato2(pEnemy);
				break;
			case ENEMYTYPE_WARP_3:		//追尾アリ、時間経過で弾を発射して消滅
				UpdateZakato3(pEnemy);
				break;
			case ENEMYTYPE_WARP_4:		//追尾ナシ、時間経過で弾を発射して消滅
				UpdateZakato4(pEnemy);
				break;
			case  FLAG_STATE:			//フラグ
				Updateflag(pEnemy);
				if (pEnemy->FlagOn)
				{
					// 頂点カラーの設定
					SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
				}
				else
				{
					// 頂点カラーの設定
					SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));
				}
				break;
			default:
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
					pBullet->bUse = false;
				}
			}

			if (pEnemy->pos.y > 720.0f + pEnemy ->fSize+200.0f)
			{
				pEnemy->bTracking = false;
			}
			if (pEnemy->pos.y < 0.0f&&pEnemy->bBack)
			{
				pEnemy->bUse = false;
			}
			// エネミーのライフが0になったとき
			if (pEnemy->nLife <= 0)
			{
				pEnemy->bUse = false;
			}
		}
		// 頂点座標の設定
		SetVtxPos(pVtx, &pEnemy->pos, pEnemy->fSize, pEnemy->fSize);

		// テクスチャの設定
		SetVtxAnimTex(pVtx, &pEnemy->AnimTex);
		SetTex2d(pVtx, (1.0f / pEnemy->AnimTex.nDivisionX)*pEnemy->AnimTex.nPatternX
			, (1.0f / pEnemy->AnimTex.nDivisionX)* (pEnemy->AnimTex.nPatternX + 1)
			, 1.0f / pEnemy->AnimTex.nDivisionY*pEnemy->AnimTex.nPatternY
			, 1.0f / pEnemy->AnimTex.nDivisionY*(pEnemy->AnimTex.nPatternY + 1));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
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

	//pEnemy->move.y = pEnemy->fSpeed;
	ReflectMove(pEnemy);

	// 弾を出す処理
	if (pEnemy->pos.y > 0.0f)
	{	// 画面内に収まっている場合
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 60)
		{
			// エネミーからプレイヤーまでの距離の算出
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//対角線の角度を算出
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// 使わない情報なので初期化
			Direction.y = 0.0f;		// 使わない情報なので初期化

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
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
		if (pEnemy->nCntBullet >= 60)
		{
			// エネミーからプレイヤーまでの距離の算出
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//対角線の角度を算出
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// 使わない情報なので初期化
			Direction.y = 0.0f;		// 使わない情報なので初期化

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
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
	{
		pEnemy->move.y += easeInOutBack(-0.25f);
	}
	// 追尾の処理
	TrackingMove(pEnemy);

	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	if (CollisionCircle(pEnemy->pos, 100, pPlayer->pos, pPlayer->size.x))
	{// 敵が逃げる処理
		pEnemy->bTracking = false;		//追尾
		pEnemy->bBack = true;		//戻る処理
	}

	// 弾を出す処理
	if (pEnemy->pos.y > 0.0f)
	{	// 画面内に収まっている場合
		pEnemy->nCntBullet++;
		if (pEnemy->nCntBullet >= 60)
		{
			// エネミーからプレイヤーまでの距離の算出
			D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

			//対角線の角度を算出
			Direction.z = atan2f(Direction.x, Direction.y);
			Direction.x = 0.0f;		// 使わない情報なので初期化
			Direction.y = 0.0f;		// 使わない情報なので初期化

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
			pEnemy->nCntBullet = 0;
		}
	}
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
			//s_aEnemy[nCnt].bUse = false;
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
		{	// シオナイトの帰還
			distance = SHEO_DIST_START_ROTATION;
			pEnemy->move.x = sinf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
			pEnemy->move.y = cosf(pEnemy->fSheoRot) * SHEO_SPEED_GOODBY;
		}
		// プレイヤーの位置からシオナイトの位置を算出
		pEnemy->pos.x = pPlayer->pos.x + distance * sinf(pEnemy->fSheoRot);
		pEnemy->pos.y = pPlayer->pos.y + distance * cosf(pEnemy->fSheoRot);
	}
}

//-------------------------------------------------
//	ザカート1の敵の更新処理
// Author：Tomidokoro Tomoki
//-------------------------------------------------
void UpdateZakato1(Enemy* pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();


	float fRotMove, fRotDest, fRotDiff;

	fRotMove = atan2f(pEnemy->move.x, pEnemy->move.y);			//現在の移動方向

	fRotDest = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);			//目的の移動方向

	fRotDiff = fRotDest - fRotMove; 	//目的の移動方向までの差分

	//角度の値を修正する
	if (fRotDiff > D3DX_PI)
	{//目的の移動方向への差分が3.14より上の場合
		fRotDiff -= D3DX_PI * 2;
	}

	else if (fRotDiff < -D3DX_PI)
	{//目的の移動方向への差分が3.14より下の場合
		fRotDiff += D3DX_PI * 2;
	}

	fRotMove += fRotDiff * ABILTIY;				//移動方向の補正

	//move 修正
	if (fRotMove > D3DX_PI)
	{//現在の移動方向の角度が3.14を超えた場合
		fRotMove -= D3DX_PI * 2;
	}

	else if (fRotMove < -D3DX_PI)
	{//現在の移動方向の角度が3.14より下の場合
		fRotMove += D3DX_PI * 2;
	}

	pEnemy->move.x = sinf(fRotMove) * MOVE_SPEED;
	pEnemy->move.y = cosf(fRotMove) * MOVE_SPEED;

	pEnemy->bTracking = false;

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

			SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
			pEnemy->nCntBullet = 0;
		}
	}
}

//-------------------------------------------------
//	ザカート2の敵の更新処理
//-------------------------------------------------
void UpdateZakato2(Enemy* pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	//追尾ナシ、一定の距離で弾を発射し、消滅
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2, 0.0f);
		pEnemy->bTracking = false;		//追尾
		pEnemy->bTP = true;
	}

	if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
		&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
		&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
		&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
	{//敵が終了する処理
		ZakatoFinish(pEnemy);
	}
}

//-------------------------------------------------
//	ザカート3の敵の更新処理
//-------------------------------------------------
void UpdateZakato3(Enemy* pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	//追尾アリ、一定の距離で弾を発射し、消滅
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
		pEnemy->bTP = true;
	}
	if (pEnemy->nCntBullet >= 110)
	{// 敵が消える処理
		ZakatoFinish(pEnemy);
	}
}

//-------------------------------------------------
//	ザカート4の敵の更新処理
//-------------------------------------------------
void UpdateZakato4(Enemy* pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	//追尾ナシ、時間経過で弾を発射して消滅
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
		pEnemy->bTracking = false;		//追尾
		pEnemy->bTP = true;
	}
	if (pEnemy->nCntBullet >= 110)
	{// 敵が消える処理
		ZakatoFinish(pEnemy);
	}
}

//=============================================
// ザカート(テレポートする敵)の終了する際の処理
//=============================================
void ZakatoFinish(Enemy* pEnemy)
{
	Player* pPlayer = GetPlayer();

	// エネミーからプレイヤーまでの距離の算出
	D3DXVECTOR3 Direction = pEnemy->pos - pPlayer->pos;

	//対角線の角度を算出
	Direction.z = atan2f(Direction.x, Direction.y);
	Direction.x = 0.0f;		// 使わない情報なので初期化
	Direction.y = 0.0f;		// 使わない情報なので初期化

	//弾の発射
	SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fSize, 0.0f), Direction, BULLETTYPE_ENEMY, -1, true);
	pEnemy->nCntBullet = 0;
	pEnemy->bTracking = false;		//追尾
	pEnemy->bTP = false;
	pEnemy->bUse = false;		//エネミーを無効化する
}

//--------------------------------------------------
// 描画
// Author：Isoe Jukia
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
		// テクスチャの設定
		pDevice->SetTexture(0, s_pTex[s_aEnemy[nCntEnemy].nType]);

		if (!s_aEnemy[nCntEnemy].bUse)
		{
			continue;
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntEnemy * 4,2);
	}
}

//--------------------------------------------------
// 設定
// Author：Isoe Jukia
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, float fSize, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse)
		{
			continue;
		}

		// エネミーに情報を持たせる
		*pEnemy = s_aTypeEnemy[nType];

		pEnemy->bUse = true;	// 使用している状態に移行

		// 引数の設定
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

		// 頂点座標の設定
		SetVtxPos(pVtx, &pEnemy->pos, pEnemy->fSize, pEnemy->fSize);
		// 頂点カラーの設定
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		break;
	}

	// 頂点をアンロックする
	s_pVtxBuff->Unlock();

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
		pEnemy->move.x = (moveEnemyX / moveEnemyR) * pEnemy->fSpeed;
		pEnemy->move.y = (moveEnemyY / moveEnemyR) * pEnemy->fSpeed;
		if (pEnemy->pos.y >= pPlayer->pos.y)
		{
			pEnemy->bTracking = false;
		}
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

//----------------------------
//ファイルの入力マップ情報
// Author：Hamada Ryuga
//----------------------------
void LoadSetFile(char *Filename)
{
	char	s_aString[256];//
	int		Num_Tex = 0;
	int     MoveSet = 0;

	// ファイルポインタの宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(Filename, "r");
	int nCntEnemy = 0;

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み

			fscanf(pFile, "%s", &s_aString[0]);

			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//これのあとコメント
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "NUM_TEXTURE") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ

				fscanf(pFile, "%d", &Num_Tex);

			}
			if (strcmp(&s_aString[0], "DIVISION") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionX);
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nDivisionY);
			}
			if (strcmp(&s_aString[0], "ANIMESPEED") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].AnimTex.nSpeed);
			}
			if (strcmp(&s_aString[0], "MOVESPEED") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%f", &s_aTypeEnemy[nCntEnemy].fSpeed);
			}
			if (strcmp(&s_aString[0], "TRACKING") == 0)
			{
				int checker = 0;
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
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
				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%d", &s_aTypeEnemy[nCntEnemy].nLife);
			}
			if (strcmp(&s_aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &s_aString[0]);	//＝読み込むやつ
				fscanf(pFile, "%s", &s_aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//デバイスの取得
				pDevice = GetDevice();
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					&s_aString[0],
					&s_pTex[(ENEMYTYPE)nCntEnemy]);
			}
			if (strcmp(&s_aString[0], "END_TEX") == 0)
			{
				nCntEnemy++;

			}

			if (strcmp(&s_aString[0], "TYPESTATE") == 0)
			{// 文字列が一致した場合
				while (strncmp(&s_aString[0], "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ

					if (strcmp(&s_aString[0], "MOVESPEED") == 0)
					{
						//fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						//fscanf(pFile, "%f", &s_aTypeEnemy[MoveSet].fSpeed);
					}
					if (strcmp(&s_aString[0], "BACK") == 0)
					{
						int checker = 0;
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
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
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
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
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &s_aTypeEnemy[MoveSet].nLife);
					}
					if (strcmp(&s_aString[0], "ENDSTATE") == 0)
					{
						MoveSet++;
					}
				}

			}
			if (strcmp(&s_aString[0], "SET") == 0)
			{// 文字列が一致した場合
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int  nType = 0;
				float  fSize = 0.0f;
				nCntEnemy = 0;
				while (1)
				{//188 2900
					// 文字列の初期化と読み込み
					s_aString[0] = {};
					fscanf(pFile, "%s", &s_aString[0]);

					if (strncmp(&s_aString[0], "#", 1) == 0)
					{//これのあとコメント
						fgets(&s_aString[0], sizeof(s_aString), pFile);
						continue;
					}

					if (strcmp(&s_aString[0], "POS") == 0)
					{// 文字列が一致した場合

						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&s_aString[0], "TYPE") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nType);
						if (nType >= (int)ENEMYTYPE_MAX)
						{//列挙型の数より大きかったら数字を初期化する
							nType = 0;
						}
					}
					if (strcmp(&s_aString[0], "SIZE") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &fSize);
					}
					if (strcmp(&s_aString[0], "END_SET") == 0)
					{//セット
						SetEnemy(D3DXVECTOR3(s_modelpos),fSize,(ENEMYTYPE)nType);
						break;
					}
					if (strcmp(&s_aString[0], "END_SCRIPT") == 0)
					{// 文字列が一致した場合
						break;
					}
				}
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}

//--------------------
//指定したfileパスをよみこむ
//--------------------
void EnemyLynk(char *Filename)
{
	if (EnemyLink[count][0] == '\0')
	{
		strcat(&EnemyLink[count][0], &Filename[0]);//合成　aFile＜-こいつに入れる
		count++;
	}
}

//--------------------
//指定したfileパスを呼び出す
//--------------------
void SetEnemyLynk(int number)
{
	LoadSetFile(&EnemyLink[number][0]);
}

//--------------------
// 当たり判定
//--------------------
void ReflectMove(Enemy* pEnemy)
{
	Player* pPlayer = GetPlayer();

	if (pEnemy->bReflect)
	{
		if (pEnemy->pos.x >= pPlayer->pos.x - pPlayer->size.x
			&& pEnemy->pos.x <= pPlayer->pos.x + pPlayer->size.x)
		{//敵が終了する処理			
			pEnemy->bReflect = false;
			pEnemy->bTracking = false;
			pEnemy->move.x += -pEnemy->move.x * 3;
			//			pEnemy->move.y += -pEnemy->move.y * 3;
		}
	}
}

//--------------------
//敵消す
// Author：Hamada Ryuga
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
//旗
// Author : 髙野馨將
// Author：Hamada Ryuga
//--------------------
void Updateflag(Enemy* pEnemy)	// 
{
	// 弾が当たった時
	Bullet* pBullet = GetBullet();
	if (CollisionCircle(pEnemy->pos, COLLISION, pBullet->pos, pBullet->size.x))
	{
		//ここでフラックの音

		pEnemy->FlagOn = true;
	}

	Player* pPlayer = GetPlayer();
	if ((CollisionCircle(pEnemy->pos, COLLISION, pPlayer->pos, pPlayer->size.x)) && pEnemy->FlagOn)
	{
		pEnemy->bUse = false;
		SetLife();
	}


}