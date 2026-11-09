//==================================================================================
//
//   鳥クラス [enemy_bird.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _ENEMY_BIRD_H_
#define _ENEMY_BIRD_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "enemy.h"
#include "texture.h"
#include "objectX.h"
#include "object3D.h"

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CPlayer;

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CEnemyBird :public CEnemy
{
public:
	// 状態
	typedef enum
	{
		STATE_SERCH = 0,		// 探索
		STATE_PREDATION,		// 捕食
		STATE_LEAVE,			// 去る
		STATE_MAX
	}STATE;

	CEnemyBird();
	~CEnemyBird();

	static CEnemyBird* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize, int nAtkTime, char* pModelFile);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float size, int nAtkTime, char* pModelFile);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void BirdRotation(void);
	bool Collision(D3DXVECTOR3* Ppos, float fPRadius, D3DXVECTOR3* EPos);
	void PredationPlayer(CPlayer* pPlayer);
	bool MotionRange(void);

private:
	CObject3D* m_pWarning;	// 警告表示ポリゴンへのポインタ
	CObjectX* m_pBird;		// モデルへのポインタ
	float m_fSize;			// 警告テクスチャの大きさ(攻撃の範囲)
	int m_nAtkTime;			// 攻撃までの時間
	int m_nAtkCount;		// 攻撃までのカウント
	float m_nRotateCount;	// モデルの回転カウント
	float m_fHitRange;		// 攻撃の範囲
	STATE m_state;			// 状態
	D3DXVECTOR3 m_move;		// 移動量

};


#endif // !_ENEMY_BIRD_H_