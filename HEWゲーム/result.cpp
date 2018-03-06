//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : 
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "mesh.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/bg_result.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO	"data/TEXTURE/result_logo.png"	// 読み込むテクスチャファイル名

#define	RESULT_LOGO_POS_X	(240)		// リザルトロゴの位置(X座標)
#define	RESULT_LOGO_POS_Y	(320)		// リザルトロゴの位置(Y座標)
#define	RESULT_LOGO_WIDTH	(800)		// リザルトロゴの幅
#define	RESULT_LOGO_HEIGHT	(240)		// リザルトロゴの高さ

#define	COUNT_APPERA_RESULT	(60)		// リザルトロゴ出現までの待ち時間	
#define	LIMIT_COUNT_WAIT	(60 * 5)	// 待ち時間


// ナンバー
#define RESULTNO_HEIGHT	(SCREEN_HEIGHT /4)
#define RESULTNO_WIDTH	(SCREEN_HEIGHT /4)
#define RESULTNO_1	("data/TEXTURE/result_1st.png")
#define RESULTNO_2	("data/TEXTURE/result_2nd.png")
#define RESULTNO_3	("data/TEXTURE/result_3rd.png")
#define RESULTNO_4	("data/TEXTURE/result_4th.png")

#define MAX_RANK	(4)				// びりのじゅんい

// 順位の表示位置
#define RANKNO_POS_X			(30.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void SetColorResultLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultLogo = NULL;	// 頂点バッファインターフェースへのポインタ
int						g_nCountAppearResult = 0;		// 出現までの待ち時間
float					g_fAlphaResult = 0.0f;			// リザルトロゴのα値
int						g_nCountWaitResult = 0;			// 待ち時間
int						checkcount;					// 生存プレイヤー数カウント
RESULT resultWk[MAX_PLAYER];

// ここから先は数字を表示するためのもの
RESULTNO resultno[4];

char *resultno_FileName[] =
{
	RESULTNO_1,
	RESULTNO_2,
	RESULTNO_3,
	RESULTNO_4,
};

bool rank_ok[MAX_PLAYER];
//*******************************************************
// void InitCount(void)
// カウント初期化
// result.cppへの移動に利用するcheckcountの値を初期化
// フラグもここで初期化
//*******************************************************
void InitCount(void)
{
	checkcount = MAX_PLAYER;
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		rank_ok[i] = false;
	}

	for(int i = 0; i < MAX_PLAYER; i++)
	{
		resultWk[i].rank = 0;			// 全員のランクを1位に
	}
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearResult = 0;
	g_fAlphaResult = 0.0f;
	g_nCountWaitResult = 0;

	// 頂点情報の作成
	MakeVertexResult(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT,				// ファイルの名前
		&g_pD3DTextureResult);		// 読み込むメモリー

									// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_RESULT_LOGO,		// ファイルの名前
		&g_pD3DTextureResultLogo);	// 読み込むメモリー

	for(int i = 0; i < MAX_RANK; i++)
	{
		// 順位表示用テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			resultno_FileName[i],
			&resultno[i].texture);
	}

	checkcount = MAX_PLAYER;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if(g_pD3DTextureResult != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if(g_pD3DVtxBuffResult != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResult->Release();
		g_pD3DVtxBuffResult = NULL;
	}

	if(g_pD3DTextureResultLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}

	if(g_pD3DVtxBuffResultLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResultLogo->Release();
		g_pD3DVtxBuffResultLogo = NULL;
	}

	for(int i = 0; i < MAX_RANK; i++)
	{
		if(resultno[i].texture != NULL)
		{// 開放
			resultno[i].buff->Release();
			resultno[i].buff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	g_nCountAppearResult++;
	if(g_nCountAppearResult >= COUNT_APPERA_RESULT)
	{
		if(g_fAlphaResult < 1.0f)
		{
			g_fAlphaResult += 0.05f;
			if(g_fAlphaResult >= 1.0f)
			{
				g_fAlphaResult = 1.0f;
			}

			SetColorResultLogo();
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、フェードアウトしてモードを切り替えいく
		SetFade(FADE_OUT);
	}

	/*g_nCountWaitResult++;
	if(g_nCountWaitResult > LIMIT_COUNT_WAIT)
	{
	SetFade(FADE_OUT);
	}*/
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ロゴ
	//************************************************************************************

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// てすと　らんく
	// 頂点バッファをデバイスのデータストリームにバインド
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		

		pDevice->SetStreamSource(0, resultno[i].buff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, resultno[resultWk[i].rank].texture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResult,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResult->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResultLogo,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}


	//+++++++++++++++++++++++++++++++
	// ここからは順位によって変わる
	//+++++++++++++++++++++++++++++++
	for(int i = 0; i < MAX_PLAYER; i++)
	{
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&resultno[i].buff,		// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			resultno[i].buff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f + (i*RESULTNO_HEIGHT), 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RESULTNO_WIDTH, 0.0f + (i*RESULTNO_HEIGHT), 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, RESULTNO_HEIGHT + (i*RESULTNO_HEIGHT), 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RESULTNO_WIDTH, RESULTNO_HEIGHT+ (i*RESULTNO_HEIGHT), 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロックする
			resultno[i].buff->Unlock();
		}
	}
	/*for(int i = 0; i < MAX_RANK; i++)
	{
		MakeVertex(pDevice, &resultno[i].buff, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), RESULTNO_WIDTH, RESULTNO_HEIGHT);
	}*/
	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResultLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}
}


//*********************************************************************
// void RankCheck(void)
// 説明: プレイヤーのライフが更新の結果0になった場合実行される
//*********************************************************************
void RankCheck(int no, int rank)
{

	// 受け取ったnoのプレイヤーが既に離脱していなければ実行
	if(rank_ok[no] == false)
	{
		rank_ok[no] = true;
		// キャラクターの生存数を減らす
		checkcount--;
		// ランクを決定

		resultWk[no].rank = rank-1;			// rankの値より1少ない番号が順位
		// 生きているキャラクターが居ない場合リザルト画面へ
		if(checkcount < 1)
		{
			SetFade(FADE_OUT);
			//PlaySound(SOUND_LABEL_GAMESTART);
		}
	}
}

//*********************************************************************
// RESULT GetResult(int no)
// ゲット関数
// 指定された番号を返す
//*********************************************************************
RESULT *GetResult(int no)
{
	return &resultWk[no];
}
