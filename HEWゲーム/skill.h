/***************************************************************
* ファイル名:	skill.h
* 制作者:		HAL東京ゲーム学科 山本 壮将
* 制作日:		2月23日
* 説明:			スキル関連のヘッダー
****************************************************************/
#ifndef _SKILL_H_
#define _SKILL_H_

//**************************************************************
// インクルードファイル
//**************************************************************
#include "main.h"
#include "player.h"

//**************************************************************
// マクロ定義
//**************************************************************

//**************************************************************
// 構造体定義
//**************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9			Texture_bar;
	LPDIRECT3DTEXTURE9			Texture_waku;
	LPDIRECT3DVERTEXBUFFER9		Buff_bar;
	LPDIRECT3DVERTEXBUFFER9		Buff_waku;
	float						gauge;								// 今のゲージの値
	float						gage_lvup;							// レベルアップに必要な値
	int							lv;									// スキルのレベル
	int							skill_player_id;					// スキル発動権を持つプレイヤー番号（-1のときは誰も持っていない）
	bool						moving;								// スキルは今実行されている？

}SKILL;

typedef struct
{
	bool					get;					// 権限持ってる？
	int						count;
	int						flag_no;				// フラグ管理用
	int						use_count;				// スキルを使った回数

}SKILL_FLAG;

typedef enum
{
	SPEEDCHANGE = 1,
	OJYAMA,
	KAMINARI,
	SKILLEFFECT_MAX

}SKILLEFFECT;

//**************************************************************
// プロトタイプ宣言
//**************************************************************
HRESULT InitSkill(void);
void UninitSkill(void);
void AddSkillGauge(float gageup);
void DrawSkill(void);
HRESULT MakeVertexSkill(LPDIRECT3DDEVICE9 pDevice);
void SetColorSkill(void);
void GiveSkillUsingRight(void);
void GetSkill(void);
void SkillReset(int no);
SKILL *GetSkillWk(int no);
void skillwinner(int no);
void SkillAct(int player_no);
void UpdateSkillAct(void);

#endif