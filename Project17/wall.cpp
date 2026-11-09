//==================================================================================
//
//  壁 [wall.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "main.h"
#include "wall.h"
#include "manager.h"
#include "player.h"
#include "input.h"
#include "util.h"

//==================================================================================
// コンストラクタ
//==================================================================================
CWall::CWall()
{
	m_pos = NULL_D3DXVECTOR3;
	m_rot = NULL_D3DXVECTOR3;
	m_vecNor = NULL_D3DXVECTOR3;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
}

//==================================================================================
// デストラクタ
//==================================================================================
CWall::~CWall()
{
}

//==================================================================================
// 生成(テクスチャ指定)
//==================================================================================
CWall* CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 texture)
{
	// インスタンス生成
	CWall *pWall = new CWall;

	// 初期化
	pWall->Init(pos, rot, size, texture);

	// ポインタを返す
	return pWall;
}

//==================================================================================
// 初期化(テクスチャ指定)
//==================================================================================
HRESULT CWall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 texture)
{
	// 3Dオブジェクトの初期化処理
	if (CObject3D::InitWall(pos, rot, size, texture) == S_OK)
	{
		// 情報保存
		m_pos = pos;
		m_rot = rot;
		m_fHeight = size.y / 2.0f;
		m_fWidth = size.x / 2.0f;
		return S_OK;
	}

	return S_FALSE;
}

//==================================================================================
// 終了処理
//==================================================================================
void CWall::uninit(void)
{
	// 3Dオブジェクトの終了処理
	CObject3D::Uninit();
}

//==================================================================================
// 更新
//==================================================================================
void CWall::Update(void)
{
	CObject3D::Update();

	CManager* pManager = CManager::GetManager();
	CScene* pScene = pManager->GetScene();
	CPlayer* pPlayer = pScene->GetPlayer();
	CInputKeyboard *pKey = pManager->GetInputKeyboard();

#ifdef DEBUG
	if (pKey->GetPress(DIK_1))
	{
		m_rot.y += 0.01f;
		D3DXVECTOR3 v0, v1;

		v0.x = (m_pos.x - m_fWidth * cosf(-m_rot.y));
		v0.y = 0.0f;
		v0.z = (m_pos.z - m_fWidth * sinf(-m_rot.y));

		v1.x = (m_pos.x + m_fWidth * cosf(-m_rot.y));
		v1.y = 0.0f;
		v1.z = (m_pos.z + m_fWidth * sinf(-m_rot.y));

		D3DXVECTOR3 vecline = v1 - v0;

		m_vecNor = D3DXVECTOR3(vecline.z, 0.0f, -vecline.x);
		m_vecNor = VectorNormalize(m_vecNor);
	}

	if (pKey->GetPress(DIK_2))
	{
		m_rot.y -= 0.01f;
		D3DXVECTOR3 v0, v1;

		v0.x = (m_pos.x - m_fWidth * cosf(-m_rot.y));
		v0.y = 0.0f;
		v0.z = (m_pos.z - m_fWidth * sinf(-m_rot.y));

		v1.x = (m_pos.x + m_fWidth * cosf(-m_rot.y));
		v1.y = 0.0f;
		v1.z = (m_pos.z + m_fWidth * sinf(-m_rot.y));

		D3DXVECTOR3 vecline = v1 - v0;

		m_vecNor = D3DXVECTOR3(vecline.z, 0.0f, -vecline.x);
		m_vecNor = VectorNormalize(m_vecNor);
	}
#endif // DEBUG

	//Collision(pPlayer->GetPPos(), pPlayer->GetPosDest(), pPlayer->GetRadius());

}

//==================================================================================
// 描画
//==================================================================================
void CWall::Draw(void)
{
	CObject3D::Draw();
}

