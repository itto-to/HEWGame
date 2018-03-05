//*************************************************
// skillact.h
// 最終更新　3月4日
// 制作者 山本壮将
//*************************************************
#ifndef _SKILLACT_H_
#define _SKILLACT_H_

//*************************************************
// インクルードファイル
//*************************************************
#include "main.h"

//*************************************************
// マクロ定義
//*************************************************
// 秒数
#define FPS			(60)			// 1秒のフレーム数
#define ACTTIME_LV1	(5*FPS)			// スキルレベル1効果発動時間
#define ACTTIME_LV2	(7*FPS)			// スキルレベル2効果発動時間

//*************************************************
// 構造体定義
//*************************************************
typedef struct
{
	bool						Up_active;
	bool						Down_active;
	bool						Ojyama_active;
	bool						Kaminari_active;
	LPDIRECT3DTEXTURE9			Up_texture;
	LPDIRECT3DVERTEXBUFFER9		Up_buff;
	LPDIRECT3DTEXTURE9			Down_texture;
	LPDIRECT3DVERTEXBUFFER9		Down_buff;
	LPDIRECT3DTEXTURE9			Ojyama_texture;
	LPDIRECT3DVERTEXBUFFER9		Ojyama_buff;
	LPDIRECT3DTEXTURE9			Kaminari_texture;
	LPDIRECT3DVERTEXBUFFER9		Kaminari_buff;
	D3DXVECTOR3					Up_pos;
	D3DXVECTOR3					Up_scl;
	D3DXVECTOR3					Up_move;
	D3DXVECTOR3					Up_rot;
	D3DXMATRIX					Up_mtxWorld;

	D3DXVECTOR3					Down_pos;
	D3DXVECTOR3					Down_scl;
	D3DXVECTOR3					Down_move;
	D3DXVECTOR3					Down_rot;
	D3DXMATRIX					Down_mtxWorld;

	D3DXVECTOR3					Ojyama_pos;
	D3DXVECTOR3					Ojyama_scl;
	D3DXVECTOR3					Ojyama_move;
	D3DXVECTOR3					Ojyama_rot;
	D3DXMATRIX					Ojyama_mtxWorld;

	D3DXVECTOR3					Kaminari_pos;
	D3DXVECTOR3					Kaminari_scl;
	D3DXVECTOR3					Kaminari_move;
	D3DXVECTOR3					Kaminari_rot;
	D3DXMATRIX					Kaminari_mtxWorld;

	int							SpeedTime;							// スピード変化のカウント
	int							OjyamaTime;						// おジャマブロック出現時


} SKILLACT;

typedef enum
{
	EFFECT_UP = 0,
	EFFECT_DOWN,
	EFFECT_OJYAMA,
	EFFECT_KAMINARI,
	EFFECT_MAX
}SKILL_OBJECT;
//***************************************************
// プロトタイプ宣言
//***************************************************
void MakeVertexSkillact(LPDIRECT3DDEVICE9 pDevice);
int SetSkillAct(D3DXVECTOR3 pos, int effect, int player_no, D3DXCOLOR col)
#endif

