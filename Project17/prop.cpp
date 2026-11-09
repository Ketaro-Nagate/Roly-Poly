//==================================================================================
//
// 小物配置 [prop.cpp]
// Author : Nagate
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "prop.h"
#include "manager.h"
#include "renderer.h"
#include "item.h"
#include "wall.h"
#include "field.h"
#include "texture.h"
#include "object_ball.h"
#include "object_cube.h"

//**********************************************************************************
// マクロ定義
//**********************************************************************************
#define MAX_FILE	32			// 読み込めるファイル名の最大数


//==================================================================================
// コンストラクタ
//==================================================================================
CProp::CProp()
{
	memset(m_pObjectX, 0, sizeof(m_pObjectX));
	m_nNumModel = 0;
	m_nCntModel = 0;
	m_nCountDeath = 0;
}

//==================================================================================
// デストラクタ
//==================================================================================
CProp::~CProp()
{
}

//==================================================================================
// ファイルロード
//==================================================================================
void CProp::Load(void)
{
	char aStr[MAX_PATH];					// ファイル入力用

	// ファイル開封
	FILE *pFile;
	pFile = fopen("data\\TEXT\\MapProp.txt", "r");

	// ファイル読み込み失敗
	if (pFile == nullptr)
	{
		return;
	}
	else
	{
		//--- 読み込みファイル分岐 ---//
		while (true)
		{
			memset(aStr, NULL, sizeof(aStr));
			(void)fscanf(pFile, "%s", &aStr);

			if (strcmp(aStr, "PROP_SCRIPT") == 0)
			{
				LoadProp(pFile);
				fclose(pFile);
				return;
			}
		}
	}
}

//==================================================================================
// 終了処理
//==================================================================================
void CProp::Unload(void)
{
	
}

