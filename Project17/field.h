//==================================================================================
//
//  地面 [field.h]
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
class CField : public CObject3D
{
public:
	CField();
	~CField();

	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 texture, const float fTexSplit);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 texture, const float fTexSplit);
	void uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 posold, float fWidth);

private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 角度
	D3DXVECTOR3 m_size;			// 大きさ

};
