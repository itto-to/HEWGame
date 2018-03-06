//****************************************************************
// ファイル名:	skillact.cpp
// 説明:		スキルの動作
// 最終更新日:	3月4日
//****************************************************************

//****************************************************************
// インクルードファイル
//****************************************************************
#include "camera.h"
#include "player.h"
#include "skill.h"
#include "skillact.h"
#include "input.h"
#include "mesh.h"

//*****************************************************************
// マクロ定義
//*****************************************************************

// エフェクト
<<<<<<< HEAD
#define TEXTURE_UPEFFECT	("data/TEXTURE/icon_speed_up.png")				// 速度上昇
#define TEXTURE_DOWNEFFECT	("data/TEXTURE/icon_speed_down.png")			// 減速
#define TEXTURE_OJYAMA		("data/TEXTURE/skill_lv2_block.png")			// おジャマブロック
#define TEXTURE_KAMINARI	("data/TEXTURE/skill_lv3_thunder.png")			// 雷
=======
#define UPEFFECT	("data/TEXTURE/icon_speed_up.png")				// 速度上昇
#define DOWNEFFECT	("data/TEXTURE/icon_speed_down.png")				// 減速
#define OJYAMA		("data/TEXTURE/skill_lv2_block.png")				// おジャマブロック
#define KAMINARI	("data/TEXTURE/skill_lv3_thunder.png")				// 雷の
>>>>>>> 4f3be628524b288d711aa7094ab03acd72035386


// サイズ
#define UPEFFECT_HEIGHT		(50)
#define UPEFFECT_WIDTH		(50)
#define DOWNEFFECT_HEIGHT	(50)
#define DOWNEFFECT_WIDTH	(50)
#define OJYAMA_HEIGHT		(90)
#define OJYAMA_WIDTH		(90)
#define KAMINARI_HEIGHT		(50)
#define KAMINARI_WIDTH		(50)

#define OJYAMA_OFFSET_WIDTH	(OJYAMA_WIDTH * 0.9f)
#define NUM_OJYAMA_BLOCK	(8)

// 座標情報
#define SPEED_EFFECT_OFFSET	(D3DXVECTOR3(10.0f, 10.0f, 0.0f))
#define OJYAMA_EFFECT_OFFSET (D3DXVECTOR3(-1040.0f, 28.0f, 0.0f))
#define KAMINARI_EFFECT_OFFSET (D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//#define

//*****************************************************************
// グローバル変数
//*****************************************************************
SKILLACT skillactWk[MAX_PLAYER];

//*****************************************************************
// プロトタイプ宣言
//*****************************************************************
HRESULT MakeVertexUp_effect(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexDown_effect(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexOjyama(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexKaminari(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int effect, int no);
void SetColorSkillAct(int nIdxSkillAct, int effect, int no, D3DXCOLOR col);



//*****************************************************************
// 関数名　HRESULT Init
//
//初期化
//*****************************************************************
HRESULT InitSkillAct(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexSkillact(pDevice);


	// フラグリセット
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		skillactWk[i].Up_active = false;
		skillactWk[i].Down_active = false;
		skillactWk[i].Ojyama_active = false;
		skillactWk[i].Kaminari_active = false;

	}
	// テクスチャ読み込み
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_UPEFFECT,							// ファイルの名前
			&skillactWk[i].Up_texture);

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_DOWNEFFECT,							// ファイルの名前
			&skillactWk[i].Down_texture);

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_OJYAMA,							// ファイルの名前
			&skillactWk[i].Ojyama_texture);

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_KAMINARI,							// ファイルの名前
			&skillactWk[i].Kaminari_texture);

	}

	return S_OK;

}

