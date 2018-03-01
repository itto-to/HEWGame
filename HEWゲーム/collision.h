//=============================================================================
//
// 衝突処理 [collision.h]
// Author : 
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
} BOUNDING_BOX;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBoundingBox(BOUNDING_BOX *box, D3DXVECTOR3 center, float length_x, float length_y, float length_z);
bool IsIntersectedBoundingBox(BOUNDING_BOX box1, BOUNDING_BOX box2);
bool IsIntersectedBoundingBox(D3DXVECTOR3 pos1, BOUNDING_BOX localBox1, D3DXVECTOR3 pos2, BOUNDING_BOX localBox2);
BOUNDING_BOX ToWorldBoundingBox(BOUNDING_BOX localBox, D3DXVECTOR3 posWorld);

// デバッグ用
HRESULT InitDebugBoundingBox(void);
void UninitDebugBoundingBox(void);
void DrawDebugBoundingBox(BOUNDING_BOX box);


#endif