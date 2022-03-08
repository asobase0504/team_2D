//=========================================
// 
// ゲーム内処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "game.h"
#include "main.h"
#include "bullet.h"
#include "bestscore.h"
#include "scoreup.h"
#include "player.h"
#include "score.h"
#include "Boss.h"
#include "life.h"
#include "enemy.h"
#include "input.h"
#include "map.h"
#include "menu.h"
#include "pause.h"
#include "target.h"
#include "scoreup.h"
#include "crater.h"
#include "sound.h"
#include "fade.h"
#include <stdio.h>
#include "particle.h"

//------------------------------------
// マクロ定義
//------------------------------------

//------------------------------------
// スタティック変数
//------------------------------------
static bool s_bPause;	// ポーズ中かどうか
static int s_nMap = 0;
//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	s_nMap = 0;
	s_bPause = false;
	InitMap();			// マップ
	InitBoss();			// ボス
	InitEnemy();		// 敵
	InitBullet();		// 弾
	InitTarget();		// ターゲット
	InitLife();			// ライフ
	InitPlayer();		// プレイヤー
	InitScore();		// スコア
	InitBestScore();	// ベストスコア
	InitScoreUp();		// スコアアップ
//	InitFlag();			// フラグ
	InitCrater();		// クレーター
	InitMenu();			// メニュー
	InitPause();		// ポーズ
	InitParticle();
	// マップの設定。
	ConteSet(s_nMap);

	PlaySound(SOUND_LABEL_BGM_GAME1);
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{

	UninitMap();		// マップ
	UninitBoss();		// ボス
	UninitEnemy();		// 敵	
	UninitBullet();		// 弾
	UninitTarget();		// ターゲット
	UninitLife();		// ライフ
	UninitPlayer();		// プレイヤー
	UninitScore();		// スコア
	UninitBestScore();	// ベストスコア
	UninitScoreUp();	// スコアアップ
//	UninitFlag();		// フラグ
	UninitCrater();		// クレーター
	UninitMenu();		// メニュー
	UninitPause();		// ポーズ
	UninitParticle();	// パーティクル
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	if (GetDirectJoypadTrigger(JOYKEY_DIRECT_7_BUTTON) || GetKeyboardTrigger(DIK_P))
	{
		s_bPause = !s_bPause;

		if (s_bPause)
		{// ポーズしてる
			SetPause();		// メニューの設定
		}
	}

	// ポーズ中ならポーズ以外を更新しない
	if (s_bPause)
	{
		UpdateMenu();		// メニュー
		UpdatePause();		// ポーズ
		return;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME1);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME2);
	}
	else if (GetKeyboardTrigger(DIK_F3))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME3);
	}
	else if (GetKeyboardTrigger(DIK_F4))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME4);
	}
	else if (GetKeyboardTrigger(DIK_F5))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME5);
	}
	else if (GetKeyboardTrigger(DIK_F6))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME6);
	}
	else if (GetKeyboardTrigger(DIK_F7))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME7);
	}

	if (GetKeyboardTrigger(DIK_1))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_SKY_1);
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_SHEONITE);
	}
	else if (GetKeyboardTrigger(DIK_3))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_WARP_3);
	}

	if (GetKeyboardTrigger(DIK_T))	// リザルトに移行デバッグ
	{
		ChangeMode(MODE_RESULT);
	}
#endif // !_DEBUG

	UpdateMap();		// マップデータ
	UpdateCrater();		// クレーター
	UpdateBoss();		// ボス
	UpdateEnemy();		// 敵
	UpdateBullet();		// 弾
	UpdateParticle();	// パーティクル
	UpdatePlayer();		// プレイヤー
	UpdateScore();		// スコア
	UpdateTarget();		// ターゲット
	UpdateLife();		// ライフ
	UpdateBestScore();	// ベストスコア
	UpdateScoreUp();	// スコアアップ
	//UpdateFlag();		// フラグ
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawMap();			// マップデータ
	DrawCrater();		// クレーター
	DrawBoss();			// ボス
	DrawEnemy();		// 敵
	DrawBullet();		// 弾
	DrawParticle();		// パーティクル
	DrawPlayer();		// プレイヤー
	DrawScore();		// スコア
	DrawTarget();		// ターゲット
	DrawLife();			// ライフ
	DrawBestScore();	// ベストスコア
	DrawScoreUp();		// スコアアップ
	//DrawFlag();			// フラグ
	if (s_bPause)
	{
		DrawPause();		// ポーズ
		DrawMenu();			// メニュー
	}
}

//=========================================
// ポーズの設定処理
//=========================================
void SetEnablePause(bool bUse)
{
	s_bPause = bUse;
}

//=========================================
// ポーズの状態を取得
//=========================================
bool GetPause(void)
{
	return s_bPause;
}
