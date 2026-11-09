//==================================================================================
//
//  オブジェクト [object.cpp]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "object.h"
#include "camera.h"
#include "manager.h"

//**********************************************************************************
// 静的メンバ変数宣言
//**********************************************************************************
//CObject* CObject::m_apObject[MAX_OBJPRIORITY][MAX_OBJECT] = {};
CObject* CObject::m_pTop[MAX_OBJPRIORITY] = {};
CObject* CObject::m_pCur[MAX_OBJPRIORITY] = {};
int CObject::m_nNumAll = 0;

//==================================================================================
// コンストラクタ
//==================================================================================
/*CObject::CObject()
{
	
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		// --- 空の配列番号を探す ---/
		if (m_apObject[nCount] == NULL)
		{
			// IDを指定
			m_nID = nCount;

			// 最大数のカウントアップ
			m_nNumAll++;

			// 自身のアドレスを代入
			m_apObject[m_nPriority][m_nID] = this;

			break;
		}
	}
	
}*/

//==================================================================================
// コンストラクタ(オーバーロード)
//==================================================================================
CObject::CObject(int nPriority)
{
	//--- メンバ変数初期化 ---//
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_bDeath = false;
	m_type = TYPE_MAX;

	// 描画優先順位指定
	m_nPriority = nPriority;

	// 最大数のカウントアップ
	m_nNumAll++;

	// --- リストに自分を追加 ---/
	if (m_pTop[nPriority] != nullptr)
	{// 先頭がNULLではない
		CObject* pObject = m_pCur[nPriority];		// 最後尾を代入

		// 最後尾の次の位置に自分を入れる
		pObject->m_pNext = this;

		// 最後尾オブジェクトを自分の前として保存
		m_pPrev = pObject;

		// 自分を最後尾として保存
		m_pCur[nPriority] = this;
	}
	else
	{// 先頭がNULL( 自分が最初のオブジェクト )
		// 自分と先頭として保存
		m_pTop[nPriority] = this;

		// 自分を最後尾として保存
		m_pCur[nPriority] = this;
	}

}

//==================================================================================
// デストラクタ
//==================================================================================
CObject::~CObject()
{
	
}

//==================================================================================
// 全オブジェクトを破棄
//==================================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_OBJPRIORITY; nCntPriority++)
	{
		CObject* pObject = m_pTop[nCntPriority];		// 先頭オブジェクトを保存

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;		// 次のオブジェクトを保存

			// 終了処理
			pObject->Uninit();

			// 次を代入
			pObject = pObjectNext;
		}
	}

	// 死亡フラグ実行
	CheckDead();
}

//==================================================================================
// 全オブジェクトの更新
//==================================================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_OBJPRIORITY; nCntPriority++)
	{
		CObject* pObject = m_pTop[nCntPriority];		// 先頭オブジェクトを保存

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;		// 次のオブジェクトを保存

			// 更新処理
			pObject->Update();

			// 次を代入
			pObject = pObjectNext;
		}

		// 死亡フラグ実行
		CheckDead();
	}
}

//==================================================================================
// 全オブジェクトの描画
//==================================================================================
void CObject::DrawAll(void)
{
	CManager* pManager = CManager::GetManager();
	CCamera* pCamera = pManager->GetCamera();

	//カメラの設定
	pCamera->SetCamera();

	for (int nCntPriority = 0; nCntPriority < MAX_OBJPRIORITY; nCntPriority++)
	{
		CObject* pObject = m_pTop[nCntPriority];		// 先頭オブジェクトを保存

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;		// 次のオブジェクトを保存

			if (pObject->m_bDeath == false)
			{
				// 描画処理
				pObject->Draw();
			}

			// 次を代入
			pObject = pObjectNext;
		}
	}
}

//==================================================================================
// 自身を破棄
//==================================================================================
void CObject::Release(void)
{
	// 死亡フラグを立てる
	m_bDeath = true;
}

//==================================================================================
// 死亡フラグの確認
//==================================================================================
void CObject::CheckDead(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_OBJPRIORITY; nCntPriority++)
	{
		CObject* pObject = m_pTop[nCntPriority];		// 先頭オブジェクトを保存

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;		// 次のオブジェクトを保存

			if (pObject->m_bDeath == true)
			{// 死亡フラグが立っていたら開放
				pObject->ReleaseDead();
			}

			// 次を代入
			pObject = pObjectNext;
		}
	}
}

//==================================================================================
// 死亡フラグの立ったオブジェクトを削除
//==================================================================================
void CObject::ReleaseDead(void)
{
	//--- 自分をリストから破棄 ---//
	if (m_pPrev != nullptr)
	{// 先頭ではない
		CObject* pObjectPrev = m_pPrev;		// 自分の前
		if (m_pNext != nullptr)
		{// 最後尾ではない
			CObject* pObjectNext = m_pNext;		// 自分の次

			// 自分の前オブジェクトに次を教える
			pObjectPrev->m_pNext = m_pNext;

			// 自分の次オブジェクトに前を教える
			pObjectNext->m_pPrev = m_pPrev;
		}
		else
		{// 自分が最後尾オブジェクト
			// 自分の前オブジェクトに次がいないことを教える
			pObjectPrev->m_pNext = nullptr;

			// 自分の前オブジェクトを最後尾にする
			m_pCur[m_nPriority] = pObjectPrev;
		}
	}
	else
	{// 自分が先頭
		if (m_pNext != nullptr)
		{// 最後尾ではない
			CObject* pObjectNext = m_pNext;		// 自分の次

			// 自分の次オブジェクトに前がいないことを教える
			pObjectNext->m_pPrev = nullptr;

			// 自分の前オブジェクトを先頭にする
			m_pTop[m_nPriority] = pObjectNext;
		}
		else
		{// 自分しかオブジェクトがいない
			// 先頭・最後尾を空にする
			m_pTop[m_nPriority] = nullptr;
			m_pCur[m_nPriority] = nullptr;

		}
	}

	// 総数を減らす
	m_nNumAll--;

	// メモリを破棄
	delete this;
}