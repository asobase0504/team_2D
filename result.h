//============================
//
// リザルト画面のヘッダー
// Author:Teruto Sato
//
//============================
#ifndef _RESULT_H_		//このマクロが定義されてなかったら
#define _RESULT_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fWidth;		//幅
	float fHeight;		//高さ
	D3DXCOLOR col;		//色
}RESULT;

//プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif