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
#include "player.h"
#include "score.h"
#include "enemy.h"
#include "input.h"
#include "map.h"
#include "bg.h"
#include "target.h"
#include "sound.h"
#include "fade.h"
#include <stdio.h>

//------------------------------------
// マクロ定義
//------------------------------------

//------------------------------------
// スタティック変数
//------------------------------------
static bool s_bPause;	// ポーズ中かどうか

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	s_bPause = false;

	InitBG();		// 背景
	InitMap();		// マップ
	InitEnemy();	// 敵
	InitBullet();	// 弾
	InitTarget();	// ターゲット
	InitPlayer();	// プレイヤー
	InitScore();	// スコア

	// マップの設定。
	InitMapSet(MAP_FILE0);

	PlaySound(SOUND_LABEL_BGM_GAME1);
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitEnemy();	// 敵	
	UninitBG();		// 背景
	UninitMap();	// マップ
	UninitBullet();	// 弾
	UninitPlayer();	// プレイヤー
	UninitScore();	// スコア
	UninitTarget();	// ターゲット
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	// ポーズの機能
	//if (GetJoypadTrigger(JOYKEY_START) || GetKeyboardTrigger(DIK_P))
	//{
	//	s_bPause = !s_bPause;
	//}

	//// ポーズ中ならポーズ以外を更新しない
	//if (s_bPause)
	//{
	//	UpdatePause();		// ポーズ
	//	return;
	//}

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
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_SHEONITE);
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_WARP_2);
	}
	else if (GetKeyboardTrigger(DIK_3))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f),25.0f, ENEMYTYPE_WARP_3);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		AddScore(123);
	}

	if (GetKeyboardTrigger(DIK_T))	// リザルトに移行デバッグ
	{
		SetFade(MODE_RESULT);
	}

	UpdateBG();		// 背景
	UpdateMap();	// マップデータ
	UpdateEnemy();	// 敵
	UpdateBullet();	// 弾
	UpdatePlayer();	// プレイヤー
	UpdateScore();	// スコア
	UpdateTarget();	// ターゲット
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawBG();		// 背景
	DrawMap();		// マップデータ
	DrawEnemy();	// 敵
	DrawBullet();	// 弾
	DrawPlayer();	// プレイヤー
	DrawScore();	// スコア
	DrawTarget();	// ターゲット
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
