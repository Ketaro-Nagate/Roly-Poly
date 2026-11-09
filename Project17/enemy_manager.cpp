//========================================================================
// 
// エネミーの出現管理クラス [enemy_manager.cpp]
// Author : Nagate
// 
//========================================================================

//************************************************************************
// インクルードファイル
//************************************************************************
#include "enemy_manager.h"
#include "manager.h"
#include "input.h"
#include <assert.h>
#include <algorithm>
#include "util.h"

//========================================================================
// コンストラクタ
//========================================================================
CEnemyManager::CEnemyManager()
{
	m_nNumEnemy = 0;
	m_nMaxEnemy = 0;
	m_nCountDead = 0;
	m_nFrequency = 0;
	m_nCountCreate = 0;
}

//========================================================================
// デストラクタ
//========================================================================
CEnemyManager::~CEnemyManager()
{
}

//========================================================================
// エネミーマネージャーの生成
//========================================================================
CEnemyManager* CEnemyManager::Create(int nFrequency, int nMaxEnemy)
{
	// メモリ生成
	CEnemyManager* pEnemyManager = new CEnemyManager;

	// 初期化
	pEnemyManager->Init(nFrequency, nMaxEnemy);

	return pEnemyManager;
}

//========================================================================
// 初期化
//========================================================================
void CEnemyManager::Init(int nFrequency, int nMaxEnemy)
{
	m_nFrequency = nFrequency;		// 生成頻度
	m_nMaxEnemy = nMaxEnemy;		// 最大数
}

//========================================================================
// 終了
//========================================================================
void CEnemyManager::Uninit(void)
{
	// 管理する敵を全消去
	for (auto iter = m_vpEnemy.begin();
		iter != m_vpEnemy.end();)
	{
		if ((*iter) != nullptr)
		{// イテレータ内のポインタがnullではないか
			// 敵の終了処理
			(*iter)->Uninit();
		}

		// 自身のvectorからの消去
		//iter = m_vpEnemy.erase(iter);
	}

	// vectorを全消去
	m_vpEnemy.clear();

	// カウンターを０にする
	m_nNumEnemy = 0;
}

//========================================================================
// 更新
//========================================================================
void CEnemyManager::Update(void)
{
	CManager *pManager = CManager::GetManager();
	CInputKeyboard *pKey = pManager->GetInputKeyboard();

	// 生成カウンターの更新
	m_nCountCreate++;

	// 生成タイミングを待機
	if (m_nFrequency == m_nCountCreate)
	{// 生成


	}
}

//==================================================================================
// 死んだエネミーが自らを消去
//==================================================================================
void CEnemyManager::Remove(CEnemy* DeadEnemy)
{
	// vectorから自身を検索
	auto iter = std::find(m_vpEnemy.begin(), m_vpEnemy.end(), DeadEnemy);

	// 見つかったら消去
	if (iter != m_vpEnemy.end())		// end()はコンテナの終端位置を示す！
	{
		m_nNumEnemy--;					// 総数を減らす
		(*iter)->Uninit();				// ()がないと、iter->Uninit()の返り値のポインタになる
		m_vpEnemy.erase(iter);			// イテレータから自身を消去
		return;
	}
}

//==================================================================================
// 敵の生成量を調整
//==================================================================================
void CEnemyManager::SetLevel(int nFrequency, int nMaxEnemy)
{
	m_nFrequency = nFrequency;		// 生成頻度
	m_nMaxEnemy = nMaxEnemy;		// 最大数
}