//==================================================
//
// �Q�[������ ( menu.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MENU_H_		//���̃}�N����`������ĂȂ�������
#define _MENU_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	int			nNumUse;		// �g�p��
	float		fLeft;			// ���[
	float		fRight;			// �E�[
	float		fTop;			// ��[
	float		fBottom;		// ���[
	float		fWidth;			// �I�����̕�
	float		fHeight;		// �I�����̍���
}MenuArgument;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
void SetMenu(const MenuArgument &menu);

#endif // !_MENU_H_
