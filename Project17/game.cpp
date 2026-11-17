//==================================================================================
//
// ゲームシーン [game.cpp]
// Author : Nagate
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "game.h"
#include "camera.h"
#include "texture.h"
#include "light.h"
#include "player.h"
#include "prop.h"
#include "score.h"
#include "field.h"
#include "wall.h"
#include "enemy_bird.h"
#include "item_manager.h"
#include "object2D.h"
#include "enemy_manager.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define NUM_LIGHT 3


//==================================================================================
// コンストラクタ
//==================================================================================
CGame::CGame():CScene(MODE_GAME)
{
	m_pPlayer = nullptr;
	m_pProp = nullptr;
	m_pScore = nullptr;
	memset(&m_pLight, 0, sizeof(m_pLight));
	m_bEnd = false;
	m_NextScene = CScene::MODE_GAME;
	m_bPause = false;
	m_pItemManager = nullptr;
	m_pScoreTex = nullptr;
	m_timer = nullptr;
	m_nCntGame = 0;
	m_pEnemyManager = nullptr;
}

//==================================================================================
// デストラクタ
//==================================================================================
CGame::~CGame()
{

}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CGame::Init(CScene::MODE mode)
{
	CObject::ReleaseAll();

	// テクスチャへのポインタ取得
	CManager* pManager = CManager::GetManager();
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	//--- ライトの生成 ---//
	if (m_pLight[0] == nullptr)
	{
		m_pLight[0] = new CLight;
		m_pLight[0]->Init(D3DXVECTOR3(0.2f, 0.8f, 0.4f), 0);
	}
	if (m_pLight[1] == nullptr)
	{
		m_pLight[1] = new CLight;
		m_pLight[1]->Init(D3DXVECTOR3(-0.2f, -0.8f, -0.4f), 1);
	}
	if (m_pLight[2] == nullptr)
	{
		m_pLight[2] = new CLight;
		m_pLight[2]->Init(D3DXVECTOR3(0.2f, -1.0f, 0.2f), 2);
	}

	//--- プロップの生成 ---//
	if (m_pProp == nullptr)
	{
		m_pProp = new CProp;
		m_pProp->Load("data\\TEXT\\MapProp.txt");
	}

	//--- プレイヤーの生成 ---//
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create();
	}

	//--- アイテムマネージャーの生成 ---//
	if (m_pItemManager == nullptr)
	{
		m_pItemManager = new CItemManager;
		m_pItemManager->Init();
	}

	//--- スコアUIの生成 ---//
	if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(1080.0f, 50.0f, 0.0f), 20.0f, 20.0f, 5);
		m_pScore->SetColor(D3DXCOLOR(0.73f, 0.52f, 0.04f, 1.0f));
		m_pScore->AddScore(pManager->GetScore());
	}

	//--- 落ち葉のUIの生成 ---//
	if (m_pScoreTex == nullptr)
	{
		m_pScoreTex = CObject2D::Create(D3DXVECTOR3(1000.0f, 40.0f, 0.0f), 70.0f, 70.0f);
		m_pScoreTex->BindTexture(pTexture->GetAddress(CTexture::TEX_FOOD));
	}

	//--- 残り時間の生成 ---//
	if (m_timer == nullptr)
	{
		m_timer = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f), 20.0f, 20.0f, 2);
		m_timer->SetColor(D3DXCOLOR(0.73f, 0.52f, 0.04f, 1.0f));
		m_timer->AddScore(pManager->GetTimeLimit());
	}

	//--- エネミーマネージャーの生成 ---//
	if (m_pEnemyManager == nullptr)
	{
		m_pEnemyManager = CEnemyManager::Create(120, 4);
	}

	CWall::Create(D3DXVECTOR3(0.0f, -250.0f, -750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1500.0f, 500.0f, 0.0f), pTexture->GetAddress(CTexture::TEX_UNDER));		// 手前
	CWall::Create(D3DXVECTOR3(-750.0f, -250.0f, 0.0f), D3DXVECTOR3(0.0f, HALF_PI, 0.0f), D3DXVECTOR3(1500.0f, 500.0f, 0.0f), pTexture->GetAddress(CTexture::TEX_UNDER));	// 左
	CWall::Create(D3DXVECTOR3(750.0f, -250.0f, 0.0f), D3DXVECTOR3(0.0f, -HALF_PI, 0.0f), D3DXVECTOR3(1500.0f, 500.0f, 0.0f), pTexture->GetAddress(CTexture::TEX_UNDER));	// 右
	CWall::Create(D3DXVECTOR3(0.0f, -250.0f, 750.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(1500.0f, 500.0f, 0.0f), pTexture->GetAddress(CTexture::TEX_UNDER));		// 奥


	return S_OK;
}

//==================================================================================
// 終了
//==================================================================================
void  CGame::Uninit(void)
{
	// マネージャーに今回のスコアを渡す
	CManager::GetManager()->SetScore(m_pScore->GetScore());

	//==================== 破棄 =========================//
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		if (m_pLight[nCntLight] != nullptr)
		{
			// ライトの終了
			m_pLight[nCntLight]->Uninit();

			// ライトのインスタンスを破棄
			delete m_pLight[nCntLight];
			m_pLight[nCntLight] = nullptr;
		}
	}

	if (m_pProp != nullptr)
	{
		// プロップ破棄
		m_pProp->Unload();

		// インスタンス破棄
		delete m_pProp;
		m_pProp = nullptr;
	}

	if (m_pItemManager != nullptr)
	{
		m_pItemManager->Uninit();

		// インスタンス破棄
		delete m_pItemManager;
		m_pItemManager = nullptr;
	}

	if (m_pEnemyManager != nullptr)
	{
		m_pEnemyManager->Uninit();

		// インスタンス破棄
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}

	// オブジェクトを破棄
	CObject::ReleaseAll();

	// カメラリセット
	CManager::GetManager()->GetCamera()->ResetCamera();

	//--- テクスチャ破棄 ---//

#ifndef CSCENE_COBJECT
	CObject::Release();
#endif // !CSCENE_COBJECT
}

//==================================================================================
// 更新
//==================================================================================
void  CGame::Update(void)
{
	CManager* pManager = CManager::GetManager();
	CInputKeyboard* pKey = pManager->GetInputKeyboard();

	m_pItemManager->Update();
	m_pEnemyManager->Update();

	// 制限時間タイマー加算
	m_nCntGame++;
	if (m_nCntGame % 60 == 0)
	{
		m_timer->AddScore(-1);
	}

	// 制限時間経過で終了フラグ立てる
	if (m_timer->GetScore() == 0)
	{
		EndScene(MODE_RANKING);
	}

	// エンター || フラグ検知で遷移
	if (pKey->GetTrigger(DIK_RETURN))
	{
		m_NextScene = MODE_RANKING;
		pManager->SetMode(m_NextScene);
	}
	else if (m_bEnd == true)
	{
		pManager->SetScore(m_pScore->GetScore());			// 獲得スコアを保存
		pManager->SetTimeLimit(m_timer->GetScore());		// 残り制限時間
		pManager->SetMode(m_NextScene);
	}
	
}

//==================================================================================
// 描画
//==================================================================================
void CGame::Draw(void)
{

}

//==================================================================================
// シーンの終了
//==================================================================================
void CGame::EndScene(CScene::MODE nextmode)
{
	m_bEnd = true;
	m_NextScene = nextmode;
}