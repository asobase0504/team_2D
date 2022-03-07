//==================================================
// 
// 3Dゲーム制作 ( fade.cpp )
// Author  : katsuki mizuki
// 
//==================================================
#include "fade.h"
#include "input.h"
#include "common.h"
#include "player.h"
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define FADE_CHANGE		(0.05f)		// フェードの変化量

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		// 頂点バッファのポインタ
static FADE							s_fade;					// 今のフェード
static MODE							s_modeNext;				// 次のモード
static D3DXCOLOR					s_colorFade;			// ポリゴン(フェード)の色
static int							s_nCntOut;				// アウトのカウンター

															//--------------------------------------------------
															// 初期化
															//--------------------------------------------------
void InitFade(MODE modeNext)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 黒いポリゴン(不透明)にしておく
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	s_fade = FADE_NONE;			// フェードイン状態に

	s_modeNext = modeNext;		// 次の画面(モード)を設定

	s_nCntOut = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

							// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;

	// 頂点座標の設定処理
	Setpos(pVtx, D3DXVECTOR3(fWidth, fHeight, 0.0f), fWidth, fHeight, SETPOS_MIDDLE);

	// rhwの初期化処理
	Initrhw(pVtx);

	// 頂点カラーの設定処理
	Setcol(pVtx, s_colorFade);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// モードの設定
	SetMode(s_modeNext);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitFade(void)
{
	if (s_pVtxBuff != NULL)
	{// 頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateFade(void)
{
	if (s_fade != FADE_NONE)
	{// フェード中
		switch (s_fade)
		{
		case FADE_OUT:		// フェードアウト状態

			s_colorFade.a += FADE_CHANGE;		// ポリゴンを不透明にしていく

			if (s_colorFade.a >= 1.0f)
			{// 不透明になった
				s_colorFade.a = 1.0f;
				s_fade = FADE_IN;		// フェードイン状態に
										// プレイヤー情報の取得
				Player* pPlayer = GetPlayer();
				if (pPlayer->damage)
				{//ダメージを受けたときはモード移行しない
					pPlayer->damage = false;				// モードの設定
					
				}
				else
				{
					SetMode(s_modeNext);
				}
			}

			break;

		case FADE_SKIP:		// フェードスキップ

			s_colorFade.a -= FADE_CHANGE;		// ポリゴンを透明にしていく

												// breakなし

		case FADE_IN:		// フェードイン状態

			s_colorFade.a -= FADE_CHANGE;		// ポリゴンを透明にしていく

			if (s_colorFade.a <= 0.0f)
			{// 透明になった
				s_colorFade.a = 0.0f;
				s_fade = FADE_NONE;		// 何もしていない状態に
			}

			break;

		default:
			assert(false);
			break;
		}

		switch (s_fade)
		{
		case FADE_OUT:		// フェードアウト状態

			if (s_nCntOut < 1)
			{// １より下ならbreak
				s_nCntOut++;
				break;
			}

			// １以上ならbreakなし

		case FADE_IN:		// フェードイン状態

			if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
				GetJoypadTrigger(JOYKEY_B, 0) || GetJoypadTrigger(JOYKEY_A, 0) ||
				GetJoypadTrigger(JOYKEY_START, 0))
			{// 決定キー(ENTERキー)が押されたかどうか

				if (s_fade == FADE_OUT)
				{
					// 黒いポリゴン(不透明)にしておく
					s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					// モードの設定
					SetMode(s_modeNext);
				}

				s_fade = FADE_SKIP;		// フェードスキップ
			}

			break;

		case FADE_NONE:		// 何もしていない状態
		case FADE_SKIP:		// フェードスキップ

			break;

		default:
			assert(false);
			break;
		}

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

								// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定処理
		Setcol(pVtx, s_colorFade);

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawFade(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		2);							// プリミティブ(ポリゴン)数
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetFade(MODE modeNext)
{
	if (s_fade == FADE_NONE)
	{// 何もしていない状態なら
		s_fade = FADE_OUT;			// フェードアウト状態に

		s_modeNext = modeNext;		// 次の画面(モード)を設定



			// 黒いポリゴン(透明)にしておく
		s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		s_nCntOut = 0;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
FADE GetFade(void)
{
	return s_fade;
}