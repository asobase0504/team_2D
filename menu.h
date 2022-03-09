//==================================================
//
// FPD���� ( menu.h )
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

/*�� ���j���[�̈��� ��*/

typedef struct
{
	LPDIRECT3DTEXTURE9		*pTexture[MAX_OPTION];		// �e�N�X�`��
	int						nNumUse;					// �g�p��
	float					fLeft;						// ���[
	float					fRight;						// �E�[
	float					fTop;						// ��[
	float					fBottom;					// ���[
	float					fWidth;						// �I�����̕�
	float					fHeight;					// �I�����̍���
	bool					bSort;						// ���ו� [ true : �c false : �� ]
}MenuArgument;

/*�� �g�̈��� ��*/

typedef struct
{
	LPDIRECT3DTEXTURE9		*pTexture;		// �e�N�X�`��
	D3DXCOLOR				col;			// �F
	bool					bUse;			// �g�����邩�ǂ��� [ true : ���� false : ����Ȃ� ]
}FrameArgument;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame);
void InitColorOption(void);
void ChangeOption(int Idx);
void DecisionOption(void);
void ResetMenu(int nIdx);

#endif // !_MENU_H_
