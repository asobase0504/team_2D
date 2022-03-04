//=============================================================================
// 
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
// 
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// *****************************************************************************
// �T�E���h�t�@�C��
// *****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// BGM�^�C�g��
	SOUND_LABEL_BGM_GAME1,			// BGM�Q�[��
	SOUND_LABEL_BGM_GAME2,			// BGM�Q�[��
	SOUND_LABEL_BGM_GAME3,			// BGM�Q�[��
	SOUND_LABEL_BGM_GAME4,			// BGM�Q�[��
	SOUND_LABEL_BGM_GAME5,			// BGM�Q�[��
	SOUND_LABEL_BGM_GAME6,			// BGM�Q�[��
	SOUND_LABEL_BGM_GAME7,			// BGM�Q�[��
	SOUND_LABEL_BGM_RESULT,			// BGM���U���g
	SOUND_LABEL_BGM_RANKING,		// BGM�����L���O
	SOUND_LABEL_BGM_STAFfROLL,		// BGM�X�^�b�t���[��
	SOUND_LABEL_SE_PLAYERDEAD,		// SE�v���C���[�̎��S��
	SOUND_LABEL_SE_SKYBULLET,		// SE�v���C���[�̒e
	SOUND_LABEL_SE_GLOUNDBULLET,	// SE�v���C���[�̒e
	SOUND_LABEL_SE_ENEMYDEAD,		// SE�G�l�~�[�̎��S��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// *****************************************************************************
// �v���g�^�C�v�錾
// *****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
