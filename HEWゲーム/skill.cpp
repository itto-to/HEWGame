/***************************************************************
* ファイル名:	skill.cpp
* 制作者:		HAL東京ゲーム学科 山本 壮将
* 制作日:		2月23日
* 説明:			スキル関連のプログラム
****************************************************************/

//***************************************************************
// インクルードファイル
//***************************************************************
#include "skill.h"

//***************************************************************
// マクロ定義
//***************************************************************
#define SKILL_WAKU		"data/TEXTURE/hpback.png"		// スキルゲージ
#define SKILL_BAR		"data/TEXTURE/hpgreen.png"		// スキルゲージのバー

// ゲージの枠部分
// 位置
#define SKILLGAGE_POS_X	(10.0f)
#define SKILLGAGE_POS_Y	(10.0f)
// 長さ
#define SKILLGAGE_WIDTH	(200.0f)
#define SKILLGAGE_HEIGHT	(100.0f)

// ゲージのバーの部分
// 位置
#define SKILLBAR_POS_X	(SKILLGAGE_POS_X + 10.0f)	// 良い感じに調整
#define SKILLBAR_POS_Y	(SKILLGAGE_POS_Y + 0.0f )
// 長さ
#define SKILLBAR_WIDTH	(180.0f)
#define SKILLBAR_HEIGHT	(90.0f)

// その他ゲージ関連
#define SKILL_LEVELUP	(5)			// レベルアップに必要な値
//***************************************************************
// プロトタイプ宣言
//***************************************************************

//***************************************************************
// グローバル変数
//***************************************************************
SKILL skillWk;			// スキル構造体

						//***************************************************************
						// 関数名:		HRESULT InitSkill(void)
						// 引数:		なし
						// 戻り値:		なし
						// 説明:		スキル関連の初期 化
						//***************************************************************
HRESULT InitSkill(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	//MakeVertexModellife(pDevice);

	// ゲージ
	D3DXCreateTextureFromFile(pDevice,
		SKILL_WAKU,							// ファイルの名前
		&skillWk.Texture_bar);

	// ゲージの枠
	D3DXCreateTextureFromFile(pDevice,
		SKILL_BAR,
		&skillWk.Texture_waku);

	// フラグ初期化
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		skillWk.flag[no].flag_no = 0;
		skillWk.flag[no].use_count = 0;
		skillWk.flag[no].get = false;
		skillWk.flag[no].count = 0;
	}

	return S_OK;

}

//*************************************************************
// 関数名:	void UninitSkill(void)
// 引数:	なし
// 戻り値:
// 説明:	スキル処理の終了処理
//*************************************************************
void UninitSkill(void)
{
	if(skillWk.Texture_bar != NULL)
	{// テクスチャ開放
		skillWk.Texture_bar->Release();
		skillWk.Texture_bar = NULL;
	}

	if(skillWk.Buff_bar != NULL)
	{ //頂点バッファの開放
		skillWk.Buff_bar->Release();
		skillWk.Buff_bar = NULL;
	}

	if(skillWk.Texture_waku != NULL)
	{
		skillWk.Texture_waku->Release();
		skillWk.Texture_waku = NULL;
	}

	// テクスチャ開放
	if(skillWk.Buff_waku != NULL)
	{
		skillWk.Buff_waku->Release();
		skillWk.Buff_waku = NULL;

	}
}

//*************************************************************
// 関数名:	void UpdateSkill(float gageup)
// 引数:	float gageup
// 戻り値:	なし
// 説明:	スキルゲージの更新
//			gageupの値だけスキルゲージの値が上昇、一定値以上でレベルアップ
//****************************************************************
void UpdateSkill(float gageup)
{
	PLAYER *player = GetPlayer(0);

	// スキルゲージ上昇
	skillWk.gage += gageup;

	// もしゲージが一定以上貯まっていたなら
	if(skillWk.gage >= 5)
	{
		skillWk.lv++;				// レベルを上げて
		skillWk.gage = 0.0f;		// 値を初期化
	}

	// skillpointが5になっているプレイヤー全員に権限を付与
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(player[i].skillpoint >= 5)
		{
			player[i].kengen = true;
		}
	}
	// 優先順位決定
	GetSkill(0);

}

