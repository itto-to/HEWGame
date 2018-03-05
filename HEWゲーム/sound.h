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
	SOUND_LABEL_BGM000 = 0,			// BGM0
	SOUND_LABEL_BGM001,				// BGM1
	SOUND_LABEL_SE_SHOT,			// �e���ˉ�
	SOUND_LABEL_SE_EXPLOSION,		// ������
	SOUND_LABEL_SE_COIN,			// �Փˉ�0
	SOUND_LABEL_COUNT,				// �J�E���g�I��
	SOUND_LABEL_GAMESTART,			// �Q�[���X�^�[�g
	SOUND_LABEL_GAMESET,			// �Q�[���Z�b�g
	SOUND_LABEL_SKILL_GAGEON,		// �X�L���Q�[�W�����܂���
	SOUND_LABEL_SKILL_GAGECHARGE,	// �X�L���Q�[�W�~��
	SOUND_LABEL_SKILL_OJYAMA,		// ���W���}�X�L������
	SOUND_LABEL_SKILL_SPEEDDOWN,	// ����
	SOUND_LABEL_SKILL_SPEEDEUP,		// ����
	SOUND_LABEL_SKILL_THANDER,		// ��
	SOUND_LABEL_RENDA,				// �A�ŉ�
	SOUND_LABEL_DAMEGE,				// �_���[�W��
	SOUND_LABEL_LIFE0,				// ���C�t0�̉�
	SOUND_LABEL_POSE,				// �|�[�Y��
	SOUND_LABEL_MINIMAM,			// �~�j�}����
	SOUND_LABEL_RESULT1,			// ���U���g���P
	SOUND_LABEL_RESULT2,			// ���U���g��2
	SOUND_LABEL_SMALLJUMP,			// ���W�����v
	SOUND_LABEL_HIGHJUMP,			// ��W�����v

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
