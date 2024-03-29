//================================
//
// スタッフロール画面
// Author:冨所知生
//
//================================
#include "staffroll.h"
#include "input.h"
#include "enemy.h"
#include "map.h"
#include "fade.h"

//マクロ定義
#define NUM_STAFFROLL	(100)					//テクスチャの最大数
#define MAP_SPEED	(0.0005f)					//画面スクロールの速さ
#define MAX_MEMBER	(16)						//エンドロールにのせる最大数

//スタティック変数
static LPDIRECT3DTEXTURE9			s_pTexture[NUM_STAFFROLL] = {};		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;					//頂点バッファへのポインタ;
static D3DXVECTOR3 StaffPos[NUM_STAFFROLL];		//スタッフロールの場所
static float StaffWidth[NUM_STAFFROLL];			//スタッフロールの幅
static float StaffHeight[NUM_STAFFROLL];		//スタッフロールの高さ
static STAFFROLL s_Staffroll[NUM_STAFFROLL];	//スタッフロールの情報の取得
static bool MODE_ROLL;							//エンドロールの経過状況
static int RollNumber;							//現在のエンドロールの番号
static int FinishRoll;							//エンドロールを終了するまでの時間
static bool TYFP;								//ThankyouforPlayingを停止する処理

//============================
// スタッフロールの初期化処理
//============================
void InitStaffroll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//------------------------------
	//	テクスチャの読み込み
	//------------------------------
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\jobilogo.png", &s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\YudaKaitoEnd.png", &s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Map.png", &s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\kitune.png", &s_pTexture[3]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\mizuki000.png", &s_pTexture[4]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Sato Teruto.png", &s_pTexture[5]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\mizuki001.png", &s_pTexture[6]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\buriNameEnemy.png", &s_pTexture[7]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\keisuke.png", &s_pTexture[8]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\hamada.png", &s_pTexture[9]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\buriNameLevel.png", &s_pTexture[10]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\mizuki002.png", &s_pTexture[11]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\StaffRoll_MyName.png", &s_pTexture[12]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\generalsupport.png", &s_pTexture[13]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\koduna.png", &s_pTexture[14]);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\buriya.png", &s_pTexture[15]);

	//これの画像を最後にしてください
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ThankyouforPlaying.png", &s_pTexture[16]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_STAFFROLL,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//------------------------------
	//	構造体の設定
	//------------------------------
	//位置の設定
	StaffPos[0] = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	StaffPos[1] = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT + 100.0f, 0.0f);

	//幅の設定
	StaffWidth[0] = SCREEN_WIDTH * 0.5f;
	StaffWidth[1] = 450.0f;

	//高さの設定
	StaffHeight[0] = SCREEN_HEIGHT * 0.5f;
	StaffHeight[1] = 100.0f;

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++)
	{
		STAFFROLL *staff = s_Staffroll + nCnt;
		staff->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		staff->bUse = false;
		staff->nCntStaff = 0;
	}

	FinishRoll = 0;
	RollNumber = 0;
	MODE_ROLL = false;
	TYFP = false;

	SetStaffroll(StaffPos[0], StaffWidth[0], StaffHeight[0], 0);
}

//============================
// スタッフロールの終了処理
//============================
void UninitStaffroll(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++)
	{
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	//頂点バッファの破壊
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//============================
// スタッフロールの更新処理
//============================
void UpdateStaffroll(void)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	STAFFROLL *staff = s_Staffroll;

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++, staff++, pVtx += 4)
	{
		if (!staff->bUse)
		{
			continue;
		}

		if (TYFP)
		{
			FinishRoll++;

			if (FinishRoll == 180 * MAX_MEMBER)
			{
				ChangeMode(MODE_TITLE);
			}
		}
		else
		{
			staff->nCntStaff++;

			if (MODE_ROLL)
			{//頂点座標の設定

				staff->pos.y -= 2.0f;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth, staff->pos.y - staff->fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth, staff->pos.y - staff->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth, staff->pos.y + staff->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth, staff->pos.y + staff->fHeight, 0.0f);

				if (RollNumber < MAX_MEMBER - 1)
				{
					if (staff->nCntStaff == 240)
					{
						RollNumber++;

						SetStaffroll(StaffPos[1], StaffWidth[1], StaffHeight[1], 1 + RollNumber);
					}
				}
				if (RollNumber == MAX_MEMBER - 1)
				{
					FinishRoll++;

					if (FinishRoll == 250 * MAX_MEMBER)
					{
						TYFP = true;
						FinishRoll = 0;
					}
				}
			}
			else
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth + staff->nCntStaff * 3, staff->pos.y - staff->fHeight + staff->nCntStaff * 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth - staff->nCntStaff * 3, staff->pos.y - staff->fHeight + staff->nCntStaff * 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth + staff->nCntStaff * 3, staff->pos.y + staff->fHeight - staff->nCntStaff * 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth - staff->nCntStaff * 3, staff->pos.y + staff->fHeight - staff->nCntStaff * 2, 0.0f);

				if (staff->nCntStaff == 180)
				{
					staff->nCntStaff = 0;
					staff->bUse = false;
					MODE_ROLL = true;
					SetStaffroll(StaffPos[1], StaffWidth[1], StaffHeight[1], 1);
				}
			}
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//----------------
	//	画面遷移
	//----------------
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A, 0))
	{

		ChangeMode(MODE_TITLE);
	}
}

//============================
// スタッフロールの描画処理
//============================
void DrawStaffroll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++)
	{
		STAFFROLL *staff = s_Staffroll + nCnt;

		if (staff->bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexture[staff->nType]);

			//スタッフロールの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,				//描画する最初の頂点インデックス
				2);						//描画するプリミティブ数
		}
	}
}

//=========================================
//スタッフロールを設置する処理
//=========================================
void SetStaffroll(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_STAFFROLL; nCnt++, pVtx += 4)
	{
		STAFFROLL *staff = s_Staffroll + nCnt;

		if (staff->bUse)
		{
			continue;
		}
		staff->pos = pos;
		staff->nType = nType;
		staff->bUse = true;
		staff->fWidth = fWidth;
		staff->fHeight = fHeight;
		staff->nCntStaff = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth, staff->pos.y - staff->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth, staff->pos.y - staff->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(staff->pos.x - staff->fWidth, staff->pos.y + staff->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(staff->pos.x + staff->fWidth, staff->pos.y + staff->fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = staff->col;
		pVtx[1].col = staff->col;
		pVtx[2].col = staff->col;
		pVtx[3].col = staff->col;

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		break;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}
