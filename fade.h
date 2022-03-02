//====================================
// 
// ��ʑJ�ڂ̃w�b�_�[�t�@�C��
// Author Yuda Kaito
// 
//====================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

// �t�F�[�h�̏��
typedef enum
{
	FADE_NONE = 0,	// �������Ă��Ȃ����
	FADE_IN,		// �t�F�C�h�C�����
	FADE_OUT,		// �t�F�[�h�A�E�g���
	FADE_MAX
}FADE;

void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext);
void ResetFade(MODE modeNext);
FADE GetFade(void);


#endif // !_FADE_H_
