//=============================================================================
//
// �v���C���[�A�N�V�������� [player_behavior.h]
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
// �}�N����`
//*****************************************************************************
#define SLIDING_COUNT	(20.0f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void EnterPlayerState(PLAYER *player);
void ExitPlayerState(PLAYER *player);



void ChangePlayerState(PLAYER *player)
{
	if (player->next_state == PLAYER_NONE)	// ���̃X�e�[�g���Ȃ���ΏI��
		return;

	// ���݂̃X�e�[�g��Exit���������s
	ExitPlayerState(player);

	// �X�e�[�g��ύX
	player->state = player->next_state;
	player->next_state = PLAYER_NONE;

	// ���̃X�e�[�g��Enter���������s
	EnterPlayerState(player);

	// �X�e�[�g�J�E���^�[�����Z�b�g
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
		assert(!"�s���ȃv���C���[�X�e�[�g");
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
		assert(!"�s���ȃv���C���[�X�e�[�g");
		break;
	}
}



void UpdatePlayerOnGround(PLAYER *player)
{
	// �W�����v����
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
	else if (GetKeyboardTrigger(DIK_DOWN))	// �X���C�f�B���O����
	{
		player->next_state = PLAYER_SLIDING;
	}
}

void UpdatePlayerJump(PLAYER *player)
{
	float speed_factor = GetLane(player->lane_no)->speed_factor;
	// y = v0 * t + 1 / 2 * g * t^2
	// �W�����v����
	player->state_counter += speed_factor;
	player->pos.y = player->ground + PLAYER_JUMP_SPEED * player->state_counter + 0.5f * GRAVITY_ACCELARATION * player->state_counter * player->state_counter;

	//player->pos += player->move * player->speed_factor;
	//player->move.y += GRAVITY_ACCELARATION * player->speed_factor;	// �d�͉����x

	// ���n����
	if (player->pos.y < player->ground)
	{
		player->pos.y = player->ground;		// �������n�ʂ��Ⴉ������n�ʈʒu�Ƀ��Z�b�g
		player->next_state = PLAYER_ONGROUND;	// �v���C���[�̏�Ԃ𒅒n����
	}
}

void UpdatePlayerSliding(PLAYER *player)
{
	float speed_factor = GetLane(player->lane_no)->speed_factor;
	player->state_counter += speed_factor;
	if (player->state_counter >= SLIDING_COUNT)	// �X���C�f�B���O���Ԃ����ȏ�Ȃ�ʏ��Ԃɖ߂�
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
	// �����蔻�������������
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
	// �����蔻������ɖ߂�
	player->hit_box.max = PLAYER_BB_MAX;
	player->hit_box.min = PLAYER_BB_MIN;
}

void ExitPlayerDead(PLAYER *player)
{

}
