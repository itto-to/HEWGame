//****************************************************************
// �t�@�C����:	skillact.cpp
// ����:		�X�L���̓���
// �ŏI�X�V��:	3��4��
//****************************************************************

//****************************************************************
// �C���N���[�h�t�@�C��
//****************************************************************
#include "camera.h"
#include "player.h"
#include "skill.h"
#include "skillact.h"
#include "input.h"
#include "mesh.h"

//*****************************************************************
// �}�N����`
//*****************************************************************

// �G�t�F�N�g
<<<<<<< HEAD
#define TEXTURE_UPEFFECT	("data/TEXTURE/icon_speed_up.png")				// ���x�㏸
#define TEXTURE_DOWNEFFECT	("data/TEXTURE/icon_speed_down.png")			// ����
#define TEXTURE_OJYAMA		("data/TEXTURE/skill_lv2_block.png")			// ���W���}�u���b�N
#define TEXTURE_KAMINARI	("data/TEXTURE/skill_lv3_thunder.png")			// ��
=======
#define UPEFFECT	("data/TEXTURE/icon_speed_up.png")				// ���x�㏸
#define DOWNEFFECT	("data/TEXTURE/icon_speed_down.png")				// ����
#define OJYAMA		("data/TEXTURE/skill_lv2_block.png")				// ���W���}�u���b�N
#define KAMINARI	("data/TEXTURE/skill_lv3_thunder.png")				// ����
>>>>>>> 4f3be628524b288d711aa7094ab03acd72035386


// �T�C�Y
#define UPEFFECT_HEIGHT		(50)
#define UPEFFECT_WIDTH		(50)
#define DOWNEFFECT_HEIGHT	(50)
#define DOWNEFFECT_WIDTH	(50)
#define OJYAMA_HEIGHT		(90)
#define OJYAMA_WIDTH		(90)
#define KAMINARI_HEIGHT		(50)
#define KAMINARI_WIDTH		(50)

#define OJYAMA_OFFSET_WIDTH	(OJYAMA_WIDTH * 0.9f)
#define NUM_OJYAMA_BLOCK	(8)

// ���W���
#define SPEED_EFFECT_OFFSET	(D3DXVECTOR3(10.0f, 10.0f, 0.0f))
#define OJYAMA_EFFECT_OFFSET (D3DXVECTOR3(-1040.0f, 28.0f, 0.0f))
#define KAMINARI_EFFECT_OFFSET (D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//#define

//*****************************************************************
// �O���[�o���ϐ�
//*****************************************************************
SKILLACT skillactWk[MAX_PLAYER];

//*****************************************************************
// �v���g�^�C�v�錾
//*****************************************************************
HRESULT MakeVertexUp_effect(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexDown_effect(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexOjyama(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexKaminari(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int effect, int no);
void SetColorSkillAct(int nIdxSkillAct, int effect, int no, D3DXCOLOR col);



//*****************************************************************
// �֐����@HRESULT Init
//
//������
//*****************************************************************
HRESULT InitSkillAct(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexSkillact(pDevice);


	// �t���O���Z�b�g
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		skillactWk[i].Up_active = false;
		skillactWk[i].Down_active = false;
		skillactWk[i].Ojyama_active = false;
		skillactWk[i].Kaminari_active = false;

	}
	// �e�N�X�`���ǂݍ���
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_UPEFFECT,							// �t�@�C���̖��O
			&skillactWk[i].Up_texture);

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_DOWNEFFECT,							// �t�@�C���̖��O
			&skillactWk[i].Down_texture);

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_OJYAMA,							// �t�@�C���̖��O
			&skillactWk[i].Ojyama_texture);

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_KAMINARI,							// �t�@�C���̖��O
			&skillactWk[i].Kaminari_texture);

	}

	return S_OK;

}

