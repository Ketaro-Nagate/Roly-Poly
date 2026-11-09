//==================================================================================
//
//  箱判定をもつオブジェクト [object_cube.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _COBJECTCUBE_H_
#define _COBJECTCUBE_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "objectX.h"
#include "particle.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CCubeObject :public CObjectX
{
public:
	CCubeObject();
	~CCubeObject();

	static CCubeObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR2 fSize, const char* pFileName);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 fSize, const char* pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CubeCollision(D3DXVECTOR3* pos, D3DXVECTOR3 posold, float fRange, float fRadius);

	D3DXVECTOR3* GetPos(void) { return &m_pos; }
	D3DXVECTOR3* GetRot(void) { return &m_rot; }

private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR2 m_size;		// 横縦の幅
};

#endif // !_COBJECTCUBE_H_
