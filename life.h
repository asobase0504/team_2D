//---------------------------------------------------------
//
//���C�t
//Author:takanooooooooooooooooooooooo
//
//---------------------------------------------------------
#ifndef _LIFE_H_
#define _LIFE_H_

#include"main.h"

//-----------------
//�}�N����`
//-----------------
#define NUM_LIFE			(1)
#define LIFE_SIZE			(20)						//���C�t�̃T�C�Y

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
void SetLife(void);
//void AddLife(int nAddlife,D3DXVECTOR3 pos);
void HitLife(void);

#endif // !_LIFE_H_