//*************************************************************
// �֐���:	void UninitSkillAct(void)
// ����:	�Ȃ�
// �߂�l:
// ����:	�X�L�������̏I������
//*************************************************************
void UninitSkillAct(void)
{
	// ���ꂼ��̃e�N�X�`���ƃo�b�t�@�����ꂼ��J��

	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(skillactWk[i].Up_texture != NULL)
		{// �e�N�X�`���J��
			skillactWk[i].Up_texture->Release();
			skillactWk[i].Up_texture = NULL;
		}

		if(skillactWk[i].Up_buff != NULL)
		{ //���_�o�b�t�@�̊J��
			skillactWk[i].Up_buff->Release();
			skillactWk[i].Up_buff = NULL;
		}

		// down
		if(skillactWk[i].Down_texture != NULL)
		{// �e�N�X�`���J��
			skillactWk[i].Down_texture->Release();
			skillactWk[i].Down_texture = NULL;
		}

		if(skillactWk[i].Down_buff != NULL)
		{ //���_�o�b�t�@�̊J��
			skillactWk[i].Down_buff->Release();
			skillactWk[i].Down_buff = NULL;
		}

		// ojyama
		if(skillactWk[i].Ojyama_texture != NULL)
		{// �e�N�X�`���J��
			skillactWk[i].Ojyama_texture->Release();
			skillactWk[i].Ojyama_texture = NULL;
		}

		if(skillactWk[i].Ojyama_buff != NULL)
		{ //���_�o�b�t�@�̊J��
			skillactWk[i].Ojyama_buff->Release();
			skillactWk[i].Ojyama_buff = NULL;
		}

		// kaminari
		if(skillactWk[i].Kaminari_texture != NULL)
		{// �e�N�X�`���J��
			skillactWk[i].Kaminari_texture->Release();
			skillactWk[i].Kaminari_texture = NULL;
		}

		if(skillactWk[i].Kaminari_buff != NULL)
		{ //���_�o�b�t�@�̊J��
			skillactWk[i].Kaminari_buff->Release();
			skillactWk[i].Kaminari_buff = NULL;
		}

	}

}



