//============================================================================================================
//
//チーム制作[ranking.cpp]
//Author:Kishimoto Eiji
//
//============================================================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "time.h"
//#include "sound.h"

#include <stdio.h>
#include <assert.h>

//************************************************************************************
//マクロ定義
//************************************************************************************
#define MAX_RANK			(5)						//ランク数
#define RANK_POS_X			(150.0f)				//順位の初期位置(X)
#define RANK_POS_Y			(470.0f)				//順位の初期位置(Y)
#define RANK_WIDTH			(75)					//順位の幅
#define RANK_HEIGHT			(210)					//順位の高さ
#define RANKINGSCORE_POS_X	(RANK_POS_X + 150.0f)	//ランキングスコアの初期位置(X)
#define RANKINGSCORE_POS_Y	(RANK_POS_Y - 190.0f)	//ランキングスコアの初期位置(Y)
#define RANKINGSCORE_WIDTH	(30)					//ランキングスコアの幅
#define RANKINGSCORE_HEIGHT	(45)					//ランキングスコアの高さ
#define RANKINGSCORE_SPACE	(40)					//ランキングスコアの間隔
#define RANKING_TIME		(10)					//ランキングの表示時間
#define RANKING_FADE_SPEED	(0.02f)					//ランキングの点滅スピード
#define RANKING_LOGO_POS_X	(315.0f)				//ロゴの位置(X)
#define RANKING_LOGO_POS_Y	(190.0f)				//ロゴの位置(Y)
#define RANKING_LOGO_WIDTH	(200.0f)				//ロゴの幅
#define RANKING_LOGO_HEIGHT	(50.0f)					//ロゴの高さ
#define RANKING_FILE		("data/txt/RankingScore.txt")

//************************************************************************************
//スタティック変数
//************************************************************************************
static LPDIRECT3DTEXTURE9		s_pTextureRank				= NULL;		//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTextureRankScore			= NULL;		//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTextureRankingBG			= NULL;		//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		s_pTextureRankingLogo		= NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRank				= NULL;		//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRankScore			= NULL;		//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRankingBG			= NULL;		//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRankingLogo		= NULL;		//頂点バッファへのポインタ
static RankScore				s_aRankScore[MAX_RANK + 1];				//ランキングスコア情報
static int						s_nRankUpdate;							//更新ランクNo.
static D3DXVECTOR2				s_posRank;								//順位の位置
static D3DXVECTOR2				s_PosLogo;								//ロゴの位置
static int						s_nCntAlpha;							//点滅のsinカーブ用

