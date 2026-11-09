//==================================================================================
//
//  箱判定をもつオブジェクト [object_cube.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object_cube.h"
#include "manager.h"
#include "game.h"
#include "objectX.h"
#include "player.h"
#include "score.h"
#include "util.h"
#include "prop.h"
#include "scene.h"

//**********************************************************************************
//	マクロ定義
//**********************************************************************************

//==================================================================================
// コンストラクタ
//==================================================================================
CCubeObject::CCubeObject()
{
	m_pos = NULL_D3DXVECTOR3;		// 位置
	m_rot = NULL_D3DXVECTOR3;		// 角度
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//==================================================================================
// デストラクタ
//==================================================================================
CCubeObject::~CCubeObject()
{
}

//==================================================================================
// 生成( 生成モデルのファイル指定 )
//==================================================================================
CCubeObject* CCubeObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 fSize,const char* pFileName)
{
	// インスタンス生成
	CCubeObject* pItem = new CCubeObject;

	// 初期化
	pItem->Init(pos, rot, fSize, pFileName);

	// ポインタをかえす
	return pItem;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CCubeObject::Init(void)
{
	return S_FALSE;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CCubeObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 fSize, const char* pFileName)
{
	// Xモデルの初期化
	CObjectX::Init(pos, rot, pFileName);

	// 位置角度の保存
	m_pos = pos;
	m_rot = rot;
	m_size = fSize;

	return S_OK;
}

//==================================================================================
// 終了処理
//==================================================================================
void CCubeObject::Uninit(void)
{
	CObjectX::Uninit();
}
//==================================================================================
// 更新処理
//==================================================================================
void CCubeObject::Update(void)
{
	// プレイヤーを取得
	CManager* pManager = CManager::GetManager();
	CScene* pScene = pManager->GetScene();
	CPlayer* pPlayer = pScene->GetPlayer();

	// 当たり判定
	CubeCollision(pPlayer->GetPPos(), pPlayer->GetPosDest(), pPlayer->GetRadius(), pPlayer->GetRadius());
}

//==================================================================================
// 描画処理呼び出し
//==================================================================================
void CCubeObject::Draw(void)
{
	CObjectX::Draw();
}

//==================================================================================
// 当たり判定(球)
//==================================================================================
bool CCubeObject::CubeCollision(D3DXVECTOR3* pos, D3DXVECTOR3 posold, float fRange, float fRadius)
{
	bool bInside = false;		// 内側にいるか

	// 箱の内外判定
	if (pos->x + fRange >= m_pos.x - m_size.x && pos->x - fRange <= m_pos.x + m_size.x &&
		pos->z + fRange >= m_pos.z - m_size.y && pos->z - fRange <= m_pos.z + m_size.y)
	{// 内側にいる
		bInside = true;
	}

	// 押し返す向きの判定
	if (bInside == true)
	{
		// 右
		if (posold.x - fRange >= m_pos.x + m_size.x)
			pos->x = m_pos.x + m_size.x + fRange;

		// 左
		if (posold.x + fRange <= m_pos.x - m_size.x)
			pos->x = m_pos.x - m_size.x - fRange;

		// 奥
		if (posold.z - fRange >= m_pos.z + m_size.y)
			pos->z = m_pos.z + m_size.y + fRange;

		// 手前
		if (posold.z + fRange <= m_pos.z - m_size.y)
			pos->z = m_pos.z - m_size.y - fRange;

		return true;
	}

	// 接触しなかった
	return false;
}