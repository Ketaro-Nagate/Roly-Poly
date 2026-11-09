//==================================================================================
//
//   [orbit.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_
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
class COrbit :public CObject
{
public:
	COrbit();
	~COrbit();

	static COrbit* Create(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress = 0);
	HRESULT Init();
	HRESULT Init(D3DXVECTOR3* pos, float fWidth, float fHeight, D3DXCOLOR col, D3DXCOLOR colcorrection, int nLife, int texadress = 0);
	void Uninit();
	void Update();
	void Draw();
	void SwitchCreate(bool bCreate) { m_bCreate = bCreate; }

private:
	vector<CEffect*> m_vpEffect;			// エフェクトへのポインタ
	D3DXVECTOR3* m_ppos;					// オービットの主の位置へのポインタ
	float m_fWidth;							// 幅
	float m_fHeight;						// 高さ
	D3DXCOLOR m_col;						// エフェクトの呼び出し色
	D3DXCOLOR m_cCorrection;				// 色補正量
	int m_nEffectlife;						// 呼び出すエフェクトの寿命
	int m_nTexIdx;							// テクスチャインデックス
	bool m_bCreate;							// 軌跡を生成するか
};

#endif	// _ORBIT_H_