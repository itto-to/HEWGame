//=============================================================================
//
// タイマー処理 [timer.h]
// Author : 
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void EnableTimer(bool bEnable);
void ResetTimer(int nTime = 999);

#endif