//****************************************************************
// �֐���:	void DrawSkillAct(void)
// ����:	�Ȃ�
// �߂�l:	�Ȃ�
// ����:	�G�t�F�N�g�̕`��
//****************************************************************
void DrawSkillAct(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// UP�G�t�F�N�g
	for (int no = 0; no < NumPlayer(); no++)
	{
		// ���s��Ԃ���Ȃ��ꍇ�`�悵�Ȃ�
		if(!skillactWk[no].Up_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];
		if (skillact->Up_active == true)
		{
			DrawMesh(skillact->Up_buff, skillact->Up_texture, player->pos + SPEED_EFFECT_OFFSET, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}
	
	// DOWN�G�t�F�N�g
	for (int no = 0; no < NumPlayer(); no++)
	{
		// ���s��Ԃ���Ȃ��ꍇ�`�悵�Ȃ�
		if (!skillactWk[no].Down_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];
		if (skillact->Down_active == true)
		{
			DrawMesh(skillact->Down_buff, skillact->Down_texture, player->pos + SPEED_EFFECT_OFFSET, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}

<<<<<<< HEAD
	// ������܃u���b�N
	for (int no = 0; no < NumPlayer(); no++)
	{
		// ���s��Ԃ���Ȃ��ꍇ�`�悵�Ȃ�
		if (!skillactWk[no].Ojyama_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];

		if (skillact->Ojyama_active == true)
		{
			D3DXVECTOR3 offset = OJYAMA_EFFECT_OFFSET;
			for (int i = 0; i < NUM_OJYAMA_BLOCK; i++) {
				DrawMesh(skillact->Ojyama_buff, skillact->Ojyama_texture, player->ground_pos + offset, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				offset.x += OJYAMA_OFFSET_WIDTH;
			}
		}
=======
	// ojyama
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		// ���s��Ԃ̏ꍇ�`��
		if(skillactWk[no].Ojyama_active == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&skillactWk[no].Ojyama_mtxWorld);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// �|���S���𐳖ʂɌ�����
#if 1
			// �t�s������Ƃ߂�
			D3DXMatrixInverse(&skillactWk[no].Ojyama_mtxWorld, NULL, &mtxView);
			skillactWk[no].Ojyama_mtxWorld._41 = 0.0f;
			skillactWk[no].Ojyama_mtxWorld._42 = 0.0f;
			skillactWk[no].Ojyama_mtxWorld._43 = 0.0f;
#else
			skillactWk[no].Ojyama_mtxWorld._11 = mtxView._11;
			skillactWk[no].Ojyama_mtxWorld._12 = mtxView._21;
			skillactWk[no].Ojyama_mtxWorld._13 = mtxView._31;
			skillactWk[no].Ojyama_mtxWorld._21 = mtxView._12;
			skillactWk[no].Ojyama_mtxWorld._22 = mtxView._22;
			skillactWk[no].Ojyama_mtxWorld._23 = mtxView._32;
			skillactWk[no].Ojyama_mtxWorld._31 = mtxView._13;
			skillactWk[no].Ojyama_mtxWorld._32 = mtxView._23;
			skillactWk[no].Ojyama_mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, skillactWk[no].Ojyama_scl.x,
				skillactWk[no].Ojyama_scl.y,
				skillactWk[no].Ojyama_scl.z);
			D3DXMatrixMultiply(&skillactWk[no].Ojyama_mtxWorld, &skillactWk[no].Ojyama_mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Ojyama_pos.x,
				skillactWk[no].Ojyama_pos.y,
				skillactWk[no].Ojyama_pos.z);
			D3DXMatrixMultiply(&skillactWk[no].Ojyama_mtxWorld, &skillactWk[no].Ojyama_mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Ojyama_mtxWorld);


			// ���C�t�Q�[�W��`��
			{
				// ���_�o�b�t�@���f�o�C�X�̃X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, skillactWk[no].Ojyama_buff, 0, sizeof(VERTEX_2D));
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, skillactWk[no].Ojyama_texture);
				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
			}
		}
	}

	// Kaminari
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		// ���s��Ԃ̏ꍇ�`��
		if(skillactWk[no].Kaminari_active == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&skillactWk[no].Kaminari_mtxWorld);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// �|���S���𐳖ʂɌ�����
#if 1
			// �t�s������Ƃ߂�
			D3DXMatrixInverse(&skillactWk[no].Kaminari_mtxWorld, NULL, &mtxView);
			skillactWk[no].Kaminari_mtxWorld._41 = 0.0f;
			skillactWk[no].Kaminari_mtxWorld._42 = 0.0f;
			skillactWk[no].Kaminari_mtxWorld._43 = 0.0f;
#else
			skillactWk[no].Kaminari_mtxWorld._11 = mtxView._11;
			skillactWk[no].Kaminari_mtxWorld._12 = mtxView._21;
			skillactWk[no].Kaminari_mtxWorld._13 = mtxView._31;
			skillactWk[no].Kaminari_mtxWorld._21 = mtxView._12;
			skillactWk[no].Kaminari_mtxWorld._22 = mtxView._22;
			skillactWk[no].Kaminari_mtxWorld._23 = mtxView._32;
			skillactWk[no].Kaminari_mtxWorld._31 = mtxView._13;
			skillactWk[no].Kaminari_mtxWorld._32 = mtxView._23;
			skillactWk[no].Kaminari_mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, skillactWk[no].Kaminari_scl.x,
				skillactWk[no].Kaminari_scl.y,
				skillactWk[no].Kaminari_scl.z);
			D3DXMatrixMultiply(&skillactWk[no].Kaminari_mtxWorld, &skillactWk[no].Kaminari_mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Kaminari_pos.x,
				skillactWk[no].Kaminari_pos.y,
				skillactWk[no].Kaminari_pos.z);
			D3DXMatrixMultiply(&skillactWk[no].Kaminari_mtxWorld, &skillactWk[no].Kaminari_mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Kaminari_mtxWorld);


			// ���C�t�Q�[�W��`��
			{
				// ���_�o�b�t�@���f�o�C�X�̃X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, skillactWk[no].Kaminari_buff, 0, sizeof(VERTEX_2D));
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, skillactWk[no].Kaminari_texture);
				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
			}
		}
