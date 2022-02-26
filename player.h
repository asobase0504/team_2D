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
typedef struct
{
	D3DXVECTOR3			pos;				// 中心点
	D3DXVECTOR3			rot;				// 向き
	D3DXVECTOR3			move;				// 移動量
	D3DXVECTOR3			size;				// 大きさ
	D3DXCOLOR			col;				// カラー
	BulletType			BulletType;			// 弾の種類
	float				fLength;			// 対角線の長さ
	float				fAngele;			// 対角線の角度
	float				fSpeed;				// 速度
	int					nLife;				// 体力
	int					nCntShot;			// 弾の発射までのカウント
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

D3DXVECTOR3 MovePlayer(void);
void ShotPlayer();
Player *GetPlayer(void);

#endif
