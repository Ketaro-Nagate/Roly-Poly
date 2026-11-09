//=====================================================================
//
// DebugProc.cppのヘッダファイル [DebugProc.h]
// Author : 
// 
//=====================================================================
#ifndef _debugProc_H_
#define _debugProc_H_

//*********************************************************************
// インクルードファイル
//*********************************************************************
#include "main.h"

//*********************************************************************
// クラス定義
//*********************************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();

	void Init(void);
	void Uninit(void);
	void Draw(void);
	void PrintDebug(const char* fmt, ...);
	void SwitchDebugProc(void);

private:
	LPD3DXFONT m_pFontDebug = NULL;		// フォント
	char m_aStrDebug[2048];				// 
	bool m_bDraw;						// 描画するか
};

#endif