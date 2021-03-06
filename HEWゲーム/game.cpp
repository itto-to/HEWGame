//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "game.h"

#include "background.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "explosion.h"
#include "item.h"
#include "life.h"
#include "light.h"
#include "player.h"
#include "score.h"
#include "shadow.h"
#include "skill.h"
#include "skillact.h"
#include "sound.h"
#include "stage.h"
#include "timer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHitPlayerObstacle(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{

	// 影の初期化
	//InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// ステージの初期化
	InitStage();

	// 背景の初期化
	InitBackground();

	// スキルゲージの初期化
	InitSkill();
	InitSkillAct();

	// ライフの初期化
	InitLife();

#ifdef _DEBUG
	// バウンディングボックス描画用初期化
	InitDebugBoundingBox();
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{

	// プレイヤーの終了処理
	UninitPlayer();

	// ステージの終了処理
	UninitStage();

	// 背景の終了処理
	UninitBackground();

	// スキルゲージの終了処理
	UninitSkill();
	UninitSkillAct();

	// ライフの終了処理
	UninitLife();

	// スコアの終了処理
	UninitScore();

#ifdef _DEBUG
	// バウンディングボックス描画用終了処理
	UninitDebugBoundingBox();
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	// 背景処理の更新
	UpdateBackground();

	// プレイヤー処理の更新
	UpdatePlayer();

	// ステージの更新処理
	UpdateStage();

	// プレイヤーと障害物の当たり判定
	CheckHitPlayerObstacle();

	// スキルの更新処理
	GiveSkillUsingRight();
	UpdateSkillAct();

	// ライフ処理の更新
	UpdateLife();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// カメラの設定
	//SetCamera();

	// 背景の描画
	DrawBackground();

	// ステージの描画処理
	DrawStage();

	// プレイヤー処理の描画
	DrawPlayer();

	// スキルの描画
	DrawSkill();
	DrawSkillAct();

	// ライフ処理の描画
	DrawLife();
}

void CheckHitPlayerObstacle(void)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		PLAYER *player = GetPlayer(player_no);

		// プレイヤーのバウンディングボックス取得
		BOUNDING_BOX playerBox = ToWorldBoundingBox(player->hit_box, player->pos);

		// 障害物のバウンディングボックス取得
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			OBSTACLE *obstacle = GetObstacle(player_no, obstacle_no);
			

			BOUNDING_BOX obstacleBox = ToWorldBoundingBox(obstacle->hit_box, obstacle->pos);

			if (IsIntersectedBoundingBox(playerBox, obstacleBox))
			{
				// 無敵じゃなければプレイヤーのライフ減少
				if (!player->is_invincible)
				{
					player->life--;
					// 無敵状態に
					player->is_invincible = true;
					player->invincible_counter = 0;
					// スキル発動権はく奪
					player->skillpoint = 0;
					if (player->lane_no == GetSkillWk(0)->skill_player_id)
						GetSkillWk(0)->skill_player_id = -1;
				}

				// ぶつかった障害物はスキルポイントを与えない
				obstacle->should_give_skillpoint = false;
			}
		}
	}
}
