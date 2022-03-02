//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "game.h"
#include "main.h"
#include "bullet.h"
#include "player.h"
#include "score.h"
#include "enemy.h"
#include "input.h"
#include "map.h"
#include "bg.h"
#include "target.h"
#include "sound.h"
#include "fade.h"
#include <stdio.h>

//------------------------------------
// �}�N����`
//------------------------------------

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static bool s_bPause;	// �|�[�Y�����ǂ���

//=========================================
// ������
//=========================================
void InitGame(void)
{
	s_bPause = false;

	InitBG();		// �w�i
	InitMap();		// �}�b�v
	InitEnemy();	// �G
	InitBullet();	// �e
	InitTarget();	// �^�[�Q�b�g
	InitPlayer();	// �v���C���[
	InitScore();	// �X�R�A

	// �}�b�v�̐ݒ�B
	InitMapSet(MAP_FILE0);

	PlaySound(SOUND_LABEL_BGM_GAME1);
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitEnemy();	// �G	
	UninitBG();		// �w�i
	UninitMap();	// �}�b�v
	UninitBullet();	// �e
	UninitPlayer();	// �v���C���[
	UninitScore();	// �X�R�A
	UninitTarget();	// �^�[�Q�b�g
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	// �|�[�Y�̋@�\
	//if (GetJoypadTrigger(JOYKEY_START) || GetKeyboardTrigger(DIK_P))
	//{
	//	s_bPause = !s_bPause;
	//}

	//// �|�[�Y���Ȃ�|�[�Y�ȊO���X�V���Ȃ�
	//if (s_bPause)
	//{
	//	UpdatePause();		// �|�[�Y
	//	return;
	//}

	if (GetKeyboardTrigger(DIK_F1))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME1);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME2);
	}
	else if (GetKeyboardTrigger(DIK_F3))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME3);
	}
	else if (GetKeyboardTrigger(DIK_F4))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME4);
	}
	else if (GetKeyboardTrigger(DIK_F5))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME5);
	}
	else if (GetKeyboardTrigger(DIK_F6))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME6);
	}
	else if (GetKeyboardTrigger(DIK_F7))
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_GAME7);
	}

	if (GetKeyboardTrigger(DIK_1))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_SHEONITE);
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_WARP_2);
	}
	else if (GetKeyboardTrigger(DIK_3))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f),25.0f, ENEMYTYPE_WARP_3);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		AddScore(123);
	}

	if (GetKeyboardTrigger(DIK_T))	// ���U���g�Ɉڍs�f�o�b�O
	{
		SetFade(MODE_RESULT);
	}

	UpdateBG();		// �w�i
	UpdateMap();	// �}�b�v�f�[�^
	UpdateEnemy();	// �G
	UpdateBullet();	// �e
	UpdatePlayer();	// �v���C���[
	UpdateScore();	// �X�R�A
	UpdateTarget();	// �^�[�Q�b�g
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawBG();		// �w�i
	DrawMap();		// �}�b�v�f�[�^
	DrawEnemy();	// �G
	DrawBullet();	// �e
	DrawPlayer();	// �v���C���[
	DrawScore();	// �X�R�A
	DrawTarget();	// �^�[�Q�b�g
}

//=========================================
// �|�[�Y�̐ݒ菈��
//=========================================
void SetEnablePause(bool bUse)
{
	s_bPause = bUse;
}

//=========================================
// �|�[�Y�̏�Ԃ��擾
//=========================================
bool GetPause(void)
{
	return s_bPause;
}
