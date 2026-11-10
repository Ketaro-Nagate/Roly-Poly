//==================================================================================
//
//  プレイヤー [player.cpp]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "player.h"
#include "object2D.h"
#include "DebugProc.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "util.h"
#include "effect.h"
#include <cmath>
#include "effect_model.h"
#include "particle_model.h"
#include "enemy.h"
#include "game.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define QUATERNION					// クォータニオンで動かす
#define MAX_TEXTURE		(8)			// テクスチャ最大数
#define SPEED			(0.5f)		// 移動速度
#define SENSITIVITY		(0.04f)		// 回転慣性
#define ROTATION_SPEED	(0.5f)		// 回転速度
#define RADIUS			(25.0f)		// 半径
#define NUM_LIGHT		(3)			// ライトの数
#define MAX_FIELD_RANGE	(750.0f)	// 移動範囲
#define JUMP_HEIGHT		(50.0f)		// ジャンプ高さ
#define MOVEY_INERTIA	(0.5f)		// Y軸慣性
#define GRAVITY D3DXVECTOR3(0.0f, 7.0f, 0.0f)		// 重力
#define MAX_JUMP		(2)			// 着地までに何回までジャンプ出来るか
#define GAMEEND_TIME	(240)		// 死亡カウント開始から遷移までの時間

//**********************************************************************************
//グローバル変数
//**********************************************************************************

//==================================================================================
// コンストラクタ
//==================================================================================
CPlayer::CPlayer()
{
	m_fValueRot = 0.0f;
	m_pos = NULL_D3DXVECTOR3;
	m_move = NULL_D3DXVECTOR3;
	m_moveDirection = NULL_D3DXVECTOR3;
	m_posDest = NULL_D3DXVECTOR3;
	m_vecAxis = NULL_D3DXVECTOR3;
	memset(m_aTexture, NULL, sizeof(m_aTexture));
	m_fSensitivity = SENSITIVITY;
	m_fRotationSpeed = ROTATION_SPEED;
	m_fRadius = RADIUS;
	m_vGravity = GRAVITY;
	m_bRun = false;
	m_nCntJump = 0;

	m_rot = NULL_D3DXVECTOR3;
	m_rotDest = NULL_D3DXVECTOR3;

	m_pOrbit = nullptr;
	m_Particle = nullptr;
	m_pKiller = nullptr;
	m_state = STATE_NONE;
	m_bControl = true;
	m_nDeathCount = 0;
}

//==================================================================================
// デストラクタ
//==================================================================================
CPlayer::~CPlayer()
{

}

//==================================================================================
// プレイヤー生成
//==================================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// インスタンス生成
	CPlayer *pPlayer = new CPlayer;

	// 初期化
	pPlayer->Init(pos, rot);

	// ポインタを返す
	return pPlayer;
}

//==================================================================================
// プレイヤー生成 (引数なし)
//==================================================================================
CPlayer *CPlayer::Create(void)
{
	// インスタンス生成
	CPlayer *pPlayer = new CPlayer;

	// 初期化
	pPlayer->Init();

	// ポインタを返す
	return pPlayer;
}

//==================================================================================
// 初期化
//==================================================================================
HRESULT CPlayer::Init(void)
{
	// デバイスを取得
	CManager *pManager = CManager::GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイル読み込み
	D3DXLoadMeshFromX("data\\MODEL\\player.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMdl = 0; nCntMdl < m_dwNumMat; nCntMdl++)
	{//テクスチャファイルを読み込む
		if (pMat[nCntMdl].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMdl].pTextureFilename, &m_aTexture[nCntMdl]);
		}

	}

	D3DXMatrixIdentity(&m_mtxRot);

	m_pos.y += 30.0f;
	m_pos.z -= 500.0f;

	// オービット生成
	if (m_pOrbit == nullptr)
	{
		m_pOrbit = COrbit::Create(&m_pos, 10.0f, 10.0f, BLUE, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 2, 0);
	}

	/*if (m_Particle == nullptr)
	{
		m_Particle = CModelParticle::Create(&m_pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 4);
	}*/

	return S_OK;
}

//==================================================================================
// 初期化 ( 位置 / 角度を指定 )
//==================================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// デバイスを取得
	CManager *pManager = CManager::GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイル読み込み
	D3DXLoadMeshFromX("data\\MODEL\\spikeball001.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMdl = 0; nCntMdl < m_dwNumMat; nCntMdl++)
	{//テクスチャファイルを読み込む
		if (pMat[nCntMdl].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMdl].pTextureFilename, &m_aTexture[nCntMdl]);
		}

	}

	// 引数を受け取る
	m_pos = pos;
	m_rot = rot;

	D3DXMatrixIdentity(&m_mtxRot);

	if (m_pOrbit == nullptr)
	{
		m_pOrbit = COrbit::Create(&m_pos, 10.0f, 10.0f, BLUE, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 2, 0);
	}

	return S_OK;
}

