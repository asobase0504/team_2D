//**************************************************************************************************
//
// 弾の設定処理
// Auther：唐﨑結斗
//
//**************************************************************************************************
#ifndef _BULLET_H_		//このマクロ定義がされてなかったら
#define _BULLET_H_		//二重インク―ルード防止のマクロ定義

#include"main.h"

//*************************************************************
// マクロ定義
//*************************************************************
#define	MAX_BULLET			(1280)

//*************************************************************
// 弾の種類の列挙型
//*************************************************************
typedef enum
{
	BULLETTYPE_SKY = 0,
	BULLETTYPE_GROUND,
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
	bool				bUse;				// 使用してるかどうか
}Bullet;

//****************************************
// プロトタイプ宣言
//****************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
void SetBulletVtx(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLength, float fAngele);

#endif