//*************************************************************
// 関数名:	void UninitSkillAct(void)
// 引数:	なし
// 戻り値:
// 説明:	スキル処理の終了処理
//*************************************************************
void UninitSkillAct(void)
{
	// それぞれのテクスチャとバッファをそれぞれ開放

	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(skillactWk[i].Up_texture != NULL)
		{// テクスチャ開放
			skillactWk[i].Up_texture->Release();
			skillactWk[i].Up_texture = NULL;
		}

		if(skillactWk[i].Up_buff != NULL)
		{ //頂点バッファの開放
			skillactWk[i].Up_buff->Release();
			skillactWk[i].Up_buff = NULL;
		}

		// down
		if(skillactWk[i].Down_texture != NULL)
		{// テクスチャ開放
			skillactWk[i].Down_texture->Release();
			skillactWk[i].Down_texture = NULL;
		}

		if(skillactWk[i].Down_buff != NULL)
		{ //頂点バッファの開放
			skillactWk[i].Down_buff->Release();
			skillactWk[i].Down_buff = NULL;
		}

		// ojyama
		if(skillactWk[i].Ojyama_texture != NULL)
		{// テクスチャ開放
			skillactWk[i].Ojyama_texture->Release();
			skillactWk[i].Ojyama_texture = NULL;
		}

		if(skillactWk[i].Ojyama_buff != NULL)
		{ //頂点バッファの開放
			skillactWk[i].Ojyama_buff->Release();
			skillactWk[i].Ojyama_buff = NULL;
		}

		// kaminari
		if(skillactWk[i].Kaminari_texture != NULL)
		{// テクスチャ開放
			skillactWk[i].Kaminari_texture->Release();
			skillactWk[i].Kaminari_texture = NULL;
		}

		if(skillactWk[i].Kaminari_buff != NULL)
		{ //頂点バッファの開放
			skillactWk[i].Kaminari_buff->Release();
			skillactWk[i].Kaminari_buff = NULL;
		}

	}

}



