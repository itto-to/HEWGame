//=============================================================================
//
// プレイヤーアクション処理 [player_behavior.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYERBEHAVIOR_H_
#define _PLAYERBEHAVIOR_H_

#include "player.h"

void ChangePlayerState(PLAYER *player);

void EnterPlayerStartDash(PLAYER *player);
void EnterPlayerOnGround(PLAYER *player);
void EnterPlayerJump(PLAYER *player);
void EnterPlayerSliding(PLAYER *player);
void EnterPlayerDead(PLAYER *player);

void ExitPlayerStartDash(PLAYER *player);
void ExitPlayerOnGround(PLAYER *player);
void ExitPlayerJump(PLAYER *player);
void ExitPlayerSliding(PLAYER *player);
void ExitPlayerDead(PLAYER *player);

void UpdatePlayerStartDash(PLAYER *player);
void UpdatePlayerOnGround(PLAYER *player);
void UpdatePlayerJump(PLAYER *player);
void UpdatePlayerSliding(PLAYER *player);
void UpdatePlayerDead(PLAYER *player);


#endif