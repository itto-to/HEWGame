//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : 
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9			resultrank_texture;
	LPDIRECT3DVERTEXBUFFER9		resultrank_buff;

	int rank;

}RESULT;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9	buff = NULL;
	LPDIRECT3DTEXTURE9		texture = NULL;
	LPDIRECT3DVERTEXBUFFER9	player_buff = NULL;
	LPDIRECT3DTEXTURE9		player_texture = NULL;
}RESULTNO;






//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
RESULT *GetResult(int no);
void RankCheck(int no, int rank);
void InitCount(void);

#endif