//****************************************************************
// 関数名:	void DrawSkillAct(void)
// 引数:	なし
// 戻り値:	なし
// 説明:	エフェクトの描画
//****************************************************************
void DrawSkillAct(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// UPエフェクト
	for (int no = 0; no < NumPlayer(); no++)
	{
		// 実行状態じゃない場合描画しない
		if(!skillactWk[no].Up_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];
		if (skillact->Up_active == true)
		{
			DrawMesh(skillact->Up_buff, skillact->Up_texture, player->pos + SPEED_EFFECT_OFFSET, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}
	
	// DOWNエフェクト
	for (int no = 0; no < NumPlayer(); no++)
	{
		// 実行状態じゃない場合描画しない
		if (!skillactWk[no].Down_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];
		if (skillact->Down_active == true)
		{
			DrawMesh(skillact->Down_buff, skillact->Down_texture, player->pos + SPEED_EFFECT_OFFSET, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}

<<<<<<< HEAD
	// おじゃまブロック
	for (int no = 0; no < NumPlayer(); no++)
	{
		// 実行状態じゃない場合描画しない
		if (!skillactWk[no].Ojyama_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];

		if (skillact->Ojyama_active == true)
		{
			D3DXVECTOR3 offset = OJYAMA_EFFECT_OFFSET;
			for (int i = 0; i < NUM_OJYAMA_BLOCK; i++) {
				DrawMesh(skillact->Ojyama_buff, skillact->Ojyama_texture, player->ground_pos + offset, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				offset.x += OJYAMA_OFFSET_WIDTH;
			}
		}
=======
	// ojyama
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		// 実行状態の場合描画
		if(skillactWk[no].Ojyama_active == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&skillactWk[no].Ojyama_mtxWorld);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ポリゴンを正面に向ける
#if 1
			// 逆行列をもとめる
			D3DXMatrixInverse(&skillactWk[no].Ojyama_mtxWorld, NULL, &mtxView);
			skillactWk[no].Ojyama_mtxWorld._41 = 0.0f;
			skillactWk[no].Ojyama_mtxWorld._42 = 0.0f;
			skillactWk[no].Ojyama_mtxWorld._43 = 0.0f;
#else
			skillactWk[no].Ojyama_mtxWorld._11 = mtxView._11;
			skillactWk[no].Ojyama_mtxWorld._12 = mtxView._21;
			skillactWk[no].Ojyama_mtxWorld._13 = mtxView._31;
			skillactWk[no].Ojyama_mtxWorld._21 = mtxView._12;
			skillactWk[no].Ojyama_mtxWorld._22 = mtxView._22;
			skillactWk[no].Ojyama_mtxWorld._23 = mtxView._32;
			skillactWk[no].Ojyama_mtxWorld._31 = mtxView._13;
			skillactWk[no].Ojyama_mtxWorld._32 = mtxView._23;
			skillactWk[no].Ojyama_mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, skillactWk[no].Ojyama_scl.x,
				skillactWk[no].Ojyama_scl.y,
				skillactWk[no].Ojyama_scl.z);
			D3DXMatrixMultiply(&skillactWk[no].Ojyama_mtxWorld, &skillactWk[no].Ojyama_mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Ojyama_pos.x,
				skillactWk[no].Ojyama_pos.y,
				skillactWk[no].Ojyama_pos.z);
			D3DXMatrixMultiply(&skillactWk[no].Ojyama_mtxWorld, &skillactWk[no].Ojyama_mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Ojyama_mtxWorld);


			// ライフゲージを描画
			{
				// 頂点バッファをデバイスのストリームにバインド
				pDevice->SetStreamSource(0, skillactWk[no].Ojyama_buff, 0, sizeof(VERTEX_2D));
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);
				// テクスチャの設定
				pDevice->SetTexture(0, skillactWk[no].Ojyama_texture);
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
			}
		}
	}

	// Kaminari
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		// 実行状態の場合描画
		if(skillactWk[no].Kaminari_active == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&skillactWk[no].Kaminari_mtxWorld);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ポリゴンを正面に向ける
#if 1
			// 逆行列をもとめる
			D3DXMatrixInverse(&skillactWk[no].Kaminari_mtxWorld, NULL, &mtxView);
			skillactWk[no].Kaminari_mtxWorld._41 = 0.0f;
			skillactWk[no].Kaminari_mtxWorld._42 = 0.0f;
			skillactWk[no].Kaminari_mtxWorld._43 = 0.0f;
#else
			skillactWk[no].Kaminari_mtxWorld._11 = mtxView._11;
			skillactWk[no].Kaminari_mtxWorld._12 = mtxView._21;
			skillactWk[no].Kaminari_mtxWorld._13 = mtxView._31;
			skillactWk[no].Kaminari_mtxWorld._21 = mtxView._12;
			skillactWk[no].Kaminari_mtxWorld._22 = mtxView._22;
			skillactWk[no].Kaminari_mtxWorld._23 = mtxView._32;
			skillactWk[no].Kaminari_mtxWorld._31 = mtxView._13;
			skillactWk[no].Kaminari_mtxWorld._32 = mtxView._23;
			skillactWk[no].Kaminari_mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, skillactWk[no].Kaminari_scl.x,
				skillactWk[no].Kaminari_scl.y,
				skillactWk[no].Kaminari_scl.z);
			D3DXMatrixMultiply(&skillactWk[no].Kaminari_mtxWorld, &skillactWk[no].Kaminari_mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Kaminari_pos.x,
				skillactWk[no].Kaminari_pos.y,
				skillactWk[no].Kaminari_pos.z);
			D3DXMatrixMultiply(&skillactWk[no].Kaminari_mtxWorld, &skillactWk[no].Kaminari_mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Kaminari_mtxWorld);


			// ライフゲージを描画
			{
				// 頂点バッファをデバイスのストリームにバインド
				pDevice->SetStreamSource(0, skillactWk[no].Kaminari_buff, 0, sizeof(VERTEX_2D));
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);
				// テクスチャの設定
				pDevice->SetTexture(0, skillactWk[no].Kaminari_texture);
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
			}
		}
