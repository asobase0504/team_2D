//====================================
// 
// 画面遷移のヘッダーファイル
// Author Yuda Kaito
// 
//====================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

// フェードの状態
typedef enum
{
	FADE_NONE = 0,	// 何もしていない状態
	FADE_IN,		// フェイドイン状態
	FADE_OUT,		// フェードアウト状態
	FADE_MAX
}FADE;

void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext);
void ResetFade(MODE modeNext);
FADE GetFade(void);


#endif // !_FADE_H_
