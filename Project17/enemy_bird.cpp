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
#define BIRD_HEIGHT			(300.0f)			// 鳥が旋回する高さ
#define MAX_FIELD_RANGE		(750.0f)			// 移動範囲
#define UTURN_FRAME			(120)				// 移動中Uターンするまでの長さ
#define POLYGON_SIZE		(200.0f)			// 警告ポリゴンのサイズ

//==================================================================================
// コンストラクタ
//==================================================================================
CEnemyBird::CEnemyBird()
{
	m_fSize = 0.0f;
	m_pBird = nullptr;
	m_nRotateCount = 0;
	m_state = STATE_SERCH;
	m_move = D3DXVECTOR2(0.0f, 0.0f);
	m_nMoveCount = 0;
	m_nTurnInterval = UTURN_FRAME;
	m_bTurn = false;
	m_sType = STYPE_BOX;
	m_offpos = NULL_D3DXVECTOR3;
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
CEnemyBird* CEnemyBird::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 move, int nType, int nInterval, const char* pModelFile)
{
	// インスタンスを生成
	CEnemyBird* pBird = new CEnemyBird;

	// 初期化
	pBird->Init(pos, rot, move, nType, nInterval, pModelFile);

	//ポインタを返す
	return pBird;
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CEnemyBird::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 move, int nType, int nInterval, const char* pModelFile)
{
	//CManager* pManager = CManager::GetManager();
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	// 位置・角度・移動量を保存
	D3DXVECTOR3* pPos = GetPosition();
	*pPos = pos;
	D3DXVECTOR3* pRot = GetRotation();
	*pRot = rot;
	m_move = move;

	// オフセット保存
	m_offpos = pos;

	// ポリゴンサイズを保存
	m_fSize = POLYGON_SIZE;

	// 動作タイプを保存
	if (nType <= STYPE_MAX)
	{
		m_sType = static_cast<SERCH_TYPE>(nType);
	}

	// 警告表示ポリゴンを生成
	m_pWarning = CObject3D::Create(*pPos, D3DXVECTOR3(m_fSize, 0.0f, m_fSize), pTexture->GetAddress(CTexture::TEX_WARNING), 1.0f);

	// 敵モデルを生成
	m_pBird = CObjectX::Create(D3DXVECTOR3(pPos->x, pPos->y + BIRD_HEIGHT, pPos->z), rot, pModelFile);

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
		//---- プレイヤー探知中動作 ----//
		if (m_sType == STYPE_UTURN)
		{
			// Uターンする
			Search_UTurn();
		}
		else if(m_sType == STYPE_ROTATION)
		{
			// 旋回
			Search_Rotation();
		}
		else if (m_sType == STYPE_BOX)
		{
			// 四角形
			Search_Box();
		}

		// 角度同機
		m_pBird->GetRotation()->y = GetRotation()->y;

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
		D3DXVECTOR3* pRot = m_pBird->GetRotation();
		m_pBird->GetPosition()->x += m_move.x;
		m_pBird->GetPosition()->y += m_move.y;

		if (MotionRange())
		{
			m_move.x *= -1;
			pRot->y = RotCorrection(pRot->y + D3DX_PI);
		}

		// 敵の位置をモデルと同機
		*GetPosition() = *m_pBird->GetPosition();
	}
}

//==================================================================================
// 描画
//==================================================================================
void CEnemyBird::Draw(void)
{
}

