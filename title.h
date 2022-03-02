//================================
//
// タイトル画面のヘッダー
// Author:Teruto Sato
//
//================================
#ifndef _TITLE_H_		//このマクロが定義されてなかったら
#define _TITLE_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	D3DXCOLOR col;		//色
}TITLE;

//プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif