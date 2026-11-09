//==================================================================================
//
//  アイテム [item.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "objectX.h"
#include "particle.h"

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CItem :public CObjectX
{
public:
	CItem();
	~CItem();

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFileName);
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool BallCollision(D3DXVECTOR3* pos, float fRadius);
	void SetScalingMode(bool bScaling) { m_bDrawScaling = bScaling; }
	void SetTimeUp(bool bTimeUp) { m_bTimeup = bTimeUp; }

	D3DXVECTOR3* GetPos(void) { return &m_pos; }
	D3DXVECTOR3* GetRot(void) { return &m_rot; }
	float GetSize(void) { return m_fmodelsize; }

private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_move;		// 移動量
	float m_fRadius;		// 半径
	float m_fMoveLoop;		// ループ運動に使用する値
	bool m_bGet;			// プレイヤーに取得されたか
	int m_nGetCount;		// 死んでからのカウント
	bool m_bDrawScaling;	// モデルの拡縮を行うか
	float m_fmodelsize;		// モデルのサイズ割合
	bool m_bTimeup;			// 時間切れによる消滅

	CParticle* m_pParticle;
};

#endif // !_ITEM_H_
