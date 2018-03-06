//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : 
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "mesh.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/bg_result.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO	"data/TEXTURE/result_logo.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	RESULT_LOGO_POS_X	(240)		// ���U���g���S�̈ʒu(X���W)
#define	RESULT_LOGO_POS_Y	(320)		// ���U���g���S�̈ʒu(Y���W)
#define	RESULT_LOGO_WIDTH	(800)		// ���U���g���S�̕�
#define	RESULT_LOGO_HEIGHT	(240)		// ���U���g���S�̍���

#define	COUNT_APPERA_RESULT	(60)		// ���U���g���S�o���܂ł̑҂�����	
#define	LIMIT_COUNT_WAIT	(60 * 5)	// �҂�����


// �i���o�[
#define RESULTNO_HEIGHT	(SCREEN_HEIGHT /4)
#define RESULTNO_WIDTH	(SCREEN_HEIGHT /4)
#define RESULTNO_1	("data/TEXTURE/result_1st.png")
#define RESULTNO_2	("data/TEXTURE/result_2nd.png")
#define RESULTNO_3	("data/TEXTURE/result_3rd.png")
#define RESULTNO_4	("data/TEXTURE/result_4th.png")

#define MAX_RANK	(4)				// �т�̂����

// ���ʂ̕\���ʒu
#define RANKNO_POS_X			(30.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void SetColorResultLogo(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int						g_nCountAppearResult = 0;		// �o���܂ł̑҂�����
float					g_fAlphaResult = 0.0f;			// ���U���g���S�̃��l
int						g_nCountWaitResult = 0;			// �҂�����
int						checkcount;					// �����v���C���[���J�E���g
RESULT resultWk[MAX_PLAYER];

// ���������͐�����\�����邽�߂̂���
RESULTNO resultno[4];

char *resultno_FileName[] =
{
	RESULTNO_1,
	RESULTNO_2,
	RESULTNO_3,
	RESULTNO_4,
};

bool rank_ok[MAX_PLAYER];
//*******************************************************
// void InitCount(void)
// �J�E���g������
// result.cpp�ւ̈ړ��ɗ��p����checkcount�̒l��������
// �t���O�������ŏ�����
//*******************************************************
void InitCount(void)
{
	checkcount = MAX_PLAYER;
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		rank_ok[i] = false;
	}

	for(int i = 0; i < MAX_PLAYER; i++)
	{
		resultWk[i].rank = 0;			// �S���̃����N��1�ʂ�
	}
}
//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearResult = 0;
	g_fAlphaResult = 0.0f;
	g_nCountWaitResult = 0;

	// ���_���̍쐬
	MakeVertexResult(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RESULT,				// �t�@�C���̖��O
		&g_pD3DTextureResult);		// �ǂݍ��ރ������[

									// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RESULT_LOGO,		// �t�@�C���̖��O
		&g_pD3DTextureResultLogo);	// �ǂݍ��ރ������[

	for(int i = 0; i < MAX_RANK; i++)
	{
		// ���ʕ\���p�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			resultno_FileName[i],
			&resultno[i].texture);
	}

	checkcount = MAX_PLAYER;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if(g_pD3DTextureResult != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if(g_pD3DVtxBuffResult != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffResult->Release();
		g_pD3DVtxBuffResult = NULL;
	}

	if(g_pD3DTextureResultLogo != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}

	if(g_pD3DVtxBuffResultLogo != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffResultLogo->Release();
		g_pD3DVtxBuffResultLogo = NULL;
	}

	for(int i = 0; i < MAX_RANK; i++)
	{
		if(resultno[i].texture != NULL)
		{// �J��
			resultno[i].buff->Release();
			resultno[i].buff = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	g_nCountAppearResult++;
	if(g_nCountAppearResult >= COUNT_APPERA_RESULT)
	{
		if(g_fAlphaResult < 1.0f)
		{
			g_fAlphaResult += 0.05f;
			if(g_fAlphaResult >= 1.0f)
			{
				g_fAlphaResult = 1.0f;
			}

			SetColorResultLogo();
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�t�F�[�h�A�E�g���ă��[�h��؂�ւ�����
		SetFade(FADE_OUT);
	}

	/*g_nCountWaitResult++;
	if(g_nCountWaitResult > LIMIT_COUNT_WAIT)
	{
	SetFade(FADE_OUT);
	}*/
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ���S
	//************************************************************************************

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResultLogo);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// �Ă��Ɓ@���
	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		

		pDevice->SetStreamSource(0, resultno[i].buff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, resultno[resultWk[i].rank].texture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffResult,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffResult->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffResultLogo,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffResultLogo->Unlock();
	}


	//+++++++++++++++++++++++++++++++
	// ��������͏��ʂɂ���ĕς��
	//+++++++++++++++++++++++++++++++
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&resultno[i].buff,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			resultno[i].buff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f + (i*RESULTNO_HEIGHT), 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RESULTNO_WIDTH, 0.0f + (i*RESULTNO_HEIGHT), 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, RESULTNO_HEIGHT + (i*RESULTNO_HEIGHT), 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RESULTNO_WIDTH, RESULTNO_HEIGHT+ (i*RESULTNO_HEIGHT), 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			resultno[i].buff->Unlock();
		}
	}
	/*for(int i = 0; i < MAX_RANK; i++)
	{
		MakeVertex(pDevice, &resultno[i].buff, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), RESULTNO_WIDTH, RESULTNO_HEIGHT);
	}*/
	return S_OK;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorResultLogo(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffResultLogo->Unlock();
	}
}


//*********************************************************************
// void RankCheck(void)
// ����: �v���C���[�̃��C�t���X�V�̌���0�ɂȂ����ꍇ���s�����
//*********************************************************************
void RankCheck(int no, int rank)
{

	// �󂯎����no�̃v���C���[�����ɗ��E���Ă��Ȃ���Ύ��s
	if(rank_ok[no] == false)
	{
		rank_ok[no] = true;
		// �L�����N�^�[�̐����������炷
		checkcount--;
		// �����N������

		resultWk[no].rank = rank-1;			// rank�̒l���1���Ȃ��ԍ�������
		// �����Ă���L�����N�^�[�����Ȃ��ꍇ���U���g��ʂ�
		if(checkcount < 1)
		{
			SetFade(FADE_OUT);
			//PlaySound(SOUND_LABEL_GAMESTART);
		}
	}
}

//*********************************************************************
// RESULT GetResult(int no)
// �Q�b�g�֐�
// �w�肳�ꂽ�ԍ���Ԃ�
//*********************************************************************
RESULT *GetResult(int no)
{
	return &resultWk[no];
}
