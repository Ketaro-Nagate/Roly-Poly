//==================================================================================
//
//  2Dオブジェクト [object.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"
#include "input.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CObject2D :public CObject
{
public:
	/*CObject2D();*/
	CObject2D(int nPriority = 3);
	~CObject2D();

	static CObject2D* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetPos(const D3DXVECTOR3 pos);
	void SetColor(const D3DXCOLOR col);

	D3DXVECTOR3 GetPosition(void){ return m_pos; }
	D3DXCOLOR GetColor(void) { return m_col; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点へのポインタ
	D3DXVECTOR2 fPolygonSize;				// ポリゴンのサイズ

	D3DXVECTOR3 m_pos;						// 位置
	D3DXCOLOR m_col;						// 色
	float m_rot;							// 角度


};

#endif // !_OBJECT2D_H_

