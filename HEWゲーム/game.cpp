//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
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
#include "sound.h"
#include "stage.h"
#include "timer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHitPlayerObstacle(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	// ���C�g�̏�����
	InitLight();

	// �e�̏�����
	//InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �X�e�[�W�̏�����
	InitStage();

	// �w�i�̏�����
	InitBackground();

	// �X�L���Q�[�W�̏�����
	InitSkill();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM000);

#ifdef _DEBUG
	// �o�E���f�B���O�{�b�N�X�`��p������
	InitDebugBoundingBox();
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// ���C�g�̏I������
	UninitLight();

	// �v���C���[�̏I������
	UninitPlayer();

	// �X�e�[�W�̏I������
	UninitStage();

	// �w�i�̏I������
	UninitBackground();

	// �X�L���Q�[�W�̏I������
	UninitSkill();

	// �X�R�A�̏I������
	UninitScore();

	// BGM��~
	PlaySound(SOUND_LABEL_BGM000);

#ifdef _DEBUG
	// �o�E���f�B���O�{�b�N�X�`��p�I������
	UninitDebugBoundingBox();
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	// �w�i�����̍X�V
	UpdateBackground();

	// �v���C���[�����̍X�V
	UpdatePlayer();

	// �X�e�[�W�̍X�V����
	UpdateStage();

	// �v���C���[�Ə�Q���̓����蔻��
	CheckHitPlayerObstacle();

	// �X�L���̍X�V����
	UpdateSkill(0);

	// ���C�t�����̍X�V
	UpdateLife();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// �w�i�̕`��
	DrawBackground();

	// �X�e�[�W�̕`�揈��
	DrawStage();

	// �v���C���[�����̕`��
	DrawPlayer();

	// �X�L���̕`��
	DrawSkill();

	// ���C�t�����̕`��
	DrawLife();
}

void CheckHitPlayerObstacle(void)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		PLAYER *player = GetPlayer(player_no);

		if (player->is_invincible)	// ���G�Ȃ画�肵�Ȃ�
			continue;

		// �v���C���[�̃o�E���f�B���O�{�b�N�X�擾
		BOUNDING_BOX playerBox = ToWorldBoundingBox(player->hit_box, player->pos);

		// ��Q���̃o�E���f�B���O�{�b�N�X�擾
		OBSTACLE *obstacle = GetObstacle();
		BOUNDING_BOX obstacleBox = ToWorldBoundingBox(obstacle->hit_box, obstacle->pos);

		if (IsIntersectedBoundingBox(playerBox, obstacleBox))
		{
			// �v���C���[�̃��C�t����
			player->life--;
			// ���G��Ԃ�
			player->is_invincible = true;
			player->invincible_counter = 0;
		}
	}
}