>>>>>>> 4f3be628524b288d711aa7094ab03acd72035386
	}

	// カミナリ
	for (int no = 0; no < NumPlayer(); no++)
	{
		// 実行状態じゃない場合描画しない
		if (!skillactWk[no].Kaminari_active)
			continue;

		PLAYER *player = GetPlayer(no);
		SKILLACT *skillact = &skillactWk[no];
		if (skillact->Kaminari_active == true)
		{
			DrawMesh(skillact->Kaminari_buff, skillact->Kaminari_texture, player->pos + KAMINARI_EFFECT_OFFSET, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}
}
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
//
//	// ラインティングを無効にする
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//
//	// Upeffect
//	for(int no = 0; no < MAX_PLAYER; no++)
//	{
//		// 実行状態の場合描画
//		if(skillactWk[no].Up_active == true)
//		{
//			// ワールドマトリックスの初期化
//			D3DXMatrixIdentity(&skillactWk[no].Up_mtxWorld);
//
//			// ビューマトリックスを取得
//			mtxView = GetMtxView();
//
//			// ポリゴンを正面に向ける
//#if 1
//			// 逆行列をもとめる
//			D3DXMatrixInverse(&skillactWk[no].Up_mtxWorld, NULL, &mtxView);
//			skillactWk[no].Up_mtxWorld._41 = 0.0f;
//			skillactWk[no].Up_mtxWorld._42 = 0.0f;
//			skillactWk[no].Up_mtxWorld._43 = 0.0f;
//#else
//			skillactWk[no].Up_mtxWorld._11 = mtxView._11;
//			skillactWk[no].Up_mtxWorld._12 = mtxView._21;
//			skillactWk[no].Up_mtxWorld._13 = mtxView._31;
//			skillactWk[no].Up_mtxWorld._21 = mtxView._12;
//			skillactWk[no].Up_mtxWorld._22 = mtxView._22;
//			skillactWk[no].Up_mtxWorld._23 = mtxView._32;
//			skillactWk[no].Up_mtxWorld._31 = mtxView._13;
//			skillactWk[no].Up_mtxWorld._32 = mtxView._23;
//			skillactWk[no].Up_mtxWorld._33 = mtxView._33;
//#endif
//
//			// スケールを反映
//			D3DXMatrixScaling(&mtxScale, skillactWk[no].Up_scl.x,
//				skillactWk[no].Up_scl.y,
//				skillactWk[no].Up_scl.z);
//			D3DXMatrixMultiply(&skillactWk[no].Up_mtxWorld, &skillactWk[no].Up_mtxWorld, &mtxScale);
//
//			// 移動を反映
//			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Up_pos.x,
//				skillactWk[no].Up_pos.y,
//				skillactWk[no].Up_pos.z);
//			D3DXMatrixMultiply(&skillactWk[no].Up_mtxWorld, &skillactWk[no].Up_mtxWorld, &mtxTranslate);
//
//			// ワールドマトリックスの設定
//			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Up_mtxWorld);
//
//
//			// ライフゲージを描画
//			{
//				// 頂点バッファをデバイスのストリームにバインド
//				pDevice->SetStreamSource(0, skillactWk[no].Up_buff, 0, sizeof(VERTEX_2D));
//				// 頂点フォーマットの設定
//				pDevice->SetFVF(FVF_VERTEX_2D);
//				// テクスチャの設定
//				pDevice->SetTexture(0, skillactWk[no].Up_texture);
//				// ポリゴンの描画
//				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//			}
//		}
//	}
//
//	// Downeffect
//	for(int no = 0; no < MAX_PLAYER; no++)
//	{
//		// 実行状態の場合描画
//		if(skillactWk[no].Down_active == true)
//		{
//			// ワールドマトリックスの初期化
//			D3DXMatrixIdentity(&skillactWk[no].Down_mtxWorld);
//
//			// ビューマトリックスを取得
//			mtxView = GetMtxView();
//
//			// ポリゴンを正面に向ける
//#if 1
//			// 逆行列をもとめる
//			D3DXMatrixInverse(&skillactWk[no].Down_mtxWorld, NULL, &mtxView);
//			skillactWk[no].Down_mtxWorld._41 = 0.0f;
//			skillactWk[no].Down_mtxWorld._42 = 0.0f;
//			skillactWk[no].Down_mtxWorld._43 = 0.0f;
//#else
//			skillactWk[no].Down_mtxWorld._11 = mtxView._11;
//			skillactWk[no].Down_mtxWorld._12 = mtxView._21;
//			skillactWk[no].Down_mtxWorld._13 = mtxView._31;
//			skillactWk[no].Down_mtxWorld._21 = mtxView._12;
//			skillactWk[no].Down_mtxWorld._22 = mtxView._22;
//			skillactWk[no].Down_mtxWorld._23 = mtxView._32;
//			skillactWk[no].Down_mtxWorld._31 = mtxView._13;
//			skillactWk[no].Down_mtxWorld._32 = mtxView._23;
//			skillactWk[no].Down_mtxWorld._33 = mtxView._33;
//#endif
//
//			// スケールを反映
//			D3DXMatrixScaling(&mtxScale, skillactWk[no].Down_scl.x,
//				skillactWk[no].Down_scl.y,
//				skillactWk[no].Down_scl.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxScale);
//
//			// 移動を反映
//			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Down_pos.x,
//				skillactWk[no].Down_pos.y,
//				skillactWk[no].Down_pos.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxTranslate);
//
//			// ワールドマトリックスの設定
//			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Down_mtxWorld);
//
//
//			// ライフゲージを描画
//			{
//				// 頂点バッファをデバイスのストリームにバインド
//				pDevice->SetStreamSource(0, skillactWk[no].Down_buff, 0, sizeof(VERTEX_2D));
//				// 頂点フォーマットの設定
//				pDevice->SetFVF(FVF_VERTEX_2D);
//				// テクスチャの設定
//				pDevice->SetTexture(0, skillactWk[no].Down_texture);
//				// ポリゴンの描画
//				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//			}
//		}
//	}
//
//	// ojyama
//	for(int no = 0; no < MAX_PLAYER; no++)
//	{
//		// 実行状態の場合描画
//		if(skillactWk[no].Down_active == true)
//		{
//			// ワールドマトリックスの初期化
//			D3DXMatrixIdentity(&skillactWk[no].Down_mtxWorld);
//
//			// ビューマトリックスを取得
//			mtxView = GetMtxView();
//
//			// ポリゴンを正面に向ける
//#if 1
//			// 逆行列をもとめる
//			D3DXMatrixInverse(&skillactWk[no].Down_mtxWorld, NULL, &mtxView);
//			skillactWk[no].Down_mtxWorld._41 = 0.0f;
//			skillactWk[no].Down_mtxWorld._42 = 0.0f;
//			skillactWk[no].Down_mtxWorld._43 = 0.0f;
//#else
//			skillactWk[no].Down_mtxWorld._11 = mtxView._11;
//			skillactWk[no].Down_mtxWorld._12 = mtxView._21;
//			skillactWk[no].Down_mtxWorld._13 = mtxView._31;
//			skillactWk[no].Down_mtxWorld._21 = mtxView._12;
//			skillactWk[no].Down_mtxWorld._22 = mtxView._22;
//			skillactWk[no].Down_mtxWorld._23 = mtxView._32;
//			skillactWk[no].Down_mtxWorld._31 = mtxView._13;
//			skillactWk[no].Down_mtxWorld._32 = mtxView._23;
//			skillactWk[no].Down_mtxWorld._33 = mtxView._33;
//#endif
//
//			// スケールを反映
//			D3DXMatrixScaling(&mtxScale, skillactWk[no].Down_scl.x,
//				skillactWk[no].Down_scl.y,
//				skillactWk[no].Down_scl.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxScale);
//
//			// 移動を反映
//			D3DXMatrixTranslation(&mtxTranslate, skillactWk[no].Down_pos.x,
//				skillactWk[no].Down_pos.y,
//				skillactWk[no].Down_pos.z);
//			D3DXMatrixMultiply(&skillactWk[no].Down_mtxWorld, &skillactWk[no].Down_mtxWorld, &mtxTranslate);
//
//			// ワールドマトリックスの設定
//			pDevice->SetTransform(D3DTS_WORLD, &skillactWk[no].Down_mtxWorld);
//
//
//			// ライフゲージを描画
//			{
//				// 頂点バッファをデバイスのストリームにバインド
//				pDevice->SetStreamSource(0, skillactWk[no].Down_buff, 0, sizeof(VERTEX_2D));
//				// 頂点フォーマットの設定
//				pDevice->SetFVF(FVF_VERTEX_2D);
//				// テクスチャの設定
//				pDevice->SetTexture(0, skillactWk[no].Down_texture);
//				// ポリゴンの描画
//				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//			}
//		}
//	}
//
//}
//

//*****************************************************************
// 関数名:	void MakeVertexSkillact(LPDIRECT3DDEVICE pDevice)
// 引数:	LPDIRECT3DDEVICE9 pDevice
// 戻り値:	なし
// 説明:	MakeVertex関数を実行する関数
//*****************************************************************
void MakeVertexSkillact(LPDIRECT3DDEVICE9 pDevice)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++) {
		PLAYER *player = GetPlayer(player_no);
		// UPエフェクト
		MakeVertex(pDevice, &skillactWk[player_no].Up_buff, NULL, UPEFFECT_WIDTH, UPEFFECT_HEIGHT);

		// DOWNエフェクト
		MakeVertex(pDevice, &skillactWk[player_no].Down_buff, NULL, DOWNEFFECT_WIDTH, DOWNEFFECT_HEIGHT);
		
		// おじゃまブロック
		MakeVertex(pDevice, &skillactWk[player_no].Ojyama_buff, NULL, OJYAMA_WIDTH, OJYAMA_HEIGHT);

		// カミナリ
		MakeVertex(pDevice, &skillactWk[player_no].Kaminari_buff, NULL, KAMINARI_WIDTH, KAMINARI_HEIGHT);
	}
}
//*****************************************************************
// 関数名:	HRESULT MakeVertexUp_effect(LPDIRECT3DDEVICE9 pDevice)
// 引数:	LPDIRECT3DDEVICE9 pDevice
// 戻り値:
// 説明		頂点の作成
//******************************************************************
HRESULT MakeVertexUp_effect(LPDIRECT3DDEVICE9 pDevice)
{
	// Up
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// バッファ作成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用、確保するバッファのサイズ
			D3DUSAGE_WRITEONLY,		// 使用法
			FVF_VERTEX_2D,				// 頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラス
			&skillactWk[i].Up_buff,			// 頂点バッファインタフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		// 頂点バッファの中身を埋める
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロック＆ポインタを取得
			skillactWk[i].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2,  UPEFFECT_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3( UPEFFECT_WIDTH / 2,  UPEFFECT_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2, -UPEFFECT_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3( UPEFFECT_WIDTH / 2, -UPEFFECT_HEIGHT / 2, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Up_buff->Unlock();
		}
	}

	

	return S_OK;
}

