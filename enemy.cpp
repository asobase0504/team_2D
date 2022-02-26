//=========================================
// 
// 敵処理
// Author : Isoe Jukia
// Author : 髙野馨將
// 
//=========================================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "Calculation.h"
#include "common.h"
#include "collision.h"
#include "score.h"
#include "player.h"
#include <stdio.h>

//----------------------------
// マクロ
//----------------------------
#define COLLISION	(25)
#define JUDGEMENT	(100)	//避ける敵の判定サイズ
#define UP_ESCAPE	(200)	//上に逃げる敵の判定サイズ

//----------------------------
// static 変数
//----------------------------
static LPDIRECT3DTEXTURE9 s_pTex[ENEMYTYPE_MAX] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		//頂点バッファへのポインタ
static Enemy s_aEnemy[MAX_ENEMY];

//----------------------------
// プロトタイプ宣言
//----------------------------
static void TrackingMove(Enemy* pEnemy);	// 追尾処理
static void UpdateSky1(Enemy* pEnemy);		// 空の敵1の更新
static void UpdateSky2(Enemy* pEnemy);		// 空の敵2の更新
static void UpdateBuckSky(Enemy* pEnemy);	// 帰る空の敵の更新
static void UpdateZakato1(Enemy* pEnemy);	//ザカートの処理1(追尾アリ、一定の間隔で弾を発射して消滅)
static void UpdateZakato2(Enemy* pEnemy);	//ザカートの処理2(追尾ナシ、一定の間隔で弾を発射して消滅)
static void UpdateZakato3(Enemy* pEnemy);	//ザカートの処理3(追尾アリ、時間経過で弾を発射して消滅)
static void UpdateZakato4(Enemy* pEnemy);	//ザカートの処理4(追尾ナシ、時間経過で弾を発射して消滅)
static void ZakatoFinish(Enemy* pEnemy);	//ザカートが終了する際の処理

//--------------------------------------------------
// 初期化
// Author：Isoe Jukia
//--------------------------------------------------
void InitEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 目玉
		&s_pTex[ENEMYTYPE_SKY_1]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 車
		&s_pTex[ENEMYTYPE_SKY_2]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"date/image/enemy/enemy 1.png",				// 
		&s_pTex[ENEMYTYPE_SKY_3]);

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
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

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
		SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

		// テクスチャ座標の設定	   u      v
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / 12.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 12.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点をアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了
// Author：Isoe Jukia
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

	// 頂点バッファ破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//
// 更新
//
// Author：Isoe Jukia
// Author：Takano Keisuke
// Author：Yuda Kaito
// Author：Tomidokoro Tomoki
// Author：Buriya Kouta
//
//--------------------------------------------------
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		Enemy* pEnemy = &s_aEnemy[nCntEnemy];

		if (!pEnemy->bUse)
		{
			continue;	//処理を１回飛ばす　[1]
		}

		TrackingMove(pEnemy);	// 追尾処理

		// 種類ごとの更新処理
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

			pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;
		case ENEMYTYPE_BAKYURA:		// 無敵壁

			break;
		case ENEMYTYPE_SHEONITE:	// 空の敵

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
		default:
			break;
		}

		// 位置を更新
		pEnemy->pos += pEnemy->move;

		// 画面端設定
		if (pEnemy->pos.x <= -100.0f || pEnemy->pos.x >= SCREEN_WIDTH + 100.0f || pEnemy->pos.y >= SCREEN_HEIGHT + 100.0f || pEnemy->pos.y <= -100.0f)
		{
			pEnemy->bUse = false;
			pEnemy->pos.x = SCREEN_WIDTH;
		}

		// 弾との当たり判定
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

		VERTEX_2D* pVtx;
		// 頂点バッファをロックし、頂点データへのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;	// 頂点座標データのポインタを４つ分進める		

		// 頂点座標の設定
		SetVtxPos(pVtx, &pEnemy->pos, ENEMY_WIDTH, ENEMY_HEIGHT);

		// テクスチャの設定
		SetVtxAnimTex(pVtx, &pEnemy->nCounterAnim, 10, &pEnemy->nPatternAnim, 12);

		switch (pEnemy->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;
		case  ENEMYSTATE_DAMAGE:
			pEnemy->nCounterState--;
			if (pEnemy->nCounterState <= 0)
			{
				pEnemy->nCounterState = ENEMYSTATE_NORMAL;

				// 頂点カラーの設定
				SetVtxColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			}
			break;
		}

		// 頂点をアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// 描画
