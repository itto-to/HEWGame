//=============================================================================
//
// 頂点処理 [vertex.h]
// Author : 
//
//=============================================================================
#ifndef _MESH_H_
#define _MESH_H_

#include "main.h"



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 *vtx, D3DXVECTOR3 *pos, float width, float height);
void DrawMesh(LPDIRECT3DVERTEXBUFFER9 vtx, LPDIRECT3DTEXTURE9 texture, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);

#endif