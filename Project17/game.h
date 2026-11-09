//==================================================================================
//
// ゲームモードクラス [game.cpp]
// Author : Nagate
//
//==================================================================================
#ifndef _GAME_H_
#define _GAME_H_
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"
#include "manager.h"
#include <memory>
#include "input.h"

//**********************************************************************************
// 前方宣言
//**********************************************************************************
class CCamera;
class CLight;
class CPlayer;
class CObject3D;
class CProp;
class CEnemy;
class CEnemyManager;
class CScore;
class CLife;
class CStartUI;
class CItemManager;

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CGame :public CScene
{
public:
	typedef enum
	{
		GAMEMODE_PLAY = 0,			// プレイ中
		GAMEMODE_CLEAR,				// クリア
		GAMEMODE_DEFEAT,			// 失敗
		GAMEMODE_MAX
	}GAMEMODE;

	CGame();
	~CGame();

	HRESULT Init(CScene::MODE mode)override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPlayer* GetPlayer(void) override { return m_pPlayer; }
	CLight* GetLight(void) { return m_pLight[0]; }
	CScore* GetScore(void) { return m_pScore; }
	CProp* GetPropA(void) { return m_pProp; }
	CItemManager* GetItemManager(void) { return m_pItemManager; }
	void EndScene(CScene::MODE nextmode);

private:
	CLight* m_pLight[3];		// ライト
	CPlayer* m_pPlayer;			// プレイヤー
	CProp* m_pProp;				// オブジェクト配置
	CScore* m_pScore;			// スコア
	bool m_bEnd;				// シーン終了
	CScene::MODE m_NextScene;	// 次のシーンモード
	bool m_bPause;				// 重力
	CItemManager* m_pItemManager;	// アイテムマネージャー
	CObject2D* m_food;			// スコアテクスチャ
	CScore* m_timer;			// 残り時間
	int m_nCntGame;				// 


};

#endif // !_GAME_H_
