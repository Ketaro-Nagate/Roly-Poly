//==================================================================================
//
// ビルボード[billboard.h]
//
//==================================================================================
#ifndef _BILLBOARD__H_
#define _BILLBOARD__H_

//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CBillboard:public CObject
{
public:
	CBillboard();
	CBillboard(int nPrioryty);
	~CBillboard();

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexIdx);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBillboard* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexIdx, int nPrioryty = 3);

	D3DXVECTOR3* GetPosition(void) { return &m_pos; }
	D3DXCOLOR* GetColor(void) { return &m_col; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_fWidth, m_fHeight); }
	void SetColor(D3DXCOLOR col);
	void SetSize(float fWidth, float fHeight);

private:
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;				// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXCOLOR m_col;							// ポリゴンの色
	D3DXVECTOR3 m_pos;							// ポリゴンの位置
	float m_fWidth;								// ポリゴンの幅
	float m_fHeight;							// ポリゴンの高さ
	bool m_bZtest;								// Zテストの有効/無効
	bool m_batest;								// アルファテストの有効/無効
};

#endif // !1
