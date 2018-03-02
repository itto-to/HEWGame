//=============================================================================
//
// ���͏����w�b�_�[ [input.h]
// Author : ���`�ꓝ
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �v���O������������Ƃ��Ɏg��
#define USE_KEYBOARD	// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define USE_MOUSE		// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define USE_PAD			// �錾����ƃp�b�h�ő���\�ɂȂ�

// game pad���
#define BUTTON_UP		0x00000001L	// �����L�[��(.IY < 0)
#define BUTTON_DOWN		0x00000002L	// �����L�[��(.IY > 0)
#define BUTTON_LEFT		0x00000004L	// �����L�[��(.IX < 0)
#define BUTTON_RIGHT	0x00000008L	// �����L�[�E(.IX > 0)
#define BUTTON_A		0x00000010L	// �`�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020L	// �a�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040L	// �b�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080L	// �w�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100L	// �x�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200L	// �y�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400L	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800L	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000L	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000L	// �l�{�^��(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

enum InputCommand {
	kInputJump,
	kInputAttack,
	kInputLeft,
	kInputRight,
	kInputUp,
	kInputDown,
	kInputConfirm
};





//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//-------------------------------- keyboard
bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

//-------------------------------- mouse
BOOL IsMouseLeftPressed(void);		// ���N���b�N�������
BOOL IsMouseLeftTriggered(void);	// ���N���b�N�����u��
BOOL IsMouseRightPressed(void);		// �E�N���b�N�������
BOOL IsMouseRightTriggered(void);	// �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);	// ���N���b�N�������
BOOL IsMouseCenterTriggered(void);	// ���N���b�N�����u��
long GetMouseX(void);				// �}�E�X��X�����ɓ��������Βl
long GetMouseY(void);				// �}�E�X��Y�����ɓ��������Βl
long GetMouseZ(void);				// �}�E�X�z�C�[�������������Βl

//-------------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);
BOOL IsButtonReleased(int padNo, DWORD button);

BOOL IsKeyOrButtonPressed(int pad_no, InputCommand input_command);
BOOL IsKeyOrButtonTriggered(int pad_no, InputCommand input_command);
BOOL IsKeyOrButtonReleased(int pad_no, InputCommand input_command);

#endif // _INPUT_H_