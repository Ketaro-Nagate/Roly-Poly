//==================================================================================
//
//  モデルパーティクル [particle_model.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "particle_model.h"
#include "effect_model.h"
#include "effect.h"
#include "util.h"
#include <random>
#include "manager.h"
#include "input.h"

//==================================================================================
// コンストラクタ
//==================================================================================
CModelParticle::CModelParticle()
{
	m_nEffectlife = 1.0f;
	m_ppos = nullptr;
	m_bCreate = true;
	m_fGravity = 0.0f;
	m_Spread = 0.0f;
	m_nModelIdx = 0;
}

//==================================================================================
// デストラクタ
//==================================================================================
CModelParticle::~CModelParticle()
{
}


//==================================================================================
// 生成処理
//==================================================================================
CModelParticle* CModelParticle::Create(D3DXVECTOR3* pos, float Spread, int nLife)
{
	// インスタンス生成
	CModelParticle* pModelParticle = new CModelParticle;

	// 初期化
	pModelParticle->Init(pos, Spread, nLife);

	// ポインタを返す
	return pModelParticle;
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CModelParticle::Init()
{
	return S_OK;
}


//==================================================================================
// 初期化( Overlord )
//==================================================================================
HRESULT CModelParticle::Init(D3DXVECTOR3* pos, float Spread, int nLife)
{
	// パーティクルの持ち主の位置を持つ
	m_ppos = pos;

	// 拡散量保存
	m_Spread = Spread;

	// 寿命
	m_nEffectlife = nLife * 60;

	return S_OK;
}

//==================================================================================
// 終了
//==================================================================================
void CModelParticle::Uninit()
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
void CModelParticle::Update()
{
	// 更新毎にパーティクルを呼び出す
	// シードを取得
	std::random_device seed_gen;
	// 疑似乱数エンジンを初期化
	std::mt19937 engine(seed_gen());
	// 生成する分布を定義
	std::uniform_int_distribution<int> dist(-m_Spread, m_Spread);

	D3DXVECTOR3 RandomMove = Random3D(-3, 3);
	if (RandomMove.y < 0)
	{
		RandomMove.y *= -1;
	}

	RandomMove.y += 3.0f;

	//============= モデルの選択 ==============//
	CManager* pManager = CManager::GetManager();
	CInputKeyboard* pKey = pManager->GetInputKeyboard();

	if (pKey->GetTrigger(DIK_UP))
	{
		if (m_nModelIdx != 1)
		{
			m_nModelIdx++;
		}
		else
		{
			m_nModelIdx = 0;
		}
	}
	else if (pKey->GetTrigger(DIK_DOWN))
	{
		if (m_nModelIdx != 0)
		{
			m_nModelIdx--;
		}
		else
		{
			m_nModelIdx = 1;
		}
	}

	char* pcFile = nullptr;

	if (m_nModelIdx == 0)
	{
		char acFileName[64] = "data\\MODEL\\spikeball000.x";
		pcFile = acFileName;
	}
	else if(m_nModelIdx == 1)
	{
		char acFileName[64] = "data\\MODEL\\head000.x";
		pcFile = acFileName;
	}

	CModelEffect* pEffect = CModelEffect::Create(*m_ppos, NULL_D3DXVECTOR3, RandomMove, RandomMove * 0.01f, m_nEffectlife, pcFile);
	m_vpEffect.emplace_back(pEffect);
	pEffect = CModelEffect::Create(*m_ppos, NULL_D3DXVECTOR3, RandomMove, RandomMove * 0.01f, m_nEffectlife, pcFile);
	m_vpEffect.emplace_back(pEffect);

	// 全てのパーティクルの更新を行う
	for (auto it = m_vpEffect.begin(); it != m_vpEffect.end(); )
	{
		pEffect = *it;

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
void CModelParticle::Draw()
{
	
}