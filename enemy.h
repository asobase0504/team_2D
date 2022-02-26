#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

// マクロ定義
#define MAX_Boss			(256)	// 敵の最大数
#define ENEMY_WIDTH			(25.0f)	// X	敵サイズ
#define ENEMY_HEIGHT		(25.0f)	// Y	敵サイズ
#define MAX_ENEMY			(255)
#define NUM_ENEMY			(2)		// 敵の種類

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
	ENEMYTYPE_WARP_1,		// ワープする敵1
	ENEMYTYPE_WARP_2,		// ワープする敵2
	ENEMYTYPE_WARP_3,		// ワープする敵3
	ENEMYTYPE_WARP_4,		// ワープする敵4
	BOSS_PARTS_1,			// BOSS_PARTS1
	BOSS_PARTS_2,			// BOSS_PARTS2
	BOSS_PARTS_3,			// BOSS_PARTS3
	BOSS_PARTS_4,			// BOSS_PARTS4
	BOSS_PARTS_5,			// BOSS_PARTS5
	ENEMYTYPE_MAX
} ENEMYTYPE;


// 敵の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	ENEMYSTATE state;		// 状態
	ENEMYTYPE nType;		// 種類
	bool bUse;				// 使用してるかどうか	
	int nCounterState;		// 状態管理カウンター
	int nLife;				// 体力
	int nCntBullet;			// 弾の出る時間
	int	nCounter;			// カウントダウン
	float SizeX;			// X軸の大きさ		
	float SizeY;			// Y軸の大きさ
	int nCounterAnim;		// モーションのカウント
	int nPatternAnim;		// モーションのカウント

	// エネミーの属性
	bool Buk;		// 戻るシステム
	bool flg;		// 追尾使用
	bool bTP;		// TP機能
}Enemy;

// プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nType );
void HitEnemy(Enemy* pEnemy, int nDamage);
Enemy* GetEnemy(void);

#endif
