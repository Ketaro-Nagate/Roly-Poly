//==================================================================================
//
//  2Dオブジェクト [object2D.cpp]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "object2D.h"
#include "renderer.h"
#include "manager.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define POLYGON_SIZEX 640				// オブジェクトの横幅
#define POLYGON_SIZEY 360				// オブジェクトの縦幅

//**********************************************************************************
// グローバル変数宣言
//**********************************************************************************

//==================================================================================
// コンストラクタ
//==================================================================================
/*CObject2D::CObject2D()
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;

	m_col = WHITE;
	m_pos = NULL_D3DXVECTOR3;
	m_rot = 0.0f;
}*/

//==================================================================================
// コンストラクタ
//==================================================================================
CObject2D::CObject2D(int nPrioryty):CObject(nPrioryty)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;

	m_col = WHITE;
	m_pos = NULL_D3DXVECTOR3;
	m_rot = 0.0f;
}

//==================================================================================
// デストラクタ
//==================================================================================
CObject2D::~CObject2D()
{

}

//==================================================================================
// 生成
//==================================================================================
CObject2D* CObject2D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// オブジェクト2Dの作成
	CObject2D* pObject2D = new CObject2D;

	// 初期化
	pObject2D->Init(pos, fWidth, fHeight);

	return pObject2D;

	//--- 生成に失敗 ---/
	return NULL;
}

//==================================================================================
// 初期化処理
//==================================================================================
HRESULT CObject2D::Init(void)
{
	CManager * pManager = CManager::GetManager();

	CRenderer* Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx;

	m_col = WHITE;
	m_rot = D3DX_PI * 0.0f;

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0,0,(void**)&pVtx,0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - POLYGON_SIZEX, m_pos.y - POLYGON_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + POLYGON_SIZEX, m_pos.y - POLYGON_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - POLYGON_SIZEX, m_pos.y + POLYGON_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + POLYGON_SIZEX, m_pos.y + POLYGON_SIZEY, 0.0f);

	fPolygonSize = D3DXVECTOR2(POLYGON_SIZEX, POLYGON_SIZEY);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
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
// 初期化処理(オーバーロード)
//==================================================================================
HRESULT CObject2D::Init(D3DXVECTOR3 pos,float fWidth, float fHeight)
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

	m_col = WHITE;
	m_rot = D3DX_PI * 0.0f;

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 座標取得
	m_pos = pos;

	//頂点座標の設定
	fPolygonSize = D3DXVECTOR2(fWidth, fHeight);
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth * 0.5f, m_pos.y - fHeight * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth * 0.5f, m_pos.y - fHeight * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth * 0.5f, m_pos.y + fHeight * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth * 0.5f, m_pos.y + fHeight * 0.5f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
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
void CObject2D::Uninit(void)
{
	// バッファの破棄
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
void CObject2D::Update(void)
{
	
}

//==================================================================================
// 描画処理
//==================================================================================
void CObject2D::Draw(void)
{
	CManager* pManager = CManager::GetManager();

	CRenderer* Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//オブジェクトの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//==================================================================================
// 描画処理
//==================================================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// テクスチャ割り当て
	m_pTexture = pTexture;
}

//==================================================================================
// 座標指定
//==================================================================================
void CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	// 座標更新
	m_pos = pos;

	VERTEX_2D* pVtx;		// 頂点バッファへのポインタ

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fPolygonSize.x, m_pos.y - fPolygonSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fPolygonSize.x, m_pos.y - fPolygonSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fPolygonSize.x, m_pos.y + fPolygonSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fPolygonSize.x, m_pos.y + fPolygonSize.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==================================================================================
// 色指定
//==================================================================================
void CObject2D::SetColor(const D3DXCOLOR col)
{
	// 色変更
	m_col = col; 

	VERTEX_2D* pVtx;		// 頂点バッファへのポインタ

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col + D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	pVtx[1].col = m_col + D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
	pVtx[2].col = m_col + D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);
	pVtx[3].col = m_col + D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}