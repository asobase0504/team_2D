//=============================================================================
//
// 入力処理 [jobiusinput.cpp]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "input.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define NUM_KEY_MAX		(256)	//キーの最大数（キーボード）
#define PLAYER_MAX		(2)		//プレイヤーの最大人数

//-----------------------------------------------------------------------------
//構造体
//-----------------------------------------------------------------------------

//ジョイパッド(DirectInput)
typedef struct
{
	LPDIRECTINPUT8 pJoyKeyInput = NULL;				//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 pDevJoyKey = NULL;			//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	DIJOYSTATE2 JoyKeyStateDirect;					//ジョイパット（プレス処理）
	DIJOYSTATE2 JoyKeyStateDirectTrigger;			//ジョイパット（トリガー処理）
	DIJOYSTATE2 JoyKeyStateDirectRelease;			//ジョイパット（リリース処理）
	DWORD OldJoyKeyDirect = 0;						//前回の十字キーの値
	JOYKEY_CROSS OldJoyKeyStickDirect;				//前回のスティックの位置
	bool bJoyKey = false;							//使っていつかどうか
}JoyKeyDirect;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------

//キーボード
static LPDIRECTINPUT8 g_pInput = NULL;						//DirectInputオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
static BYTE g_aKeyState[NUM_KEY_MAX];						//キーボードのプレス情報
static BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//キーボードのトリガー情報
static BYTE g_aKeyStateRelease[NUM_KEY_MAX];				//キーボードのリリース情報

//ジョイパッド(DirectInput)
static JoyKeyDirect g_aJoyKeyDirectState;					//ジョイパッド(DirectInput)の構造体変数

//ジョイパッド
static XINPUT_STATE g_JoyKeyState[PLAYER_MAX];				//ジョイパットのプレス情報
static XINPUT_STATE g_JoyKeyStateTrigger[PLAYER_MAX];		//ジョイパットのトリガー情報
static D3DXVECTOR3 g_JoyStickPos[PLAYER_MAX];				//ジョイスティックの傾き
static JOYKEY_CROSS g_OldJoyKeyStick;						//前回のスティックの位置

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

//キーボード
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);		//初期化
void UninitKeyboard(void);									//終了処理
void UpdateKeyboard(void);									//更新処理

//ジョイパッド(DirectInput)
HRESULT InitJoypadDirect(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitJoypadDirect(void);								//終了処理
void UpdateJoypadDirect(void);								//更新処理

//ジョイパッド
HRESULT InitJoypad(void);									//初期化
void UninitJoypad(void);									//終了処理
void UpdateJoypad(void);									//更新処理

//*************************************************************************************
//入力処理全体
//*************************************************************************************

//入力処理全部の初期化
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッド(DirectInput)の初期化処理

	/*if (FAILED(InitJoypadDirect(hInstance, hWnd)))
	{
		g_aJoyKeyDirectState.bJoyKey = false;
	}
	else
	{
		g_aJoyKeyDirectState.bJoyKey = true;
	}*/

	//ジョイパッド初期化
	//InitJoypad();

	return S_OK;
}

//入力処理全部の終了処理
void UninitInput(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//ジョイパッド(DirectInput)終了処理
	UninitJoypadDirect();

	//ジョイパッド終了処理
	UninitJoypad();
}

//入力処理全部の更新処理
void UpdateInput(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッド(DirectInput)更新処理
	//UpdateJoypadDirect();

	//ジョイパッド更新処理
	//UpdateJoypad();
}


//*****************************************************************************
//キーボード入力処理
//*****************************************************************************

//キーボード初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//キーボードの終了処理
void UninitKeyboard(void)
{
	//入力デバイス（キーボード）の放棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();		//キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破壊
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//キーボードの更新処理
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報
	int nCntKey;
	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];			//キーボードのトリガー情報を保存
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];		//キーボードのリリース情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];																//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();			//キーボードへのアクセス権を獲得
	}
}

//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//キーボードのリリース情報を取得
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//キーボードの全キープレス情報を取得
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

//キーボードの全キートリガー情報を取得
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
//ジョイパッド(DirectInput)入力処理
//*************************************************************************************