>>>>>>> 4f3be628524b288d711aa7094ab03acd72035386
	}

	// �J�~�i��
	for (int no = 0; no < NumPlayer(); no++)
	{
		// ���s��Ԃ���Ȃ��ꍇ�`�悵�Ȃ�
		if (!skillactWk[no].Kaminari_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];
		if (skillact->Kaminari_active == true)
		{
			DrawMesh(skillact->Kaminari_buff, skillact->Kaminari_texture, player->pos + KAMINARI_EFFECT_OFFSET, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}
}
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
//
//	// ���C���e�B���O�𖳌��ɂ���
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//
//	// Upeffect
//	for(int no = 0; no < MAX_PLAYER; no++)
//	{
//		// ���s��Ԃ̏ꍇ�`��
//		if(skillactWk[no].Up_active == true)
//		{
//			// ���[���h�}�g���b�N�X�̏�����
//			D3DXMatrixIdentity(&skillactWk[no].Up_mtxWorld);
//
//			// �r���[�}�g���b�N�X���擾
//			mtxView = GetMtxView();
//
//			// �|���S���𐳖ʂɌ�����
//#if 1
//			// �t�s������Ƃ߂�
//			D3DXMatrixInverse(&skillactWk[no].Up_mtxWorld, NULL, &mtxView);
//			skillactWk[no].Up_mtxWorld._41 = 0.0f;
//			skillactWk[no].Up_mtxWorld._42 = 0.0f;
//			skillactWk[no].Up_mtxWorld._43 = 0.0f;
//#else
//			skillactWk[no].Up_mtxWorld._11 = mtxView._11;
//			skillactWk[no].Up_mtxWorld._12 = mtxView._21;
//			skillactWk[no].Up_mtxWorld._13 = mtxView._31;
//			skillactWk[no].Up_mtxWorld._21 = mtxView._12;
//			skillactWk[no].Up_mtxWorld._22 = mtxView._22;
//			skillactWk[no].Up_mtxWorld._23 = mtxView._32;
//			skillactWk[no].Up_mtxWorld._31 = mtxView._13;
//			skillactWk[no].Up_mtxWorld._32 = mtxView._23;
//			skillactWk[no].Up_mtxWorld._33 = mtxView._33;
//#endif
//
//			// �X�P�[���𔽉f
//			D3DXMatrixScaling(&mtxScale, skillactWk[no].Up_scl.x,
//				skillactWk[no].Up_scl.y,
//				skillactWk[no].Up_scl.z);
//			D3DXMatrixMultiply(&skillactWk[no].Up_mtxWorld, &skillactWk[no].Up_mtxWorld, &mtxScale);
//
//			// �ړ��𔽉f
//			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Up_pos.x,
//				skillactWk[no].Up_pos.y,
//				skillactWk[no].Up_pos.z);
//			D3DXMatrixMultiply(&skillactWk[no].Up_mtxWorld, &skillactWk[no].Up_mtxWorld, &mtxTranslate);
//
//			// ���[���h�}�g���b�N�X�̐ݒ�
//			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Up_mtxWorld);
//
//
//			// ���C�t�Q�[�W��`��
//			{
//				// ���_�o�b�t�@���f�o�C�X�̃X�g���[���Ƀo�C���h
//				pDevice->SetStreamSource(0, skillactWk[no].Up_buff, 0, sizeof(VERTEX_2D));
//				// ���_�t�H�[�}�b�g�̐ݒ�
//				pDevice->SetFVF(FVF_VERTEX_2D);
//				// �e�N�X�`���̐ݒ�
//				pDevice->SetTexture(0, skillactWk[no].Up_texture);
//				// �|���S���̕`��
//				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//			}
//		}
//	}
//
//	// Downeffect
//	for(int no = 0; no < MAX_PLAYER; no++)
//	{
//		// ���s��Ԃ̏ꍇ�`��
//		if(skillactWk[no].Down_active == true)
//		{
//			// ���[���h�}�g���b�N�X�̏�����
//			D3DXMatrixIdentity(&skillactWk[no].Down_mtxWorld);
//
//			// �r���[�}�g���b�N�X���擾
//			mtxView = GetMtxView();
//
//			// �|���S���𐳖ʂɌ�����
//#if 1
//			// �t�s������Ƃ߂�
//			D3DXMatrixInverse(&skillactWk[no].Down_mtxWorld, NULL, &mtxView);
//			skillactWk[no].Down_mtxWorld._41 = 0.0f;
//			skillactWk[no].Down_mtxWorld._42 = 0.0f;
//			skillactWk[no].Down_mtxWorld._43 = 0.0f;
//#else
//			skillactWk[no].Down_mtxWorld._11 = mtxView._11;
//			skillactWk[no].Down_mtxWorld._12 = mtxView._21;
//			skillactWk[no].Down_mtxWorld._13 = mtxView._31;
//			skillactWk[no].Down_mtxWorld._21 = mtxView._12;
//			skillactWk[no].Down_mtxWorld._22 = mtxView._22;
//			skillactWk[no].Down_mtxWorld._23 = mtxView._32;
//			skillactWk[no].Down_mtxWorld._31 = mtxView._13;
//			skillactWk[no].Down_mtxWorld._32 = mtxView._23;
//			skillactWk[no].Down_mtxWorld._33 = mtxView._33;
//#endif
//
//			// �X�P�[���𔽉f
//			D3DXMatrixScaling(&mtxScale, skillactWk[no].Down_scl.x,
//				skillactWk[no].Down_scl.y,
//				skillactWk[no].Down_scl.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxScale);
//
//			// �ړ��𔽉f
//			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Down_pos.x,
//				skillactWk[no].Down_pos.y,
//				skillactWk[no].Down_pos.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxTranslate);
//
//			// ���[���h�}�g���b�N�X�̐ݒ�
//			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Down_mtxWorld);
//
//
//			// ���C�t�Q�[�W��`��
//			{
//				// ���_�o�b�t�@���f�o�C�X�̃X�g���[���Ƀo�C���h
//				pDevice->SetStreamSource(0, skillactWk[no].Down_buff, 0, sizeof(VERTEX_2D));
//				// ���_�t�H�[�}�b�g�̐ݒ�
//				pDevice->SetFVF(FVF_VERTEX_2D);
//				// �e�N�X�`���̐ݒ�
//				pDevice->SetTexture(0, skillactWk[no].Down_texture);
//				// �|���S���̕`��
//				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//			}
//		}
//	}
//
//	// ojyama
//	for(int no = 0; no < MAX_PLAYER; no++)
//	{
//		// ���s��Ԃ̏ꍇ�`��
//		if(skillactWk[no].Down_active == true)
//		{
//			// ���[���h�}�g���b�N�X�̏�����
//			D3DXMatrixIdentity(&skillactWk[no].Down_mtxWorld);
//
//			// �r���[�}�g���b�N�X���擾
//			mtxView = GetMtxView();
//
//			// �|���S���𐳖ʂɌ�����
//#if 1
//			// �t�s������Ƃ߂�
//			D3DXMatrixInverse(&skillactWk[no].Down_mtxWorld, NULL, &mtxView);
//			skillactWk[no].Down_mtxWorld._41 = 0.0f;
//			skillactWk[no].Down_mtxWorld._42 = 0.0f;
//			skillactWk[no].Down_mtxWorld._43 = 0.0f;
//#else
//			skillactWk[no].Down_mtxWorld._11 = mtxView._11;
//			skillactWk[no].Down_mtxWorld._12 = mtxView._21;
//			skillactWk[no].Down_mtxWorld._13 = mtxView._31;
//			skillactWk[no].Down_mtxWorld._21 = mtxView._12;
//			skillactWk[no].Down_mtxWorld._22 = mtxView._22;
//			skillactWk[no].Down_mtxWorld._23 = mtxView._32;
//			skillactWk[no].Down_mtxWorld._31 = mtxView._13;
//			skillactWk[no].Down_mtxWorld._32 = mtxView._23;
//			skillactWk[no].Down_mtxWorld._33 = mtxView._33;
//#endif
//
//			// �X�P�[���𔽉f
//			D3DXMatrixScaling(&mtxScale, skillactWk[no].Down_scl.x,
//				skillactWk[no].Down_scl.y,
//				skillactWk[no].Down_scl.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxScale);
//
//			// �ړ��𔽉f
//			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Down_pos.x,
//				skillactWk[no].Down_pos.y,
//				skillactWk[no].Down_pos.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxTranslate);
//
//			// ���[���h�}�g���b�N�X�̐ݒ�
//			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Down_mtxWorld);
//
//
//			// ���C�t�Q�[�W��`��
//			{
//				// ���_�o�b�t�@���f�o�C�X�̃X�g���[���Ƀo�C���h
//				pDevice->SetStreamSource(0, skillactWk[no].Down_buff, 0, sizeof(VERTEX_2D));
//				// ���_�t�H�[�}�b�g�̐ݒ�
//				pDevice->SetFVF(FVF_VERTEX_2D);
//				// �e�N�X�`���̐ݒ�
//				pDevice->SetTexture(0, skillactWk[no].Down_texture);
//				// �|���S���̕`��
//				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//			}
//		}
//	}
//
//}
//

//*****************************************************************
// �֐���:	void MakeVertexSkillact(LPDIRECT3DDEVICE pDevice)
// ����:	LPDIRECT3DDEVICE9 pDevice
// �߂�l:	�Ȃ�
// ����:	MakeVertex�֐������s����֐�
//*****************************************************************
void MakeVertexSkillact(LPDIRECT3DDEVICE9 pDevice)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++) {
		PLAYER *player = GetPlayer(player_no);
		// UP�G�t�F�N�g
		MakeVertex(pDevice, &skillactWk[player_no].Up_buff, NULL, UPEFFECT_WIDTH, UPEFFECT_HEIGHT);

		// DOWN�G�t�F�N�g
		MakeVertex(pDevice, &skillactWk[player_no].Down_buff, NULL, DOWNEFFECT_WIDTH, DOWNEFFECT_HEIGHT);
		
		// ������܃u���b�N
		MakeVertex(pDevice, &skillactWk[player_no].Ojyama_buff, NULL, OJYAMA_WIDTH, OJYAMA_HEIGHT);

		// �J�~�i��
		MakeVertex(pDevice, &skillactWk[player_no].Kaminari_buff, NULL, KAMINARI_WIDTH, KAMINARI_HEIGHT);
	}
}
//*****************************************************************
// �֐���:	HRESULT MakeVertexUp_effect(LPDIRECT3DDEVICE9 pDevice)
// ����:	LPDIRECT3DDEVICE9 pDevice
// �߂�l:
// ����		���_�̍쐬
//******************************************************************
HRESULT MakeVertexUp_effect(LPDIRECT3DDEVICE9 pDevice)
{
	// Up
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// �o�b�t�@�쐬
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
			D3DUSAGE_WRITEONLY,		// �g�p�@
			FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
			&skillactWk[i].Up_buff,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̒��g�𖄂߂�
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
			skillactWk[i].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2,  UPEFFECT_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3( UPEFFECT_WIDTH / 2,  UPEFFECT_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2, -UPEFFECT_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3( UPEFFECT_WIDTH / 2, -UPEFFECT_HEIGHT / 2, 0.0f);

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
			skillactWk[i].Up_buff->Unlock();
		}
	}

	

	return S_OK;
}

