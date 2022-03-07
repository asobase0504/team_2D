//===================
//パーティクル
//autho hamadaryuuga
//===================
#include"particle.h"
#include"common.h"
#include"player.h"

#define NUR_PARTICLE (4)

//スタティック
static LPDIRECT3DTEXTURE9 s_pTextureParticle[NUR_PARTICLE] = {}; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffParticle = NULL; //頂点バッファの設定
static Particle s_aParticle[MAX_PARTICLE];

//==================================
//初期化
//==================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	int nCntParticle;
	
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/effect000.jpg",
		&s_pTextureParticle[0]);


	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//初期化忘れない
		s_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		s_aParticle[nCntParticle].nLife = 0;
		s_aParticle[nCntParticle].bUse = false;
		s_aParticle[nCntParticle].fRadeius = 0;
		s_aParticle[nCntParticle].nPatternAnim = 0;
		s_aParticle[nCntParticle].nCounterAnim = 0;
	}

	//頂点バッファ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//ここ頂点フォーマット
		&s_pVtxBuffParticle,
		NULL);

	VERTEX_2D*pVtx; //頂点へのポインタ
	s_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点バッファをロックし頂点情報へのポインタを取得
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		////頂点座標


		SetNormalpos2d(pVtx, s_aParticle[nCntParticle].pos.x - s_aParticle[nCntParticle].fRadeius,
				s_aParticle[nCntParticle].pos.x + s_aParticle[nCntParticle].fRadeius,
				s_aParticle[nCntParticle].pos.y - s_aParticle[nCntParticle].fRadeius,
				s_aParticle[nCntParticle].pos.y + s_aParticle[nCntParticle].fRadeius);

		//RHWの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = s_aParticle[nCntParticle].col;
		pVtx[1].col = s_aParticle[nCntParticle].col;
		pVtx[2].col = s_aParticle[nCntParticle].col;
		pVtx[3].col = s_aParticle[nCntParticle].col;

		//テクスチャの座標設定
		Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4; //頂点ポイントを四つ進む
	}

	//頂点バッファをアンロック
	s_pVtxBuffParticle->Unlock();
}
void UninitParticle(void)
{
	for (int cut = 0; cut < NUR_PARTICLE; cut++)
	{
		if (s_pTextureParticle[cut] != NULL)
		{					  
			s_pTextureParticle[cut]->Release();
			s_pTextureParticle[cut] = NULL;
		}
	}
	

	//頂点バッファの破棄
	if (s_pVtxBuffParticle != NULL)
	{
		s_pVtxBuffParticle->Release();
		s_pVtxBuffParticle = NULL;
	}
}
void UpdateParticle(void)
{
	int nCntParticle;
	VERTEX_2D*pVtx; //頂点へのポインタ
					//頂点バッファをアンロック
	s_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (s_aParticle[nCntParticle].bUse)
		{
			Player* pPlayer = GetPlayer();

			
			s_aParticle[nCntParticle].pos += s_aParticle[nCntParticle].move;
			s_aParticle[nCntParticle].pos.x += pPlayer->move.x/2;
			if (pPlayer->pos.y > s_aParticle[nCntParticle].pos.y)
			{
				s_aParticle[nCntParticle].bUse = false;
			}
		
			SetNormalpos2d(pVtx, s_aParticle[nCntParticle].pos.x - s_aParticle[nCntParticle].fRadeius,
						s_aParticle[nCntParticle].pos.x + s_aParticle[nCntParticle].fRadeius,
						s_aParticle[nCntParticle].pos.y - s_aParticle[nCntParticle].fRadeius,
						s_aParticle[nCntParticle].pos.y + s_aParticle[nCntParticle].fRadeius);

			s_aParticle[nCntParticle].col.a -= 1.0f / s_aParticle[nCntParticle].nMaxLife;
			//頂点カラーの設定
			pVtx[0].col = s_aParticle[nCntParticle].col;
			pVtx[1].col = s_aParticle[nCntParticle].col;
			pVtx[2].col = s_aParticle[nCntParticle].col;
			pVtx[3].col = s_aParticle[nCntParticle].col;

		
			
		
		}
		pVtx += 4;
	}
	
	//頂点バッファをアンロック
	s_pVtxBuffParticle->Unlock();
}
void DrawParticle(void)
{
	int nCntParticle;

	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ

	pDevice = GetDevice(); //代入

	 //頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	//テクスチャの描画
	//頂点バッファをロックし頂点情報へのポインタを取得
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (s_aParticle[nCntParticle].bUse)
		{
		
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTextureParticle[0]);
			
			//ポリゴンの描画 四角
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntParticle * 4,			//描画する最初の頂点インデックス
				2);
		}//プリミティブ(ポリゴン)数
	}
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadeius, int nLife , int nType)
{
	int nCntParticle;
	VERTEX_2D*pVtx; //頂点へのポインタ
	s_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!s_aParticle[nCntParticle].bUse)
		{
			//敵が使用されてない場合
			s_aParticle[nCntParticle].fRadeius = fRadeius;
			s_aParticle[nCntParticle].pos.x = pos.x;
			s_aParticle[nCntParticle].pos.y = pos.y;
			s_aParticle[nCntParticle].pos.z = pos.z;

			s_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
			s_aParticle[nCntParticle].move.y = 3.0f;//sinf((float)(rand() % 629 - 314) / 100.0f + 100.0f);

			//頂点座標
			SetNormalpos2d(pVtx, s_aParticle[nCntParticle].pos.x - fRadeius,
				s_aParticle[nCntParticle].pos.x + fRadeius, 
				s_aParticle[nCntParticle].pos.y - fRadeius,
				s_aParticle[nCntParticle].pos.y + fRadeius);
			
			s_aParticle[nCntParticle].nType = nType;
			s_aParticle[nCntParticle].nLife = nLife;
			s_aParticle[nCntParticle].bUse = true;
			s_aParticle[nCntParticle].col = col;
			s_aParticle[nCntParticle].nMaxLife = nLife;
			
			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	s_pVtxBuffParticle->Unlock();

}