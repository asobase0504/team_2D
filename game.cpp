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
#include "sound.h"
#include <stdio.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define MAP_FILE	"data/txt/map.txt"

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
	InitPlayer();	// プレイヤー
	InitScore();	// スコア

	// マップの設定。
	InitMapSet(MAP_FILE);

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
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), TYPEENEMY_SKY_1);
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), TYPEENEMY_SKY_2);
	}
	else if (GetKeyboardTrigger(DIK_3))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), TYPEENEMY_SKY_3);
	}
	//確認用(消してok)
	if (GetKeyboardTrigger(DIK_P))
	{
		AddScore(123);
	}

	UpdateBG();		// 背景
	UpdateMap();	// マップデータ
	UpdateEnemy();	// 敵
	UpdateBullet();	// 弾
	UpdatePlayer();	// プレイヤー
	UpdateScore();	// スコア
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
