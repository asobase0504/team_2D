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
#define NUM_RESULT	(4)					//テクスチャの最大数
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
	//SCORE *pScore = GetScore();	//スコアの取得

	//------------------------------
	//	テクスチャの読み込み
	//------------------------------
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Enemy\\sky001.jpg",
							  &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\Congratulation.png",
							  &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\flag.png",
							  &s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
							  "",
							  &s_pTexture[3]);

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
	//位置の初期化
	s_Result[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_Result[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 20.0f, 200.0f, 0.0f);
	s_Result[2].pos = D3DXVECTOR3(750.0f, 450.0f, 0.0f);
	s_Result[3].pos = D3DXVECTOR3(350.0f, 450.0f, 0.0f);

	//幅の初期化
	s_Result[0].fWidth = SCREEN_WIDTH / 2;
	s_Result[1].fWidth = 500.0f;
	s_Result[2].fWidth = 300.0f;
	s_Result[3].fWidth = 100.0f;

	//高さの初期化
	s_Result[0].fHeight = SCREEN_HEIGHT / 2;
	s_Result[1].fHeight = 100.0f;
	s_Result[2].fHeight = 50.0f;
	s_Result[3].fHeight = 100.0f;

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
		pVtx[0].col = result->col;
		pVtx[1].col = result->col;
		pVtx[2].col = result->col;
		pVtx[3].col = result->col;

		//テクスチャ座標の設定
		if (nCnt == 0)
		{//背景のテクスチャなら
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}
		else
		{//それ以外のテクスチャ
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//スコアの初期化処理
	InitScore();

	//スコアの設定処理
	//SetScore(pScore->nScore);
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
	//SCORE *pScore = GetScore();	//スコアの取得

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nTime++;				//タイムの加算
	s_nTime %= BLINK_TIME;	//タイムの初期化

	for (int nCnt = 0; nCnt < NUM_RESULT; nCnt++)
	{
		RESULT *result = s_Result + nCnt;

		//------------------------
		//	テクスチャの上下
		//------------------------
		if (s_nTime >= BLINK_HALF)
		{//タイムが150以上なら
			s_Result[2].pos.y -= 0.12f;	//上げる
			s_Result[3].pos.y -= 0.12f;
		}
		else
		{//タイムが150未満なら
			s_Result[2].pos.y += 0.12f;	//下げる
			s_Result[3].pos.y += 0.12f;
		}

		//------------------------
		//	背景のスクロール
		//------------------------
		if (nCnt == 0)
		{//背景のテクスチャなら
		 //テクスチャ座標の設定
			pVtx[0].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
			pVtx[1].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
			pVtx[2].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
			pVtx[3].tex += D3DXVECTOR2(MAP_SPEED, 0.0f);
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
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A,0))
	{
		SetFade(MODE_TITLE);
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