//==================================================================================
//
//  3Dオブジェクト [object.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CObject3D :public CObject
{
public:
	CObject3D();
	~CObject3D();

	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const LPDIRECT3DTEXTURE9 pTexture, const float fTexSplit);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture,float fTexSplit);
	HRESULT InitWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetColor(const D3DXCOLOR col) { m_col = col; }
	void LoadTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXCOLOR GetColor(void) { return m_col; }
	float GetHeight(D3DXVECTOR3 pos);

private:
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;				// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 角度
	D3DXVECTOR3 m_size;							// 大きさ
	D3DXCOLOR m_col;							// 色

	float m_fLength;							// 対角線長さ
	float m_fAngle;								// 対角線角度
};

#endif // !_OBJECT3D_H_
