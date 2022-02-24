#ifndef _enemy_H_
#define _enemy_H_

#include "main.h"

// マクロ定義
#define MAX_Boss	256			// 敵の最大数
#define Boss_WIDTH  (25.0f)		// X	敵サイズ
#define Boss_HEIGHT (25.0f)		// Y	敵サイズ

typedef enum
{
	BossSTATE_NORMAL = 0,	// 通常状態
	BossSTATE_DAMAGE,		// ダメージ判定
	BossSTATE_MAX
}BossSTATE;

typedef enum
{
	TYPEENEMY_SKY_1,		// 空の敵１
	TYPEENEMY_SKY_2,		// 空の敵２
	TYPEENEMY_SKY_3,		// 空の敵３
	TYPEENEMY_GROUND_1,		// 地上の敵1
	TYPEENEMY_GROUND_2,		// 地上の敵2
	TYPEENEMY_MAX
} TYPEENEMY;

// 敵の構造体定義
typedef struct
{
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 pos;		// 位置
	BossSTATE state;		// 状態
	int nCounerState;		// 状態管理カウンター
	int nLife;				// 体力
	int nCntBullet;			// 弾の出る時間
	TYPEENEMY nType;		// 種類
	bool bUse;				// 使用してるかどうか	
	int	nCounter;			// カウントダウン
	int SizeX;				// X軸の大きさ		
	int SizeY;				// Y軸の大きさ

	//---------------
	//ENEMY　MOVE
	bool Buk;		//戻るシステム
	bool flg;		//追尾使用

}enemy;

// プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, TYPEENEMY nType );
void HitEnemy(int nCntBoss, int nDamage);
enemy* GetEnemy(void);


#endif
