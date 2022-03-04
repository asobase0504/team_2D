//========================================================
//
//SPtbO
//Author:ûüì]
//
//========================================================
#ifndef _FLAG_H_
#define _FLAG_H_

#include "main.h"
//====================================
//SPtbOÌóÔ
//====================================
typedef enum
{
	FLAG_STATE_ON = 0,	//©¦éóÔ
	FLAG_STATE_OFF,		//Bê½óÔ
	FLAG_STATE_MAX
}FLAG_STATE;
//====================================
//SPtbO\¢ÌÌè`
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//Êu
	D3DXVECTOR3 posOld;	//Êu
	D3DXVECTOR3 move;	//Ú®Ê
	FLAG_STATE state;	//óÔ
	float fWidth;		//
	float fHeight;		//³
	float fAngle;		//px
	float fLength;		//·³
	int nType;			//íÞ
	bool bUse;			//gpµÄ¢é©Ç¤©
}Flag;
//====================================
//vg^Cvé¾
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