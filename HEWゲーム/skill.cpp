/***************************************************************
* �t�@�C����:	skill.cpp
* �����:		HAL�����Q�[���w�� �R�{ �s��
* �ŏI�X�V��:	3��4��
* ����:			�X�L���֘A�̃v���O����
****************************************************************/

//***************************************************************
// �C���N���[�h�t�@�C��
//***************************************************************
#include "skill.h"

#include <assert.h>
#include <time.h>

#include <vector>

#include "stage.h"
#include "skillact.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "debugproc.h"
#include "mesh.h"
//***************************************************************
// �}�N����`
//***************************************************************
#define SKILL_WAKU		"data/TEXTURE/skill_frame.png"		// �X�L���Q�[�W
#define SKILL_BAR		"data/TEXTURE/lifegreen.png"		// �X�L���Q�[�W�̃o�[
//#define SKILL_BAR2
#define TEXTURE_1P				"data/TEXTURE/1P.png"
#define TEXTURE_2P				"data/TEXTURE/2P.png"
#define TEXTURE_3P				"data/TEXTURE/3P.png"
#define TEXTURE_4P				"data/TEXTURE/4P.png"


// �Q�[�W�̘g����
// ����
#define SKILLGAGE_WIDTH		(469.0f * 0.8f)
#define SKILLGAGE_HEIGHT	(98.0f * 0.8f)
// �ʒu
#define SKILLGAGE_POS	(D3DXVECTOR3(-SCREEN_WIDTH / 2.0f + 10.0f, SCREEN_HEIGHT / 2.0f - SKILLGAGE_HEIGHT / 2.0f - 10.0f, NearZ()))
#define SKILLGAGE_POS_X	(10.0f)
#define SKILLGAGE_POS_Y	(10.0f)

// �Q�[�W�̃o�[�̕���
// ����
#define SKILLBAR_WIDTH	(300.0f)
#define SKILLBAR_HEIGHT	(20.0f)
// �ʒu
#define SKILLBAR_POS	(SKILLGAGE_POS + D3DXVECTOR3(100.0f, 20.0f, 0.0f))
#define SKILLBAR_POS_X	(SKILLGAGE_POS_X + 10.0f)	// �ǂ������ɒ���
#define SKILLBAR_POS_Y	(SKILLGAGE_POS_Y + 0.0f )

// 1P�`4P�\��
// ����
#define PLAYER_NO_WIDTH		(70.0f)
#define PLAYER_NO_HEIGHT	(70.0f)
// �ʒu
#define PLAYER_NO_POS	(D3DXVECTOR3(SCREEN_LEFT + 130.0f, SCREEN_TOP - 60.0f, NearZ()))

// ���̑��Q�[�W�֘A
#define SKILL_LEVELUP	(5)			// ���x���A�b�v�ɕK�v�Ȓl
#define MAX_SKILL_LEVEL	(3)			// �ő�X�L�����x��

//
// �X�L���̉����y�ь���
#define LANESPEED_UP	(0.05f)
#define LANESPEED_DOWN	(0.1f)

// ���ꂼ��̏ꏊ
#define UPDOWNPOS_X			(10.0f)
#define UPDOWNPOS_Y			(10.0f)
#define OJYAMA_POS_X		(300.0f)
#define THANDER_POS_Y		(15.0f)



//***************************************************************
// �v���g�^�C�v�錾
//***************************************************************
std::vector<int> skillsort_life(std::vector<int> candidate_player);
std::vector<int> skill_count_winner(std::vector<int> candidate_player);
//***************************************************************
// �O���[�o���ϐ�
//***************************************************************
SKILL g_skillWk;							// �X�L���\����
SKILL_FLAG g_skill_flag[MAX_PLAYER];
bool g_skillcheck_ok;						// �X�L�������̌����������Ă���v���C���[��1�l���ǂ���
bool g_firstflag;							// �ŏ��Ɍ������������v���C���[�������������ǂ���
LPDIRECT3DTEXTURE9 g_player_no_texture[MAX_PLAYER];	// 1P�`4P�̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_player_no_vtx;			// 1P�`4P�̒��_

