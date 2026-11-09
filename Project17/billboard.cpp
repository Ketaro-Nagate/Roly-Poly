//==================================================================================
//
// ビルボード[billboard.cpp]
// Author : 長手　桂太朗
//
//==================================================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//==================================================================================
//マクロ定義 
//==================================================================================
#define BILLBOARD_SIZE (50.0f)	//ビルボードの大きさ

//==================================================================================
// コンストラクタ
//==================================================================================
CBillboard::CBillboard()
{
	m_batest = false;
	m_bZtest = false;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_col = WHITE;
}

//==================================================================================
// コンストラクタ(オーバーロード)
//==================================================================================
CBillboard::CBillboard(int nPrioryty) :CObject(nPrioryty)
{
	m_batest = false;
	m_bZtest = false;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_col = WHITE;
}

//==================================================================================
// デストラクタ
//==================================================================================
CBillboard::~CBillboard()
{

}

//==================================================================================
// 生成
//==================================================================================
CBillboard* CBillboard::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexIdx, int nPrioryty)
{
	CBillboard* pBillboard = new CBillboard(nPrioryty);

	pBillboard->Init(pos, fWidth, fHeight, nTexIdx);

	return pBillboard;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CBillboard::Init(void)
{
	//デバイスの取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\0381.png", &m_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_col = WHITE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f,240.0f);
	m_bZtest = true;
	m_batest = true;

	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(- BILLBOARD_SIZE,  BILLBOARD_SIZE * 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ BILLBOARD_SIZE,  BILLBOARD_SIZE * 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- BILLBOARD_SIZE, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ BILLBOARD_SIZE, 0.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
// 初期化処理( オーバーロード )
//==================================================================================
HRESULT CBillboard::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexIdx)
{
	//デバイスの取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	//テクスチャの読み込み
	m_pTexture = pTexture->GetAddress(nTexIdx);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 引数の保存
	m_col = WHITE;
	m_pos = pos;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_bZtest = false;
	m_batest = false;

	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth * 0.5f, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth * 0.5f, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
// 終了処理
//==================================================================================
void  CBillboard::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CObject::Release();
}

//==================================================================================
// 更新処理
//==================================================================================
void  CBillboard::Update(void)
{
	
}

//==================================================================================
// 描画処理
//==================================================================================
void CBillboard::Draw(void)
{
	//デバイスの取得
	CManager* pManager = CManager::GetManager();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス

	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	if (m_bZtest == false)
	{
		// Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zテストの有効／無効
	}
	else if (m_bZtest == true)
	{
		// Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zテストの有効／無効
	}

	if (m_batest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	//アルファテストの有効
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	}
	else if (m_batest == false)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//アルファテストの無効

		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	}

	// 加算合成開始
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zテストの有効／無効

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	//アルファテストの無効
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 加算合成終了
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==================================================================================
// サイズ設定
//==================================================================================
void CBillboard::SetSize(float fWidth, float fHeight)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth * 0.5f, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth * 0.5f, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==================================================================================
// サイズ設定
//==================================================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_col = col;

	VERTEX_3D* pVtx;	//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}