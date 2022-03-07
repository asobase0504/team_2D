//**************************************************************************************************
//
// プレイヤーの設定処理
// Author：唐﨑結斗
//
//**************************************************************************************************
#include "main.h"
#include "input.h"
#include "life.h"
#include "common.h"
#include "player.h"
#include "collision.h"
#include "target.h"
#include "map.h"
#include "fade.h"

//*******************************************************************************
// 定数
//*******************************************************************************
#define MAX_SPEED				(10.0f)																		// 移動速度の最大値
#define MIN_SPEED				(0.0f)																		// 移動速度の最小値
#define MOVE_SPEED				(50.0f)																		// 設定時の移動量
#define MOVE_FRICTION			(0.5f)																		// 動摩擦係数
#define MAX_CNT_SKY_SHOT		(0.25f * 60)																// 空中弾発射間隔(秒数 * フレーム数)
#define MAX_CNT_GRAND_SHOT		(0.7f * 60)																	// 地上弾発射間隔(秒数 * フレーム数)
#define TARGET_DISTANCE			(300.0f)																	// ターゲットの間隔
#define PLAYER_RADIUS			(50.0f)																		// 半径
#define	START_LIFE				(5)																			// 最初の体力
#define RESPAWN_PLAYER			(60)																		// リスポーン待機時間
#define START_POS_Y				(SCREEN_HEIGHT * 0.8f)														// スタート時の位置(Y)
#define SPAWN_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT + PLAYER_RADIUS, 0.0f))		// スポーン時の位置


//******************************************************************************
// グローバル変数
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// 頂点バッファへのポインタ
static Player s_Player;										// プレイヤー数

