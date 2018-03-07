//=============================================================================
//
// �X�e�[�W���� [stage.cpp]
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
// �}�N����`
//*****************************************************************************
#define STAGE_FILE		"data/stage.csv"

// ��Q��
#define TEXTURE_PLAIN_JUMP      "data/TEXTURE/�X���C��_����_���W�����v.png"
#define TEXTURE_PLAIN_LARGEJUMP "data/TEXTURE/��_����_��W�����v.png"
#define TEXTURE_PLAIN_SLIDING   "data/TEXTURE/���V_����_�X���C�f�B���O.png"
#define TEXTURE_DESERT_JUMP      "data/TEXTURE/�T�\��_����_���W�����v.png"
#define TEXTURE_DESERT_LARGEJUMP "data/TEXTURE/�T�{�e��_����_��W�����v.png"
#define TEXTURE_DESERT_SLIDING   "data/TEXTURE/�v�e���m�h��_����_�X���C�f�B���O.png"
#define TEXTURE_VOLCANO_JUMP      "data/TEXTURE/�R���h�h���S��_�ΎR_���W�����v.png"
#define TEXTURE_VOLCANO_LARGEJUMP "data/TEXTURE/�ΎR_��_��W�����v.png"
#define TEXTURE_VOLCANO_SLIDING   "data/TEXTURE/��������_�ΎR_�X���C�f�B���O.png"
#define TEXTURE_SNOW_JUMP      "data/TEXTURE/�Ⴄ����_��_���W�����v.png"
#define TEXTURE_SNOW_LARGEJUMP "data/TEXTURE/�X�m�[�}��_��_��W�����v.png"
#define TEXTURE_SNOW_SLIDING   "data/TEXTURE/���_��_�X���C�f�B���O.png"

// ���[��
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

#define MAX_COUNTDOWN	(3)		// �J�E���g�_�E����3
#define COUNT_WIDTH		(218)
#define COUNT_HEIGHT	(350)

#define CHAR_SPACE		('0')
#define CHAR_JUMP		('1')
#define CHAR_LARGEJUMP	('2')
#define CHAR_SLIDING	('3')

#define COUNTDOWN_INTERVAL				(100)		// �J�E���g�_�E���̊Ԋu
#define MAX_CHANGE_STAGE_COUNT			(10 * 60)	// �X�e�[�W���ς��܂ł̃J�E���g


//*****************************************************************************
// �v���g�^�C�v�錾
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
// �O���[�o���ϐ�
//*****************************************************************************
STAGE_STATE g_stage_state;			// ���݂̃X�e�[�W���
STAGE_TYPE g_stage_type;			// ���݂̃X�e�[�W
int g_stage_count;					// ���݂̃X�e�[�W�̃J�E���g
LANE g_lane[MAX_PLAYER];
LPDIRECT3DVERTEXBUFFER9 g_lane_vtx;	// ���[���̒��_
LPDIRECT3DTEXTURE9 g_lane_tex;		// ���[���̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_count_vtx;	// �J�E���g�_�E���̒��_
LPDIRECT3DTEXTURE9 g_count_tex[MAX_COUNTDOWN];		// �J�E���g�_�E���̃e�N�X�`��
OBSTACLE g_obstacle[MAX_PLAYER][MAX_NUM_OBSTACLE];	// ��Q���z��
STAGE_DATA g_stage_data[STAGE_MAX];
LPDIRECT3DTEXTURE9 g_obstacle_texture[STAGE_MAX][OBSTACLE_MAX];
LPDIRECT3DVERTEXBUFFER9 g_obstacle_vtx;


//****************************************************************************
// �֐���:	HRESULT InitStage(void)
// ����:	�Ȃ�
// �߂�l:	�������̐����E���s
// ����:	�X�e�[�W������
//*****************************************************************************
HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �t�@�C������X�e�[�W���Ǎ�
	ReadStageData();

	// ��Q���e�N�X�`���̓ǂݍ���
	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++)
	{
		for (int obstacle_no = 0; obstacle_no < OBSTACLE_MAX; obstacle_no++)
		{
			char filename[MAX_PATH] = "data/TEXTURE/obstacle_";	
			// �X�e�[�W�^�C�v�Ə�Q���^�C�v�ɉ����ăe�N�X�`���̕������A��
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

			D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				filename,		// �t�@�C���̖��O
				&g_obstacle_texture[stage_no][obstacle_no]);	// �ǂݍ��ރ������[
		}
	}

	// ���[���e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_LANE, &g_lane_tex)))
	{
		return E_FAIL;
	}

	// ���_�쐬
	MakeVertex(pDevice, &g_obstacle_vtx, &D3DXVECTOR3(0.0f, OBSTACLE_HEIGHT / 2.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
	MakeVertex(pDevice, &g_lane_vtx, NULL, LANE_WIDTH, LANE_HEIGHT);
	MakeVertex(pDevice, &g_count_vtx, NULL, COUNT_WIDTH, COUNT_HEIGHT);	// �J�E���g�_�E���p

	// �ŏ��̓J�E���g�_�E������
	g_stage_state = STAGE_STATE_COUNTDOWN;

	// �ŏ��͕����X�e�[�W����
	g_stage_type = STAGE_PLAIN;

	// �X�e�[�W�̃J�E���g��0��
	g_stage_count = 0;

	// ���[��������
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		g_lane[i].use = true;
		g_lane[i].speed_factor = 1.0f;
	}

	// �X�e�[�W�ɍ��킹�ď�Q���ڒn
	SetStageObstacle(g_stage_type);


	return S_OK;
}


