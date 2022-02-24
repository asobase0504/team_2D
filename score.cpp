//============================================================================================================
//
//チーム制作[score.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "score.h"

#include <stdio.h>

//============================================================================================================
//マクロ定義
//============================================================================================================
#define MAX_SCORE			(8)			//スコアの最大桁数
#define NOWSCORE_POS_X		(0.0f)		//現在のスコアの位置(X)
#define NOWSCORE_POS_Y		(10.0f)		//現在のスコアの位置(Y)
#define BESTSCORE_POS_X		(245.0f)	//ベストスコアの位置(X)
#define BESTSCORE_POS_Y		(10.0f)		//ベストスコアの位置(Y)
#define SCORE_WIDTH			(30)		//スコアの幅
#define SCORE_HEIGHT		(45)		//スコアの高さ
#define BESTSCORE_FILE		("data/txt/BestScore.txt")	// ベストスコアの管理するファイル

//============================================================================================================
//スタティック変数
//============================================================================================================
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  s_pVtxBuffNowScore = NULL;		//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9  s_pVtxBuffBestScore = NULL;		//頂点バッファへのポインタ
static D3DXVECTOR3				s_posNowScore;					//現在のスコアの位置
static D3DXVECTOR3				s_posBestScore;					//ベストスコアの位置
static int						s_nNowScore;					//現在のスコアの値
static int						s_nBestScore;					//ベストスコアの値

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
static void SetNowScore(void);
static void SetBestScore(void);

//============================================================================================================
//スコアの初期化処理
//============================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/number000.png",
								&s_pTexture);

	//変数の初期化
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		s_posNowScore	= D3DXVECTOR3(NOWSCORE_POS_X, NOWSCORE_POS_Y, 0.0f);	//現在のスコアの位置
		s_posBestScore	= D3DXVECTOR3(BESTSCORE_POS_X, BESTSCORE_POS_Y, 0.0f);	//ベストスコアの位置
		s_nNowScore		= 0;													//スコアの値
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffNowScore,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffBestScore,
								NULL);

	/********************* 現在のスコアの方 *********************/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//頂点座標の設定
		float fLeft		= (s_posNowScore.x + (nCnt * SCORE_WIDTH));
		float fRight	= (s_posNowScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH);
		float fUp		= (s_posNowScore.y);
		float fDown		= (s_posNowScore.y + SCORE_HEIGHT);

		pVtx[0].pos = D3DXVECTOR3(fLeft,	fUp,	0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight,	fUp,	0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,	fDown,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight,	fDown,	0.0f);

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

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffNowScore->Unlock();

	/********************* ベストスコアの方 *********************/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBestScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//頂点座標の設定
		float fLeft		= (s_posBestScore.x + (nCnt * SCORE_WIDTH));
		float fRight	= (s_posBestScore.x + (nCnt * SCORE_WIDTH) + SCORE_WIDTH);
		float fUp		= (s_posBestScore.y);
		float fDown		= (s_posBestScore.y + SCORE_HEIGHT);

		pVtx[0].pos = D3DXVECTOR3(fLeft,	fUp,	0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight,	fUp,	0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,	fDown,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight,	fDown,	0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffBestScore->Unlock();

	//現在のスコアとベストスコアをセット
	SetNowScore();
	SetBestScore();
}

//============================================================================================================
//スコアの終了処理
//============================================================================================================
void UninitScore(void)
{
	///保存するタイミングはご自由にどうぞ
	//ベストスコアの保存
	SaveBestScore();

	/*********** テクスチャの破棄 ***********/
	
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	/*********** 頂点バッファの破棄 ***********/
	
	if (s_pVtxBuffNowScore != NULL)
	{//現在のスコア
		s_pVtxBuffNowScore->Release();
		s_pVtxBuffNowScore = NULL;
	}

	if (s_pVtxBuffBestScore != NULL)
	{//ベストスコア
		s_pVtxBuffBestScore->Release();
		s_pVtxBuffBestScore = NULL;
	}
}

//============================================================================================================
//スコアの更新処理
//============================================================================================================
void UpdateScore(void)
{
}

