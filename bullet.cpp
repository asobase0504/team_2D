//**************************************************************************************************
//
// 弾の設定処理
// Author：唐﨑結斗
// Author : 髙野馨將
//
//**************************************************************************************************
#include"main.h"
#include"input.h"
#include "bullet.h"
#include "target.h"
#include "sound.h"

//*******************************************************************************
// マクロ定義
//*******************************************************************************
#define BULLET_SPEED_SKY	(15.0f)		// 空中の弾速度
#define BULLET_SPEED_GROUND	(10.0f)		// 地上の弾速度
#define BULLET_SPEED_ENEMY	(10.0f)		// エネミーの弾速度
#define SIZE_BULLET			(15.0f)		// 弾の大きさ

//*******************************************************************************
// グローバル変数
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_BULLET] = {};	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static Bullet s_aBullet[MAX_BULLET];					// 弾数
static char s_texName[MAX_BULLET][128];					// 弾テクスチャの名前情報を取得
static int TestCount;									// テクスチャの色が切り替わるまでのカウント

//---------------------------------------------------------------------------
// 弾初期化
//---------------------------------------------------------------------------
void InitBullet(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 弾テクスチャの名前情報の初期化
	memset(&s_texName[0], 0, sizeof(s_texName));

	TestCount = 0;
	
	// 弾に張り付けるテクスチャの読み込み
	//空の弾
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/bullet_sky.png",
		&s_pTexture[0]);
	//陸への弾
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/bullet_ground.png",
		&s_pTexture[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,		// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,													// 頂点ファーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点情報へのポインタを生成						
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 弾情報の初期化
	memset(&s_aBullet[0], 0, sizeof(s_aBullet));

	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];
		
		// 対角線の長さを算出する
		pBullet->fLength = sqrtf((pBullet->size.x * pBullet->size.x) + (pBullet->size.y * pBullet->size.y))* 0.5f;

		// 対角線の角度を算出
		pBullet->fAngele = atan2f(pBullet->size.x, pBullet->size.y);

		//頂点座標の設定
		SetBulletVtx(pVtx, pBullet->pos, pBullet->rot, pBullet->col, pBullet->fLength, pBullet->fAngele);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// 弾終了
//---------------------------------------------------------------------------
void UninitBullet(void)
{
	for (int i = 0; i < MAX_BULLET_TYPE; i++)
	{// テクスチャの破棄	  
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();

			s_pTexture[i] = NULL;
		}
	}

	// 頂点バッファを破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();

		s_pVtxBuff = NULL;
	}
}

//---------------------------------------------------------------------------
// 弾更新
//---------------------------------------------------------------------------
void UpdateBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];

		if (!pBullet->bUse)
		{
			continue;
		}

		// 寿命を減少
		pBullet->nLife--;

		if (pBullet->nLife <= 0)
		{// 寿命が0以下の時
			pBullet->bUse = false;
		}

		// 移動
		pBullet->pos += pBullet->move;

		if (pBullet->BulletType == BULLETTYPE_PLAYER_GROUND)
		{// 地上弾の時
		 // ターゲット情報の取得
			Target *pTarget = GetTarget();

			pTarget += pBullet->nIdxTarget;

			if (pBullet->pos.y - pBullet->size.y < pTarget->pos.y - pTarget->size.y)
			{
				pBullet->bUse = false;
			}
		}

		//対角線の長さを算出する
		pBullet->fLength = sqrtf((pBullet->size.x * pBullet->size.x) + (pBullet->size.y * pBullet->size.y))* 0.5f;

		//対角線の角度を算出
		pBullet->fAngele = atan2f(pBullet->size.x, pBullet->size.y);

		//頂点情報へのポインタを生成						
		VERTEX_2D *pVtx;
		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		TestCount++;

		if (TestCount >= 30)
		{
			if (TestCount >= 60)
			{
				TestCount = 0;
			}
			pBullet->col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);		// カラー
		}
		else
		{
			pBullet->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー
		}

		//頂点座標の設定
		SetBulletVtx(pVtx, pBullet->pos, pBullet->rot, pBullet->col, pBullet->fLength, pBullet->fAngele);

		//頂点バッファをアンロック
		s_pVtxBuff->Unlock();

		if (pBullet->pos.y <= -SIZE_BULLET)
		{//弾が画面外に出た
			pBullet->bUse = false;	//使用していない状態にする
		}
	}
}