//---------------------------------------------------------------------------
// プレイヤー初期化
//---------------------------------------------------------------------------
void InitPlayer(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// プレイヤーに張り付けるテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,					// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,													// 頂点ファーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// プレイヤー情報の初期化
	memset(&s_Player, 0, sizeof(s_Player));

	// 頂点情報へのポインタを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 対角線の長さを算出する
	s_Player.fLength = sqrtf(((s_Player.size.x * s_Player.size.x) + (s_Player.size.y * s_Player.size.y)) * 0.5f);

	// 対角線の角度を算出
	s_Player.fAngele = atan2f(s_Player.size.x, s_Player.size.y);

	// 頂点座標の設定
	SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

	// 頂点バッファをアンロック
	s_pVtxBuff->Unlock();

	// プレイヤーのセット
	SetPlayer(SPAWN_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//---------------------------------------------------------------------------
// プレイヤー終了
//---------------------------------------------------------------------------
void UninitPlayer(void)
{
	// テクスチャの破棄	  
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// 頂点バッファを破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//---------------------------------------------------------------------------
// プレイヤー更新
//---------------------------------------------------------------------------
void UpdatePlayer(void)
{
	if (s_Player.state == PLAYER_STATE_NORMAL)
	{// 通常状態の時
		// 移動ベクトルの更新
		s_Player.move = MovePlayer();

		// 位置の更新
		s_Player.pos += s_Player.move;

		// 弾の発射
		ShotPlayer();

		// プレイヤーの移動制限
		LockMovePlayer(&s_Player.pos, s_Player.size);
	}	

	if (s_Player.nIdxTarge != -1)
	{
		// ターゲットの移動
		SetPositionTarget(s_Player.nIdxTarge, D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f));
	}

	if (s_Player.state == PLAYER_STATE_NORMAL)
	{// 通常状態の時
		// 弾が当たった時
		Bullet* pBullet = GetBullet();
		for (int i = 0; i < MAX_BULLET; i++, pBullet++)
		{
			if (!pBullet->bUse
				|| pBullet->BulletType != BULLETTYPE_ENEMY)
			{// 弾が使用されていないか、敵の弾以外の時
				continue;
			}
			if (CollisionCircle(s_Player.pos, s_Player.size.x, pBullet->pos, pBullet->size.x))
			{// 弾に当たった//玉が同時に二発あたった時１だけ減るようにしてby浜田琉雅
				Target *pTarget = GetTarget();
				StartFadeOut();//ここれがフェード
				s_Player.nLife--;
				DeleteBullet();
				int Map = GetStage();
				ConteSet(Map);//MAP設定
				s_Player.state = PLAYER_STATE_DAMAGE;
				s_Player.nCntState = RESPAWN_PLAYER;
				s_Player.pos = SPAWN_POS;
				pTarget += s_Player.nIdxTarge;
				pTarget->bUse = false;
				pBullet->bUse = false;
				HitLife();
				if (s_Player.nLife <= 0)
				{	// リザルトに移行
					ChangeMode(MODE_RESULT);
				}
			}
		}
	}

	// プレイヤーの状態
	switch (s_Player.state)
	{
	case PLAYER_STATE_NORMAL:
		s_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case PLAYER_STATE_DAMAGE:
		s_Player.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_Player.nCntState--;

		if (s_Player.nCntState <= 0)
		{// カウントが0以下の時
			s_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			s_Player.state = PLAYER_STATE_START;
			s_Player.nCntState = 0;
		}
		break;

	case PLAYER_STATE_START:
		// 位置の更新
		if (s_Player.nLife == START_LIFE)
		{
			s_Player.pos.y += -1.0f;
		}
		else
		{
			s_Player.pos.y += -5.0f;
		}

		if (s_Player.pos.y <= START_POS_Y)
		{// スタート位置に至った時
			s_Player.pos.y = START_POS_Y;
			s_Player.state = PLAYER_STATE_NORMAL;

			// ターゲット
			s_Player.nIdxTarge = SetTarget(D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f), s_Player.rot);
		}

		break;
	}
	
	// 頂点情報へのポインタを生成						
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

	// 頂点バッファをアンロック
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// プレイヤー描画
//---------------------------------------------------------------------------
void DrawPlayer(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (s_Player.bUse)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		//プレイヤー描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//---------------------------------------------------------------------------
// プレイヤー設定
//---------------------------------------------------------------------------
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//頂点情報へのポインタを生成						
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!s_Player.bUse)
	{//使用されてない場合
		// プレイヤー情報の設定
		s_Player.pos = pos;													// 中心点
		s_Player.rot = rot;													// 向き
		s_Player.size = D3DXVECTOR3(PLAYER_RADIUS, PLAYER_RADIUS,0.0f);		// サイズ
		s_Player.col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);						// カラー
		s_Player.BulletType = (BulletType)(0);								// 弾の種類
		s_Player.state = PLAYER_STATE_START;								// 通常状態
		s_Player.nLife = START_LIFE;										// 体力
		s_Player.nCntShot = 1;												// 弾発射までのカウント
		s_Player.fSpeed = MOVE_SPEED;										// 速度
		s_Player.nIdxTarge = -1;											// ターゲット
		s_Player.nCntShotUse = 0;											// 弾の発射ができるまでのカウント
		s_Player.nCntState = 0;												// 状態変更までのカウント
		s_Player.bTriggerShot = false;										// トリガー弾発射の可不可
		s_Player.bPressShot = false;										// プレス弾発射の可不可
		s_Player.bUse = true;												// 使用してる

		//対角線の長さを算出する
		s_Player.fLength = sqrtf((s_Player.size.x * s_Player.size.x) + (s_Player.size.y * s_Player.size.y))* 0.5f;

		//対角線の角度を算出
		s_Player.fAngele = atan2f(s_Player.size.x, s_Player.size.y);

		//頂点座標の設定
		SetPlayerVtx(pVtx, s_Player.pos, s_Player.rot, s_Player.col, s_Player.fLength, s_Player.fAngele);

		for (int i = 0; i < s_Player.nLife; i++)
		{
			SetLife();
		}
	}

	//頂点バッファをアンロック
	s_pVtxBuff->Unlock();
}

//---------------------------------------------------------------------------
// プレイヤーの頂点関係の設定
//---------------------------------------------------------------------------
void SetPlayerVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele)
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
	SetVtxColor(pVtx,&col);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//---------------------------------------------------------------------------
