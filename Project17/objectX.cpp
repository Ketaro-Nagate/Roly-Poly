//==================================================================================
//
//	3Dモデル[model.cpp]
//
//==================================================================================
//==================================================================================
//	ファイルインクルード
//==================================================
#include "objectX.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"

//==================================================
//	マクロ定義
//==================================================

//==================================================
//	構造体
//==================================================

//==================================================================================
// コンストラクタ
//==================================================================================
CObjectX::CObjectX()
{
	m_pos = NULL_D3DXVECTOR3;
	m_rot = NULL_D3DXVECTOR3;
	m_pBuffMat = NULL;
	m_dwNumMat = NULL;
	m_pMesh = NULL;
	memset(m_aTexture, 0, sizeof(m_aTexture));
	m_bScaling = false;
}

//==================================================================================
// デストラクタ
//==================================================================================
CObjectX::~CObjectX()
{
	
}

//==================================================================================
// 生成
//==================================================================================
CObjectX* CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// オブジェクト3Dの作成
	CObjectX* pObjectX = new CObjectX;

	// 引数の座標を代入
	pObjectX->SetPos(pos);

	// 初期化
	pObjectX->Init();

	return pObjectX;

	//--- 生成に失敗 ---/
	return NULL;
}

//==================================================================================
// 生成( オーバーロード )
//==================================================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName)
{
	// オブジェクト3Dの作成
	CObjectX* pObjectX = new CObjectX;

	// 初期化
	pObjectX->Init(pos, rot, pFileName);

	return pObjectX;

	//--- 生成に失敗 ---/
	return NULL;
}

//==================================================================================
//	3Dモデルの初期化
//==================================================================================
HRESULT CObjectX::Init(void)
{
	// デバイスを取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイル読み込み
	D3DXLoadMeshFromX("data\\MODEL\\airplane000.x",
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_pBuffMat,
					NULL,
					&m_dwNumMat,
					&m_pMesh);

	D3DXMATERIAL* pMat;

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMdl = 0; nCntMdl < m_dwNumMat; nCntMdl++)
	{//テクスチャファイルを読み込む
		if (pMat[nCntMdl].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMdl].pTextureFilename, &m_aTexture[nCntMdl]);
		}

	}

	return S_OK;
}

//==================================================================================
//	3Dモデルの初期化( オーバーロード )
//==================================================================================
HRESULT CObjectX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFileName)
{
	m_pos = pos;
	m_rot = rot;

	// デバイスを取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイル読み込み
	D3DXLoadMeshFromX(pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	D3DXMATERIAL* pMat;

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMdl = 0; nCntMdl < m_dwNumMat; nCntMdl++)
	{//テクスチャファイルを読み込む
		if (pMat[nCntMdl].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMdl].pTextureFilename, &m_aTexture[nCntMdl]);
		}

	}

	return S_OK;
}

//==================================================================================
//	3Dモデルの終了処理
//==================================================================================
void CObjectX::Uninit(void)
{
	// メッシュ破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	CObject::Release();
}

//==================================================================================
//	3Dモデルの更新処理
//==================================================================================
void CObjectX::Update(void)
{

}
//==================================================================================
//	3Dモデルの描画処理
//==================================================================================
void CObjectX::Draw(void)
{
	if (m_bScaling == false)
	{
		// デバイスを取得
		CManager* pManager = CManager::GetManager();
		CRenderer* pRenderer = pManager->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
		D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
		D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//法線の正規化
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// モデル(パーツ)の描画
			m_pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		for (int nCntTex = 0; nCntTex < MAX_XTEXTURE; nCntTex++)
		{
			if (m_aTexture[nCntTex] != NULL)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, m_aTexture[nCntTex]);
			}
		}
	}
	else
	{
		// サイズ変更描画
		DrawScaling(m_Scaling);
	}
}

//==================================================================================
//	3Dモデルの描画処理
//==================================================================================
void CObjectX::DrawScaling(D3DXVECTOR3 size)
{
	// デバイスを取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// モデルを拡縮
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, size.x, size.y, size.z);
	m_mtxWorld = matScale;

	//法線の正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	for (int nCntTex = 0; nCntTex < MAX_XTEXTURE; nCntTex++)
	{
		if (m_aTexture[nCntTex] != NULL)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, m_aTexture[nCntTex]);
		}
	}
}