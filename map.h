//============================
//
// �}�b�v�`�b�v�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _MAP_H_
#define _MAP_H_

#include"main.h"
#define MAP_FILE0	"Data/txt/map.txt"
#define MAP_FILE1	"Data/txt/map1.txt"
#define MAP_FILE2	"Data/txt/map2.txt"
#define MAP_FILE3	"Data/txt/map3.txt"
#define MAP_FILE4	"Data/txt/map4.txt"
#define MAP_FILE5	"Data/txt/map5.txt"
#define MAP_FILE6	"Data/txt/map6.txt"
// �}�N����`
#define NUM_MAP	    (2048)		//�w�i����
#define X_MAP		(22)		//�}�b�v�`�b�v�T�C�YX
#define Y_MAP		(2)			//�}�b�v�`�b�v�T�C�YY
#define MAP_SIZEX	(20)		//�}�b�v�T�C�YX
#define MAP_SIZEY	(100)		//�}�b�v�T�C�YY
#define MAPYLENGTH	(10)		//�}�b�v�̒���
#define BLOCKSIZEX	(((float)SCREEN_WIDTH /MAP_SIZEX))
#define BLOCKSIZEY	(((float)SCREEN_HEIGHT*MAPYLENGTH / MAP_SIZEY))
#define MAPMOVE (2.5f)

//-------------
//�\����
//-------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�J���[
	int nType;			//�e�N�X�`��
	int tex;			//�e�N�X�`���^�C�v
	bool bUse;			//
	char filename[256];
}Map;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos, int nType, int tex);	//�}�b�v�f�[�^�̐ݒ�(�������W�^�C�v�e�N�X�`���̉��Ԗڂ��g����)
void InitMapSet(char *Filename);
void ConversionMap(D3DXVECTOR3 pos, int tex);		//�}�b�v�f�[�^�̃e�N�X�`���̕ύX(�������W,�^�C�v,�e�N�X�`���̉��Ԗڂɂ��邩)
void OutputMap(char *Filename);						//�}�b�v�f�[�^�̃e�N�X�`���̕ύX(�������W�A�e�N�X�`���̉��Ԗڂɂ��邩)
void SizeMap(float SIZ);							//�}�b�v�f�[�^�̃e�N�X�`���̕ύX(�����@�ύX��̃T�C�Y)
Map *GetMap(void);
int GetStage(void);
bool CollisionMap(D3DXVECTOR3 pos);					//�����蔻��
void ConteSet(int stage);							//�R���e
void PasSetMap(char *Filename);						//�}�b�v�p�X���Z�b�g����
int sopiteMap(D3DXVECTOR3 pos);						//�}�b�v�f�[�^�̃e�N�X�`���̕ύX(�����@���W)
void falseSetMap(void);								//���ׂẴ}�b�v�`�b�v���폜����
#endif // !_Map_H_
