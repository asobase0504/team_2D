//==================================================
//
// �Q�[������ ( menu.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MENU_H_		//���̃}�N����`������ĂȂ�������
#define _MENU_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_OPTION		(16)		// �I�����̍ő吔

//--------------------------------------------------
// �\����
//--------------------------------------------------
typedef struct
{
	int						nNumUse;					// �g�p��
	float					fLeft;						// ���[
	float					fRight;						// �E�[
	float					fTop;						// ��[
	float					fBottom;					// ���[
	float					fWidth;						// �I�����̕�
	float					fHeight;					// �I�����̍���
	LPDIRECT3DTEXTURE9		*pTexture[MAX_OPTION];		// �e�N�X�`��
}MenuArgument;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
int SetMenu(const MenuArgument &menu);
void ResetMenu(int nIdx);

#endif // !_MENU_H_
