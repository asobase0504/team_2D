//==================================================
//
// 3Dゲーム制作 ( bestscore.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _BESTSCORE_H_		//このマクロ定義がされてなかったら
#define _BESTSCORE_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitBestScore(void);
void UninitBestScore(void);
void UpdateBestScore(void);
void DrawBestScore(void);
void BlinkBestScore(void);

#endif // !_BESTSCORE_H_
