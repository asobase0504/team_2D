//------------------------------------------------------------------------------
//
// �L�[�̐ݒ�
// Author;Yuda Kaito
//
//------------------------------------------------------------------------------
//--------------------------------------------------
// include
//--------------------------------------------------
#include "key_config.h"

//==================================================
// �ړ��L�[
//==================================================
D3DXVECTOR3 MoveKey(void)
{
	D3DXVECTOR3 pDirection = D3DXVECTOR3(0.0f,0.0f,0.0f);

	if (GetKeyboardPress(DIK_W) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP))
	{// �����
		pDirection.y += -1.0f;
	}
	if (GetKeyboardPress(DIK_A) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_LEFT))
	{// ������
		pDirection.x += -1.0f;
	}
	if (GetKeyboardPress(DIK_S) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN))
	{// ������
		pDirection.y += 1.0f;
	}
	if (GetKeyboardPress(DIK_D) || GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_RIGHT))
	{// �E����
		pDirection.x += 1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP_LEFT))
	{// ����
		pDirection.y += -1.0f;
		pDirection.x += -1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN_LEFT))
	{// ����
		pDirection.x += -1.0f;
		pDirection.y += 1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP_RIGHT))
	{// �E��
		pDirection.x += 1.0f;
		pDirection.y += -1.0f;
	}
	if (GetDirectJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN_RIGHT))
	{// �E��
		pDirection.x += 1.0f;
		pDirection.y += 1.0f;
	}

	// �ړ������x�N�g���̑傫����1.0f�ɂ���
	D3DXVec3Normalize(&pDirection, &pDirection);

	return pDirection;
}
