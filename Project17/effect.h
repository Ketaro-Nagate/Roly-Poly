//==================================================================================
//
//  エフェクト [effect.h]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "billboard.h"
#include "texture.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CEffect :public CBillboard
{
public:
	CEffect();
	~CEffect();

	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, D3DXCOLOR col, int nLife, int texadress = 0);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, D3DXCOLOR col, int nLife, int texadress = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetDead(void) { return m_bDead; }

private:
	D3DXVECTOR3 m_move;			// 移動量
	int m_nLife;				// 寿命
	bool m_bDead;				// 死亡フラグ

};