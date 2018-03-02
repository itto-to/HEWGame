//=============================================================================
//
// 入力処理ヘッダー [input.h]
// Author : 尾形一統
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// プログラム分けするときに使う
#define USE_KEYBOARD	// 宣言するとキーボードで操作可能になる
#define USE_MOUSE		// 宣言するとマウスで操作可能になる
#define USE_PAD			// 宣言するとパッドで操作可能になる

// game pad情報
#define BUTTON_UP		0x00000001L	// 方向キー上(.IY < 0)
#define BUTTON_DOWN		0x00000002L	// 方向キー下(.IY > 0)
#define BUTTON_LEFT		0x00000004L	// 方向キー左(.IX < 0)
#define BUTTON_RIGHT	0x00000008L	// 方向キー右(.IX > 0)
#define BUTTON_A		0x00000010L	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020L	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040L	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080L	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100L	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200L	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400L	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800L	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000L	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000L	// Ｍボタン(.rgbButtons[9]&0x80)
#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット

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
// プロトタイプ宣言
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
BOOL IsMouseLeftPressed(void);		// 左クリックした状態
BOOL IsMouseLeftTriggered(void);	// 左クリックした瞬間
BOOL IsMouseRightPressed(void);		// 右クリックした状態
BOOL IsMouseRightTriggered(void);	// 右クリックした瞬間
BOOL IsMouseCenterPressed(void);	// 中クリックした状態
BOOL IsMouseCenterTriggered(void);	// 中クリックした瞬間
long GetMouseX(void);				// マウスがX方向に動いた相対値
long GetMouseY(void);				// マウスがY方向に動いた相対値
long GetMouseZ(void);				// マウスホイールが動いた相対値

//-------------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);
BOOL IsButtonReleased(int padNo, DWORD button);

BOOL IsKeyOrButtonPressed(int pad_no, InputCommand input_command);
BOOL IsKeyOrButtonTriggered(int pad_no, InputCommand input_command);
BOOL IsKeyOrButtonReleased(int pad_no, InputCommand input_command);

#endif // _INPUT_H_