//*************************************************
// skillact.h
// �ŏI�X�V�@3��4��
// ����� �R�{�s��
//*************************************************
#ifndef _SKILLACT_H_
#define _SKILLACT_H_

//*************************************************
// �C���N���[�h�t�@�C��
//*************************************************
#include "main.h"

//*************************************************
// �}�N����`
//*************************************************
// �b��
#define FPS			(60)			// 1�b�̃t���[����
#define ACTTIME_LV1	(5*FPS)			// �X�L�����x��1���ʔ�������
#define ACTTIME_LV2	(7*FPS)			// �X�L�����x��2���ʔ�������

//*************************************************
// �\���̒�`
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

	int							SpeedTime;							// �X�s�[�h�ω��̃J�E���g
	int							OjyamaTime;						// ���W���}�u���b�N�o����


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
// �v���g�^�C�v�錾
//***************************************************
void MakeVertexSkillact(LPDIRECT3DDEVICE9 pDevice);
int SetSkillAct(D3DXVECTOR3 pos, int effect, int player_no, D3DXCOLOR col)
#endif

