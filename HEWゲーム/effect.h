//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);

#endif
