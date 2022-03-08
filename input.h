//=============================================================================
//
// ���͏��� [jobiusinput.h]
// Author1 : KOZUNA HIROHITO
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
	JOYKEY_MAX
}JOYKEY;

//�L�[���(�W���C�p�b�h(DirectInput))
typedef enum
{										//	PS5					�X�C�b�`por				�X�C�b�`�W���C�R���i�������j
	JOYKEY_DIRECT_0_BUTTON = 0,			//���{�^��			//B�{�^��				//A
	JOYKEY_DIRECT_1_BUTTON,				//�~�{�^��			//A�{�^��				//X
	JOYKEY_DIRECT_2_BUTTON,				//�Z�{�^��			//Y�{�^��				//B
	JOYKEY_DIRECT_3_BUTTON,				//���{�^��			//X�{�^��				//Y
	JOYKEY_DIRECT_4_BUTTON,				//L1�{�^��			//L1�{�^��				//SL
	JOYKEY_DIRECT_5_BUTTON,				//R1�{�^��			//R1�{�^��				//SR
	JOYKEY_DIRECT_6_BUTTON,				//L2�{�^��			//L2�{�^��				//
	JOYKEY_DIRECT_7_BUTTON,				//R2�{�^��			//R2�{�^��				//
	JOYKEY_DIRECT_8_BUTTON,				//�N���G�C�g		//-�{�^��				//
	JOYKEY_DIRECT_9_BUTTON,				//�I�v�V����		//+�{�^��				//
	JOYKEY_DIRECT_10_BUTTON,			//L3�{�^��			//L3�{�^��				//
	JOYKEY_DIRECT_11_BUTTON,			//R3�{�^��			//R3�{�^��				//
	JOYKEY_DIRECT_12_BUTTON,			//�z�[���{�^��		//�z�[���{�^��			//
	JOYKEY_DIRECT_13_BUTTON,			//�^�b�`�p�b�h		//�L���v�`���[			//
	JOYKEY_DIRECT_14_BUTTON,			//		//
	JOYKEY_DIRECT_15_BUTTON,			//		//
	JOYKEY_DIRECT_16_BUTTON,			//		//
	JOYKEY_DIRECT_17_BUTTON,			//���X�e�B�b�N		//
	JOYKEY_DIRECT_18_BUTTON,			//�E�X�e�B�b�N		//
	JOYKEY_DIRECT_19_BUTTON,			//�\���L�[�i��j
	JOYKEY_DIRECT_20_BUTTON,			//�\���L�[�i���j
	JOYKEY_DIRECT_21_BUTTON,			//�\���L�[�i���j
	JOYKEY_DIRECT_22_BUTTON,			//�\���L�[�i�E�j
	JOYKEY_DIRECT_MAX
}JOYKEY_DIRECT;

//�L�[���(�W���C�p�b�h(DirectInput))(�\���L�[&8�����X�e�B�b�N)
typedef enum
{
	JOYKEY_CROSS_UP = 0,								//��
	JOYKEY_CROSS_UP_RIGHT = 4500,					//�E��
	JOYKEY_CROSS_RIGHT = 9000,						//�E
	JOYKEY_CROSS_DOWN_RIGHT = 13500,					//�E��
	JOYKEY_CROSS_DOWN = 18000,						//��
	JOYKEY_CROSS_DOWN_LEFT = 22500,					//����
	JOYKEY_CROSS_LEFT = 27000,						//��
	JOYKEY_CROSS_UP_LEFT = 31500,					//����
	JOYKEY_CROSS_MAX
}JOYKEY_CROSS;

//�X�e�B�b�N����(�W���C�p�b�h)
typedef enum
{
	JOYKEY_RIGHT_STICK = 0,
	JOYKEY_LEFT_STICK,
	JOYKEY_RIGHT_LEFT_MAX
}JOYKEY_RIGHT_LEFT;

//----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//----------------------------------------------------------------------------

//���͏����̃v���g�^�C�v�錾

HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);		//���͏����S���̏�����
void UninitInput(void);									//���͏����S���̏I������
void UpdateInput(void);									//���͏����S���̍X�V����

//�v���g�^�C�v�錾�L�[�{�[�h
bool GetKeyboardPress(int nKey);						//�L�[�{�[�h�v���X����
bool GetKeyboardTrigger(int nKey);						//�L�[�{�[�h�g���K�[����
bool GetKeyboardRelease(int nKey);						//�L�[�{�[�h�����[�X����
bool GetKeyboardAllPress(void);							//�L�[�{�[�h�S�L�[�v���X����
bool GetKeyboardAllTrigger(void);						//�L�[�{�[�h�S�L�[�g���K�[����

//�v���g�^�C�v�錾�W���C�p�b�g(DirectInput)
bool GetDirectJoypadPress(JOYKEY_DIRECT Key);												//�W���C�p�b�h�v���X����
bool GetDirectJoypadPress(JOYKEY_CROSS Key);												//�W���C�p�b�h�v���X����
bool GetDirectJoypadTrigger(JOYKEY_DIRECT Key);												//�W���C�p�b�h�g���K�[����
bool GetDirectJoypadTrigger(JOYKEY_CROSS Key);												//�W���C�p�b�h�g���K�[����
bool GetDirectJoypadRelease(JOYKEY_DIRECT Key);												//�W���C�p�b�h�����[�X����
bool GetDirectJoypadAllPress(void);															//�W���C�p�b�h�S�L�[�v���X����
D3DXVECTOR3 GetDirectJoypadStick(JOYKEY_RIGHT_LEFT Key);									//�W���C�p�b�h�X�e�B�b�N����
bool GetDirectJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);				//�W���C�p�b�h�X�e�B�b�N8�����v���X����
bool GetDirectJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key);			//�W���C�p�b�h�X�e�B�b�N8�����g���K�[����

//�v���g�^�C�v�錾�W���C�p�b�g
bool GetJoypadPress(JOYKEY Key, int nPlayer);															//�W���C�p�b�h�v���X����
bool GetJoypadTrigger(JOYKEY Key, int nPlayer);															//�W���C�p�b�h�g���K�[����
D3DXVECTOR3 GetJoypadStick(JOYKEY_RIGHT_LEFT RightLeft, int nPlayer);									//�W���C�p�b�h�X�e�B�b�N����
int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);														//�W���C�p�b�h�g���K�[�y�_������
bool GetJoypadStickPress(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer);					//�W���C�p�b�h�X�e�B�b�N�v���X�W����
bool GetJoypadStickTrigger(JOYKEY_RIGHT_LEFT RightLeft, JOYKEY_CROSS Key, int nPlayer);					//�W���C�p�b�h�X�e�B�b�N�g���K�[�W����
#endif