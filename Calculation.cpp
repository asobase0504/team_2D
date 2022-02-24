//=========================================
// 
// イージング関数計算処理
// Author : Isoe Jukia
// Author : Hamano Ryuuga
// 
//=========================================
#include "main.h"
#include "Calculation.h"

//イージングカーブ
float easeInOutBack(float x)
{
	float c1 = 1.70158f;
	float c2 = c1 * 1.525f;

	float Answer1 = (powf(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) * 0.5f;
	float Answer2 = (powf(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;

	return x < 0.5f ? Answer1 : Answer2;
}

//イージングカーブ
float easeInElastic(float x)
{
	float c4 = (2 * D3DX_PI) / 3;

	float Answer1 = -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);

	return x == 0.0f ? 0.0f : x == 1.0f ? 1.0f : Answer1;
}

//イージングカーブ
float easeOutBounce(float x)
{
	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (x < 1 / d1)
	{
		return n1 * x * x;
	}
	else if (x < 2 / d1)
	{
		return n1 * (x -= 1.5f / d1) * x + 0.75f;
	}
	else if (x < 2.5f / d1)
	{
		return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	}
	else
	{
		return n1 * (x -= 2.625f / d1) * x + 0.984375f;
	}
}

//イージングカーブ
float easeInBounce(float x)
{
	return 1.0f - easeOutBounce(1.0f - x);
}
