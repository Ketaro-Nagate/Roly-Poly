//==================================================================================
//
// DirectXを使う[main.cpp]
// Author : 長手　桂太朗
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include <crtdbg.h>
#include "main.h"
#include "input.h"
#include "manager.h"
#include <cstdlib>
#include <ctime>
using namespace std;

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define CLASS_NAME	"WindowClass"			//WindowClassの名前
#define WINDOW_NAME	"ROLY-POLY!"		//ウィンドウ教示処理

#define FPS 60								//更新

#define INITIALIZATION_VECTOR D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INITIALIZATION_COLOR D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)

//**********************************************************************************
// プロトタイプ宣言
//**********************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//**********************************************************************************
//グローバル変数
//**********************************************************************************

//==================================================================================
//メイン関数
//==================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//--- ローカル変数宣言 ---//
	CManager* pManager = NULL;				// マネージャインスタンスへのポインタ
	int g_nCountFPS = 0;			// FPSへのカウンタ

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
#endif

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//画面サイズの構造体

	HWND hWnd;
	MSG msg;

	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時間(保存)
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測したときの時刻

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// レンダラーのインスタンスを生成
	pManager = CManager::GetManager();

	//初期化処理
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{//初期化処理に失敗した場合
		if (pManager != nullptr)
			delete pManager;
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得(保存)

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//--- 乱数設定 ---//
	srand((unsigned)time(NULL));

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != NULL)
		{

			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();	//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5s経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPSを計測した時刻を保存
				dwFrameCount = 0;				//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))
			{//６０分の1秒経過
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻[現在時刻]を保存

				//更新処理
				pManager->Update();

				//描画処理
				pManager->Draw();

				dwFrameCount++;
			}

		}
	}

	//分解能を戻す
	timeEndPeriod(1);
	
	pManager->Uninit();

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//==================================================================================
// ウィンドウプロシージャ
//==================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WMQUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN://キー押下メッセージ
		switch (wParam)
		{
		case VK_ESCAPE:
			//ウィンドウを破棄する（WM_DESTROYメッセージを送る）
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE://閉じるボタン押下
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;//0を返さないと終了してしまう
		}
		break;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);//規定の処理を返す
}