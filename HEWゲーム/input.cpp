//=============================================================================
//
// ���͏��� [input.cpp]
// Author : ���`�ꓝ
//
//=============================================================================
#include "input.h"

#include <tchar.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)				// �L�[���̍ő�l

// �Q�[���p�b�h�p�ݒ�l
#define DEADZONE		(2500)	// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		(1000)	// �L���͈͂̍ő�l
#define RANGE_MIN		(-1000)	// �L���͈͂̍ŏ��l

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
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void	UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT	InitMouse(HINSTANCE hInst, HWND hWindow);	// �}�E�X�̏�����
void	UninitMouse(void);							// �}�E�X�̏I������
HRESULT UpdateMouse(void);							// �}�E�X�̍X�V����

HRESULT InitPad(void);								// �p�b�h�̏�����
void	UninitPad(void);							// �p�b�h�I������
void	UpdatePad(void);							// �p�b�h�̍X�V����
// BOOL CALLBACK SearchPadCallback(LPDEVICEINSTANCE lpddi, LPVOID); // �p�b�h�����R�[���o�b�N

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//-------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

//-------------------------- mouse
static LPDIRECTINPUTDEVICE8	pMouse = NULL;	// mouse

static DIMOUSESTATE2		mouseState;		// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2		mouseTrigger;	// �����ꂽ�u�Ԃ���ON

//-------------------------- game pad
static LPDIRECTINPUTDEVICE8 pGamePad[GAMEPADMAX] = {NULL, NULL, NULL, NULL};

static DWORD	padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];
static int		padCount = 0;			// ���o�����p�b�h�̐�

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitMouse(hInst, hWnd);

	// �p�b�h�̏�����
	InitPad();

	return hr;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	// �p�b�h�̏I������
	UninitPad();

	if(g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();

	// �p�b�h�̍X�V
	UpdatePad();
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, _T("�L�[�{�[�h���˂��I"), _T("�x���I"), MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, _T("�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B"), _T("�x���I"), MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, _T("�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B"), _T("�x���I"), MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
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
// �L�[�{�[�h�̍X�V����
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		hr = g_pDIDevKeyboard->Acquire();
	}

	return hr;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================
// �}�E�X�̏�����
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWnd, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �ق��̃A�v���Ƌ������[�h�ɐݒ�
	result = pMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;	// �}�E�X�̈ړ��ʁ@���Βl

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse property erroe", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �A�N�Z�X���𓾂�
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
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = mouseState;
	// �f�[�^�擾
	result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i < 8; i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^ mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else // �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = pMouse->Acquire();
	}
	return result;
}

BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
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
	return DIENUM_CONTINUE; // ���̃f�o�C�X���
}

HRESULT InitPad(void)		// �p�b�h������
{
	HRESULT result;

	padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����B�p�b�h�𔭌������������Ă΂��

	for (int i = 0; i < padCount; i++)
	{
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		result = pGamePad[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if (FAILED(result))
//		return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����
		// (max-min)�͍ő�10,000(?) (max-min)/2�������l�ɂȂ�
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ������Ƃ炦����i�p�b�h�̐��\�ɂ��j
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����Ƃɖ����̃]�[���l��ݒ肷��
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂���
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		// X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// �W���C�X�e�B�b�N���͐���J�n
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
		padState[i] = 0x00000000L; // ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result))
		{
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
			{
				result = pGamePad[i]->Acquire();
			}
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
			{
				result = pGamePad[i]->Acquire();
			}
		}

		// 32�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		// �\���L�[��
		if (dijs.rgdwPOV[0] == 0 )		padState[i] |= BUTTON_UP;
		// �\���L�[�E��
		if (dijs.rgdwPOV[0] == 4500)	padState[i] |= (BUTTON_RIGHT | BUTTON_UP);
		// �\���L�[�E
		if (dijs.rgdwPOV[0] == 9000)	padState[i] |= BUTTON_RIGHT;
		// �\���L�[�E��
		if (dijs.rgdwPOV[0] == 13500)	padState[i] |= (BUTTON_RIGHT | BUTTON_DOWN);
		// �\���L�[��
		if (dijs.rgdwPOV[0] == 18000)	padState[i] |= BUTTON_DOWN;
		// �\���L�[����
		if (dijs.rgdwPOV[0] == 22500)	padState[i] |= (BUTTON_DOWN | BUTTON_LEFT);
		// �\���L�[��
		if (dijs.rgdwPOV[0] == 27000)	padState[i] |= BUTTON_LEFT;
		// �\���L�[����
		if (dijs.rgdwPOV[0] == 31500)	padState[i] |= (BUTTON_LEFT | BUTTON_UP);
		// �`�{�^��
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		// �a�{�^��
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		// �b�{�^��
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		// �w�{�^��
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		// �x�{�^��
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		// �y�{�^��
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		// �k�{�^��
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		// �q�{�^��
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		// �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		// �l�{�^��
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;

		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i]) & padState[i]);  // �O��ƈ���Ă��Ă�������ON�̂��
		// Release�ݒ�
		padRelease[i] = ((lastPadState ^ padState[i]) & ~padState[i]); // �O��ƈ���Ă��Ă�������OFF�̂��
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