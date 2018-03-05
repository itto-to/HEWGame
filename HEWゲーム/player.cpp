//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"

#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "game.h"
#include "item.h"
#include "mesh.h"
#include "player_behavior.h"
#include "score.h"
#include "sound.h"
#include "skill.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BUTTON_SKILL				(BUTTON_C)	// �X�L�������{�^��
#define KEY_SKILL					(DIK_C)		// �X�L�������L�[

#define TEXTURE_PLAYER_KNIGHT		"data/TEXTURE/player_knight.png"	// �R�m�e�N�X�`����
#define TEXTURE_PLAYER_THIEF		"data/TEXTURE/player_thief.png"		// �����e�N�X�`����
#define TEXTURE_PLAYER_COOK			"data/TEXTURE/player_cook.png"		// �����l�e�N�X�`����
#define TEXTURE_PLAYER_WIZARD		"data/TEXTURE/player_wizard.png"	// ���@�g���e�N�X�`����
#define TEXTURE_DASH_GAUGE			"data/TEXTURE/lifegreen.png"		// �_�b�V���Q�[�W�e�N�X�`����

#define PLAYER_POS_X		(400.0f)					// �v���C���[��X���W
#define	PLAYER_PADDING		(-100.0f)					// �v���C���[���m�̊Ԋu
#define	VALUE_MOVE_PLAYER	(0.155f)					// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.10f)						// ��]�����W��
#define	VALUE_MOVE_BULLET	(7.5f)						// �e�̈ړ����x
#define GRAVITY_ACCELARATION (-0.5f)					// �d�͉����x
#define MAX_LIFE			(3)							// �ő�̗�
#define INVINCIBLE_FRAME	(100)						// �G�Ɠ���������̖��G�t���[��

#define OFFSET_DASH_GAUGE		(D3DXVECTOR3(0.0f, 100.0f, 0.0f))	// �_�b�V���Q�[�W�̃v���C���[����̑��΍��W
#define MAX_DASH_GAUGE			(16.0f)		// �_�b�V���Q�[�W�̍ő�l
#define DASH_GAUGE_HEIGHT		(20.0f)		// �_�b�V���Q�[�W�̍���
#define MAX_DASH_GAUGE_WIDTH	(100.0f)	// �_�b�V���Q�[�W�̍ő啝


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureKnight;		// �e�N�X�`���ǂݍ��ݏꏊ
int					g_num_player;				// �v���C���[�̐l��
PLAYER				g_playerWk[MAX_PLAYER];		// �v���C���[���[�N

LPDIRECT3DVERTEXBUFFER9 g_vtx_dash_gauge;
LPDIRECT3DTEXTURE9 g_texture_dash_gauge;

