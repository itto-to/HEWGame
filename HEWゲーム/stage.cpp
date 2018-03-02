//=============================================================================
//
// ステージ処理 [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include <assert.h>
#include <stdio.h>

#include "input.h"
#include "camera.h"
#include "collision.h"
#include "debugproc.h"
#include "main.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_FILE		"data/stage.csv"

#define TEXTURE_PLAIN_JUMP      "data/TEXTURE/スライム_平原_小ジャンプ.png"
#define TEXTURE_PLAIN_LARGEJUMP "data/TEXTURE/木_平原_大ジャンプ.png"
#define TEXTURE_PLAIN_SLIDING   "data/TEXTURE/ワシ_平原_スライディング.png"

#define TEXTURE_DESERT_JUMP      "data/TEXTURE/サソリ_砂漠_小ジャンプ.png"
#define TEXTURE_DESERT_LARGEJUMP "data/TEXTURE/サボテン_砂漠_大ジャンプ.png"
#define TEXTURE_DESERT_SLIDING   "data/TEXTURE/プテラノドン_砂漠_スライディング.png"

#define TEXTURE_VOLCANO_JUMP      "data/TEXTURE/コモドドラゴン_火山_小ジャンプ.png"
#define TEXTURE_VOLCANO_LARGEJUMP "data/TEXTURE/火山_岩_大ジャンプ.png"
#define TEXTURE_VOLCANO_SLIDING   "data/TEXTURE/こうもり_火山_スライディング.png"

#define TEXTURE_SNOW_JUMP      "data/TEXTURE/雪うさぎ_雪_小ジャンプ.png"
#define TEXTURE_SNOW_LARGEJUMP "data/TEXTURE/スノーマン_雪_大ジャンプ.png"
#define TEXTURE_SNOW_SLIDING   "data/TEXTURE/雪玉_雪_スライディング.png"

#define JUMP_OVER_DECISION_POS_X	(400.0f)

#define OBSTACLE_WIDTH	(100.0f)
#define OBSTACLE_HEIGHT	(100.0f)
#define OBSTACLE_DEFAULT_MOVE	(D3DXVECTOR3(10.0f, 0.0f, 0.0f))
#define OBSTACLE_PADDING	(SCREEN_WIDTH)

#define OBSTACLE_SCREEN_BOX	{D3DXVECTOR3(-OBSTACLE_WIDTH / 2, -OBSTACLE_HEIGHT / 2, 0.0f), D3DXVECTOR3(OBSTACLE_WIDTH / 2, OBSTACLE_HEIGHT / 2, 0.0f)}
#define OBSTACLE_JUMP_HIT_BOX	{D3DXVECTOR3(-OBSTACLE_WIDTH / 2, -OBSTACLE_HEIGHT / 2, 0.0f), D3DXVECTOR3(OBSTACLE_WIDTH / 2, OBSTACLE_HEIGHT / 2, 0.0f)}

#define CHAR_SPACE		('0')
#define CHAR_JUMP		('1')
#define CHAR_LARGEJUMP	('2')
#define CHAR_SLIDING	('3')


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawObstacle(OBSTACLE *obstacle);
void ReadStageData(void);
void SetStageData(STAGE stage);
void SetObstacle(STAGE stage);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

STAGE g_stage;		// 現在のステージ
LANE g_lane[MAX_PLAYER];
OBSTACLE g_obstacle[MAX_PLAYER][MAX_NUM_OBSTACLE];
STAGE_DATA g_stage_data[STAGE_MAX];
LPDIRECT3DTEXTURE9 g_texture[STAGE_MAX][OBSTACLE_MAX];
LPDIRECT3DVERTEXBUFFER9 g_vtx;


HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ファイルからステージ情報読込
	ReadStageData();

	// テクスチャの読み込み
	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++)
	{
		for (int obstacle_no = 0; obstacle_no < OBSTACLE_MAX; obstacle_no++)
		{
			char filename[MAX_PATH] = "data/TEXTURE/obstacle_";

			switch ((STAGE)stage_no)
			{
			case STAGE_PLAIN:
				strcat(filename, "plain_");
				break;
			case STAGE_DESERT:
				strcat(filename, "desert_");
				break;
			case STAGE_VOLCANO:
				strcat(filename, "volcano_");
				break;
			case STAGE_SNOW:
				strcat(filename, "snow_");
				break;
			}

			switch ((OBSTACLE_TYPE)obstacle_no)
			{
			case OBSTACLE_JUMP:
				strcat(filename, "jump.png");
				break;
			case OBSTACLE_LARGE_JUMP:
				strcat(filename, "largejump.png");
				break;
			case OBSTACLE_SLIDING:
				strcat(filename, "sliding.png");
				break;
			}

			D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
				filename,		// ファイルの名前
				&g_texture[stage_no][obstacle_no]);	// 読み込むメモリー
		}
	}

	// 頂点作成
	MakeVertex(pDevice, &g_vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);

	// 最初は平原ステージから
	g_stage = STAGE_PLAIN;

	// レーン初期化
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		g_lane[i].use = true;
		g_lane[i].speed_factor = 1.0f;
	}

	// ステージに合わせて障害物接地
	SetObstacle(g_stage);

	return S_OK;
}

void UninitStage(void)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++) {
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++) {
			SAFE_RELEASE(g_obstacle[player_no][obstacle_no].vtx);
			SAFE_RELEASE(g_obstacle[player_no][obstacle_no].texture);
		}
	}
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

		// TEST: 座標表示
		PrintDebugProc("速度係数：%f\n", g_lane[no].speed_factor);
		PrintDebugProc("敵座標X：%f\n", g_obstacle[0][0].pos.x);
	}
