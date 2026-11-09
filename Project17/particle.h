//==================================================================================
//
//   [particle.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

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
class CEffect;

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CParticle :public CObject
{
public:
	CParticle();
	~CParticle();

	static CParticle* Create(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress = 0);
	HRESULT Init();
	HRESULT Init(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress = 0);
	void Uninit();
	void Update();
	void Draw();

private:
	vector<CEffect*> m_vpEffect;			// パーティクルへのポインタ
	D3DXVECTOR3* m_ppos;					// オービットの主の位置へのポインタ
	float m_fWidth;							// 幅
	float m_fHeight;						// 高さ
	D3DXCOLOR m_col;						// パーティクルの呼び出し色
	D3DXCOLOR m_cCorrection;				// 色補正量
	int m_nEffectlife;						// 呼び出すパーティクルの寿命
	int m_nTexIdx;							// テクスチャインデックス
	bool m_bCreate;							// 軌跡を生成するか
};

#endif // !_PARTICLE_H_
