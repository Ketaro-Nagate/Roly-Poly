//==================================================================================
//
//  ゲームマネージャー [manager.cpp]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "manager.h"
#include "renderer.h"
#include "DebugProc.h"
#include "object2D.h"
#include "object3D.h"
#include "texture.h"
#include "scene.h"

#include "player.h"
#include "prop.h"
#include "field.h"
#include "wall.h"
#include "item.h"
#include "score.h"

//**********************************************************************************
//変数宣言
//**********************************************************************************
bool CManager::m_bFade = false;

#define NUM_LIGHT 3		// ライトの数

//==================================================================================
// コンストラクタ
//==================================================================================
CManager::CManager()
{
	m_pRenderer = nullptr;
	m_pInputKeyboard = nullptr;
	m_bInGame = false;
	m_pDebugProc = nullptr;
	m_pInputKeyboard = nullptr;
	m_pInputMouse = nullptr;
	m_pCamera = nullptr;
	m_pScene = nullptr;
	m_pTexture = nullptr;
}

//==================================================================================
// デストラクタ
//==================================================================================
CManager::~CManager()
{

}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	CObject::ReleaseAll();

	//--- レンダラーインスタンス生成・初期化 ---//
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, bWindow);
	}

	//--- テクスチャの読み込み ---//
	if (m_pTexture == nullptr)
	{
		m_pTexture = CTexture::GetInstance();
		m_pTexture->Register("data\\TEXTURE\\0_effect000.jpg");		// エフェクト
		m_pTexture->Register("data\\TEXTURE\\wall004.png");			// 壁
		m_pTexture->Register("data\\TEXTURE\\field_001.png");		// 床
		m_pTexture->Register("data\\TEXTURE\\warning_001.png");		// 敵の警告表示
		m_pTexture->Register("data\\TEXTURE\\title.png");			// タイトルロゴ
		m_pTexture->Register("data\\TEXTURE\\under.png");			// 地面
		m_pTexture->Register("data\\TEXTURE\\food.png");			// 地面
	}

	//--- キーボードインスタンス生成・初期化 ---//
	if (m_pInputKeyboard == nullptr)
	{
		m_pInputKeyboard = new CInputKeyboard;
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	//--- マウスインスタンス生成・初期化 ---//
	if (m_pInputMouse == nullptr)
	{
		m_pInputMouse = new CInputMouse;
		m_pInputMouse->Init(hInstance, hWnd);
	}

	//--- デバッグ表示 ---//
	if (m_pDebugProc == nullptr)
	{
		m_pDebugProc = new CDebugProc;
		m_pDebugProc->Init();
	}

	//--- カメラの生成 ---//
	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;
		m_pCamera->Init();
	}

	//--- シーンを生成 ---//
	if (m_pScene == nullptr)
	{
		m_pScene = CScene::Create(CScene::MODE_TITLE);
	}
	
	return S_OK;
}

//==================================================================================
// 終了
//==================================================================================
void CManager::Uninit(void)
{
	CObject::ReleaseAll();
	//CObject::CheckDead();

	if (m_pRenderer != nullptr)
	{
		// レンダラーの終了
		m_pRenderer->Uninit();

#ifndef CSCENE_COBJECT			// オブジェクトが破棄してくれる
#else							// マネージャーで消す
		// レンダラーのインスタンスを破棄
		delete m_pRenderer;
		m_pRenderer = nullptr;
#endif // !CSCENE_COBJECT
	}

	if (m_pInputKeyboard != nullptr)
	{
		// キーボードの終了
		m_pInputKeyboard->Uninit();

		// キーボードのインスタンスを破棄
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputMouse != nullptr)
	{
		// マウスの終了
		m_pInputMouse->Uninit();

		// マウスのインスタンスを破棄
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{
		// デバッグ表示の終了
		m_pDebugProc->Uninit();

		// デバッグ表示のインスタンスを破棄
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pScene != nullptr)
	{
		// シーンの終了処理
		m_pScene->Uninit();

		// シーンのインスタンスを破棄
		delete m_pScene;
		m_pScene = nullptr;

	}

	if (m_pCamera != nullptr)
	{
		// カメラの終了
		m_pCamera->Uninit();

		// カメラのインスタンスを破棄
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pTexture != nullptr)
	{
		// テクスチャの破棄
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//==================================================================================
// 更新
//==================================================================================
void CManager::Update(void)
{
	//--- デバイス類更新 ---//
	m_pInputKeyboard->Update();
	m_pInputMouse->Update();
	m_pCamera->Update();

	if(m_bPause == false)		// ポーズ中なら止める
	m_pRenderer->Update();

	if (m_pScene != nullptr)	// シーンが空なら更新しない
		m_pScene->Update();

	// デバック表示の切り替え
	if (m_pInputKeyboard->GetTrigger(DIK_F1))
	{
		m_pDebugProc->SwitchDebugProc();
	}

	// ポーズ切り替え
	if (m_pInputKeyboard->GetTrigger(DIK_P))
	{
		m_bPause ^= true;
	}

	// DebugProcの表示切り替え
	m_pDebugProc->PrintDebug("-----------------------------------\n");
	m_pDebugProc->PrintDebug("F1 / デバッグ表示切り替え\n");
	m_pDebugProc->PrintDebug("F2 / 取得アイテム再配置\n");
	m_pDebugProc->PrintDebug("P  / ポーズ\n");
}

//==================================================================================
// 描画
//==================================================================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();

	//if(m_pScene != nullptr)
		//m_pScene->Draw();
}

//==================================================================================
// レンダラー取得
//==================================================================================
CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//==================================================================================
// キーボード取得
//==================================================================================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//==================================================================================
// キーボード取得
//==================================================================================
CInputMouse* CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}

//==================================================================================
// デバッグ表示取得
//==================================================================================
CManager* CManager::GetManager(void)
{
	static CManager manager;

	return &manager;
}

//==================================================================================
// デバッグ表示取得
//==================================================================================
CDebugProc* CManager::GetDebugProc(void)
{
	return m_pDebugProc;
}

//==================================================================================
// モードセット
//==================================================================================
void CManager::SetMode(CScene::MODE mode)
{
	// フェードオン
	//m_bFade = true;

	// サウンド停止

	// 現在のモードの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();

#ifndef CSCENE_COBJECT			// オブジェクトが破棄

#else							// マネージャー側で破棄
		delete m_pScene;
#endif // !CSCENE_COBJEKT
		m_pScene = nullptr;
	}

	// すべてのオブジェクトの破棄
	CObject::ReleaseAll();

	// 新しいモードの生成
	m_pScene = CScene::Create(mode);

}

//==================================================================================
// 現在のモードを取得
//==================================================================================
CScene::MODE CManager::GetMode(void)
{
	return m_pScene->GetMode();
}