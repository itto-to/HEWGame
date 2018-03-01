//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : 尾形一統
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);

void PrintDebugProc(char *fmt,...);

#endif