//============================================================================================================
//ランキングの初期化処理
//============================================================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/Rank_1to5.png",&s_pTextureRank);
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/Number_0To9.png",&s_pTextureRankScore);
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/ACG_Ranking.png",&s_pTextureRankingBG);
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/RANKING.png",&s_pTextureRankingLogo);

	//ランキングの情報の初期化
	for (int i = 0; i < (MAX_RANK + 1); i++)
	{
		s_aRankScore[i].pos.x = RANKINGSCORE_POS_X;
		s_aRankScore[i].pos.y = RANKINGSCORE_POS_Y + (i * RANKINGSCORE_SPACE);
		s_aRankScore[i].pos.z = 0.0f;
		s_aRankScore[i].col	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//変数の初期化
	s_nRankUpdate = -1;													//更新ランクNo.
	s_posRank = D3DXVECTOR2(RANK_POS_X, RANK_POS_Y);					//順位の位置
	s_PosLogo = D3DXVECTOR2(RANKING_LOGO_POS_X, RANKING_LOGO_POS_Y);	//ロゴの位置
	s_nCntAlpha = 0;													//sinカーブ用カウント

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRank,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE * MAX_RANK,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRankScore,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRankingBG,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffRankingLogo,
								NULL);

	/**************** 順位 ****************/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	float fLeft		= (s_posRank.x - RANK_WIDTH);	//左
	float fRight	= (s_posRank.x + RANK_WIDTH);	//右
	float fUp		= (s_posRank.y - RANK_HEIGHT);	//上
	float fDown		= (s_posRank.y + RANK_HEIGHT);	//下

	pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);
	
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//頂点バッファをアンロックする
	s_pVtxBuffRank->Unlock();

	/**************** ランキングスコア ****************/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_RANK; i++)
	{
		for (int j = 0; j < MAX_SCORE; j++)
		{
			//頂点座標の設定
			float fLeft	  = (s_aRankScore[i].pos.x + (j * RANKINGSCORE_WIDTH));							//左
			float fRight  = (s_aRankScore[i].pos.x + (j * RANKINGSCORE_WIDTH) + RANKINGSCORE_WIDTH);	//右
			float fUp	  = (s_aRankScore[i].pos.y + (i * RANKINGSCORE_HEIGHT));						//上
			float fDown	  = (s_aRankScore[i].pos.y + (i * RANKINGSCORE_HEIGHT) + RANKINGSCORE_HEIGHT);	//下

			pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
			pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);
			
			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
			
			pVtx += 4;		//頂点データのポインタを４つ分進める
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuffRankScore->Unlock();

	/**************** 背景画像 ****************/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankingBG->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

	//頂点バッファをアンロックする
	s_pVtxBuffRankingBG->Unlock();

	/**************** ランキングロゴ ****************/

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankingLogo->Lock(0, 0, (void**)&pVtx, 0);

	{//頂点座標の設定
		float fLeft		= (s_PosLogo.x - RANKING_LOGO_WIDTH);
		float fRight	= (s_PosLogo.x + RANKING_LOGO_WIDTH);
		float fUp		= (s_PosLogo.y - RANKING_LOGO_HEIGHT);
		float fDown		= (s_PosLogo.y + RANKING_LOGO_HEIGHT);

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fUp,	 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fUp,	 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fDown, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fDown, 0.0f);
	}

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

	//頂点バッファをアンロックする
	s_pVtxBuffRankingLogo->Unlock();

	////サウンドの再生
	//PlaySound(SOUND_LABEL_RANKING);

	////タイムの初期化処理
	//InitTime();

	////タイムの設定処理
	//SetTime(RANKING_TIME);
}

//============================================================================================================
//ランキングの終了処理
//============================================================================================================
void UninitRanking(void)
{
	////サウンドの停止
	//StopSound();

	/**************** テクスチャの破棄 ****************/

	if (s_pTextureRank != NULL)
	{//順位
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	if (s_pTextureRankScore != NULL)
	{//スコア表示
		s_pTextureRankScore->Release();
		s_pTextureRankScore = NULL;
	}

	if (s_pTextureRankingBG != NULL)
	{//背景画像
		s_pTextureRankingBG->Release();
		s_pTextureRankingBG = NULL;
	}

	if (s_pTextureRankingLogo != NULL)
	{//ロゴ
		s_pTextureRankingLogo->Release();
		s_pTextureRankingLogo = NULL;
	}

	/**************** 頂点バッファの破棄 ****************/

	if (s_pVtxBuffRank != NULL)
	{//順位
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	if (s_pVtxBuffRankScore != NULL)
	{//スコア表示
		s_pVtxBuffRankScore->Release();
		s_pVtxBuffRankScore = NULL;
	}

	if (s_pVtxBuffRankingBG != NULL)
	{//背景画像
		s_pVtxBuffRankingBG->Release();
		s_pVtxBuffRankingBG = NULL;
	}

	if (s_pVtxBuffRankingLogo != NULL)
	{//ロゴ
		s_pVtxBuffRankingLogo->Release();
		s_pVtxBuffRankingLogo = NULL;
	}

	////タイムの終了処理
	//UninitTime();
}

//============================================================================================================
//ランキングの更新処理
//============================================================================================================
void UpdateRanking(void)
{
	/**************** ランキングスコアの点滅 ****************/

	s_nCntAlpha++;		//カウントアップ
	
	if (s_nRankUpdate != -1)
	{//更新ランクNo.が0以上の時
		//スコアの点滅
		float fRot = s_nCntAlpha * 0.01f * (D3DX_PI * 2.0f);
		float fAlpha = (sinf(fRot) * 0.5f) + 0.5f;

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (s_nRankUpdate * 4) * MAX_SCORE;

		//頂点カラーの設定
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, fAlpha);

			pVtx += 4;		//ポインタを4つ分進める
		}

		//頂点バッファをアンロックする
		s_pVtxBuffRankScore->Unlock();
	}

	if (GetFade() == FADE_NONE)
	{//フェードしていない時
		if (GetKeyboardTrigger(DIK_RETURN))
		{//Enterキーが押された
			//タイトル画面に遷移
			SetFade(MODE_TITLE);
		}
	}

	////タイムの更新処理
	//UpdateTime();
}

