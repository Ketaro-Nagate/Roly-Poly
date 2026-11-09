//==================================================================================
// 
// プレイヤー [player.h]
// Author : Nagate
//==================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//==================================================================================
//	インクルードファイル
//==================================================================================
#include "object.h"
#include "main.h"
#include "input.h"
#include "camera.h"
#include "orbit.h"

//==================================================================================
//	マクロ定義
//==================================================================================
#define MAX_XTEXTURE (8)		//テクスチャの最大数

//==================================================================================
//	前方宣言
//==================================================================================
class CObjectX;
class CModelParticle;
class CEnemy;

//==================================================
//	クラス定義
//==================================================
class CPlayer :public CObject
{
public:
	typedef enum
	{
		STATE_NONE = 0,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	CPlayer();
	~CPlayer();

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static CPlayer *Create(void);
	bool Control(CInputKeyboard *pkey, CInputMouse *pMouse, CCamera *pCamera);
	void MotionRange(void);
	void ResetJumpCount(void) { m_nCntJump = 0; }
	void Move(void);
	void SetState(STATE setstate) { m_state = setstate; }
	void ResistKiller(CEnemy* pKiller) { m_pKiller = pKiller; }
	void DeadProcessing(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3* GetPPos(void) { return &m_pos; }
	D3DXVECTOR3 GetPosDest(void) { return m_posDest; }
	float GetRadius(void) { return m_fRadius; }
	void SetPos(D3DXVECTOR3 set) { m_pos = set; }
	void SetPosY(float set) { m_pos.y = set; }
	void SetRot(D3DXVECTOR3 set) { m_rot = set; }
	void SwitchControl(bool bControl) { m_bControl = bControl; }
	STATE GetState(void) { return m_state; }

private:
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_moveDirection;		// 移動方向
	D3DXVECTOR3 m_posDest;				// 位置
	D3DXMATRIX m_mtxRot;				// 回転マトリックス
	D3DXQUATERNION m_quat;				// クオータニオン
	D3DXVECTOR3 m_vecAxis;				// 回転軸
	float m_fValueRot;					// 回転角(回転量)
	float m_fSensitivity;				// 回転感性
	float m_fRotationSpeed;				// 回転速度
	float m_fRadius;					// 半径
	D3DXVECTOR3 m_vGravity;				// 重力のベクトル
	bool m_bRun;						// 移動しているか
	int m_nCntJump;						// 使用したジャンプ回数のカウント
	STATE m_state;						// 状態
	CEnemy* m_pKiller;					// 自身を殺害した敵を記憶
	bool m_bControl;					// 操作可能か

	D3DXVECTOR3 m_rot;					// 角度
	D3DXVECTOR3 m_rotDest;				// 目標角度
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	CObjectX* m_pModel;					// モデル(X)

	LPD3DXMESH m_pMesh;					// メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat;			// マテリアルへのポインタ
	DWORD m_dwNumMat;					// マテリアルの数
	LPDIRECT3DTEXTURE9 m_aTexture[MAX_XTEXTURE];	// テクスチャへのポインタ

	COrbit* m_pOrbit;					// オービット
	CModelParticle* m_Particle;			// パーティクル
	int m_nDeathCount;					// 死亡カウント

};

#endif _PLAYER_H_