//**************************************************************************************************
//
// 当たり判定
// Author：小綱啓仁
//
//**************************************************************************************************

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "collision.h"

//-----------------------------------------------------------------------------
//円の当たり判定
//-----------------------------------------------------------------------------
bool CollisionCircle(D3DXVECTOR3 Pos1, float fRadius1, D3DXVECTOR3 Pos2, float fRadius2)
{
	//２この物体の半径同士の和
	float fDiff = fRadius1 + fRadius2;

	//計算用変数
	float fCalculationX, fCalculationY;

	//Xの差分
	fCalculationX = Pos1.x - Pos2.x;
	//Yの差分
	fCalculationY = Pos1.y - Pos2.y;

	//現在の２点の距離
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationY * fCalculationY);

	//２この物体の半径同士の和より現在の２点の距離が小さいかどうか
	if (fDiff >= fLength)
	{
		//当たった
		return true;
	}

	//当たってない
	return false;
}