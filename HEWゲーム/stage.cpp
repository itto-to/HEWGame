//=============================================================================
//
// ステージ処理 [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include <stdio.h>

#include "input.h"
#include "camera.h"
#include "collision.h"
#include "debugproc.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_FILE		"data/stage.csv"
#
#define TEXTURE_PLAIN_SLIDING "data/TEXTURE/ワシ_平原_スライディング.png"
#define MAX_OBSTACLE	(256)
#define OBSTACLE_WIDTH	(100)
#define OBSTACLE_HEIGHT	(100)
#define OBSTACLE_DEFAULT_MOVE	(D3DXVECTOR3(10.0f, 0.0f, 0.0f))


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ReadStageData(void);
void SetStageData(STAGE stage);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

STAGE g_stage;		// 現在のステージ
LANE g_lane[MAX_PLAYER];
OBSTACLE g_obstacle;
STAGE_DATA g_stage_data[STAGE_MAX];


HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ファイルからステージ情報読込
	ReadStageData();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_PLAIN_SLIDING,		// ファイルの名前
		&g_obstacle.texture);	// 読み込むメモリー

	// レーン初期化
	for (int no = 0; no < 1; no++)
	{
		g_lane[no].use = true;
		g_lane[no].speed_factor = 1.0f;

		g_obstacle.pos = D3DXVECTOR3(-SCREEN_WIDTH / 2.0f, LANE_Y(no), LANE_Z(no));
		g_obstacle.move = OBSTACLE_DEFAULT_MOVE;
		g_obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_obstacle.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_obstacle.lane_no = 0;

		// 当たり判定初期化
		InitBoundingBox(&g_obstacle.hit_box, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);

		// 画面外判定初期化
		InitBoundingBox(&g_obstacle.screen_box, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);

		// 頂点作成
		MakeVertex(pDevice, &g_obstacle.vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
	}

	return S_OK;
}

void UninitStage(void)
{
	SAFE_RELEASE(g_obstacle.vtx);
	SAFE_RELEASE(g_obstacle.texture);
}

void UpdateStage(void)
{
#ifdef _DEBUG
	// TEST: 速度変化テスト
	for (int no = 0; no < MAX_PLAYER; no++) {
		if (GetKeyboardPress(DIK_I)) {
			g_lane[no].speed_factor += 0.1f;
		}
		if (GetKeyboardPress(DIK_O)) {
			g_lane[no].speed_factor -= 0.1f;
		}
		PrintDebugProc("速度係数：%f", g_lane[no].speed_factor);
	}
#endif

	// 障害物の移動処理
	g_obstacle.pos += g_obstacle.move * g_lane[g_obstacle.lane_no].speed_factor;

	// 画面外に出たら位置リセット
	BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle.hit_box, g_obstacle.pos);
	if (IsObjectOffscreen(worldBox))
	{
		g_obstacle.pos.x = -SCREEN_WIDTH / 2.0f;
	}
}

void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_obstacle.vtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_obstacle.rot.y, g_obstacle.rot.x, g_obstacle.rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_obstacle.pos.x, g_obstacle.pos.y, g_obstacle.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, g_obstacle.texture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

#ifdef _DEBUG
	BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle.hit_box, g_obstacle.pos);
	DrawDebugBoundingBox(worldBox);
#endif
}


LANE *GetLane(int no) {
	return &g_lane[no];
}

OBSTACLE *GetObstacle(void)
{
	return &g_obstacle;
}

void ReadStageData(void)
{
	// ファイルオープン
	FILE *file = fopen(STAGE_FILE, "r");

	for (int i = 0; i < STAGE_MAX; i++) {
		char stage_name[MAX_PATH];
		fscanf(file, "%[^,],", stage_name);
		int stage_no;
		if (strcmp(stage_name, "平原") == 0) {
			stage_no = STAGE_PLAIN;
		}
		else if (strcmp(stage_name, "砂漠") == 0) {
			stage_no = STAGE_DESERT;
		}
		else if (strcmp(stage_name, "火山") == 0) {
			stage_no = STAGE_VOLCANO;
		}
		else if (strcmp(stage_name, "雪") == 0) {
			stage_no = STAGE_SNOW;
		}

		int tile_no = 0;
		while (true)
		{
			char no = fgetc(file);

			if (no == ',')
			{
				fscanf(file, "%*[^\n]\n");
				break;
			}
			else if (no == '0')
			{
				g_stage_data[stage_no].tile[tile_no] = 0;
			}
			else if (no == '1')
			{
				g_stage_data[stage_no].tile[tile_no] = 1;
			}
			else if (no == '2')
			{
				g_stage_data[stage_no].tile[tile_no] = 2;
			}
			else if (no == '3')
			{
				g_stage_data[stage_no].tile[tile_no] = 3;
			}
			tile_no++;

			char c = fgetc(file);

			if (c == '\n' || c == EOF)
			{
				break;
			}
		}
		g_stage_data[stage_no].length = tile_no;
	}

	// ファイルクローズ
	fclose(file);
}

void SetStageData(STAGE stage)
{
	for (int i = 0; i < g_stage_data[stage].length; i++)
	{
		g_stage_data[stage].tile;
	}
}

