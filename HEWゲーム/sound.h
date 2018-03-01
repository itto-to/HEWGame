//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_COIN,		// 衝突音0
	SOUND_LABEL_MAX,
} SoundLabel;

//typedef enum
//{
//	kSoundBgm00 = 0,		// BGM0
//	kSoundBgm01,			// BGM1
//	kSoundShot,		// 弾発射音
//	kSoundExplosion,	// 爆発音
//	kSoundCoin,		// 衝突音0
//	kSoundLabelMax,
//} SoundLabel;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SoundLabel label);
void StopSound(SoundLabel label);
void StopSound(void);

#endif
