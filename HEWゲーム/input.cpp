//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 尾形一統
//
//=============================================================================
#include "input.h"

#include <tchar.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)				// キー情報の最大値

// ゲームパッド用設定値
#define DEADZONE		(2500)	// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		(1000)	// 有効範囲の最大値
#define RANGE_MIN		(-1000)	// 有効範囲の最小値

#define KEY_ATTACK		DIK_X
#define KEY_JUMP		DIK_Z
#define KEY_CONFIRM		DIK_Z
#define KEY_LEFT		DIK_LEFT
#define KEY_RIGHT		DIK_RIGHT
#define KEY_UP			DIK_UP
#define KEY_DOWN		DIK_DOWN

#define PAD_ATTACK		BUTTON_A
#define PAD_JUMP		BUTTON_B
#define PAD_CONFIRM		BUTTON_B
#define PAD_LEFT		BUTTON_LEFT
#define PAD_RIGHT		BUTTON_RIGHT
#define PAD_UP			BUTTON_UP
#define PAD_DOWN		BUTTON_DOWN

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void	UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT	InitMouse(HINSTANCE hInst, HWND hWindow);	// マウスの初期化
void	UninitMouse(void);							// マウスの終了処理
HRESULT UpdateMouse(void);							// マウスの更新処理

HRESULT InitPad(void);								// パッドの初期化
void	UninitPad(void);							// パッド終了処理
void	UpdatePad(void);							// パッドの更新処理
// BOOL CALLBACK SearchPadCallback(LPDEVICEINSTANCE lpddi, LPVOID); // パッド検査コールバック

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//-------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

//-------------------------- mouse
static LPDIRECTINPUTDEVICE8	pMouse = NULL;	// mouse

static DIMOUSESTATE2		mouseState;		// マウスのダイレクトな状態
static DIMOUSESTATE2		mouseTrigger;	// 押された瞬間だけON

//-------------------------- game pad
static LPDIRECTINPUTDEVICE8 pGamePad[GAMEPADMAX] = {NULL, NULL, NULL, NULL};

static DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];
static int		padCount = 0;			// 検出したパッドの数

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// マウスの初期化
	InitMouse(hInst, hWnd);

	// パッドの初期化
	InitPad();

	return hr;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	// パッドの終了処理
	UninitPad();

	if(g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	// パッドの更新
	UpdatePad();
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, _T("キーボードがねぇ！"), _T("警告！"), MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, _T("キーボードのデータフォーマットを設定できませんでした。"), _T("警告！"), MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, _T("キーボードの協調モードを設定できませんでした。"), _T("警告！"), MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Unacquire();

		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if(g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if(g_keyStateRepeatCnt[cnt] >= 20)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		hr = g_pDIDevKeyboard->Acquire();
	}

	return hr;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// マウス関係の処理
//=============================================================================
// マウスの初期化
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT result;
	// デバイス作成
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWnd, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// ほかのアプリと強調モードに設定
	result = pMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// デバイスの設定
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;	// マウスの移動量　相対値

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse property erroe", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// アクセス権を得る
	pMouse->Acquire();
	return result;
}

void UninitMouse(void)
{
	if (pMouse)
	{
		pMouse->Unacquire();
		SAFE_RELEASE(pMouse);
	}
}

HRESULT UpdateMouse(void)
{
	HRESULT result;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mouseState;
	// データ取得
	result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// マウスのボタン状態
		for (int i = 0; i < 8; i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^ mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else // 取得失敗
	{
		// アクセス権を得てみる
		result = pMouse->Acquire();
	}
	return result;
}

BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
}

BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}

BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}

BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}

BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}

BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}

long GetMouseX(void)
{
	return mouseState.lX;
}

long GetMouseY(void)
{
	return mouseState.lY;
}

long GetMouseZ(void)
{
	return mouseState.lZ;
}

//======================================== game pad
//-------------------------------- 
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE; // 次のデバイスを列挙
}

HRESULT InitPad(void)		// パッド初期化
{
	HRESULT result;

	padCount = 0;
	// ジョイパッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が。パッドを発見した数だけ呼ばれる

	for (int i = 0; i < padCount; i++)
	{
		// ジョイスティック用のデータ・フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(result))
			return false; // データフォーマットの設定に失敗

		// モードを設定（フォアグラウンド＆非排他モード）
//		result = pGamePad[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if (FAILED(result))
//		return false; // モードの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする
		// (max-min)は最大10,000(?) (max-min)/2が中央値になる
		// 差を大きくすれば、アナログ値の細かな動きをとらえられる（パッドの性能による）
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに無効のゾーン値を設定する
		// 無効ゾーンとは、中央からの微小なジョイスティックの動きを無視する範囲のこと
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		// X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}

	return true;
}

void UninitPad(void)
{
	for (int i = 0; i < GAMEPADMAX; i++)
	{
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}
}

void UpdatePad(void)
{
	HRESULT		result;
	DIJOYSTATE2	dijs;

	for (int i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000L; // 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result))
		{
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
			{
				result = pGamePad[i]->Acquire();
			}
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
			{
				result = pGamePad[i]->Acquire();
			}
		}

		// 32の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		// 十字キー上
		if (dijs.rgdwPOV[0] == 0 )		padState[i] |= BUTTON_UP;
		// 十字キー右上
		if (dijs.rgdwPOV[0] == 4500)	padState[i] |= (BUTTON_RIGHT | BUTTON_UP);
		// 十字キー右
		if (dijs.rgdwPOV[0] == 9000)	padState[i] |= BUTTON_RIGHT;
		// 十字キー右下
		if (dijs.rgdwPOV[0] == 13500)	padState[i] |= (BUTTON_RIGHT | BUTTON_DOWN);
		// 十字キー下
		if (dijs.rgdwPOV[0] == 18000)	padState[i] |= BUTTON_DOWN;
		// 十字キー左下
		if (dijs.rgdwPOV[0] == 22500)	padState[i] |= (BUTTON_DOWN | BUTTON_LEFT);
		// 十字キー左
		if (dijs.rgdwPOV[0] == 27000)	padState[i] |= BUTTON_LEFT;
		// 十字キー左上
		if (dijs.rgdwPOV[0] == 31500)	padState[i] |= (BUTTON_LEFT | BUTTON_UP);
		// Ａボタン
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		// Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		// Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		// Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		// Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		// Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		// Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		// Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		// ＳＴＡＲＴボタン
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		// Ｍボタン
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;

		// Trigger設定
		padTrigger[i] = ((lastPadState ^ padState[i]) & padState[i]);  // 前回と違っていてしかも今ONのやつ
		// Release設定
		padRelease[i] = ((lastPadState ^ padState[i]) & ~padState[i]); // 前回と違っていてしかも今OFFのやつ
	}
}


BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}

