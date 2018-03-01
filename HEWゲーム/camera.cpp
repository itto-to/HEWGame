//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "camera.h"

#include "collision.h"
#include "debugproc.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// ����p
#define	VIEW_NEAR_Z			(10.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(1000.0f)				// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)					// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// �J�����̉�]��

#define	INTERVAL_CAMERA_R	(12.5f)					// ���f���̎����̐�܂ł̋���
#define	RATE_CHASE_CAMERA_P	(0.35f)					// �J�����̎��_�ւ̕␳�W��
#define	RATE_CHASE_CAMERA_R	(0.20f)					// �J�����̒����_�ւ̕␳�W��

#define	CHASE_HEIGHT_P		(100.0f)				// �ǐՎ��̎��_�̍���
#define	CHASE_HEIGHT_R		(10.0f)					// �ǐՎ��̒����_�̍���

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {
	D3DXVECTOR3		posEye;					// �J�����̎��_
	D3DXVECTOR3		posAt;					// �J�����̒����_
	D3DXVECTOR3		vecUp;					// �J�����̏����
	D3DXVECTOR3		posEyeDest;				// �J�����̎��_�̖ړI�ʒu
	D3DXVECTOR3		posAtDest;				// �J�����̒����_�̖ړI�ʒu
	D3DXVECTOR3		rotCamera;				// �J�����̉�]
	float			lengthIntervalCamera;	// �J�����̎��_�ƒ����_�̋���
	float			lengthIntervalPlayer;	// �v���C���[�ƒ����_�̋���
	D3DXMATRIX		mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	BOUNDING_BOX	screenBox;				// ��ʊO����p�o�E���f�B���O�{�b�N�X
} CAMERA;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA g_camera;

//=============================================================================
// �J�����̏�����
//=============================================================================
HRESULT InitCamera(void)
{
	g_camera.posEye = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	g_camera.posAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	g_camera.vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.posEyeDest = g_camera.posEye;
	g_camera.posAtDest = g_camera.posAtDest;
	g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ��ʊO����p�o�E���f�B���O�{�b�N�X������
	g_camera.screenBox.max = D3DXVECTOR3(g_camera.posEye.x + SCREEN_WIDTH / 2.0f, g_camera.posEye.y + SCREEN_HEIGHT / 2.0f, g_camera.posEye.z + VIEW_FAR_Z);
	g_camera.screenBox.min = D3DXVECTOR3(g_camera.posEye.x - SCREEN_WIDTH / 2.0f, g_camera.posEye.y - SCREEN_HEIGHT / 2.0f, g_camera.posEye.z + VIEW_NEAR_Z);

	//float vx,vz;
	//vx = g_camera.posEye.x - g_camera.posAt.x;
	//vz = g_posCameraEye.z - g_posCameraAt.z;
	//g_fLengthIntervalCamera = sqrtf(vx * vx + vz * vz);
	//g_fLengthIntervalPlayer = INTERVAL_CAMERA_R;

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
#ifdef _DEBUG
	PrintDebugProc("�J�������_   X:%f, Y:%f, Z:%f\n", g_camera.posEye.x, g_camera.posEye.y, g_camera.posEye.z);
	PrintDebugProc("�J���������_ X:%f, Y:%f, Z:%f\n", g_camera.posAt.x, g_camera.posAt.y, g_camera.posAt.z);
#endif
	//D3DXVECTOR3 posPlayer;
	//D3DXVECTOR3 rotPlayer;
	//D3DXVECTOR3 movePlayer;
	//float fLength;

	//// ���f���̈ʒu�擾
	//posPlayer = GetPositionPlayer();

	//// ���f���̖ړI�̌����擾
	//rotPlayer = GetRotationPlayer();

	//// ���f���̈ړ��ʎ擾
	//movePlayer = GetMovePlayer();

	//// ���_�̖ړI�ʒu
	//fLength = sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraPDest.x = posPlayer.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera - sin(rotPlayer.y) * fLength;
	//g_posCameraPDest.y = posPlayer.y + CHASE_HEIGHT_P;
	//g_posCameraPDest.z = posPlayer.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera - cos(rotPlayer.y) * fLength;

	//// �����_�̖ړI�ʒu
	//fLength = g_fLengthIntervalPlayer + sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraRDest.x = posPlayer.x - sin(rotPlayer.y) * fLength;
	//g_posCameraRDest.y = posPlayer.y - CHASE_HEIGHT_R;
	//g_posCameraRDest.z = posPlayer.z - cos(rotPlayer.y) * fLength;

	//// ���_�̕␳
	//g_posCameraEye.x += (g_posCameraPDest.x - g_posCameraEye.x) * RATE_CHASE_CAMERA_P;
	//g_posCameraEye.y += (g_posCameraPDest.y - g_posCameraEye.y) * RATE_CHASE_CAMERA_P;
	//g_posCameraEye.z += (g_posCameraPDest.z - g_posCameraEye.z) * RATE_CHASE_CAMERA_P;

	//// �����_�̕␳
	//g_posCameraAt.x += (g_posCameraRDest.x - g_posCameraAt.x) * RATE_CHASE_CAMERA_R;
	//g_posCameraAt.y += (g_posCameraRDest.y - g_posCameraAt.y) * RATE_CHASE_CAMERA_R;
	//g_posCameraAt.z += (g_posCameraRDest.z - g_posCameraAt.z) * RATE_CHASE_CAMERA_R;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	CAMERA *camera = &g_camera;

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&camera->mtxView, 
						&camera->posEye,		// �J�����̎��_
						&camera->posAt,			// �J�����̒����_
						&camera->vecUp);		// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixOrthoLH(&camera->mtxProjection, SCREEN_WIDTH, SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}



//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_camera.mtxView;
}


//=============================================================================
// �I�u�W�F�N�g����ʊO�ɂł���true��Ԃ�
//=============================================================================
bool IsObjectOffscreen(BOUNDING_BOX objectBox)
{
	BOUNDING_BOX cameraBox;
	cameraBox.min = g_camera.posEye;
	if (IsIntersectedBoundingBox(g_camera.screenBox, objectBox)) {
		return false;
	}

	return true;
}