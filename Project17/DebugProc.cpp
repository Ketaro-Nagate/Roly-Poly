//=====================================================================
//
// DebugProc [DebugProc.cpp]
// Author : Nagate
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "debugProc.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************

//=====================================================================
// コンストラクタ
//=====================================================================
CDebugProc::CDebugProc()
{
	memset(&m_aStrDebug[0], 0, sizeof(m_aStrDebug));
	m_pFontDebug = NULL;
	m_bDraw = true;
}

//=====================================================================
// デストラクタ
//=====================================================================
CDebugProc::~CDebugProc()
{

}

//=====================================================================
// 初期化処理
//=====================================================================
void CDebugProc::Init(void)
{
#ifdef _DEBUG
	CManager* pManager = CManager::GetManager();

	CRenderer* Renderer = pManager->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	D3DXFONT_DESC fontDesc = {
		30,								// Height
		0,								// Width (0 = 自動)
		FW_NORMAL,						// Weight
		1,								// MipLevels
		FALSE,							// Italic
		DEFAULT_CHARSET,				// CharSet
		OUT_DEFAULT_PRECIS,				// OutputPrecision
		CLIP_DEFAULT_PRECIS,			// Quality
		DEFAULT_PITCH | FF_DONTCARE,	// PitchAndFamily
		""
	};

	D3DXCreateFontIndirect(pDevice, &fontDesc, &m_pFontDebug);

	ZeroMemory(&m_aStrDebug[0], sizeof(m_aStrDebug));
#endif
}

//=====================================================================
// 終了処理
//=====================================================================
void CDebugProc::Uninit(void)
{
#ifdef _DEBUG
	if (m_pFontDebug != NULL)
	{
		m_pFontDebug->Release();
		m_pFontDebug = NULL;
	}
#endif
}

//=====================================================================
// 描画処理
//=====================================================================
void CDebugProc::Draw(void)
{
#ifdef _DEBUG
		RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		m_pFontDebug->DrawText(NULL, &m_aStrDebug[0], -1, &rect, DT_LEFT, WHITE);

		ZeroMemory(&m_aStrDebug[0], sizeof(m_aStrDebug));
#endif
}

//=====================================================================
// 入力
//=====================================================================
void CDebugProc::PrintDebug(const char* fmt, ...)
{
#ifdef _DEBUG
	if (m_bDraw == true)
	{// 消せるようにする
		va_list ap;
		char aBuffer[2048] = {};
		char aValue[64] = {};

		va_start(ap, fmt);

		for (int i = 0; i < strlen(fmt); i++)
		{
			if (fmt[i] == '%')
			{
				switch (fmt[i + 1])
				{
				case 'd':
					sprintf(aValue, "%d", va_arg(ap, int));
					break;

				case 'f':
					sprintf(aValue, "%f", (float)va_arg(ap, double));
					break;

				case 'c':
					sprintf(aValue, "%c", va_arg(ap, char));
					break;

				case 's':
					sprintf(aValue, "%s", va_arg(ap, char*));
					break;
				}
				strcat(aBuffer, aValue);
				i++;
			}
			else
			{
				aBuffer[strlen(aBuffer)] = fmt[i];
			}
		}

		va_end(ap);

		strcat(&m_aStrDebug[0], &aBuffer[0]);
	}
#endif
}

//=====================================================================
// 表示切り替え
//=====================================================================
void CDebugProc::SwitchDebugProc(void)
{
	m_bDraw ^= true;
}