//*****************************************************************
// �֐���:	HRESULT MakeVertexDown_effect(LPDIRECT3DDEVICE9 pDevice)
// ����:	LPDIRECT3DDEVICE9 pDevice
// �߂�l:
// ����		���_�̍쐬
//******************************************************************
HRESULT MakeVertexDown_effect(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// �o�b�t�@�쐬
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
			D3DUSAGE_WRITEONLY,		// �g�p�@
			FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
			&skillactWk[i].Down_buff,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̒��g�𖄂߂�
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
			skillactWk[i].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2,  DOWNEFFECT_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3( DOWNEFFECT_WIDTH / 2,  DOWNEFFECT_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2, -DOWNEFFECT_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3( DOWNEFFECT_WIDTH / 2, -DOWNEFFECT_HEIGHT / 2, 0.0f);

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
			skillactWk[i].Down_buff->Unlock();
		}
	}
	return S_OK;
}

//*****************************************************************
// �֐���:	HRESULT MakeVertexOjyama(LPDIRECT3DDEVICE9 pDevice)
// ����:	LPDIRECT3DDEVICE9 pDevice
// �߂�l:
// ����		���_�̍쐬
//******************************************************************
HRESULT MakeVertexOjyama(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// �o�b�t�@�쐬
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
			D3DUSAGE_WRITEONLY,		// �g�p�@
			FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
			&skillactWk[i].Ojyama_buff,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̒��g�𖄂߂�
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
			skillactWk[i].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-OJYAMA_WIDTH / 2, OJYAMA_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2, OJYAMA_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-OJYAMA_WIDTH / 2, -OJYAMA_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2, -OJYAMA_HEIGHT / 2, 0.0f);

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
			skillactWk[i].Ojyama_buff->Unlock();
		}
	}
	return S_OK;
}

