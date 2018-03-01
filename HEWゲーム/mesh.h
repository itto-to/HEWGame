//=============================================================================
//
// ���_���� [vertex.h]
// Author : 
//
//=============================================================================
#ifndef _MESH_H_
#define _MESH_H_

#include "main.h"



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 *vtx, float width, float height);
void DrawMesh(LPDIRECT3DVERTEXBUFFER9 vtx, LPDIRECT3DTEXTURE9 texture, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);

#endif