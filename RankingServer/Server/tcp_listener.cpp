//==================================================================================
// 
// サーバークラス [tcp_listener.cpp]
// Author : Nagate
// 
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "tcp_listener.h"
#include<stdio.h>
#include<map>
#include<string>
#include<iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <functional>

#include<mutex>

//**********************************************************************************
// グローバル変数宣言
//**********************************************************************************
std::mutex g_filemutex;			// ファイル出力の排他制御用

//==================================================================================
// コンストラクタ
//==================================================================================
CTcpListener::CTcpListener()
{
	m_sockServer = INVALID_SOCKET;		// 初期化
}

//==================================================================================
// デストラクタ
//==================================================================================
CTcpListener::~CTcpListener()
{

}

//==================================================================================
// 初期化
//==================================================================================
bool CTcpListener::Init(int nPortNum)
{
	//---------------//
	// ソケット作成
	//---------------//
	printf("[接続受付用のソケットを作成]\n");
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sockServer == INVALID_SOCKET)
	{
		// ソケットの作成に失敗した場合
		printf("ソケットの作成に失敗\n");
		getchar();
		rewind(stdin);

		return 0;
	}

	//----------------------------//
	// 接続を受け付けるための準備
	//----------------------------//
	printf("[接続受付の準備]\n");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNum);		// すでにPCで使っている番号だと失敗する(ポート番号)
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(m_sockServer, (struct sockaddr*)&addr, sizeof(addr));
	listen(m_sockServer, 10);				// (保留中の接続最大数)

	// bind関数 : ソケットにポートを割り当てる
	// listen関数 : 接続受付の準備をする

	return true;
}

//==================================================================================
// 接続
//==================================================================================
CTcpClient* CTcpListener::Accept(void)
{
	//------------//
	// 接続を待つ
	//------------//
	printf("[接続待機中...]\n");
	struct sockaddr_in clientAddr;
	int nLength = sizeof(clientAddr);
	SOCKET sock = accept(m_sockServer, (struct sockaddr*)&clientAddr, &nLength);
	if (sock != INVALID_SOCKET)
	{
		printf("[[[[[[[[[[[クライアントが接続しました]]]]]]]]]]]\n");

		// 71L accept関数 : 接続を受け付ける  /  クライアントが接続すると、ソケットが生成される

		// TCP送受信クラスを生成
		CTcpClient *pTcpClient = new CTcpClient;

		if (pTcpClient != NULL)
		{
			pTcpClient->Init(sock);		// 接続済みのソケットで初期化
		}

		return pTcpClient;
	}
	else
	{
		printf("接続に失敗しました;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
		return nullptr;
	}
}

//==================================================================================
// 終了
//==================================================================================
void CTcpListener::Uninit(void)
{
	//---------------//
	// 接続を切断する
	//---------------//
	// 待ち受けソケット生成済みかチェック
	if (m_sockServer == INVALID_SOCKET)
		return;

	// 接続受付用ソケットを閉じる
	closesocket(m_sockServer);
	m_sockServer = INVALID_SOCKET;
}

//==================================================================================
// 受信メッセージの確認
//==================================================================================
char* CTcpListener::ReadMessage(char* pcRecv, char* pcSend)
{
	std::map<std::string, std::string> answer;
	std::string* send;

	answer["Hello"] = "こんにちは！";
	answer["hello"] = "こんにちは！";
	answer["Pokemon"] = "大好き！";
	answer["ポケモン"] = "大好き！";
	answer["ピカチュウ"] = "”様”を付けなよ";
	answer["ピカチュウ様"] = "よろしい";
	answer["さよなら"] = "さようなら！またね！";

	if (answer.find(pcRecv) != answer.end())
	{
		*pcSend = *answer[pcRecv].c_str();
		printf("Send Massage\n");
	}
	else
	{
		std::cout << "[ ?? Hello ??      ↓ ]\nSend Massage \n" << std::endl;
		strcpy(pcSend,"?? Hello ??");
	}

	return pcSend;
}

//==================================================================================
// ランキングデータの書き込み
//==================================================================================
void CTcpListener::WriteRankingData(int pnRankingDdata)
{
	std::lock_guard<std::mutex>lock(g_filemutex);
	std::vector<int>vFiledata;

	Sleep(10000);

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
	vFiledata.push_back(pnRankingDdata);

	// 読み込みファイルを閉じる
	readfile.close();

	//====== ランキング更新処理 ======//
	std::sort(vFiledata.begin(), vFiledata.end(), std::greater<int>());

	//====== データを書き出す ======//
	std::ofstream writefile("data\\ranking.txt");		// 書き出し用ファイル

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