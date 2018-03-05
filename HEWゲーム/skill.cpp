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
#include <time.h>

//***************************************************************
// �}�N����`
//***************************************************************
#define SKILL_WAKU		"data/TEXTURE/skill_frame.png"		// �X�L���Q�[�W
#define SKILL_BAR		"data/TEXTURE/lifegreen.png"		// �X�L���Q�[�W�̃o�[
//#define SKILL_BAR2


// �Q�[�W�̘g����
// �ʒu
#define SKILLGAGE_POS_X	(10.0f)
#define SKILLGAGE_POS_Y	(10.0f)
// ����
#define SKILLGAGE_WIDTH	(469.0f)
#define SKILLGAGE_HEIGHT	(98.0f)

// �Q�[�W�̃o�[�̕���
// �ʒu
#define SKILLBAR_POS_X	(SKILLGAGE_POS_X + 10.0f)	// �ǂ������ɒ���
#define SKILLBAR_POS_Y	(SKILLGAGE_POS_Y + 0.0f )
// ����
#define SKILLBAR_WIDTH	(180.0f)
#define SKILLBAR_HEIGHT	(40.0f)

// ���̑��Q�[�W�֘A
#define SKILL_LEVELUP	(5)			// ���x���A�b�v�ɕK�v�Ȓl
//***************************************************************
// �v���g�^�C�v�錾
//***************************************************************
int skillsort_life(void);
int skill_count_winner(void);
//***************************************************************
// �O���[�o���ϐ�
//***************************************************************
SKILL skillWk;							// �X�L���\����
SKILL_FLAG skill_flag[MAX_PLAYER];
bool skillcheck_ok;						// �X�L�������̌����������Ă���v���C���[��1�l���ǂ���

//char *bulletFileName[] =
//{

//};
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
	MakeVertexSkill(pDevice);

	// �Q�[�W
	D3DXCreateTextureFromFile(pDevice,
		SKILL_BAR,							// �t�@�C���̖��O
		&skillWk.Texture_bar);

	// �Q�[�W�̘g
	D3DXCreateTextureFromFile(pDevice,
		SKILL_WAKU,
		&skillWk.Texture_waku);

	// �t���O������
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		skill_flag[no].flag_no = 0;
		skill_flag[no].use_count = 0;
		skill_flag[no].get = false;
		skill_flag[no].count = 0;
	}

	// �܂������͊���U���Ă��Ȃ�
	skillWk.kengen = false;

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
	if(skillWk.Texture_bar != NULL)
	{// �e�N�X�`���J��
		skillWk.Texture_bar->Release();
		skillWk.Texture_bar = NULL;
	}

	if(skillWk.Buff_bar != NULL)
	{ //���_�o�b�t�@�̊J��
		skillWk.Buff_bar->Release();
		skillWk.Buff_bar = NULL;
	}

	if(skillWk.Texture_waku != NULL)
	{
		skillWk.Texture_waku->Release();
		skillWk.Texture_waku = NULL;
	}

	// �e�N�X�`���J��
	if(skillWk.Buff_waku != NULL)
	{
		skillWk.Buff_waku->Release();
		skillWk.Buff_waku = NULL;

	}
}

