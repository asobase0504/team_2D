//================================
//
// タイトル画面
// Author:Teruto Sato
//
//================================
#include "title.h"
#include "input.h"
#include "map.h"
#include "fade.h"

//マクロ定義
#define NUM_TITLE	(3)					//テクスチャの最大数
#define BLINK_TIME	(200)				//点滅にかかる時間
#define BLINK_HALF	(BLINK_TIME / 2)	//点滅が切り替わる時間
#define MAP_SPEED	(0.0005f)			//画面スクロールの速さ

//スタティック変数
static LPDIRECT3DTEXTURE9			s_pTexture[NUM_TITLE] = {};		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//頂点バッファへのポインタ;
static TITLE s_Title[NUM_TITLE];	//タイトルの情報の取得
static int s_nTime = 0;			//点滅の時間

//============================
// タイトルの初期化処理
//============================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//------------------------------
	//	テクスチャの読み込み
	//------------------------------
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\map.png", &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\rogo.png", &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press.png", &s_pTexture[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//------------------------------
	//	構造体の初期化
	//------------------------------
	//位置の初期化
	s_Title[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	s_Title[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f);
	s_Title[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f);

	//幅の初期化
	s_Title[0].fWidth = SCREEN_WIDTH / 2;
	s_Title[1].fWidth = 300.0f;
	s_Title[2].fWidth = 450.0f;

	//高さの初期化
	s_Title[0].fHeight = SCREEN_HEIGHT / 2;
	s_Title[1].fHeight = 80.0f;
	s_Title[2].fHeight = 50.0f;

	//色の初期化
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		s_Title[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------
	//	頂点情報の設定
	//------------------------------
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		TITLE *title = s_Title + nCnt;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(title->pos.x - title->fWidth, title->pos.y - title->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(title->pos.x + title->fWidth, title->pos.y - title->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(title->pos.x - title->fWidth, title->pos.y + title->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(title->pos.x + title->fWidth, title->pos.y + title->fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = title->col;
		pVtx[1].col = title->col;
		pVtx[2].col = title->col;
		pVtx[3].col = title->col;

		//テクスチャ座標の設定
		if (nCnt == 0)
		{//背景のテクスチャなら
			pVtx[0].tex = D3DXVECTOR2(0.2f, 0.7f);
			pVtx[1].tex = D3DXVECTOR2(0.4f, 0.7f);
			pVtx[2].tex = D3DXVECTOR2(0.2f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);
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

	InitMap();		// マップ
	// マップの設定。
	InitMapSet(MAP_FILE0);
}

//============================
// タイトルの終了処理
//============================
void UninitTitle(void)
{
	UninitMap();	// マップ
					
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
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
// タイトルの更新処理
//============================
void UpdateTitle(void)
{
	UpdateMap();	// マップデータ

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	s_nTime++;				//タイムの加算
	s_nTime %= BLINK_TIME;	//タイムの初期化

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		TITLE *title = s_Title + nCnt;

		//------------------------
		//	テクスチャの点滅
		//------------------------
		if (s_nTime >= BLINK_HALF)
		{//タイムが40以上なら
			s_Title[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//表示する
		}
		else
		{//タイムが40未満なら
			s_Title[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	//表示しない
		}

		//------------------------
		//	背景のスクロール
		//------------------------
		if (nCnt == 0)
		{//背景のテクスチャなら
			//テクスチャ座標の設定
			pVtx[0].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
			pVtx[1].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
			pVtx[2].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
			pVtx[3].tex -= D3DXVECTOR2(0.0f, MAP_SPEED);
		}

		//頂点カラーの設定
		pVtx[0].col = title->col;
		pVtx[1].col = title->col;
		pVtx[2].col = title->col;
		pVtx[3].col = title->col;

		pVtx += 4;
	}

	//----------------
	//	画面遷移
	//----------------
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A,0))
	{

		SetFade(MODE_GAME);
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//============================
// タイトルの描画処理
//============================
void DrawTitle(void)
{
	DrawMap();		// マップデータ

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 1; nCnt < NUM_TITLE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[nCnt]);

		//タイトルの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							   nCnt * 4,				//描画する最初の頂点インデックス
							   2);						//描画するプリミティブ数
	}
}