//==================================================================================
//
//  地面 [field.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "main.h"
#include "field.h"
#include "manager.h"
#include "player.h"
#include "scene.h"

//==================================================================================
// コンストラクタ
//==================================================================================
CField::CField()
{
	m_pos = NULL_D3DXVECTOR3;
	m_rot = NULL_D3DXVECTOR3;
	m_size = NULL_D3DXVECTOR3;
}

//==================================================================================
// デストラクタ
//==================================================================================
CField::~CField()
{
}

//==================================================================================
// 生成
//==================================================================================
CField* CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンス生成
	CField *pField = new CField;

	// 初期化
	pField->Init(pos, size);

	// ポインタを返す
	return pField;
}

//==================================================================================
// 生成(テクスチャ指定)
//==================================================================================
CField* CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 texture, const float fTexSplit)
{
	// インスタンス生成
	CField *pField = new CField;

	// 初期化
	pField->Init(pos, size, texture, fTexSplit);

	// ポインタを返す
	return pField;
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 3Dオブジェクトの初期化処理
	if (CObject3D::Init(pos, size) == S_OK)
	{
		// 値保存
		m_pos = pos;
		m_size = size;

		float fLength = 0.0f;		// 対角線の長さ
		float fAngle = 0.0f;		// 対角線の角度

		fLength = sqrtf(size.x * size.x + size.z * size.z) * 0.5f;
		fAngle = (float)atan2(size.x, size.z);

		m_size.x = sinf(m_rot.y + fAngle) * fLength;
		m_size.y = size.y;
		m_size.z = cosf(m_rot.y + fAngle) * fLength;

		return S_OK;
	}

	return false;
}


//==================================================================================
// 初期化(テクスチャ指定)
//==================================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 texture, const float fTexSplit)
{
	// 3Dオブジェクトの初期化処理
	if (CObject3D::Init(pos, size, texture, fTexSplit) == S_OK)
	{
		// 値保存
		m_pos = pos;
		m_size = size;

		float fLength = 0.0f;		// 対角線の長さ
		float fAngle = 0.0f;		// 対角線の角度

		fLength = sqrtf(size.x * size.x + size.z * size.z) * 0.5f;
		fAngle = (float)atan2(size.x, size.z);

		m_size.x = sinf(m_rot.y + fAngle) * fLength;
		m_size.y = size.y;
		m_size.z = cosf(m_rot.y + fAngle) * fLength;
	}

	return false;
}

//==================================================================================
// 終了処理
//==================================================================================
void CField::uninit(void)
{
	// 3Dオブジェクトの終了処理
	CObject3D::Uninit();
}

//==================================================================================
// 更新
//==================================================================================
void CField::Update(void)
{
	// プレイヤーを取得
	CManager* pManager = CManager::GetManager();
	CScene* pScene = pManager->GetScene();
	CPlayer *pPlayer = pScene->GetPlayer();

	CObject3D::Update();

	// プレイヤーとの当たり判定
	if (Collision(pPlayer->GetPPos(), pPlayer->GetPosDest(), pPlayer->GetRadius()))
	{
		pPlayer->ResetJumpCount();
	}
}

//==================================================================================
// 描画
//==================================================================================
void CField::Draw(void)
{
	CObject3D::Draw();
}

//==================================================================================
// 当たり判定
//==================================================================================
bool CField::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 posold, float fWidth)
{
	bool bInCube = false;		// 範囲内にいるか
	bool bStand = false;		// 上に立っているか

	// 四角形の内外判定
	if (pos->x >= m_pos.x - m_size.x &&
		pos->x <= m_pos.x + m_size.x &&
		pos->z >= m_pos.z - m_size.z &&
		pos->z <= m_pos.z + m_size.z)
	{// 床の内側にいる
		bInCube = true;
	}

	// めり込み判定
	if (pos->y - fWidth <= m_pos.y &&
		posold.y - fWidth >= m_pos.y)
	{
		bStand = true;
	}

	// 床の全判定検証
	if (bInCube == true && bStand == true)
	{
		pos->y = m_pos.y + fWidth;			// 上に乗せる
		return true;
	}

	return false;
}