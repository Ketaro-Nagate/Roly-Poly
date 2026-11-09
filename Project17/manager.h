//==================================================================================
//
//  ゲームマネージャー [manager.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"
#include "camera.h"
#include "scene.h"
#include "input.h"

// シーン代替読み込み
#include "light.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CRenderer;
class CInputKeyboard;
class CDebugProc;
class CInputMouse;
class CObject3D;
class CObjectX;
class CObject2D;
class CTexture;

//**********************************************************************************
// マネージャークラス定義
//**********************************************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CRenderer* GetRenderer(void);
	CInputKeyboard* GetInputKeyboard(void);
	static CManager* GetManager(void);
	CInputMouse* GetInputMouse(void);
	CDebugProc* GetDebugProc(void);
	CScene* GetScene(void) { return m_pScene; }
	CCamera* GetCamera(void) { return m_pCamera; }
	CTexture* GetTexture(void) { return m_pTexture; }
	bool GetPause(void) { return m_bPause; }
	void SetPause(bool bPause) { m_bPause = bPause; }

	void SetMode(CScene::MODE mode);
	CScene::MODE GetMode(void);

private:
	CRenderer* m_pRenderer;				// レンダラーインスタンスへのポインタ
	CInputKeyboard* m_pInputKeyboard;	// キーボードインスタンスへのポインタ
	CInputMouse* m_pInputMouse;			// マウスインスタンスへのポインタ
	CDebugProc* m_pDebugProc;			// デバッグ表示
	CCamera* m_pCamera;					// カメラ
	CTexture* m_pTexture;				// テクスチャ管理

	static bool m_bFade;				// フェード中か
	CScene* m_pScene;					// 現在のシーン

	bool m_bPause;						// ポーズ中か
	bool m_bInGame;						// リスタート
};

#endif // !_MANAGER_H_