//*****************************************************************
// 関数名:	HRESULT MakeVertexDown_effect(LPDIRECT3DDEVICE9 pDevice)
// 引数:	LPDIRECT3DDEVICE9 pDevice
// 戻り値:
// 説明		頂点の作成
//******************************************************************
HRESULT MakeVertexDown_effect(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// バッファ作成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用、確保するバッファのサイズ
			D3DUSAGE_WRITEONLY,		// 使用法
			FVF_VERTEX_2D,				// 頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラス
			&skillactWk[i].Down_buff,			// 頂点バッファインタフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		// 頂点バッファの中身を埋める
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロック＆ポインタを取得
			skillactWk[i].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2,  DOWNEFFECT_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3( DOWNEFFECT_WIDTH / 2,  DOWNEFFECT_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2, -DOWNEFFECT_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3( DOWNEFFECT_WIDTH / 2, -DOWNEFFECT_HEIGHT / 2, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Down_buff->Unlock();
		}
	}
	return S_OK;
}

//*****************************************************************
// 関数名:	HRESULT MakeVertexOjyama(LPDIRECT3DDEVICE9 pDevice)
// 引数:	LPDIRECT3DDEVICE9 pDevice
// 戻り値:
// 説明		頂点の作成
//******************************************************************
HRESULT MakeVertexOjyama(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// バッファ作成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用、確保するバッファのサイズ
			D3DUSAGE_WRITEONLY,		// 使用法
			FVF_VERTEX_2D,				// 頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラス
			&skillactWk[i].Ojyama_buff,			// 頂点バッファインタフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		// 頂点バッファの中身を埋める
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロック＆ポインタを取得
			skillactWk[i].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-OJYAMA_WIDTH / 2, OJYAMA_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2, OJYAMA_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-OJYAMA_WIDTH / 2, -OJYAMA_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2, -OJYAMA_HEIGHT / 2, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Ojyama_buff->Unlock();
		}
	}
	return S_OK;
}

