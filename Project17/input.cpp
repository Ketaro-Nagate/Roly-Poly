//==================================================================================
//
//  デバイス入力 [input.cpp]
//  Author : NAGATE
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "input.h"
#include "DebugProc.h"
#include "manager.h"
#include "renderer.h"

//**********************************************************************************
// 静的メンバ変数宣言
//**********************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//**********************************************************************************
// グローバル変数宣言
//**********************************************************************************
XINPUT_STATE g_joykeystate;				//ジョイパッドのボタンのプレス情報
XINPUT_STATE g_JoyStateTrigger;		//ジョイパッドのボタントリガー情報
XINPUT_STATE g_joystick;			//ジョイパッドのスティック情報
DIJOYSTATE pad_data;

//==================================================================
// 基底コンストラクタ
//==================================================================
CInput::CInput()
{
	m_pInput = NULL;
	m_pDevice = NULL;
}

//==================================================================
// 基底デストラクタ
//==================================================================
CInput::~CInput()
{

}

//==================================================================
//初期化処理
//==================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pDevice == NULL)
	{
		// Directinputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==================================================================
// 入力デバイスの終了処理
//==================================================================
void CInput::Uninit(void)
{
	//入力デバイス(キーボード)の破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//ダイレクトオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==================================================================
//キーボードのコンストラクタ
//==================================================================
CInputKeyboard::CInputKeyboard()
{
	memset(&m_aKeyStatePress, 0, sizeof(m_aKeyStatePress));
	memset(&m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
}

//==================================================================
//キーボードのデストラクタ
//==================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//==================================================================
//キーボードの初期化処理
//==================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==================================================================
//キーボードの終了処理
//==================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//==================================================================
//キーボードの更新処理
//==================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//キーボードのトリガー情報を取得
			m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];	//キーボードのプレス情報を取得
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーボードのアクセス権を獲得
	}
}

//==================================================================
//キーボードのプレス情報を取得
//==================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//==================================================================
//キーボードのトリガー情報を取得
//==================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==================================================================
// マウスのコンストラクタ
//==================================================================
CInputMouse::CInputMouse()
{
	memset(&m_MouseState, 0, sizeof(DIMOUSESTATE2));
	memset(&m_Press, 0, sizeof(m_Press));
	memset(&m_Trigger, 0, sizeof(m_Trigger));
}

//==================================================================
// マウスのデストラクタ
//==================================================================
CInputMouse::~CInputMouse()
{

}

//==================================================================
// マウスの初期化処理
//==================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	m_hWnd = hWnd;

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==================================================================
// マウスの終了処理
//==================================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//==================================================================
// マウスの更新処理
//==================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 MouseState;	// マウスの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		// マウスの入力情報を受け取る
		m_MouseState = MouseState;

		//--- マウスのボタン情報を受け取る ---//
		for (int nCount = 0; nCount < NUM_MOUSE_BUTTON; nCount++)
		{
			// マウスボタンのトリガー情報を取得
			m_Trigger[nCount] = (m_Press[nCount] ^ MouseState.rgbButtons[nCount]) & MouseState.rgbButtons[nCount];

			// マウスのプレス情報を取得
			m_Press[nCount] = MouseState.rgbButtons[nCount];
		}
	}
	else
	{
		m_pDevice->Acquire();	// マウスのアクセス権を獲得
	}

	// カーソルの位置情報を取得
	GetCursorPos(&m_ScrPos);

	GetCursorPos(&m_WinPos);

	ScreenToClient(m_hWnd, &m_WinPos);

	CManager* pManager = CManager::GetManager();

	CDebugProc* pDebugProc = pManager->GetDebugProc();

	/*pDebugProc->PrintDebug("%d\n",m_Press[0]);
	pDebugProc->PrintDebug("SCR POS : X %d, Y %d\n", m_ScrPos.x, m_ScrPos.y);
	pDebugProc->PrintDebug("WIN POS : X %d, Y %d\n", m_WinPos.x, m_WinPos.y);*/
}

//==================================================================
// マウスのクリック(プレス)
//==================================================================
bool CInputMouse::GetPress(MOUSEKEY Key)
{
	return(m_Press[Key] & 0x80) ? true : false;
}

//==================================================================
// マウスのクリック(トリガー)
//==================================================================
bool CInputMouse::GetTrigger(MOUSEKEY Key)
{
	return(m_Trigger[Key] & 0x80) ? true : false;
}

//==================================================================
//ジョイパッドの初期化処理
//==================================================================
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joykeystate, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//==================================================================
//ジョイパッドの終了処理
//==================================================================
void UninitJoypad(void)
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//==================================================================
//ジョイパッドの更新処理
//==================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeystate;	//ジョイパッドの入力情報

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeystate) == ERROR_SUCCESS)
	{
		//ジョイパッドのトリガー情報を取得
		g_JoyStateTrigger.Gamepad.wButtons = (g_joykeystate.Gamepad.wButtons ^ joykeystate.Gamepad.wButtons) & joykeystate.Gamepad.wButtons;	//パッドのトリガー情報
		g_joykeystate = joykeystate;	//ジョイパッドのプレス情報を保存
		g_joystick = joykeystate;	//左スティック情報を取得
	}

}

//==================================================================
//ジョイパッドのボタンのプレス情報を取得
//==================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return(g_JoyStateTrigger.Gamepad.wButtons & (0x01 << key) ? true : false);
}

//==================================================================
//ジョイパッドのプレス情報を取得
//==================================================================
bool GetJoypad(JOYKEY key)
{
	return(g_joykeystate.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//==================================================================
//ジョイパッドのスティック情報を取得
//==================================================================
XINPUT_STATE* GetStick(void)
{
	return &g_joystick;
}