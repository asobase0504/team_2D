//------------------------------------------------------------------------------
//
// キーの設定
// Author;Yuda Kaito
//
//------------------------------------------------------------------------------
//--------------------------------------------------
// include
//--------------------------------------------------
#include "key_config.h"

//==================================================
// 移動キー
//==================================================
D3DXVECTOR3 MoveKey(void)
{
	D3DXVECTOR3 pDirection = D3DXVECTOR3(0.0f,0.0f,0.0f);

	if (GetKeyboardPress(DIK_W) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP))
	{// 上方向
		pDirection.y += -1.0f;
	}
	if (GetKeyboardPress(DIK_A) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_LEFT))
	{// 左方向
		pDirection.x += -1.0f;
	}
	if (GetKeyboardPress(DIK_S) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN))
	{// 下方向
		pDirection.y += 1.0f;
	}
	if (GetKeyboardPress(DIK_D) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_RIGHT))
	{// 右方向
		pDirection.x += 1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP_LEFT))
	{// 左上
		pDirection.y += -1.0f;
		pDirection.x += -1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN_LEFT))
	{// 左下
		pDirection.x += -1.0f;
		pDirection.y += 1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP_RIGHT))
	{// 右上
		pDirection.x += 1.0f;
		pDirection.y += -1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN_RIGHT))
	{// 右下
		pDirection.x += 1.0f;
		pDirection.y += 1.0f;
	}

	// 移動方向ベクトルの大きさを1.0fにする
	D3DXVec3Normalize(&pDirection, &pDirection);

	return pDirection;
}