BOOL IsButtonReleased(int padNo, DWORD button)
{
	return (button & padRelease[padNo]);
}


BOOL IsKeyOrButtonPressed(int pad_no, InputCommand input_command) {
	switch(input_command) {
	case kInputJump:
		if (IsButtonPressed(pad_no, PAD_JUMP) || GetKeyboardPress(KEY_JUMP))
			return true;
		break;
	case kInputAttack:
		if (IsButtonPressed(pad_no, PAD_ATTACK) || GetKeyboardPress(KEY_ATTACK))
			return true;
		break;
	case kInputConfirm:
		if (IsButtonPressed(pad_no, PAD_CONFIRM) || GetKeyboardPress(KEY_CONFIRM))
			return true;
		break;
	case kInputLeft:
		if (IsButtonPressed(pad_no, PAD_LEFT) || GetKeyboardPress(KEY_LEFT))
			return true;
		break;
	case kInputRight:
		if (IsButtonPressed(pad_no, PAD_RIGHT) || GetKeyboardPress(KEY_RIGHT))
			return true;
		break;
	case kInputUp:
		if (IsButtonPressed(pad_no, PAD_UP) || GetKeyboardPress(KEY_UP))
			return true;
		break;
	case kInputDown:
		if (IsButtonPressed(pad_no, PAD_DOWN) || GetKeyboardPress(KEY_DOWN))
			return true;
		break;
	}
	return false;
}

BOOL IsKeyOrButtonTriggered(int pad_no, InputCommand input_command) {
	switch (input_command) {
	case kInputJump:
		if (IsButtonTriggered(pad_no, PAD_JUMP) || GetKeyboardTrigger(KEY_JUMP))
			return true;
		break;
	case kInputAttack:
		if (IsButtonTriggered(pad_no, PAD_ATTACK) || GetKeyboardTrigger(KEY_ATTACK))
			return true;
		break;
	case kInputConfirm:
		if (IsButtonTriggered(pad_no, PAD_CONFIRM) || GetKeyboardTrigger(KEY_CONFIRM))
			return true;
		break;
	case kInputLeft:
		if (IsButtonTriggered(pad_no, PAD_LEFT) || GetKeyboardTrigger(KEY_LEFT))
			return true;
		break;
	case kInputRight:
		if (IsButtonTriggered(pad_no, PAD_RIGHT) || GetKeyboardTrigger(KEY_RIGHT))
			return true;
		break;
	case kInputUp:
		if (IsButtonTriggered(pad_no, PAD_UP) || GetKeyboardTrigger(KEY_UP))
			return true;
		break;
	case kInputDown:
		if (IsButtonTriggered(pad_no, PAD_DOWN) || GetKeyboardTrigger(KEY_DOWN))
			return true;
		break;
	}
	return false;
}

BOOL IsKeyOrButtonReleased(int pad_no, InputCommand input_command) {
	switch (input_command) {
	case kInputJump:
		if (IsButtonReleased(pad_no, PAD_JUMP) || GetKeyboardRelease(KEY_JUMP))
			return true;
		break;
	case kInputAttack:
		if (IsButtonReleased(pad_no, PAD_ATTACK) || GetKeyboardRelease(KEY_ATTACK))
			return true;
		break;
	case kInputConfirm:
		if (IsButtonReleased(pad_no, PAD_CONFIRM) || GetKeyboardRelease(KEY_CONFIRM))
			return true;
		break;
	case kInputLeft:
		if (IsButtonReleased(pad_no, PAD_LEFT) || GetKeyboardRelease(KEY_LEFT))
			return true;
		break;
	case kInputRight:
		if (IsButtonReleased(pad_no, PAD_RIGHT) || GetKeyboardRelease(KEY_RIGHT))
			return true;
		break;
	case kInputUp:
		if (IsButtonReleased(pad_no, PAD_UP) || GetKeyboardRelease(KEY_UP))
			return true;
		break;
	case kInputDown:
		if (IsButtonReleased(pad_no, PAD_DOWN) || GetKeyboardRelease(KEY_DOWN))
			return true;
		break;
	}
	return false;

}