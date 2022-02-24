//========================================================
//
//弾処理
//Author:髙野馨將
//
//========================================================
#include "enemybullet.h"
#include "enemy.h"
#include "player.h"
//====================================
//マクロ定義
//====================================
#define MAX_ENEMYBULLET		128		//弾の最大数
#define SIZE_BULLET			15.0f	//弾の大きさ
//====================================
//グローバル変数
//====================================
static LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
static EnemyBullet g_aBullet[MAX_ENEMYBULLET];		//弾の情報
//====================================
//メイン関数
//====================================

//====================================
//弾の初期化処理
//====================================
void InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet002.png",
		&g_pTextureBullet);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
		g_aBullet[nCntBullet].bChase = true;	//使用している状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMYBULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}
//====================================
//弾の終了処理
//====================================
void UninitEnemyBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//====================================
//弾の更新処理
//====================================
void UpdateEnemyBullet(void)
{
	int nCntBullet;
	Player * pPlayer = GetPlayer();
	enemy * pEnemy = GetEnemy();
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//弾が使用されている
		 //弾の位置更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			//頂点座標の更新
			VERTEX_2D * pVtx;		//頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntBullet * 4);		//ポインタを4つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			//頂点バッファをアンロックする
			g_pVtxBuffBullet->Unlock();

			if (g_aBullet[nCntBullet].pos.x <= -SIZE_BULLET || g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH + SIZE_BULLET || g_aBullet[nCntBullet].pos.y <= -SIZE_BULLET || g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT + SIZE_BULLET)
			{//弾が画面外に出た
				g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
			}
		}
	}
}
//====================================
//弾の描画処理
//====================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);
	for (nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
		 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);	//プリミティブ(ポリゴン)数
		}
	}
}
//====================================
//弾の設定処理
//====================================
void SetEnemyBullet(D3DXVECTOR3 pos)
{
	Player * pPlayer = GetPlayer();

	VERTEX_2D * pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_ENEMYBULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBullet].pos = pos;
			pVtx += (nCntBullet * 4);		//該当の位置まで進める
			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y - SIZE_BULLET, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + SIZE_BULLET, g_aBullet[nCntBullet].pos.y + SIZE_BULLET, 0.0f);
			g_aBullet[nCntBullet].nLife = 0;
			g_aBullet[nCntBullet].bChase = true;		//使用している状態
			g_aBullet[nCntBullet].bUse = true;		//使用している状態
			if (g_aBullet[nCntBullet].bChase)
			{
				//プレイヤーに向かって飛ぶ
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
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}