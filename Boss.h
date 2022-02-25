#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "enemy.h"

#define COLLISION_B	(25)	//敵サイズ判定
#define JUDGEMENT_B	(25)	//避ける敵の判定サイズ

typedef enum
{
	BossSTATE_NORMAL = 0,	// 通常状態
	BossSTATE_DAMAGE,		// ダメージ判定
	BossSTATE_MAX
}BOSSSTATE;

// 敵の構造体定義
typedef struct
{
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 pos;		// 位置
	int nCounerState;		// 状態管理カウンター
	ENEMYTYPE nType;		// 種類
	bool bUse;				// 使用してるかどうか	
	int	nCounter;			// カウントダウン
	int nLife;				// 体力
	BOSSSTATE state;		// 状態
	Enemy* Enemy[5];
}BOSS;

// プロトタイプ宣言
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 pos/*, TYPEENEMY nType*/);

#endif