//***************************************************************
// �֐���:		HRESULT InitSkill(void)
// ����:		�Ȃ�
// �߂�l:		�Ȃ�
// ����:		�X�L���֘A�̏��� ��
//***************************************************************
HRESULT InitSkill(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertex(pDevice, &g_skillWk.Buff_waku, &D3DXVECTOR3(SKILLGAGE_WIDTH / 2.0f, 0.0f, 0.0f), SKILLGAGE_WIDTH, SKILLGAGE_HEIGHT);
	MakeVertex(pDevice, &g_skillWk.Buff_bar, &D3DXVECTOR3(SKILLBAR_WIDTH / 2.0f, 0.0f, 0.0f), SKILLBAR_WIDTH, SKILLBAR_HEIGHT);
	MakeVertex(pDevice, &g_player_no_vtx, NULL, PLAYER_NO_WIDTH, PLAYER_NO_HEIGHT);

	// �Q�[�W
	D3DXCreateTextureFromFile(pDevice,
		SKILL_BAR,							// �t�@�C���̖��O
		&g_skillWk.Texture_bar);

	// �Q�[�W�̘g
	D3DXCreateTextureFromFile(pDevice,
		SKILL_WAKU,
		&g_skillWk.Texture_waku);

	// 1P�`4P�e�N�X�`���ǂݍ���
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		char filename[MAX_PATH] = "data/TEXTURE/";
		char num[] = "1";
		num[0] += i;
		strcat(filename, num);
		strcat(filename, "P.png");
		if (FAILED(D3DXCreateTextureFromFile(pDevice, filename, &g_player_no_texture[i])))
		{
			return E_FAIL;
		}
	}

	// �t���O������
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		g_skill_flag[no].flag_no = 0;
		g_skill_flag[no].use_count = 0;
		g_skill_flag[no].get = false;
		g_skill_flag[no].count = 0;
	}

	// �܂������͊���U���Ă��Ȃ�
	g_skillWk.skill_player_id = -1;
	g_firstflag = false;
	// �܂��X�L���͎��s��Ԃł͂Ȃ�
	g_skillWk.moving = false;

	g_skillWk.gauge = 0.0f;
	g_skillWk.gage_lvup = 5;

	return S_OK;

}

//*************************************************************
// �֐���:	void UninitSkill(void)
// ����:	�Ȃ�
// �߂�l:
// ����:	�X�L�������̏I������
//*************************************************************
void UninitSkill(void)
{
	if(g_skillWk.Texture_bar != NULL)
	{// �e�N�X�`���J��
		g_skillWk.Texture_bar->Release();
		g_skillWk.Texture_bar = NULL;
	}

	if(g_skillWk.Buff_bar != NULL)
	{ //���_�o�b�t�@�̊J��
		g_skillWk.Buff_bar->Release();
		g_skillWk.Buff_bar = NULL;
	}

	if(g_skillWk.Texture_waku != NULL)
	{
		g_skillWk.Texture_waku->Release();
		g_skillWk.Texture_waku = NULL;
	}

	// �e�N�X�`���J��
	if(g_skillWk.Buff_waku != NULL)
	{
		g_skillWk.Buff_waku->Release();
		g_skillWk.Buff_waku = NULL;
	}

	// 1P�`4P���_�J��
	SAFE_RELEASE(g_player_no_vtx);

	// 1P�`4P�e�N�X�`���J��
	for (int i = 0; i < MAX_PLAYER; i++) {
		SAFE_RELEASE(g_player_no_texture[i]);
	}
}

//*************************************************************
// �֐���:	void AddSkillGauge(float gageup)
// ����:	float gageup
// �߂�l:	�Ȃ�
// ����:	�X�L���Q�[�W�̍X�V
//			gageup�̒l�����X�L���Q�[�W�̒l���㏸�A���l�ȏ�Ń��x���A�b�v
//****************************************************************
void AddSkillGauge(float gauge_up)
{
	bool first_skill_flag;
	bool gage_max = false;

	PLAYER *player = GetPlayer(0);
	g_skillcheck_ok = true;					// �X�L���̔��������������Ă���v���C���[��1�l���ǂ���

	// �����̒l��0��荂����Ύ��s
	assert(gauge_up > 0);

	// �����Q�[�W�����ȏ㒙�܂��Ă��� ���� �X�L�����x�����ő�łȂ��Ȃ�
	if(g_skillWk.gauge < SKILL_LEVELUP && g_skillWk.lv < MAX_SKILL_LEVEL)
	{
		// �X�L���Q�[�W�㏸
		g_skillWk.gauge += gauge_up;

		// �X�L�����x���㏸��
		if (g_skillWk.gauge >= SKILL_LEVELUP) {
			PlaySound(SOUND_LABEL_SKILL_GAGEON);
			g_skillWk.lv = min(g_skillWk.lv + 1, MAX_SKILL_LEVEL);				// ���x�����グ��
			g_skillWk.gauge = 0.0f;		// �l��������
		}
		else // �X�L���Q�[�W�����㏸���ăX�L�����x���͏オ��Ȃ���
		{
			PlaySound(SOUND_LABEL_SKILL_GAGECHARGE);
		}
	}	
}


