//==================================================================================
//
//  オブジェクト [object.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_OBJPRIORITY	8		// 描画優先順位の最大数

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_HIT,
		TYPE_BULLET,
		TYPE_MAX
	}TYPE;

	/*CObject();*/
	CObject(int nPriority = 3);
	virtual ~CObject();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	static int GetNumAll(void) { return m_nNumAll; }
	static CObject* GetTop(int nPriority) { return m_pTop[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

	static void CheckDead(void);
	void ReleaseDead(void);

	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }

protected:
	void Release(void);

private:
	//static CObject* m_apObject[MAX_OBJPRIORITY][MAX_OBJECT];			// オブジェクトインスタンスへのポインタ
	static CObject* m_pTop[MAX_OBJPRIORITY];		// 先頭オブジェクトのポインタ
	static CObject* m_pCur[MAX_OBJPRIORITY];		// 最後尾オブジェクトのポインタ
	CObject* m_pPrev;								// 前のオブジェクトへのポインタ
	CObject* m_pNext;								// 次のオブジェクトへのポインタ
	bool m_bDeath;									// 死亡フラグ

	static int m_nNumAll;			// オブジェクトの総数
	int m_nID;						// 自分自身のID
	TYPE m_type;					// オブジェクトの種類
	int m_nPriority;				// 優先順位の位置

};

#endif // !_OBJECT_H_