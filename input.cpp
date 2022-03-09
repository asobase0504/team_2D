//=============================================================================
//
// ���͏��� [jobiusinput.cpp]
// Author1 : KOZUNA HIROHITO
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
#define PLAYER_MAX		(2)		//�v���C���[�̍ő�l��

//-----------------------------------------------------------------------------
//�\����
//-----------------------------------------------------------------------------

//�W���C�p�b�h(DirectInput)
typedef struct
{
	LPDIRECTINPUT8 pJoyKeyInput = NULL;				//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 pDevJoyKey = NULL;			//���̓f�o�C�X�i�L�[�{�[�h�i�R���g���[���[�p�͕ʂɍ��j�j�ւ̃|�C���^
	DIJOYSTATE2 JoyKeyStateDirect;					//�W���C�p�b�g�i�v���X�����j
	DIJOYSTATE2 JoyKeyStateDirectTrigger;			//�W���C�p�b�g�i�g���K�[�����j
	DIJOYSTATE2 JoyKeyStateDirectRelease;			//�W���C�p�b�g�i�����[�X�����j
	DWORD OldJoyKeyDirect = 0;						//�O��̏\���L�[�̒l
	JOYKEY_CROSS OldJoyKeyStickDirect;				//�O��̃X�e�B�b�N�̈ʒu
	bool bJoyKey = false;							//�g���Ă����ǂ���
}JoyKeyDirect;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------

//�L�[�{�[�h
static LPDIRECTINPUT8 g_pInput = NULL;						//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//���̓f�o�C�X�i�L�[�{�[�h�i�R���g���[���[�p�͕ʂɍ��j�j�ւ̃|�C���^
static BYTE g_aKeyState[NUM_KEY_MAX];						//�L�[�{�[�h�̃v���X���
static BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//�L�[�{�[�h�̃g���K�[���
static BYTE g_aKeyStateRelease[NUM_KEY_MAX];				//�L�[�{�[�h�̃����[�X���

//�W���C�p�b�h(DirectInput)
static JoyKeyDirect g_aJoyKeyDirectState;					//�W���C�p�b�h(DirectInput)�̍\���̕ϐ�

//�W���C�p�b�h
static XINPUT_STATE g_JoyKeyState[PLAYER_MAX];				//�W���C�p�b�g�̃v���X���
static XINPUT_STATE g_JoyKeyStateTrigger[PLAYER_MAX];		//�W���C�p�b�g�̃g���K�[���
static D3DXVECTOR3 g_JoyStickPos[PLAYER_MAX];				//�W���C�X�e�B�b�N�̌X��
static JOYKEY_CROSS g_OldJoyKeyStick;						//�O��̃X�e�B�b�N�̈ʒu

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//�L�[�{�[�h
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);		//������
void UninitKeyboard(void);									//�I������
void UpdateKeyboard(void);									//�X�V����

//�W���C�p�b�h(DirectInput)
HRESULT InitJoypadDirect(HINSTANCE hInstance, HWND hWnd);	//������
void UninitJoypadDirect(void);								//�I������
void UpdateJoypadDirect(void);								//�X�V����

//�W���C�p�b�h
HRESULT InitJoypad(void);									//������
void UninitJoypad(void);									//�I������
void UpdateJoypad(void);									//�X�V����

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

	//�W���C�p�b�h(DirectInput)�̏���������

	/*if (FAILED(InitJoypadDirect(hInstance, hWnd)))
	{
		g_aJoyKeyDirectState.bJoyKey = false;
	}
	else
	{
		g_aJoyKeyDirectState.bJoyKey = true;
	}*/

	//�W���C�p�b�h������
	//InitJoypad();

	return S_OK;
}

//���͏����S���̏I������
void UninitInput(void)
{
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�W���C�p�b�h(DirectInput)�I������
	UninitJoypadDirect();

	//�W���C�p�b�h�I������
	UninitJoypad();
}

//���͏����S���̍X�V����
void UpdateInput(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�W���C�p�b�h(DirectInput)�X�V����
	//UpdateJoypadDirect();

	//�W���C�p�b�h�X�V����
	//UpdateJoypad();
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
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];			//�L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];		//�L�[�{�[�h�̃����[�X����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];																//�L�[�{�[�h�̃v���X����ۑ�
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

