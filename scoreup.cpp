//==================================================
// 
// 3Dゲーム制作 ( scoreup.cpp )
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "scoreup.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_SCORE			(64)			// スコアの最大数
#define MAX_DIGITS			(8)				// 桁の最大数数
#define MAX_MOVE			(-1.0f)			// 移動量の最大値
#define INTERVAL_WIDTH		(30.0f)			// 数の間隔の幅
#define SCORE_WIDTH			(25.0f)			// スコアの幅
#define SCORE_HEIGHT		(35.0f)			// スコアの高さ
#define OPERATOR_WIDTH		(20.0f)			// 演算子の幅
#define OPERATOR_HEIGHT		(25.0f)			// 演算子の高さ
#define START_POS_X			(240.0f)		// Xの初期位置
#define START_POS_Y			(77.5f)			// Yの初期位置

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXCOLOR		col;			// 色
	int				nDigits;		// 桁数
	int				nAlphaTime;		// α値変更用の時間
	bool			bUse;			// 使用しているかどうか
}ScoreUp;

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------

/*↓ スコア関連 ↓*/

static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			// 頂点バッファへのポインタ
static ScoreUp						s_ScoreUp[MAX_SCORE];		// スコアの情報

/*↓ 演算子関連 ↓*/

static LPDIRECT3DTEXTURE9			s_pTexturePlus = NULL;			// プラスのテクスチャへのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMinus = NULL;			// マイナスのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffOperator = NULL;		// 演算子の頂点バッファへのポインタ
static bool							s_bOperator[MAX_SCORE];			// 演算子がどちらか [ true  : プラス  false  : マイナス ]

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitScoreUp(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Number_0To9.png",
		&s_pTexture);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/plus.png",
		&s_pTexturePlus);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/minus.png",
		&s_pTextureMinus);

	// メモリのクリア
	memset(s_ScoreUp, 0, sizeof(s_ScoreUp));

	// メモリのクリア
	memset(s_bOperator, 0, sizeof(s_bOperator));

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	/*↓ スコア関連 ↓*/

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_DIGITS * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		for (int j = 0; j < MAX_DIGITS; j++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
			
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			
			pVtx += 4;
		}
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/*↓ 演算子関連 ↓*/

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffOperator,
		NULL);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffOperator->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuffOperator->Unlock();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitScoreUp(void)
{
	/*↓ スコア関連 ↓*/

	if (s_pTexture != NULL)
	{// テクスチャの解放
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	/*↓ 演算子関連 ↓*/

	if (s_pTexturePlus != NULL)
	{// テクスチャの解放
		s_pTexturePlus->Release();
		s_pTexturePlus = NULL;
	}

	if (s_pTextureMinus != NULL)
	{// テクスチャの解放
		s_pTextureMinus->Release();
		s_pTextureMinus = NULL;
	}

	if (s_pVtxBuffOperator != NULL)
	{// 頂点バッファの解放
		s_pVtxBuffOperator->Release();
		s_pVtxBuffOperator = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateScoreUp(void)
{
	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (!pScoreUp->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		pScoreUp->pos.y += MAX_MOVE;
		pScoreUp->col.a = cosf((pScoreUp->nAlphaTime * 0.01f) * (D3DX_PI * 2.0f));
		pScoreUp->nAlphaTime++;

		if (pScoreUp->col.a <= 0.0f)
		{// 指定の値以下になった
			pScoreUp->bUse = false;
		}

		VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

		/*↓ スコア関連 ↓*/

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += MAX_DIGITS * i * 4;		// 指定の位置までポインタを進める

		for (int j = 0; j < pScoreUp->nDigits; j++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(pScoreUp->pos.x + (-INTERVAL_WIDTH * j) + (-INTERVAL_WIDTH * 0.5f), pScoreUp->pos.y, 0.0f);

			// 頂点座標の設定
			pVtx[0].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = pScoreUp->col;
			pVtx[1].col = pScoreUp->col;
			pVtx[2].col = pScoreUp->col;
			pVtx[3].col = pScoreUp->col;

			pVtx += 4;		// ポインタを進める
		}

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		/*↓ 演算子関連 ↓*/

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffOperator->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// 指定の位置までポインタを進める

		D3DXVECTOR3 pos = pScoreUp->pos;
		pos.x += (-INTERVAL_WIDTH * pScoreUp->nDigits) + (-INTERVAL_WIDTH * 0.5f);

		// 頂点座標の設定
		pVtx[0].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = pScoreUp->col;
		pVtx[1].col = pScoreUp->col;
		pVtx[2].col = pScoreUp->col;
		pVtx[3].col = pScoreUp->col;

		// 頂点バッファをアンロックする
		s_pVtxBuffOperator->Unlock();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawScoreUp(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*↓ スコア関連 ↓*/

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (!pScoreUp->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		for (int j = 0; j < pScoreUp->nDigits; j++)
		{
			int nIdx = (MAX_DIGITS * i * 4) + (j * 4);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				nIdx,						// 描画する最初の頂点インデックス
				2);							// プリミティブ(ポリゴン)数
		}
	}

	/*↓ 演算子関連 ↓*/

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffOperator, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (!pScoreUp->bUse)
		{// 使用していない
			continue;
		}

		/*↓ 使用している ↓*/

		// テクスチャの設定
		if (s_bOperator[i])
		{// プラス
			pDevice->SetTexture(0, s_pTexturePlus);
		}
		else
		{// マイナス
			pDevice->SetTexture(0, s_pTextureMinus);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			i * 4,						// 描画する最初の頂点インデックス
			2);							// プリミティブ(ポリゴン)数
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetScoreUp(int nScore)
{
	for (int i = 0; i < MAX_SCORE; i++)
	{
		ScoreUp *pScoreUp = &s_ScoreUp[i];

		if (pScoreUp->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pScoreUp->pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pScoreUp->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pScoreUp->nDigits = 0;
		pScoreUp->nAlphaTime = 0;
		pScoreUp->bUse = true;

		s_bOperator[i] = true;

		if (nScore < 0)
		{// マイナス
			nScore *= -1;
			s_bOperator[i] = false;
		}

		int nNumber = nScore;

		while (1)
		{// 無限ループ
			if (nNumber >= 10)
			{// 2桁以上
				nNumber /= 10;
				pScoreUp->nDigits++;
			}
			else
			{// 1桁
				pScoreUp->nDigits++;
				break;
			}
		}

		VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

		/*↓ スコア関連 ↓*/

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += MAX_DIGITS * i * 4;		// 指定の位置までポインタを進める

		int aNumber[MAX_DIGITS];

		for (int j = 0; j < pScoreUp->nDigits; j++)
		{
			aNumber[j] = nScore % 10;
			nScore /= 10;

			D3DXVECTOR3 pos = pScoreUp->pos;
			pos.x += (-INTERVAL_WIDTH * j) + (-INTERVAL_WIDTH * 0.5f);

			// 頂点座標の設定
			pVtx[0].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f, -SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(-SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3( SCORE_WIDTH * 0.5f,  SCORE_HEIGHT * 0.5f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = pScoreUp->col;
			pVtx[1].col = pScoreUp->col;
			pVtx[2].col = pScoreUp->col;
			pVtx[3].col = pScoreUp->col;

			float fLeft = 0.1f * aNumber[j];
			float fRight = (0.1f * aNumber[j]) + 0.1f;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(fLeft, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fRight, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(fLeft, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(fRight, 1.0f);

			pVtx += 4;		// ポインタを進める
		}

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		/*↓ 演算子関連 ↓*/

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffOperator->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// 指定の位置までポインタを進める

		D3DXVECTOR3 pos = pScoreUp->pos;
		pos.x += (-INTERVAL_WIDTH * pScoreUp->nDigits) + (-INTERVAL_WIDTH * 0.5f);

		// 頂点座標の設定
		pVtx[0].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f, -OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3( OPERATOR_WIDTH * 0.5f,  OPERATOR_HEIGHT * 0.5f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = pScoreUp->col;
		pVtx[1].col = pScoreUp->col;
		pVtx[2].col = pScoreUp->col;
		pVtx[3].col = pScoreUp->col;

		// 頂点バッファをアンロックする
		s_pVtxBuffOperator->Unlock();

		break;
	}
}