// �X�L�����g����v���C���[�����߂�
void GiveSkillUsingRight(void)
{
	// �������������łɊ��蓖�Ă��Ă�����I��
	if (g_skillWk.skill_player_id != -1)
		return;

	// skillpoint��5�ɂȂ��Ă���v���C���[�̐l�����m�F
	std::vector<int> candidate_player;	// ���v���C���[�̔ԍ����X�g
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		PLAYER *player = GetPlayer(player_no);
		if (player->skillpoint >= 5 && IsPlayerAlive(player_no))
		{
			candidate_player.push_back(player_no);
			player->kengen = true;
			
		}
		else
		{
			player->kengen = false;
		}
	}

	if (candidate_player.size() == 0) {
		g_skillWk.skill_player_id = -1;
		return;
	}
	if (candidate_player.size() == 1)
	{
		g_skillWk.skill_player_id = candidate_player[0];
		return;
	}

	// 2�l�ȏオ���̏ꍇ�A��������ֈړ�

	// ���C�t�̏��Ȃ���
	candidate_player = skillsort_life(candidate_player);
	if (candidate_player.size() == 1)
	{
		g_skillWk.skill_player_id = candidate_player[0];
		return;
	}

	// �X�L�������񐔂����Ȃ���
	candidate_player = skill_count_winner(candidate_player);
	if (candidate_player.size() == 1)
	{
		g_skillWk.skill_player_id = candidate_player[0];
		return;
	}

	// ����ł����������烉���_���Ō���
	int rand_idx = rand() % candidate_player.size();
	g_skillWk.skill_player_id = candidate_player[rand_idx];
}

//****************************************************************
// �֐���:	void UpdateSkillAct(void)
// ����:
// �߂�l:
// ����:	�X�L���̌��ʊ֘A�̍X�V�ł�
//****************************************************************
void UpdateSkillAct(void)
{
	PLAYER *player = GetPlayer(0);
	LANE *lane = GetLane(0);
	SKILLACT *skillactWk = GetSkillAct(0);

	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// ���s��ԂȂ珈�����s��
		if(skillactWk[i].Up_active == true)
		{
			// ���s���Ԃ����炵0�ȉ��Ȃ�I��
			skillactWk[i].SpeedTime--;
			if(skillactWk[i].SpeedTime <= 0)
			{//	������ԂȂ̂Ō������Č��̑��x�ɖ߂�
				lane[i].speed_factor -= LANESPEED_UP;
				skillactWk[i].Up_active = false;
				// ���s�I���I�t���O�����ɖ߂�
				g_skillWk.moving = false;
			}
		}
		if(skillactWk[i].Down_active == true)
		{	// ���s���Ԃ����炵��0�ȉ��Ȃ�I��
			skillactWk[i].SpeedTime--;
			if(skillactWk[i].SpeedTime <= 0)
			{// ������ԂȂ̂ŉ������Č��̑��x�ɖ߂�
				lane[i].speed_factor += LANESPEED_DOWN;
				skillactWk[i].Down_active = false;
				// ���s�I���I�t���O�����ɖ߂�
				g_skillWk.moving = false;

			}
			
		}

		if(skillactWk[i].Ojyama_active == true)
		{
			// ���s���Ԃ����炵��0�ȉ��Ȃ�I��
			skillactWk[i].OjyamaTime--;

			if(skillactWk[i].OjyamaTime <= 0)
			{
				skillactWk[i].Ojyama_active = false;
				g_skillWk.moving = false;
			}
		}

		if(skillactWk[i].Kaminari_active == true)
		{
			skillactWk[i].Kaminari_pos.y -= KAMINARI_SPEED;
			//	����Y���W�ƃv���C���[��Y���W�������āA�v���C���[�̔����̍�����������Ă�����q�b�g
			if(skillactWk[i].Kaminari_pos.y - player[i].pos.y < HALF_PLAYER_HEIGHT)
			{
				// ���C�t������
				player[i].life--;
				// ���g�p�Ɂc
				skillactWk[i].Kaminari_active = false;
				g_skillWk.moving = false;

			}

		}
	}
}

