//**************************************************************************************************
//
// ターゲットの設定処理
// Auther：唐﨑結斗
//
//**************************************************************************************************
#ifndef _TARGET_H_		//このマクロ定義がされてなかったら
#define _TARGET_H_		//二重インク―ルード防止のマクロ定義

#include "main.h"
#include "enemy.h"

//*************************************************************
// マクロ定義
//*************************************************************
#define	MAX_TARGET			(1280)

//*************************************************************
// ターゲットの種類の列挙型
//*************************************************************
typedef enum
{
	TARGETSTATE_LOCK_OFF = 0,
	TARGETSTATE_LOCK_ON,
	MAX_TARGETTYPE,
}TargetState;

//************************************************************
// ターゲットテクスチャの名前を定義
//************************************************************
typedef struct
{
	char	aName[128];
}TargetTextureName;

//************************************************************
// ターゲットの構造体を定義
//************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// 中心点
	D3DXVECTOR3			rot;				// 向き
	D3DXVECTOR3			size;				// 大きさ
	D3DXCOLOR			col;				// カラー
	TargetState			TargetType;			// ターゲットの種類
	float				fLength;			// 対角線の長さ
	float				fAngele;			// 対角線の角度
	int					nIdxBullet;			// 弾の識別番号
	bool				bLockOnEnemy;		// ターゲット内に敵がいる
	bool				bUse;				// 使用してるかどうか
}Target;

//****************************************
// プロトタイプ宣言
//****************************************
void InitTarget(void);
void UninitTarget(void);
void UpdateTarget(void);
void DrawTarget(void);
int SetTarget(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetPositionTarget(int nIdxShadow, D3DXVECTOR3 pos);
void LockOnTarget(int nIdxShadow);
void SetTargetVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele);
void CollisionTarget(D3DXVECTOR3 pos, float fLength, ENEMYTYPE TypeEnemy);
Target *GetTarget(void);

#endif


