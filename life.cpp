//------------------------------------------------------------------------------
//
//ライフ
//Author;Takano Minto
//
//------------------------------------------------------------------------------
#include"life.h"

//---------------
//マクロ定義
//---------------
#define MAX_LIFE			(256)							//最大ライフ

//---------------------
//スタティック変数
//---------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				//テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//頂点バッファへのポインタ
static bool s_bUse[MAX_LIFE];

//-------------------------
//ライフ画面の初期化処理
//-------------------------
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスのポインタ

	//デヴァイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff, NULL);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/open_my_hart.png", &s_pTexture);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{//初期化
		s_bUse[nCnt] = false;
	}

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		//テキスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				//データが4個分進む
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//-------------------------
//ライフ画面の終了処理
//-------------------------
void UninitLife(void)
{
	//テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//-------------------------
//ライフ画面の更新処理
//-------------------------
void UpdateLife(void)
{

}

//-------------------------
//ライフ画面の描画処理
//-------------------------
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスのポインタ

	//デヴァイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_LIFE; i++)
	{
		if (s_bUse[i])
		{//使用されている時

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(D3DPT_TRIANGLESTRIP,
				i * 4,
				2);
		}
	}
}

//-----------------------------------
//ライフの配置
//-----------------------------------
void SetLife(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		if (!s_bUse[nCnt])
		{//使用されてない時
			s_bUse[nCnt] = true;		//使用する
			
			VERTEX_2D *pVtx;

			//頂点バッファをロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += nCnt * 4;						//データに合わせた数値分進む

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - LIFE_SIZE, pos.y - LIFE_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + LIFE_SIZE, pos.y - LIFE_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - LIFE_SIZE, pos.y + LIFE_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + LIFE_SIZE, pos.y + LIFE_SIZE, 0.0f);

			//頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
			break;
		}
	}
}

//-----------------------------------
//ライフのヒット処理
//-----------------------------------
void HitLife(int nRemainlife)
{

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	//頂点バッファをコック＆ロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nRemainlife * 4;						//データに合わせた数値分進む

	s_bUse[nRemainlife] = false;

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}