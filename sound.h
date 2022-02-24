//=============================================================================
// 
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
// 
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// *****************************************************************************
// サウンドファイル
// *****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// BGMタイトル
	SOUND_LABEL_BGM_GAME1,			// BGMゲーム
	SOUND_LABEL_BGM_GAME2,			// BGMゲーム
	SOUND_LABEL_BGM_GAME3,			// BGMゲーム
	SOUND_LABEL_BGM_GAME4,			// BGMゲーム
	SOUND_LABEL_BGM_GAME5,			// BGMゲーム
	SOUND_LABEL_BGM_GAME6,			// BGMゲーム
	SOUND_LABEL_BGM_GAME7,			// BGMゲーム
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// *****************************************************************************
// プロトタイプ宣言
// *****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
