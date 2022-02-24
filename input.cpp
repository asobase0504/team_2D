//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
// Author2 : ISOE JUKIA
//
// input.hを参照を推奨
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
#define MOUSE_KEY_MAX	(8)		//キーの最大数（マウス）
#define PLAYER_MAX		(4)		//プレイヤーの最大人数

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------

//キーボード
LPDIRECTINPUT8 g_pInput = NULL;						//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;			//入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];						//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];				//キーボードのトリガー情報


													//ジョイパッド
XINPUT_STATE g_JoyKeyState[PLAYER_MAX];				//ジョイパットのプレス情報
XINPUT_STATE g_JoyKeyStateTrigger[PLAYER_MAX];		//ジョイパットのトリガー情報
D3DXVECTOR3 g_JoyStickPos[PLAYER_MAX];				//ジョイスティックの傾き
XINPUT_VIBRATION g_JoyMoter[PLAYER_MAX];			//ジョイパッドのモーター
int g_nTime[PLAYER_MAX];							//振動持続時間
WORD g_nStrength[PLAYER_MAX];						//振動の強さ (0 - 65535)


													//マウス
LPDIRECTINPUT8 g_pMouseInput = NULL;				//Directinutオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;			//入力でパスへのポインタ
DIMOUSESTATE2 g_aKeyStateMouse;						//マウスのプレス処理
DIMOUSESTATE2 g_aKeyStatetriggerMouse;				//マウスのトリガー処理
POINT g_MousePos;									//マウスのカーソル用
HWND g_hMouseWnd;									//ウィンドウハンドル

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

//キーボード
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitKeyboard(void);								//終了処理
void UpdateKeyboard(void);								//更新処理

//ジョイパッド
HRESULT InitJoypad(void);								//初期化
void UninitJoypad(void);								//終了処理
void UpdateJoypad(void);								//更新処理

//マウス
HRESULT InitMouse(HINSTANCE hlnstance, HWND hWnd);		//初期化
void UninitMouse(void);									//終了処理
void UpdateMouse(void);									//更新処理


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

	//マウスの初期化処理
	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッド初期化
	InitJoypad();

	return S_OK;
}

//入力処理全部の終了処理
void UninitInput(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//マウスの終了処理
	UninitMouse();

	//ジョイパッド終了処理
	UninitJoypad();
}

//入力処理全部の更新処理
void UpdateInput(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//マウスの更新処理
	UpdateMouse();

	//ジョイパッド更新処理
	UpdateJoypad();
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
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //キーボードのトリガー情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保存
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

		//ジョイパッドの振動制御の０クリア
		ZeroMemory(&g_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//振動制御用の初期化
		g_nStrength[nCnt] = 0;
		g_nTime[nCnt] = 0;
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

		//ジョイパッドの振動
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

//コントローラーの振動制御
void JoypadVibration(int nTime, WORD nStrength, int nPlayer)
{
	g_nTime[nPlayer] = nTime;			//振動持続時間
	g_nStrength[nPlayer] = nStrength;	//振動の強さ
}

//*************************************************************************************
//マウス入力処理
//*************************************************************************************

//マウスの初期化
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pMouseInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（マウス）の生成
	if (FAILED(g_pMouseInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//ウィンドウハンドルの保管
	g_hMouseWnd = hWnd;

	//キーボードへのアクセス権を獲得
	g_pDevMouse->Acquire();

	return S_OK;
}

//マウスの終了処理
void UninitMouse(void)
{
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse = NULL;
	}
}

//マウスの更新処理
void UpdateMouse(void)
{
	DIMOUSESTATE2 aKeyState;	//マウスの入力情報

								//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{

		for (int nCntKey = 0; nCntKey < MOUSE_KEY_MAX; nCntKey++)
		{
			g_aKeyStatetriggerMouse.rgbButtons[nCntKey]
				= (g_aKeyStateMouse.rgbButtons[nCntKey]
					^ aKeyState.rgbButtons[nCntKey])
				& aKeyState.rgbButtons[nCntKey]; //キーボードのトリガー情報を保存
		}

		g_aKeyStateMouse = aKeyState;		//マウスのプレス情報を保存

	}
	else
	{
		g_pDevMouse->Acquire();			//キーボードへのアクセス権を獲得
	}
}

//マウスのプレス処理
bool GetMousePress(MOUSE mouse)
{
	return (g_aKeyStateMouse.rgbButtons[mouse] & 0x80) ? true : false;
}

//マウスのトリガー処理
bool GetMouseTrigger(MOUSE mouse)
{
	return (g_aKeyStatetriggerMouse.rgbButtons[mouse] & 0x80) ? true : false;
}

//マウスポインターの位置
D3DXVECTOR3 GetMouse(void)
{
	//画面上のマウスポインターの位置
	GetCursorPos(&g_MousePos);
	//ウィンドウ上のマウスポインターの位置
	ScreenToClient(g_hMouseWnd, &g_MousePos);

	return D3DXVECTOR3((float)g_MousePos.x, (float)g_MousePos.y, 0.0f);
}

//マウスのホイールの動き感知
int GetMouseWheel(void)
{
	return (int)g_aKeyStateMouse.lZ;
}