//�L�[�{�[�h�̃����[�X�����擾
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
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
//�W���C�p�b�h(DirectInput)���͏���
//*************************************************************************************

//�W���C�p�b�h(DirectInput)����������
HRESULT InitJoypadDirect(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_aJoyKeyDirectState.pJoyKeyInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�W���C�p�b�h(DirectInput)�j�̐���
	if (FAILED(g_aJoyKeyDirectState.pJoyKeyInput->CreateDevice(GUID_Joystick, &g_aJoyKeyDirectState.pDevJoyKey, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_aJoyKeyDirectState.pDevJoyKey->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_aJoyKeyDirectState.pDevJoyKey->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�W���C�p�b�h(DirectInput)�ւ̃A�N�Z�X�����l��
	g_aJoyKeyDirectState.pDevJoyKey->Acquire();

	return S_OK;
}

//�W���C�p�b�h(DirectInput)�̏I������
void UninitJoypadDirect(void)
{

	//���̓f�o�C�X�i�W���C�p�b�h(DirectInput)�j�̕���
	if (g_aJoyKeyDirectState.pDevJoyKey != NULL)
	{
		g_aJoyKeyDirectState.pDevJoyKey->Unacquire();		//�W���C�p�b�h(DirectInput)�ւ̃A�N�Z�X�������
		g_aJoyKeyDirectState.pDevJoyKey->Release();
		g_aJoyKeyDirectState.pDevJoyKey = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_aJoyKeyDirectState.pJoyKeyInput != NULL)
	{
		g_aJoyKeyDirectState.pJoyKeyInput->Release();
		g_aJoyKeyDirectState.pJoyKeyInput = NULL;
	}

}

//�W���C�p�b�h(DirectInput)�̍X�V����
void UpdateJoypadDirect(void)
{
	DIJOYSTATE2 aKeyState;		//�W���C�p�b�h(DirectInput)�̓��͏��

	if (g_aJoyKeyDirectState.bJoyKey)
	{
		//���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(g_aJoyKeyDirectState.pDevJoyKey->GetDeviceState(sizeof(aKeyState), &aKeyState)))
		{
			for (int nCnt = 0; nCnt < 32; nCnt++)
			{
				g_aJoyKeyDirectState.JoyKeyStateDirectTrigger.rgbButtons[nCnt] = (g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & aKeyState.rgbButtons[nCnt];									//�L�[�{�[�h�̃g���K�[����ۑ�
				g_aJoyKeyDirectState.JoyKeyStateDirectRelease.rgbButtons[nCnt] = (g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[nCnt];		//�L�[�{�[�h�̃����[�X����ۑ�
			}
			g_aJoyKeyDirectState.JoyKeyStateDirect = aKeyState;		//�v���X����
		}
		else
		{
			g_aJoyKeyDirectState.pDevJoyKey->Acquire();			//�W���C�p�b�h(DirectInput)�ւ̃A�N�Z�X�����l��
		}
	}
}

//�W���C�p�b�h(DirectInput)�S�L�[�v���X����
bool GetDirectJoypadAllPress(void)
{
	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		if (GetDirectJoypadPress(JOYKEY_DIRECT(nCnt)))
		{
			return true;
		}
	}
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[nCnt] != 0xFFFFFFFF)
		{
			return true;
		}
	}
	return false;
}

//�W���C�p�b�h(DirectInput)�v���X����
bool GetDirectJoypadPress(JOYKEY_DIRECT Key)
{
	return (g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[Key] & 0x80) ? true : false;
}

//�W���C�p�b�h(DirectInput)�v���X����(�\���L�[�̂�)
bool GetDirectJoypadPress(JOYKEY_CROSS Key)
{
	return g_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] == Key;
}

//�W���C�p�b�h(DirectInput)�g���K�[����(�\���L�[�̂�)
bool GetDirectJoypadTrigger(JOYKEY_CROSS Key)
{
	if (g_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] == g_aJoyKeyDirectState.OldJoyKeyDirect)
	{
		return false;
	}
	else if (g_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] == Key
		&& g_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] != g_aJoyKeyDirectState.OldJoyKeyDirect)
	{
		g_aJoyKeyDirectState.OldJoyKeyDirect = g_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0];
		return true;
	}
	g_aJoyKeyDirectState.OldJoyKeyDirect = 0xffffffff;
	return false;
}