//*****************************************************************
// 関数名:	HRESULT MakeVertexKaminari(LPDIRECT3DDEVICE9 pDevice)
// 引数:	LPDIRECT3DDEVICE9 pDevice
// 戻り値:
// 説明		頂点の作成
//******************************************************************
HRESULT MakeVertexKaminari(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// バッファ作成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用、確保するバッファのサイズ
			D3DUSAGE_WRITEONLY,		// 使用法
			FVF_VERTEX_2D,				// 頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラス
			&skillactWk[i].Kaminari_buff,			// 頂点バッファインタフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		// 頂点バッファの中身を埋める
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロック＆ポインタを取得
			skillactWk[i].Kaminari_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-KAMINARI_WIDTH / 2, KAMINARI_HEIGHT / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(KAMINARI_WIDTH / 2, KAMINARI_HEIGHT / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-KAMINARI_WIDTH / 2, -KAMINARI_HEIGHT / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(KAMINARI_WIDTH / 2, -KAMINARI_HEIGHT / 2, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Kaminari_buff->Unlock();
		}
	}
	return S_OK;
}

//****************************************************************************
// 関数名:	void SetColorSkillAct(void)
// 引数:	なし
// 戻り値:	なし
// 説明:	ディフューズ色設定
//*****************************************************************************
void SetColorSkillAct(int player_no, int effect)
{

	// 頂点バッファの中身を埋める
	// Up
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[i].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Up_buff->Unlock();
		}
	}

	// Down
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[i].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Down_buff->Unlock();
		}
	}

	// Ojyama
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[i].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Ojyama_buff->Unlock();
		}
	}


	// Kaminari
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		{
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[i].Kaminari_buff->Lock(0, 0, (void**)&pVtx, 0);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点データをアンロック
			skillactWk[i].Kaminari_buff->Unlock();
		}
	}
}



