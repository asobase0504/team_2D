//==================================================
//
// 3D�Q�[������ ( fade.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _FADE_H_		//���̃}�N����`������ĂȂ�������
#define _FADE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
// �񋓌^
//--------------------------------------------------
typedef enum
{
	FADE_NONE = 0,		// �������Ă��Ȃ����
	FADE_OUT,			// �t�F�[�h�A�E�g(�����Ȃ�)
	FADE_IN,			// �t�F�[�h�C��(������)
	FADE_MAX
}FADE;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void StartFadeOut(void);
FADE GetFade(void);

#endif // !_FADE_H_