// Author：Isoe Jukia
//--------------------------------------------------
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
}

//--------------------------------------------------
// 設定
// Author：Isoe Jukia
//--------------------------------------------------
Enemy* SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nType)
{
	Enemy* pEnemy = s_aEnemy;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse)
		{// 敵が使用されている場合
			continue;
		}

		pEnemy->pos = pos;
		pEnemy->nLife = 1;
		pEnemy->bUse = true;	// 使用している状態
		pEnemy->flg = true;
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
	return pEnemy;
}

//--------------------------------------------------
// 当たり処理
// Author：Isoe Jukia
//--------------------------------------------------
void HitEnemy(Enemy* pEnemy, int nDamage)
{
	pEnemy->nLife -= nDamage;

	//
	// ダメージ時のSE
	//
}

//--------------------------------------------------
// 取得処理
// Author：Isoe Jukia
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return s_aEnemy;
}

//--------------------------------------------------
// 追尾処理
// Author：Isoe Jukia
// Author：Yuda Kaito
//--------------------------------------------------
void TrackingMove(Enemy * pEnemy)
{
	// プレイヤー情報の取得
	Player* pPlayer = GetPlayer();

	if (pEnemy->flg)	//追尾
	{//true の時
		float moveEnemyX = pPlayer->pos.x - pEnemy->pos.x;
		float moveEnemyY = pPlayer->pos.y - pEnemy->pos.y;
		float moveEnemyR = sqrtf(moveEnemyX * moveEnemyX + moveEnemyY * moveEnemyY);
		pEnemy->move.x = (moveEnemyX / moveEnemyR) * 3.0f;
		pEnemy->move.y = (moveEnemyY / moveEnemyR) * 3.0f;
	}
}

//----------------------------
//ファイルの入力マップ情報
// Author：Hamada Ryuga
//----------------------------
void LoadSetFile(char *Filename)
{
	// ファイルポインタの宣言
	FILE* pFile;
	char aString[255];
	int Num_Tex;

	//ファイルを開く
	pFile = fopen(&Filename[0], "r");
	int nCntEnemy = 0;
	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			aString[0] = {};
			fscanf(pFile, "%s", &aString[0]);
		}
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "NUM_TEXTURE") == 0)
			{
				fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
				fscanf(pFile, "%d", &Num_Tex);

			}

			if (strcmp(&aString[0], "TEXTURE_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
				fscanf(pFile, "%s", &aString[0]);
				LPDIRECT3DDEVICE9  pDevice;
				//デバイスの取得
				pDevice = GetDevice();
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					&aString[0],
					&s_pTex[nCntEnemy]);
				nCntEnemy++;
			}

			if (strcmp(&aString[0], "SET") == 0)
			{// 文字列が一致した場合
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int  nType = 0;
				float  fSize = 0.0f;
				while (1)
				{
					// 文字列の初期化と読み込み
					aString[0] = {};
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{//これのあとコメント
						fgets(&aString[0], sizeof(aString), pFile);
						continue;
					}

					if (strcmp(&aString[0], "POS") == 0)
					{// 文字列が一致した場合

						fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&aString[0], "TYPE") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&aString[0], "SIZE") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &fSize);
					}
					if (strcmp(&aString[0], "END_SET") == 0)
					{//アイテムの場合
						SetEnemy(D3DXVECTOR3(s_modelpos), (ENEMYTYPE)nType);
						break;
					}
					if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{// 文字列が一致した場合
						break;
					}
				}
			}
		}
	}

	//ファイルを閉じる
	fclose(pFile);
}

