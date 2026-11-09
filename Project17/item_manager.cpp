//========================================================================
// 
// アイテムの出現管理クラス [item_manager.cpp]
// Author : Nagate
// 
//========================================================================

//************************************************************************
// インクルードファイル
//************************************************************************
#include "item_manager.h"
#include "manager.h"
#include "input.h"
#include <assert.h>
#include <algorithm>
#include "util.h"

#include<string>
#include<fstream>
#include<functional>
#include<sstream>
#include <random>
#include <iostream>  // std::cout

//************************************************************************
// マクロ定義
//************************************************************************
#define AWAVE_LIMIT		(600)			// １ウェーブごとの時間制限
#define CREATE_TIME		(120)			// アイテム生成の待機時間

//========================================================================
// コンストラクタ
//========================================================================
CItemManager::CItemManager()
{
	m_nTimeLimit = 0;
	m_nCountCreate = 0;
	m_nNumPattern = 0;
}

//========================================================================
// デストラクタ
//========================================================================
CItemManager::~CItemManager()
{
}

//========================================================================
// 初期化
//========================================================================
void CItemManager::Init(void)
{
	// アイテム設置情報読み込み
	LoadFile();
}

//========================================================================
// 終了
//========================================================================
void CItemManager::Uninit(void)
{  
	// 管理するアイテムを全消去
	//for (auto iter = m_vpItem.begin();
	//	iter != m_vpItem.end();)
	//{
	//	if ((*iter) != nullptr)
	//	{// イテレータ内のポインタがnullではないか
	//		// アイテムの終了処理
	//		(*iter)->Uninit();
	//	}

	//	iter++;

	//	// 自身のvectorからの消去
	//	//iter = m_vpItem.erase(iter);
	//}

	// vectorを全消去
	m_vpItem.clear();

	// カウンターを０にする
	m_nNumItem = 0;
}

//========================================================================
// 更新
//========================================================================
void CItemManager::Update(void)
{
	CManager *pManager = CManager::GetManager();
	CInputKeyboard *pKey = pManager->GetInputKeyboard();

	// 生成カウンターの更新
	if (m_vpItem.empty())		// アイテムが生成されているか
	{// 未生成
		m_nTimeLimit = 0;
		m_nCountCreate++;

		// 生成タイミングを待機
		if (m_nCountCreate >= CREATE_TIME)
		{// 待機時間終了
			//--- 配置パターン生成 ---//
			CallPattern(2);
			m_nCountCreate = 0;		// カウントリセット
		}
	}
	else
	{// 生成済
		// 時間制限で消去準備
		if (m_nTimeLimit >= AWAVE_LIMIT)
		{
			for (auto pItem : m_vpItem)
			{
				pItem->SetScalingMode(true);		// サイズの縮小開始
				pItem->SetTimeUp(true);				// 時間切れ告知
			}
		}
		else
		{
			// 時間制限のカウントアップ
			m_nTimeLimit++;
		}

		// 時間制限到達後処理
		if (m_nTimeLimit == AWAVE_LIMIT)
		{
			// アイテムのモデルサイズを確認し、縮小終了なら消去
			for (auto iter = m_vpItem.begin(); iter != m_vpItem.end(); )
			{
				CItem* pItem = *iter;

				if (pItem->GetSize() <= 0.0f)
				{// 消去
					m_nNumItem--;
					pItem->Uninit();

					iter = m_vpItem.erase(iter);
				}
				else
				{// イテレータを進める
					++iter;
				}
			}

		}
	}
}

//==================================================================================
// 死んだアイテムが自らを消去
//==================================================================================
void CItemManager::Remove(CItem* DeadItem)
{
	// vectorから自身を検索
	auto iter = std::find(m_vpItem.begin(), m_vpItem.end(), DeadItem);

	// 見つかったら消去
	if (iter != m_vpItem.end())		// end()はコンテナの終端位置を示す！
	{
		m_nNumItem--;					// 総数を減らす
		(*iter)->Uninit();				// ()がないと、iter->Uninit()の返り値のポインタになる
		m_vpItem.erase(iter);			// イテレータから自身を消去
		return;
	}
}

//==================================================================================
// アイテムの生成
//==================================================================================
void CItemManager::CallPattern(int nNum)
{
	//----- ランダム生成準備 -----//
	// シードを取得
	std::random_device seed_gen;
	// 疑似乱数エンジンを初期化
	std::mt19937 engine(seed_gen());
	// ランダム範囲を指定
	std::uniform_int_distribution<int> dist(0, m_nNumPattern-1);

	// 生成パターン数が最大値を超えていないか確認
	if (nNum > m_nNumPattern)
	{// 超えていたら最大値に矯正
		nNum = m_nNumPattern;
	}

	//----- 指定分パターン生成 -----//
	vector<int> vnUsed;		// すでに生成したパターンのインデックス

	for (int nPattern = 0; nPattern < nNum; nPattern++)
	{
		// ランダムにパターンを指定
		int nRand = -1;
		bool bloop = true;

		// 値抽選
		nRand = dist(engine);

		// 生成済でないか確認
		for (auto it = vnUsed.begin(); it != vnUsed.end();)
		{
			if (nRand != *it)
			{
				++it;
			}
		}


		// 指定パターンのアイテムを生成
		/*for (int nCnt = 0; nCnt < m_vPattern[nRand].nNumItem; nCnt++)
		{
			m_vpItem.push_back(CItem::Create(m_vPattern[nRand].vpos[nCnt], NULL_D3DXVECTOR3, "data\\MODEL\\food.x"));
		}*/

		// 指定パターンのアイテムを生成
		for (int nCnt = 0; nCnt < m_vPattern[nRand].nNumItem; nCnt++)
		{
			m_vpItem.push_back(CItem::Create(m_vPattern[nRand].vpos[nCnt], NULL_D3DXVECTOR3, "data\\MODEL\\food.x"));
		}

	}
}

//==================================================================================
// アイテムの設置情報テキスト読み込み
//==================================================================================
void CItemManager::LoadFile(void)
{
	std::string line;		// 行データ
	std::string label;		// ラベル
	char equal;				// = テスト

	PATTERN_INFO PatternInfo;		// 一時保存用
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
				PatternInfo.vpos.clear();
				PatternInfo.nNumItem = 0;
				PatternInfo.nIdx = -1;
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
}