//****************************************************************************
// �֐���:	void UninitStage(void)
// ����:	�Ȃ�
// �߂�l:	�Ȃ�
// ����:	�X�e�[�W�I������
//****************************************************************************
void UninitStage(void)
{
	// ��Q���e�N�X�`��
	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++) {
		for (int obstacle_no = 0; obstacle_no < OBSTACLE_MAX; obstacle_no++) {
			SAFE_RELEASE(g_obstacle_texture[stage_no][obstacle_no]);
			g_obstacle[stage_no][obstacle_no].use = false;
		}
	}

	SAFE_RELEASE(g_obstacle_vtx);	// ��Q�����_
	SAFE_RELEASE(g_lane_vtx);		// ���[�����_
	SAFE_RELEASE(g_lane_tex);		// ���[���e�N�X�`��
	SAFE_RELEASE(g_count_vtx);		// �J�E���g�_�E�����_
	for (int i = 0; i < MAX_COUNTDOWN; i++)	// �J�E���g�_�E���e�N�X�`��
	{
		SAFE_RELEASE(g_count_tex[i]);
	}

	// use��false��
	for (int player_no = 0; player_no < MAX_PLAYER; player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			g_obstacle[player_no][obstacle_no].use = false;
		}
	}
}

// �X�e�[�W�X�V����
void UpdateStage(void)
{
#ifdef _DEBUG
	// TEST: ���x�ω��e�X�g
	for (int no = 0; no < MAX_PLAYER; no++) {
		if (GetKeyboardPress(DIK_I)) {
			g_lane[no].speed_factor += 0.1f;
		}
		if (GetKeyboardPress(DIK_O)) {
			g_lane[no].speed_factor -= 0.1f;
		}

		// TEST: ���W�\��
		PrintDebugProc("���x�W���F%f\n", g_lane[no].speed_factor);
		PrintDebugProc("�G���WX�F%f\n", g_obstacle[0][0].pos.x);
	}
#endif

	// �X�e�[�W�̏�Ԃɉ����ăA�b�v�f�[�g
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

// �J�E���g�_�E�����̍X�V����
void UpdateStageCountDown(void)
{
	if (!IsFading())
	{
		// �J�E���g��i�߂�
		g_stage_count++;
		// �J�E���g�_�E�����Đ�
		if (!IsPlaying(SOUND_LABEL_COUNT))
			PlaySound(SOUND_LABEL_COUNT);
	}

	if (g_stage_count >= COUNTDOWN_INTERVAL * 2)
	{
		// ���J�E���g�ŃX�^�[�g�_�b�V����Ԃ�
		g_stage_state = STAGE_STATE_STARTDASH;
		PlaySound(SOUND_LABEL_BGM000);
	}
}

// �X�^�[�g�_�b�V�����̍X�V����
void UpdateStageStartDash(void)
{
	// �_�b�V���Q�[�W�����^���̃v���C���[�ԍ����L�^����z��
	int dashing_players[MAX_PLAYER];
	int num_dashing_player = 0;

	// �_�b�V���Q�[�W�𒙂߂������v���C���[��������Q�[���J�n
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		if (IsDashGaugeFull(player_no))
		{
			dashing_players[num_dashing_player] = player_no;	// �_�b�V���Q�[�W�����^���̃v���C���[�ԍ����L�^
			num_dashing_player++;								// �_�b�V���Q�[�W�����^���̃v���C���[��
		}
	}

	if (num_dashing_player != 0)	// �_�b�V���Q�[�W�����^���̃v���C���[��������
	{
		// ���̒����烉���_���Ō��肵�X�L����������������
		srand((unsigned int)time(NULL));			// ����������
		int idx = rand() % num_dashing_player;
		int skill_player_no = dashing_players[idx];
		skillwinner(skill_player_no);

		g_stage_state = STAGE_STATE_GAMEPLAY;	// �Q�[���{�҂Ɉڍs
		PlaySound(SOUND_LABEL_BGM000);			// BGM�Đ��J�n
	}
}

