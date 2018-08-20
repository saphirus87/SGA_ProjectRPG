#pragma once
#include "stdafx.h"

class Ray
{
public:
	Ray();
	Ray(Vector3 pos, Vector3 dir); 
	~Ray();

	// 스크린 위치
	static Ray RayAtWorldSpace(int x, int y);

	// 바운딩 스피어 충돌
	bool CalcIntersectSphere(BoundingSphere* pSphere, float* distance = NULL);

	// 삼각형이랑 충돌
	bool CalcIntersectTri(Vector3* pStart, float* distance);
	bool CalcIntersectTri(Vector3* v0, Vector3* v1, Vector3* v2, float* distance);

public:
	Vector3 m_pos;
	Vector3 m_dir;

private:
	static Device9 pDevice9;
};

