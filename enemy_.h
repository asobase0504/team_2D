#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

// マクロ定義
#define ENEMY_WIDTH  (25.0f)		// X	敵サイズ
#define ENEMY_HEIGHT (25.0f)		// Y	敵サイズ
#define MAX_ENEMY	(255)	//敵の数

typedef enum
{
	EnemySTATE_NORMAL = 0,	// 通常状態
	EnemySTATE_DAMAGE,		// ダメージ判定
	EnemySTATE_MAX
}ENEMYSTATE;

typedef enum
{
	TYPEENEMY_SKY_1,		// 空の敵１
	TYPEENEMY_SKY_2,		// 空の敵２
	TYPEENEMY_SKY_3,		// 空の敵３

	//--------------------
	//ボス
	BOSS_PARTS_1,
	BOSS_PARTS_2,
	BOSS_PARTS_3,
	BOSS_PARTS_4,
	BOSS_PARTS_5,

	TYPEENEMY_MAX
} TYPEENEMY;

// 敵の構造体定義
typedef struct
{
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 pos;		// 位置
	ENEMYSTATE state;		// 状態
	int nCounerState;		// 状態管理カウンター
	int nLife;				// 体力
	TYPEENEMY nType;		// 種類
	bool bUse;				// 使用してるかどうか	
	int	nCounter;			// カウントダウン
	int SizeX;				// X軸の大きさ		
	int SizeY;				// Y軸の大きさ

	//---------------
	//ENEMY　MOVE
	bool Buk;		//戻るシステム
	bool flg;		//追尾使用

}Enemy;

// プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, TYPEENEMY nType );
void HitEnemy(int nCntBoss, int nDamage);
Enemy* GetEnemy(void);


#endif
