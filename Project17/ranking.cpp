//==================================================================================
//
//  ランキングシーン [ranking.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "ranking.h"
#include "tcp_client.h"
#include "score.h"
#include "manager.h"
#include "input.h"

#include <fstream>
#include <algorithm>

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define _ONLINE_RANKING	_						// ランキングのオンライン対応
#define IPADDRESS			("127.0.0.1")		// ランキング接続先のIPアドレス
#define PORTNUMBER			(22333)				// ポート番号

//==================================================================================
// コンストラクタ
//==================================================================================
CRanking::CRanking() :CScene(MODE_RANKING)
{
	m_pClient = nullptr;
	m_pBg = nullptr;
}

//==================================================================================
// デストラクタ
//==================================================================================
CRanking::~CRanking()
{
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CRanking::Init(CScene::MODE mode)
{
#ifndef _ONLINE_RANKING
	// ランキングクライアント生成
	if (m_pClient == nullptr)
	{
		//--- IPアドレスをローカルに固定 ---//
		const char* pIPAddress = IPADDRESS;

		//--- ポート番号---//
		int nPortNum = PORTNUMBER;

		m_pClient = new CTcpClient;
		m_pClient->Init(pIPAddress, nPortNum);
	}
#endif // !_ONLINE_RANKING
	 
	// 背景生成
	if (m_pBg == nullptr)
	{
		// 画面中心に設定
		m_pBg = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 1280.0f, 720.0f);

		// テクスチャ割り当て
		
	}

	// テキストファイルからスコア読み込み

	// 順位表示の生成
	m_vpRank.push_back(CObject2D::Create(D3DXVECTOR3(320.0f,100.0f, 0.0f),50.0f, 50.0f));

	// 表示スコアの生成
	m_vpScore.push_back(CScore::Create(D3DXVECTOR3(320.0f, 200.0f, 0.0f), 200.0f, 50.0f, 5));

	return S_OK;
}

//==================================================================================
// 終了
//==================================================================================
void CRanking::Uninit(void)
{
	// 順位表示の削除
	for (auto*& rank : m_vpRank)
	{
		if (rank != nullptr)
		{
			rank->Uninit();
		}
	}
	m_vpRank.clear();

	// スコア表示の削除
	for (auto*& score : m_vpScore)
	{
		if (score != nullptr)
		{
			score->Uninit();
		}
	}
	m_vpScore.clear();

#ifndef _ONLINE_RANKING
	// クライアント
	if (m_pClient != nullptr)
	{
		// サーバークライアントの終了処理
		m_pClient->Uninit();

		// インスタンス破棄
		delete m_pClient;
		m_pClient = nullptr;
		
	}
#endif // !_ONLINE_RANKING
}

//==================================================================================
// 更新
//==================================================================================
void CRanking::Update(void)
{
	CInputKeyboard* pKey = CManager::GetManager()->GetInputKeyboard();	// キーボード取得

	if (pKey->GetTrigger(DIK_RETURN))
	{
		CManager::GetManager()->SetMode(CScene::MODE_TITLE);
	}
}

//==================================================================================
// 描画
//==================================================================================
void CRanking::Draw(void)
{

}

//==================================================================================
// ランキングデータの書き込み
//==================================================================================
void CRanking::WriteRankingData(int nRankingData)
{
	std::vector<int>vFiledata;

	//====== ファイルデータを読み込み ======//
	std::ifstream readfile("data\\ranking.txt");

	int nData;		// 読み込んだ値を受け取る

	// ファイルから読み込み
	if (readfile.is_open())
	{
		while (readfile >> nData)
		{
			// 値を保存
			vFiledata.push_back(nData);
		}
	}

	// 最新の値もソートに組み込む
	vFiledata.push_back(nRankingData);

	// 読み込みファイルを閉じる
	readfile.close();

	//====== ランキング更新処理 ======//
	std::sort(vFiledata.begin(), vFiledata.end(), std::greater<int>());

	//====== データを書き出す ======//
	std::ofstream writefile("data\\TEXT\\Ranking.txt");		// 書き出し用ファイル

	// 最小の値を削除(resize()を覚えよう！)
	if (vFiledata.size() > 5)
	{
		vFiledata.resize(5);
	}

	// ファイルに出力
	if (writefile.is_open())
	{
		for (auto score : vFiledata)
		{
			// 数値 + 改行
			writefile << score << std::endl;
		}
	}

	// ファイルクローズ
	writefile.close();

}