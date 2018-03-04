//=============================================================================
//
// 背景処理 [background.cpp]
// Author : 
//
//=============================================================================
#include "background.h"

#include "debugproc.h"
#include "mesh.h"
#include "stage.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_PATH			"data/TEXTURE/"
#define TEXTURE_BG_PLAIN		"bg_plain.png"
#define TEXTURE_BG_DESERT		"bg_desert.jpg"
#define TEXTURE_BG_VOLCANO		"bg_volcano.jpg"
#define TEXTURE_BG_SNOW			"bg_snow.jpg"



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
LPDIRECT3DTEXTURE9 g_bg_texture[STAGE_MAX];		// テクスチャのポインタ


HRESULT InitBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_background.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);

	// 頂点作成
	MakeVertex(pDevice, &g_background.vtx, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);

	// テクスチャ作成
	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++)
	{
		char texture_filename[MAX_PATH] = TEXTURE_PATH;

		switch ((STAGE_TYPE)stage_no)
		{
		case STAGE_PLAIN:
			strcat(texture_filename, TEXTURE_BG_PLAIN);
			break;
		case STAGE_DESERT:
			strcat(texture_filename, TEXTURE_BG_DESERT);
			break;
		case STAGE_VOLCANO:
			strcat(texture_filename, TEXTURE_BG_VOLCANO);
			break;
		case STAGE_SNOW:
			strcat(texture_filename, TEXTURE_BG_SNOW);
			break;
		}

		if (FAILED(D3DXCreateTextureFromFile(pDevice, texture_filename, &g_bg_texture[stage_no])))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void UninitBackground()
{
	SAFE_RELEASE(g_background.vtx);

	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++)
	{
		SAFE_RELEASE(g_bg_texture[stage_no]);
	}
}


void UpdateBackground()
{
	STAGE_TYPE stage = GetStage();
	g_background.texture = g_bg_texture[stage];

#ifdef _DEBUG
	PrintDebugProc("***背景***\n");
	PrintDebugProc("座標： X %f Y%f Z%f\n", g_background.pos.x, g_background.pos.y, g_background.pos.z);
#endif
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
}