char *player_textureFileName[MAX_PLAYER] =
{
	TEXTURE_PLAYER_KNIGHT,
	TEXTURE_PLAYER_THIEF,
	TEXTURE_PLAYER_COOK,
	TEXTURE_PLAYER_WIZARD,
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v���C�l���ݒ�
	g_num_player = MAX_PLAYER;

	for(int no = 0; no < MAX_PLAYER; no++)
	{
		g_playerWk[no].texture = NULL;

		g_playerWk[no].lane_no = no;
		g_playerWk[no].skillpoint = 0;
		g_playerWk[no].pos     = D3DXVECTOR3(PLAYER_POS_X, LANE_Y(no), LANE_Z(no));
		g_playerWk[no].move    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[no].rot     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[no].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[no].scl     = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_playerWk[no].ground  = g_playerWk[no].pos.y;
		g_playerWk[no].life    = MAX_LIFE;
		g_playerWk[no].state   = PLAYER_STARTDASH;
		g_playerWk[no].speed_factor = 1.0f;
		g_playerWk[no].dash_gauge = 0.0f;
		g_playerWk[no].kengen = false;
		// �����蔻�菉����
		g_playerWk[no].hit_box = PLAYER_HIT_BOX;

		// ��ʊO����p�o�E���f�B���O�{�b�N�X������
		// ��ʊO���菉����
		InitBoundingBox(&g_playerWk[no].screen_box, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f);

		// ���_�쐬
		MakeVertex(pDevice, &g_playerWk[no].vtx, &D3DXVECTOR3(0.0f, PLAYER_HEIGHT / 2.0f, 0.0f), PLAYER_WIDTH, PLAYER_HEIGHT);
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_KNIGHT,			// �t�@�C���̖��O
		&g_playerWk[KNIGHT].texture);		// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_WIZARD,			// �t�@�C���̖��O
		&g_playerWk[WIZARD].texture);		// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_THIEF,			// �t�@�C���̖��O
		&g_playerWk[THIEF].texture);		// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_COOK,			// �t�@�C���̖��O
		&g_playerWk[COOK].texture);		// �ǂݍ��ރ������[

	// �_�b�V���Q�[�W�̒��_�쐬
	MakeVertex(pDevice, &g_vtx_dash_gauge, &D3DXVECTOR3(MAX_DASH_GAUGE_WIDTH / MAX_DASH_GAUGE / 2, 0.0f, 0.0f), MAX_DASH_GAUGE_WIDTH / MAX_DASH_GAUGE, DASH_GAUGE_HEIGHT);
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_DASH_GAUGE,			// �t�@�C���̖��O
		&g_texture_dash_gauge);		// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		// �e�N�X�`���̊J��
		SAFE_RELEASE(g_playerWk[no].texture);

		// ���b�V���̊J��
		SAFE_RELEASE(g_playerWk[no].vtx);
	}

	SAFE_RELEASE(g_vtx_dash_gauge);
	SAFE_RELEASE(g_texture_dash_gauge);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	SKILL *skillWk = GetSkillWk(0);

	for(int no = 0; no < MAX_PLAYER; no++)
	{
		switch (g_playerWk[no].state)
		{
		case PLAYER_STARTDASH:
			UpdatePlayerStartDash(&g_playerWk[no]);
			break;

		case PLAYER_ONGROUND:
			UpdatePlayerOnGround(&g_playerWk[no]);
			break;

		case PLAYER_JUMP:
			UpdatePlayerJump(&g_playerWk[no]);
			break;

		case PLAYER_SLIDING:
			UpdatePlayerSliding(&g_playerWk[no]);
			break;

		case PLAYER_DEAD:
			UpdatePlayerDead(&g_playerWk[no]);
			break;

		default:
			break;
		}

#ifdef _DEBUG
		//for (int i = 0; i < MAX_PLAYER; i++)
		//{
		//	if (GetKeyboardPress(DIK_DOWN)) {

		//		g_playerWk[i].pos.y -= 1.0f;
		//		g_playerWk[i].ground -= 1.0f;
		//	}
		//	if (GetKeyboardPress(DIK_UP)) {
		//		g_playerWk[i].pos.y += 1.0f;
		//		g_playerWk[i].ground += 1.0f;

		//	}
		//}

		//float interval = g_playerWk[0].pos.y - g_playerWk[1].pos.y;
		//for (int i = 1; i < MAX_PLAYER; i++)
		//{
		//	if (GetKeyboardPress(DIK_W))
		//	{
		//		g_playerWk[i].pos.y = g_playerWk[i - 1].pos.y - interval - 1.0f;
		//		g_playerWk[i].ground = g_playerWk[i].pos.y;
		//	}
		//	if (GetKeyboardPress(DIK_N))
		//	{
		//		g_playerWk[i].pos.y = g_playerWk[i - 1].pos.y - interval + 1.0f;
		//		g_playerWk[i].ground = g_playerWk[i].pos.y;
		//	}
		//}
#endif

		// �X�L������
		if (IsButtonTriggered(no, BUTTON_SKILL) || GetKeyboardTrigger(KEY_SKILL)) {
			// �܂�5����ł���̂����m�F
			//if(g_playerWk[no].skillpoint >= 5)
			{
				skillWk->lv = 3;
				//5����ł����ꍇ�X�L������
				SkillAct(no);
			}
		}

		// ���S����
		if (g_playerWk[no].life <= 0)
		{
			g_playerWk[no].next_state = PLAYER_DEAD;
		}


		// ���G�J�E���g�����ȏ�Ȃ猳�̏�Ԃ�
		if (g_playerWk[no].is_invincible)
		{
			g_playerWk[no].invincible_counter++;
			if (g_playerWk[no].invincible_counter > INVINCIBLE_FRAME)
			{
				g_playerWk[no].is_invincible = false;
			}
		}

		// �X�e�[�g�ύX
		ChangePlayerState(&g_playerWk[no]);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;

	for (int no = 0; no < MAX_PLAYER; no++)
	{
		PLAYER *player = &g_playerWk[no];

		// �v���C���[�`��
		if (!g_playerWk[no].is_invincible || ((g_playerWk[no].invincible_counter % 10) > 4))
		{
			DrawMesh(g_playerWk[no].vtx, g_playerWk[no].texture, g_playerWk[no].pos, g_playerWk[no].rot, g_playerWk[no].scl);
		}

		// �X�^�[�g���̃Q�[�W�`��
		if (g_playerWk[no].state == PLAYER_STARTDASH)
		{
			DrawMesh(g_vtx_dash_gauge, g_texture_dash_gauge, player->pos + OFFSET_DASH_GAUGE, player->rot, D3DXVECTOR3(player->dash_gauge, 1.0f, 1.0f));
		}


#ifdef _DEBUG
		// �o�E���f�B���O�{�b�N�X��`��
		BOUNDING_BOX worldBox = ToWorldBoundingBox(g_playerWk[no].hit_box, g_playerWk[no].pos);
		DrawDebugBoundingBox(worldBox);

		// �f�o�b�O����\��
		D3DXVECTOR3 pos = g_playerWk[no].pos;
		PrintDebugProc("***�v���C���[%d��***\n", no);
		PrintDebugProc("���W X�F%f Y:%f Z:%f\n", pos.x, pos.y, pos.z);
		PrintDebugProc("���C�t : %d\n", g_playerWk[no].life);
		PrintDebugProc("�X�L���|�C���g : %d \n", g_playerWk[no].skillpoint);
		PrintDebugProc("�_�b�V���Q�[�W : %f \n", g_playerWk[no].dash_gauge);

		switch (g_playerWk[no].state)
		{
		case PLAYER_ONGROUND:
			PrintDebugProc("��ԁF%s", "���n\n");
			break;

		case PLAYER_JUMP:
			PrintDebugProc("��ԁF%s", "�W�����v\n");
			break;

		case PLAYER_DEAD:
			PrintDebugProc("��ԁF%s", "���S\n");
			break;
		}

		if (g_playerWk[no].is_invincible) {
			PrintDebugProc("���G\n");
		}
		else {
			PrintDebugProc("���G����Ȃ�\n");
		}

#endif
	}
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&player->vtx,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		player->vtx->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-HALF_PLAYER_WIDTH,  HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3( HALF_PLAYER_WIDTH,  HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3( HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, 0.0f);

		// �@���̐ݒ�
		pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		player->vtx->Unlock();
	}

	return S_OK;
}


