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

	LPDIRECT3DTEXTURE9			number_texture;
	LPDIRECT3DVERTEXBUFFER9		number_buff;



	int rank;

}RESULT;



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
