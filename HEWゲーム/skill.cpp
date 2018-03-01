/***************************************************************
* �t�@�C����:	skill.cpp
* �����:		HAL�����Q�[���w�� �R�{ �s��
* �����:		2��23��
* ����:			�X�L���֘A�̃v���O����
****************************************************************/

//***************************************************************
// �C���N���[�h�t�@�C��
//***************************************************************
#include "skill.h"
#include "player.h"

//***************************************************************
// �}�N����`
//***************************************************************
#define SKILL_WAKU		"data/TEXTURE/hpback.png"		// �X�L���Q�[�W
#define SKILL_BAR		"data/TEXTURE/hpgreen.png"		// �X�L���Q�[�W�̃o�[

// �Q�[�W�̘g����
// �ʒu
#define SKILLGAGE_POS_X	(10.0f)
#define SKILLGAGE_POS_Y	(10.0f)
// ����
#define SKILLGAGE_WIDTH	(200.0f)
#define SKILLGAGE_HEIGHT	(100.0f)

// �Q�[�W�̃o�[�̕���
// �ʒu
#define SKILLBAR_POS_X	(SKILLGAGE_POS_X + 10.0f)	// �ǂ������ɒ���
#define SKILLBAR_POS_Y	(SKILLGAGE_POS_Y + 0.0f )
// ����
#define SKILLBAR_WIDTH	(180.0f)
#define SKILLBAR_HEIGHT	(90.0f)

// ���̑��Q�[�W�֘A
#define LVUP_POINT	(5)			// ���x���A�b�v�ɕK�v�Ȓl
//***************************************************************
// �v���g�^�C�v�錾
//***************************************************************


//***************************************************************
// �O���[�o���ϐ�
//***************************************************************
SKILL skillWk;			// �X�L���\����

//***************************************************************
// �֐���:		HRESULT InitSkill(void)
// ����:		�Ȃ�
// �߂�l:		�Ȃ�
// ����:		�X�L���֘A�̏�����
//***************************************************************
HRESULT InitSkill(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexSkill(pDevice);

	//�X�L�����x����������
	skillWk.lv = 0;
	// �X�L���Q�[�W�̒l��������
	skillWk.gage = 0.0f;
	// ���x���A�b�v�ɕK�v�Ȓl��ݒ�
	skillWk.gage_lvup = LVUP_POINT;

	// �Q�[�W
	D3DXCreateTextureFromFile(pDevice,
		SKILL_WAKU,							// �t�@�C���̖��O
		&skillWk.Texture_bar);

	// �Q�[�W�̘g
	D3DXCreateTextureFromFile(pDevice,
		SKILL_BAR,
		&skillWk.Texture_waku);
			
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
	PLAYER *player = GetPlayer(0);

	// �X�L���Q�[�W�㏸
	skillWk.gage += gageup;
	
	// �����Q�[�W�����ȏ㒙�܂��Ă����Ȃ�
	if(skillWk.gage >= LVUP_POINT)
	{
		skillWk.lv++;				// ���x�����グ��
		skillWk.gage = 0.0f;		// �l��������
	}

	// skillpoint��5�ɂȂ��Ă���v���C���[�S���Ɍ�����t�^
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(player[i].skillpoint >= 5)
		{
			player[i].kengen = true;
		}
	}
	// �D�揇�ʌ���
	GetSkill(0);

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
// ����:	�X�L���̌����������Ă���v���C���[������
//*****************************************************************************
void GetSkill(int no)
{	
	// �v���C���[�̃|�C���^�[�������I
	PLAYER *player = GetPlayer(0);
	int old_getskill;				// ���O�Ɍ����������Ă���v���C���[
	

	//// ���Ɍ����������Ă���v���C���[�͂���H
	//for(int i = 0; i< MAX_PLAYER; no++)
	//{
	//	if(player[i].kengen == true)
	//	{
	//		old_getskill = i;					// �����������Ă���v���C���[�̔ԍ���ۑ���
	//		player[i].kengen = false;			// �t���O���I�t��
	//		break;
	//	}
	//}


	//// ��x�S�Ẵt���O��������
	//player[no].kengen = true;		// 5���񂾃v���C���[�Ɍ����t�^
	
	// ���C�t�����Ȃ��l

	// �������������l����ԏ��Ȃ�����


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


//*******************************************************************
// ���������I
//*******************************************************************

// ���G���Ԃ̊Ԃ̓J�E���g���v���X����Ȃ��I�I�I

