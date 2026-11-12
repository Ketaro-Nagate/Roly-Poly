//==================================================================================
//
// カメラ[camera.cpp]
// Author : Nagate
//
//==================================================================================
//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "util.h"
#include "DebugProc.h"
#include "player.h"
#include "scene.h"

//#include "scene.h"

//**********************************************************************************
//	マクロ定義
//**********************************************************************************
#define POSV_FROM_PLAYER	// 視点の基準をプレイヤーにする
//#undef POSV_FROM_PLAYER

#define MAX_CAMERA (1)						// カメラの最大数
#define CAM_OFSET (10.0f)					// 注視点と対象物の差
#define CAM_MOVE_SPEED (4.0f)				// カメラの移動速度
#define CAM_FOLLOW_SPEED (0.4f)				// カメラ追従の減速係数	
#define POSR_Y			(50.0f)				// プレイヤーより少し上に
#define DISTANCE		(800.0f)			// 視点・注視点の距離
#define START_ROTX		(0.5f)				// 開始時のカメラの角度.x
#define MAX_UP_ROTX		(1.0f)				// 角度.xの最大値
#define MIN_DOWN_ROTX	(1.0f)				// 角度.xの最小値

#define CONTROL_SPEED	(5.0f)				// 視点移動の速度

//==================================================================================
// コンストラクタ
//==================================================================================
CCamera::CCamera()
{
	m_posR = NULL_D3DXVECTOR3;
	m_posRDest = NULL_D3DXVECTOR3;
	m_rot = NULL_D3DXVECTOR3;

	m_bControl = false;
}

//==================================================================================
// デストラクタ
//==================================================================================
CCamera::~CCamera()
{

}

//==================================================================================
// カメラの初期化処理
//==================================================================================
void CCamera::Init(void)
{
	m_rot.x = START_ROTX;
	m_rot.y = 0.0f;
	m_rot.z = 0.0f;

	m_VRDistance = DISTANCE;

	m_posV.x = 0.0f;
	m_posV.y = 150.0f;
	m_posV.z = -150.0f;			//視点
	m_posVDest.x = 0.0f;
	m_posVDest.y = 0.0f;
	m_posVDest.z = 0.0f;		//目的の視点

	m_posR = NULL_D3DXVECTOR3;
	m_posRDest = NULL_D3DXVECTOR3;

	m_fPTRot = 0.0f;
	m_rotDest = NULL_D3DXVECTOR3;

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
	m_fofset = CAM_OFSET;
}

//==================================================================================
// カメラの終了処理
//==================================================================================
void CCamera::Uninit(void)
{

}

//==================================================================================
// カメラの更新処理
//==================================================================================
void CCamera::Update(void)
{
	CManager* pManager = CManager::GetManager();
	CInputKeyboard* pKey = pManager->GetInputKeyboard();
	CScene* pScene = pManager->GetScene();
	CDebugProc *pDebugProc = pManager->GetDebugProc();

	CPlayer *pPlayer = pScene->GetPlayer();		// プレイヤーを取得

	if (pPlayer != nullptr)
	{
		if (pKey->GetTrigger(DIK_F3))					// カメラ操作切り替え
		{
			m_bControl ^= true;
			pPlayer->SwitchControl(!m_bControl);		// プレイヤーの操作権を切る
		}

		//----- 視点基準変更 -----//
		if (m_bControl == false)
		{// プレイヤー追従
			m_posR = pPlayer->GetPos();						// 注視点をプレイヤーに固定

			if (pPlayer->GetState() != CPlayer::STATE_DEATH)
			{// 死んでいなければ注視点に追従
				// 視点の設定
				m_posVDest.x = m_posR.x - sinf(m_rot.y) * m_VRDistance;
				m_posVDest.z = m_posR.z - cosf(m_rot.y) * m_VRDistance;
				m_posVDest.y = m_posR.y + sinf(m_rot.x) * m_VRDistance;

				// 視点慣性
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.9f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.9f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.9f;
			}
		}
	}

	// 視点移動
	Control(pKey);

	//--- カメラの角度を正規化 ---//
	RotCorrection(&m_rot);

	//--- カメラを設定 ---//
	SetCamera();

	pDebugProc->PrintDebug("CamRot / X:%f\n", m_rot.x);
}

//==================================================================================
// カメラ設定処理
//==================================================================================
void CCamera::SetCamera(void)
{
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),							//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//アスペクト比
		10.0f,											//Z値の最小値
		10000.0f);										//Z値の最大値

	//--- 平面投影を作成 ---//
	/*D3DXMatrixOrthoLH(&m_mtxProjection,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		10.0f,
		1000.0f);*/

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//==================================================================================
// カメラリセット
//==================================================================================
void CCamera::ResetCamera(void)
{
	m_posR = NULL_D3DXVECTOR3;
	m_posRDest = NULL_D3DXVECTOR3;
	m_posV = NULL_D3DXVECTOR3;
	m_posVDest = NULL_D3DXVECTOR3;
	m_rot = D3DXVECTOR3(START_ROTX, 0.0f, 0.0f);
	m_rotDest = NULL_D3DXVECTOR3;
	m_bControl = false;
	m_VRDistance = DISTANCE;
}

