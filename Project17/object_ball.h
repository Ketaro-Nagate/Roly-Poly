//==================================================================================
//
//  球判定をもつオブジェクト [item.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _COBJECTBALL_H_
#define _COBJECTBALL_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "objectX.h"
#include "particle.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CBallObject :public CObjectX
{
public:
	CBallObject();
	~CBallObject();

	static CBallObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, const char* pFileName);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float mRadius, const char* pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool BallCollision(D3DXVECTOR3* pos, float fRadius);

	D3DXVECTOR3* GetPos(void) { return &m_pos; }
	D3DXVECTOR3* GetRot(void) { return &m_rot; }

private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_move;		// 移動量
	float m_fRadius;		// 半径
};

#endif // !_ITEM_H_
