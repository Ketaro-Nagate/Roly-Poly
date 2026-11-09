//==================================================================================
//
// オブジェクト３Ⅾ [object3D.cpp]
// Author : 長手　桂太朗
//
//==================================================================================
#include "object3D.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "util.h"
#include "texture.h"

//**********************************************************************************
//マクロ定義
//**********************************************************************************
#define WALLTEX_SPLIT		// 壁のテクスチャを分割するか

//**********************************************************************************
// グローバル変数
//**********************************************************************************
float g_UpVtx = 0.0f;

//==================================================================================
// コンストラクタ
//==================================================================================
CObject3D::CObject3D()
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_col = WHITE;
	m_pos = NULL_D3DXVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = NULL_D3DXVECTOR3;
}

//==================================================================================
// デストラクタ
//==================================================================================
CObject3D::~CObject3D()
{
	
}

//==================================================================================
// 生成( サイズ、テクスチャ指定 )
//==================================================================================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const LPDIRECT3DTEXTURE9 pTexture, const float fTexSplit)
{
	// オブジェクト3Dの作成
	CObject3D* pObject3D = new CObject3D;

	// 初期化
	pObject3D->Init(pos, size, pTexture,fTexSplit);

	return pObject3D;

	//--- 生成に失敗 ---/
	return NULL;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CObject3D::Init(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\hq720.jp", &m_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - 100.0f, m_pos.y, m_pos.z + 100.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + 100.0f, m_pos.y, m_pos.z + 100.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - 100.0f, m_pos.y, m_pos.z - 100.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + 100.0f, m_pos.y, m_pos.z - 100.0f);

	m_size = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 法線を求める
	D3DXVECTOR3 vec1, vec2, vec3, vec4;
	vec1 = pVtx[1].pos - pVtx[0].pos;
	vec2 = pVtx[2].pos - pVtx[0].pos;
	vec3 = pVtx[1].pos - pVtx[3].pos;
	vec4 = pVtx[2].pos - pVtx[3].pos;

	// 外積
	D3DXVec3Cross(&pVtx[0].nor, &vec1, &vec2);
	D3DXVec3Cross(&pVtx[3].nor, &vec4, &vec3);
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;

	// 正規化
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

	//頂点カラーの設定
	pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================================
// 初期化処理 (床用)
//==================================================================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	CManager *pManager = CManager::GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\field002.jpg", &m_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 位置の設定
	m_pos = pos;

	// 対角線、角度の設定
	m_size = size;
	m_fLength = sqrtf(size.x * size.x + size.z * size.z) * 0.5f;	// 対角線の長さ
	m_fAngle = (float)atan2(size.x, size.z);						// 対角線の角度

	// 頂点座標の設定
	pVtx[0].pos.x = sinf(m_rot.y + m_fAngle + D3DX_PI) * m_fLength;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = cosf(m_rot.y + m_fAngle + D3DX_PI) * m_fLength;

	pVtx[1].pos.x = sinf(m_rot.y - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_size.y;
	pVtx[1].pos.z = cosf(m_rot.y - m_fAngle) * m_fLength;

	pVtx[2].pos.x = sinf(m_rot.y + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = cosf(m_rot.y + D3DX_PI - m_fAngle) * m_fLength;

	pVtx[3].pos.x = sinf(m_rot.y + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_size.y;
	pVtx[3].pos.z = cosf(m_rot.y + m_fAngle) * m_fLength;

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 法線を求める
	D3DXVECTOR3 vec1, vec2, vec3, vec4;
	vec1 = pVtx[1].pos - pVtx[0].pos;
	vec2 = pVtx[2].pos - pVtx[0].pos;
	vec3 = pVtx[1].pos - pVtx[3].pos;
	vec4 = pVtx[2].pos - pVtx[3].pos;

	// 外積
	D3DXVec3Cross(&pVtx[0].nor, &vec1, &vec2);
	D3DXVec3Cross(&pVtx[3].nor, &vec4, &vec3);
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;

	// 正規化
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, m_fLength / 100.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fLength / 100.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fLength / 100.0f, m_fLength / 100.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================================
// 初期化処理( 床用 / テクスチャ指定 )
//==================================================================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture,float fTexSplit)
{
	// デバイスの取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	m_pTexture = pTexture;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置の設定
	m_pos = pos;

	// 対角線、角度の設定
	m_size = size;
	m_fLength = sqrtf(size.x * size.x + size.z * size.z) * 0.5f;	// 対角線の長さ
	m_fAngle = (float)atan2(size.x, size.z);						// 対角線の角度

	// 分割数が１
	if (fTexSplit == 1.0f)
	{
		fTexSplit = m_fLength;
	}


	// 頂点座標の設定
	pVtx[1].pos.x = sinf(m_rot.y - m_fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = cosf(m_rot.y - m_fAngle + D3DX_PI) * m_fLength;

	pVtx[0].pos.x = sinf(m_rot.y + m_fAngle) * m_fLength;
	pVtx[0].pos.y = m_size.y;
	pVtx[0].pos.z = cosf(m_rot.y + m_fAngle) * m_fLength;

	pVtx[2].pos.x = sinf(m_rot.y - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_size.y;
	pVtx[2].pos.z = cosf(m_rot.y - m_fAngle) * m_fLength;

	pVtx[3].pos.x = sinf(m_rot.y + m_fAngle + D3DX_PI) * m_fLength;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = cosf(m_rot.y + m_fAngle + D3DX_PI) * m_fLength;

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 法線を求める
	D3DXVECTOR3 vec1, vec2, vec3, vec4;
	vec1 = pVtx[1].pos - pVtx[0].pos;
	vec2 = pVtx[2].pos - pVtx[0].pos;
	vec3 = pVtx[1].pos - pVtx[3].pos;
	vec4 = pVtx[2].pos - pVtx[3].pos;

	// 外積
	D3DXVec3Cross(&pVtx[0].nor, &vec1, &vec2);
	D3DXVec3Cross(&pVtx[3].nor, &vec4, &vec3);
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;

	// 正規化
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, m_fLength / fTexSplit);
	pVtx[2].tex = D3DXVECTOR2(m_fLength / fTexSplit, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fLength / fTexSplit, m_fLength / fTexSplit);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================================
// 初期化処理 ( 壁用 / テクスチャ指定 )
//==================================================================================
HRESULT CObject3D::InitWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture)
{
	// デバイスの取得
	CManager *pManager = CManager::GetManager();
	CRenderer *pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャ読み込み
	m_pTexture = pTexture;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 位置の設定
	m_pos = pos;

	// 対角線、角度の設定
	m_rot = rot;
	m_size = size;
	m_fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;	// 対角線の長さ
	m_fAngle = (float)atan2(size.x, size.y);						// 対角線の角度

	// 頂点座標の設定
	pVtx[3].pos.x = sinf(m_rot.x + m_fAngle + D3DX_PI) * m_fLength;
	pVtx[3].pos.y = cosf(m_rot.x + m_fAngle + D3DX_PI) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(m_rot.x - m_fAngle) * m_fLength;
	pVtx[2].pos.y = cosf(m_rot.x - m_fAngle) * m_fLength;
	pVtx[2].pos.z = m_size.z;

	pVtx[1].pos.x = sinf(m_rot.x + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = cosf(m_rot.x + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[0].pos.x = sinf(m_rot.x + m_fAngle) * m_fLength;
	pVtx[0].pos.y = cosf(m_rot.x + m_fAngle) * m_fLength;
	pVtx[0].pos.z = m_size.z;

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 法線を求める
	D3DXVECTOR3 vec1, vec2, vec3, vec4;
	vec1 = pVtx[1].pos - pVtx[0].pos;
	vec2 = pVtx[2].pos - pVtx[0].pos;
	vec3 = pVtx[1].pos - pVtx[3].pos;
	vec4 = pVtx[2].pos - pVtx[3].pos;

	// 外積
	D3DXVec3Cross(&pVtx[0].nor, &vec1, &vec2);
	D3DXVec3Cross(&pVtx[3].nor, &vec4, &vec3);
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2.0f;

	// 正規化
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

#ifndef WALLTEX_SPLIT
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, m_fLength / 100.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fLength / 100.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fLength / 100.0f, m_fLength / 100.0f);
#else
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

#endif // !WALLTEX_SPLIT

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================================
// 終了処理
//==================================================================================
void CObject3D::Uninit(void)
{
	//テクスチャの破棄
	/*if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}*/

	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトを破棄
	CObject::Release();
}

//==================================================================================
// 更新処理
//==================================================================================
void CObject3D::Update(void)
{
	CManager* pManager = CManager::GetManager();
	CInputKeyboard* pKey = pManager->GetInputKeyboard();

	if (pKey->GetPress(DIK_1))
	{
		m_rot.y += 0.01f;
	}

	if (pKey->GetPress(DIK_2))
	{
		m_rot.y -= 0.01f;
	}

	RotCorrection(&m_rot);
}

//==================================================================================
// 描画処理
//==================================================================================
void CObject3D::Draw(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTex = CManager::GetManager()->GetTexture();

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	LPDIRECT3DTEXTURE9 pTextureMT;

	//pTextureMT = pRenderer->GetTextureMT();
	//pDevice->SetTexture(0, pTextureMT);
}

//==================================================================================
// 高さ取得
//==================================================================================
float CObject3D::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ
	D3DXVECTOR3 aVtxPos[4];		// 

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--- 頂点の位置情報を置き換え ---//
	for (int nCount = 0; nCount < 4; nCount++)
	{
		D3DXVec3TransformCoord(&aVtxPos[nCount], &pVtx[nCount].pos, &m_mtxWorld);
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//--- ポリゴンの上にposがあるか判定 ---//
	if (InsideTriangle(pos, &aVtxPos[0],true))
	{// 三角形の内側にいる
		D3DXVECTOR3 vec1, vec2, nor;
		vec1 = aVtxPos[1] - aVtxPos[0];
		vec2 = aVtxPos[2] - aVtxPos[0];
		float fHeight = 0.0f;			// 立っている高さ

		// ２つのベクトルから法線を求める
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// 法線を正規化する
		D3DXVec3Normalize(&nor, &nor);

		// 高さを求める
		if (nor.y != 0.0f)
		{
			fHeight = (-((pos.x - aVtxPos[0].x) * nor.x) +
					   -((pos.z - aVtxPos[0].z) * nor.z) +
						(aVtxPos[0].y * nor.y)) / nor.y;
		}

		return fHeight;
	}

	//--- 反対の三角形もテスト ---//
	if (InsideTriangle(pos, &aVtxPos[1],false))
	{// 三角形の内側にいる
		D3DXVECTOR3 vec1, vec2, nor;
		vec1 = aVtxPos[2] - aVtxPos[3];
		vec2 = aVtxPos[1] - aVtxPos[3];
		float fHeight = 0.0f;			// 立っている高さ

		// ２つのベクトルから法線を求める
		D3DXVec3Cross(&nor, &vec2, &vec1);

		// 法線を正規化する
		D3DXVec3Normalize(&nor, &nor);

		// 高さを求める
		if (nor.y != 0.0f)
		{
			fHeight = (-((pos.x - aVtxPos[3].x) * nor.x) +
					   -((pos.z - aVtxPos[3].z) * nor.z) +
						(aVtxPos[3].y * nor.y)) / nor.y;
		}

		return fHeight;
	}

	return 0.0f;
}