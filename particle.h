#pragma once
#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"

#define	MAX_PARTICLE	(4096)
#define	MAX_RECIPEPARTICLE	(15)

//構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col; //カラー
	int nLife;			//寿命
	bool bUse;		//使用してるかどうか
	int Trigger;   //種類
	float fRadeius;//半径
	int nType;//タイプ
	int nMaxLife;//ライフの最大
	int nCounterAnim;//カウンターを加算
	int nPatternAnim;//カウンター
}Particle;

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadeius, int nLife, int nType);

#endif // !_Particle_H_

