//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "collision.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER			(4)							// プレイヤーの数

#define PLAYER_WIDTH		(40)
#define PLAYER_HEIGHT		(77)
#define HALF_PLAYER_WIDTH	(PLAYER_WIDTH / 2)
#define HALF_PLAYER_HEIGHT	(PLAYER_HEIGHT / 2)
#define HALF_PLAYER_DEPTH	(0.0f)

#define PLAYER_JUMP_SPEED	(9.f)						// プレイヤーのジャンプ初速
#define PLAYER_LARGE_JUMP_SPEED (12.f)					// プレイヤーの大ジャンプ初速
#define GRAVITY_ACCELARATION (-0.5f)					// 重力加速度

#define PLAYER_BB_MAX		(D3DXVECTOR3(HALF_PLAYER_WIDTH, HALF_PLAYER_HEIGHT, HALF_PLAYER_DEPTH))// プレイヤーバウンディングボックスのmax頂点座標
#define PLAYER_BB_MIN		(-PLAYER_BB_MAX)			// プレイヤーバウンディングボックスのmin頂点座標

#define SLIDING_HIT_BOX		{D3DXVECTOR3(-HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, -HALF_PLAYER_DEPTH), D3DXVECTOR3(HALF_PLAYER_WIDTH, 0.0f, HALF_PLAYER_DEPTH)}



//*****************************************************************************
// 列挙体定義
//*****************************************************************************
enum PLAYER_STATE {
	PLAYER_NONE,
	PLAYER_ONGROUND,	// 着地
	PLAYER_JUMP,		// ジャンプ中
	PLAYER_SLIDING,		// スライディング
	PLAYER_DEAD,		// 死亡
	PLAYER_STATEMAX,
};


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	int lane_no;				// 所属するレーン番号
	int skillpoint;				// スキル利用の為に追加
	bool kengen;				// スキルを発動する権限を持っているかどうか
	LPDIRECT3DTEXTURE9 texture; // テクスチャ読み込み場所
	LPDIRECT3DVERTEXBUFFER9 vtx;
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 現在の向き
	D3DXVECTOR3 rotDest;		// 目的の向き
	D3DXVECTOR3 scl;			// スケール
	float ground;				// 地面の高さ
	float distance;				// 進んだ距離
	float jump_speed;			// ジャンプ時の初速
	int life;					// プレイヤーの体力
	PLAYER_STATE state;			// プレイヤーの状態
	PLAYER_STATE next_state;	// プレイヤーの次の状態
	float speed_factor;			// 速度係数
	float state_counter;		// 状態のタイマー
	bool is_invincible;			// 無敵状態かどうか
	int invincible_counter;		// 無敵時間のカウント
	BOUNDING_BOX hit_box;		// プレイヤーの当たり判定
	BOUNDING_BOX screen_box;	// 画面外判定用ボックス
} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(int no);

D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);
D3DXVECTOR3 GetMovePlayer(void);
int NumPlayer(void);
void IncreaseSkillpoint(int player_no);

#endif
