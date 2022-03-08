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
#include "bestscore.h"
#include "scoreup.h"
#include "player.h"
#include "score.h"
#include "Boss.h"
#include "life.h"
#include "enemy.h"
#include "input.h"
#include "map.h"
#include "menu.h"
#include "pause.h"
#include "target.h"
#include "scoreup.h"
#include "crater.h"
#include "sound.h"
#include "fade.h"
#include <stdio.h>
#include "particle.h"

//------------------------------------
// �}�N����`
//------------------------------------

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static bool s_bPause;	// �|�[�Y�����ǂ���
static int s_nMap = 0;
//=========================================
// ������
//=========================================
void InitGame(void)
{
	s_nMap = 0;
	s_bPause = false;
	InitMap();			// �}�b�v
	InitBoss();			// �{�X
	InitEnemy();		// �G
	InitBullet();		// �e
	InitTarget();		// �^�[�Q�b�g
	InitLife();			// ���C�t
	InitPlayer();		// �v���C���[
	InitScore();		// �X�R�A
	InitBestScore();	// �x�X�g�X�R�A
	InitScoreUp();		// �X�R�A�A�b�v
//	InitFlag();			// �t���O
	InitCrater();		// �N���[�^�[
	InitMenu();			// ���j���[
	InitPause();		// �|�[�Y
	InitParticle();
	// �}�b�v�̐ݒ�B
	ConteSet(s_nMap);

	PlaySound(SOUND_LABEL_BGM_GAME1);
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{

	UninitMap();		// �}�b�v
	UninitBoss();		// �{�X
	UninitEnemy();		// �G	
	UninitBullet();		// �e
	UninitTarget();		// �^�[�Q�b�g
	UninitLife();		// ���C�t
	UninitPlayer();		// �v���C���[
	UninitScore();		// �X�R�A
	UninitBestScore();	// �x�X�g�X�R�A
	UninitScoreUp();	// �X�R�A�A�b�v
//	UninitFlag();		// �t���O
	UninitCrater();		// �N���[�^�[
	UninitMenu();		// ���j���[
	UninitPause();		// �|�[�Y
	UninitParticle();	// �p�[�e�B�N��
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	if (GetDirectJoypadTrigger(JOYKEY_DIRECT_7_BUTTON) || GetKeyboardTrigger(DIK_P))
	{
		s_bPause = !s_bPause;

		if (s_bPause)
		{// �|�[�Y���Ă�
			SetPause();		// ���j���[�̐ݒ�
		}
	}

	// �|�[�Y���Ȃ�|�[�Y�ȊO���X�V���Ȃ�
	if (s_bPause)
	{
		UpdateMenu();		// ���j���[
		UpdatePause();		// �|�[�Y
		return;
	}

#ifdef _DEBUG
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
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_SKY_1);
	}
	else if (GetKeyboardTrigger(DIK_2))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_SHEONITE);
	}
	else if (GetKeyboardTrigger(DIK_3))
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), 25.0f, ENEMYTYPE_WARP_3);
	}

	if (GetKeyboardTrigger(DIK_T))	// ���U���g�Ɉڍs�f�o�b�O
	{
		ChangeMode(MODE_RESULT);
	}
#endif // !_DEBUG

	UpdateMap();		// �}�b�v�f�[�^
	UpdateCrater();		// �N���[�^�[
	UpdateBoss();		// �{�X
	UpdateEnemy();		// �G
	UpdateBullet();		// �e
	UpdateParticle();	// �p�[�e�B�N��
	UpdatePlayer();		// �v���C���[
	UpdateScore();		// �X�R�A
	UpdateTarget();		// �^�[�Q�b�g
	UpdateLife();		// ���C�t
	UpdateBestScore();	// �x�X�g�X�R�A
	UpdateScoreUp();	// �X�R�A�A�b�v
	//UpdateFlag();		// �t���O
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	DrawMap();			// �}�b�v�f�[�^
	DrawCrater();		// �N���[�^�[
	DrawBoss();			// �{�X
	DrawEnemy();		// �G
	DrawBullet();		// �e
	DrawParticle();		// �p�[�e�B�N��
	DrawPlayer();		// �v���C���[
	DrawScore();		// �X�R�A
	DrawTarget();		// �^�[�Q�b�g
	DrawLife();			// ���C�t
	DrawBestScore();	// �x�X�g�X�R�A
	DrawScoreUp();		// �X�R�A�A�b�v
	//DrawFlag();			// �t���O
	if (s_bPause)
	{
		DrawPause();		// �|�[�Y
		DrawMenu();			// ���j���[
	}
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
