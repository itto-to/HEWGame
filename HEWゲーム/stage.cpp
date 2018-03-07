//=============================================================================
//
// ステージ処理 [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "camera.h"
#include "collision.h"
#include "debugproc.h"
#include "fade.h"
#include "main.h"
#include "mesh.h"
#include "skill.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_FILE		"data/stage.csv"

// 障害物
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

// レーン
#define TEXTURE_LANE			"data/TEXTURE/stage_lane.png"
#define LANE_WIDTH				(120.0f)
#define LANE_HEIGHT				(10.f)
#define LANE_INTERVAL			(LANE_WIDTH * 0.8)
#define NUM_DRAW_LANE			(20)

#define JUMP_OVER_DECISION_POS_X	(400.0f)

#define OBSTACLE_WIDTH	(100.0f)
#define OBSTACLE_HEIGHT	(100.0f)
#define OBSTACLE_DEFAULT_VELOCITY	(D3DXVECTOR3(5.0f, 0.0f, 0.0f))
#define OBSTACLE_PADDING	(-SCREEN_WIDTH)

#define OBSTACLE_SCREEN_BOX			{D3DXVECTOR3(-OBSTACLE_WIDTH / 2, 0.0f, 0.0f), D3DXVECTOR3(OBSTACLE_WIDTH / 2, OBSTACLE_HEIGHT, 0.0f)}
#define OBSTACLE_JUMP_HIT_BOX		{D3DXVECTOR3(-20.0f,  0.0f, 0.0f), D3DXVECTOR3(20.0f,  25.0f, 0.0f)}
#define OBSTACLE_LARGEJUMP_HIT_BOX	{D3DXVECTOR3(-20.0f,  0.0f, 0.0f), D3DXVECTOR3(20.0f,  70.0f, 0.0f)}
#define OBSTACLE_SLIDING_HIT_BOX	{D3DXVECTOR3(-20.0f, 50.0f, 0.0f), D3DXVECTOR3(20.0f, 100.0f, 0.0f)}

#define MAX_COUNTDOWN	(3)		// カウントダウンは3
#define COUNT_WIDTH		(218)
#define COUNT_HEIGHT	(350)

#define CHAR_SPACE		('0')
#define CHAR_JUMP		('1')
#define CHAR_LARGEJUMP	('2')
#define CHAR_SLIDING	('3')

#define COUNTDOWN_INTERVAL				(100)		// カウントダウンの間隔
#define MAX_CHANGE_STAGE_COUNT			(10 * 60)	// ステージが変わるまでのカウント


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void UpdateStageCountDown(void);
void UpdateStageGameplay(void);
void UpdateStageStartDash(void);
void DrawLane(void);
void DrawObstacle(OBSTACLE *obstacle);
void ReadStageData(void);
void SetStageData(STAGE_TYPE stage);
void SetStageObstacle(STAGE_TYPE stage);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
STAGE_STATE g_stage_state;			// 現在のステージ状態
STAGE_TYPE g_stage_type;			// 現在のステージ
int g_stage_count;					// 現在のステージのカウント
LANE g_lane[MAX_PLAYER];
LPDIRECT3DVERTEXBUFFER9 g_lane_vtx;	// レーンの頂点
LPDIRECT3DTEXTURE9 g_lane_tex;		// レーンのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_count_vtx;	// カウントダウンの頂点
LPDIRECT3DTEXTURE9 g_count_tex[MAX_COUNTDOWN];		// カウントダウンのテクスチャ
OBSTACLE g_obstacle[MAX_PLAYER][MAX_NUM_OBSTACLE];	// 障害物配列
STAGE_DATA g_stage_data[STAGE_MAX];
LPDIRECT3DTEXTURE9 g_obstacle_texture[STAGE_MAX][OBSTACLE_MAX];
LPDIRECT3DVERTEXBUFFER9 g_obstacle_vtx;


//****************************************************************************
// 関数名:	HRESULT InitStage(void)
// 引数:	なし
// 戻り値:	初期化の成功・失敗
// 説明:	ステージ初期化
//*****************************************************************************
HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ファイルからステージ情報読込
	ReadStageData();

	// 障害物テクスチャの読み込み
	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++)
	{
		for (int obstacle_no = 0; obstacle_no < OBSTACLE_MAX; obstacle_no++)
		{
			char filename[MAX_PATH] = "data/TEXTURE/obstacle_";	
			// ステージタイプと障害物タイプに応じてテクスチャの文字列を連結
			switch ((STAGE_TYPE)stage_no)
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
				&g_obstacle_texture[stage_no][obstacle_no]);	// 読み込むメモリー
		}
	}

	// レーンテクスチャ読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_LANE, &g_lane_tex)))
	{
		return E_FAIL;
	}

	// 頂点作成
	MakeVertex(pDevice, &g_obstacle_vtx, &D3DXVECTOR3(0.0f, OBSTACLE_HEIGHT / 2.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
	MakeVertex(pDevice, &g_lane_vtx, NULL, LANE_WIDTH, LANE_HEIGHT);
	MakeVertex(pDevice, &g_count_vtx, NULL, COUNT_WIDTH, COUNT_HEIGHT);	// カウントダウン用

	// 最初はカウントダウンから
	g_stage_state = STAGE_STATE_COUNTDOWN;

	// 最初は平原ステージから
	g_stage_type = STAGE_PLAIN;

	// ステージのカウントを0に
	g_stage_count = 0;

	// レーン初期化
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		g_lane[i].use = true;
		g_lane[i].speed_factor = 1.0f;
	}

	// ステージに合わせて障害物接地
	SetStageObstacle(g_stage_type);


	return S_OK;
}