//==================================================================================
// 索敵動作 Uターン
//==================================================================================
void CEnemyBird::Search_UTurn(void)
{
	// 自身の位置角度を取得
	D3DXVECTOR3* pPos = GetPosition();
	D3DXVECTOR3* pRot = GetRotation();

	// モデルの位置ポインタを取得
	D3DXVECTOR3* pBirdPos = m_pBird->GetPosition();
	D3DXVECTOR3* pBirdRot = m_pBird->GetRotation();

	// 位置更新
	pPos->x += m_move.x * -sinf(pRot->y);
	pPos->z += m_move.x * -cosf(pRot->y);

	// 位置同期
	m_pWarning->SetPos(D3DXVECTOR3(pPos->x, m_pWarning->GetPosition().y, pPos->z));
	pBirdPos->x = pPos->x;
	pBirdPos->z = pPos->z;

	// 移動中カウント更新
	if (m_bTurn == false)
	{
		m_nMoveCount++;
	}

	// カウント経過でUターン
	if (m_nMoveCount % m_nTurnInterval == 0)
	{
		m_fUturnRot = RotCorrection(pRot->y + D3DX_PI);
		m_bTurn = true;
		m_nMoveCount = 1;
	}

	// Uターン中処理
	if (m_bTurn == true)
	{
		pRot->y += 0.05f;		// 反対向きにする
		pRot->y = RotCorrection(pRot->y);				// 角度補正

		// Uターン終了
		if (m_fUturnRot + 0.05f > pRot->y && m_fUturnRot - 0.05f < pRot->y)
		{// 一定範囲まで回転
			// 残りの角度を矯正する
			pRot->y = m_fUturnRot;
			m_bTurn = false;
		}
	}
}

//==================================================================================
// 索敵動作 回転
//==================================================================================
void CEnemyBird::Search_Rotation(void)
{
	// 自身の位置角度を取得
	D3DXVECTOR3* pPos = GetPosition();
	D3DXVECTOR3* pRot = GetRotation();

	// 中心から現在地までの距離を取得
	float xdist = 50.0f;
	float zdist = 50.0f;

	// 回転角度を更新
	pRot->y += 0.03f;
	pRot->y = RotCorrection(pRot->y);

	// 中心を軸に位置を回転
	pPos->x = m_offpos.x + xdist * cosf(-pRot->y);
	pPos->z = m_offpos.z + zdist * sinf(-pRot->y);

	// モデル・ポリゴンの位置同期
	*m_pBird->GetPosition() = D3DXVECTOR3(pPos->x, m_pBird->GetPosition()->y, pPos->z);
	*m_pBird->GetRotation() = *pRot;
	m_pWarning->SetPos(*pPos);
}

//==================================================================================
// 索敵動作 回転
//==================================================================================
void CEnemyBird::Search_Box(void)
{
	// 自身の位置角度を取得
	D3DXVECTOR3* pPos = GetPosition();
	D3DXVECTOR3* pRot = GetRotation();

	// 位置更新
	pPos->x += m_move.x * -sinf(pRot->y);
	pPos->z += m_move.x * -cosf(pRot->y);

	// 移動中カウント更新
	if (m_bTurn == false)
	{
		m_nMoveCount++;
	}

	// カウント経過で角で曲がる
	if (m_nMoveCount % m_nTurnInterval == 0)
	{
		m_fUturnRot = RotCorrection(pRot->y + HALF_PI);
		m_bTurn = true;
		m_nMoveCount = 1;
	}

	// 角で曲がっているときの処理
	if (m_bTurn == true)
	{
		pRot->y += 0.05f;		// 反対向きにする
		pRot->y = RotCorrection(pRot->y);				// 角度補正

		// Uターン終了
		if (m_fUturnRot + 0.05f > pRot->y && m_fUturnRot - 0.05f < pRot->y)
		{// 一定範囲まで回転
			// 残りの角度を矯正する
			pRot->y = m_fUturnRot;
			m_bTurn = false;
		}
	}

	// 位置同期
	m_pWarning->SetPos(D3DXVECTOR3(pPos->x, m_pWarning->GetPosition().y, pPos->z));
	m_pBird->GetPosition()->x = pPos->x;
	m_pBird->GetPosition()->z = pPos->z;
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
		m_move = D3DXVECTOR2(4.0f, 4.0f);		// 右上に飛んでいく
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