// プレイヤー移動
//---------------------------------------------------------------------------
D3DXVECTOR3 MovePlayer(void)
{
	// 変数宣言
	D3DXVECTOR3 move = s_Player.move;

	// 変数宣言
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動方向

	if (GetKeyboardPress(DIK_W))
	{// [W]キーが押された時
		moveDir.y += -1.0f;
	}
	if (GetKeyboardPress(DIK_A))
	{// [A]キーが押された時
		moveDir.x += -1.0f;
	}
	if (GetKeyboardPress(DIK_D))
	{// [D]キーが押された時
		moveDir.x += 1.0f;
	}
	if (GetKeyboardPress(DIK_S))
	{// [S]キーが押された時
		moveDir.y += 1.0f;
	}

	if (moveDir.x != 0.0f || moveDir.y != 0.0f)
	{
		// 移動方向ベクトルの大きさを1.0fにする
		D3DXVec3Normalize(&moveDir, &moveDir);

		// 速度の算出
		move += moveDir * s_Player.fSpeed;

		// 速度リミッター
		float speed = D3DXVec3Length(&move);
		if (speed > MAX_SPEED)
		{
			move = move / speed * MAX_SPEED;
		}
	}


	// 動摩擦係数を加える
	move *= (1.0f - MOVE_FRICTION);

	// 速度リミッター
	if (D3DXVec3Length(&move) < MIN_SPEED)
	{
		move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return move;
}

//---------------------------------------------------------------------------
// 弾の発射
//---------------------------------------------------------------------------
void ShotPlayer()
{
	// 変数宣言
	int nCntMaxShot;

	if (GetKeyboardTrigger(DIK_RIGHT))
	{// 右ボタンが押された
		s_Player.BulletType = (BulletType)((int)(s_Player.BulletType) + 1);

		if (s_Player.BulletType >= MAX_BULLETTYPE - 1)
		{// タイプ数を超えたとき
			s_Player.BulletType = (BulletType)(0);
		}
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{// 左ボタンが押された
		s_Player.BulletType = (BulletType)((int)(s_Player.BulletType) - 1);

		if (s_Player.BulletType <= -1)
		{// タイプ数を超えたとき
			s_Player.BulletType = (BulletType)((int)(MAX_BULLETTYPE)-2);
		}
	}

	switch (s_Player.BulletType)
	{
	case BULLETTYPE_PLAYER_SKY:
		nCntMaxShot = (int)(MAX_CNT_SKY_SHOT);
		break;

	case BULLETTYPE_PLAYER_GROUND:
		nCntMaxShot = (int)(MAX_CNT_GRAND_SHOT);
		break;
	}

	if (GetKeyboardTrigger(DIK_SPACE)
		&& !s_Player.bTriggerShot
		&& !s_Player.bPressShot)
	{
		// トリガー弾とプレス弾を使用している
		s_Player.bTriggerShot = true;
		s_Player.bPressShot = true;

		// カウントを最大値にする
		s_Player.nCntShot = nCntMaxShot;
	}

	if (GetKeyboardPress(DIK_SPACE))
	{// SPACEが押された
		s_Player.nCntShot++;
	}
	if (GetKeyboardRelease(DIK_SPACE)
		&& s_Player.bPressShot
		&& s_Player.bTriggerShot)
	{// キーが離されたら
		s_Player.nCntShotUse = s_Player.nCntShot;
		s_Player.nCntShot = 0;
		s_Player.bPressShot = false;
	}

	if (s_Player.bTriggerShot
		&& !s_Player.bPressShot)
	{
		// カウントインクリメント
		s_Player.nCntShotUse++;

		if (s_Player.nCntShotUse >= nCntMaxShot)
		{
			// カウントを初期化
			s_Player.nCntShotUse = 0;
			s_Player.bTriggerShot = false;
		}
	}

	if (s_Player.nCntShot >= nCntMaxShot)
	{// 弾の発射
		switch (s_Player.BulletType)
		{
		case BULLETTYPE_PLAYER_SKY:
			SetBullet(D3DXVECTOR3(s_Player.pos.x - (s_Player.size.x / 2.0f), s_Player.pos.y - s_Player.size.y, 0.0f), s_Player.rot, s_Player.BulletType, -1, true);
			SetBullet(D3DXVECTOR3(s_Player.pos.x + (s_Player.size.x / 2.0f), s_Player.pos.y - s_Player.size.y, 0.0f), s_Player.rot, s_Player.BulletType, -1, true);
			break;

		case BULLETTYPE_PLAYER_GROUND:
			LockOnTarget(s_Player.nIdxTarge);
			// ターゲット
			s_Player.nIdxTarge = SetTarget(D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y - TARGET_DISTANCE, 0.0f), s_Player.rot);
			break;
		}
		
		s_Player.nCntShot = 0;
	}
}

//---------------------------------------------------------------------------
// プレイヤーの移動制限
//---------------------------------------------------------------------------
void LockMovePlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 size)
{
	if (0.0f + (size.x / 2.0f) >= pos->x)
	{
		pos->x = 0.0f + (size.x / 2.0f);
	}
	if (0.0f + (size.y / 2.0f) >= pos->y)
	{
		pos->y = 0.0f + (size.y / 2.0f);
	}
	if (SCREEN_WIDTH - (size.x / 2.0f) <= pos->x)
	{
		pos->x = SCREEN_WIDTH - (size.x / 2.0f);
	}
	if (SCREEN_HEIGHT - (size.y / 2.0f) <= pos->y)
	{
		pos->y = SCREEN_HEIGHT - (size.y / 2.0f);
	}
}

Player *GetPlayer(void)
{
	return &s_Player;
}