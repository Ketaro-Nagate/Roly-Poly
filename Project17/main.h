//==================================================================================
//
// メイン [main.h]
// Author : 長手　桂太朗
//
//==================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//**********************************************************************************
// ファイルインクルード
//**********************************************************************************
#include<WinSock2.h>					// サーバー処理用
#include<windows.h>
#include "d3dx9.h"						//描画処理に必要
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド処理に必要
#include "xinput.h"						//ジョイパッド処理に必要

#define _CRTDBG_MAP_ALLOC


#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)		// メモリリーク検知new


//**********************************************************************************
// ライブラリのリンク
//**********************************************************************************
#pragma comment (lib,"ws2_32.lib")	// サーバー処理用
#pragma comment(lib,"d3d9.lib")		//描画に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻終了に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment(lib,"xinput.lib")	//ジョイパッド処理に必要


//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define SCREEN_WIDTH (1280)					//ウィンドウの幅
#define SCREEN_HEIGHT (720)					//ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット[2D]
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット[3D]

#define NULL_D3DXVECTOR3 D3DXVECTOR3(0.0f,0.0f,0.0f)	//D3DXVECTOR3の全てが0
#define WHITE D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)			// 白
#define RED D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)				// 赤
#define GREEN D3DXCOLOR(0.0f,1.0f,0.0f,1.0f)			// 緑
#define BLUE D3DXCOLOR(0.0f,0.0f,1.0f,1.0f)				// 青

#define DOUBLE_PI	(D3DX_PI * 2.0f)			// 3.14 * 2.0
#define HALF_PI		(D3DX_PI * 0.5f)			// 3.14 * 0.5


//**********************************************************************************
// 頂点情報[2D]の構造体を定義
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標(x,y,z)
	float rhw;				//座標変更用係数(1.0fで固定)
	D3DCOLOR col;			//頂点カラー(r,g,b,a)
	D3DXVECTOR2 tex;		//テクスチャ座標(x,y)

}VERTEX_2D;

//**********************************************************************************
//頂点情報[3D]の構造体を定義
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標(x,y,z)
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DCOLOR col;			//頂点カラー(r,g,b,a)
	D3DXVECTOR2 tex;		//テクスチャ座標(x,y)
}VERTEX_3D;

#endif // !_MAIN_H_