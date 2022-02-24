//=========================================
// 
// ゲーム内処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _GAME_H_
#define _GAME_H_

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitGame(void);	// 初期化処理
void UninitGame(void);	// 終了処理
void UpdateGame(void);	// 更新処理
void DrawGame();	// 描画処理
void SetEnablePause(bool bUse);	// ポーズの状態を設定する
bool GetPause(void);			// ポーズの状態取得

#endif // !_COMMON_H_
