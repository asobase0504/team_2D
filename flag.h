//========================================================
//
//SPフラッグ
//Author:髙野馨將
//
//========================================================
#ifndef _FLAG_H_
#define _FLAG_H_

#include "main.h"
//====================================
//SPフラッグの状態
//====================================
typedef enum
{
	FLAG_STATE_ON = 0,	//見える状態
	FLAG_STATE_OFF,		//隠れた状態
	FLAG_STATE_MAX
}FLAG_STATE;
//====================================
//SPフラッグ構造体の定義
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOld;	//位置
	D3DXVECTOR3 move;	//移動量
	FLAG_STATE state;	//状態
	float fWidth;		//幅
	float fHeight;		//高さ
	float fAngle;		//角度
	float fLength;		//長さ
	int nType;			//種類
	bool bUse;			//使用しているかどうか
}Flag;
//====================================
//プロトタイプ宣言
//====================================
void InitFlag(void);
void UninitFlag(void);
void UpdateFlag(void);
void DrawFlag(void);
void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
Flag * GetFlag(void);
void CollisionFlagBullet(D3DXVECTOR3 pPos, float fWidth, float fHeight);
void CollisionFlagPlayer(D3DXVECTOR3 pPos, float fWidth, float fHeight);
#endif