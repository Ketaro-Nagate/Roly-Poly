//==================================================================================
//
//  スコア管理処理 [score.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"
#include "number.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_NUMBER		16			// 生成できる数値の桁の最大数

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CScore :public CObject
{
public:
	CScore();
	~CScore();

	static CScore* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int nDigit);
	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int nDigit);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddScore(int nAdd);
	void SetDraw(bool bDraw) { m_bUnDraw = bDraw; }
	void SetColor(D3DXCOLOR col);
	int GetScore(void) { return m_nScore; }

private:
	CNumber* m_aNumber[MAX_NUMBER];			// ナンバーへのポインタ
	int m_nDigit;							// 桁数
	int m_nScore;							// スコア
	float m_fWidth;							// 幅
	float m_fHeight;						// 高さ
	bool m_bUnDraw;							// 描画切り
};

#endif // !_SCORE_H_
