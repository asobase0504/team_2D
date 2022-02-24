//=============================================================================
//
// ���͏��� [input.cpp]
// Author1 : KOZUNA HIROHITO
// Author2 : ISOE JUKIA
//
// input.h���Q�Ƃ𐄏�
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include "input.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------

#define NUM_KEY_MAX		(256)	//�L�[�̍ő吔�i�L�[�{�[�h�j
#define MOUSE_KEY_MAX	(8)		//�L�[�̍ő吔�i�}�E�X�j
#define PLAYER_MAX		(4)		//�v���C���[�̍ő�l��

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------

//�L�[�{�[�h
LPDIRECTINPUT8 g_pInput = NULL;						//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//���̓f�o�C�X�i�L�[�{�[�h�i�R���g���[���[�p�͕ʂɍ��j�j�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];						//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//�L�[�{�[�h�̃g���K�[���


													//�W���C�p�b�h
XINPUT_STATE g_JoyKeyState[PLAYER_MAX];				//�W���C�p�b�g�̃v���X���
XINPUT_STATE g_JoyKeyStateTrigger[PLAYER_MAX];		//�W���C�p�b�g�̃g���K�[���
D3DXVECTOR3 g_JoyStickPos[PLAYER_MAX];				//�W���C�X�e�B�b�N�̌X��
XINPUT_VIBRATION g_JoyMoter[PLAYER_MAX];			//�W���C�p�b�h�̃��[�^�[
int g_nTime[PLAYER_MAX];							//�U����������
WORD g_nStrength[PLAYER_MAX];						//�U���̋��� (0 - 65535)


													//�}�E�X
LPDIRECTINPUT8 g_pMouseInput = NULL;				//Directinut�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;			//���͂Ńp�X�ւ̃|�C���^
DIMOUSESTATE2 g_aKeyStateMouse;						//�}�E�X�̃v���X����
DIMOUSESTATE2 g_aKeyStatetriggerMouse;				//�}�E�X�̃g���K�[����
POINT g_MousePos;									//�}�E�X�̃J�[�\���p
HWND g_hMouseWnd;									//�E�B���h�E�n���h��

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//�L�[�{�[�h
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	//������
void UninitKeyboard(void);								//�I������
void UpdateKeyboard(void);								//�X�V����

//�W���C�p�b�h
HRESULT InitJoypad(void);								//������
void UninitJoypad(void);								//�I������
void UpdateJoypad(void);								//�X�V����

//�}�E�X
HRESULT InitMouse(HINSTANCE hlnstance, HWND hWnd);		//������
void UninitMouse(void);									//�I������
void UpdateMouse(void);									//�X�V����


//*************************************************************************************
//���͏����S��
//*************************************************************************************

//���͏����S���̏�����
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�}�E�X�̏���������
	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�W���C�p�b�h������
	InitJoypad();

	return S_OK;
}

//���͏����S���̏I������
void UninitInput(void)
{
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�}�E�X�̏I������
	UninitMouse();

	//�W���C�p�b�h�I������
	UninitJoypad();
}

//���͏����S���̍X�V����
void UpdateInput(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�}�E�X�̍X�V����
	UpdateMouse();

	//�W���C�p�b�h�X�V����
	UpdateJoypad();
}


//*****************************************************************************
//�L�[�{�[�h���͏���
//*****************************************************************************

