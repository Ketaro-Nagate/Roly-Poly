//==================================================================================
//
//  壁 [wall.h]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object3D.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CWall : public CObject3D
{
public:
	CWall();
	~CWall();

	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 texture);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 m_rot, LPDIRECT3DTEXTURE9 texture);
	void uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3* pos, D3DXVECTOR3 posold, float fWidth);
	bool CollisionCross(D3DXVECTOR3 *pos, D3DXVECTOR3 posold, float fWidth);

private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 角度
	float m_fWidth;				// 幅
	float m_fHeight;			// 高さ
	D3DXVECTOR3 m_vecNor;		// 法線
};
