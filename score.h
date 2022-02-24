//============================================================================================================
//
//君をたずねてにゃん千里[score.h]
//Author:Kishimoto Eiji
//
//============================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//============================================================================================================
//プロトタイプ宣言
//============================================================================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void SubScore(int nValue);
void SaveBestScore(void);
int LoadBestScore(void);
int GetScore(void);

#endif