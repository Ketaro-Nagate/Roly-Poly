//==================================================================================
//
// テクスチャの管理クラス [texture.h]
// Author : NAGATE
//
//==================================================================================
#ifndef _TEXTURE_H_

#define _TEXTURE_H_

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_TEXTURE		128			// 読み込めるテクスチャの最大数

//**********************************************************************************
// クラス定義
//**********************************************************************************
class CTexture
{
public:
	// テクスチャのインデックス
	typedef enum
	{
		TEX_EFFECT = 0,		// エフェクト
		TEX_WALL,			// 壁
		TEX_FIELD,			// 地面
		TEX_WARNING,		// 敵の警告表示
		TEX_TITLE,			// タイトルロゴ
		TEX_UNDER,			// 地面の中
		TEX_FOOD,			// ごはん
		TEX_MAX
	}TEXNUM;

	CTexture();
	~CTexture();

	static CTexture* GetInstance(void);
	HRESULT Load(void);
	int Load(const char* pFileName);
	void Unload(void);
	int  Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];	// テクスチャへのポインタ
	char* m_apFileName[MAX_TEXTURE];				// 読み込んだテクスチャのファイル名
	static int m_nNumAll;							// 読み込んだテクスチャの総数
};
#endif // !_TEXTURE_H_