//*****************************************************************
// �֐���:	HRESULT MakeVertexKaminari(LPDIRECT3DDEVICE9 pDevice)
// ����:	LPDIRECT3DDEVICE9 pDevice
// �߂�l:
// ����		���_�̍쐬
//******************************************************************
HRESULT MakeVertexKaminari(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// �o�b�t�@�쐬
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�A�m�ۂ���o�b�t�@�̃T�C�Y
			D3DUSAGE_WRITEONLY,		// �g�p�@
			FVF_VERTEX_2D,				// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X
			&skillactWk[i].Kaminari_buff,			// ���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̒��g�𖄂߂�
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���|�C���^���擾
			skillactWk[i].Kaminari_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-KAMINARI_WIDTH / 2, KAMINARI_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(KAMINARI_WIDTH / 2, KAMINARI_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-KAMINARI_WIDTH / 2, -KAMINARI_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(KAMINARI_WIDTH / 2, -KAMINARI_HEIGHT / 2, 0.0f);

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
			skillactWk[i].Kaminari_buff->Unlock();
		}
	}
	return S_OK;
}

//****************************************************************************
// �֐���:	void SetColorSkillAct(void)
// ����:	�Ȃ�
// �߂�l:	�Ȃ�
// ����:	�f�B�t���[�Y�F�ݒ�
//*****************************************************************************
void SetColorSkillAct(int player_no, int effect)
{

	// ���_�o�b�t�@�̒��g�𖄂߂�
	// Up
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[i].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N
			skillactWk[i].Up_buff->Unlock();
		}
	}

	// Down
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[i].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N
			skillactWk[i].Down_buff->Unlock();
		}
	}

	// Ojyama
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[i].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N
			skillactWk[i].Ojyama_buff->Unlock();
		}
	}


	// Kaminari
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[i].Kaminari_buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N
			skillactWk[i].Kaminari_buff->Unlock();
		}
	}
}



