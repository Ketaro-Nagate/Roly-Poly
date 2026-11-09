//==================================================================================
//
//  パーティクル [orbit.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "particle.h"
#include "effect.h"
#include <random>

//==================================================================================
// コンストラクタ
//==================================================================================
CParticle::CParticle()
{
	m_cCorrection = WHITE;
	m_col = WHITE;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_nEffectlife = 1.0f;
	m_nTexIdx = 0;
	m_ppos = nullptr;
}

//==================================================================================
// デストラクタ
//==================================================================================
CParticle::~CParticle()
{
}


//==================================================================================
// 生成処理
//==================================================================================
CParticle* CParticle::Create(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress)
{
	// インスタンス生成
	CParticle* pParticle = new CParticle;

	// 初期化
	pParticle->Init(pos, fWidth, fHeight, col, colcorrection, nLife, texadress);

	// ポインタを返す
	return pParticle;
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CParticle::Init()
{
	return S_OK;
}


//==================================================================================
// 初期化( Overlord )
//==================================================================================
HRESULT CParticle::Init(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress)
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
void CParticle::Uninit()
{
	// 全パーティクルを終了・削除
	for (auto pEffect : m_vpEffect)
	{
		if (pEffect != nullptr)
		{
			// パーティクルの終了処理
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
void CParticle::Update()
{
	// 更新毎にパーティクルを呼び出す
	// 1. 非決定論的な乱数生成器でシード（種）を取得
	std::random_device seed_gen;
	// 2. メルセンヌ・ツイスタ法による疑似乱数エンジンを初期化
	std::mt19937 engine(seed_gen());
	// 3. 1から6までの「一様な整数」を生成する分布を定義
	std::uniform_int_distribution<int> dist(-3, 3);

	D3DXVECTOR3 Rand;
	Rand.x = dist(engine);
	Rand.y = dist(engine);
	Rand.z = dist(engine);

	if (Rand.y < 0)
	{
		Rand.y *= -1;
	}

	CEffect* pEffect = CEffect::Create(*m_ppos, D3DXVECTOR3(Rand.x, Rand.y, Rand.z), m_fWidth, m_fHeight, m_col, m_nEffectlife, m_nTexIdx);
	m_vpEffect.emplace_back(pEffect);
	pEffect = CEffect::Create(*m_ppos, D3DXVECTOR3(Rand.z, Rand.y, Rand.x), m_fWidth, m_fHeight, m_col, m_nEffectlife, m_nTexIdx);
	m_vpEffect.emplace_back(pEffect);

	// 全てのパーティクルの更新を行う
	for (auto it = m_vpEffect.begin(); it != m_vpEffect.end(); )
	{
		pEffect = *it;

		// 色の補正
		D3DXCOLOR* pCol = pEffect->GetColor();			// 色変数のポインタ取得
		*pCol -= m_cCorrection / (float)m_nEffectlife;	// 補正量 / 寿命のフレーム数分色調補正

		// サイズ更新
		/*D3DXVECTOR2 size = pEffect->GetSize();			// サイズ取得
		size.x -= m_fWidth / (float)m_nEffectlife;		// 拡縮前幅 / 寿命のフレーム数分拡縮
		size.y -= m_fHeight / (float)m_nEffectlife;		// 拡縮前高

		// サイズが０以下の場合、超えないようにする
		if (size.x < 0.0f)
			size.x = 0.0f;

		if (size.y < 0.0f)
			size.y = 0.0f;

		pEffect->SetSize(size.x, size.y);*/

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
void CParticle::Draw()
{
	
}