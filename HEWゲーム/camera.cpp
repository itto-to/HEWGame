//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "camera.h"

#include "collision.h"
#include "debugproc.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// 視野角
#define	VIEW_NEAR_Z			(10.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(1000.0f)				// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA	(2.0f)					// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// カメラの回転量

#define	INTERVAL_CAMERA_R	(12.5f)					// モデルの視線の先までの距離
#define	RATE_CHASE_CAMERA_P	(0.35f)					// カメラの視点への補正係数
#define	RATE_CHASE_CAMERA_R	(0.20f)					// カメラの注視点への補正係数

#define	CHASE_HEIGHT_P		(100.0f)				// 追跡時の視点の高さ
#define	CHASE_HEIGHT_R		(10.0f)					// 追跡時の注視点の高さ

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	D3DXVECTOR3		posEye;					// カメラの視点
	D3DXVECTOR3		posAt;					// カメラの注視点
	D3DXVECTOR3		vecUp;					// カメラの上方向
	D3DXVECTOR3		posEyeDest;				// カメラの視点の目的位置
	D3DXVECTOR3		posAtDest;				// カメラの注視点の目的位置
	D3DXVECTOR3		rotCamera;				// カメラの回転
	float			lengthIntervalCamera;	// カメラの視点と注視点の距離
	float			lengthIntervalPlayer;	// プレイヤーと注視点の距離
	D3DXMATRIX		mtxView;				// ビューマトリックス
	D3DXMATRIX		mtxProjection;			// プロジェクションマトリックス
	BOUNDING_BOX	screenBox;				// 画面外判定用バウンディングボックス
} CAMERA;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA g_camera;

//=============================================================================
// カメラの初期化
//=============================================================================
HRESULT InitCamera(void)
{
	g_camera.posEye = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	g_camera.posAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	g_camera.vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.posEyeDest = g_camera.posEye;
	g_camera.posAtDest = g_camera.posAtDest;
	g_camera.rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 画面外判定用バウンディングボックス初期化
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
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
#ifdef _DEBUG
	PrintDebugProc("カメラ視点   X:%f, Y:%f, Z:%f\n", g_camera.posEye.x, g_camera.posEye.y, g_camera.posEye.z);
	PrintDebugProc("カメラ注視点 X:%f, Y:%f, Z:%f\n", g_camera.posAt.x, g_camera.posAt.y, g_camera.posAt.z);
#endif
	//D3DXVECTOR3 posPlayer;
	//D3DXVECTOR3 rotPlayer;
	//D3DXVECTOR3 movePlayer;
	//float fLength;

	//// モデルの位置取得
	//posPlayer = GetPositionPlayer();

	//// モデルの目的の向き取得
	//rotPlayer = GetRotationPlayer();

	//// モデルの移動量取得
	//movePlayer = GetMovePlayer();

	//// 視点の目的位置
	//fLength = sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraPDest.x = posPlayer.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera - sin(rotPlayer.y) * fLength;
	//g_posCameraPDest.y = posPlayer.y + CHASE_HEIGHT_P;
	//g_posCameraPDest.z = posPlayer.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera - cos(rotPlayer.y) * fLength;

	//// 注視点の目的位置
	//fLength = g_fLengthIntervalPlayer + sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraRDest.x = posPlayer.x - sin(rotPlayer.y) * fLength;
	//g_posCameraRDest.y = posPlayer.y - CHASE_HEIGHT_R;
	//g_posCameraRDest.z = posPlayer.z - cos(rotPlayer.y) * fLength;

	//// 視点の補正
	//g_posCameraEye.x += (g_posCameraPDest.x - g_posCameraEye.x) * RATE_CHASE_CAMERA_P;
	//g_posCameraEye.y += (g_posCameraPDest.y - g_posCameraEye.y) * RATE_CHASE_CAMERA_P;
	//g_posCameraEye.z += (g_posCameraPDest.z - g_posCameraEye.z) * RATE_CHASE_CAMERA_P;

	//// 注視点の補正
	//g_posCameraAt.x += (g_posCameraRDest.x - g_posCameraAt.x) * RATE_CHASE_CAMERA_R;
	//g_posCameraAt.y += (g_posCameraRDest.y - g_posCameraAt.y) * RATE_CHASE_CAMERA_R;
	//g_posCameraAt.z += (g_posCameraRDest.z - g_posCameraAt.z) * RATE_CHASE_CAMERA_R;
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	CAMERA *camera = &g_camera;

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camera->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&camera->mtxView, 
						&camera->posEye,		// カメラの視点
						&camera->posAt,			// カメラの注視点
						&camera->vecUp);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	// プロジェクションマトリックスの初期化
	D3DXMatrixOrthoLH(&camera->mtxProjection, SCREEN_WIDTH, SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}



//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_camera.mtxView;
}


//=============================================================================
// オブジェクトが画面外にでたらtrueを返す
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