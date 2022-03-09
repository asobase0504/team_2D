//==================================================
//
// FPD制作 ( menu.h )
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

/*↓ メニューの引数 ↓*/

typedef struct
{
	LPDIRECT3DTEXTURE9		*pTexture[MAX_OPTION];		// テクスチャ
	int						nNumUse;					// 使用数
	float					fLeft;						// 左端
	float					fRight;						// 右端
	float					fTop;						// 上端
	float					fBottom;					// 下端
	float					fWidth;						// 選択肢の幅
	float					fHeight;					// 選択肢の高さ
	bool					bSort;						// 並べ方 [ true : 縦 false : 横 ]
}MenuArgument;

/*↓ 枠の引数 ↓*/

typedef struct
{
	LPDIRECT3DTEXTURE9		*pTexture;		// テクスチャ
	D3DXCOLOR				col;			// 色
	bool					bUse;			// 枠がいるかどうか [ true : いる false : いらない ]
}FrameArgument;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame);
void InitColorOption(void);
void ChangeOption(int Idx);
void DecisionOption(void);
void ResetMenu(int nIdx);

#endif // !_MENU_H_
