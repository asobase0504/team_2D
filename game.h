//=========================================
// 
// �Q�[��������(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _GAME_H_
#define _GAME_H_

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitGame(void);	// ����������
void UninitGame(void);	// �I������
void UpdateGame(void);	// �X�V����
void DrawGame();	// �`�揈��
void SetEnablePause(bool bUse);	// �|�[�Y�̏�Ԃ�ݒ肷��
bool GetPause(void);			// �|�[�Y�̏�Ԏ擾

#endif // !_COMMON_H_
