//==================================================================================
//
//  アイテム [item.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "main.h"
#include "item.h"
#include "manager.h"
#include "game.h"
#include "objectX.h"
#include "player.h"
#include "score.h"
#include "util.h"
#include "prop.h"
#include "scene.h"
#include "item_manager.h"

//**********************************************************************************
//	マクロ定義
//**********************************************************************************
#define SIZE (40.0f)			// 判定上サイズ

//==================================================================================
// コンストラクタ
//==================================================================================
CItem::CItem()
{
	m_pos = NULL_D3DXVECTOR3;		// 位置
	m_rot = NULL_D3DXVECTOR3;		// 角度
	m_fRadius = SIZE;

	m_fMoveLoop = 0.0f;
	m_bGet = false;
	m_nGetCount = 0;

	m_fmodelsize = 1.0f;
	m_bDrawScaling = false;
	m_bTimeup = false;

	m_pParticle = nullptr;
}

//==================================================================================
// デストラクタ
//==================================================================================
CItem::~CItem()
{
}

//==================================================================================
// 生成( 生成モデルのファイル指定 )
//==================================================================================
CItem* CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,const char* pFileName)
{
	// インスタンス生成
	CItem* pItem = new CItem;

	// 初期化
	pItem->Init(pos, rot, pFileName);

	// ポインタをかえす
	return pItem;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CItem::Init(void)
{
	return S_FALSE;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFileName)
{
	// Xモデルの初期化
	CObjectX::Init(pos, rot, pFileName);

	// 位置角度の保存
	m_pos = pos;
	m_rot = rot;

	return S_OK;
}

//==================================================================================
// 終了処理
//==================================================================================
void CItem::Uninit(void)
{
	if (m_pParticle != nullptr)
	{
		m_pParticle->Uninit();
	}

	CObjectX::Uninit();
}
//==================================================================================
// 更新処理
//==================================================================================
void CItem::Update(void)
{
	// プレイヤーを取得
	CManager* pManager = CManager::GetManager();
	CScene* pScene = pManager->GetScene();
	CPlayer* pPlayer = pScene->GetPlayer();

	// 当たり判定
	if (BallCollision(pPlayer->GetPPos(), pPlayer->GetRadius()))
	{
		m_bGet = true;
	}

	if (m_bGet == true)
	{
		if (m_pParticle == nullptr)
		{
			m_pParticle = CParticle::Create(&m_pos, 5.0f, 5.0f, WHITE, WHITE, 1, 0);
		}

		// 上昇
		m_nGetCount++;

		// ゲットされてからのカウント
		if (m_nGetCount > 45)
		{// 消去時処理
			CManager* pManager = CManager::GetManager();
			CGame* pGame = dynamic_cast<CGame*>(pManager->GetScene());
			CItemManager* pItemManager = pGame->GetItemManager();

			CProp* pProp = pGame->GetPropA();
			pProp->AddDeath();

			// スコアを加算
			CScore* pScore = pGame->GetScore();
			pScore->AddScore(100);

			// 終了処理
			pItemManager->Remove(this);
			return;
		}
	}
	else// 生きている
	{
		// 上下に動かす
		m_fMoveLoop += 0.05f;
		m_fMoveLoop = RotCorrection(m_fMoveLoop);
		m_pos.y += cosf(m_fMoveLoop) * 0.5f;

		// CobjectXとの同機
		SetPos(m_pos);
		SetRot(m_rot);
	}

	if (m_bTimeup == true && m_fmodelsize >= 0.0f)
	{
		m_fmodelsize -= 0.02f;
	}
}

//==================================================================================
// 描画処理呼び出し
//==================================================================================
void CItem::Draw(void)
{
	if (m_bDrawScaling == false)
	{
		CObjectX::Draw();
	}
	else
	{
		CObjectX::DrawScaling(D3DXVECTOR3(m_fmodelsize, m_fmodelsize, m_fmodelsize));
	}
}

//==================================================================================
// 当たり判定(球)
//==================================================================================
bool CItem::BallCollision(D3DXVECTOR3* pos, float fRadius)
{
	D3DXVECTOR3 DPos = m_pos;		// 自身の位置
	D3DXVECTOR3 vecToPos = DPos - *pos;

	if (D3DXVec3Length(&vecToPos) <= (double)m_fRadius + (double)fRadius)
	{// 球判定・真
		return true;
	}

	return false;
}