//*************************************************************
// �֐���:	void UpdateSkill(float gageup)
// ����:	float gageup
// �߂�l:	�Ȃ�
// ����:	�X�L���Q�[�W�̍X�V
//			gageup�̒l�����X�L���Q�[�W�̒l���㏸�A���l�ȏ�Ń��x���A�b�v
//****************************************************************
void UpdateSkill(float gageup)
{
	int skillget_count = 0;
	PLAYER *player = GetPlayer(0);
	skillcheck_ok = true;					// �X�L���̔��������������Ă���v���C���[��1�l���ǂ���

	// �X�L���Q�[�W�㏸
	skillWk.gage += gageup;

	// �����Q�[�W�����ȏ㒙�܂��Ă����Ȃ�
	if(skillWk.gage >= 5)
	{
		skillWk.lv++;				// ���x�����グ��
		skillWk.gage = 0.0f;		// �l��������
	}

	// �����������܂��N�ɂ����蓖�Ă��Ă��Ȃ��ꍇ
	// ���������߂�

	if(skillWk.kengen == false)
	{
		// skillpoint��5�ɂȂ��Ă���v���C���[�̐l�����m�F
		for(int i = 0; i < MAX_PLAYER; i++)
		{
			if(player[i].skillpoint >= 5)
			{
				player[i].kengen = true;
				skillget_count++;
			}
			else
			{
				player[i].kengen = false;
			}

		}
		// 2�l�ȏオ�����������Ă���ꍇ�A��������ֈړ�
		if(skillget_count >= 2)
		{
			skillcheck_ok = false;			// �����̏��L�҂�1�l�����ł͂Ȃ�
			GetSkill(0);
		}
		else
		{
			skillWk.kengen = true;
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
	float skill_hiritsu;		// �o�[�̔䗦
	float Draw_Skillbar;		// �`�悳���X�L���Q�[�W�̒���

	skill_hiritsu = (skillWk.gage / skillWk.gage_lvup);
	// �\������o�[�̒���
	Draw_Skillbar = (skill_hiritsu * SKILLBAR_WIDTH);

	// �g��`��
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, skillWk.Buff_waku, 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, skillWk.Texture_waku);
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

	// ���C�t�Q�[�W��`��
	{
		// ���_�o�b�t�@���f�o�C�X�̃X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, skillWk.Buff_bar, 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, skillWk.Texture_bar);
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
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
		D3DUSAGE_WRITEONLY,		// �g�p�@
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
		&skillWk.Buff_waku,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̒��g�𖄂߂�
	{
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
		skillWk.Buff_waku->Lock(0, 0, (void**)&pVtx, 0);

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
		skillWk.Buff_waku->Unlock();
	}

	// �X�L���o�[�̃o�b�t�@�쐬
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,		// �g�p�@
		FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
		&skillWk.Buff_bar,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̒��g�𖄂߂�
	{
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
		skillWk.Buff_bar->Lock(0, 0, (void**)&pVtx, 0);

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
		skillWk.Buff_bar->Unlock();
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
		skillWk.Buff_waku->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N
		skillWk.Buff_waku->Unlock();
	}
}


//*****************************************************************************
// �֐���:	void GetSkill(no)
// ����:	5���񂾃v���C���[�̔ԍ�
// �߂�l:
// ����:	updateskill�Ō����������Ă���v���C���[��2�l�ȏ�̏ꍇ���s�A1�l�̃v���C���[�Ɍ�����^����
//			[Get�֐��ł͂Ȃ��ł��I]
//*****************************************************************************
void GetSkill(int no)
{
	PLAYER *player = GetPlayer(0);
	int skill_life_lower;
	int skill_count_lower;
	int counts = 0;
	srand((unsigned)time(NULL));
	int unmakase;

	// ������2�l�ȏオ�����𖞂����Ă����牺�L�̔���

	// ���C�t�����Ȃ��l
	skill_life_lower = skillsort_life();

	// ��L�������������L�҂�2�l�ȏア���ꍇ�̏���
	if(skillcheck_ok == false)
	{
		// �������������l����ԏ��Ȃ�����
		skill_count_lower = skill_count_winner();
	}
	else
	{
		skillWk.kengen = true;
	}


	// ����ł��ʖڂȏꍇ�̏���
	if(skillcheck_ok == false)
	{
		while(0)
		{
			srand((unsigned)time(NULL));
			unmakase = rand() % MAX_PLAYER;			// �K���ȂƂ�����J�n
			if(player[unmakase].kengen == true)
			{
				break;
				skillcheck_ok = true;
				skillWk.kengen = true;
			}
		}

		// ����ȊO�̃v���C���[�͌������D�y�у|�C���g���Z�b�g
		for(int i = 0; i < MAX_PLAYER; i++)
		{
			if(player[i].kengen == true && i != unmakase)
			{
				player[i].kengen = false;
				SkillReset(i);
			}
		}
	}



}


//******************************************************************************
// �֐���:	int skillsort_life(void)
// ����:	�Ȃ�
// �߂�l:	int winner
// ����:	�v���C���[�̗̑͂��r���Ĉ�ԃ��C�t�����Ȃ��v���C���[�̔ԍ���Ԃ�
//******************************************************************************
int skillsort_life(void)
{
	PLAYER *player = GetPlayer(0);
	int winner = 0;					// ���҂̔ԍ�
	bool sort_life = true;			// ���̕��@�Ńv���C���[����l�ɍi�ꂽ���@

									// ���̓��C�t�������ꍇ�A�ԍ����Ⴂ�v���C���[�������𓾂�

	for(int no = 1; no < MAX_PLAYER; no++)
	{
		// ��݂��̗̑͂������ꍇ
		if(player[winner].life == player[no].life)
		{
			sort_life = false;
		}

		// ���݂̏��҂̗̑͂�菭�Ȃ��l���ł���
		if(player[winner].life > player[no].life)
		{
			player[winner].kengen = false;				// ����������
			SkillReset(winner);							// �|�C���g������
			winner = no;								// ���̐l��winner��
		}




	}

	return winner;
}

//******************************************************************
// �֐���:	int skill_count_winner(void)
// ����:	�Ȃ�
// �߂�l:	count_winner
// ����:	�X�L���̔����񐔂����Ȃ��l�̔ԍ���Ԃ�
//******************************************************************
int skill_count_winner(void)
{
	PLAYER *player = GetPlayer(0);
	int count_winner = 0;				// 0�Ԗڂ����r�J�n
	int shaka = 0;						// ����������T���Ƃ��Ɏg���֐�
	skillcheck_ok = true;				// ��������Ɖ���

										// �Ⴂ�����猩�Ă����Č����������Ă���v���C���[��T��
	while(shaka = 0)
	{
		if(player[count_winner].kengen == false)
		{
			count_winner++;
		}
		else
		{
			shaka++;
		}
	}

	for(int i = 1; i < MAX_PLAYER; i++)	// 0�Ԗڂ�1�Ԗڂ����r�J�n
	{
		if(skill_flag[count_winner].count == skill_flag[i].count)
		{
			skillcheck_ok = false;			// 2�l�ȏア��I
		}
		else if(skill_flag[count_winner].count > skill_flag[i].count)
		{// ���������������l����������
			player[count_winner].kengen = false;		// �������͂��D����
			SkillReset(count_winner);					// �|�C���g�����Z�b�g
			count_winner = i;							// ���҂��c��
		}
	}
	return count_winner;
}

//******************************************************************************
// �X�L���|�C���g���Z�b�g
// ����:	int no
// ����:	�W�����v�Ɏ��s�����l�̔ԍ����󂯎���āA���̐l�̃|�C���g��0�ɂ����Ⴄ
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
// ����:	�����������Ă���v���C���[�̔ԍ����󂯎����ʔ���
//************************************************************************************************
void SkillAct(int player_no)
{
	switch(skillWk.lv)
	{
	case SPEEDCHANGE:
		// ����

		case 
	}
	// ���ʂ𔭓�
	// �e�N�X�`���̃Z�b�g
}




//*******************************************************************************
// �֐���:	SKILL GetSkillWk(int no)
// ����:	int no
// �߂�l:	SkillWk(no)
// ����:	skill.cpp�݂̂��������Q�b�g�֐��ł�
//*******************************************************************************
SKILL *GetSkillWk(int no)
{
	return &skillWk;
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