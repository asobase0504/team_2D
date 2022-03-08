//=============================================================================
//
// 入力処理 [jobiusinput.h]
// Author1 : KOZUNA HIROHITO
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
	JOYKEY_MAX
}JOYKEY;

//キー種類(ジョイパッド(DirectInput))
typedef enum
{										//	PS5					スイッチpor				スイッチジョイコン（未調査）
	JOYKEY_DIRECT_0_BUTTON = 0,			//□ボタン			//Bボタン				//A
	JOYKEY_DIRECT_1_BUTTON,				//×ボタン			//Aボタン				//X
	JOYKEY_DIRECT_2_BUTTON,				//〇ボタン			//Yボタン				//B
	JOYKEY_DIRECT_3_BUTTON,				//△ボタン			//Xボタン				//Y
	JOYKEY_DIRECT_4_BUTTON,				//L1ボタン			//L1ボタン				//SL
	JOYKEY_DIRECT_5_BUTTON,				//R1ボタン			//R1ボタン				//SR
	JOYKEY_DIRECT_6_BUTTON,				//L2ボタン			//L2ボタン				//
	JOYKEY_DIRECT_7_BUTTON,				//R2ボタン			//R2ボタン				//
	JOYKEY_DIRECT_8_BUTTON,				//クリエイト		//-ボタン				//
	JOYKEY_DIRECT_9_BUTTON,				//オプション		//+ボタン				//
	JOYKEY_DIRECT_10_BUTTON,			//L3ボタン			//L3ボタン				//
	JOYKEY_DIRECT_11_BUTTON,			//R3ボタン			//R3ボタン				//
	JOYKEY_DIRECT_12_BUTTON,			//ホームボタン		//ホームボタン			//
	JOYKEY_DIRECT_13_BUTTON,			//タッチパッド		//キャプチャー			//
	JOYKEY_DIRECT_14_BUTTON,			//		//
	JOYKEY_DIRECT_15_BUTTON,			//		//
	JOYKEY_DIRECT_16_BUTTON,			//		//
	JOYKEY_DIRECT_17_BUTTON,			//左スティック		//
	JOYKEY_DIRECT_18_BUTTON,			//右スティック		//
	JOYKEY_DIRECT_19_BUTTON,			//十字キー（上）
	JOYKEY_DIRECT_20_BUTTON,			//十字キー（下）
	JOYKEY_DIRECT_21_BUTTON,			//十字キー（左）
	JOYKEY_DIRECT_22_BUTTON,			//十字キー（右）
	JOYKEY_DIRECT_MAX
}JOYKEY_DIRECT;

//キー種類(ジョイパッド(DirectInput))(十字キー&8方向スティック)
typedef enum
{
	JOYKEY_CROSS_UP = 0,								//上
	JOYKEY_CROSS_UP_RIGHT = 4500,					//右上
	JOYKEY_CROSS_RIGHT = 9000,						//右
	JOYKEY_CROSS_DOWN_RIGHT = 13500,					//右下
	JOYKEY_CROSS_DOWN = 18000,						//下
	JOYKEY_CROSS_DOWN_LEFT = 22500,					//左下
	JOYKEY_CROSS_LEFT = 27000,						//左
	JOYKEY_CROSS_UP_LEFT = 31500,					//左上
	JOYKEY_CROSS_MAX
}JOYKEY_CROSS;

//スティック識別(ジョイパッド)
typedef enum
{
	JOYKEY_RIGHT_STICK = 0,
	JOYKEY_LEFT_STICK,
	JOYKEY_RIGHT_LEFT_MAX
}JOYKEY_RIGHT_LEFT;

//----------------------------------------------------------------------------
//プロトタイプ宣言
//----------------------------------------------------------------------------

//入力処理のプロトタイプ宣言

HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);		//入力処理全部の初期化
void UninitInput(void);									//入力処理全部の終了処理
void UpdateInput(void);									//入力処理全部の更新処理

//プロトタイプ宣言キーボード
bool GetKeyboardPress(int nKey);						//キーボードプレス処理
bool GetKeyboardTrigger(int nKey);						//キーボードトリガー処理
bool GetKeyboardRelease(int nKey);						//キーボードリリース処理
bool GetKeyboardAllPress(void);							//キーボード全キープレス処理
bool GetKeyboardAllTrigger(void);						//キーボード全キートリガー処理

//プロトタイプ宣言ジョイパット(DirectInput)
bool GetDirectJoypadPress(JOYKEY_DIRECT Key);												//ジョイパッドプレス処理
bool GetDirectJoypadPress(JOYKEY_CROSS Key);												//ジョイパッドプレス処理
bool GetDirectJoypadTrigger(JOYKEY_DIRECT Key);												//ジョイパッドトリガー処理
bool GetDirectJoypadTrigger(JOYKEY_CROSS Key);												//ジョイパッドトリガー処理
bool GetDirectJoypadRelease(JOYKEY_DIRECT Key);												//ジョイパッドリリース処理
bool GetDirectJoypadAllPress(void);															//ジョイパッド全キープレス処理
D3DXVECTOR3 GetDirectJoypadStick(JOYKEY_RIGHT_LEFT Key);									//ジョイパッドスティック処理
bool GetDirectJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);				//ジョイパッドスティック8方向プレス処理
bool GetDirectJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);			//ジョイパッドスティック8方向トリガー処理

//プロトタイプ宣言ジョイパット
bool GetJoypadPress(JOYKEY Key, int nPlayer);															//ジョイパッドプレス処理
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);															//ジョイパッドトリガー処理
D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft, int nPlayer);									//ジョイパッドスティック処理
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);														//ジョイパッドトリガーペダル処理
bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer);					//ジョイパッドスティックプレス８方向
bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer);					//ジョイパッドスティックトリガー８方向
#endif