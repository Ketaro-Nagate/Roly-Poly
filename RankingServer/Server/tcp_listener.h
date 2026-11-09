//==================================================================================
// 
// サーバークラス [tcp_listener.h]
// Author : Nagate
// 
//==================================================================================
#ifndef _TCP_LISTENER_H_
#define _TCP_LISTENER_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "tcp_client.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CTcpListener
{
public:
	CTcpListener();
	~CTcpListener();

	bool Init(int nPortNum);
	CTcpClient* Accept(void);
	void Uninit(void);

	char* ReadMessage(char* pcRecv, char* pcSend);
	void WriteRankingData(int pcRankingDdata);

private:
	SOCKET m_sockServer;

};


#endif // !_TCP_LISTENER_H_