//============================================================================================================
//スコアの描画処理
//============================================================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	/********************* 現在のスコアの方 *********************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffNowScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		//スコアの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								nCnt * 4,				//描画する最初の頂点インデックス
								2 * MAX_SCORE);			//描画するプリミティブ数
	}

	/********************* ベストスコアの方 *********************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBestScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		//スコアの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								nCnt * 4,				//描画する最初の頂点インデックス
								2 * MAX_SCORE);			//描画するプリミティブ数
	}
}

//============================================================================================================
//現在のスコアの設定処理
//============================================================================================================
void SetNowScore(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);

	//現在のスコアを0に設定する
	s_nNowScore = 0;

	//スコアの値に合わせてテクスチャを変更
	aPosTexU[0] = s_nNowScore % 100000000 / 10000000;
	aPosTexU[1] = s_nNowScore % 10000000 / 1000000;
	aPosTexU[2] = s_nNowScore % 1000000 / 100000;
	aPosTexU[3] = s_nNowScore % 100000 / 10000;
	aPosTexU[4] = s_nNowScore % 10000 / 1000;
	aPosTexU[5] = s_nNowScore % 1000 / 100;
	aPosTexU[6] = s_nNowScore % 100 / 10;
	aPosTexU[7] = s_nNowScore % 10 / 1;

	//テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffNowScore->Unlock();
}

//============================================================================================================
//ベストスコアの設定処理
//============================================================================================================
void SetBestScore(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBestScore->Lock(0, 0, (void**)&pVtx, 0);

	//読み込んだベストスコアを設定する
	s_nBestScore = LoadBestScore();

	//スコアの値に合わせてテクスチャを変更
	aPosTexU[0] = s_nBestScore % 100000000 / 10000000;
	aPosTexU[1] = s_nBestScore % 10000000 / 1000000;
	aPosTexU[2] = s_nBestScore % 1000000 / 100000;
	aPosTexU[3] = s_nBestScore % 100000 / 10000;
	aPosTexU[4] = s_nBestScore % 10000 / 1000;
	aPosTexU[5] = s_nBestScore % 1000 / 100;
	aPosTexU[6] = s_nBestScore % 100 / 10;
	aPosTexU[7] = s_nBestScore % 10 / 1;

	//テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffBestScore->Unlock();
}

//============================================================================================================
//スコアの加算処理
//============================================================================================================
void AddScore(int nValue)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);

	//スコアの値に任意の値を加算する
	s_nNowScore += nValue;

	//スコアの値に合わせてテクスチャを変更
	aPosTexU[0]	= s_nNowScore % 100000000 / 10000000;
	aPosTexU[1] = s_nNowScore % 10000000 / 1000000;
	aPosTexU[2] = s_nNowScore % 1000000 / 100000;
	aPosTexU[3] = s_nNowScore % 100000 / 10000;
	aPosTexU[4] = s_nNowScore % 10000 / 1000;
	aPosTexU[5] = s_nNowScore % 1000 / 100;
	aPosTexU[6] = s_nNowScore % 100 / 10;
	aPosTexU[7] = s_nNowScore % 10 / 1;

	//テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffNowScore->Unlock();

	if (s_nNowScore >= s_nBestScore)
	{//現在のスコアがベストスコアを更新したとき
		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffBestScore->Lock(0, 0, (void**)&pVtx, 0);

		//「Best」と「Now」を連動して加算する
		s_nBestScore = s_nNowScore;

		//スコアの値に合わせてテクスチャを変更
		aPosTexU[0] = s_nBestScore % 100000000 / 10000000;
		aPosTexU[1] = s_nBestScore % 10000000 / 1000000;
		aPosTexU[2] = s_nBestScore % 1000000 / 100000;
		aPosTexU[3] = s_nBestScore % 100000 / 10000;
		aPosTexU[4] = s_nBestScore % 10000 / 1000;
		aPosTexU[5] = s_nBestScore % 1000 / 100;
		aPosTexU[6] = s_nBestScore % 100 / 10;
		aPosTexU[7] = s_nBestScore % 10 / 1;

		//テクスチャ座標の設定
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

			pVtx += 4;		//頂点データのポインタを４つ分進める
		}

		//頂点バッファをアンロックする
		s_pVtxBuffBestScore->Unlock();
	}
}

//============================================================================================================
//スコアの減算処理
//============================================================================================================
void SubScore(int nValue)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffNowScore->Lock(0, 0, (void**)&pVtx, 0);
	
	//スコアの値から任意の値を減算する
	s_nNowScore -= nValue;

	if (s_nNowScore <= 0)
	{//0以下だったら
		s_nNowScore = 0;
	}

	//スコアの値に合わせてテクスチャを変更
	aPosTexU[0] = s_nNowScore % 100000000 / 10000000;
	aPosTexU[1] = s_nNowScore % 10000000 / 1000000;
	aPosTexU[2] = s_nNowScore % 1000000 / 100000;
	aPosTexU[3] = s_nNowScore % 100000 / 10000;
	aPosTexU[4] = s_nNowScore % 10000 / 1000;
	aPosTexU[5] = s_nNowScore % 1000 / 100;
	aPosTexU[6] = s_nNowScore % 100 / 10;
	aPosTexU[7] = s_nNowScore % 10 / 1;

	//テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffNowScore->Unlock();
}

//============================================================================================================
//ベストスコアの保存
//============================================================================================================
void SaveBestScore(void)
{
	FILE *pFile = fopen(BESTSCORE_FILE, "w");		//ファイルを開く

	if (pFile != NULL)
	{//ファイルが開けた場合
		//ファイルにベストスコアをセーブする
		fprintf(pFile, "%d\n", s_nBestScore);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf(" *** ファイルが開けない…だと… *** \n");
	}
}

//============================================================================================================
//ベストスコアの読み込み
//============================================================================================================
int LoadBestScore(void)
{
	FILE *pFile = fopen(BESTSCORE_FILE, "r");		//ファイルを開く
	int nBestScore;									//ベストスコア読み込む用

	if (pFile != NULL)
	{//ファイルが開けた場合
		//ファイルからスコアをロードする
		fscanf(pFile, "%d\n", &nBestScore);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf(" *** ファイルが開けない…だと… *** \n");
	}

	return nBestScore;		//読み込んだベストスコアを返す
}

//============================================================================================================
//スコアの取得
//============================================================================================================
int GetScore(void)
{
	return s_nNowScore;
}