//============================
//
// �}�b�v�`�b�v�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _MAP_H_
#define _MAP_H_

#include"main.h"

#define MAP_FILE0	"data/txt/map.txt"
#define MAP_FILE1	"data/txt/map1.txt"
#define MAP_FILE2	"data/txt/map2.txt"
#define MAP_FILE3	"data/txt/map3.txt"
#define MAP_FILE4	"data/txt/map4.txt"
#define MAP_FILE5	"data/txt/map5.txt"
#define MAP_FILE6	"data/txt/map6.txt"

//-------------
//�\����
//-------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�J���[
	int nType;			//�e�N�X�`��
	int tex;
	bool bUse;
	char filename[256];
}Map;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos, int nType, int tex);
void InitMapSet(char *Filename);
void ConversionMap(D3DXVECTOR3 pos, int tex);
void OutputMap(char *Filename);
void SizeMap(float SIZ);
Map *GetMap(void);
bool CollisionMap(D3DXVECTOR3 pos);
void ConteSet(void);
void PasSetMap(char *Filename);//�}�b�v�p�X���Z�b�g����

#endif // !_Map_H_
