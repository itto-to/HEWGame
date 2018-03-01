//=============================================================================
//
// 背景処理 [background.cpp]
// Author : 
//
//=============================================================================
#include "background.h"

#include "debugproc.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BG_PLAIN		"data/TEXTURE/bg_plain.png"


//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	LPDIRECT3DVERTEXBUFFER9 vtx;	// 頂点バッファ
	LPDIRECT3DTEXTURE9 texture;		// テクスチャのポインタ
	D3DXVECTOR3 pos;				// 座標
} BACKGROUND;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
BACKGROUND g_background;


HRESULT InitBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_background.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);

	// 頂点作成
	MakeVertex(pDevice, &g_background.vtx, SCREEN_WIDTH, SCREEN_HEIGHT);

	// テクスチャ作成
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_BG_PLAIN, &g_background.texture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void UninitBackground()
{
	SAFE_RELEASE(g_background.vtx);
	SAFE_RELEASE(g_background.texture);
}


void UpdateBackground()
{
	PrintDebugProc("***背景***\n");
	PrintDebugProc("座標： X %f Y%f Z%f\n", g_background.pos.x, g_background.pos.y, g_background.pos.z);
}

void DrawBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_background.vtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0, 0, 0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_background.pos.x, g_background.pos.y, g_background.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, g_background.texture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	
	//DrawMesh(g_background.vtx, g_background.texture, g_background.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));


	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXMATRIX mtxWorld, mtxRot, mtxTranslate;

	//// 頂点バッファをデバイスのデータストリームにバインド
	//pDevice->SetStreamSource(0, g_background.vtx, 0, sizeof(VERTEX_3D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_3D);

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&mtxWorld);

	//// 移動を反映
	//D3DXMatrixTranslation(&mtxTranslate, g_background.pos.x, g_background.pos.y, g_background.pos.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_background.texture);

	//// 描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
