//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : 
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
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
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
RESULT *GetResult(int no);
void RankCheck(int no, int rank);
void InitCount(void);

#endif
