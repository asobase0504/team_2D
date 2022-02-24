//=============================================================================
//
// 入力処理 [input.h]
// Author1 : KOZUNA HIROHITO
// Author2 : ISOE JUKIA
//
//	＜最後の行を見ることを推奨＞
//
//=============================================================================

#ifndef _INPUT_H_		//このマクロ定義がされなかったら
#define _INPUT_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//ライブラリーファイルのリンク
//----------------------------------------------------------------------------

#pragma comment(lib,"dinput8.lib")//入力処理に必要(キーボード)
#pragma comment(lib,"xinput.lib")//入力処理に必要（ジョイパット）

//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------

#include "main.h"

//----------------------------------------------------------------------------
//列挙型
//----------------------------------------------------------------------------

//キー種類(ジョイパッド)
typedef enum
{
	JOYKEY_UP = 0,				//十字ボタン上
	JOYKEY_DOWN,				//十字ボタン下
	JOYKEY_LEFT,				//十字ボタン左
	JOYKEY_RIGHT,				//十字ボタン右
	JOYKEY_START,				//スタートボタン
	JOYKEY_BACK,				//バックボタン
	JOYKEY_LEFT_THUMB,			//左スティック押込み
	JOYKEY_RIGHT_THUMB,			//右スティック押込み
	JOYKEY_LEFT_SHOULDER,		//L１ボタン
	JOYKEY_RIGHT_SHOULDER,		//R１ボタン
	JOYKEY_LEFT_TRIGGER,		//L２トリガー
	JOYKEY_RIGHT_TRIGGER,		//R２トリガー
	JOYKEY_A,					//Aボタン
	JOYKEY_B,					//Bボタン
	JOYKEY_X,					//Xボタン
	JOYKEY_Y,					//Yボタン
	JOYKEY_LEFT_STICK,			//左スティック
	JOYKEY_RIGHT_STICK,			//右スティック
	JOYKEY_MAX
}JOYKEY;

//キー種類(マウス)
typedef enum
{
	MOUSE_INPUT_LEFT,			//マウス左ボタン
	MOUSE_INPUT_RIGHT,			//マウス右ボタン
	MOUSE_INPUT_MIDDLE,			//マウス中央ボタン
	MOUSE_INPUT_4,				//マウスボタン4
	MOUSE_INPUT_5,				//マウスボタン5
	MOUSE_INPUT_6,				//マウスボタン6
	MOUSE_INPUT_7,				//マウスボタン7
	MOUSE_INPUT_8,				//マウスボタン8
}MOUSE;

//----------------------------------------------------------------------------
//プロトタイプ宣言
//----------------------------------------------------------------------------

//入力処理のプロトタイプ宣言

HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);		//入力処理全部の初期化
void UninitInput(void);									//入力処理全部の終了処理
void UpdateInput(void);									//入力処理全部の更新処理

														//プロトタイプ宣言キーボード
bool GetKeyboardPress(int nKey);			//キーボードプレス処理
bool GetKeyboardTrigger(int nKey);			//キーボードトリガー処理
bool GetKeyboardAllPress(void);				//キーボード全キープレス処理
bool GetKeyboardAllTrigger(void);			//キーボード全キートリガー処理

											//プロトタイプ宣言ジョイパット
bool GetJoypadPress(JOYKEY Key, int nPlayer);					//ジョイパッドプレス処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//ジョイパッドトリガー処理
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			//ジョイパッドスティック処理
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//ジョイパッドトリガーペダル処理
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//ジョイパッド振動制御

																//プロトタイプ宣言マウス
bool GetMousePress(MOUSE mouse);			//マウスのプレス処理
bool GetMouseTrigger(MOUSE mouse);			//マウスのトリガー処理
D3DXVECTOR3 GetMouse(void);					//マウスの画面内の位置
int GetMouseWheel(void);					//マウスのホイールの動き処理

#endif

/*************************************************************************************************************

//main.hに追加分

//インクルード
#include "dinput.h"						//入力処理に必要
#include "Xinput.h"						//ジョイパット処理

//main.cppに追加分

//インクルード
#include "input.h"

//初期化処理に以下のコードを入力

//入力処理の初期化処理
if (FAILED(InitInput(hInstance, hWnd)))
{
return E_FAIL;
}

//終了処理に以下のコードを入力

//入力処理の終了処理
UninitInput();

//更新処理に以下のコードを入力

//入力処理の更新処理
UpdateInput();

//各関数の説明

//--------------------------------------------------------------------------------------------------------
//プロトタイプ宣言キーボード
//--------------------------------------------------------------------------------------------------------

{//引数にキーのマクロを入れそれが押されたかどうかをboolで返す
bool GetKeyboardPress(int nKey);			//キーボードプレス処理
bool GetKeyboardTrigger(int nKey);			//キーボードトリガー処理
bool GetKeyboardAllPress(void);				//キーボード全キープレス処理
bool GetKeyboardAllTrigger(void);			//キーボード全キートリガー処理
}

//--------------------------------------------------------------------------------------------------------
//プロトタイプ宣言ジョイパット
//--------------------------------------------------------------------------------------------------------

{//引数にキーの列挙型を入れそれが押されたかどうかをboolで返す
bool GetJoypadPress(JOYKEY Key, int nPlayer);					//ジョイパッドプレス処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//ジョイパッドトリガー処理
}

//返す値　スティックを倒せば倒すほど(1.0f or -1.0f)に近く
//スティックの遊びはifの条件式に組む
//例　if(GetJoypadStick(JOYKEY_RIGHT_STICK,0)  > 0.5f)
//    {//右に半分より多く倒したら

{第一引数はキー（スティック）,第二引数はプレイヤー番号
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			//ジョイパッドスティック処理
}

//返す値　ペダルがどれだけ押し込まれたかをintで返す
//ペダルの遊びはifの条件式に組む
//例　if(GetJoypadTriggerPedal(JOYKEY_RIGHT_TRIGGER,0) > 100))
//    {//右ペダルを100より多く倒したら

{第一引数はキー（スティック）,第二引数はプレイヤー番号
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//ジョイパッドトリガーペダル処理
}

{第一引数は揺らす時間, 第二引数はどれくらいの強さで揺らすか, 第三引数はプレイヤー番号
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//ジョイパッド振動制御
}

//--------------------------------------------------------------------------------------------------------
//プロトタイプ宣言マウス
//--------------------------------------------------------------------------------------------------------

{//引数にキーの列挙型を入れそれが押されたかどうかをboolで返す
bool GetMousePress(MOUSE mouse);			//マウスのプレス処理
bool GetMouseTrigger(MOUSE mouse);			//マウスのトリガー処理
}

//返す値　マウスのposを返す値
{
D3DXVECTOR3 GetMouse(void);					//マウスの画面内の位置
}

{//引数無し、返す値は奥に回すと＋で返り手前に回すとーで返ってきます
int GetMouseWheel(void);					//マウスのホイールの動き処理
}
************************************************************************************************************/


