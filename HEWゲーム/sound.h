//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// �T�E���h�t�@�C��
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_COIN,		// �Փˉ�0
	SOUND_LABEL_MAX,
} SoundLabel;

//typedef enum
//{
//	kSoundBgm00 = 0,		// BGM0
//	kSoundBgm01,			// BGM1
//	kSoundShot,		// �e���ˉ�
//	kSoundExplosion,	// ������
//	kSoundCoin,		// �Փˉ�0
//	kSoundLabelMax,
//} SoundLabel;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SoundLabel label);
void StopSound(SoundLabel label);
void StopSound(void);

#endif
