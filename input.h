//=============================================================================
//
// ���͏��� [input.h]
// Author1 : KOZUNA HIROHITO
// Author2 : ISOE JUKIA
//
//	���Ō�̍s�����邱�Ƃ𐄏���
//
//=============================================================================

#ifndef _INPUT_H_		//���̃}�N����`������Ȃ�������
#define _INPUT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//----------------------------------------------------------------------------
//���C�u�����[�t�@�C���̃����N
//----------------------------------------------------------------------------

#pragma comment(lib,"dinput8.lib")//���͏����ɕK�v(�L�[�{�[�h)
#pragma comment(lib,"xinput.lib")//���͏����ɕK�v�i�W���C�p�b�g�j

//----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//----------------------------------------------------------------------------

#include "main.h"

//----------------------------------------------------------------------------
//�񋓌^
//----------------------------------------------------------------------------

//�L�[���(�W���C�p�b�h)
typedef enum
{
	JOYKEY_UP = 0,				//�\���{�^����
	JOYKEY_DOWN,				//�\���{�^����
	JOYKEY_LEFT,				//�\���{�^����
	JOYKEY_RIGHT,				//�\���{�^���E
	JOYKEY_START,				//�X�^�[�g�{�^��
	JOYKEY_BACK,				//�o�b�N�{�^��
	JOYKEY_LEFT_THUMB,			//���X�e�B�b�N������
	JOYKEY_RIGHT_THUMB,			//�E�X�e�B�b�N������
	JOYKEY_LEFT_SHOULDER,		//L�P�{�^��
	JOYKEY_RIGHT_SHOULDER,		//R�P�{�^��
	JOYKEY_LEFT_TRIGGER,		//L�Q�g���K�[
	JOYKEY_RIGHT_TRIGGER,		//R�Q�g���K�[
	JOYKEY_A,					//A�{�^��
	JOYKEY_B,					//B�{�^��
	JOYKEY_X,					//X�{�^��
	JOYKEY_Y,					//Y�{�^��
	JOYKEY_LEFT_STICK,			//���X�e�B�b�N
	JOYKEY_RIGHT_STICK,			//�E�X�e�B�b�N
	JOYKEY_MAX
}JOYKEY;

//�L�[���(�}�E�X)
typedef enum
{
	MOUSE_INPUT_LEFT,			//�}�E�X���{�^��
	MOUSE_INPUT_RIGHT,			//�}�E�X�E�{�^��
	MOUSE_INPUT_MIDDLE,			//�}�E�X�����{�^��
	MOUSE_INPUT_4,				//�}�E�X�{�^��4
	MOUSE_INPUT_5,				//�}�E�X�{�^��5
	MOUSE_INPUT_6,				//�}�E�X�{�^��6
	MOUSE_INPUT_7,				//�}�E�X�{�^��7
	MOUSE_INPUT_8,				//�}�E�X�{�^��8
}MOUSE;

//----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//----------------------------------------------------------------------------

//���͏����̃v���g�^�C�v�錾

HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);		//���͏����S���̏�����
void UninitInput(void);									//���͏����S���̏I������
void UpdateInput(void);									//���͏����S���̍X�V����

														//�v���g�^�C�v�錾�L�[�{�[�h
bool GetKeyboardPress(int nKey);			//�L�[�{�[�h�v���X����
bool GetKeyboardTrigger(int nKey);			//�L�[�{�[�h�g���K�[����
bool GetKeyboardAllPress(void);				//�L�[�{�[�h�S�L�[�v���X����
bool GetKeyboardAllTrigger(void);			//�L�[�{�[�h�S�L�[�g���K�[����

											//�v���g�^�C�v�錾�W���C�p�b�g
bool GetJoypadPress(JOYKEY Key, int nPlayer);					//�W���C�p�b�h�v���X����
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//�W���C�p�b�h�g���K�[����
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			//�W���C�p�b�h�X�e�B�b�N����
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//�W���C�p�b�h�g���K�[�y�_������
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//�W���C�p�b�h�U������

																//�v���g�^�C�v�錾�}�E�X
bool GetMousePress(MOUSE mouse);			//�}�E�X�̃v���X����
bool GetMouseTrigger(MOUSE mouse);			//�}�E�X�̃g���K�[����
D3DXVECTOR3 GetMouse(void);					//�}�E�X�̉�ʓ��̈ʒu
int GetMouseWheel(void);					//�}�E�X�̃z�C�[���̓�������

