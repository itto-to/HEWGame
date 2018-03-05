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
#include "sound.h"
#include "stage.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SLIDING_COUNT	(20.0f)

#define BUTTON_JUMP			(BUTTON_A)
#define BUTTON_LARGEJUMP	(BUTTON_C)
#define BUTTON_DASH			(BUTTON_A)
#define BUTTON_SLIDING		(BUTTON_DOWN)

#define KEY_JUMP			(DIK_Z)
#define KEY_LARGEJUMP		(DIK_X)
#define KEY_DASH			(DIK_Z)
#define KEY_SLIDING			(DIK_DOWN)


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
	case PLAYER_STARTDASH:
		EnterPlayerStartDash(player);
		break;
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
	case PLAYER_STARTDASH:
		ExitPlayerStartDash(player);
		break;
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



void UpdatePlayerStartDash(PLAYER * player)
{
	// �_�b�V���{�^���A�łŃQ�[�W�𒙂߂�
	if (IsButtonTriggered(player->lane_no, BUTTON_DASH) || GetKeyboardTrigger(KEY_DASH))
	{
		player->dash_gauge += 1.0f;
		PlaySound(SOUND_LABEL_RENDA);	// �A�ŉ��Đ�
	}

	// �Q�[�W�����܂�����Q�[���J�n
	if (GetStageState() == STAGE_STATE_GAMEPLAY)
	{
		player->next_state = PLAYER_ONGROUND;
	}
}

void UpdatePlayerOnGround(PLAYER *player)
{
	// �W�����v����
	if (IsButtonTriggered(player->lane_no, BUTTON_JUMP) || GetKeyboardTrigger(KEY_JUMP))
	{
		player->jump_speed = PLAYER_JUMP_SPEED;
		player->next_state = PLAYER_JUMP;
		PlaySound(SOUND_LABEL_SMALLJUMP);// �W�����v���Đ�
	}
	else if (IsButtonTriggered(player->lane_no, BUTTON_LARGEJUMP) || GetKeyboardTrigger(KEY_LARGEJUMP))
	{
		player->jump_speed = PLAYER_LARGE_JUMP_SPEED;
		player->next_state = PLAYER_JUMP;
		PlaySound(SOUND_LABEL_HIGHJUMP);// ��W�����v���Đ�
	}
	else if (IsButtonTriggered(player->lane_no, BUTTON_SLIDING) || GetKeyboardTrigger(KEY_SLIDING))	// �X���C�f�B���O����
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
	player->pos.y = player->ground + player->jump_speed * player->state_counter + 0.5f * GRAVITY_ACCELARATION * player->state_counter * player->state_counter;

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
	player->pos.x += 5.0f;
	player->rot.z -= 0.05f;
}

void EnterPlayerStartDash(PLAYER * player)
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

void ExitPlayerStartDash(PLAYER * player)
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
	player->hit_box = PLAYER_HIT_BOX;
}

void ExitPlayerDead(PLAYER *player)
{

}
