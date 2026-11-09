//==================================================================================
//
//  ユーティリティ [util.h]
//  Author : NAGATE
//
//==================================================================================

//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "main.h"

//**********************************************************************************
// プロトタイプ宣言
//**********************************************************************************
float RotCorrection(float rot);
void RotCorrection(D3DXVECTOR3* rot);
float RotFollow(float Dest, float Now, float speed);
float CrossProduct2D(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
bool InsideTriangle(const D3DXVECTOR3 pos, const D3DXVECTOR3* pVtx, const bool bLR);
D3DXVECTOR3 VectorNormalize(D3DXVECTOR3 A, D3DXVECTOR3 B);
float VectorSize(D3DXVECTOR3 vec);
D3DXVECTOR3 VectorNormalize(D3DXVECTOR3 vec);
D3DXVECTOR3 CrossProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB);
D3DXVECTOR3 Random3D(int Min, int Max);