#endif

/*************************************************************************************************************

//main.h�ɒǉ���

//�C���N���[�h
#include "dinput.h"						//���͏����ɕK�v
#include "Xinput.h"						//�W���C�p�b�g����

//main.cpp�ɒǉ���

//�C���N���[�h
#include "input.h"

//�����������Ɉȉ��̃R�[�h�����

//���͏����̏���������
if (FAILED(InitInput(hInstance, hWnd)))
{
return E_FAIL;
}

//�I�������Ɉȉ��̃R�[�h�����

//���͏����̏I������
UninitInput();

//�X�V�����Ɉȉ��̃R�[�h�����

//���͏����̍X�V����
UpdateInput();

//�e�֐��̐���

//--------------------------------------------------------------------------------------------------------
//�v���g�^�C�v�錾�L�[�{�[�h
//--------------------------------------------------------------------------------------------------------

{//�����ɃL�[�̃}�N������ꂻ�ꂪ�����ꂽ���ǂ�����bool�ŕԂ�
bool GetKeyboardPress(int nKey);			//�L�[�{�[�h�v���X����
bool GetKeyboardTrigger(int nKey);			//�L�[�{�[�h�g���K�[����
bool GetKeyboardAllPress(void);				//�L�[�{�[�h�S�L�[�v���X����
bool GetKeyboardAllTrigger(void);			//�L�[�{�[�h�S�L�[�g���K�[����
}

//--------------------------------------------------------------------------------------------------------
//�v���g�^�C�v�錾�W���C�p�b�g
//--------------------------------------------------------------------------------------------------------

{//�����ɃL�[�̗񋓌^����ꂻ�ꂪ�����ꂽ���ǂ�����bool�ŕԂ�
bool GetJoypadPress(JOYKEY Key, int nPlayer);					//�W���C�p�b�h�v���X����
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//�W���C�p�b�h�g���K�[����
}

//�Ԃ��l�@�X�e�B�b�N��|���Γ|���ق�(1.0f or -1.0f)�ɋ߂�
//�X�e�B�b�N�̗V�т�if�̏������ɑg��
//��@if(GetJoypadStick(JOYKEY_RIGHT_STICK,0)  > 0.5f)
//    {//�E�ɔ�����葽���|������

{�������̓L�[�i�X�e�B�b�N�j,�������̓v���C���[�ԍ�
D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);			//�W���C�p�b�h�X�e�B�b�N����
}

//�Ԃ��l�@�y�_�����ǂꂾ���������܂ꂽ����int�ŕԂ�
//�y�_���̗V�т�if�̏������ɑg��
//��@if(GetJoypadTriggerPedal(JOYKEY_RIGHT_TRIGGER,0) > 100))
//    {//�E�y�_����100��葽���|������

{�������̓L�[�i�X�e�B�b�N�j,�������̓v���C���[�ԍ�
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//�W���C�p�b�h�g���K�[�y�_������
}

{�������͗h�炷����, �������͂ǂꂭ�炢�̋����ŗh�炷��, ��O�����̓v���C���[�ԍ�
void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//�W���C�p�b�h�U������
}

//--------------------------------------------------------------------------------------------------------
//�v���g�^�C�v�錾�}�E�X
//--------------------------------------------------------------------------------------------------------

{//�����ɃL�[�̗񋓌^����ꂻ�ꂪ�����ꂽ���ǂ�����bool�ŕԂ�
bool GetMousePress(MOUSE mouse);			//�}�E�X�̃v���X����
bool GetMouseTrigger(MOUSE mouse);			//�}�E�X�̃g���K�[����
}

//�Ԃ��l�@�}�E�X��pos��Ԃ��l
{
D3DXVECTOR3 GetMouse(void);					//�}�E�X�̉�ʓ��̈ʒu
}

{//���������A�Ԃ��l�͉��ɉ񂷂Ɓ{�ŕԂ��O�ɉ񂷂Ɓ[�ŕԂ��Ă��܂�
int GetMouseWheel(void);					//�}�E�X�̃z�C�[���̓�������
}
************************************************************************************************************/


