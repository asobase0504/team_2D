#pragma once
#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"

#define	MAX_PARTICLE	(4096)
#define	MAX_RECIPEPARTICLE	(15)

//�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col; //�J���[
	int nLife;			//����
	bool bUse;		//�g�p���Ă邩�ǂ���
	int Trigger;   //���
	float fRadeius;//���a
	int nType;//�^�C�v
	int nMaxLife;//���C�t�̍ő�
	int nCounterAnim;//�J�E���^�[�����Z
	int nPatternAnim;//�J�E���^�[
}Particle;

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadeius, int nLife, int nType);

#endif // !_Particle_H_

