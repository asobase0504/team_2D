#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

// マクロ定義
#define MAX_ENEMY			(255)

typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// 通常状態
	ENEMYSTATE_DAMAGE,		// ダメージ判定
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{
	ENEMYTYPE_SKY_1,		// 空の敵1
	ENEMYTYPE_SKY_2,		// 空の敵2
	ENEMYTYPE_SKY_3,		// 引き換える敵
	ENEMYTYPE_SHEONITE,		// 空の敵
	ENEMYTYPE_BAKYURA,		// 無敵壁
	ENEMYTYPE_GROUND_1,		// 地上の敵1
	ENEMYTYPE_GROUND_2,		// 地上の敵2
	BOSS_PARTS_1,
	BOSS_PARTS_2,
	BOSS_PARTS_3,
	BOSS_PARTS_4,
	BOSS_PARTS_5,
	ENEMYTYPE_MAX
} ENEMYTYPE;

// 敵の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	ENEMYSTATE state;		// 状態
	ENEMYTYPE nType;		// 種類
	float Size;				// 大きさ		
	bool bUse;				// 使用してるかどうか	
	int nLife;				// 体力
	int nCntState;			// 状態管理カウント
	int nCntBullet;			// 弾の出る時間
	int nCntAnim;			// モーションのカウント
	int nPatternAnim;		// モーションのカウント
	// 挙動管理
	bool bBack;				// 戻るシステム
	bool bTracking;			// 追尾使用
	// シオナイト用
	float fSheoRot;			// シオナイト専用角度
	int nSheoCnt;
}Enemy;

// プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, float Size, ENEMYTYPE nType);
void HitEnemy(Enemy* pEnemy, int nDamage);
Enemy* GetEnemy(void);

#endif
