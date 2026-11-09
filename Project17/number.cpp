//==================================================================================
//
//  オブジェクト [number.cpp]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define POLYGON_SIZEX	50				// レンダラーの横幅
#define POLYGON_SIZEY	50				// レンダラーの縦幅
#define ANIM_PATTERN	8				// アニメーションのパターン数
#define ONE_ANIM_SIZE	0.125f			// 1パターンの幅
#define SPEED	5.0f;			// 速さ

//**********************************************************************************
// 静的メンバ変数宣言
//**********************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//==================================================================================
// コンストラクタ
//==================================================================================
CNumber::CNumber()
{
	m_nNumber = 0;
	m_col = WHITE;
}

//==================================================================================
// デストラクタ
//==================================================================================
CNumber::~CNumber()
{

}

//==================================================================================
// 読み込み
//==================================================================================
HRESULT CNumber::Load(void)
{
	CManager* pManager = CManager::GetManager();

	CRenderer* Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &m_pTexture);

	return S_OK;
}

//==================================================================================
// 破棄
//==================================================================================
void CNumber::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================================================================
// 生成(0指定)
//==================================================================================
CNumber* CNumber::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	
	//ナンバーインスタンスの作成
	CNumber* pNumber = new CNumber;

	// テクスチャ読み込み
	Load();

	// 初期化
	pNumber->Init(pos, fWidth, fHeight);

	pNumber->SetNumber(0);

	return pNumber;
}

//==================================================================================
// 生成 色指定あり
//==================================================================================
CNumber *CNumber::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const D3DXCOLOR col)
{
	//ナンバーインスタンスの作成
	CNumber *pNumber = new CNumber;

	// テクスチャ読み込み
	Load();

	// 初期化
	pNumber->Init(pos, fWidth, fHeight, col);

	pNumber->SetNumber(0);

	return pNumber;
}

//==================================================================================
// 生成(ナンバー指定)
//==================================================================================
CNumber* CNumber::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight,int nNumber)
{
	//ナンバーインスタンスの作成
	CNumber* pNumber = new CNumber;

	// 初期化
	pNumber->Init(pos, fWidth, fHeight, nNumber);

	return pNumber;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CNumber::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CManager* pManager = CManager::GetManager();

	CRenderer* Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = WHITE;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CNumber::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nNumber)
{
	CManager* pManager = CManager::GetManager();

	CRenderer* Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = WHITE;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================================
// 初期化処理 (色指定)
//==================================================================================
HRESULT CNumber::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const D3DXCOLOR col)
{
	CManager *pManager = CManager::GetManager();

	CRenderer *Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	m_col = col;

	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================================
// 終了処理
//==================================================================================
void CNumber::Uninit(void)
{
	// バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Unload();
}

//==================================================================================
// 更新処理
//==================================================================================
void CNumber::Update(void)
{

}

//==================================================================================
// 描画処理
//==================================================================================
void CNumber::Draw(void)
{
	CManager* pManager = CManager::GetManager();

	CRenderer* Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// 加算合成開始
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//オブジェクトの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 加算合成終了
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==================================================================================
// 値設定処理
//==================================================================================
void CNumber::SetNumber(int nNumber)
{
	//頂点情報へのポインタ
	VERTEX_2D* pVtx;

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CNumber::SetColor(D3DXCOLOR col)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	m_col = col;

	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