//************************************************************************************
// �֐���:	void SetVertexEffect(int nIdxEffect,int effect, int no)
// ����:	int effect int no
// �߂�l:	�Ȃ�
// ����:	effect�̒l�ɂ���Ē��_���W��ݒ� no�̓v���C���[�̔ԍ�
//************************************************************************************
void SetVertexEffect(int effect,int no)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�

		VERTEX_3D *pVtx;

		// ���݂̃��x���ɂ���ď����𕪊�
		switch(effect)
		{
		case EFFECT_UP:
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[no].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2.0f, UPEFFECT_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(UPEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(UPEFFECT_WIDTH / 2.0f, UPEFFECT_HEIGHT, 0.0f);

			// ���_�f�[�^���A�����b�N����
			skillactWk[no].Up_buff->Unlock();
			break;

		case EFFECT_DOWN:
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[no].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2.0f, DOWNEFFECT_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(DOWNEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(DOWNEFFECT_WIDTH / 2.0f, DOWNEFFECT_HEIGHT, 0.0f);

			// ���_�f�[�^���A�����b�N����
			skillactWk[no].Down_buff->Unlock();
			break;

		case EFFECT_OJYAMA:
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[no].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);

			// ���_�f�[�^���A�����b�N����
			skillactWk[no].Ojyama_buff->Unlock();
			break;

		case EFFECT_KAMINARI:
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[no].Kaminari_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);

			// ���_�f�[�^���A�����b�N����
			skillactWk[no].Kaminari_buff->Unlock();
			break;


		
		}
	}
}


