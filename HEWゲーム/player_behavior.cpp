//=============================================================================
//
// プレイヤーアクション処理 [player_behavior.h]
// Author : 
//
//=============================================================================
#include "player_behavior.h"

#include <assert.h>

#include "debugproc.h"
#include "input.h"
#include "player.h"
#include "stage.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SLIDING_COUNT	(20.0f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void EnterPlayerState(PLAYER *player);
void ExitPlayerState(PLAYER *player);



void ChangePlayerState(PLAYER *player)
{
	if (player->next_state == PLAYER_NONE)	// 次のステートがなければ終了
		return;

	// 現在のステートのExit処理を実行
	ExitPlayerState(player);

	// ステートを変更
	player->state = player->next_state;
	player->next_state = PLAYER_NONE;

	// 次のステートのEnter処理を実行
	EnterPlayerState(player);

	// ステートカウンターをリセット
	player->state_counter = 0.0f;
}

void EnterPlayerState(PLAYER *player) {
	switch (player->state)
	{
	case PLAYER_ONGROUND:
		EnterPlayerOnGround(player);
		break;
	case PLAYER_JUMP:
		EnterPlayerJump(player);
		break;
	case PLAYER_SLIDING:
		EnterPlayerSliding(player);
		break;
	case PLAYER_DEAD:
		EnterPlayerDead(player);
		break;
	default:
		assert(!"不正なプレイヤーステート");
		break;
	}
}

void ExitPlayerState(PLAYER *player) {
	switch (player->state)
	{
	case PLAYER_ONGROUND:
		ExitPlayerOnGround(player);
		break;
	case PLAYER_JUMP:
		ExitPlayerJump(player);
		break;
	case PLAYER_SLIDING:
		ExitPlayerSliding(player);
		break;
	case PLAYER_DEAD:
		ExitPlayerDead(player);
		break;
	default:
		assert(!"不正なプレイヤーステート");
		break;
	}
}



void UpdatePlayerOnGround(PLAYER *player)
{
	// ジャンプ処理
	if (GetKeyboardTrigger(DIK_Z))
	{
		player->move.y = PLAYER_JUMP_SPEED;
		player->next_state = PLAYER_JUMP;
	}
	else if (GetKeyboardTrigger(DIK_X))
	{
		player->move.y = PLAYER_BIG_JUMP_SPEED;
		player->next_state = PLAYER_JUMP;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))	// スライディング処理
	{
		player->next_state = PLAYER_SLIDING;
	}
}

void UpdatePlayerJump(PLAYER *player)
{
	float speed_factor = GetLane(player->lane_no)->speed_factor;
	// y = v0 * t + 1 / 2 * g * t^2
	// ジャンプ処理
	player->state_counter += speed_factor;
	player->pos.y = player->ground + PLAYER_JUMP_SPEED * player->state_counter + 0.5f * GRAVITY_ACCELARATION * player->state_counter * player->state_counter;

	//player->pos += player->move * player->speed_factor;
	//player->move.y += GRAVITY_ACCELARATION * player->speed_factor;	// 重力加速度

	// 着地判定
	if (player->pos.y < player->ground)
	{
		player->pos.y = player->ground;		// 高さが地面より低かったら地面位置にリセット
		player->next_state = PLAYER_ONGROUND;	// プレイヤーの状態を着地中に
	}
}

void UpdatePlayerSliding(PLAYER *player)
{
	float speed_factor = GetLane(player->lane_no)->speed_factor;
	player->state_counter += speed_factor;
	if (player->state_counter >= SLIDING_COUNT)	// スライディング時間が一定以上なら通常状態に戻す
	{
		player->next_state = PLAYER_ONGROUND;
	}
}

void UpdatePlayerDead(PLAYER *player)
{

}

void EnterPlayerOnGround(PLAYER *player)
{
	
}

void EnterPlayerJump(PLAYER *player)
{

}

void EnterPlayerSliding(PLAYER *player)
{
	// 当たり判定を小さくする
	player->hit_box = SLIDING_HIT_BOX;
}

void EnterPlayerDead(PLAYER *player)
{

}

void ExitPlayerOnGround(PLAYER *player)
{

}

void ExitPlayerJump(PLAYER *player)
{
	player->state_counter = 0.0f;
}

void ExitPlayerSliding(PLAYER *player)
{
	// 当たり判定を元に戻す
	player->hit_box.max = PLAYER_BB_MAX;
	player->hit_box.min = PLAYER_BB_MIN;
}

void ExitPlayerDead(PLAYER *player)
{

}
