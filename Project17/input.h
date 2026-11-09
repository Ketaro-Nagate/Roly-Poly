//==================================================================================
//
//  デバイス入力 [input.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define NUM_KEY_MAX			256		// キーの最大数
#define NUM_MOUSE_BUTTON	8		// マウスボタンの最大数

//**********************************************************************************
// 列挙型定義
//**********************************************************************************
typedef enum
{
	JOYKEY_UP = 0,		//[0] 十字キー（↑）
	JOYKEY_DOWN,		//[1] 十字キー（↓）
	JOYKEY_RIGHT,		//[2] 十字キー（→）
	JOYKEY_LEFT,		//[3] 十字キー（←）
	JOYKEY_START,		//[4] スタートボタン
	JOYKEY_BACK,		//[5] Backボタン
	JOYKEY_LTHUMB,		//[6] Lボタン
	JOYKEY_RTHUMB,		//[7] Rボタン
	JOYKEY_LSHOULDER,	//[8] ZLボタン
	JOYKEY_RSHOULDER,	//[9] ZRボタン
	DUMMY1,
	DUMMY2,
	JOYKEY_A,			//[12]Aボタン
	JOYKEY_B,			//[13]Bボタン
	JOYKEY_X,			//[14]Xボタン
	JOYKEY_Y,			//[15]Yボタン
	JOYKEY_MAX			//
}JOYKEY;

typedef enum
{
	MC_LEFT = 0,
	MC_RIGHT,
	MC_MAX
}MOUSEKEY;

//**********************************************************************************
// クラス定義
//**********************************************************************************
//*** デバイス入力基底クラス ***//
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;			//Directinputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;			//入力デバイス(キーボード)へのポインタ
};

//*** キーボードクラス定義 ***//
class CInputKeyboard:public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int Key);
	bool GetTrigger(int Key);

private:
	BYTE m_aKeyStatePress[NUM_KEY_MAX];		// キーボードへのプレス入力情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードへのトリガー入力情報

};

//*** マウスクラス定義 ***//
class CInputMouse :public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(MOUSEKEY Key);
	bool GetTrigger(MOUSEKEY Key);
	POINT GetScreenPos(void) { return m_ScrPos; }
	POINT GetWindowPos(void) { return m_WinPos; }

private:
	DIMOUSESTATE2 m_MouseState;				// マウスの情報
	BYTE m_Press[NUM_MOUSE_BUTTON];			// マウスのプレス情報
	BYTE m_Trigger[NUM_MOUSE_BUTTON];		// マウスのトリガー情報
	POINT m_ScrPos;							// スクリーン (デスクトップ)上のマウスカーソルの位置
	POINT m_WinPos;							// ウィンドウ上の位置
	HWND m_hWnd;							// ウィンドウ情報

};

//**********************************************************************************
// プロトタイプ宣言
//**********************************************************************************a
XINPUT_STATE* GetStick(void);

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypad(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);

#endif
