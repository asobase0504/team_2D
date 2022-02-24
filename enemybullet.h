#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

#include "main.h"
//====================================
//弾構造体の定義
//====================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	int nLife;			//寿命
	bool bUse;			//使用しているかどうか
	bool bChase;		//追うかどうか
}EnemyBullet;
//====================================
//プロトタイプ宣言
//====================================
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos);

#endif