//=============================================================================
// �v���C���[���擾
//=============================================================================
PLAYER *GetPlayer(int no)
{
	return &g_playerWk[no];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_playerWk[0].pos;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_playerWk[0].rot;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_playerWk[0].rotDest;
}

//=============================================================================
// �ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_playerWk[0].move;
}

//=============================================================================
// ���݂̃v���C���[�l���擾
//=============================================================================
int NumPlayer(void)
{
	return g_num_player;
}

//******************************************************************************
// �֐���:	void IncreaseSkillpoint(int player_no)
// ����:	int �v���C���[�ԍ�
// �߂�l:	void
// ����:	�w�肵���v���C���[�̃X�L���|�C���g�𑝉�
//******************************************************************************
void IncreaseSkillpoint(int player_no)
{
	g_playerWk[player_no].skillpoint++;
}

//******************************************************************************
// �֐���:	int MaxLife(void)
// ����:	void
// �߂�l:	void
// ����:	���C�t�̍ő�l
//******************************************************************************
int MaxLife(void)
{
	return MAX_LIFE;
}

//******************************************************************************
// �֐���:	bool IsDashGaugeFull(int player_no)
// ����:	int player_no
// �߂�l:	bool
// ����:	player_no�Ŏw�肵���v���C���[�̃_�b�V���Q�[�W�����^���Ȃ�true��Ԃ�
//******************************************************************************
bool IsDashGaugeFull(int player_no)
{
	return g_playerWk[player_no].dash_gauge >= MAX_DASH_GAUGE;
}

//******************************************************************************
// �֐���:	bool IsGameSet(void)
// ����:	�Ȃ�
// �߂�l:	bool
// ����:	�����Ă���v���C���[��1�l��0�l�Ȃ�true��Ԃ�
//******************************************************************************
bool IsGameSet(void)
{
	for (int player_no = 0; player_no < g_num_player; player_no++)
	{
		if (g_playerWk[player_no].state != PLAYER_DEAD)
		{
			return false;
		}
	}
	return true;
}