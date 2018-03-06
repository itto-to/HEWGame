/***************************************************************
* ファイル名:	skill.cpp
* 制作者:		HAL東京ゲーム学科 山本 壮将
* 最終更新日:	3月4日
* 説明:			スキル関連のプログラム
****************************************************************/

//***************************************************************
// インクルードファイル
//***************************************************************
#include "skill.h"
#include <time.h>
#include "stage.h"
#include "skillact.h"
#include "player.h"
#include "sound.h"
#include "debugproc.h"
//***************************************************************
// マクロ定義
//***************************************************************
#define SKILL_WAKU		"data/TEXTURE/skill_frame.png"		// スキルゲージ
#define SKILL_BAR		"data/TEXTURE/lifegreen.png"		// スキルゲージのバー
//#define SKILL_BAR2


// ゲージの枠部分
// 位置
#define SKILLGAGE_POS_X	(10.0f)
#define SKILLGAGE_POS_Y	(10.0f)
// 長さ
#define SKILLGAGE_WIDTH	(469.0f)
#define SKILLGAGE_HEIGHT	(98.0f)

// ゲージのバーの部分
// 位置
#define SKILLBAR_POS_X	(SKILLGAGE_POS_X + 10.0f)	// 良い感じに調整
#define SKILLBAR_POS_Y	(SKILLGAGE_POS_Y + 0.0f )
// 長さ
#define SKILLBAR_WIDTH	(180.0f)
#define SKILLBAR_HEIGHT	(40.0f)

// その他ゲージ関連
#define SKILL_LEVELUP	(5)			// レベルアップに必要な値

//
// スキルの加速及び減速
#define LANESPEED_UP	(0.05f)
#define LANESPEED_DOWN	(0.1f)

// それぞれの場所
#define UPDOWNPOS_X			(10.0f)
#define UPDOWNPOS_Y			(10.0f)
#define OJYAMA_POS_X		(300.0f)
#define THANDER_POS_Y		(15.0f)

//***************************************************************
// プロトタイプ宣言
//***************************************************************
int skillsort_life(void);
int skill_count_winner(void);
//***************************************************************
// グローバル変数
//***************************************************************
SKILL skillWk;							// スキル構造体
SKILL_FLAG skill_flag[MAX_PLAYER];
bool skillcheck_ok;						// スキル発動の権利を持っているプレイヤーが1人かどうか
bool firstflag;							// 最初に権限を持ったプレイヤーが発生したかどうか

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
	MakeVertexSkill(pDevice);

	// ゲージ
	D3DXCreateTextureFromFile(pDevice,
		SKILL_BAR,							// ファイルの名前
		&skillWk.Texture_bar);

	// ゲージの枠
	D3DXCreateTextureFromFile(pDevice,
		SKILL_WAKU,
		&skillWk.Texture_waku);

	// フラグ初期化
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		skill_flag[no].flag_no = 0;
		skill_flag[no].use_count = 0;
		skill_flag[no].get = false;
		skill_flag[no].count = 0;
	}

	// まだ権限は割り振られていない
	skillWk.kengen = false;
	firstflag = false;
	// まだスキルは実行状態ではない
	skillWk.moving = false;

	skillWk.gage = 0.0f;
	skillWk.gage_lvup = 5;

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
	bool first_skill_flag;
	bool gage_max = false;
	int skillget_count = 0;
	PLAYER *player = GetPlayer(0);
	skillcheck_ok = true;					// スキルの発動権利を持っているプレイヤーは1人かどうか


	// 引数の値が0より高ければ実行(常に実行でも問題なさそうなら消します)
	if(gageup > 0)
	{

		// スキルゲージ上昇
		PlaySound(SOUND_LABEL_SKILL_GAGECHARGE);
		skillWk.gage += gageup;

		// もしゲージが一定以上貯まっていたなら
		if(skillWk.gage >= 5 && skillWk.lv <= 3)
		{
			PlaySound(SOUND_LABEL_SKILL_GAGEON);
			skillWk.lv++;				// レベルを上げて
			skillWk.gage = 0.0f;		// 値を初期化
		}

		// もし権限がまだ誰にも割り当てられていない場合
		// 権限を決める

		if(skillWk.kengen == false)
		{
			// skillpointが5になっているプレイヤーの人数を確認
			for(int i = 0; i < MAX_PLAYER; i++)
			{
				if(player[i].skillpoint >= 5)
				{
					player[i].kengen = true;
					skillget_count++;
				}
				else
				{
					player[i].kengen = false;
				}

			}
			// 2人以上が権限を持っている場合、条件判定へ移動
			if(skillget_count >= 2)
			{
				skillcheck_ok = false;			// 権利の所有者は1人だけではない
				GetSkill();
			}
			else
			{
				skillWk.kengen = true;
			}
		}
		
	}
	

	// ダッシュゲージの量を測定
		for(int i = 0; i < MAX_PLAYER;i++)
		{	// 一度もスキルが割り振られていなければ実行
			if(firstflag == false)
			{
				gage_max = IsDashGaugeFull(i);
				if(gage_max == true)
				{
					player[i].kengen = true;
					firstflag = true;
				}
			}
		}
	
}

