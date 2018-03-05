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
	SOUND_LABEL_BGM000 = 0,			// BGM0
	SOUND_LABEL_BGM001,				// BGM1
	SOUND_LABEL_SE_SHOT,			// 弾発射音
	SOUND_LABEL_SE_EXPLOSION,		// 爆発音
	SOUND_LABEL_SE_COIN,			// 衝突音0
	SOUND_LABEL_COUNT,				// カウントオン
	SOUND_LABEL_GAMESTART,			// ゲームスタート
	SOUND_LABEL_GAMESET,			// ゲームセット
	SOUND_LABEL_SKILL_GAGEON,		// スキルゲージがたまった
	SOUND_LABEL_SKILL_GAGECHARGE,	// スキルゲージ蓄積
	SOUND_LABEL_SKILL_OJYAMA,		// おジャマスキル発動
	SOUND_LABEL_SKILL_SPEEDDOWN,	// 減速
	SOUND_LABEL_SKILL_SPEEDEUP,		// 加速
	SOUND_LABEL_SKILL_THANDER,		// 雷
	SOUND_LABEL_RENDA,				// 連打音
	SOUND_LABEL_DAMEGE,				// ダメージ音
	SOUND_LABEL_LIFE0,				// ライフ0の音
	SOUND_LABEL_POSE,				// ポーズ音
	SOUND_LABEL_MINIMAM,			// ミニマム音
	SOUND_LABEL_RESULT1,			// リザルト音１
	SOUND_LABEL_RESULT2,			// リザルト音2
	SOUND_LABEL_SMALLJUMP,			// 小ジャンプ
	SOUND_LABEL_HIGHJUMP,			// 大ジャンプ

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
