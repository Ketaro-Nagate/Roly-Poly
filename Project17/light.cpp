//==================================================================================
//
//	照明[light.cpp]
//	Author : NAGATE
//
//==================================================================================
//**********************************************************************************
//	インクルードファイル
//**********************************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//==================================================================================
// コンストラクタ
//==================================================================================
CLight::CLight()
{
	memset(&m_aLight,0,sizeof(m_aLight));
	m_nIdx = 0;
}

//==================================================================================
// デストラクタ
//==================================================================================
CLight::~CLight()
{

}

//==================================================================================
//	ライトの初期化処理
//==================================================================================
void CLight::Init(D3DXVECTOR3 LightVec, int nIdx)
{
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();//ライトの情報
	D3DXVECTOR3 vecDir;

	m_nIdx = nIdx;

	//ライトの情報をクリアにする
	ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_aLight.Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_aLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	SetLightVec(LightVec);

	//ライトを設定する
	pDevice->SetLight(nIdx, &m_aLight);

	//ライトを有効にする
	pDevice->LightEnable(nIdx, TRUE);
}
//==================================================================================
//	ライトの終了処理
//==================================================================================
void CLight::Uninit(void)
{

}
//==================================================================================
//	ライトの更新処理
//==================================================================================
void CLight::Update(void)
{
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();//ライトの情報

	/*CManager* pManager = CManager::GetManager();*/
	CInputKeyboard* pKey = pManager->GetInputKeyboard();

	//ライトを設定する
	pDevice->SetLight(m_nIdx, &m_aLight);
}

//==================================================================================
//	ライトのベクトル設定処理
//==================================================================================
void CLight::SetLightVec(D3DXVECTOR3 vec)
{
	D3DXVECTOR3 vecDir;

	//ライトの方向を設定
	vecDir = vec;
	D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトルを正規化する
	m_aLight.Direction = vecDir;
	/*m_aLight[Number].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/
}