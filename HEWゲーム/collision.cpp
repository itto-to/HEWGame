//=============================================================================
//
// 衝突処理 [collision.cpp]
// Author : 
//
//=============================================================================
#include "collision.h"

#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BOUNDING_BOX	"data/TEXTURE/bounding_box.png"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawDebugBoundingBox(BOUNDING_BOX box);
D3DXVECTOR3 BoundingBoxCenter(BOUNDING_BOX box);
float BoundingBoxSizeX(BOUNDING_BOX box);
float BoundingBoxSizeY(BOUNDING_BOX box);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_vtxBoundingBox;	// デバッグ用バウンディングボックス頂点バッファ
LPDIRECT3DTEXTURE9 g_textureBoundingBox;	// デバッグ用バウンディングボックステクスチャへのポインタ

// バウンディングボックス初期化
void InitBoundingBox(BOUNDING_BOX *box, D3DXVECTOR3 center, float length_x, float length_y, float length_z)
{
	D3DXVECTOR3 half_length = D3DXVECTOR3(length_x, length_y, length_z) / 2.0f;
	box->min = center - half_length;
	box->max = center + half_length;
}


HRESULT InitDebugBoundingBox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// バウンディングボックス描画用頂点作成
	MakeVertex(pDevice, &g_vtxBoundingBox, 1.0f, 1.0f);

	// バウンディングボックス描画用テクスチャ読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_BOUNDING_BOX, &g_textureBoundingBox))) {
		return E_FAIL;
	}

	return S_OK;
}

void UninitDebugBoundingBox(void)
{
	SAFE_RELEASE(g_vtxBoundingBox);
	SAFE_RELEASE(g_textureBoundingBox);
}

// オブジェクトのワールド座標とローカルバウンディングボックスからワールドバウンディングボックスを返す
BOUNDING_BOX ToWorldBoundingBox(BOUNDING_BOX localBox, D3DXVECTOR3 posWorld)
{
	BOUNDING_BOX worldBox;
	worldBox.max = posWorld + localBox.max;
	worldBox.min = posWorld + localBox.min;

	return worldBox;
}

// バウンディングボックスが衝突していたらtrueを返す
bool IsIntersectedBoundingBox(BOUNDING_BOX box1, BOUNDING_BOX box2)
{
	if (box1.max.x < box2.min.x)
		return false;
	if (box1.min.x > box2.max.x)
		return false;
	if (box1.max.y < box2.min.y)
		return false;
	if (box1.min.y > box2.max.y)
		return false;
	if (box1.max.z < box2.min.z)
		return false;
	if (box1.min.z > box2.max.z)
		return false;

	return true;
}

// バウンディングボックスが衝突していたらtrueを返す
bool IsIntersectedBoundingBox(D3DXVECTOR3 pos1, BOUNDING_BOX localBox1, D3DXVECTOR3 pos2, BOUNDING_BOX localBox2)
{
	BOUNDING_BOX worldBox1, worldBox2;
	worldBox1.max = pos1 + localBox1.max;
	worldBox1.min = pos1 + localBox1.min;
	worldBox2.max = pos2 + localBox2.max;
	worldBox2.min = pos2 + localBox2.min;

	if (IsIntersectedBoundingBox(worldBox1, worldBox2))
		return true;

	return false;
}

D3DXVECTOR3 BoundingBoxCenter(BOUNDING_BOX box)
{
	return (box.max + box.min) / 2.0f;
}

float BoundingBoxSizeX(BOUNDING_BOX box)
{
	return box.max.x - box.min.x;
}

float BoundingBoxSizeY(BOUNDING_BOX box)
{
	return box.max.y - box.min.y;
}


// バウンディングボックスを描画
void DrawDebugBoundingBox(BOUNDING_BOX box)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxWorld, mtxTranslate, mtxScl;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_vtxBoundingBox, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	float x = BoundingBoxSizeX(box);
	float y = BoundingBoxSizeY(box);
	D3DXMatrixScaling(&mtxScl, x, y, 1.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	//// 移動を反映
	D3DXVECTOR3 center = BoundingBoxCenter(box);
	D3DXMatrixTranslation(&mtxTranslate, center.x, center.y, center.z - 10.f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, g_textureBoundingBox);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