//****************************************************************
// 関数名:	void DrawSkill(void)
// 引数:	なし
// 戻り値:	なし
// 説明:	スキルゲージの描画
//****************************************************************
void DrawSkill(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float skill_hiritsu;		// バーの比率
	float Draw_Skillbar;		// 描画されるスキルゲージの長さ

	skill_hiritsu = (skillWk.gage / skillWk.gage_lvup);
	// 表示するバーの長さ
	Draw_Skillbar = (skill_hiritsu * SKILLBAR_WIDTH);

	// 枠を描画
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, skillWk.Buff_waku, 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定
		pDevice->SetTexture(0, skillWk.Texture_waku);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

	// ライフゲージを描画
	{
		// 頂点バッファをデバイスのストリームにバインド
		pDevice->SetStreamSource(0, skillWk.Buff_bar, 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定
		pDevice->SetTexture(0, skillWk.Texture_bar);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//*****************************************************************
// 関数名:	HRESULT MakeVertexSkill(LPDIRECT3DDEVICE9 pDevice)
// 引数:	LPDIRECT3DDEVICE9 pDevice
// 戻り値:
// 説明		頂点の作成(枠部分)
//******************************************************************
HRESULT MakeVertexSkill(LPDIRECT3DDEVICE9 pDevice)
{
	// ゲージの枠部分のバッファ作成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用、確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,		// 使用法
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラス
		&skillWk.Buff_waku,			// 頂点バッファインタフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	// 頂点バッファの中身を埋める
	{
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロック＆ポインタを取得
		skillWk.Buff_waku->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(SKILLGAGE_POS_X, SKILLGAGE_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SKILLGAGE_POS_X + SKILLGAGE_WIDTH, SKILLGAGE_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SKILLGAGE_POS_X, SKILLGAGE_POS_Y + SKILLGAGE_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SKILLGAGE_POS_X + SKILLGAGE_WIDTH, SKILLGAGE_POS_Y + SKILLGAGE_HEIGHT, 0.0f);

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
		skillWk.Buff_waku->Unlock();
	}

	return S_OK;
}

//****************************************************************************
// 関数名:	void SetColorSkill(void)
// 引数:	なし
// 戻り値:	なし
// 説明:	ディフューズ色設定
//*****************************************************************************
void SetColorSkill(void)
{
	// 頂点バッファの中身を埋める
	{
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		skillWk.Buff_waku->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点データをアンロック
		skillWk.Buff_waku->Unlock();
	}
}


//*****************************************************************************
// 関数名:	void GetSkill(no)
// 引数:	5回飛んだプレイヤーの番号
// 戻り値:
// 説明:	条件を満たしているプレイヤーのうち、誰に権限を与えるか決定
//*****************************************************************************
void GetSkill(int no)
{
	// プレイヤーのポインター初期化！
	PLAYER *player = GetPlayer(0);
	int old_getskill;				// 直前に権限を持っているプレイヤー
	int skill_gets;					// スキルを入手した人の番号

									// 5回飛んだプレイヤーが全員権限を持っている

									// 権限を持っているプレイヤーの番号を取得し
									// 権限を無効化
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		if(player[no].kengen == true)
		{
			skillWk.flag[no].get = true;
			player[no].kengen == false;
		}
		else
		{
			skillWk.flag[no].get = false;
		}
	}




	// ライフが少ない人

	// 権限を持った人が一番少ない順番

	// 権限割り当て


}

//******************************************************************************
// スキルポイントリセット
// 引数:	int no
// 説明:	ジャンプに失敗した人の番号を受け取って、その人のポイントを0にしちゃう
//******************************************************************************
void SkillReset(int no)
{
	PLAYER *player = GetPlayer(0);
	player[no].skillpoint = 0;
}


//*******************************************************************
// メモ書き！
//*******************************************************************

// 無敵時間の間はカウントをプラスされない！！！

