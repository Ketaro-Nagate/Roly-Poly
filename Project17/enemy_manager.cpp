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
#include <string>
#include "util.h"
#include "enemy_bird.h"

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
		//m_vpEnemy.push_back(CEnemyBird::Create());
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

//==================================================================================
// 敵の設置情報テキスト読み込み
//==================================================================================
void CEnemyManager::LoadFile(void)
{
#if false
	std::string line;		// 行データ
	std::string label;		// ラベル
	char equal;				// = テスト

	ENEMY_INFO EnemyInfo;		// 一時保存用
	D3DXVECTOR3 pos;

	//====== ファイルデータを読み込み ======//
	std::ifstream readfile("data\\TEXT\\ItemManager.txt");

	if (readfile.is_open())
	{
		std::cout << "ファイルは開けています\n";
	}

	// ファイルから読み込み
	if (readfile.is_open())
	{
		if (readfile.eof())
		{
			// すでにファイル終端の状態
			int n = 0;
		}

		// １行ずつ読み込む
		while (std::getline(readfile, line))
		{
			std::istringstream iss(line);

			// 空行などを除外
			if (!(iss >> label))
			{
				continue;
			}

			// アイテムの読み込み開始
			if (label == "SET_PATTERN")
			{// 次の読み込みに備える
				EnemyInfo.vpos.clear();
				EnemyInfo.nNumItem = 0;
				EnemyInfo.nIdx = -1;
				continue;
			}

			// アイテムの生成
			if (label == "END_PATTERNSET")
			{
				// ここでセットパターンを登録する
				m_vPattern.push_back(PatternInfo);
				m_nNumPattern++;
				continue;
			}

			// 項目名と = を読み込む
			if (!(iss >> equal) || equal != '=')
			{
				continue;
			}

			if (label == "POS")
			{// 位置を読み込み
				iss >> pos.x >> pos.y >> pos.z;
				PatternInfo.vpos.push_back(pos);		// 値を保存
				PatternInfo.nNumItem++;					// カウント増
			}
			else if (label == "ID")
			{// インデックスを読み込み
				iss >> PatternInfo.nIdx;
			}
		}
	}

	// 読み込みファイルを閉じる
	readfile.close();
#endif
}