//==================================================================================
// マップ内オブジェクトの配置情報取得
//==================================================================================
void CProp::LoadProp(FILE* pFile)
{
	char aStr[MAX_PATH];					// ファイル入力用
	char acFileName[MAX_FILE][MAX_PATH];	// ファイルパス保存
	int nCntFile = 0;						// 保存したファイルパスのカウント
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	while (strcmp(aStr, "END_SCRIPT") != 0)
	{
		memset(aStr, NULL, sizeof(aStr));
		(void)fscanf(pFile, "%s", &aStr);

		//--- 使用するモデルなどのファイルパスを保存 ---//
		if (strcmp(aStr, "MODEL_FILENAME") == 0)
		{
			// ファイルパスを保存
			fscanf(pFile, " = %s", &acFileName[nCntFile][0]);

			// ファイルパスカウントアップ
			nCntFile++;
		}

		//--- モデル数を保存 ---//
		if (strcmp(aStr, "NUM_MODEL") == 0)
		{
			fscanf(pFile, " = %d", &m_nNumModel);
		}

		//======================================//
		//--- モデル配置データの読み込み開始 ---//
		//======================================//
		if (strcmp(aStr, "MODEL_SET") == 0)
		{
			while (strcmp(aStr, "END_MODELSET") != 0)		// モデルセットの終了
			{
				memset(aStr, NULL, sizeof(aStr));
				(void)fscanf(pFile, "%s", &aStr);

				//=====================================================================//
				//---------------------- プレイヤーの配置情報 -------------------------//
				if (strcmp(aStr, "SET_PLAYER") == 0)
				{
					while (strcmp(aStr, "END_PLAYERSET") != 0)
					{
						memset(aStr, NULL, sizeof(aStr));
						(void)fscanf(pFile, "%s", &aStr);

						//--- 位置 ---//
						if (strcmp(aStr, "POS") == 0)
						{
							fscanf(pFile, " = %f %f %f", &m_Playerpos.x, &m_Playerpos.y, &m_Playerpos.z);
						}

						//--- 角度 ---//
						if (strcmp(aStr, "ROT") == 0)
						{
							fscanf(pFile, " = %f %f %f", &m_Playerrot.x, &m_Playerrot.y, &m_Playerrot.z);
						}
					}
				}

				//=====================================================================//
				//---------------------- アイテムの配置情報 -------------------------//
#if 1
				if (strcmp(aStr, "SET_ITEM") == 0)
				{
					int nId = 0;			// モデル番号
					D3DXVECTOR3 pos, rot;	// 位置・角度

					while (strcmp(aStr, "END_ITEMSET") != 0)
					{
						memset(aStr, NULL, sizeof(aStr));
						(void)fscanf(pFile, "%s", &aStr);

						//--- モデル番号保存 ---//
						if (strcmp(aStr, "MODELNUMBER") == 0)
						{
							fscanf(pFile, " = %d", &nId);
						}

						//--- 位置 ---//
						if (strcmp(aStr, "POS") == 0)
						{
							fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
						}

						//--- 角度 ---//
						if (strcmp(aStr, "ROT") == 0)
						{
							fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);
						}
					}

					// アイテムデータ読み取り終了・セット
					CItem::Create(pos, rot, acFileName[nId]);
					m_nCntModel++;
				}
#endif // 0
				//=====================================================================//
				//---------------- マップ球オブジェクトの配置情報 -----------------------//
#if 1
				if (strcmp(aStr, "SET_OBJECT") == 0)
				{
					int nId = 0;			// モデル番号
					D3DXVECTOR3 pos, rot;	// 位置・角度

					while (strcmp(aStr, "END_OBJECT") != 0)
					{
						memset(aStr, NULL, sizeof(aStr));
						(void)fscanf(pFile, "%s", &aStr);

						//--- モデル番号保存 ---//
						if (strcmp(aStr, "MODELNUMBER") == 0)
						{
							fscanf(pFile, " = %d", &nId);
						}

						//--- 位置 ---//
						if (strcmp(aStr, "POS") == 0)
						{
							fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
						}

						//--- 角度 ---//
						if (strcmp(aStr, "ROT") == 0)
						{
							fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);
						}
					}

					// 球オブジェクトデータ読み取り終了・セット
					CBallObject::Create(pos, rot, 100.0f, acFileName[nId]);
					m_nCntModel++;
				}
#endif // 0

				//=====================================================================//
				//---------------- マップ箱オブジェクトの配置情報 -----------------------//
#if 1
				if (strcmp(aStr, "SET_BOXOBJECT") == 0)
				{
					int nId = 0;			// モデル番号
					D3DXVECTOR3 pos, rot;	// 位置・角度
					D3DXVECTOR2 size;		// 大きさ

					while (strcmp(aStr, "END_BOXOBJECT") != 0)
					{
						memset(aStr, NULL, sizeof(aStr));
						(void)fscanf(pFile, "%s", &aStr);

						//--- モデル番号保存 ---//
						if (strcmp(aStr, "MODELNUMBER") == 0)
						{
							fscanf(pFile, " = %d", &nId);
						}

						//--- 位置 ---//
						if (strcmp(aStr, "POS") == 0)
						{
							fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
						}

						//--- 角度 ---//
						if (strcmp(aStr, "ROT") == 0)
						{
							fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);
						}

						//--- 大きさ ---//
						if (strcmp(aStr, "SIZE") == 0)
						{
							fscanf(pFile, " = %f %f", &size.x, &size.y);
						}
					}

					// 球オブジェクトデータ読み取り終了・セット
					CCubeObject::Create(pos, rot, size, acFileName[nId]);
					m_nCntModel++;
				}
#endif // 0
				//=====================================================================//
				//------------------------- 壁の配置情報 ------------------------------//
#if 1
				if (strcmp(aStr, "SET_WALL") == 0)
				{
					int nTexId = 0;					// テクスチャ番号
					D3DXVECTOR3 pos, rot, size;		// 位置・角度

					while (strcmp(aStr, "END_WALLSET") != 0)
					{
						memset(aStr, NULL, sizeof(aStr));
						(void)fscanf(pFile, "%s", &aStr);

						//--- 位置 ---//
						if (strcmp(aStr, "POS") == 0)
						{
							fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
						}

						//--- 角度 ---//
						if (strcmp(aStr, "ROT") == 0)
						{
							fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);

							// 角度に誤差がでないよう矯正
							if (rot.y == 180.0f)
							{
								rot.y = D3DX_PI;
							}
							else if (rot.y == 90.0f)
							{
								rot.y = D3DX_PI * 0.5f;
							}
							else if (rot.y == -90.0f)
							{
								rot.y = D3DX_PI * -0.5f;
							}
						}

						//--- 大きさ ---//
						if (strcmp(aStr, "SIZE") == 0)
						{
							fscanf(pFile, " = %f %f %f", &size.x, &size.y, &size.z);
						}

						//--- モデル番号保存 ---//
						if (strcmp(aStr, "TEXID") == 0)
						{
							fscanf(pFile, " = %d", &nTexId);
						}
					}

					// 壁の設置データ読み取り終了・セット
					CWall::Create(pos, rot, size, pTexture->GetAddress(nTexId));
				}
#endif // 0

				//=====================================================================//
				//------------------------- 床の配置情報 ------------------------------//
#if 1
				if (strcmp(aStr, "SET_FIELD") == 0)
				{
					int nTexId = 0;					// テクスチャ番号
					D3DXVECTOR3 pos, rot, size;		// 位置・角度

					while (strcmp(aStr, "END_FIELDSET") != 0)
					{
						memset(aStr, NULL, sizeof(aStr));
						(void)fscanf(pFile, "%s", &aStr);

						//--- 位置 ---//
						if (strcmp(aStr, "POS") == 0)
						{
							fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
						}

						//--- 大きさ ---//
						if (strcmp(aStr, "SIZE") == 0)
						{
							fscanf(pFile, " = %f %f %f", &size.x, &size.y, &size.z);
						}

						//--- モデル番号保存 ---//
						if (strcmp(aStr, "TEXID") == 0)
						{
							fscanf(pFile, " = %d", &nTexId);
						}
					}

					// 壁の設置データ読み取り終了・セット
					CField::Create(pos, size, pTexture->GetAddress(nTexId), 200.0f);
				}
#endif // 0
			}
		}
	}
}

//==================================================================================
// プレイヤーデータの送信
// 使用する際、Propの生成 → プレイヤーを確実に行うこと
//==================================================================================
void CProp::SendPlayer(CPlayer* pPlayer)
{
	// プレイヤーにテキストからのデータを送信
	pPlayer->SetPos(m_Playerpos);
	pPlayer->SetRot(m_Playerrot);
}