//==================================================
//
// 3D�Q�[������ ( crater.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _CRATER_H_		//���̃}�N����`������ĂȂ�������
#define _CRATER_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitCrater(void);
void UninitCrater(void);
void UpdateCrater(void);
void DrawCrater(void);
void SetCrater(D3DXVECTOR3 pos);

#endif // !_CRATER_H_