//�W���C�p�b�h(DirectInput)�g���K�[����
bool GetDirectJoypadTrigger(JOYKEY_DIRECT Key)
{
	return (g_aJoyKeyDirectState.JoyKeyStateDirectTrigger.rgbButtons[Key] & 0x80) ? true : false;
}

//�W���C�p�b�h(DirectInput)�����[�X����
bool GetDirectJoypadRelease(JOYKEY_DIRECT Key)
{
	return (g_aJoyKeyDirectState.JoyKeyStateDirectRelease.rgbButtons[Key] & 0x80) ? true : false;
}

//�W���C�p�b�h(DirectInput)�X�e�B�b�N����
D3DXVECTOR3 GetDirectJoypadStick(JOYKEY_RIGHT_LEFT Key)
{

	if (g_aJoyKeyDirectState.bJoyKey)
	{//�g�p���Ă�����
		switch (Key)
		{
		case JOYKEY_RIGHT_STICK:
			return D3DXVECTOR3(float(g_aJoyKeyDirectState.JoyKeyStateDirect.lZ) / 32767.0f - 1.0f, -float(g_aJoyKeyDirectState.JoyKeyStateDirect.lRz) / 32767.0f + 1.0f, 0.0f);
			break;
		case JOYKEY_LEFT_STICK:
			return D3DXVECTOR3(float(g_aJoyKeyDirectState.JoyKeyStateDirect.lX) / 32767.0f - 1.0f, -float(g_aJoyKeyDirectState.JoyKeyStateDirect.lY) / 32767.0f + 1.0f, 0.0f);
			break;
		}
	}
	//�����AX�͉E���P�A����-�P�AY�͏オ�P�A����-�P

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//�W���C�p�b�h�X�e�B�b�N8�����v���X����
bool GetDirectJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key)
{
	D3DXVECTOR3 pos = GetDirectJoypadStick(RightLeft);

	if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_RIGHT == Key)
	{
		return true;
	}
	else if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_LEFT == Key)
	{
		return true;
	}
	return false;
}

//�W���C�p�b�h�X�e�B�b�N8�����g���K�[����
bool GetDirectJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key)
{
	if (GetDirectJoypadStickPress(RightLeft, Key)
		&& Key != g_aJoyKeyDirectState.OldJoyKeyStickDirect)
	{
		g_aJoyKeyDirectState.OldJoyKeyStickDirect = Key;
		return true;
	}
	else if (GetDirectJoypadStickPress(RightLeft, Key)
		&& Key == g_aJoyKeyDirectState.OldJoyKeyStickDirect)
	{
		return false;
	}

	g_aJoyKeyDirectState.OldJoyKeyStickDirect = JOYKEY_CROSS_MAX;
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
D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft, int nPlayer)
{
	switch (RightLeft)
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

//�W���C�p�b�h�X�e�B�b�N�v���X�W����
bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer)
{
	D3DXVECTOR3 pos = GetJoypadStick(RightLeft, nPlayer);

	if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_RIGHT == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_RIGHT == Key)
	{
		return true;
	}
	else if ((pos.x < 0.5f && pos.x > -0.5f)
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y < -0.5f
		&& JOYKEY_CROSS_DOWN_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& (pos.y < 0.5f && pos.y > -0.5f)
		&& JOYKEY_CROSS_LEFT == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& pos.y > 0.5f
		&& JOYKEY_CROSS_UP_LEFT == Key)
	{
		return true;
	}
	return false;
}

//�W���C�p�b�h�X�e�B�b�N�g���K�W����
bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer)
{
	if (GetJoypadStickPress(RightLeft, Key, nPlayer)
		&& Key != g_OldJoyKeyStick)
	{
		g_OldJoyKeyStick = Key;
		return true;
	}
	else if (GetJoypadStickPress(RightLeft, Key, nPlayer)
		&& Key == g_OldJoyKeyStick)
	{
		return false;
	}

	g_OldJoyKeyStick = JOYKEY_CROSS_MAX;
	return false;
}