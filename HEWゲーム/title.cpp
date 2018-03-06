//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : 
//
//=============================================================================
#include "title.h"

#include "camera.h"
#include "main.h"
#include "mesh.h"
#include "debugproc.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BUTTON_GAME_START	(BUTTON_START)

#define	TEXTURE_TITLE		"data/TEXTURE/bg_title.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_LOGO	"data/TEXTURE/title_logo.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LOGO_START	"data/TEXTURE/press_start.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TITLE_LOGO_POS_X		(SCREEN_CENTER_X)		// �^�C�g�����S�̈ʒu(X���W)
#define	TITLE_LOGO_POS_Y		(SCREEN_CENTER_Y)		// �^�C�g�����S�̈ʒu(Y���W)
#define	TITLE_LOGO_WIDTH		(353.0f)		// �^�C�g�����S�̕�
#define	TITLE_LOGO_HEIGHT		(172.0f)		// �^�C�g�����S�̍���

#define	START_POS_X				(SCREEN_CENTER_X - 300)		// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y				(SCREEN_CENTER_Y + 200)		// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH				(353.0f * 2)		// �X�^�[�g�{�^���̕�
#define	START_HEIGHT			(172.0f * 2)		// �X�^�[�g�{�^���̍���

#define	COUNT_APPERA_START		(60)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_START		(60)		// �X�^�[�g�{�^���_�ł̎���

#define	COUNT_WAIT_DEMO			(60 * 5)	// �f���܂ł̑҂�����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetColorTitleLogo(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int						g_nCountAppearStart = 0;		//
float					g_fAlphaLogo = 0.0f;			//
int						g_nCountDisp = 0;				//
bool					g_bDispStart = false;			//
int						g_nConutDemo = 0;				//

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearStart = 0;
	g_fAlphaLogo = 0.0f;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_nConutDemo = 0;

	// ���_���̍쐬
	MakeVertex(pDevice, &g_pD3DVtxBuffTitle, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);
	MakeVertex(pDevice, &g_pD3DVtxBuffStart, NULL, START_WIDTH, START_HEIGHT);	// "PRESS START"
	
	// �e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_TITLE,				// �t�@�C���̖��O
		&g_pD3DTextureTitle)))		// �ǂݍ��ރ������[
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_LOGO_START,			// �t�@�C���̖��O
		&g_pD3DTextureStart)))		// �ǂݍ��ރ������[
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if(g_pD3DTextureTitle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if(g_pD3DVtxBuffTitle != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffTitle->Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	if(g_pD3DTextureStart != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if(g_pD3DVtxBuffStart != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
#if 0
	if(g_nCountAppearStart >= COUNT_APPERA_START)
	{
		g_nConutDemo++;
		if(g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
#endif

	if(g_fAlphaLogo < 1.0f)
	{
		g_fAlphaLogo += 0.005f;
		if(g_fAlphaLogo >= 1.0f)
		{
			g_fAlphaLogo = 1.0f;
		}
		SetColorTitleLogo();
	}
	else
	{
		g_nCountAppearStart++;
		if(g_nCountAppearStart > COUNT_APPERA_START)
		{
			g_nCountDisp = (g_nCountDisp + 1) % 80;
			if(g_nCountDisp > INTERVAL_DISP_START)
			{
				g_bDispStart = false;
			}
			else
			{
				g_bDispStart = true;
			}
		}
	}

	bool start = false;
	// �p�b�h�ŃX�^�[�g
	for (int pad_no = 0; pad_no < NumPad(); pad_no++) {
		if (IsButtonTriggered(pad_no, BUTTON_GAME_START))
			start = true;
	}
	// �L�[�{�[�h�ŃX�^�[�g
	if (GetKeyboardTrigger(DIK_RETURN))
		start = true;;

	if (start)
	{
		if (g_nCountAppearStart == 0)
		{// �^�C�g���o��X�L�b�v
			g_fAlphaLogo = 1.0f;
			SetColorTitleLogo();

			g_nCountAppearStart = COUNT_APPERA_START;
		}
		else
		{// �Q�[����

			SetFade(FADE_OUT);
			PlaySound(SOUND_LABEL_GAMESTART);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �^�C�g���`��
	DrawMesh(g_pD3DVtxBuffTitle, g_pD3DTextureTitle, D3DXVECTOR3(0.0f, 0.0f, NearZ()), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// "PRESS START"�`��
	DrawMesh(g_pD3DVtxBuffStart, g_pD3DTextureStart, D3DXVECTOR3(0.0f, -300.0f, NearZ()), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
 //   pDevice->SetStreamSource(0, g_pD3DVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//// ���_�t�H�[�}�b�g�̐ݒ�
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// �e�N�X�`���̐ݒ�
	//pDevice->SetTexture(0, g_pD3DTextureTitle);

	//// �|���S���̕`��
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	if(g_bDispStart == true)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureStart);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void SetColorTitleLogo(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffStart->Unlock();
	}

}

