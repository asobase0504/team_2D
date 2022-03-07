//==================================================
//
// 3Dゲーム制作 ( fade.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _FADE_H_		//このマクロ定義がされてなかったら
#define _FADE_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
// 列挙型
//--------------------------------------------------
typedef enum
{
	FADE_NONE = 0,		// 何もしていない状態
	FADE_OUT,			// フェードアウト(見えない)
	FADE_IN,			// フェードイン(見える)
	FADE_SKIP,			// フェードスキップ
	FADE_MAX
}FADE;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext);
FADE GetFade(void);

#endif // !_FADE_H_
