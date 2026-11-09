//==================================================================================
//
//  モデルのパーティクル [particle_model.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _MODELPARTICLE_H_
#define _MODELPARTICLE_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"
#include <vector>

using namespace std; // std:: を省略

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CModelEffect;

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CModelParticle :public CObject
{
public:
	CModelParticle();
	~CModelParticle();

	static CModelParticle* Create(D3DXVECTOR3* pos, float Spread, int nLife);
	HRESULT Init();
	HRESULT Init(D3DXVECTOR3* pos, float Spread, int nLife);
	void Uninit();
	void Update();
	void Draw();

private:
	vector<CModelEffect*> m_vpEffect;		// パーティクルへのポインタ
	vector<char*> m_pcFileName;				// モデルファイル名
	D3DXVECTOR3* m_ppos;					// オービットの主の位置へのポインタ
	float m_Spread;							// パーティクルの拡散量
	float m_fGravity;						// パーティクルにかかる重力
	int m_nEffectlife;						// 呼び出すパーティクルの寿命
	bool m_bCreate;							// 軌跡を生成するか
	int m_nModelIdx;						// 呼び出すモデルの番号
};

#endif // !_PARTICLE_H_
