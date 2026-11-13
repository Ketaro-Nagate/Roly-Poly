//==================================================================================
//
//   ランキング[ranking.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _RANKING_H_
#define _RAMKING_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include<vector>
#include "scene.h"
#include "object2D.h"

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CTcpClient;
class CScore;

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CRanking : public CScene
{
public:
	CRanking();
	~CRanking();

	HRESULT Init(CScene::MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void WriteRankingData(int nRankingData);

private:
	CTcpClient* m_pClient;				// ランキングサーバーのクライアント
	CObject2D* m_pBg;					// 背景テクスチャ
	std::vector<CScore*>m_vpScore;		// 表示スコア
	std::vector<CObject2D*>m_vpRank;	// スコア横順位表示
	
};

#endif // !_RANKING_H_