#endif

	// 障害物の移動処理
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			OBSTACLE *obstacle = &g_obstacle[player_no][obstacle_no];
			if (!obstacle->use)
				continue;

			obstacle->pos += obstacle->move * g_lane[obstacle->lane_no].speed_factor;

			PLAYER *player = GetPlayer(player_no);
			// プレイヤーが障害物を飛び越えたか判定
			// プレイヤーが無敵状態でない かつ 障害物がスキルポイントを未付与
			if ((obstacle->pos.x > JUMP_OVER_DECISION_POS_X) && obstacle->should_give_skillpoint && !player->is_invincible)
			{
				player->skillpoint++;
				obstacle->should_give_skillpoint = false;
			}
		}
	}

	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle[player_no][obstacle_no].hit_box, g_obstacle[player_no][obstacle_no].pos);
			// 画面右外に出たか判定
			if (IsObjectOffscreen(worldBox) && (g_obstacle[player_no][obstacle_no].pos.x > 0.0f))
			{	
				// 現在のステージと同じならリセット
				if (g_obstacle[player_no][obstacle_no].stage == g_stage)
				{
					// レーンの長さ分戻す
					g_obstacle[player_no][obstacle_no].pos.x -= g_stage_data[g_stage].lane_length;
					// スキルポイントを与えられるように
					g_obstacle[player_no][obstacle_no].should_give_skillpoint = true;
				}
				else // 現在のステージと違うなら消去
				{
					g_obstacle[player_no][obstacle_no].use = false;
				}
			}
		}
	}
}

void DrawStage(void)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			if (!g_obstacle[player_no][obstacle_no].use)
				continue;

			DrawObstacle(&g_obstacle[player_no][obstacle_no]);
		}
	}
}

void DrawObstacle(OBSTACLE *obstacle)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, obstacle->vtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, obstacle->rot.y, obstacle->rot.x, obstacle->rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, obstacle->pos.x, obstacle->pos.y, obstacle->pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, obstacle->texture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

#ifdef _DEBUG
	BOUNDING_BOX worldBox = ToWorldBoundingBox(obstacle->hit_box, obstacle->pos);
	DrawDebugBoundingBox(worldBox);
#endif

}


LANE *GetLane(int no) {
	return &g_lane[no];
}

OBSTACLE *GetObstacle(int lane_no, int obstacle_no)
{
	return &g_obstacle[lane_no][obstacle_no];
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
			else if (no == CHAR_SPACE)
			{
				g_stage_data[stage_no].tile[tile_no] = '0';
			}
			else if (no == CHAR_JUMP)
			{
				g_stage_data[stage_no].tile[tile_no] = '1';
			}
			else if (no == CHAR_LARGEJUMP)
			{
				g_stage_data[stage_no].tile[tile_no] = '2';
			}
			else if (no == CHAR_SLIDING)
			{
				g_stage_data[stage_no].tile[tile_no] = '3';
			}
			tile_no++;

			char c = fgetc(file);

			if (c == '\n' || c == EOF)
			{
				break;
			}
		}
		g_stage_data[stage_no].num_tile = tile_no;
		g_stage_data[stage_no].lane_length = tile_no * OBSTACLE_WIDTH;
	}

	// ファイルクローズ
	fclose(file);
}

void SetStageData(STAGE stage)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{

		for (int i = 0; i < g_stage_data[stage].num_tile; i++)
		{
			g_stage_data[stage].tile;
		}
	}
}

void SetObstacle(STAGE stage)
{
	for (int tile_no = 0; tile_no < g_stage_data[stage].num_tile; tile_no++) {
		for (int player_no = 0; player_no < NumPlayer(); player_no++) {
			char tile = g_stage_data[stage].tile[tile_no];
			int length = g_stage_data[stage].num_tile;
			if (tile == CHAR_SPACE)
				continue;

			OBSTACLE obstacle;
			obstacle.use = true;
			obstacle.should_give_skillpoint = true;
			obstacle.lane_no = player_no;
			obstacle.pos = D3DXVECTOR3((length - tile_no) * -OBSTACLE_WIDTH + OBSTACLE_PADDING, LANE_Y(player_no), LANE_Z(player_no));
			obstacle.move = OBSTACLE_DEFAULT_MOVE;
			obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			obstacle.rot_dest = obstacle.rot;
			obstacle.stage = stage;
			obstacle.screen_box = OBSTACLE_SCREEN_BOX;
			obstacle.hit_box = OBSTACLE_JUMP_HIT_BOX;
			obstacle.vtx = g_vtx;

			switch(tile) {
			case CHAR_JUMP:
				obstacle.texture = g_texture[stage][OBSTACLE_JUMP];
				break;
			case CHAR_LARGEJUMP:
				obstacle.texture = g_texture[stage][OBSTACLE_LARGE_JUMP];
				break;
			case CHAR_SLIDING:
				obstacle.texture = g_texture[stage][OBSTACLE_SLIDING];
				break;
			default:
				assert(!"不正なキャラクター");
				break;
			}

			// 障害物セット
			for (int i = 0; i < MAX_NUM_OBSTACLE; i++) {
				if (!g_obstacle[player_no][i].use) {
					g_obstacle[player_no][i] = obstacle;
					break;
				}

			}
		}
	}
}

void SetObstacleStatus(OBSTACLE *obstacle, D3DXVECTOR3 pos, STAGE stage, OBSTACLE_TYPE type)
{

}