//================================
//
// タイトル画面のヘッダー
// Author:Tomoki Tomidokoro
//
//================================
#ifndef _STAFFROLL_H_		//このマクロが定義されてなかったら
#define _STAFFROLL_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	D3DXCOLOR col;		//色
	int		nCntStaff;	//大きさを決めるためのカウント
	int		nType;		//使用している画像のタイプ
	bool	bUse;		//使用しているかしていないか
}STAFFROLL;

//プロトタイプ宣言
void InitStaffroll(void);
void UninitStaffroll(void);
void UpdateStaffroll(void);
void DrawStaffroll(void);
void SetStaffroll(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);

#endif