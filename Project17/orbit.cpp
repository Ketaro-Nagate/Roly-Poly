//==================================================================================
//
//  オービット [orbit.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "orbit.h"
#include "effect.h"

//==================================================================================
// コンストラクタ
//==================================================================================
COrbit::COrbit()
{
	m_cCorrection = WHITE;
	m_col = WHITE;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_nEffectlife = 1.0f;
	m_nTexIdx = 0;
	m_ppos = nullptr;
	m_bCreate = true;
}

//==================================================================================
// デストラクタ
//==================================================================================
COrbit::~COrbit()
{
}


//==================================================================================
// 生成処理
//==================================================================================
COrbit* COrbit::Create(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress)
{
	// インスタンス生成
	COrbit* pOrbit = new COrbit;

	// 初期化
	pOrbit->Init(pos, fWidth, fHeight, col, colcorrection, nLife, texadress);

	// ポインタを返す
	return pOrbit;
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT COrbit::Init()
{
	return S_OK;
}


//==================================================================================
// 初期化( Overlord )
//==================================================================================
HRESULT COrbit::Init(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress)
{
	// オービットの持ち主の位置を持つ
	m_ppos = pos;

	// サイズ保存
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// 色、色の補正量を保存
	m_col = col;
	m_cCorrection = colcorrection;

	// 寿命
	m_nEffectlife = nLife * 60;

	// テクスチャインデックス
	m_nTexIdx = texadress;

	return S_OK;
}

//==================================================================================
// 終了
//==================================================================================
void COrbit::Uninit()
{
	// 全エフェクトを終了・削除
	for (auto pEffect : m_vpEffect)
	{
		if (pEffect != nullptr)
		{
			// エフェクトの終了処理
			pEffect->Uninit();
		}
	}

	m_vpEffect.clear();  // vector内の全要素を削除

	// 自身の消去
	CObject::Release();
}

//==================================================================================
// 更新
//==================================================================================
void COrbit::Update()
{
	// 更新毎にエフェクトを呼び出す
	CEffect* pEffect = CEffect::Create(*m_ppos, NULL_D3DXVECTOR3, m_fWidth, m_fHeight, m_col, m_nEffectlife, m_nTexIdx);
	m_vpEffect.emplace_back(pEffect);

	// 全てのエフェクトの更新を行う
	for (auto it = m_vpEffect.begin(); it != m_vpEffect.end(); )
	{
		pEffect = *it;

		// 色の補正
		D3DXCOLOR* pCol = pEffect->GetColor();			// 色変数のポインタ取得
		*pCol -= m_cCorrection / (float)m_nEffectlife;	// 補正量 / 寿命のフレーム数分色調補正

		// サイズ更新
		D3DXVECTOR2 size = pEffect->GetSize();			// サイズ取得
		size.x -= m_fWidth / (float)m_nEffectlife;		// 拡縮前幅 / 寿命のフレーム数分拡縮
		size.y -= m_fHeight / (float)m_nEffectlife;		// 拡縮前高

		// サイズが０以下の場合、超えないようにする
		if (size.x < 0.0f)
			size.x = 0.0f;

		if (size.y < 0.0f)
			size.y = 0.0f;

		pEffect->SetSize(size.x, size.y);

		// 死亡フラグが立っていれば、消去
		if (pEffect->GetDead())
		{
			pEffect->Uninit();
			it = m_vpEffect.erase(it);
		}
		else
		{// 死亡していない場合イテレータを進める
			++it;
		}
	}
}

//==================================================================================
// 描画
//==================================================================================
void COrbit::Draw()
{
	
}