//ジョイパッド(DirectInput)初期化処理
HRESULT InitJoypadDirect(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_aJoyKeyDirectState.pJoyKeyInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（ジョイパッド(DirectInput)）の生成
	if (FAILED(g_aJoyKeyDirectState.pJoyKeyInput->CreateDevice(GUID_Joystick, &g_aJoyKeyDirectState.pDevJoyKey, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_aJoyKeyDirectState.pDevJoyKey->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_aJoyKeyDirectState.pDevJoyKey->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//ジョイパッド(DirectInput)へのアクセス権を獲得
	g_aJoyKeyDirectState.pDevJoyKey->Acquire();

	return S_OK;
}

//ジョイパッド(DirectInput)の終了処理
void UninitJoypadDirect(void)
{

	//入力デバイス（ジョイパッド(DirectInput)）の放棄
	if (g_aJoyKeyDirectState.pDevJoyKey != NULL)
	{
		g_aJoyKeyDirectState.pDevJoyKey->Unacquire();		//ジョイパッド(DirectInput)へのアクセス権を放棄
		g_aJoyKeyDirectState.pDevJoyKey->Release();
		g_aJoyKeyDirectState.pDevJoyKey = NULL;
	}

	//DirectInputオブジェクトの破壊
	if (g_aJoyKeyDirectState.pJoyKeyInput != NULL)
	{
		g_aJoyKeyDirectState.pJoyKeyInput->Release();
		g_aJoyKeyDirectState.pJoyKeyInput = NULL;
	}

}

//ジョイパッド(DirectInput)の更新処理
void UpdateJoypadDirect(void)
{
	DIJOYSTATE2 aKeyState;		//ジョイパッド(DirectInput)の入力情報

	if (g_aJoyKeyDirectState.bJoyKey)
	{
		//入力デバイスからデータを取得
		if (SUCCEEDED(g_aJoyKeyDirectState.pDevJoyKey->GetDeviceState(sizeof(aKeyState), &aKeyState)))
		{
			for (int nCnt = 0; nCnt < 32; nCnt++)
			{
				g_aJoyKeyDirectState.JoyKeyStateDirectTrigger.rgbButtons[nCnt] = (g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & aKeyState.rgbButtons[nCnt];									//キーボードのトリガー情報を保存
				g_aJoyKeyDirectState.JoyKeyStateDirectRelease.rgbButtons[nCnt] = (g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[nCnt] ^ aKeyState.rgbButtons[nCnt]) & g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[nCnt];		//キーボードのリリース情報を保存
			}
			g_aJoyKeyDirectState.JoyKeyStateDirect = aKeyState;		//プレス処理
		}
		else
		{
			g_aJoyKeyDirectState.pDevJoyKey->Acquire();			//ジョイパッド(DirectInput)へのアクセス権を獲得
		}
	}
}

//ジョイパッド(DirectInput)全キープレス処理
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

//ジョイパッド(DirectInput)プレス処理
bool GetDirectJoypadPress(JOYKEY_DIRECT Key)
{
	return (g_aJoyKeyDirectState.JoyKeyStateDirect.rgbButtons[Key] & 0x80) ? true : false;
}

//ジョイパッド(DirectInput)プレス処理(十字キーのみ)
bool GetDirectJoypadPress(JOYKEY_CROSS Key)
{
	return g_aJoyKeyDirectState.JoyKeyStateDirect.rgdwPOV[0] == Key;
}

//ジョイパッド(DirectInput)トリガー処理(十字キーのみ)
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

//ジョイパッド(DirectInput)トリガー処理
bool GetDirectJoypadTrigger(JOYKEY_DIRECT Key)
{
	return (g_aJoyKeyDirectState.JoyKeyStateDirectTrigger.rgbButtons[Key] & 0x80) ? true : false;
}

//ジョイパッド(DirectInput)リリース処理
bool GetDirectJoypadRelease(JOYKEY_DIRECT Key)
{
	return (g_aJoyKeyDirectState.JoyKeyStateDirectRelease.rgbButtons[Key] & 0x80) ? true : false;
}

//ジョイパッド(DirectInput)スティック処理
D3DXVECTOR3 GetDirectJoypadStick(JOYKEY_RIGHT_LEFT Key)
{

	if (g_aJoyKeyDirectState.bJoyKey)
	{//使用していたら
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
	//メモ、Xは右が１、左が-１、Yは上が１、下が-１

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//ジョイパッドスティック8方向プレス処理
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

//ジョイパッドスティック8方向トリガー処理
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
//ジョイパッド入力処理
//*************************************************************************************

//ジョイパッドの初期化
HRESULT InitJoypad(void)
{
	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//メモリーのクリア
		memset(&g_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&g_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));
	}
	return S_OK;
}

//ジョイパッドの終了
void UninitJoypad(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//ジョイパッドの更新
void UpdateJoypad(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~g_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons; //トリガー情報を保存
			g_JoyKeyState[nCnt] = JoyKeyState[nCnt];  //プレス処理
		}
	}
}

//ジョイパッドのプレス処理
bool GetJoypadPress(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパッドのトリガー処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパット（スティックプレス）処理
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

//ジョイパット（トリガーペダル）処理
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

//ジョイパッドスティックプレス８方向
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

//ジョイパッドスティックトリガ８方向
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