//---------------------------------------------------------------------------
// 弾描画
//---------------------------------------------------------------------------
void DrawBullet(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];
		
		if (!pBullet->bUse)
		{
			continue;
		}

		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[(int)(pBullet->BulletType)]);

		//弾描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

//---------------------------------------------------------------------------
// 弾設定
//---------------------------------------------------------------------------
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdxTarget, bool bOnCollision)
{
	// 変数宣言
	int nCntBullet = 0;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		Bullet* pBullet = &s_aBullet[nCntBullet];

		if (pBullet->bUse)
		{//使用されてない場合
			continue;
		}

		//タイトルロゴ情報の設定
		pBullet->pos = pos;									// 中心点
		pBullet->rot = rot;									// 向き
		pBullet->BulletType = (BulletType)(nType);			// タイプ
		pBullet->nIdxTarget = nIdxTarget;					// ターゲットの番号
		pBullet->bUse = true;								// 使用してる

		pBullet->fRotMove = pBullet->rot.z - D3DX_PI;		// 移動方向

		if (pBullet->fRotMove >= D3DX_PI)
		{// 移動方向の正規化
			pBullet->fRotMove -= D3DX_PI * 2;
		}
		else if (pBullet->fRotMove <= -D3DX_PI)
		{// 移動方向の正規化
			pBullet->fRotMove += D3DX_PI * 2;
		}

		switch (pBullet->BulletType)
		{
		case BULLETTYPE_PLAYER_SKY:
			pBullet->size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);		// 大きさ
			pBullet->col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);		// カラー
			pBullet->fSpeed = BULLET_SPEED_SKY;						// 速度
			pBullet->bOnCollision = true;							// 当たり判定の有無
			pBullet->nLife = 180;									// 寿命
			PlaySound(SOUND_LABEL_SE_SKYBULLET);
			break;

		case BULLETTYPE_PLAYER_GROUND:
			pBullet->size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);		// 大きさ
			pBullet->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		// カラー
			pBullet->fSpeed = BULLET_SPEED_GROUND;					// 速度
			pBullet->bOnCollision = bOnCollision;					// 当たり判定の有無
			pBullet->nLife = 180;									// 寿命
			PlaySound(SOUND_LABEL_SE_GLOUNDBULLET);
			break;

		case BULLETTYPE_ENEMY:
			pBullet->size = D3DXVECTOR3(SIZE_BULLET, SIZE_BULLET, 0.0f);		// 大きさ
			pBullet->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// カラー
			pBullet->fSpeed = BULLET_SPEED_ENEMY;					// 速度
			pBullet->bOnCollision = true;							// 当たり判定の有無
			pBullet->nLife = 180;									// 寿命
			break;
		}

		// 移動ベクトルの算出
		pBullet->move.x = sinf(pBullet->fRotMove);
		pBullet->move.y = cosf(pBullet->fRotMove);
		pBullet->move *= pBullet->fSpeed;

		//対角線の長さを算出する
		pBullet->fLength = sqrtf((pBullet->size.x * pBullet->size.x) + (pBullet->size.y * pBullet->size.y))* 0.5f;

		//対角線の角度を算出
		pBullet->fAngele = atan2f(pBullet->size.x, pBullet->size.y);

		//頂点情報へのポインタを生成						
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBullet * 4;

		//頂点座標の設定
		SetBulletVtx(pVtx, pBullet->pos, pBullet->rot, pBullet->col, pBullet->fLength, pBullet->fAngele);

		//頂点バッファをアンロック
		s_pVtxBuff->Unlock();

		return nCntBullet;
		break;
	}	

	return nCntBullet;
}

//---------------------------------------------------------------------------
// 弾情報の取得
//---------------------------------------------------------------------------
Bullet *GetBullet(void)
{
	return &s_aBullet[0];
}

//---------------------------------------------------------------------------
// 弾の頂点関係の設定
//---------------------------------------------------------------------------
void SetBulletVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele)
{
	//頂点座標の設定
	pVtx[0].pos.x = pos.x + sinf(rot.z + (D3DX_PI + fAngele)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z + (D3DX_PI + fAngele)) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngele)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngele)) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - (0 + fAngele)) * fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - (0 + fAngele)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z - (0 - fAngele)) * fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z - (0 - fAngele)) * fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//---------------------------------------------------------------------------
// 弾の削除設定
//---------------------------------------------------------------------------
void DeleteBullet(void)
{

	for (int i = 0; i < MAX_BULLET; i++)
	{
		Bullet* pBullet = &s_aBullet[i];

		if (!pBullet->bUse)
		{
			continue;
		}
		pBullet->bUse = false;
	}
}