#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"
#include "common.h"

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
	ENEMYTYPE_SHEONITE,		// しおないと		
	ENEMYTYPE_BAKYURA,		// 無敵壁
	ENEMYTYPE_GROUND_1,		// 地上の敵1
	ENEMYTYPE_GROUND_2,		// 地上の敵2
	ENEMYTYPE_WARP_1,		// ワープする敵1	(挙動不審)
	ENEMYTYPE_WARP_2,		// ワープする敵2	(挙動不審)
	ENEMYTYPE_WARP_3,		// ワープする敵3	(挙動不審)
	ENEMYTYPE_WARP_4,		// ワープする敵4	(挙動不審)
	ENEMYTYPE_BOSS,			// ボス
	BOSS_PARTS_1,			// BOSS_PARTS1
	BOSS_PARTS_2,			// BOSS_PARTS2
	BOSS_PARTS_3,			// BOSS_PARTS3
	BOSS_PARTS_4,			// BOSS_PARTS4
	BOSS_PARTS_5,			// BOSS_PARTS5
	FLAG_STATE,				// 旗
	ENEMYTYPE_MAX
} ENEMYTYPE;

// 敵の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	ENEMYSTATE state;		// 状態
	ENEMYTYPE nType;		// 種類
	ANIM_TEX AnimTex;		// アニメーションカウント
	float fSpeed;			// 移動量
	float fSize;			// 大きさ		
	bool bUse;				// 使用してるかどうか	
	int nLife;				// 体力
	int nCntState;			// 状態管理カウント
	int nCntBullet;			// 弾の出る時間
	// 挙動管理
	bool bBack;				// 戻るシステム
	bool bTracking;			// 追尾使用
	bool bTP;
	// シオナイト用
	float fSheoRot;			// シオナイト専用角度
	int nSheoCnt;			// シオナイトカウント
	bool bReflect;			// 反射
	bool FlagOn;			//フラグ用
}Enemy;

// プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy* SetEnemy(D3DXVECTOR3 pos, float Size, ENEMYTYPE nType);
void HitEnemy(Enemy* pEnemy, int nDamage);
void LoadSetFile(char *Filename);
void EnemyLynk(char *Filename);		// 
void SetEnemyLynk(int number);		// 
Enemy* GetEnemy(void);
void falseSetEnemy(void);

#endif