//****************************************************************************
// 関数名:	void UninitStage(void)
// 引数:	なし
// 戻り値:	なし
// 説明:	ステージ終了処理
//****************************************************************************
void UninitStage(void)
{
	// 障害物テクスチャ
	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++) {
		for (int obstacle_no = 0; obstacle_no < OBSTACLE_MAX; obstacle_no++) {
			SAFE_RELEASE(g_obstacle_texture[stage_no][obstacle_no]);
			g_obstacle[stage_no][obstacle_no].use = false;
		}
	}

	SAFE_RELEASE(g_obstacle_vtx);	// 障害物頂点
	SAFE_RELEASE(g_lane_vtx);		// レーン頂点
	SAFE_RELEASE(g_lane_tex);		// レーンテクスチャ
	SAFE_RELEASE(g_count_vtx);		// カウントダウン頂点
	for (int i = 0; i < MAX_COUNTDOWN; i++)	// カウントダウンテクスチャ
	{
		SAFE_RELEASE(g_count_tex[i]);
	}

	// useをfalseに
	for (int player_no = 0; player_no < MAX_PLAYER; player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			g_obstacle[player_no][obstacle_no].use = false;
		}
	}
}

// ステージ更新処理
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

	// ステージの状態に応じてアップデート
	switch (g_stage_state)
	{
	case STAGE_STATE_COUNTDOWN:
		UpdateStageCountDown();
		break;
	case STAGE_STATE_STARTDASH:
		UpdateStageStartDash();
		break;
	case STAGE_STATE_GAMEPLAY:
		UpdateStageGameplay();
		break;
	}

}

// カウントダウン中の更新処理
void UpdateStageCountDown(void)
{
	if (!IsFading())
	{
		// カウントを進める
		g_stage_count++;
		// カウントダウン音再生
		if (!IsPlaying(SOUND_LABEL_COUNT))
			PlaySound(SOUND_LABEL_COUNT);
	}

	if (g_stage_count >= COUNTDOWN_INTERVAL * 2)
	{
		// 一定カウントでスタートダッシュ状態へ
		g_stage_state = STAGE_STATE_STARTDASH;
		PlaySound(SOUND_LABEL_BGM000);
	}
}

// スタートダッシュ中の更新処理
void UpdateStageStartDash(void)
{
	// ダッシュゲージが満タンのプレイヤー番号を記録する配列
	int dashing_players[MAX_PLAYER];
	int num_dashing_player = 0;

	// ダッシュゲージを貯めきったプレイヤーがいたらゲーム開始
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		if (IsDashGaugeFull(player_no))
		{
			dashing_players[num_dashing_player] = player_no;	// ダッシュゲージが満タンのプレイヤー番号を記録
			num_dashing_player++;								// ダッシュゲージが満タンのプレイヤー数
		}
	}

	if (num_dashing_player != 0)	// ダッシュゲージが満タンのプレイヤーがいたら
	{
		// その中からランダムで決定しスキル発動権をあげる
		srand((unsigned int)time(NULL));			// 乱数初期化
		int idx = rand() % num_dashing_player;
		int skill_player_no = dashing_players[idx];
		skillwinner(skill_player_no);

		g_stage_state = STAGE_STATE_GAMEPLAY;	// ゲーム本編に移行
		PlaySound(SOUND_LABEL_BGM000);			// BGM再生開始
	}
}

// ゲーム本編の更新処理
void UpdateStageGameplay(void)
{
	if (g_stage_count >= MAX_CHANGE_STAGE_COUNT)
	{
		g_stage_count = 0;	// ステージのカウントリセット

		// 次のステージへ変更
		g_stage_type = (STAGE_TYPE)((g_stage_type + 1) % STAGE_MAX);
		SetStageObstacle(g_stage_type);
	}

	// ステージのカウント増加
	g_stage_count++;

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
			
				// スキルポイントを上げる
				player->skillpoint++;
				if(player->kengen == true)
				{
					AddSkillGauge(1);
				}
				obstacle->should_give_skillpoint = false;
			}
		}
	}

	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle[player_no][obstacle_no].hit_box, g_obstacle[player_no][obstacle_no].pos);
			bool is_offscreen = IsObjectOffscreen(worldBox);	// 完全に画面外にあるならtrue

			// 画面外にあり現在のステージに所属していないオブジェクトは消去
			if (is_offscreen && (g_obstacle[player_no][obstacle_no].stage != g_stage_type))
			{
				g_obstacle[player_no][obstacle_no].use = false;
			}

			// 画面右外に出たか判定
			if (is_offscreen && (g_obstacle[player_no][obstacle_no].pos.x > 0.0f))
			{
					// レーンの長さ分戻す
					g_obstacle[player_no][obstacle_no].pos.x -= g_stage_data[g_stage_type].lane_length;
					// スキルポイントを与えられるように
					g_obstacle[player_no][obstacle_no].should_give_skillpoint = true;
			}
		}
	}
}