//�L�[�{�[�h����������
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�L�[�{�[�h�j�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//�L�[�{�[�h�̏I������
void UninitKeyboard(void)
{
	//���̓f�o�C�X�i�L�[�{�[�h�j�̕���
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();		//�L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//�L�[�{�[�h�̍X�V����
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		//�L�[�{�[�h�̓��͏��
	int nCntKey;
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //�L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();			//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//�L�[�{�[�h�̃v���X�����擾
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�̃g���K�[�����擾
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�̑S�L�[�v���X�����擾
bool GetKeyboardAllPress(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardPress(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//�L�[�{�[�h�̑S�L�[�g���K�[�����擾
bool GetKeyboardAllTrigger(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardTrigger(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//*************************************************************************************
//�W���C�p�b�h���͏���
//*************************************************************************************

//�W���C�p�b�h�̏�����
HRESULT InitJoypad(void)
{
	//XInput�̃X�e�[�g��ݒ�i�L���ɂ���j
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//�������[�̃N���A
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		//�W���C�p�b�h�̐U������̂O�N���A
		ZeroMemory(&g_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//�U������p�̏�����
		g_nStrength[nCnt] = 0;
		g_nTime[nCnt] = 0;
	}
	return S_OK;
}

//�W���C�p�b�h�̏I��
void UninitJoypad(void)
{
	//XInput�̃X�e�[�g��ݒ�i�����ɂ���j
	XInputEnable(false);
}

//�W���C�p�b�h�̍X�V
void UpdateJoypad(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		//�W���C�p�b�h���͏��

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//�W���C�p�b�h�̏�Ԃ��擾
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~g_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons; //�g���K�[����ۑ�
			g_JoyKeyState[nCnt] = JoyKeyState[nCnt];  //�v���X����
		}

		//�W���C�p�b�h�̐U��
		g_JoyMoter[nCnt].wLeftMotorSpeed = g_nStrength[nCnt];
		g_JoyMoter[nCnt].wRightMotorSpeed = g_nStrength[nCnt];
		XInputSetState(nCnt, &g_JoyMoter[nCnt]);

		if (g_nTime[nCnt] > 0)
		{
			g_nTime[nCnt]--;
		}
		else
		{
			g_nStrength[nCnt] = 0;
			g_nTime[nCnt] = 0;
		}
	}
}

//�W���C�p�b�h�̃v���X����
bool GetJoypadPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//�W���C�p�b�h�̃g���K�[����
bool GetJoypadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//�W���C�p�b�g�i�X�e�B�b�N�v���X�j����
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		g_JoyStickPos[nPlayer] = D3DXVECTOR3(g_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -g_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return g_JoyStickPos[nPlayer];
}

//�W���C�p�b�g�i�g���K�[�y�_���j����
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = g_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//�R���g���[���[�̐U������
void JoypadVibration(int nTime, WORD nStrength, int nPlayer)
{
	g_nTime[nPlayer] = nTime;			//�U����������
	g_nStrength[nPlayer] = nStrength;	//�U���̋���
}

//*************************************************************************************
//�}�E�X���͏���
//*************************************************************************************

//�}�E�X�̏�����
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pMouseInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�}�E�X�j�̐���
	if (FAILED(g_pMouseInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�E�B���h�E�n���h���̕ۊ�
	g_hMouseWnd = hWnd;

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevMouse->Acquire();

	return S_OK;
}

//�}�E�X�̏I������
void UninitMouse(void)
{
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse = NULL;
	}
}

//�}�E�X�̍X�V����
void UpdateMouse(void)
{
	DIMOUSESTATE2 aKeyState;	//�}�E�X�̓��͏��

								//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{

		for (int nCntKey = 0; nCntKey < MOUSE_KEY_MAX; nCntKey++)
		{
			g_aKeyStatetriggerMouse.rgbButtons[nCntKey]
				= (g_aKeyStateMouse.rgbButtons[nCntKey]
					^ aKeyState.rgbButtons[nCntKey])
				& aKeyState.rgbButtons[nCntKey]; //�L�[�{�[�h�̃g���K�[����ۑ�
		}

		g_aKeyStateMouse = aKeyState;		//�}�E�X�̃v���X����ۑ�

	}
	else
	{
		g_pDevMouse->Acquire();			//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//�}�E�X�̃v���X����
bool GetMousePress(MOUSE mouse)
{
	return (g_aKeyStateMouse.rgbButtons[mouse] & 0x80) ? true : false;
}

//�}�E�X�̃g���K�[����
bool GetMouseTrigger(MOUSE mouse)
{
	return (g_aKeyStatetriggerMouse.rgbButtons[mouse] & 0x80) ? true : false;
}

//�}�E�X�|�C���^�[�̈ʒu
D3DXVECTOR3 GetMouse(void)
{
	//��ʏ�̃}�E�X�|�C���^�[�̈ʒu
	GetCursorPos(&g_MousePos);
	//�E�B���h�E��̃}�E�X�|�C���^�[�̈ʒu
	ScreenToClient(g_hMouseWnd, &g_MousePos);

	return D3DXVECTOR3((float)g_MousePos.x, (float)g_MousePos.y, 0.0f);
}

//�}�E�X�̃z�C�[���̓������m
int GetMouseWheel(void)
{
	return (int)g_aKeyStateMouse.lZ;
}