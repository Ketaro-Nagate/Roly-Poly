//==================================================================================
//
// ユーティリティ [util.cpp]
// Author : Nagate
//
//==================================================================================
//**********************************************************************************
// インクルードファイル
//**********************************************************************************
#include "util.h"
#include <random>

//==================================================================================
// 角度補正
//==================================================================================
float RotCorrection(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2.0f;
	}
	else if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2.0f;
	}
	return rot;
}

//==================================================================================
// 角度補正(vector3)
//==================================================================================
void RotCorrection(D3DXVECTOR3* rot)
{
	// ±3.14fを超える場合、補正をかける

	//--- X補正 ---//
	if (rot->x > D3DX_PI)
	{
		rot->x -= D3DX_PI * 2.0f;
	}
	else if (rot->x < -D3DX_PI)
	{
		rot->x += D3DX_PI * 2.0f;
	}
	
	//--- Y補正 ---//
	if (rot->y > D3DX_PI)
	{
		rot->y -= D3DX_PI * 2.0f;
	}
	else if (rot->y < -D3DX_PI)
	{
		rot->y += D3DX_PI * 2.0f;
	}

	//--- Z補正 ---//
	if (rot->z > D3DX_PI)
	{
		rot->z -= D3DX_PI * 2.0f;
	}
	else if (rot->z < -D3DX_PI)
	{
		rot->z += D3DX_PI * 2.0f;
	}
}

//==========================================================
//　角度の追従処理
//==========================================================
float RotFollow(float Dest, float Now, float speed)
{
	float Diff = Dest - Now;			// 修正目標と現在値の差

	if (Diff > D3DX_PI)					// Diffで回転の向きを決める
	{
		Diff = Diff - D3DX_PI * 2.0f;

		Now += Diff * speed;

		if (Now > D3DX_PI)
		{
			Now -= D3DX_PI * 2.0f;
		}
		else if (Now < -D3DX_PI)
		{
			Now += D3DX_PI * 2.0f;
		}
	}
	else if (Diff < -D3DX_PI)
	{
		Diff = Diff + D3DX_PI * 2.0f;

		Now += Diff * speed;

		if (Now > D3DX_PI)
		{
			Now -= D3DX_PI * 2.0f;
		}
		else if (Now < -D3DX_PI)
		{
			Now += D3DX_PI * 2.0f;
		}
	}
	else
	{
		Now += (Diff) * speed;
	}

	return Now;
}

//==========================================================
// 三角形の内側か判定
//==========================================================
bool InsideTriangle(const D3DXVECTOR3 pos, const D3DXVECTOR3* pVtx,const bool bLR)
{
	D3DXVECTOR3 vecLine[3];			// 壁
	D3DXVECTOR3 vectoPos[3];		// 壁 to pos
	float aAns[3];

	//--- ポリゴンの内側にいるか ---//
	vecLine[0] = pVtx[1] - pVtx[0];
	vecLine[1] = pVtx[2] - pVtx[1];
	vecLine[2] = pVtx[0] - pVtx[2];

	vectoPos[0] = pos - pVtx[0];
	vectoPos[1] = pos - pVtx[1];
	vectoPos[2] = pos - pVtx[2];

	for (int nCount = 0; nCount < 3; nCount++)
	{
		D3DXVec3Normalize(&vecLine[nCount], &vecLine[nCount]);
		D3DXVec3Normalize(&vectoPos[nCount], &vectoPos[nCount]);
	}

	aAns[0] = CrossProduct2D(vecLine[0], vectoPos[0]);
	aAns[1] = CrossProduct2D(vecLine[1], vectoPos[1]);
	aAns[2] = CrossProduct2D(vecLine[2], vectoPos[2]);

	// 内側にいる
	if (aAns[0] >= 0 && aAns[1] >= 0 && aAns[2] >= 0 && bLR == true)
	{
		return true;
	}
	else if(aAns[0] <= 0 && aAns[1] <= 0 && aAns[2] <= 0 && bLR == false)
	{
		return true;
	}

	return false;
}

//==========================================================
// 外積（2要素）
//==========================================================
float CrossProduct2D(const D3DXVECTOR3 vec2, const D3DXVECTOR3 vec1)
{
	return ((vec2.z * vec1.x) - (vec2.x * vec1.z));
}

//==================================================================================
// ３次元 ２点間のベクトル（正規化あり）
//==================================================================================
D3DXVECTOR3 VectorNormalize(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	D3DXVECTOR3 vec;
	float size;

	// 求めるベクトル
	vec = D3DXVECTOR3(
		A.x - B.x,
		A.y - B.y,
		A.z - B.z);

	// 大きさ
	size = sqrtf((vec.x * vec.x) +
		(vec.y * vec.y)+
		(vec.z * vec.z));

	// 大きさで割って１にする
	return D3DXVECTOR3(vec.x / size, vec.y / size, vec.z / size);
}

//==================================================================================
// ３次元 ベクトルの大きさ
//==================================================================================
float VectorSize(D3DXVECTOR3 vec)
{
	float size = (sqrtf(vec.x * vec.x),
		sqrtf(vec.y * vec.y),
		sqrtf(vec.z * vec.z));

	return size;
}

//==================================================================================
// ３次元 ベクトル正規化
//==================================================================================
D3DXVECTOR3 VectorNormalize(D3DXVECTOR3 vec)
{
	float size = sqrtf((vec.x * vec.x)
		+ (vec.y * vec.y)
		+ (vec.z * vec.z));

	if (size != 0.0f)
	{// ０で割るのを防ぐ
		// 大きさで割って１にする
		return D3DXVECTOR3((vec.x / size), (vec.y / size), (vec.z / size));
	}
	else
	{
		return NULL_D3DXVECTOR3;
	}
}

//==================================================================================
// ベクトルの外積
//==================================================================================
D3DXVECTOR3 CrossProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB)
{
	return D3DXVECTOR3(
		vectorA.y * vectorB.z - vectorA.z * vectorB.y,
		vectorA.z * vectorB.x - vectorA.x * vectorB.z,
		vectorA.x * vectorB.y - vectorA.y * vectorB.z
	);
}

//==================================================================================
// ランダムな値を生成
//==================================================================================
D3DXVECTOR3 Random3D(int Min, int Max)
{
	// シードを取得
	std::random_device seed_gen;
	// 疑似乱数エンジンを初期化
	std::mt19937 engine(seed_gen());
	// 生成する分布を定義
	std::uniform_int_distribution<int> dist(Min, Max);

	D3DXVECTOR3 Rand;
	Rand.x = dist(engine);
	Rand.y = dist(engine);
	Rand.z = dist(engine);

	return Rand;
}