//==================================================================================
// カメラ操作
//==================================================================================
void CCamera::Control(CInputKeyboard* pKey)
{
	// カメラ操作時
	if (m_bControl == true)
	{
		// 注視点を移動させる
		if (pKey->GetPress(DIK_W))
		{
			if (pKey->GetPress(DIK_A))
			{// WA
				m_posR.x += sinf(RotCorrection(m_rot.y + D3DX_PI * -0.25f)) * CONTROL_SPEED;		// CamRot + ↖Rot
				m_posR.z += cosf(RotCorrection(m_rot.y + D3DX_PI * -0.25f)) * CONTROL_SPEED;
				m_rotDest.y = RotCorrection(m_rot.y + D3DX_PI * 0.75f);
			}
			else if (pKey->GetPress(DIK_D))
			{// WA
				m_posR.x += sinf(RotCorrection(m_rot.y + D3DX_PI * 0.25f)) * CONTROL_SPEED;			// CamRot + ↗Rot
				m_posR.z += cosf(RotCorrection(m_rot.y + D3DX_PI * 0.25f)) * CONTROL_SPEED;
				m_rotDest.y = RotCorrection(m_rot.y + D3DX_PI * -0.75f);
			}
			else
			{// W
				m_posR.x += sinf(m_rot.y) * CONTROL_SPEED;											// Only CamRot
				m_posR.z += cosf(m_rot.y) * CONTROL_SPEED;
				m_rotDest.y = RotCorrection(m_rot.y + D3DX_PI);
			}
		}
		else if (pKey->GetPress(DIK_S))
		{
			if (pKey->GetPress(DIK_A))
			{// WS
				m_posR.x += sinf(RotCorrection(m_rot.y + D3DX_PI * -0.75f)) * CONTROL_SPEED;		// CamRot + ↙Rot
				m_posR.z += cosf(RotCorrection(m_rot.y + D3DX_PI * -0.75f)) * CONTROL_SPEED;
				m_rotDest.y = RotCorrection(m_rot.y + D3DX_PI * 0.25f);
			}
			else if (pKey->GetPress(DIK_D))
			{// WS
				m_posR.x += sinf(RotCorrection(m_rot.y + D3DX_PI * 0.75f)) * CONTROL_SPEED;			// CamRot + ↘Rot
				m_posR.z += cosf(RotCorrection(m_rot.y + D3DX_PI * 0.75f)) * CONTROL_SPEED;
				m_rotDest.y = RotCorrection(m_rot.y + D3DX_PI * -0.25f);
			}
			else
			{// S
				m_posR.x -= sinf(m_rot.y) * CONTROL_SPEED;
				m_posR.z -= cosf(m_rot.y) * CONTROL_SPEED;
				m_rotDest.y = RotCorrection(m_rot.y);
			}
		}
		else if (pKey->GetPress(DIK_A))
		{// A
			m_posR.x -= sinf(RotCorrection(m_rot.y + D3DX_PI * 0.5f)) * CONTROL_SPEED;
			m_posR.z -= cosf(RotCorrection(m_rot.y + D3DX_PI * 0.5f)) * CONTROL_SPEED;
			m_rotDest.y = RotCorrection(m_rot.y + D3DX_PI * 0.5f);

		}
		else if (pKey->GetPress(DIK_D))
		{// D
			m_posR.x += sinf(RotCorrection(m_rot.y + D3DX_PI * 0.5f)) * CONTROL_SPEED;
			m_posR.z += cosf(RotCorrection(m_rot.y + D3DX_PI * 0.5f)) * CONTROL_SPEED;
			m_rotDest.y = RotCorrection(m_rot.y + D3DX_PI * -0.5f);
		}

		//--- 視点に注視点を追わせる ---//
		m_posV.x = m_posR.x - cosf(m_rot.x) * sinf(m_rot.y) * m_VRDistance;
		m_posV.z = m_posR.z - cosf(m_rot.x) * cosf(m_rot.y) * m_VRDistance;
		m_posV.y = m_posR.y + sinf(m_rot.x) * m_VRDistance;

		// 視点⇔注視点距離調整
		if (pKey->GetPress(DIK_RSHIFT))
		{
			m_VRDistance += 2.0f;
		}
		if (pKey->GetPress(DIK_LSHIFT))
		{
			m_VRDistance -= 2.0f;
		}

		//--- 注視点の上下 ---//
		if (pKey->GetPress(DIK_SPACE))
		{
			m_posR.y += 5.0f;
		}
		if (pKey->GetPress(DIK_LCONTROL))
		{
			m_posR.y -= 5.0f;
		}
	}

	//--- 視点を回す---//
	if (pKey->GetPress(DIK_RIGHT))
	{
		m_rot.y += 0.01f;
	}
	if (pKey->GetPress(DIK_LEFT))
	{
		m_rot.y -= 0.01f;
	}

	//--- 視点の上下 ---//
	if (pKey->GetPress(DIK_UP) && m_rot.x < MAX_UP_ROTX)
	{
		m_rot.x += 0.1f;
	}

	if (pKey->GetPress(DIK_DOWN) && m_rot.x > 0.0f)
	{
		m_rot.x -= 0.1f;
	}
}