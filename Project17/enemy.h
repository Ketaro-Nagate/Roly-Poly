//==================================================================================
//
//   敵クラス [enemy.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CPlayer;

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CEnemy :public CObject
{
public:
	CEnemy();
	~CEnemy();

	static CPlayer* GetPlayer(void);

	virtual HRESULT Init(void);
	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	D3DXVECTOR3* GetPosition(void) { return &m_pos; }
	D3DXVECTOR3* GetRotation(void) { return &m_rot; }
	bool GetDead(void) { return m_bDead; }

private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 角度
	bool m_bDead;				// 生きているか

};


#endif // !_ENEMY_H_