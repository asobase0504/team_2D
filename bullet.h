//**************************************************************************************************
//
// 弾の設定処理
// Author：唐�ｱ結斗
//
//**************************************************************************************************
#ifndef _BULLET_H_		//このマクロ定義がされてなかったら
#define _BULLET_H_		//二重インク―ルード防止のマクロ定義

#include"main.h"

//*************************************************************
// マクロ定義
//*************************************************************
#define	MAX_BULLET			(1280)
#define MAX_BULLET_TYPE		(128)

//*************************************************************
// 弾の種類の列挙型
//*************************************************************
typedef enum
{
	BULLETTYPE_PLAYER_SKY = 0,
	BULLETTYPE_PLAYER_GROUND,
	BULLETTYPE_ENEMY,
	MAX_BULLETTYPE,
}BulletType;

//************************************************************
// ポリゴンテクスチャの名前を定義
//************************************************************
typedef struct
{
	char	aName[128];
}BulletTextureName;

//************************************************************
// ポリゴンの構造体を定義
//************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// 中心点
	D3DXVECTOR3			rot;				// 向き
	D3DXVECTOR3			size;				// 大きさ
	D3DXVECTOR3			move;				// 移動ベクトル
	D3DXCOLOR			col;				// カラー
	BulletType			BulletType;			// 弾の種類
	float				fLength;			// 対角線の長さ
	float				fAngele;			// 対角線の角度
	float				fRotMove;			// 移動方向の角度
	float				fSpeed;				// 移動速度
	int					nLife;				// 寿命
	int					nIdxTarget;			// ターゲットの識別番号
	bool				bOnCollision;		// 当たり判定の有無
	bool				bUse;				// 使用してるかどうか
}Bullet;

//****************************************
// プロトタイプ宣言
//****************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdxTarget, bool bOnCollision);
void SetBulletVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele);
Bullet* GetBullet(void);
void DeleteBullet(void);
#endif