//============================================================================================================
//ランキングの描画処理
//============================================================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	/**************** 背景画像 ****************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffRankingBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureRankingBG);
		
	//ランキングの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数

	/**************** 順位 ****************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureRank);
		
	//ランキングの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数

	/**************** ランキングスコア ****************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureRankScore);
		
	for (int i = 0; i < MAX_SCORE * MAX_RANK; i++)
	{
		//ランキングの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								i * 4,					//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}

	/**************** ランキングロゴ ****************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffRankingLogo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureRankingLogo);
		
	//ランキングの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							0,						//描画する最初の頂点インデックス
							2);						//描画するプリミティブ数
}

//============================================================================================================
//ランキングのリセット
//============================================================================================================
void ResetRanking(void)
{
	//ランキングスコアのロード
	LoadRanking();
}

//============================================================================================================
//ランキングの設定処理
//============================================================================================================
void SetRanking(int nScore)
{		
	//スコアを入れ替える
	s_aRankScore[5].nScore = nScore;

	//選択ソートによる並び変え
	for (int i = 0; i < MAX_RANK; i++)
	{
		for (int nCnt = i + 1; nCnt < (MAX_RANK + 1); nCnt++)
		{
			int nChange = 0;					//初期化
			nChange = s_aRankScore[i].nScore;	//スコアを入れる

			if (s_aRankScore[i].nScore < s_aRankScore[nCnt].nScore)
			{//大きい順に並び変える
				s_aRankScore[i].nScore = s_aRankScore[nCnt].nScore;
				s_aRankScore[nCnt].nScore = nChange;
			}
		}
	}

	//ランキングスコアのセーブ
	SaveRanking();

	//更新ランクNo.を更新
	for (int i = 0; i < MAX_RANK; i++)
	{
		if (s_aRankScore[i].nScore == nScore)
		{//新しいスコアと一致した場合
			s_nRankUpdate = i;		//カウント数を代入する
			break;
		}
	}

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	int aPosTexU[MAX_SCORE];	//各桁の数字を格納

	//各桁にスコアの数字を入れる
	for (int i = 0; i < MAX_RANK; i++)
	{
		aPosTexU[0] = s_aRankScore[i].nScore % 100000000 / 10000000;
		aPosTexU[1] = s_aRankScore[i].nScore % 10000000 / 1000000;
		aPosTexU[2] = s_aRankScore[i].nScore % 1000000 / 100000;
		aPosTexU[3] = s_aRankScore[i].nScore % 100000 / 10000;
		aPosTexU[4] = s_aRankScore[i].nScore % 10000 / 1000;
		aPosTexU[5] = s_aRankScore[i].nScore % 1000 / 100;
		aPosTexU[6] = s_aRankScore[i].nScore % 100 / 10;
		aPosTexU[7] = s_aRankScore[i].nScore % 10 / 1;

		//テクスチャ座標の設定
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (aPosTexU[nCnt] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (aPosTexU[nCnt] * 0.1f), 1.0f);

			pVtx += 4;		//頂点データのポインタを４つ分進める
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuffRankScore->Unlock();
}

//============================================================================================================
//ランキングのセーブ処理
//============================================================================================================
void SaveRanking(void)
{
	FILE *pFile = fopen(RANKING_FILE, "w");	//ファイルを開く

	if (pFile != NULL)
	{//ファイルが開けた場合
		for (int i = 0; i < MAX_RANK; i++)
		{
			//ファイルにスコアをセーブする
			fprintf(pFile, "%d\n", s_aRankScore[i].nScore);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		assert(false);
	}
}

//============================================================================================================
//ランキングのロード処理
//============================================================================================================
void LoadRanking(void)
{
	FILE *pFile = fopen(RANKING_FILE, "r");	//ファイルを開く

	if (pFile != NULL)
	{//ファイルが開けた場合
		for (int i = 0; i < MAX_RANK; i++)
		{
			//ファイルからスコアをロードする
			fscanf(pFile, "%d", &s_aRankScore[i].nScore);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		assert(false);
	}
}