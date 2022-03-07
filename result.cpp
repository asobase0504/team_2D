//================================
//
// リザルト画面
// Author:Teruto Sato
//
//================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include <time.h>

//マクロ定義
#define NUM_RESULT	(6)					//テクスチャの最大数
#define BLINK_TIME	(300)				//点滅にかかる時間
#define BLINK_HALF	(BLINK_TIME / 2)	//点滅が切り替わる時間
#define MAP_SPEED	(0.0001f)			//画面スクロールの速さ

//スタティック変数
static LPDIRECT3DTEXTURE9			s_pTexture[NUM_RESULT] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//頂点バッファへのポインタ;
static RESULT s_Result[NUM_RESULT];	//リザルトの情報の取得
static int s_nTime;			//点滅の時間
static int s_nMapTime;

//============================
// リザルトの初期化処理
//============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//------------------------------
	//	テクスチャの読み込み
	//------------------------------
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Result_BG.png",
							  &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\ScoreLine.png",
							  &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\BestScoreLine.png",
							  &s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\RESULT.png",
							  &s_pTexture[3]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\player000.png",
							  &s_pTexture[4]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\ENEMY\\sky_enemy_001.png",
							  &s_pTexture[5]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RESULT,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//------------------------------
	//	構造体の初期化
	//------------------------------
	//リザルトの背景
	s_Result[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_Result[0].fWidth = SCREEN_WIDTH / 2;
	s_Result[0].fHeight = SCREEN_HEIGHT / 2;

	//スコアの線
	s_Result[1].pos = D3DXVECTOR3(500.0f, 280.0f, 0.0f);
	s_Result[1].fWidth = 350.0f;
	s_Result[1].fHeight = 50.0f;

	//ベストスコアの線
	s_Result[2].pos = D3DXVECTOR3(500.0f, 450.0f, 0.0f);
	s_Result[2].fWidth = 350.0f;
	s_Result[2].fHeight = 50.0f;

	//プレイヤーのテクスチャ
	s_Result[3].pos = D3DXVECTOR3(300.0f, 100.0f, 0.0f);
	s_Result[3].fWidth = 250.0f;
	s_Result[3].fHeight = 50.0f;

	//敵のテクスチャ
	s_Result[4].pos = D3DXVECTOR3(1050.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
	s_Result[4].fWidth = 80.0f;
	s_Result[4].fHeight = 80.0f;

	s_Result[5].pos = D3DXVECTOR3(100.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
	s_Result[5].fWidth = 80.0f;
	s_Result[5].fHeight = 80.0f;

	//色の初期化
	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		s_Result[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------
	//	頂点情報の設定
	//------------------------------
	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		RESULT *result = s_Result + nCnt;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y + result->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y + result->fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		if (nCnt == 0)
		{//背景のテクスチャなら
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		}
		else
		{//それ以外のテクスチャ
			pVtx[0].col = result->col;
			pVtx[1].col = result->col;
			pVtx[2].col = result->col;
			pVtx[3].col = result->col;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//スコアの初期化処理
	InitScore();

	SetScorePos(D3DXVECTOR3(520.0f, 240.0f, 0.0f));
	SetBestScorePos(D3DXVECTOR3(520.0f, 410.0f, 0.0f));
}

//============================
// リザルトの終了処理
//============================
void UninitResult(void)
{
	//スコアの終了処理
	UninitScore();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	//頂点バッファの破壊
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================
// リザルトの更新処理
//============================
void UpdateResult(void)
{
	VERTEX_2D*pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nTime++;				//タイムの加算
	s_nTime %= BLINK_TIME;	//タイムの初期化

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		RESULT *result = s_Result + nCnt;

		if (s_nTime >= BLINK_HALF)
		{
			s_Result[4].pos.y += 0.6f;
			s_Result[5].pos.x -= 0.8f;
		}
		else
		{
			s_Result[4].pos.y -= 0.6f;
			s_Result[5].pos.x += 0.8f;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y - result->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(result->pos.x - result->fWidth, result->pos.y + result->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(result->pos.x + result->fWidth, result->pos.y + result->fHeight, 0.0f);

		pVtx += 4;
	}

	//----------------
	//	画面遷移
	//----------------
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A, 0))
	{
		ChangeMode(MODE_RANKING);
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//スコアの更新処理
	UpdateScore();
}

//============================
// リザルトの描画処理
//============================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

												//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[nCnt]);

		//リザルトの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCnt * 4,				//描画する最初の頂点インデックス
			2);						//描画するプリミティブ数
	}

	//スコアの描画処理
	DrawScore();
}