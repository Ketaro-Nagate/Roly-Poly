#ifndef _CAMERA_H_
#define _CAMERA_H_

//==================================================
//
//	ファイルインクルード
//
//==================================================
#include "main.h"

//==================================================
//	前方宣言
//==================================================
class CInputKeyboard;

//==================================================
// クラス定義
//==================================================
class CCamera
{
public:
	typedef enum //カメラの操作タイプ
	{
		CONTROL_TYPE_NORMAL = 0,	// 通常
		CONTROL_TYPE_LOCK,			// ターゲットロック
		CONTROL_TYPE_MAX

	}CONTROL_TYPE;

	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void Control(CInputKeyboard* pKey);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetVecU(void) { return m_vecU; }
	float GetTargetRot(void) { return m_fPTRot; }
	void SwitchControl(bool bControl) { m_bControl = bControl; }
	void ResetCamera(void);

private:
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXVECTOR3 m_rot;			// 角度
	float m_VRDistance;			// 対象物との距離
	float m_fPTRot;				// ターゲット視点時に使用するPlayer->Targetの角度

	D3DXVECTOR3 m_posVDest;		// 目的の視点
	D3DXVECTOR3 m_posRDest;		// 目的の注視点
	D3DXVECTOR3 m_rotDest;		// 目的の角度
	float m_fofset;				// 対象と注視点の差

	bool m_bControl;			// カメラの操作モードに切り替え
};

#endif // !_CAMERA_H_

