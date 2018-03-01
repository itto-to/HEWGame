//=============================================================================
//
// �X�e�[�W���� [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include <stdio.h>

#include "input.h"
#include "camera.h"
#include "collision.h"
#include "debugproc.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGE_FILE		"data/stage.csv"
#define TEXTURE_OBSTACLE "data/TEXTURE/���V_����_�X���C�f�B���O.png"
#define MAX_OBSTACLE	(256)
#define OBSTACLE_WIDTH	(100)
#define OBSTACLE_HEIGHT	(100)
#define OBSTACLE_DEFAULT_MOVE	(D3DXVECTOR3(10.0f, 0.0f, 0.0f))


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ReadStageData(void);
void SetStageData(STAGE stage);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
STAGE g_stage;		// ���݂̃X�e�[�W
LANE g_lane[MAX_PLAYER];
OBSTACLE g_obstacle;
LANE_DATA g_lane_data[STAGE_MAX];


HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �t�@�C������X�e�[�W���Ǎ�
	ReadStageData();

	// ���[��������
	for (int no = 0; no < MAX_PLAYER; no++)
	{
		g_lane[no].use = true;
		g_lane[no].speed_factor = 1.0f;

		g_obstacle.pos = D3DXVECTOR3(-SCREEN_WIDTH / 2.0f, LANE_Y(no), LANE_Z(no));
		g_obstacle.move = OBSTACLE_DEFAULT_MOVE;
		g_obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_obstacle.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_obstacle.lane_no = 0;

		// �����蔻�菉����
		InitBoundingBox(&g_obstacle.hit_box, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);

		// ��ʊO���菉����
		InitBoundingBox(&g_obstacle.screen_box, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);
	}

	// ���_�쐬
	MakeVertex(pDevice, &g_obstacle.vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_OBSTACLE,		// �t�@�C���̖��O
		&g_obstacle.texture);	// �ǂݍ��ރ������[



	return S_OK;
}

void UninitStage(void)
{
	SAFE_RELEASE(g_obstacle.vtx);
	SAFE_RELEASE(g_obstacle.texture);
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
		PrintDebugProc("���x�W���F%f", g_lane[no].speed_factor);
	}
#endif

	// ��Q���̈ړ�����
	g_obstacle.pos += g_obstacle.move * g_lane[g_obstacle.lane_no].speed_factor;

	// ��ʊO�ɏo����ʒu���Z�b�g
	BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle.hit_box, g_obstacle.pos);
	if (IsObjectOffscreen(worldBox))
	{
		g_obstacle.pos.x = -SCREEN_WIDTH / 2.0f;
	}
}

void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_obstacle.vtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_obstacle.rot.y, g_obstacle.rot.x, g_obstacle.rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_obstacle.pos.x, g_obstacle.pos.y, g_obstacle.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_obstacle.texture);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

#ifdef _DEBUG
	BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle.hit_box, g_obstacle.pos);
	DrawDebugBoundingBox(worldBox);
#endif
}


LANE *GetLane(int no) {
	return &g_lane[no];
}

OBSTACLE *GetObstacle(void)
{
	return &g_obstacle;
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
				fscanf(file, "%[^\n]\n");
				break;
			}
			else if (no == '0')
			{
				g_lane_data[stage_no].tile[tile_no] = 0;
			}
			else if (no == '1')
			{
				g_lane_data[stage_no].tile[tile_no] = 1;
			}
			else if (no == '2')
			{
				g_lane_data[stage_no].tile[tile_no] = 2;
			}
			else if (no == '3')
			{
				g_lane_data[stage_no].tile[tile_no] = 3;
			}
			tile_no++;

			char c = fgetc(file);

			if (c == '\n' || c == EOF)
			{
				int a = 0;
				break;
			}
		}
		g_lane_data[stage_no].length = tile_no;
	}

	// �t�@�C���N���[�Y
	if (file)
		fclose(file);
}

void SetStageData(STAGE stage)
{
	for (int i = 0; i < g_lane_data[stage].length; i++)
	{
		g_lane_data[stage];
	}
}
