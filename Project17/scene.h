//==================================================================================
//
//  ゲームマネージャー [manager.h]
//  Author : NAGATE
//
//==================================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "object.h"
#include "camera.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define CSCENE_COBJECT

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CLight;
class CPlayer;
class CProp;
class CScore;

//**********************************************************************************
// クラス定義
//**********************************************************************************
#ifndef CSCENE_COBJECT		// CObject継承あり
class CScene :public CObject
#else
class CScene				// CObject継承なし
#endif // !CSCENE_COBJECT
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RANKING,
		MODE_MAX
	}MODE;

	CScene();
	CScene(MODE mode);
	virtual ~CScene();

	static CScene* Create(MODE mode);
	virtual HRESULT Init(MODE mode);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual MODE GetMode(void) { return m_mode; }

	virtual CPlayer* GetPlayer(){ return nullptr; }

private:
	MODE m_mode;			// 現在のシーンモード

};

#endif // !_SCENE_H_