//==================================================================================
// 終了
//==================================================================================
void CPlayer::Uninit(void)
{
	// メッシュ破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// オービットの破棄
	if (m_pOrbit != nullptr)
	{
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}

	// パーティクルの破棄
	if (m_Particle != nullptr)
	{
		m_Particle->Uninit();
		m_Particle = nullptr;
	}

	CObject::Release();
}

//==================================================================================
// 更新
//==================================================================================
void CPlayer::Update(void)
{
	CManager *pManager = CManager::GetManager();
	CDebugProc *pDebugProc = pManager->GetDebugProc();
	
	CInputKeyboard *pKey = pManager->GetInputKeyboard();
	CInputMouse *pMouse = pManager->GetInputMouse();
	CCamera *pCamera = pManager->GetCamera();

	// 位置保存
	m_posDest = m_pos;

	// 重力負荷
	m_pos -= m_vGravity;

	//==========================//
	//--- プレイヤー操作処理 ---//
	bool bMove = false;			// 移動キーが押されたか
	if (m_bControl == true)
	{// 操作権確認
		bMove = Control(pKey, pMouse, pCamera);
	}

	//==========================//
	//--- モデル回転量設定 ---//
	if (bMove == true)
	{// 目標回転速度に近づける
		m_fValueRot += (m_fRotationSpeed - m_fValueRot) * m_fSensitivity;
	}
	if (bMove == false)
	{// 0に近づける
		m_fValueRot += (0.0f - m_fValueRot) * m_fSensitivity;
	}

	//==========================//
	//-------- 移動処理 --------//
	Move();

	// 補正確度を求める
	float fRotDiff = RotCorrection(m_rotDest.y - m_rot.y);

	// 角度補正
	if (fRotDiff >= D3DX_PI / 1.05f || fRotDiff <= -D3DX_PI / 1.05f)
	{
		// 角度差が90度以上なら即座に変更
		m_rot.y = m_rotDest.y;
		m_fValueRot *= -1.0f;
	}
	else
	{
		// それ以外は徐々に近づける
		m_rot.y = RotFollow(m_rotDest.y, m_rot.y, 0.05f);
	}

	//--- 移動 ---//
	m_pos += m_move;

	//==========================//
	//------- 死亡時処理 -------//
	if (m_state == STATE_DEATH)
	{
		DeadProcessing();
	}

	// 回転速度の操作
	if (pKey->GetTrigger(DIK_R))
	{
		m_fRotationSpeed += 0.01;
	}
	else if (pKey->GetTrigger(DIK_F))
	{
		m_fRotationSpeed -= 0.01;
	}

	// 回転感度の操作
	if (pKey->GetTrigger(DIK_T))
	{
		m_fSensitivity += 0.001;
	}
	else if (pKey->GetTrigger(DIK_G))
	{
		m_fSensitivity -= 0.001;
	}

	pDebugProc->PrintDebug("-----------プレイヤー情報-----------\n");
	pDebugProc->PrintDebug("位置 / X:%f Y:%f Z%f\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->PrintDebug("角度 / X:%f Y:%f Z%f\n", m_rot.x, m_rot.y, m_rot.z);
	pDebugProc->PrintDebug("移動量 / X:%f Y:%f Z%f\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->PrintDebug("回転軸 / X:%f / Y:%f / Z%f\n", m_vecAxis.x, m_vecAxis.y, m_vecAxis.z);
	pDebugProc->PrintDebug("回転量 / X:%f\n", m_fValueRot);
	pDebugProc->PrintDebug("回転速度 / %f (R + / F -)\n", m_fRotationSpeed);
	pDebugProc->PrintDebug("回転感性 / %f (T + / G -)\n", m_fSensitivity);
	pDebugProc->PrintDebug("-----------プレイヤー操作-----------\n");
	pDebugProc->PrintDebug("移動 / WASD\n");
	pDebugProc->PrintDebug("ジャンプ / SPACE\n");

	// 行動範囲制限
	MotionRange();

	//========================
	// 強制床処理
	if (m_pos.y <= -10.0f)
	{
		m_pos.y = -10.0f;
	}
}

//==================================================================================
// 描画
//==================================================================================
void CPlayer::Draw(void)
{
	//--- デバイスを取得 ---//
	CManager *pManager = CManager::GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScl;	// 計算用マトリックス
	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用

	//--- ワールドマトリックスの初期化 ---//
	D3DXMatrixIdentity(&m_mtxWorld);

	//--- スケーリング ---//
	D3DXMatrixScaling(&mtxScl, 2.0f, 2.0f, 2.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// クォータニオンを使用していない時、使用
#ifndef QUATERNION
	////--- 向きを反映 ---//
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
#endif // !QUATERNION

	// 回転軸における指定の回転軸からクォータニオンを作成
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	// クォータニオンから回転マトリックスを作成
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	//--- 位置を反映---//
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//--- ワールドマトリックスの設定 ---//
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//--- 現在のマテリアルを取得 ---//
	pDevice->GetMaterial(&matDef);

	//--- マテリアルデータへのポインタを取得 ---//
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
		{
			if (m_aTexture[nCntTex] != NULL)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, m_aTexture[nCntTex]);
			}
			else
			{
				pDevice->SetTexture(0,NULL);
			}
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//--- 保存していたマテリアルを戻す ---//
	pDevice->SetMaterial(&matDef);

}

//==================================================================================
// 操作
//==================================================================================
bool CPlayer::Control(CInputKeyboard *pkey, CInputMouse *pMouse, CCamera *pCamera)
{
	CManager* pManager = CManager::GetManager();
	float fCamRotY = pManager->GetCamera()->GetRot().y;
	bool bMove = false;		// 移動入力しているか

	//--- プレイヤー移動 ---//
	if (pkey->GetPress(DIK_W))
	{
		if (pkey->GetPress(DIK_A))
		{// WA
			m_rotDest.y = RotCorrection(D3DX_PI * 0.75f + fCamRotY);
		}
		else if (pkey->GetPress(DIK_D))
		{// WD
			m_rotDest.y = RotCorrection(D3DX_PI * -0.75f + fCamRotY);
		}
		else
		{// W
			m_rotDest.y = RotCorrection(D3DX_PI + fCamRotY);
		}
		bMove = true;			// 移動判定TRUE
	}
	else if (pkey->GetPress(DIK_S))
	{
		if (pkey->GetPress(DIK_A))
		{// SA
			m_rotDest.y = RotCorrection(D3DX_PI * 0.25f + fCamRotY);
		}
		else if (pkey->GetPress(DIK_D))
		{// SD
			m_rotDest.y = RotCorrection(D3DX_PI * -0.25f + fCamRotY);
		}
		else
		{// S
			m_rotDest.y = RotCorrection(0.0f + fCamRotY);
		}
		bMove = true;			// 移動判定TRUE
	}
	else if (pkey->GetPress(DIK_A))
	{// A
		m_rotDest.y = RotCorrection(D3DX_PI * 0.5f + fCamRotY);
		bMove = true;			// 移動判定TRUE

	}
	else if (pkey->GetPress(DIK_D))
	{// D
		m_rotDest.y = RotCorrection(D3DX_PI * -0.5f + fCamRotY);
		bMove = true;			// 移動判定TRUE
	}

	// ジャンプ
	/*if (pkey->GetTrigger(DIK_SPACE) && m_nCntJump < MAX_JUMP)
	{
		m_move.y += JUMP_HEIGHT;
		m_nCntJump++;
	}*/

	return bMove;
}

//==================================================================================
// 行動範囲制限
//==================================================================================
void CPlayer::MotionRange(void)
{
	if (m_pos.x + m_fRadius >= MAX_FIELD_RANGE)
	{
		m_pos.x = MAX_FIELD_RANGE - m_fRadius;
	}

	if (m_pos.x - m_fRadius <= -MAX_FIELD_RANGE)
	{
		m_pos.x = -MAX_FIELD_RANGE + m_fRadius;
	}

	if (m_pos.z + m_fRadius >= MAX_FIELD_RANGE)
	{
		m_pos.z = MAX_FIELD_RANGE - m_fRadius;
	}

	if (m_pos.z - m_fRadius <= -MAX_FIELD_RANGE)
	{
		m_pos.z = -MAX_FIELD_RANGE + m_fRadius;
	}
}

//==================================================================================
// 始動処理
//==================================================================================
void CPlayer::Move(void)
{
	//==========================//
	//--- 移動 / 感性設定 ---//
	// 移動量設定
	float fMove = (m_fValueRot / DOUBLE_PI) * (m_fRadius * DOUBLE_PI);	// 移動量
	m_move.x = fMove * -sinf(m_rot.y);
	m_move.z = fMove * -cosf(m_rot.y);
	m_move.y += (0.0f - m_move.y) * 0.2f;

	// 移動方向設定
	D3DXVECTOR3 NormalizeMove = VectorNormalize(m_move);
	m_vecAxis.x = NormalizeMove.z;
	m_vecAxis.z = -NormalizeMove.x;

	//==================================//
	// 移動しているかテスト
	if (std::abs(m_move.x) + std::abs(m_move.z) > 0.01f)
	{
		// 移動確認・真
		m_bRun = true;
	}
	else
	{
		// 移動確認・偽
		m_bRun = false;
	}
}

//==================================================================================
// 死亡時処理
//==================================================================================
void CPlayer::DeadProcessing(void)
{
	if (m_pKiller != nullptr)
	{// 殺された相手が登録済みか
		// キラーの位置を取得
		D3DXVECTOR3* pKPos = m_pKiller->GetPosition();

		// 連れていかれる
		m_pos = *pKPos;
		m_pos.y -= m_fRadius;

		// 死亡カウント
		m_nDeathCount++;

		// カウント終了でシーンやり直し
		if (m_nDeathCount > GAMEEND_TIME)
		{
			dynamic_cast<CGame*>(CManager::GetManager()->GetScene())->EndScene(CScene::MODE_GAME);
		}
	}
}