//*************************************************************************************
// �֐���:	void SetColorSkillact(int nIdxSkillAct,D3DXCOLOR col)
// ����:	int nIdxSkillAct, int effect,int no, D3DXCOLOR col
// �߂�l:	�Ȃ�
// ����		�F�̐ݒ�
// nIdxSkillAct��no���������l�ɂȂ邩�瑽���ǂ�����1�ł����H
//*************************************************************************************

void SetColorSkillAct(int nIdxSkillAct, int effect,int no, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		switch(effect)
		{
		case EFFECT_UP:
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[no].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdxSkillAct * 4);

			// ���_���W�̐ݒ�
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// ���_�f�[�^���A�����b�N����
			skillactWk[no].Up_buff->Unlock();
			break;

		case EFFECT_DOWN:
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[no].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdxSkillAct * 4);

			// ���_���W�̐ݒ�
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// ���_�f�[�^���A�����b�N����
			skillactWk[no].Down_buff->Unlock();
			break;

		case EFFECT_OJYAMA:
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			skillactWk[no].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdxSkillAct * 4);

			// ���_���W�̐ݒ�
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// ���_�f�[�^���A�����b�N����
			skillactWk[no].Ojyama_buff->Unlock();
			break;
		}
	}
}

//*************************************************************************************************
// �֐���:	int SetSkillAct(D3DXVECTOR3 pos,int no,int effect,int player_no
// ����:
// �߂�l:	int  nIdxSkillAct;
// ����:	�X�L���̔����y�уe�N�X�`���̃Z�b�g
//**************************************************************************************************
int SetSkillAct(D3DXVECTOR3 pos,int effect,int player_no, D3DXCOLOR col)
{
	SKILL *skill = GetSkillWk(0);
	int nIdxSkillact = -1;

	switch(effect)
	{
	case EFFECT_UP:
		// no�͔��������v���C���[�̔ԍ�
		skillactWk[player_no].Up_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Up_pos = pos;
		skillactWk[player_no].Up_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Up_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Up_active = true;
		skillactWk[player_no].SpeedTime = ACTTIME_LV1;					// ���s���Ԑݒ�

		// ���_�J���[�̐ݒ�
		SetColorSkillAct(player_no, effect, player_no, col);
		break;

	case EFFECT_DOWN:
		// no�͔��������v���C���[�̔ԍ�
		skillactWk[player_no].Down_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Down_pos = pos;
		skillactWk[player_no].Down_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Down_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Down_active = true;
		skillactWk[player_no].SpeedTime = ACTTIME_LV1;				// ���s���Ԑݒ�

		// ���_�J���[�̐ݒ�
		SetColorSkillAct(player_no, effect, player_no, col);
		break;

	case EFFECT_OJYAMA:
		// 
		for(int i = 0; i < OJYAMASET_MAX; i++)
		{
		skillactWk[player_no].Ojyama_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Ojyama_pos = D3DXVECTOR3((i*OJYAMA_WIDE),pos.y,pos.z);
		skillactWk[player_no].Ojyama_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Ojyama_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Ojyama_active = true;
		skillactWk[player_no].OjyamaTime = ACTTIME_LV2;

		// ���_�J���[�̐ݒ�
		SetColorSkillAct(player_no, effect, player_no, col);
		}
		break;


	case EFFECT_KAMINARI:
		// no�͔��������v���C���[�̔ԍ�
		skillactWk[player_no].Kaminari_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Kaminari_pos = pos;
		skillactWk[player_no].Kaminari_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Kaminari_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Kaminari_active = true;


		// ���_�J���[�̐ݒ�
		SetColorSkillAct(player_no, effect, player_no, col);
		break;
	}




	nIdxSkillact = player_no;

	return nIdxSkillact;
}



//**************************************************************************
// �֐���:	SKILLACT *GetSkillAct(int no)
// ����:	int no
// �߂�l:	SKILLACT skillactWk
// ����:	Get�֐�
//**************************************************************************
SKILLACT *GetSkillAct(int no)
{
	return &skillactWk[0];
}



