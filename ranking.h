//============================================================================================================
//
//チーム制作[ranking.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//============================================================================================================
//ランキングスコアの構造体の定義
//============================================================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	int nScore;			//スコア
}RankScore;

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);
void SaveRanking(void);
void LoadRanking(void);

#endif