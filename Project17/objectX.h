//==================================================
//
// オブジェクト(Xファイル) [objectX.h]
//
//==================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//==================================================
//	インクルードファイル
//==================================================
#include "object.h"
#include "main.h"

//==================================================
//	マクロ定義
//==================================================
#define MAX_XTEXTURE (8)		//テクスチャの最大数

//==================================================
//	クラス定義
//==================================================
class CObjectX:public CObject
{
public:
	CObjectX();
	~CObjectX();

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawScaling(D3DXVECTOR3 size);
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const char* pFileName);
	/*static CObjectX* */

	D3DXVECTOR3* GetPosition(void) { return &m_pos; }
	D3DXVECTOR3* GetRotation(void) { return &m_rot; }

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetScale(D3DXVECTOR3 scale) { m_Scaling = scale; }
	void SwitchScaling(bool bScaling) { m_bScaling = bScaling; }

private:
	LPD3DXMESH m_pMesh;					// メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat;			// マテリアルへのポインタ
	DWORD m_dwNumMat;					// マテリアルの数
	LPDIRECT3DTEXTURE9 m_aTexture[MAX_XTEXTURE];	// テクスチャへのポインタ

	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス

	D3DXVECTOR3 m_Scaling;				// モデルのサイズ倍率
	bool m_bScaling;					// モデルのサイズ変更を行うか
};

#endif // !_OBJECTX_H_

