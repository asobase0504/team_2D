//==================================================
//
// 3Dゲーム制作 ( scoreup.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _SCOREUP_H_		//このマクロ定義がされてなかったら
#define _SCOREUP_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitScoreUp(void);
void UninitScoreUp(void);
void UpdateScoreUp(void);
void DrawScoreUp(void);
void SetScoreUp(int nScore);

#endif // !_SCOREUP_H_