// �Q�[���{�҂̍X�V����
void UpdateStageGameplay(void)
{
	if (g_stage_count >= MAX_CHANGE_STAGE_COUNT)
	{
		g_stage_count = 0;	// �X�e�[�W�̃J�E���g���Z�b�g

		// ���̃X�e�[�W�֕ύX
		g_stage_type = (STAGE_TYPE)((g_stage_type + 1) % STAGE_MAX);
		SetStageObstacle(g_stage_type);
	}

	// �X�e�[�W�̃J�E���g����
	g_stage_count++;

	// ��Q���̈ړ�����
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			OBSTACLE *obstacle = &g_obstacle[player_no][obstacle_no];
			if (!obstacle->use)
				continue;

			obstacle->pos += obstacle->move * g_lane[obstacle->lane_no].speed_factor;


			PLAYER *player = GetPlayer(player_no);
			// �v���C���[����Q�����щz����������
			// �v���C���[�����G��ԂłȂ� ���� ��Q�����X�L���|�C���g�𖢕t�^
			if ((obstacle->pos.x > JUMP_OVER_DECISION_POS_X) && obstacle->should_give_skillpoint && !player->is_invincible)
			{
			
				// �X�L���|�C���g���グ��
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
			bool is_offscreen = IsObjectOffscreen(worldBox);	// ���S�ɉ�ʊO�ɂ���Ȃ�true

			// ��ʊO�ɂ��茻�݂̃X�e�[�W�ɏ������Ă��Ȃ��I�u�W�F�N�g�͏���
			if (is_offscreen && (g_obstacle[player_no][obstacle_no].stage != g_stage_type))
			{
				g_obstacle[player_no][obstacle_no].use = false;
			}

			// ��ʉE�O�ɏo��������
			if (is_offscreen && (g_obstacle[player_no][obstacle_no].pos.x > 0.0f))
			{
					// ���[���̒������߂�
					g_obstacle[player_no][obstacle_no].pos.x -= g_stage_data[g_stage_type].lane_length;
					// �X�L���|�C���g��^������悤��
					g_obstacle[player_no][obstacle_no].should_give_skillpoint = true;
			}
		}
	}
}

void ChangeStageState(STAGE_STATE next_stage_state)
{
	// �X�e�[�W�J�E���g�����Z�b�g
	g_stage_count = 0;
}

void DrawStage(void)
{
	// ���[���`��
	DrawLane();

	// ��Q���`��
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		for (int obstacle_no = 0; obstacle_no < MAX_NUM_OBSTACLE; obstacle_no++)
		{
			if (!g_obstacle[player_no][obstacle_no].use)
				continue;

			DrawObstacle(&g_obstacle[player_no][obstacle_no]);
		}
	}

	// �J�E���g�_�E���`��
	//DrawCountDown();
}

void DrawCountDown()
{
	// �J�E���g�_�E����ԂȂ�J�E���g�`��
	//if (g_stage_state == STAGE_STATE_COUNTDOWN)
	//{
	//	if (g_stage_count >= COUNTDOWN_INTERVAL * 2)	
	//	{// '1'�`��
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

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_obstacle_vtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, obstacle->rot.y, obstacle->rot.x, obstacle->rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, obstacle->pos.x, obstacle->pos.y, obstacle->pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, obstacle->texture);

	// �`��
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
	// �t�@�C���I�[�v��
	FILE *file = fopen(STAGE_FILE, "r");

	for (int i = 0; i < STAGE_MAX; i++) {
		char stage_name[MAX_PATH];
		fscanf(file, "%[^,],", stage_name);
		int stage_no;
		if (strcmp(stage_name, "����") == 0) {
			stage_no = STAGE_PLAIN;
		}
		else if (strcmp(stage_name, "����") == 0) {
			stage_no = STAGE_DESERT;
		}
		else if (strcmp(stage_name, "�ΎR") == 0) {
			stage_no = STAGE_VOLCANO;
		}
		else if (strcmp(stage_name, "��") == 0) {
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

	// �t�@�C���N���[�Y
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
			if (tile == CHAR_SPACE)	// �X�y�[�X�Ȃ牽���u���Ȃ�
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
				assert(!"�s���ȃL�����N�^�[");
				break;
			}

			// ��Q���Z�b�g
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

// ���݂̃X�e�[�W���擾
STAGE_TYPE GetStage(void)
{
	return g_stage_type;
}

STAGE_STATE GetStageState(void)
{
	return g_stage_state;
}