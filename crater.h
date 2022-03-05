//==================================================
//
// 3Dゲーム制作 ( crater.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _CRATER_H_		//このマクロ定義がされてなかったら
#define _CRATER_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitCrater(void);
void UninitCrater(void);
void UpdateCrater(void);
void DrawCrater(void);
void SetCrater(D3DXVECTOR3 pos);

#endif // !_CRATER_H_