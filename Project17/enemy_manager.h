//==================================================================================
// 
// エネミーの出現管理クラス.h [enemy_manager.h]
// Author : Nagate
// 
//==================================================================================
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "enemy.h"
#include <vector>
#include <string>

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_ENEMY_AWAVE		64			// 1ウェーブに呼べる敵の最大数
#define MAX_WAVE			32			// ウェーブ数の最大数
#define MAX_FILENAME		64			// ファイル名の保存上限

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CEnemyManager
{
public:
	// 敵生成情報構造体
	typedef struct
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 角度
		D3DXVECTOR2 move;		// 移動量.x / 上昇量.y
		int nType;				// 動作タイプ
		int nInterval;			// 探知動作のターンインターバル
		int nIdx;				// 登録番号
	}ENEMY_INFO;

	CEnemyManager();
	~CEnemyManager();

	static CEnemyManager* Create(int nFrequency, int nMaxEnemy);
	void Init(int nFrequency, int nMaxEnemy);
	void Uninit(void);
	void Update(void);
	void SetLevel(int nFrequency, int nMaxEnemy);
	void LoadFile(void);

	const std::vector<CEnemy *> *GetAllEnemy(void)const { return &m_vpEnemy; }
	void Remove(CEnemy *DeadEnemy);

private:
	std::vector<CEnemy*> m_vpEnemy;				// エネミーへのポインタ
	std::vector<ENEMY_INFO> m_vEnemyInfo;		// エネミーへのポインタ
	int m_nNumEnemy;							// 敵の数
	int m_nCountDead;							// 死んだ敵の数
	int m_nMaxEnemy;							// 呼び出す最大数
	int m_nCountCreate;							// 生成までのカウンター
	int m_nFrequency;							// 呼び出す頻度

	std::string m_sFileName;

};


#endif // !_ENEMYMANAGER_H_