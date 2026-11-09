//==================================================================================
//
//  鳥クラスの処理 [enemy_bird.cpp]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "enemy_bird.h"
#include "manager.h"
#include "game.h"
#include "util.h"
#include "player.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define BIRD_HEIGHT		300.0f			// 鳥が旋回する高さ
#define MAX_FIELD_RANGE	(750.0f)	// 移動範囲

//==================================================================================
// コンストラクタ
//==================================================================================
CEnemyBird::CEnemyBird()
{
	m_fSize = 0.0f;
	m_nAtkCount = 0;
	m_nAtkTime = 0;
	m_pBird = nullptr;
	m_nRotateCount = 0;
	m_state = STATE_SERCH;
	m_move = NULL_D3DXVECTOR3;
}

//==================================================================================
// デストラクタ
//==================================================================================
CEnemyBird::~CEnemyBird()
{
}

//==================================================================================
// 生成
//==================================================================================
CEnemyBird* CEnemyBird::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSize, int nAtkTime, char* pModelFile)
{
	// インスタンスを生成
	CEnemyBird* pBird = new CEnemyBird;

	// 初期化
	pBird->Init(pos, rot, fSize, nAtkTime, pModelFile);

	//ポインタを返す
	return pBird;
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CEnemyBird::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float size, int nAtkTime, char* pModelFile)
{
	//CManager* pManager = CManager::GetManager();
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	// 位置・角度を保存
	D3DXVECTOR3* pPos = GetPosition();
	*pPos = pos;
	D3DXVECTOR3* pRot = GetRotation();
	*pRot = rot;

	// ポリゴンサイズを保存
	m_fSize = size;

	// 警告表示ポリゴンを生成
	m_pWarning = CObject3D::Create(pos, D3DXVECTOR3(size, 0.0f, size), pTexture->GetAddress(CTexture::TEX_WARNING), 1.0f);

	// 敵モデルを生成
	m_pBird = CObjectX::Create(D3DXVECTOR3(pos.x, pos.y + BIRD_HEIGHT, pos.z), rot, pModelFile);

	// 攻撃までの時間を設定
	m_nAtkTime = nAtkTime;

	// 当たり判定のサイズ設定
	m_fHitRange = m_fSize * 0.4f;

	// モデルを大きくする
	m_pBird->SetScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
	m_pBird->SwitchScaling(true);

	return S_FALSE;
}

//==================================================================================
// 終了
//==================================================================================
void CEnemyBird::Uninit(void)
{
	//m_pWarning = NULL;;

	// オブジェクトの終了処理
	CEnemy::Release();
}

//==================================================================================
// 更新
//==================================================================================
void CEnemyBird::Update(void)
{
	// プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();

	//===== 状態管理 =====//
	if (m_state == STATE_SERCH)						//--- 探索状態 ---//
	{
		// モデルを回転させる
		BirdRotation();

		// プレイヤーとの当たり判定
		if (Collision(pPlayer->GetPPos(), pPlayer->GetRadius(), GetPosition()))
		{
			// 当たったとき、捕食モードへ
			m_state = STATE_PREDATION;
		}
	}
	else if (m_state == STATE_PREDATION)			//--- 捕食状態 ---//
	{
		// プレイヤーを追っかけて捕まえる
		PredationPlayer(pPlayer);
	}
	else if (m_state == STATE_LEAVE)				//--- 退場状態 ---//
	{
		// 飛んでいく
		D3DXVECTOR3* pos = GetPosition();
		D3DXVECTOR3* rot = m_pBird->GetRotation();

		// 回転
		/*rot->y += 0.03;
		rot->y = RotCorrection(rot->y);
		m_pBird->GetPosition()->x += cosf(-rot->y) * m_move.x;
		m_pBird->GetPosition()->z += sinf(-rot->y) * m_move.x;*/
		*m_pBird->GetPosition() += m_move;
		if (MotionRange())
		{
			m_move.x *= -1;
			rot->y = RotCorrection(rot->y + D3DX_PI);
		}

		// 敵の位置をモデルと同機
		*pos = *m_pBird->GetPosition();
	}
}

//==================================================================================
// 描画
//==================================================================================
void CEnemyBird::Draw(void)
{
}

