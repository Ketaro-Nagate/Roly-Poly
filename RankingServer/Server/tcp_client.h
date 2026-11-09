//==================================================================================
//
// ソケット通信プログラム[クライアント側] (main.h)
// AUTHOR : NAGATE
//
//==================================================================================
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//**********************************************************************************
//インクルードファイル
//**********************************************************************************
#include "main.h"

//**********************************************************************************
// クライアントクラス定義
//**********************************************************************************
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	bool Init(const char* pIPAddress, int nPortNum);
	bool Init(SOCKET sock);
	int Send(char* pSendData, int nSendDataSize);
	int Recv(char* pRecvData, int nRecvDataSize);
	void Uninit(void);
	SOCKET GetSocket(void) { return m_Socket; }

private:
	SOCKET m_Socket;		// 通信用ソケット
};
#endif // !_MAIN_H_