void ChangeStageState(STAGE_STATE next_stage_state)
{
	// ステージカウントをリセット
	g_stage_count = 0;
}

void DrawStage(void)
{
	// レーン描画
	DrawLane();

	// 障害物描画
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			if (!g_obstacle[player_no][obstacle_no].use)
				continue;

			DrawObstacle(&g_obstacle[player_no][obstacle_no]);
		}
	}

	// カウントダウン描画
	//DrawCountDown();
}

void DrawCountDown()
{
	// カウントダウン状態ならカウント描画
	//if (g_stage_state == STAGE_STATE_COUNTDOWN)
	//{
	//	if (g_stage_count >= COUNTDOWN_INTERVAL * 2)	
	//	{// '1'描画
	//		DrawMesh()
	//	}
	//	else if (g_stage_count >= COUNTDOWN_INTERVAL)
	//	{

	//	}
	//	else
	//	{

	//	}
	//}
	//	
}

void DrawLane(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int lane = 0; lane < NUM_DRAW_LANE; lane++)
		{
			DrawMesh(g_lane_vtx, g_lane_tex, D3DXVECTOR3(-SCREEN_WIDTH / 2 + LANE_INTERVAL * lane, LANE_Y(player_no), LANE_Z(player_no)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}
}

void DrawObstacle(OBSTACLE *obstacle)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_obstacle_vtx, 0, sizeof(VERTEX_3D));

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

void SetStageData(STAGE_TYPE stage)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{

		for (int i = 0; i < g_stage_data[stage].num_tile; i++)
		{
			g_stage_data[stage].tile;
		}
	}
}


void SetStageObstacle(STAGE_TYPE stage)
{
	for (int tile_no = 0; tile_no < g_stage_data[stage].num_tile; tile_no++) {
		for (int player_no = 0; player_no < NumPlayer(); player_no++) {
			char tile = g_stage_data[stage].tile[tile_no];
			if (tile == CHAR_SPACE)	// スペースなら何も置かない
				continue;

			OBSTACLE obstacle;
			int length = g_stage_data[stage].num_tile;
			obstacle.use = true;
			obstacle.should_give_skillpoint = true;
			obstacle.lane_no = player_no;
			obstacle.pos = D3DXVECTOR3((length - tile_no) * -OBSTACLE_WIDTH + OBSTACLE_PADDING, LANE_Y(player_no), LANE_Z(player_no));
			obstacle.move = OBSTACLE_DEFAULT_VELOCITY;
			obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			obstacle.rot_dest = obstacle.rot;
			obstacle.stage = stage;
			obstacle.screen_box = OBSTACLE_SCREEN_BOX;
			obstacle.hit_box = OBSTACLE_JUMP_HIT_BOX;

			switch(tile) {
			case CHAR_JUMP:
				obstacle.texture = g_obstacle_texture[stage][OBSTACLE_JUMP];
				obstacle.hit_box = OBSTACLE_JUMP_HIT_BOX;
				break;
			case CHAR_LARGEJUMP:
				obstacle.texture = g_obstacle_texture[stage][OBSTACLE_LARGE_JUMP];
				obstacle.hit_box = OBSTACLE_LARGEJUMP_HIT_BOX;
				break;
			case CHAR_SLIDING:
				obstacle.texture = g_obstacle_texture[stage][OBSTACLE_SLIDING];
				obstacle.hit_box = OBSTACLE_SLIDING_HIT_BOX;
				break;
			default:
				assert(!"不正なキャラクター");
				break;
			}

			// 障害物セット
			for (int i = 0; i < MAX_NUM_OBSTACLE; i++)
			{
				if (!g_obstacle[player_no][i].use)
				{
					g_obstacle[player_no][i] = obstacle;
					break;
				}
			}
		}
	}
}

void SetObstacleStatus(OBSTACLE *obstacle, D3DXVECTOR3 pos, STAGE_TYPE stage, OBSTACLE_TYPE type)
{

}

// 現在のステージを取得
STAGE_TYPE GetStage(void)
{
	return g_stage_type;
}

STAGE_STATE GetStageState(void)
{
	return g_stage_state;
}