//==================================================================================
//
//  数字表示処理クラス [number.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _NUMBER_H
#define _NUMBER_H

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();

	static HRESULT Load(void);
	static void Unload(void);
	static CNumber* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nNumber);
	static CNumber* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	static CNumber *Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const D3DXCOLOR col);
	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nNumber);
	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
	void SetColor(D3DXCOLOR col);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// バッファへのポインタ
	int m_nNumber;									// 表示する数値
	D3DXCOLOR m_col;

};

#endif // !_NUMBER_H
