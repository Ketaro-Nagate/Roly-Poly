//==================================================================================
//
//  モデルエフェクト [effect_model.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "objectX.h"
#include "texture.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CModelEffect :public CObjectX
{
public:
	CModelEffect();
	~CModelEffect();

	static CModelEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotValue, int nLife, const char* pFileName);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotValue, int nLife, const char* pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetDead(void) { return m_bDead; }

private:
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rotvalue;		// 回転量
	int m_nLife;				// 寿命
	bool m_bDead;				// 死亡フラグ
	float m_ModelScale;			// モデルサイズ調整用

};

#endif // !_MODELEFFECT_H_