//==================================================================================
//
// ソケット通信[サーバー側] (main.cpp)
// AUTHOR : NAGATE
//
//==================================================================================
//**********************************************************************************
//インクルードファイル
//**********************************************************************************
#include "main.h"
#include<stdio.h>
#include "tcp_listener.h"
#include "tcp_client.h"
#include <thread>
#include <mutex>

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_CDATA	64			// char型サイズ


//**********************************************************************************
// 
//**********************************************************************************

//==================================================================================
// スレッド内処理
//==================================================================================
void ThreadProc(CTcpClient *pTcpClient, CTcpListener *pTcpListener)
{
		//----------------//
		// データの受信
		//----------------//
		printf("入力の受信を待機します...\n");
		char aRecvData[MAX_CDATA] = {};
		int nRecvByte = pTcpClient->Recv(&aRecvData[0], sizeof(aRecvData));		// 受信したデータのサイズ

		// 受信したデータが0byte以上か
		if (nRecvByte > 0)
		{
			int nData = 0;
			memcpy(&nData, &aRecvData[0], sizeof(nData));			// nDataにバイトコピー
			printf("受信した値 : %d\n", nData);
			// recv関数 : データを受信する	/  recv(ソケットディスクリプタ, 受信データ格納先, 格納先バイト数, フラグ)
			// recvを実行すると、データを受信するまで一時停止する
			// ※受信バイト数が０の場合、接続が切断されたことを示す

			//----------------//
			// データの送信
			//----------------//
			char cSendData[30];		// データ送信用

			/*pTcpListener->ReadMessage(nData, cSendData);*/

			// 送信用のバイト配列に数値をコピー
			memcpy(cSendData, &nData, sizeof(nData));

			//printf("文字列を返します\n");

			// 接続先に送信
			pTcpClient->Send(&cSendData[0], sizeof(cSendData));


			printf("送信完了\n");

			printf("\nランキングデータの書き込みを行います\n");
			pTcpListener->WriteRankingData(nData);
		}
		else
		{
			printf("!!! 受信に失敗 !!!\n");
			getchar();
			rewind(stdin);
		}

		printf("接続を切断し、再度受信を待機します\n\n");

		// 接続を切断
		pTcpClient->Uninit();
		delete pTcpClient;
		pTcpClient = NULL;
}

//==================================================================================
// メイン
//==================================================================================
int main(void)
{
	//-------------------------------//
	// Winsockの初期化関数を実行する
	//-------------------------------//
	printf("[Winsockの初期化を実行]\n");
	WSADATA wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{// 初期化に失敗した場合
		printf("[[[[[[[[初期化に失敗]]]]]]]]\n");
		getchar();
		rewind(stdin);

		return 0;
	}

	//!!! ポート番号直入れ !!!//
	int nPort = 22333;
	/*printf("ポート番号を設定 : ");
	scanf("%d", &nPort);*/

	
		// サーバークラス生成
		CTcpListener *pTcpListener = new CTcpListener;
		if (pTcpListener != NULL && pTcpListener->Init(nPort) == TRUE)
		{
			while (1)
			{
				// クライアントと接続
				CTcpClient *pTcpClient = pTcpListener->Accept();

				if (pTcpClient != nullptr)
				{// クライアント接続成功
					// スレッドの生成
					std::thread th(ThreadProc, pTcpClient, pTcpListener);

					// スレッドを切り離す
					th.detach();
				}
				else
				{
					printf("!!! クライアントの取得に失敗 !!!\n");
				}
			}
		}

	//-----------------//
	// Winsock終了処理
	//-----------------//
	WSACleanup();		// Winsockの終了処理

	getchar();
	rewind(stdin);

	return 0;

}