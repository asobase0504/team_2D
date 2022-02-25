//----------------------------------------------
//
//Enemy.cpp
//磯江ジュキア
//
//----------------------------------------------

#include "main.h"
#include "enemy.h"
#include "Boss.h"
#include "player.h"

#define MAX_BOSS	(255)	//敵の数

static LPDIRECT3DTEXTURE9 s_TextureBOSS[ENEMYTYPE_MAX] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_PvtxBuffBOSS = NULL;			//頂点バッファへのポインタ
static BOSS s_aBoss[MAX_BOSS];
static Player s_Player;

// プロトタイプ宣言
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data/image/スライムA_移動000.png",				// 頑張って画像貼り付けてねｗ
		&s_TextureBOSS[BOSS_PARTS_1]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data/image/スライムB_移動000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_2]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data/image/スライムC_移動000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_3]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data/image/スライムD_移動000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_4]);

	// テクスチャ読み込み	敵テクスチャ
	D3DXCreateTextureFromFile(pDevice,
		"data/image/どくろA_攻撃000.png",				// 
		&s_TextureBOSS[BOSS_PARTS_5]);

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{
		s_aBoss[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aBoss[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aBoss[nCount].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		// 頂点フォーマット
		D3DPOOL_MANAGED,
		&s_PvtxBuffBOSS,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_PvtxBuffBOSS->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x - ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x + ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x - ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aBoss[nCntEnemy].pos.x + ENEMY_WIDTH, s_aBoss[nCntEnemy].pos.y + ENEMY_HEIGHT, 0.0f);

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
	s_PvtxBuffBOSS->Unlock();

}
void UninitBoss(void)
{
	for (int Count = 0; Count < ENEMYTYPE_MAX; Count++)
	{
		if (s_TextureBOSS[Count] != NULL)
		{
			s_TextureBOSS[Count]->Release();
			s_TextureBOSS[Count] = NULL;
		}
	}

	// 頂点バッファ破棄
	if (s_PvtxBuffBOSS != NULL)
	{
		s_PvtxBuffBOSS->Release();
		s_PvtxBuffBOSS = NULL;
	}
}
void UpdateBoss(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!s_aBoss->bUse)
		{
			continue;	//処理を１回飛ばす　[1]
		}
		if (s_aBoss->pos.x + COLLISION_B >= s_Player.pos.x - COLLISION_B
			&& s_aBoss->pos.x - COLLISION_B <= s_Player.pos.x + COLLISION_B
			&& s_aBoss->pos.y + COLLISION_B >= s_Player.pos.y - COLLISION_B
			&& s_aBoss->pos.y - COLLISION_B <= s_Player.pos.y + COLLISION_B)
		{//弾座標重なり

		 //HitPlayer(2);			//ダメージを受ける

		}

		if (s_aBoss->nLife > 0)
		{
			switch (s_aBoss->nType)
			{
			case BOSS_PARTS_1:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//敵が逃げる処理
					

				}
				break;
			case BOSS_PARTS_2:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//敵が逃げる処理
				 

				}
				break;
			case BOSS_PARTS_3:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//敵が逃げる処理
				
				
				}
				break;
			case BOSS_PARTS_4:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//敵が逃げる処理
				 

				}
				break;
			case BOSS_PARTS_5:
				if (s_aBoss->pos.x + JUDGEMENT_B >= s_Player.pos.x - JUDGEMENT_B
					&& s_aBoss->pos.x - JUDGEMENT_B <= s_Player.pos.x + JUDGEMENT_B
					&& s_aBoss->pos.y + JUDGEMENT_B >= s_Player.pos.y - JUDGEMENT_B
					&& s_aBoss->pos.y - JUDGEMENT_B <= s_Player.pos.y + JUDGEMENT_B)
				{//敵が逃げる処理
				

				}
				break;
			default:
				break;
			}
		}
		else
		{// 敵が死んだときの処理をここに撃ち込んでください


		}

		// 位置を更新
		s_aBoss->pos += s_aBoss->move;

		VERTEX_2D* pVtx;

		// 頂点バッファをロックし、頂点データへのポインタを取得
		s_PvtxBuffBOSS->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCntEnemy;	// 頂点座標データのポインタを４つ分進める		

		pVtx[0].pos = D3DXVECTOR3(s_aBoss->pos.x - ENEMY_WIDTH, s_aBoss->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aBoss->pos.x + ENEMY_WIDTH, s_aBoss->pos.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aBoss->pos.x - ENEMY_WIDTH, s_aBoss->pos.y + ENEMY_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aBoss->pos.x + ENEMY_WIDTH, s_aBoss->pos.y + ENEMY_HEIGHT, 0.0f);

		switch (s_aBoss->state)
		{
		case  ENEMYSTATE_NORMAL:
			break;

		case  ENEMYSTATE_DAMAGE:
			s_aBoss->nCounerState--;
			if (s_aBoss->nCounerState <= 0)
			{
				s_aBoss->nCounerState = ENEMYSTATE_NORMAL;

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		// 頂点をアンロックする
		s_PvtxBuffBOSS->Unlock();
	}
}
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_PvtxBuffBOSS, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, s_TextureBOSS[s_aBoss[nCntEnemy].nType]);

		if (!s_aBoss[nCntEnemy].bUse)
		{
			continue;
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntEnemy * 4,
			2);
	}
}
void SetBoss(D3DXVECTOR3 pos/*, TYPEENEMY nType*/)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (s_aBoss->bUse)
		{// 敵が使用されている場合
			continue;
		}

		SetEnemy(D3DXVECTOR3(pos.x-50.0f, pos.y+50.0f,0.0f), BOSS_PARTS_1);
		SetEnemy(D3DXVECTOR3(pos.x+50.0f, pos.y + 50.0f, 0.0f), BOSS_PARTS_2);
		SetEnemy(D3DXVECTOR3(pos.x -50.0f, pos.y-50.0f, 0.0f), BOSS_PARTS_3);
		SetEnemy(D3DXVECTOR3(pos.x+50.0f, pos.y -50.0f, 0.0f), BOSS_PARTS_4);
		SetEnemy(D3DXVECTOR3(pos.x+0.0f, pos.y + 0.0f, 0.0f), BOSS_PARTS_5);

		s_aBoss->pos = pos;
		//s_aBoss->nLife = 1;
		s_aBoss->bUse = true;	// 使用している状態	
		//s_aBoss->nCounter = 0;	// インクリメント
		//s_aBoss->nType = nType;

		VERTEX_2D*pVtx;
		// 頂点バッファをロックし、頂点データへのポインタを取得
		s_PvtxBuffBOSS->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * 4;	// 頂点座標データのポインタを４つ分進める

		// 頂点座標の設定
		pVtx[0].pos.x = s_aBoss->pos.x - ENEMY_WIDTH;
		pVtx[0].pos.y = s_aBoss->pos.y - ENEMY_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = s_aBoss->pos.x + ENEMY_WIDTH;
		pVtx[1].pos.y = s_aBoss->pos.y - ENEMY_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = s_aBoss->pos.x - ENEMY_WIDTH;
		pVtx[2].pos.y = s_aBoss->pos.y + ENEMY_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = s_aBoss->pos.x + ENEMY_WIDTH;
		pVtx[3].pos.y = s_aBoss->pos.y + ENEMY_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// 頂点をアンロックする
		s_PvtxBuffBOSS->Unlock();
		break;
	}
}


//Set　でおいていく
//プログラムの確認
//動きの処理
