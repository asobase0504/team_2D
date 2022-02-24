//=========================================
//
//	�}�b�v�`�b�v�̃w�b�_�[�t�@�C��
//	Author : �l�c����
//	Author : �y���m��
//
//=========================================
#ifndef _MAP_H_
#define _MAP_H_

#include"main.h"

//=========================================
//�\���̂̐錾
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�J���[
	D3DXVECTOR3 siz;	//�T�C�Y
	int nType;			//�e�N�X�`��
	bool bUse;
}Map;

//=========================================
//�v���g�^�C�v�錾
//=========================================
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos, int nType, int tex);
void InitMapSet(char *Filename);
Map *GetMap(void);
void SetNorotpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);

#endif // !_Map_H_
#pragma once
