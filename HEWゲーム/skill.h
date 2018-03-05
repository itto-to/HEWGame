/***************************************************************
* �t�@�C����:	skill.h
* �����:		HAL�����Q�[���w�� �R�{ �s��
* �����:		2��23��
* ����:			�X�L���֘A�̃w�b�_�[
****************************************************************/
#ifndef _SKILL_H_
#define _SKILL_H_

//**************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************
#include "main.h"
#include "player.h"

//**************************************************************
// �}�N����`
//**************************************************************

//**************************************************************
// �\���̒�`
//**************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9			Texture_bar;
	LPDIRECT3DTEXTURE9			Texture_waku;
	LPDIRECT3DVERTEXBUFFER9		Buff_bar;
	LPDIRECT3DVERTEXBUFFER9		Buff_waku;
	float					gage;								// ���̃Q�[�W�̒l
	float					gage_lvup;							// ���x���A�b�v�ɕK�v�Ȓl
	int						lv;									// �X�L���̃��x��
	bool					kengen;								// �����͊��Ɋ���ӂĂ��Ă���H


}SKILL;

typedef struct
{
	bool					get;					// ���������Ă�H
	int						count;
	int						flag_no;				// �t���O�Ǘ��p
	int						use_count;				// �X�L�����g������

}SKILL_FLAG;

typedef enum
{
	SPEEDCHANGE,
	OJYAMA,
	KAMINARI,
	SKILLEFFECT_MAX

}SKILLEFFECT;

//**************************************************************
// �v���g�^�C�v�錾
//**************************************************************
HRESULT InitSkill(void);
void UninitSkill(void);
void UpdateSkill(float gageup);
void DrawSkill(void);
HRESULT MakeVertexSkill(LPDIRECT3DDEVICE9 pDevice);
void SetColorSkill(void);
void GetSkill(int no);
void SkillReset(int no);
SKILL *GetSkillWk(int no);

#endif