//************************************************************************************
// 関数名:	void SetVertexEffect(int nIdxEffect,int effect, int no)
// 引数:	int effect int no
// 戻り値:	なし
// 説明:	effectの値によって頂点座標を設定 noはプレイヤーの番号
//************************************************************************************
void SetVertexEffect(int effect,int no)
{
	{// 頂点バッファの中身を埋める

		VERTEX_3D *pVtx;

		// 現在のレベルによって処理を分岐
		switch(effect)
		{
		case EFFECT_UP:
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[no].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-UPEFFECT_WIDTH / 2.0f, UPEFFECT_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(UPEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(UPEFFECT_WIDTH / 2.0f, UPEFFECT_HEIGHT, 0.0f);

			// 頂点データをアンロックする
			skillactWk[no].Up_buff->Unlock();
			break;

		case EFFECT_DOWN:
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[no].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-DOWNEFFECT_WIDTH / 2.0f, DOWNEFFECT_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(DOWNEFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(DOWNEFFECT_WIDTH / 2.0f, DOWNEFFECT_HEIGHT, 0.0f);

			// 頂点データをアンロックする
			skillactWk[no].Down_buff->Unlock();
			break;

		case EFFECT_OJYAMA:
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[no].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);

			// 頂点データをアンロックする
			skillactWk[no].Ojyama_buff->Unlock();
			break;

		case EFFECT_KAMINARI:
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[no].Kaminari_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (no * 4);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(OJYAMA_WIDTH / 2.0f, OJYAMA_HEIGHT, 0.0f);

			// 頂点データをアンロックする
			skillactWk[no].Kaminari_buff->Unlock();
			break;


		
		}
	}
}


//*************************************************************************************
// 関数名:	void SetColorSkillact(int nIdxSkillAct,D3DXCOLOR col)
// 引数:	int nIdxSkillAct, int effect,int no, D3DXCOLOR col
// 戻り値:	なし
// 説明		色の設定
// nIdxSkillActとnoが同じ数値になるから多分どっちか1つでいい？
//*************************************************************************************

void SetColorSkillAct(int nIdxSkillAct, int effect,int no, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		switch(effect)
		{
		case EFFECT_UP:
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[no].Up_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdxSkillAct * 4);

			// 頂点座標の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// 頂点データをアンロックする
			skillactWk[no].Up_buff->Unlock();
			break;

		case EFFECT_DOWN:
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[no].Down_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdxSkillAct * 4);

			// 頂点座標の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// 頂点データをアンロックする
			skillactWk[no].Down_buff->Unlock();
			break;

		case EFFECT_OJYAMA:
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			skillactWk[no].Ojyama_buff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nIdxSkillAct * 4);

			// 頂点座標の設定
			pVtx[0].diffuse = col;
			pVtx[1].diffuse = col;
			pVtx[2].diffuse = col;
			pVtx[3].diffuse = col;

			// 頂点データをアンロックする
			skillactWk[no].Ojyama_buff->Unlock();
			break;
		}
	}
}

