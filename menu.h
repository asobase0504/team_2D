//==================================================
//
// ゲーム制作 ( menu.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MENU_H_		//このマクロ定義がされてなかったら
#define _MENU_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_OPTION		(16)		// 選択肢の最大数

//--------------------------------------------------
// 構造体
//--------------------------------------------------
typedef struct
{
	int						nNumUse;					// 使用数
	float					fLeft;						// 左端
	float					fRight;						// 右端
	float					fTop;						// 上端
	float					fBottom;					// 下端
	float					fWidth;						// 選択肢の幅
	float					fHeight;					// 選択肢の高さ
	LPDIRECT3DTEXTURE9		*pTexture[MAX_OPTION];		// テクスチャ
}MenuArgument;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
int SetMenu(const MenuArgument &menu);
void ResetMenu(int nIdx);

#endif // !_MENU_H_