//****************************************************************
// 関数名:	void UpdateSkillAct(void)
// 引数:
// 戻り値:
// 説明:	スキルの効果関連の更新です
//****************************************************************
void UpdateSkillAct(void)
{
	PLAYER *player = GetPlayer(0);
	LANE *lane = GetLane(0);
	SKILLACT *skillactWk = GetSkillAct(0);

	for(int i = 0; i < MAX_PLAYER; i++)
	{
		// 実行状態なら処理を行う
		if(skillactWk[i].Up_active == true)
		{
			// 実行時間を減らし0以下なら終了
			skillactWk[i].SpeedTime--;
			if(skillactWk[i].SpeedTime <= 0)
			{//	加速状態なので減速して元の速度に戻す
				lane[i].speed_factor -= LANESPEED_UP;
				skillactWk[i].Up_active = false;
				// 実行終了！フラグを元に戻す
				skillWk.moving = false;
			}
		}
		if(skillactWk[i].Down_active == true)
		{	// 実行時間を減らして0以下なら終了
			skillactWk[i].SpeedTime--;
			if(skillactWk[i].SpeedTime <= 0)
			{// 減速状態なので加速して元の速度に戻す
				lane[i].speed_factor += LANESPEED_DOWN;
				skillactWk[i].Down_active = false;
				// 実行終了！フラグを元に戻す
				skillWk.moving = false;

			}
			
		}

		if(skillactWk[i].Ojyama_active == true)
		{
			// 実行時間を減らして0以下なら終了
			skillactWk[i].OjyamaTime--;

			if(skillactWk[i].OjyamaTime <= 0)
			{
				skillactWk[i].Ojyama_active = false;
				skillWk.moving = false;
			}
		}

		if(skillactWk[i].Kaminari_active == true)
		{
			skillactWk[i].Kaminari_pos.y -= KAMINARI_SPEED;
			//	雷のY座標とプレイヤーのY座標を引いて、プレイヤーの半分の高さを下回っていたらヒット
			if(skillactWk[i].Kaminari_pos.y - player[i].pos.y < HALF_PLAYER_HEIGHT)
			{
				// ライフが減る
				player[i].life--;
				// 未使用に…
				skillactWk[i].Kaminari_active = false;
				skillWk.moving = false;

			}

		}
	}
}

//****************************************************************
// 関数名:	void DrawSkill(void)
// 引数:	なし
// 戻り値:	なし
// 説明:	スキルゲージの描画
//****************************************************************
HRESULT DrawSkill(void)
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

	// スキルバーのバッファ作成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用、確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,		// 使用法
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラス
		&skillWk.Buff_bar,			// 頂点バッファインタフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}



	// 頂点バッファの中身を埋める
	{
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロック＆ポインタを取得
		skillWk.Buff_bar->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SKILLBAR_POS_X + Draw_Skillbar, SKILLBAR_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SKILLBAR_POS_X + Draw_Skillbar, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);

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
		skillWk.Buff_bar->Unlock();
	}


	// バーを描画
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, skillWk.Buff_bar, 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定
		pDevice->SetTexture(0, skillWk.Texture_bar);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

#ifdef _DEBUG
	PrintDebugProc("スキルレベル%d\n", skillWk.lv);
