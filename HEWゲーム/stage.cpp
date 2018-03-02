//=============================================================================
//
// �X�e�[�W���� [stage.cpp]
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
// �}�N����`
//*****************************************************************************
#define STAGE_FILE		"data/stage.csv"

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
// �v���g�^�C�v�錾
//*****************************************************************************
void DrawObstacle(OBSTACLE *obstacle);
void ReadStageData(void);
void SetStageData(STAGE stage);
void SetObstacle(STAGE stage);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

STAGE g_stage;		// ���݂̃X�e�[�W
LANE g_lane[MAX_PLAYER];
OBSTACLE g_obstacle[MAX_PLAYER][MAX_NUM_OBSTACLE];
STAGE_DATA g_stage_data[STAGE_MAX];
LPDIRECT3DTEXTURE9 g_texture[STAGE_MAX][OBSTACLE_MAX];
LPDIRECT3DVERTEXBUFFER9 g_vtx;


HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �t�@�C������X�e�[�W���Ǎ�
	ReadStageData();

	// �e�N�X�`���̓ǂݍ���
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

			D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				filename,		// �t�@�C���̖��O
				&g_texture[stage_no][obstacle_no]);	// �ǂݍ��ރ������[
		}
	}

	// ���_�쐬
	MakeVertex(pDevice, &g_vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);

	// �ŏ��͕����X�e�[�W����
	g_stage = STAGE_PLAIN;

	// ���[��������
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		g_lane[i].use = true;
		g_lane[i].speed_factor = 1.0f;
	}

	// �X�e�[�W�ɍ��킹�ď�Q���ڒn
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
			// ��ʉE�O�ɏo��������
			if (IsObjectOffscreen(worldBox) && (g_obstacle[player_no][obstacle_no].pos.x > 0.0f))
			{	
				// ���݂̃X�e�[�W�Ɠ����Ȃ烊�Z�b�g
				if (g_obstacle[player_no][obstacle_no].stage == g_stage)
				{
					// ���[���̒������߂�
					g_obstacle[player_no][obstacle_no].pos.x -= g_stage_data[g_stage].lane_length;
					// �X�L���|�C���g��^������悤��
					g_obstacle[player_no][obstacle_no].should_give_skillpoint = true;
				}
				else // ���݂̃X�e�[�W�ƈႤ�Ȃ����
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

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, obstacle->vtx, 0, sizeof(VERTEX_3D));

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
				assert(!"�s���ȃL�����N�^�[");
				break;
			}

			// ��Q���Z�b�g
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