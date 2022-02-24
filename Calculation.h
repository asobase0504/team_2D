#ifndef _CALCULATION_H_
#define _CALCULATION_H_

#include "main.h"

//---------------------
//イージングカーブ

//プロトタイプ宣言
float easeInOutBack(float x);	//少し進んで戻る感じ

float easeInElastic(float x);	//だんだん早くなる感じ

float easeOutBounce(float x);	//ボールが跳ねる感じの反対

float easeInBounce(float x);	//ボールが跳ねる感じ

#endif

