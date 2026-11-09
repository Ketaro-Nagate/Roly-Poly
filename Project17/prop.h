//==================================================================================
//
// 小物配置 [prop.h]
// Author : Nagate
//
//==================================================================================
#include "main.h"
#include "object.h"
#include "objectX.h"

#include "player.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_REGIST		64			// 登録できるモデルの最大数
#define MAX_MODEL_SET	512			// モデルの配置上限(Prop内での)

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CObjectX;

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CProp
{
public:
	CProp();
	~CProp();

	void Load(void);
	void Unload(void);
	void LoadProp(FILE* pFile);

	void SendPlayer(CPlayer* pPlayer);

	int GetCreateModel(void) { return m_nCntModel; }
	void AddDeath(void) { m_nCountDeath++; }
	int GetDeath(void) { return m_nCountDeath; }
	void ResetDeath(void) { m_nCountDeath = 0; }

private:
	int m_nNumModel;								// 登録モデル数
	CObjectX* m_pObjectX[MAX_MODEL_SET];			// 生成されたモデルへのポインタ
	int m_nCntModel;								// 生成されたモデルのカウント
	int m_nCountDeath;		// 死んだアイテムを数える

	D3DXVECTOR3 m_Playerpos;			// プレイヤーの位置
	D3DXVECTOR3 m_Playerrot;			// プレイヤーの角度
};
