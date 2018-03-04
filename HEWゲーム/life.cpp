//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : 
//
//=============================================================================
#include "life.h"

#include "mesh.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_LIFE		"data/TEXTURE/heart.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LIFE_BLACK	"data/TEXTURE/heart_black.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_LIFE	"data/TEXTURE/frame_life.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	LIFE_SIZE_X			(35.0f)							// ���C�t�̐����̕�
#define	LIFE_SIZE_Y			(45.0f)							// ���C�t�̐����̍���
#define	LIFE_INTERVAL_X		(LIFE_SIZE_X)					// ���C�t�̐����̕\���Ԋu

#define	NUM_TEXTURE			(3)								// �e�N�X�`���̐�

#define	OFFSET_LIFE_X			(45.0f)							// ���C�t�̕\����ʒu�w���W
#define	OFFSET_LIFE_Y		(0.0f)							// ���C�t�̕\����ʒu�x���W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
void SetTextureLife(int nIdx);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureLife[NUM_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLife = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//=============================================================================
// ����������
//=============================================================================
HRESULT InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertex(pDevice, &g_pD3DVtxBuffLife, NULL, LIFE_SIZE_X, LIFE_SIZE_Y);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_LIFE,			// �t�@�C���̖��O
								&g_pD3DTextureLife[0]);	// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_LIFE_BLACK,			// �t�@�C���̖��O
		&g_pD3DTextureLife[1]);	// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FRAME_LIFE,			// �t�@�C���̖��O
								&g_pD3DTextureLife[2]);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLife(void)
{
	if(g_pD3DTextureLife[0] != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureLife[0]->Release();
		g_pD3DTextureLife[0] = NULL;
	}
	if(g_pD3DTextureLife[1] != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureLife[1]->Release();
		g_pD3DTextureLife[1] = NULL;
	}

	if(g_pD3DVtxBuffLife != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffLife->Release();
		g_pD3DVtxBuffLife = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		PLAYER *player = GetPlayer(player_no);

		// 1�ڂ̃n�[�g�̈ʒu�ݒ�
		D3DXVECTOR3 pos_life;
		pos_life.x = player->pos.x + OFFSET_LIFE_X;
		pos_life.y = player->ground + OFFSET_LIFE_Y;
		pos_life.z = 10.0f;

		LPDIRECT3DTEXTURE9 texture;
		for (int i = 0; i < MaxLife(); i++)
		{
			if (i < player->life)	// ���ʂ̃n�[�g�`��
			{
				texture = g_pD3DTextureLife[0];
			}
			else // ���n�[�g�`��
			{
				texture = g_pD3DTextureLife[1];
			}

			DrawMesh(g_pD3DVtxBuffLife, texture, pos_life, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			pos_life.x += LIFE_INTERVAL_X;
		}
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
//HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
//{
//	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
//    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
//												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
//												FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
//												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
//												&g_pD3DVtxBuffLife,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//												NULL)))										// NULL�ɐݒ�
//	{
//        return E_FAIL;
//	}
//
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_2D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);
//
//		for(int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
//		{
//			// ���_���W�̐ݒ�
//			pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y, 0.0f);
//			pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y, 0.0f);
//			pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);
//			pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);
//
//			// rhw�̐ݒ�
//			pVtx[0].rhw =
//			pVtx[1].rhw =
//			pVtx[2].rhw =
//			pVtx[3].rhw = 1.0f;
//
//			// ���ˌ��̐ݒ�
//			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// �e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// ���_���W�̐ݒ�
//		pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X - 15, LIFE_POS_Y - 25, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * NUM_PLACE + 15, LIFE_POS_Y - 25, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X - 15, LIFE_POS_Y + 55, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * NUM_PLACE + 15, LIFE_POS_Y + 55, 0.0f);
//
//		// rhw�̐ݒ�
//		pVtx[0].rhw =
//		pVtx[1].rhw =
//		pVtx[2].rhw =
//		pVtx[3].rhw = 1.0f;
//
//		// ���ˌ��̐ݒ�
//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
//
//		// �e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffLife->Unlock();
//	}
//
//	return S_OK;
//}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureLife(int nIdx)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffLife->Unlock();
}