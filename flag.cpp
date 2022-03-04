//========================================================
//
//SPフラッグ
//Author:髙野馨將
//
//========================================================
#include "flag.h"
#include "player.h"
#include "bullet.h"
//====================================
//マクロ定義
//====================================
#define MAX_FLAG		(128)		//SPフラッグの最大数
#define NUM_FLAG		(2)			//SPフラッグの種類
//====================================
//グローバル変数
//====================================
LPDIRECT3DTEXTURE9 g_pTextureFlag[NUM_FLAG] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFlag = NULL;		//頂点バッファへのポインタ
Flag g_aFlag[MAX_FLAG];								//SPフラッグの情報
//====================================
//SPフラッグの初期化処理
//====================================
void InitFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntFlag;

	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag001.png",
		&g_pTextureFlag[0]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag000.png",
		&g_pTextureFlag[1]);
	//SPフラッグの情報の初期化
	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		g_aFlag[nCntFlag].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFlag[nCntFlag].state = FLAG_STATE_OFF;
		g_aFlag[nCntFlag].fHeight = 0.0f;
		g_aFlag[nCntFlag].fWidth = 0.0f;
		g_aFlag[nCntFlag].fAngle = 0.0f;
		g_aFlag[nCntFlag].fLength = 0.0f;
		g_aFlag[nCntFlag].nType = 0;
		g_aFlag[nCntFlag].bUse = false;	//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FLAG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFlag,
		NULL);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
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
	g_pVtxBuffFlag->Unlock();
}
//====================================
//SPフラッグの終了処理
//====================================
void UninitFlag(void)
{
	//テクスチャの破棄
	if (g_pTextureFlag[NUM_FLAG] != NULL)
	{
		g_pTextureFlag[NUM_FLAG]->Release();
		g_pTextureFlag[NUM_FLAG] = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffFlag != NULL)
	{
		g_pVtxBuffFlag->Release();
		g_pVtxBuffFlag = NULL;
	}
}
//====================================
//SPフラッグの更新処理
//====================================
void UpdateFlag(void)
{

		VERTEX_2D * pVtx;		//頂点情報へのポインタ
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
		{
			if (g_aFlag[nCntFlag].nType == 0)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

				pVtx += 4;	//頂点データのポインタを4つ分進める
			}
			else
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx += 4;	//頂点データのポインタを4つ分進める
			}
		}
		//頂点バッファをアンロックする
		g_pVtxBuffFlag->Unlock();
}
//====================================
//SPフラッグの描画処理
//====================================
void DrawFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntFlag;

	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFlag, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == true)
		{//SPフラッグが使用されている
		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureFlag[g_aFlag[nCntFlag].nType]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFlag * 4, 2);	//プリミティブ(ポリゴン)数
		}
	}
}
//====================================
//SPフラッグの削除処理
//====================================
void DeleteFlag(int nCntFlag)
{
	//SPフラッグの非表示
	g_aFlag[nCntFlag].bUse = false;
}
//====================================
//SPフラッグの設定処理
//====================================
void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	int nCntFlag;
	VERTEX_2D * pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == false)
		{//SPフラッグが使用されていない
			g_aFlag[nCntFlag].pos = pos;
			g_aFlag[nCntFlag].fWidth = fWidth;
			g_aFlag[nCntFlag].fHeight = fHeight;
			g_aFlag[nCntFlag].nType = nType;
			g_aFlag[nCntFlag].state = FLAG_STATE_OFF;
			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2, g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2, 0.0f);

			g_aFlag[nCntFlag].bUse = true;		//使用している状態
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffFlag->Unlock();
}
//====================================
//SPフラッグ情報の取得
//====================================
Flag * GetFlag(void)
{
	return &g_aFlag[0];
}
//====================================
//SPフラッグの当たり判定(弾用)
//====================================
void CollisionFlagBullet(D3DXVECTOR3 pPos, float fWidth, float fHeight)
{
	Bullet *pBullet = GetBullet();
	Flag *pFlag = GetFlag();
	float RightPlayer = pPos.x + fWidth / 2;
	float LeftPlayer = pPos.x - fWidth / 2;
	float BottomPlayer = pPos.y;
	float TopPlayer = pPos.y - fHeight;
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		float  RightFlag = g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2;
		float  LeftFlag = g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2;
		float  BottomFlag = g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2;
		float  TopFlag = g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2;
		if (g_aFlag[nCntFlag].bUse == true)
		{
			if (RightFlag >= LeftPlayer&& LeftFlag <= RightPlayer && BottomFlag >= TopPlayer && TopFlag <= BottomPlayer)
			{
				if (g_aFlag[nCntFlag].nType == 0)
				{//弾との当たり判定
					g_aFlag[nCntFlag].nType = 1;
					pBullet->bUse = false;
				}
			}
		}
		pFlag++;
	}
}
//====================================
//SPフラッグの当たり判定(プレイヤー)
//====================================
void CollisionFlagPlayer(D3DXVECTOR3 pPos, float fWidth, float fHeight)
{
	Player *pPlayer = GetPlayer();
	Flag * pFlag = GetFlag();
	float RightPlayer = pPos.x + fWidth / 2;
	float LeftPlayer = pPos.x - fWidth / 2;
	float BottomPlayer = pPos.y;
	float TopPlayer = pPos.y - fHeight;
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		float  RightFlag = g_aFlag[nCntFlag].pos.x + g_aFlag[nCntFlag].fWidth / 2;
		float  LeftFlag = g_aFlag[nCntFlag].pos.x - g_aFlag[nCntFlag].fWidth / 2;
		float  BottomFlag = g_aFlag[nCntFlag].pos.y + g_aFlag[nCntFlag].fHeight / 2;
		float  TopFlag = g_aFlag[nCntFlag].pos.y - g_aFlag[nCntFlag].fHeight / 2;
		if (g_aFlag[nCntFlag].bUse == true)
		{
			if (RightFlag >= LeftPlayer&& LeftFlag <= RightPlayer && BottomFlag >= TopPlayer && TopFlag <= BottomPlayer)
			{
				if (g_aFlag[nCntFlag].nType == 1)
				{//プレイヤーとの当たり判定
					pPlayer->nLife++;
					DeleteFlag(nCntFlag);
				}
			}
		}
		pFlag++;
	}
}