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
	// 索敵タイプ
	typedef enum
	{
		STYPE_UTURN = 0,		// いったりきたり
		STYPE_ROTATION,			// 回る
		STYPE_BOX,				// 正方形に回る
		STYPE_MAX
	}SERCH_TYPE;

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

	static CEnemyBird* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 move, int nType, int nInterval, char* pModelFile);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 move, int nType, int nInterval, char* pModelFile);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void Search_UTurn(void);
	void Search_Rotation(void);
	void Search_Box(void);
	bool Collision(D3DXVECTOR3* Ppos, float fPRadius, D3DXVECTOR3* EPos);
	void PredationPlayer(CPlayer* pPlayer);
	bool MotionRange(void);

private:
	CObject3D* m_pWarning;	// 警告表示ポリゴンへのポインタ
	CObjectX* m_pBird;		// モデルへのポインタ
	float m_fSize;			// 警告テクスチャの大きさ(攻撃の範囲)
	float m_nRotateCount;	// モデルの回転カウント
	float m_fHitRange;		// 攻撃の範囲
	STATE m_state;			// 状態

	D3DXVECTOR3 m_offpos;	// 位置のオフセット
	D3DXVECTOR2 m_move;		// 移動量
	int m_nMoveCount;		// 移動時間カウント
	int m_nTurnInterval;		// ターンまでの時間
	bool m_bTurn;			// Uターン中か
	float m_fUturnRot;		// Uターン先角度
	SERCH_TYPE m_sType;		// 探すときの動き方

};


#endif // !_ENEMY_BIRD_H_