//==================================================================================
// 当たり判定
//==================================================================================
bool CWall::Collision(D3DXVECTOR3* pos, D3DXVECTOR3 posold, float fWidth)
{
	bool bInside = false;		// 内側にいるか

	//--- 壁の角度で判定方向変更 ---//
	if (m_rot.y == 0.0f || m_rot.y == D3DX_PI)
	{// 平行壁
		// 内外判定
		if (pos->x - fWidth >= m_pos.x - m_fWidth &&
			pos->x + fWidth <= m_pos.x + m_fWidth)
		{
			bInside = true;		// 内外判定が真
		}

		//- 壁の範囲内にいるとき -/
		if (bInside == true)
		{
			// 正面向きの壁のめり込み判定
			if (m_rot.y == 0.0f &&
				pos->z + fWidth >= m_pos.z &&
				posold.z + fWidth <= m_pos.z)
			{
				pos->z = posold.z;
				return true;		// 壁の接触判定が真
			}

			// 後ろ向きの壁のめり込み判定
			if (m_rot.y == D3DX_PI &&
				pos->z - fWidth <= m_pos.z &&
				posold.z - fWidth >= m_pos.z)
			{
				pos->z = posold.z;
				return true;		// 壁の接触判定が真
			}
		}
	}

	if (m_rot.y == D3DX_PI * 0.5f || m_rot.y == D3DX_PI * -0.5f)
	{// 垂直壁
		// 内外判定
		if (pos->z - fWidth >= m_pos.z - m_fWidth &&
			pos->z + fWidth <= m_pos.z + m_fWidth)
		{
			bInside = true;		// 内外判定が真
		}

		//- 壁の範囲内にいるとき -/
		if (bInside == true)
		{
			// 左向きの壁のめり込み判定
			if (m_rot.y == D3DX_PI * 0.5f &&
				pos->x + fWidth >= m_pos.x &&
				posold.x + fWidth <= m_pos.x)
			{
				pos->x = posold.x;
				return true;		// 壁の接触判定が真
			}

			// 右向きの壁のめり込み判定
			if (m_rot.y == D3DX_PI * -0.5f &&
				pos->x - fWidth <= m_pos.x &&
				posold.x - fWidth >= m_pos.x)
			{
				pos->x = posold.x;
				return true;		// 壁の接触判定が真
			}
		}
	}

	return false;
}

//==================================================================================
// 当たり判定( 外積 )
//==================================================================================
bool CWall::CollisionCross(D3DXVECTOR3 *pos, D3DXVECTOR3 posold, float fWidth)
{
	bool check = false, checkold = false;
	bool bChek = false, bcheckold = false;

	D3DXVECTOR3 VecLine, VecToPos, VecMove, VecToPosOld;
	D3DXVECTOR3 v0, v1;
	float fRate, fAll, fIntersect;

	VecMove = *pos - posold;
	
	v0.x = (m_pos.x - m_fWidth * cosf(-m_rot.y));
	v0.y = 0.0f;
	v0.z = (m_pos.z - m_fWidth * sinf(-m_rot.y));

	v1.x = (m_pos.x + m_fWidth * cosf(-m_rot.y));
	v1.y = 0.0f;
	v1.z = (m_pos.z + m_fWidth * sinf(-m_rot.y));

	VecLine = v1 - v0;					// 壁の両端

	VecToPos.x = pos->x - v0.x;			// 壁の左端からpos
	VecToPos.z = pos->z - v0.z;

	VecToPosOld = posold - v0;

	if ((VecLine.z * VecToPos.x) - (VecLine.x * VecToPos.z) < 0)
	{//posが右にいる
		check = true;

	}
	else
	{//posが左にいる
		check = false;

	}

	if ((VecLine.z * VecToPosOld.x) - (VecLine.x * VecToPosOld.z) > 0)
	{//posoldが左にいる
		checkold = true;

	}
	else
	{//posoldが右にいる
		checkold = false;

	}

	if (check == true && checkold == true)
	{//範囲検証
		fIntersect = (VecToPos.z * VecMove.x) - (VecToPos.x * VecMove.z);
		fAll = (VecLine.z * VecMove.x) - (VecLine.x * VecMove.z);
		fRate = fIntersect / fAll;

		if (0.0f <= fRate && fRate <= 1.0f)
		{
			bChek = true;

		}
		else
		{
			bChek = false;

		}
	}

	// 位置強制
	if (bChek == true)
	{
		D3DXVECTOR3 vecHit = v0 + VecLine * fRate;
		pos->x = vecHit.x;
		pos->z = vecHit.z;

		*pos += m_vecNor * 4.0f;
		/*D3DXVECTOR3 p = *pos - m_pos;
		D3DXVec3Normalize(&p, &p);
		*pos = m_pos + (p);
		return true;*/
	}

	return bChek;
}