//==================================================================================
// 鳥モデル回転処理
//==================================================================================
void CEnemyBird::BirdRotation(void)
{
	// モデルの位置ポインタを取得
	D3DXVECTOR3* pBirdPos = m_pBird->GetPosition();
	D3DXVECTOR3* pBirdRot = m_pBird->GetRotation();

	// ポリゴンの位置
	D3DXVECTOR3 centerPos = m_pWarning->GetPosition();

	// 中心からBirdまでの距離を取得
	float xdist = 50.0f;
	float zdist = 50.0f;

	// 回転角度を更新
	pBirdRot->y += 0.03f;
	pBirdRot->y = RotCorrection(pBirdRot->y);

	// 中心を軸にBirdの位置を回転
	pBirdPos->x = centerPos.x + xdist * cosf(-pBirdRot->y);
	pBirdPos->z = centerPos.z + zdist * sinf(-pBirdRot->y);
}

//==================================================================================
// 当たり判定
//==================================================================================
bool CEnemyBird::Collision(D3DXVECTOR3* Ppos, float fPRadius, D3DXVECTOR3* EPos)
{
	// 自分とHit対象のベクトル
	D3DXVECTOR3 vecToPos = *EPos - *Ppos;

	// 球判定
	if (D3DXVec3Length(&vecToPos) <= (double)m_fHitRange + (double)fPRadius)
	{
		/*D3DXVECTOR3 p = *pos - DPos;
		D3DXVec3Normalize(&p, &p);
		*pos = DPos + (p * (m_fHitRange + fRadius));*/		// 位置矯正の処理
		return true;
	}

	return false;
}

//==================================================================================
// プレイヤーを捕食
//==================================================================================
void CEnemyBird::PredationPlayer(CPlayer* pPlayer)
{
	// モデルの位置ポインタを取得
	D3DXVECTOR3* pBirdPos = m_pBird->GetPosition();
	D3DXVECTOR3* pBirdRot = m_pBird->GetRotation();

	// プレイヤーの情報取得
	D3DXVECTOR3* pPlayerPos = pPlayer->GetPPos();		// プレイヤーの位置へのポインタ
	float fPRadius = pPlayer->GetRadius();				// プレイヤーの半径

	//===== 自身のモデルとプレイヤーの当たり判定を行う =====//
	// 近距離で当たれるようにする
	m_fHitRange = 10.0f;

	if (Collision(pPlayerPos, fPRadius, pBirdPos) == false)
	{// *非接触*
		// プレイヤーに向く
		D3DXVECTOR3 EtoP = *pPlayerPos - *pBirdPos;			// 自身からプレイヤーへのベクトル
		pBirdRot->y = atan2f(-EtoP.x, -EtoP.z);				// ベクトルから角度を出す

		// 飛んで追っかける
		*pBirdPos += VectorNormalize(EtoP) * 10.5f;
	}
	else
	{// *接触*
		//----- プレイヤーを掴んで飛んでいく -----//
		D3DXVECTOR3* pos = GetPosition();		// プレイヤーがついてこれるようにposを鳥の位置に更新
		*pos = *pBirdPos;

		pBirdRot->y = -HALF_PI - 0.1f;			// 右上を見る
		pBirdRot->x = HALF_PI * 0.3f;
		m_move = D3DXVECTOR3(4.0f, 4.0f, 0.0f);	// 右上に飛んでいく
		m_state = STATE_LEAVE;					// 状態を退場にする

		// 自分をプレイヤーキラーに登録
		pPlayer->ResistKiller(this);

		// プレイヤーは死ぬ
		pPlayer->SetState(CPlayer::STATE_DEATH);

		// プレイヤーを操作不能にする
		pPlayer->SwitchControl(false);
	}
}

//==================================================================================
// 行動範囲制限
//==================================================================================
bool CEnemyBird::MotionRange(void)
{
	D3DXVECTOR3* pos = m_pBird->GetPosition();
	float fRadius = 50.0f;

	bool bHit = false;

	if (pos->x + fRadius >= MAX_FIELD_RANGE)
	{
		pos->x = MAX_FIELD_RANGE - fRadius;
		bHit = true;
	}

	if (pos->x - fRadius <= -MAX_FIELD_RANGE)
	{
		pos->x = -MAX_FIELD_RANGE + fRadius;
		bHit = true;
	}

	if (pos->z + fRadius >= MAX_FIELD_RANGE)
	{
		pos->z = MAX_FIELD_RANGE - fRadius;
		bHit = true;
	}

	if (pos->z - fRadius <= -MAX_FIELD_RANGE)
	{
		pos->z = -MAX_FIELD_RANGE + fRadius;
		bHit = true;
	}

	return bHit;
}