//==================================================================================
//
//  スコア計算処理 [score.cpp]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"
#include "input.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define SPEED		5.0f;			// 速さ
#define MAX_DIGIT	8				// 桁数の最大

//**********************************************************************************
// 静的メンバ変数宣言
//**********************************************************************************

//==================================================================================
// コンストラクタ
//==================================================================================
CScore::CScore()
{
	memset(&m_aNumber[0], 0, sizeof(m_aNumber));
	m_nDigit = NULL;
	m_nScore = NULL;
	m_fWidth = NULL;
	m_fHeight = NULL;
	m_bUnDraw = false;
}

//==================================================================================
// デストラクタ
//==================================================================================
CScore::~CScore()
{

}

//==================================================================================
// 生成処理
//==================================================================================
CScore* CScore::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int nDigit)
{
	CScore *pScore;

	pScore = new CScore;

	pScore->Init(pos, fWidth, fHeight, nDigit);

	return pScore;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CScore::Init(void)
{
	return S_OK;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CScore::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int nDigit)
{
	D3DXVECTOR3 lPos = pos;

	// 桁数をメンバ変数に保存
	m_nDigit = (nDigit <= MAX_DIGIT) ? nDigit : MAX_DIGIT;

	for (int nCount = 0; nCount < m_nDigit; nCount++)
	{
		lPos.x = pos.x + (nCount * fWidth * 1.75f);

		m_aNumber[nCount] = CNumber::Create(lPos, fWidth, fHeight);
	}

	return S_OK;
}

//==================================================================================
// 終了処理
//==================================================================================
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < m_nDigit; nCount++)
	{
		if (m_aNumber[nCount] != NULL)
		{
			m_aNumber[nCount]->Uninit();
			delete m_aNumber[nCount];
			m_aNumber[nCount] = nullptr;
		}
	}

	CObject::Release();
}

//==================================================================================
// 更新処理
//==================================================================================
void CScore::Update(void)
{
	CInputKeyboard* pKey = CManager::GetManager()->GetInputKeyboard();

	if (pKey->GetTrigger(DIK_F1))
		m_bUnDraw ^= true;

	for (int nCount = 0; nCount < m_nDigit; nCount++)
	{
		if (m_aNumber[nCount] != NULL)
		{
			m_aNumber[nCount]->Update();
		}
	}
}

//==================================================================================
// 描画処理
//==================================================================================
void CScore::Draw(void)
{
	if (m_bUnDraw == false)
	{
		for (int nCount = 0; nCount < m_nDigit; nCount++)
		{
			if (m_aNumber[nCount] != NULL)
			{
				m_aNumber[nCount]->Draw();
			}
		}
	}
}

//==================================================================================
// 描画処理
//==================================================================================
void CScore::AddScore(int nAdd)
{
	m_nScore += nAdd;

	int aTexU[MAX_DIGIT];
	
	for (int nCountDigit = 0; nCountDigit < m_nDigit; nCountDigit++)
	{// 桁数分回す
		aTexU[nCountDigit] = m_nScore % ((int)powf(10, (float)m_nDigit - nCountDigit)) / (int)powf(10, (float)m_nDigit - nCountDigit - 1);
	}


	for (int nCount = 0; nCount < m_nDigit; nCount++)
	{
		if (m_aNumber[nCount] != NULL)
		{
			m_aNumber[nCount]->SetNumber(aTexU[nCount]);
		}
	}
}

//==================================================================================
// 色変更
//==================================================================================
void CScore::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < m_nDigit; nCount++)
	{
		if (m_aNumber[nCount] != NULL)
		{
			m_aNumber[nCount]->SetColor(col);
		}
	}
}