//**************************************************************************************************
//
// プレイヤーの設定処理
// Author：唐﨑結斗
//
//**************************************************************************************************
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//二重インク―ルード防止のマクロ定義

#include"main.h"
#include"bullet.h"

//************************************************************
// プレイヤーの構造体を定義
//************************************************************
typedef enum
{
	PLAYER_STATE_NORMAL = 0,		// 通常状態
	PLAYER_STATE_DAMAGE,			// ダメージ状態
	PLAYER_STATE_START,				// スタート状
	MAX_PLAYER_STATE,				// 状態の最大数
}PLAYER_STATE;

//************************************************************
// プレイヤーの構造体を定義
//************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// 中心点
	D3DXVECTOR3			rot;				// 向き
	D3DXVECTOR3			move;				// 移動量
	D3DXVECTOR3			size;				// 大きさ
	D3DXCOLOR			col;				// カラー
	BulletType			BulletType;			// 弾の種類
	PLAYER_STATE		state;				// 状態
	float				fLength;			// 対角線の長さ
	float				fAngele;			// 対角線の角度
	float				fSpeed;				// 速度
	int					nLife;				// 体力
	int					nCntShot;			// 弾の発射までのカウント
	int					nIdxTarge;			// ターゲットナンバー
	int					nCntShotUse;		// 弾の発射ができるまでのカウント
	int					nCntState;			// 状態変更までのカウント
	bool				bTriggerShot;		// トリガー弾発射の可不可
	bool				bPressShot;			// プレス弾発射の可不可
	bool				bUse;				// 使用してるかどうか
}Player;

//****************************************
// プロトタイプ宣言
//****************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetPlayerVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele);

void ShotPlayer();
Player *GetPlayer(void);

#endif
