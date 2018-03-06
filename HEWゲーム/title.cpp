//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "title.h"

#include "camera.h"
#include "main.h"
#include "mesh.h"
#include "debugproc.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BUTTON_GAME_START	(BUTTON_START)

#define	TEXTURE_TITLE		"data/TEXTURE/bg_title.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO	"data/TEXTURE/title_logo.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START	"data/TEXTURE/press_start.png"	// 読み込むテクスチャファイル名

#define	TITLE_LOGO_POS_X		(SCREEN_CENTER_X)		// タイトルロゴの位置(X座標)
#define	TITLE_LOGO_POS_Y		(SCREEN_CENTER_Y)		// タイトルロゴの位置(Y座標)
#define	TITLE_LOGO_WIDTH		(353.0f)		// タイトルロゴの幅
#define	TITLE_LOGO_HEIGHT		(172.0f)		// タイトルロゴの高さ

#define	START_POS_X				(SCREEN_CENTER_X - 300)		// スタートボタンの位置(X座標)
#define	START_POS_Y				(SCREEN_CENTER_Y + 200)		// スタートボタンの位置(Y座標)
#define	START_WIDTH				(353.0f * 2)		// スタートボタンの幅
#define	START_HEIGHT			(172.0f * 2)		// スタートボタンの高さ

#define	COUNT_APPERA_START		(60)		// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(60)		// スタートボタン点滅の時間

#define	COUNT_WAIT_DEMO			(60 * 5)	// デモまでの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetColorTitleLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// 頂点バッファインターフェースへのポインタ
int						g_nCountAppearStart = 0;		//
float					g_fAlphaLogo = 0.0f;			//
int						g_nCountDisp = 0;				//
bool					g_bDispStart = false;			//
int						g_nConutDemo = 0;				//

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearStart = 0;
	g_fAlphaLogo = 0.0f;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_nConutDemo = 0;

	// 頂点情報の作成
	MakeVertex(pDevice, &g_pD3DVtxBuffTitle, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);
	MakeVertex(pDevice, &g_pD3DVtxBuffStart, NULL, START_WIDTH, START_HEIGHT);	// "PRESS START"
	
	// テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE,				// ファイルの名前
		&g_pD3DTextureTitle)))		// 読み込むメモリー
	{
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_LOGO_START,			// ファイルの名前
		&g_pD3DTextureStart)))		// 読み込むメモリー
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if(g_pD3DTextureTitle != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if(g_pD3DVtxBuffTitle != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTitle->Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	if(g_pD3DTextureStart != NULL)
	{// テクスチャの開放
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if(g_pD3DVtxBuffStart != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
#if 0
	if(g_nCountAppearStart >= COUNT_APPERA_START)
	{
		g_nConutDemo++;
		if(g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
#endif

	if(g_fAlphaLogo < 1.0f)
	{
		g_fAlphaLogo += 0.005f;
		if(g_fAlphaLogo >= 1.0f)
		{
			g_fAlphaLogo = 1.0f;
		}
		SetColorTitleLogo();
	}
	else
	{
		g_nCountAppearStart++;
		if(g_nCountAppearStart > COUNT_APPERA_START)
		{
			g_nCountDisp = (g_nCountDisp + 1) % 80;
			if(g_nCountDisp > INTERVAL_DISP_START)
			{
				g_bDispStart = false;
			}
			else
			{
				g_bDispStart = true;
			}
		}
	}

	bool start = false;
	// パッドでスタート
	for (int pad_no = 0; pad_no < NumPad(); pad_no++) {
		if (IsButtonTriggered(pad_no, BUTTON_GAME_START))
			start = true;
	}
	// キーボードでスタート
	if (GetKeyboardTrigger(DIK_RETURN))
		start = true;;

	if (start)
	{
		if (g_nCountAppearStart == 0)
		{// タイトル登場スキップ
			g_fAlphaLogo = 1.0f;
			SetColorTitleLogo();

			g_nCountAppearStart = COUNT_APPERA_START;
		}
		else
		{// ゲームへ

			SetFade(FADE_OUT);
			PlaySound(SOUND_LABEL_GAMESTART);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// タイトル描画
	DrawMesh(g_pD3DVtxBuffTitle, g_pD3DTextureTitle, D3DXVECTOR3(0.0f, 0.0f, NearZ()), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// "PRESS START"描画
	DrawMesh(g_pD3DVtxBuffStart, g_pD3DTextureStart, D3DXVECTOR3(0.0f, -300.0f, NearZ()), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//// 頂点バッファをデバイスのデータストリームにバインド
 //   pDevice->SetStreamSource(0, g_pD3DVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_pD3DTextureTitle);

	//// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	if(g_bDispStart == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureStart);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
void SetColorTitleLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// 頂点データをアンロックする
		g_pD3DVtxBuffStart->Unlock();
	}

}

