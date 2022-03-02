//**************************************************************************************************
//
// ターゲットの設定処理
// Auther：唐﨑結斗
// Author : 髙野馨將
//
//**************************************************************************************************
#include "main.h"
#include "input.h"
		 
#include "target.h"
#include "bullet.h"
#include "player.h"

//*******************************************************************************
// マクロ定義
//*******************************************************************************
#define Target_SPEED_SKY	(15.0f)		// 空中のターゲット速度
#define Target_SPEED_GROUND	(20.0f)		// 地上のターゲット速度
#define SIZE_TARGET			(30.0f)		// ターゲットの大きさ

//*******************************************************************************
// グローバル変数
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_TARGET] = {};	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;		// 頂点バッファへのポインタ
static Target s_aTarget[MAX_TARGET];					// ターゲット数
static char s_texName[MAX_TARGET][128];					// ターゲットテクスチャの名前情報を取得

//---------------------------------------------------------------------------
// ターゲット初期化
//---------------------------------------------------------------------------
void InitTarget(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ターゲットテクスチャの名前情報の初期化
	memset(&s_texName[0], 0, sizeof(s_texName));

	for (int i = 0; i < MAX_TARGET; i++)
	{// ターゲットに張り付けるテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			&s_texName[i][0],
			&s_pTexture[i]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TARGET,		// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,													// 頂点ファーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点情報へのポインタを生成						
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ターゲット情報の初期化
	memset(&s_aTarget[0], 0, sizeof(s_aTarget));

	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		// 対角線の長さを算出する
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		// 対角線の角度を算出
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		//頂点座標の設定
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// ターゲット終了
//---------------------------------------------------------------------------
void UninitTarget(void)
{
	for (int i = 0; i < MAX_TARGET; i++)
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
// ターゲット更新
//---------------------------------------------------------------------------
void UpdateTarget(void)
{
	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		if (!pTarget->bUse)
		{
			continue;
		}

		if (pTarget->bLockOnEnemy)
		{
			pTarget->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);		// カラー
		}
		else if (!pTarget->bLockOnEnemy)
		{
			switch (pTarget->TargetType)
			{
			case TARGETSTATE_LOCK_OFF:
				pTarget->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);		// カラー
				break;

			case TARGETSTATE_LOCK_ON:
				pTarget->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f);		// カラー
				break;
			}
		}

		//////////////////////////////////////
		//// スクロール
		//pTarget->pos += スクロール分値;
		//////////////////////////////////////

		if (pTarget->nIdxBullet != -1)
		{
			Bullet *pBullet = GetBullet();

			pBullet += pTarget->nIdxBullet;

			if (!pBullet->bUse)
			{
				pTarget->bUse = false;
			}
		}

		//対角線の長さを算出する
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		//対角線の角度を算出
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		//頂点情報へのポインタを生成						
		VERTEX_2D *pVtx;
		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		//頂点座標の設定
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		//頂点バッファをアンロック
		s_pVtxBuff->Unlock();
	}
}

//---------------------------------------------------------------------------
// ターゲット描画
//---------------------------------------------------------------------------
void DrawTarget(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		if (!pTarget->bUse)
		{
			continue;
		}

		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[(int)(pTarget->TargetType)]);

		//ターゲット描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

//---------------------------------------------------------------------------
// ターゲット設定
//---------------------------------------------------------------------------
int SetTarget(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 変数宣言
	int nCntTarget;

	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		Target* pTarget = &s_aTarget[nCntTarget];

		if (pTarget->bUse)
		{//使用されてない場合
			continue;
		}

		//タイトルロゴ情報の設定
		pTarget->pos = pos;										// 中心点
		pTarget->rot = rot;										// 向き
		pTarget->size = D3DXVECTOR3(SIZE_TARGET, SIZE_TARGET, 0.0f);		// 大きさ
		pTarget->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);		// カラー
		pTarget->TargetType = TARGETSTATE_LOCK_OFF;				// 状態
		pTarget->nIdxBullet = -1;								// 対応する弾の番号
		pTarget->bLockOnEnemy = false;							// 敵がいる
		pTarget->bUse = true;									// 使用してる

		//対角線の長さを算出する
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		//対角線の角度を算出
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		//頂点情報へのポインタを生成						
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntTarget * 4;

		//頂点座標の設定
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		//頂点バッファをアンロック
		s_pVtxBuff->Unlock();

		break;
	}

	return nCntTarget;
}

//---------------------------------------------------------------------------
// ターゲットの移動
//---------------------------------------------------------------------------
void SetPositionTarget(int nIdxShadow, D3DXVECTOR3 pos)
{
	Target* pTarget = &s_aTarget[nIdxShadow];

	if (pTarget->TargetType == TARGETSTATE_LOCK_OFF)
	{
		// ターゲット情報の設定
		pTarget->pos = pos;										// 中心点

		// 対角線の長さを算出する
		pTarget->fLength = sqrtf((pTarget->size.x * pTarget->size.x) + (pTarget->size.y * pTarget->size.y))* 0.5f;

		// 対角線の角度を算出
		pTarget->fAngele = atan2f(pTarget->size.x, pTarget->size.y);

		// 頂点情報へのポインタを生成						
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nIdxShadow * 4;

		// 頂点座標の設定
		SetTargetVtx(pVtx, pTarget->pos, pTarget->rot, pTarget->col, pTarget->fLength, pTarget->fAngele);

		// 頂点バッファをアンロック
		s_pVtxBuff->Unlock();
	}
}

//---------------------------------------------------------------------------
// ターゲットのロックオン
//---------------------------------------------------------------------------
void LockOnTarget(int nIdxShadow)
{
	Target* pTarget = &s_aTarget[nIdxShadow];
	Player *pPlayer = GetPlayer();

	if (pTarget->TargetType == TARGETSTATE_LOCK_OFF)
	{// ロックオフの時
		pTarget->TargetType = TARGETSTATE_LOCK_ON;
		pTarget->nIdxBullet = SetBullet(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - pPlayer->size.y, 0.0f), pPlayer->rot, pPlayer->BulletType, nIdxShadow, pTarget->bLockOnEnemy);
	}
}

//---------------------------------------------------------------------------
// ターゲットの当たり判定
//---------------------------------------------------------------------------
void CollisionTarget(D3DXVECTOR3 pos, float fLength, ENEMYTYPE TypeEnemy)
{
	for (int i = 0; i < MAX_TARGET; i++)
	{
		Target* pTarget = &s_aTarget[i];

		if (!pTarget->bUse)
		{
			continue;
		}

		pTarget->bLockOnEnemy = false;

		// 変数宣言
		float fTotalLength = pTarget->fLength + fLength;		// 半径の合計
		D3DXVECTOR3 DiffPos = pos - pTarget->pos;				// 位置の差

		// 位置の差
		float DiffLength = sqrtf((DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y));

		if (fTotalLength >= DiffLength
			&& pTarget->bLockOnEnemy == false
			&& TypeEnemy == ENEMYTYPE_GROUND_1
			&& TypeEnemy == ENEMYTYPE_GROUND_2)
		{
			pTarget->bLockOnEnemy = true;
		}
	}
}

//---------------------------------------------------------------------------
// ターゲット情報の取得
//---------------------------------------------------------------------------
Target *GetTarget(void)
{
	return &s_aTarget[0];
}

//---------------------------------------------------------------------------
// ターゲットの頂点関係の設定
//---------------------------------------------------------------------------
void SetTargetVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele)
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