//*************************************************************************************************
// 関数名:	int SetSkillAct(D3DXVECTOR3 pos,int no,int effect,int player_no
// 引数:
// 戻り値:	int  nIdxSkillAct;
// 説明:	スキルの発動及びテクスチャのセット
//**************************************************************************************************
int SetSkillAct(D3DXVECTOR3 pos,int effect,int player_no, D3DXCOLOR col)
{
	SKILL *skill = GetSkillWk(0);
	int nIdxSkillact = -1;

	switch(effect)
	{
	case EFFECT_UP:
		// noは発動したプレイヤーの番号
		skillactWk[player_no].Up_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Up_pos = pos;
		skillactWk[player_no].Up_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Up_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Up_active = true;
		skillactWk[player_no].SpeedTime = ACTTIME_LV1;					// 実行時間設定

		// 頂点カラーの設定
		SetColorSkillAct(player_no, effect, player_no, col);
		break;

	case EFFECT_DOWN:
		// noは発動したプレイヤーの番号
		skillactWk[player_no].Down_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Down_pos = pos;
		skillactWk[player_no].Down_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Down_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Down_active = true;
		skillactWk[player_no].SpeedTime = ACTTIME_LV1;				// 実行時間設定

		// 頂点カラーの設定
		SetColorSkillAct(player_no, effect, player_no, col);
		break;

	case EFFECT_OJYAMA:
		// 
		for(int i = 0; i < OJYAMASET_MAX; i++)
		{
		skillactWk[player_no].Ojyama_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Ojyama_pos = D3DXVECTOR3((i*OJYAMA_WIDE),pos.y,pos.z);
		skillactWk[player_no].Ojyama_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Ojyama_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Ojyama_active = true;
		skillactWk[player_no].OjyamaTime = ACTTIME_LV2;

		// 頂点カラーの設定
		SetColorSkillAct(player_no, effect, player_no, col);
		}
		break;


	case EFFECT_KAMINARI:
		// noは発動したプレイヤーの番号
		skillactWk[player_no].Kaminari_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Kaminari_pos = pos;
		skillactWk[player_no].Kaminari_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		skillactWk[player_no].Kaminari_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skillactWk[player_no].Kaminari_active = true;


		// 頂点カラーの設定
		SetColorSkillAct(player_no, effect, player_no, col);
		break;
	}




	nIdxSkillact = player_no;

	return nIdxSkillact;
}



//**************************************************************************
// 関数名:	SKILLACT *GetSkillAct(int no)
// 引数:	int no
// 戻り値:	SKILLACT skillactWk
// 説明:	Get関数
//**************************************************************************
SKILLACT *GetSkillAct(int no)
{
	return &skillactWk[0];
}