//--------------------------------------------------
// スカイ1の敵の更新処理
// Author：Isoe Jukia
// Author：Yuda Kaito
//--------------------------------------------------
void UpdateSky1(Enemy* pEnemy)
{
	if (pEnemy->nLife > 0)
	{	// 生きている場合
		// プレイヤー情報の取得
		Player* pPlayer = GetPlayer();

		if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
			&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
			&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
			&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
		{//敵が逃げる処理

			pEnemy->flg = false;
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

				SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
				pEnemy->nCntBullet = 0;
			}
		}
	}
	else
	{
		pEnemy->bUse = false;
	}
}

//--------------------------------------------------
// スカイ2の敵の更新処理
// Author：Isoe Jukia
// Author：Yuda Kaito
//--------------------------------------------------
void UpdateSky2(Enemy* pEnemy)
{
	if (pEnemy->nLife > 0)
	{	// 生きている場合
		// プレイヤー情報の取得
		Player* pPlayer = GetPlayer();

		if (pEnemy->pos.x + JUDGEMENT >= pPlayer->pos.x - JUDGEMENT
			&& pEnemy->pos.x - JUDGEMENT <= pPlayer->pos.x + JUDGEMENT
			&& pEnemy->pos.y + JUDGEMENT >= pPlayer->pos.y - JUDGEMENT
			&& pEnemy->pos.y - JUDGEMENT <= pPlayer->pos.y + JUDGEMENT)
		{// 敵が逃げる処理
			pEnemy->flg = false;
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

				SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
				pEnemy->nCntBullet = 0;
			}
		}
	}
	else
	{
		pEnemy->bUse = false;
	}
}

//--------------------------------------------------
// スカイ3の敵の更新処理
// Author：Isoe Jukia
// Author：Yuda Kaito
// Author：Tomidokoro Tomoki
//--------------------------------------------------
void UpdateBuckSky(Enemy* pEnemy)
{
	if (pEnemy->nLife > 0)
	{	// 生きている場合
		if (pEnemy->Buk)	//戻る処理
		{//true の時
			pEnemy->move.y += easeInOutBack(-0.25f);
		}

		// プレイヤー情報の取得
		Player* pPlayer = GetPlayer();

		if (pEnemy->pos.x + UP_ESCAPE >= pPlayer->pos.x - UP_ESCAPE
			&& pEnemy->pos.x - UP_ESCAPE <= pPlayer->pos.x + UP_ESCAPE
			&& pEnemy->pos.y + UP_ESCAPE >= pPlayer->pos.y - UP_ESCAPE
			&& pEnemy->pos.y - UP_ESCAPE <= pPlayer->pos.y + UP_ESCAPE)
		{// 敵が逃げる処理
			pEnemy->flg = false;		//追尾
			pEnemy->Buk = true;		//戻る処理
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

				SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
				pEnemy->nCntBullet = 0;
			}
		}
	}
	else
	{
		pEnemy->bUse = false;
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

	//追尾アリ、一定の距離で弾を発射し、消滅
	if (!pEnemy->bTP)
	{
		pEnemy->pos = D3DXVECTOR3(pEnemy->pos.x, SCREEN_HEIGHT / 2, 0.0f);
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
		pEnemy->flg = false;		//追尾
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
		pEnemy->flg = false;		//追尾
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
	SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->SizeY, 0.0f), Direction, BULLETTYPE_ENEMY);
	pEnemy->nCntBullet = 0;
	pEnemy->flg = false;		//追尾
	pEnemy->bTP = false;
	pEnemy->bUse = false;		//エネミーを無効化する
}