//****************************************************************
// �֐���:	void DrawSkill(void)
// ����:	�Ȃ�
// �߂�l:	�Ȃ�
// ����:	�X�L���Q�[�W�̕`��
//****************************************************************
void DrawSkill(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	float Draw_Skillbar;		// �`�悳���X�L���Q�[�W�̒���

	float skillbar_ratio = (g_skillWk.gauge / g_skillWk.gage_lvup);
	// �\������o�[�̒���
	//Draw_Skillbar = (skill_hiritsu * SKILLBAR_WIDTH);

	// �g��`��
	DrawMesh(g_skillWk.Buff_waku, g_skillWk.Texture_waku, SKILLGAGE_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	// �o�[��`��
	DrawMesh(g_skillWk.Buff_bar, g_skillWk.Texture_bar, SKILLBAR_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(skillbar_ratio, 1.0f, 1.0f));
	// �X�L�������������v���C���[�\��
	if (g_skillWk.skill_player_id != -1)
	{
		DrawMesh(g_player_no_vtx, g_player_no_texture[g_skillWk.skill_player_id], PLAYER_NO_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}

	//// �X�L���o�[�̃o�b�t�@�쐬
	//if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
	//	D3DUSAGE_WRITEONLY,		// �g�p�@
	//	FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
	//	D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
	//	&skillWk.Buff_bar,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
	//	NULL)))						// NULL�ɐݒ�
	//{
	//	return E_FAIL;
	//}



	//// ���_�o�b�t�@�̒��g�𖄂߂�
	//{
	//	VERTEX_2D *pVtx;

	//	// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
	//	skillWk.Buff_bar->Lock(0, 0, (void**)&pVtx, 0);

	//	// ���_���W�̐ݒ�
	//	pVtx[0].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y, 0.0f);
	//	pVtx[1].vtx = D3DXVECTOR3(SKILLBAR_POS_X + Draw_Skillbar, SKILLBAR_POS_Y, 0.0f);
	//	pVtx[2].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);
	//	pVtx[3].vtx = D3DXVECTOR3(SKILLBAR_POS_X + Draw_Skillbar, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);

	//	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	//	pVtx[0].rhw =
	//		pVtx[1].rhw =
	//		pVtx[2].rhw =
	//		pVtx[3].rhw = 1.0f;

	//	// ���ˌ��̐ݒ�
	//	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//	// �e�N�X�`�����W�̐ݒ�
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	// ���_�f�[�^���A�����b�N
	//	skillWk.Buff_bar->Unlock();
	//}


	//// �o�[��`��
	//{
	//	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	//	pDevice->SetStreamSource(0, skillWk.Buff_bar, 0, sizeof(VERTEX_2D));
	//	// ���_�t�H�[�}�b�g�̐ݒ�
	//	pDevice->SetFVF(FVF_VERTEX_2D);
	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, skillWk.Texture_bar);
	//	// �|���S���̕`��
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	//}

#ifdef _DEBUG
	PrintDebugProc("�X�L���Q�[�W %d\n", g_skillWk.gauge);
	PrintDebugProc("�X�L�����x��%d\n", g_skillWk.lv);
#endif

}

//*****************************************************************
// �֐���:	HRESULT MakeVertexSkill(LPDIRECT3DDEVICE9 pDevice)
// ����:	LPDIRECT3DDEVICE9 pDevice
// �߂�l:
// ����		���_�̍쐬(�g����)
//******************************************************************
HRESULT MakeVertexSkill(LPDIRECT3DDEVICE9 pDevice)
{
	// �Q�[�W�̘g�����̃o�b�t�@�쐬
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,			// �g�p�@
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
		&g_skillWk.Buff_waku,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̒��g�𖄂߂�
	{
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
		g_skillWk.Buff_waku->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(SKILLGAGE_POS_X, SKILLGAGE_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SKILLGAGE_POS_X + SKILLGAGE_WIDTH, SKILLGAGE_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SKILLGAGE_POS_X, SKILLGAGE_POS_Y + SKILLGAGE_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SKILLGAGE_POS_X + SKILLGAGE_WIDTH, SKILLGAGE_POS_Y + SKILLGAGE_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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

		// ���_�f�[�^���A�����b�N
		g_skillWk.Buff_waku->Unlock();
	}

	// �X�L���o�[�̃o�b�t�@�쐬
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,		// �g�p�@
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
		&g_skillWk.Buff_bar,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̒��g�𖄂߂�
	{
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
		g_skillWk.Buff_bar->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SKILLBAR_POS_X + SKILLBAR_WIDTH, SKILLBAR_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SKILLBAR_POS_X + SKILLBAR_WIDTH, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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

		// ���_�f�[�^���A�����b�N
		g_skillWk.Buff_bar->Unlock();
	}


	return S_OK;
}

//****************************************************************************
// �֐���:	void SetColorSkill(void)
// ����:	�Ȃ�
// �߂�l:	�Ȃ�
// ����:	�f�B�t���[�Y�F�ݒ�
//*****************************************************************************
void SetColorSkill(void)
{
	// ���_�o�b�t�@�̒��g�𖄂߂�
	{
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_skillWk.Buff_waku->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N
		g_skillWk.Buff_waku->Unlock();
	}
}


//*****************************************************************************
// �֐���:	void GetSkill(void)
// ����:	5���񂾃v���C���[�̔ԍ�
// �߂�l:
// ����:	updateskill�Ō����������Ă���v���C���[��2�l�ȏ�̏ꍇ���s�A1�l�̃v���C���[�Ɍ�����^����
//			[Get�֐��ł͂Ȃ��ł��I]
//*****************************************************************************
//void GetSkill(void)
//{
//	PLAYER *player = GetPlayer(0);
//	int skill_life_lower;
//	int skill_count_lower;
//	int counts = 0;
//	srand((unsigned)time(NULL));
//	int unmakase = 0;						// 0�ŏ�����
//
//	// ������2�l�ȏオ�����𖞂����Ă����牺�L�̔���
//
//	// ���C�t�����Ȃ��l
//	skill_life_lower = skillsort_life();
//
//	// ��L�������������L�҂�2�l�ȏア���ꍇ�̏���
//	if(g_skillcheck_ok == false)
//	{
//		// �������������l����ԏ��Ȃ�����
//		skill_count_lower = skill_count_winner();
//	}
//	else
//	{
//		g_skillWk.skill_player_id = true;
//	}
//
//
//	// ����ł��ʖڂȏꍇ�̏���
//	if(g_skillcheck_ok == false)
//	{
//		while(0)
//		{
//			srand((unsigned)time(NULL));
//			unmakase = rand() % MAX_PLAYER;			// �K���ȂƂ�����J�n
//			if(player[unmakase].kengen == true)
//			{
//				break;
//				g_skillcheck_ok = true;
//				g_skillWk.skill_player_id = true;
//			}
//		}
//
//		// ����ȊO�̃v���C���[�͌������D�y�у|�C���g���Z�b�g
//		for(int i = 0; i < MAX_PLAYER; i++)
//		{
//			if(player[i].kengen == true && i != unmakase)
//			{
//				player[i].kengen = false;
//				SkillReset(i);
//			}
//		}
//	}
//
//
//
//}


//******************************************************************************
// �֐���:	std::vector<int> skillsort_life(std::vector<int> candidate_player)
// ����:	�Ȃ�
// �߂�l:	int winner
// ����:	�v���C���[�̗̑͂��r���Ĉ�ԃ��C�t�����Ȃ��v���C���[�̔ԍ����X�g��Ԃ�
//******************************************************************************
std::vector<int> skillsort_life(std::vector<int> candidate_player)
{
	std::vector<int> result_candidate_player;	// ���ʗp
	int min_life = INT_MAX;						// �ł����Ȃ��̗͒l
	for (auto player_no : candidate_player)
	{
		PLAYER *player = GetPlayer(player_no);
		if (min_life > player->life)
		{
			result_candidate_player.clear();				// ���܂ł̌����N���A����
			result_candidate_player.push_back(player_no);	// �v���C���[�o�^
			min_life = player->life;	// �ŏ��l�X�V
		}
		else if (min_life == player->life)
		{
			result_candidate_player.push_back(player_no);
		}
	}

	return result_candidate_player;
	//PLAYER *player = GetPlayer(0);
	//int winner = 0;					// ���҂̔ԍ�
	//bool sort_life = true;			// ���̕��@�Ńv���C���[����l�ɍi�ꂽ���@

	//								// ���̓��C�t�������ꍇ�A�ԍ����Ⴂ�v���C���[�������𓾂�

	//for(int no = 1; no < MAX_PLAYER; no++)
	//{
	//	// ��݂��̗̑͂������ꍇ
	//	if(player[winner].life == player[no].life)
	//	{
	//		sort_life = false;
	//	}

	//	// ���݂̏��҂̗̑͂�菭�Ȃ��l���ł���
	//	if(player[winner].life > player[no].life)
	//	{
	//		player[winner].kengen = false;				// ����������
	//		SkillReset(winner);							// �|�C���g������
	//		winner = no;								// ���̐l��winner��
	//	}




	//}
}

//******************************************************************
// �֐���:	std::vector<int> skill_count_winner(std::vector<int> candidate_player)
// ����:	�Ȃ�
// �߂�l:	count_winner
// ����:	�X�L���̔����񐔂����Ȃ��l�̔ԍ����X�g��Ԃ�
//******************************************************************
std::vector<int> skill_count_winner(std::vector<int> candidate_player)
{
	std::vector<int> result_candidate_player;
	int min_skill_count = INT_MAX;	// �X�L���g�p�񐔂̍ŏ��l
	for (auto player_no : candidate_player)
	{
		if (min_skill_count > g_skill_flag[player_no].use_count)
		{
			result_candidate_player.clear();				// ���܂ł̃v���C���[���폜����
			result_candidate_player.push_back(player_no);	// �V�����o�^
			min_skill_count = g_skill_flag[player_no].use_count;
		}
		else if (min_skill_count == g_skill_flag[player_no].use_count)
		{
			result_candidate_player.push_back(player_no);	// �ǉ��o�^
		}
	}

	return result_candidate_player;

	//PLAYER *player = GetPlayer(0);
	//int count_winner = 0;				// 0�Ԗڂ����r�J�n
	//int shaka = 0;						// ����������T���Ƃ��Ɏg���֐�
	//g_skillcheck_ok = true;				// ��������Ɖ���

	//									// �Ⴂ�����猩�Ă����Č����������Ă���v���C���[��T��
	//while(shaka = 0)
	//{
	//	if(player[count_winner].kengen == false)
	//	{
	//		count_winner++;
	//	}
	//	else
	//	{
	//		shaka++;
	//	}
	//}

	//for(int i = 1; i < MAX_PLAYER; i++)	// 0�Ԗڂ�1�Ԗڂ����r�J�n
	//{
	//	if(g_skill_flag[count_winner].count == g_skill_flag[i].count)
	//	{
	//		g_skillcheck_ok = false;			// 2�l�ȏア��I
	//	}
	//	else if(g_skill_flag[count_winner].count > g_skill_flag[i].count)
	//	{// ���������������l����������
	//		player[count_winner].kengen = false;		// �������͂��D����
	//		SkillReset(count_winner);					// �|�C���g�����Z�b�g
	//		count_winner = i;							// ���҂��c��
	//	}
	//}
	//return count_winner;
}

//******************************************************************************
// �X�L���|�C���g���Z�b�g
// ����:	int no
// ����:	�v���C���[�̔ԍ����󂯎���āA���̐l�̃|�C���g��0�ɂ����Ⴄ
//******************************************************************************
void SkillReset(int no)
{
	PLAYER *player = GetPlayer(0);
	player[no].skillpoint = 0;
}

//************************************************************************************************
// �֐���:	void SkillAct(int player_no)
// ����:	int player_no(�����������Ă���v���C���[�̔ԍ��j
// �߂�l:	�Ȃ�
// ����:	�X�L������
//			�����������Ă���v���C���[�̔ԍ����󂯎����ʔ���
//			SetSkillAct����POS�𒲐�����Ɖ�ʓ��̈ʒu���ς��c�͂�
//************************************************************************************************
void SkillAct(int player_no)
{
	PLAYER *player = GetPlayer(0);
	LANE *lane = GetLane(0);
	srand((unsigned)time(NULL));
	int randum;



	// �X�L���������̏ꍇ���s����Ȃ�
	if(g_skillWk.moving != true && g_skillWk.lv > 0)
	{
		// �X�L�������񐔑���
		g_skill_flag[player_no].use_count++;
		// ���ʔ������Ɍ���������
		g_skillWk.skill_player_id = -1;
		player[player_no].kengen = false;
		SkillReset(player_no);
		g_skillWk.moving = true;

		// ���ʂ̔���
		// �e�v���C���[���ƂɌ��ʂ𔭓����Ă���
		for(int i = 0; i < MAX_PLAYER; i++)
		{
			// �X�L�������҂͂͂���
			if(i != player_no)
			{
				// �X�L�����x���ŕ��򂳂�����
				switch(g_skillWk.lv)
				{
				case SPEEDCHANGE:
					// ����
					randum = rand() % 2;
					switch(randum)
					{
					case 0:

						// ����
						lane[i].speed_factor += LANESPEED_UP;
						SetSkillAct(D3DXVECTOR3(player[i].pos.x- UPDOWNPOS_X, player[i].pos.y+UPDOWNPOS_Y, player[i].pos.z),
							EFFECT_UP, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						// ���炷
						PlaySound(SOUND_LABEL_SKILL_SPEEDUP);
						// ���Ԑݒ�

						break;
					case 1:
						// ����
						lane[i].speed_factor -= LANESPEED_DOWN;
						SetSkillAct(D3DXVECTOR3(player[i].pos.x-UPDOWNPOS_X, player[i].pos.y, player[i].pos.z),
							EFFECT_UP, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						// ���炷
						PlaySound(SOUND_LABEL_SKILL_SPEEDDOWN);
						break;
					}
					break;

				case OJYAMA:
					// ����
					// �u���b�N�̕\���̂�
					// ���炷
					PlaySound(SOUND_LABEL_SKILL_OJYAMA);
					SetSkillAct(player[i].pos, EFFECT_OJYAMA, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;

				case KAMINARI:
					// ����
					// ���炷
					PlaySound(SOUND_LABEL_SKILL_THANDER);
					// ���C�t�����͗��ƃv���C���[���Ԃ������Ƃ���
					SetSkillAct(D3DXVECTOR3(player[i].pos.x, player[i].pos.y+ THANDER_POS_Y, player[i].pos.z),
						EFFECT_KAMINARI, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				}
			}
		}

		// �X�L�����x�������Z�b�g
		g_skillWk.lv = 0;
	}

}

//****************************************************
// void skillwinner(int no)
// �^����ꂽ�ԍ��̂݌����t�^�����̑����D
//****************************************************
void skillwinner(int no)
{
	PLAYER *player = GetPlayer(0);
	g_skillWk.skill_player_id = no;
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(i == no)
		{
			player[i].kengen = true;
		}
		else
		{
			player[i].kengen = false;
			SkillReset(i);
		}
	}
}


//*******************************************************************************
// �֐���:	SKILL GetSkillWk(int no)
// ����:	int no
// �߂�l:	SkillWk(no)
// ����:	skill.cpp�݂̂��������Q�b�g�֐��ł�
//*******************************************************************************
SKILL *GetSkillWk(int no)
{
	return &g_skillWk;
}

//*******************************************************************
// ���������I
//*******************************************************************

// ���G���Ԃ̊Ԃ̓J�E���g���v���X����Ȃ��I�I�I
/*
stage.h����GetRane
speedfactor
//*****************************************************************************
typedef struct {
bool use;			// �v���C���[���Q�����Ă����true
float speed_factor;	������M��ƃX�s�[�h���삪�\(1.0�����Œx��)
float lane_length;	// ���[��1�T���̒���
} LANE;


*/