#endif

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

	// スキルバーのバッファ作成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用、確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,		// 使用法
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラス
		&skillWk.Buff_bar,			// 頂点バッファインタフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	// 頂点バッファの中身を埋める
	{
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロック＆ポインタを取得
		skillWk.Buff_bar->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SKILLBAR_POS_X + SKILLBAR_WIDTH, SKILLBAR_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SKILLBAR_POS_X, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SKILLBAR_POS_X + SKILLBAR_WIDTH, SKILLBAR_POS_Y + SKILLBAR_HEIGHT, 0.0f);

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
		skillWk.Buff_bar->Unlock();
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
// 関数名:	void GetSkill(void)
// 引数:	5回飛んだプレイヤーの番号
// 戻り値:
// 説明:	updateskillで権限を持っているプレイヤーが2人以上の場合実行、1人のプレイヤーに権限を与える
//			[Get関数ではないです！]
//*****************************************************************************
void GetSkill(void)
{
	PLAYER *player = GetPlayer(0);
	int skill_life_lower;
	int skill_count_lower;
	int counts = 0;
	srand((unsigned)time(NULL));
	int unmakase = 0;						// 0で初期化

	// 同時に2人以上が条件を満たしていたら下記の判定

	// ライフが少ない人
	skill_life_lower = skillsort_life();

	// 上記判定後も権利所有者が2人以上いた場合の処理
	if(skillcheck_ok == false)
	{
		// 権限を持った人が一番少ない順番
		skill_count_lower = skill_count_winner();
	}
	else
	{
		skillWk.kengen = true;
	}


	// それでも駄目な場合の処理
	if(skillcheck_ok == false)
	{
		while(0)
		{
			srand((unsigned)time(NULL));
			unmakase = rand() % MAX_PLAYER;			// 適当なとこから開始
			if(player[unmakase].kengen == true)
			{
				break;
				skillcheck_ok = true;
				skillWk.kengen = true;
			}
		}

		// それ以外のプレイヤーは権限剥奪及びポイントリセット
		for(int i = 0; i < MAX_PLAYER; i++)
		{
			if(player[i].kengen == true && i != unmakase)
			{
				player[i].kengen = false;
				SkillReset(i);
			}
		}
	}



}


//******************************************************************************
// 関数名:	int skillsort_life(void)
// 引数:	なし
// 戻り値:	int winner
// 説明:	プレイヤーの体力を比較して一番ライフが少ないプレイヤーの番号を返す
//******************************************************************************
int skillsort_life(void)
{
	PLAYER *player = GetPlayer(0);
	int winner = 0;					// 勝者の番号
	bool sort_life = true;			// この方法でプレイヤーを一人に絞れたか　

									// 今はライフが同じ場合、番号が若いプレイヤーが権利を得る

	for(int no = 1; no < MAX_PLAYER; no++)
	{
		// 御互いの体力が同じ場合
		if(player[winner].life == player[no].life)
		{
			sort_life = false;
		}

		// 現在の勝者の体力より少ない人がでたら
		if(player[winner].life > player[no].life)
		{
			player[winner].kengen = false;				// 権限を失い
			SkillReset(winner);							// ポイントも失う
			winner = no;								// その人をwinnerに
		}




	}

	return winner;
}

