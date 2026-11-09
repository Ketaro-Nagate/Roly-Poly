//==================================================================================
// 
// アイテムの出現管理クラス.h [item_manager.h]
// Author : Nagate
// 
//==================================================================================
#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "item.h"
#include <vector>

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_ITEM_AWAVE		64			// 1ウェーブに呼べる敵の最大数
#define MAX_WAVE			32			// ウェーブ数の最大数
#define MAX_FILENAME		64			// ファイル名の保存上限

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CItemManager
{
public:
	// アイテムの呼び出しパターン情報構造体
	typedef struct
	{
		vector<D3DXVECTOR3> vpos;		// アイテムの生成位置
		int nNumItem;					// 生成するアイテムの数
		int nIdx;						// パターンのインデックス
	}PATTERN_INFO;

	CItemManager();
	~CItemManager();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void CallPattern(int nNum);
	void LoadFile(void);

	const std::vector<CItem *> *GetAllItem(void)const { return &m_vpItem; }
	void Remove(CItem *DeadItem);

private:
	std::vector<CItem*> m_vpItem;				// アイテムへのポインタ
	std::vector<PATTERN_INFO> m_vPattern;		// アイテムの呼び出しパターン
	int m_nNumPattern;							// パターン数
	int m_nNumItem;								// 生成されているアイテムの数
	int m_nTimeLimit;							// アイテムが消えるまでの時間制限カウント
	int m_nCountCreate;							// 生成までのカウンター

};


#endif // !_ITEMMANAGER_H_