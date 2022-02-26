//**************************************************************************************************
//
// 計算処理
// Author：YudaKaito
//
//**************************************************************************************************
#include "common.h"


//--------------------------------------------------
// テクスチャアニメーション
// Author：Hamada Ryuga
//--------------------------------------------------
void SetVtxAnimTex(VERTEX_2D* pVtx, int* nCnt, int nMaxCnt, int* nPatternm, int nMaxPatternm)
{
	*nCnt++;	// アニメーション
	if (*nCnt >= nMaxCnt)
	{
		*nCnt = 0;
		*nPatternm++;

		if (*nPatternm >= nMaxPatternm)
		{
			*nPatternm = 0;
		}

		pVtx[0].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * *nPatternm, 0.0f);			//--------------------------敵が増えたとき増やす
		pVtx[1].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * (*nPatternm + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * *nPatternm, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / (float)(nMaxPatternm)) * (*nPatternm + 1), 1.0f);
	}
}

//--------------------------------------------------
// 頂点カラーの設定
// Author：Yuda Kaito
//--------------------------------------------------
void SetVtxColor(VERTEX_2D* pVtx, D3DXCOLOR* col)
{
	pVtx[0].col = *col;
	pVtx[1].col = *col;
	pVtx[2].col = *col;
	pVtx[3].col = *col;
}

//--------------------------------------------------
// 頂点座標の設定
// Author：Yuda Kaito
//--------------------------------------------------
void SetVtxPos(VERTEX_2D* pVtx, D3DXVECTOR3* pos, float fWidth, float fHeight)
{
	pVtx[0].pos = D3DXVECTOR3(pos->x - fWidth, pos->y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos->x + fWidth, pos->y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos->x - fWidth, pos->y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos->x + fWidth, pos->y + fHeight, 0.0f);
}