//******************************************************************
// 関数名:	int skill_count_winner(void)
// 引数:	なし
// 戻り値:	count_winner
// 説明:	スキルの発動回数が少ない人の番号を返す
//******************************************************************
int skill_count_winner(void)
{
	PLAYER *player = GetPlayer(0);
	int count_winner = 0;				// 0番目から比較開始
	int shaka = 0;						// 権限持ちを探すときに使う関数
	skillcheck_ok = true;				// 抜けられると仮定

										// 若い順から見ていって権限を持っているプレイヤーを探す
	while(shaka = 0)
	{
		if(player[count_winner].kengen == false)
		{
			count_winner++;
		}
		else
		{
			shaka++;
		}
	}

	for(int i = 1; i < MAX_PLAYER; i++)	// 0番目と1番目から比較開始
	{
		if(skill_flag[count_winner].count == skill_flag[i].count)
		{
			skillcheck_ok = false;			// 2人以上いる！
		}
		else if(skill_flag[count_winner].count > skill_flag[i].count)
		{// 自分よりも小さい値が見つかった
			player[count_winner].kengen = false;		// 権限をはく奪して
			SkillReset(count_winner);					// ポイントもリセット
			count_winner = i;							// 勝者を残す
		}
	}
	return count_winner;
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

//************************************************************************************************
// 関数名:	void SkillAct(int player_no)
// 引数:	int player_no(権限を持っているプレイヤーの番号）
// 戻り値:	なし
// 説明:	権限を持っているプレイヤーの番号を受け取り効果発動
//			SetSkillAct内のPOSを調整すると画面内の位置も変わる…はず
//************************************************************************************************
void SkillAct(int player_no)
{
	PLAYER *player = GetPlayer(0);
	LANE *lane = GetLane(0);
	srand((unsigned)time(NULL));
	int randum;

	// スキル発動中の場合実行されない
	if(skillWk.moving != true)
	{
		// 効果発動時に権限を失う
		player[player_no].kengen = false;
		SkillReset(player_no);
		skillWk.moving = true;
		

		// 効果の発動
		// 各プレイヤーごとに効果を発動していく
		for(int i = 0; i < MAX_PLAYER; i++)
		{
			// スキル発動者ははじく
			if(i != player_no)
			{
				// スキルレベルで分岐させたい
				switch(skillWk.lv)
				{
				case SPEEDCHANGE:
					// 動作
					randum = rand() % 2;
					switch(randum)
					{
					case 0:

						// 加速
						lane[i].speed_factor += LANESPEED_UP;
						SetSkillAct(D3DXVECTOR3(player[i].pos.x- UPDOWNPOS_X, player[i].pos.y+UPDOWNPOS_Y, player[i].pos.z),
							EFFECT_UP, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						// 音鳴らす
						PlaySound(SOUND_LABEL_SKILL_SPEEDUP);
						// 時間設定

						break;
					case 1:
						// 減速
						lane[i].speed_factor -= LANESPEED_DOWN;
						SetSkillAct(D3DXVECTOR3(player[i].pos.x-UPDOWNPOS_X, player[i].pos.y, player[i].pos.z),
							EFFECT_UP, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						// 音鳴らす
						PlaySound(SOUND_LABEL_SKILL_SPEEDDOWN);
						break;
					}
					break;

				case OJYAMA:
					// 動作
					// ブロックの表示のみ
					// 音鳴らす
					PlaySound(SOUND_LABEL_SKILL_OJYAMA);
					SetSkillAct(D3DXVECTOR3(player[i].pos.x, player[i].pos.y, player[i].pos.z),
						EFFECT_OJYAMA, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				case KAMINARI:
					// 動作
					// 音鳴らす
					PlaySound(SOUND_LABEL_SKILL_THANDER);
					// ライフ減少は雷とプレイヤーがぶつかったときに
					SetSkillAct(D3DXVECTOR3(player[i].pos.x, player[i].pos.y, player[i].pos.z),
						EFFECT_KAMINARI, i, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				}
			}
		}

		skillWk.lv = 0;
	}

}

//****************************************************
// void skillwinner(int no)
// 与えられた番号のみ権限付与しその他剥奪
//****************************************************
void skillwinner(int no)
{
	PLAYER *player = GetPlayer(0);

	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(i == no)
		{
			player[i].kengen = true;
		}
		else
		{
			player[i].kengen = false;
			SkillReset(i);
		}
	}
}


//*******************************************************************************
// 関数名:	SKILL GetSkillWk(int no)
// 引数:	int no
// 戻り値:	SkillWk(no)
// 説明:	skill.cppのみこっちがゲット関数です
//*******************************************************************************
SKILL *GetSkillWk(int no)
{
	return &skillWk;
}

//*******************************************************************
// メモ書き！
//*******************************************************************

// 無敵時間の間はカウントをプラスされない！！！
/*
stage.h内のGetRane
speedfactor
//*****************************************************************************
typedef struct {
bool use;			// プレイヤーが参加していればtrue
float speed_factor;	ここを弄るとスピード操作が可能(1.0未満で遅く)
float lane_length;	// レーン1週分の長さ
} LANE;


*/