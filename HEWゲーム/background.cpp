//=============================================================================
//
// �w�i���� [background.cpp]
// Author : 
//
//=============================================================================
#include "background.h"

#include "debugproc.h"
#include "mesh.h"
#include "stage.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_PATH			"data/TEXTURE/"
#define TEXTURE_BG_PLAIN		"bg_plain.png"
#define TEXTURE_BG_DESERT		"bg_desert.jpg"
#define TEXTURE_BG_VOLCANO		"bg_volcano.jpg"
#define TEXTURE_BG_SNOW			"bg_snow.jpg"



//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {
	LPDIRECT3DVERTEXBUFFER9 vtx;	// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9 texture;		// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 pos;				// ���W
} BACKGROUND;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
BACKGROUND g_background;
LPDIRECT3DTEXTURE9 g_bg_texture[STAGE_MAX];		// �e�N�X�`���̃|�C���^


HRESULT InitBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_background.pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);

	// ���_�쐬
	MakeVertex(pDevice, &g_background.vtx, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);

	// �e�N�X�`���쐬
	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++)
	{
		char texture_filename[MAX_PATH] = TEXTURE_PATH;

		switch ((STAGE_TYPE)stage_no)
		{
		case STAGE_PLAIN:
			strcat(texture_filename, TEXTURE_BG_PLAIN);
			break;
		case STAGE_DESERT:
			strcat(texture_filename, TEXTURE_BG_DESERT);
			break;
		case STAGE_VOLCANO:
			strcat(texture_filename, TEXTURE_BG_VOLCANO);
			break;
		case STAGE_SNOW:
			strcat(texture_filename, TEXTURE_BG_SNOW);
			break;
		}

		if (FAILED(D3DXCreateTextureFromFile(pDevice, texture_filename, &g_bg_texture[stage_no])))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void UninitBackground()
{
	SAFE_RELEASE(g_background.vtx);

	for (int stage_no = 0; stage_no < STAGE_MAX; stage_no++)
	{
		SAFE_RELEASE(g_bg_texture[stage_no]);
	}
}


void UpdateBackground()
{
	STAGE_TYPE stage = GetStage();
	g_background.texture = g_bg_texture[stage];

#ifdef _DEBUG
	PrintDebugProc("***�w�i***\n");
	PrintDebugProc("���W�F X %f Y%f Z%f\n", g_background.pos.x, g_background.pos.y, g_background.pos.z);
#endif
}

void DrawBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_background.vtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0, 0, 0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_background.pos.x, g_background.pos.y, g_background.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_background.texture);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
