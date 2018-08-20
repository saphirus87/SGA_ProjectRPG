#pragma once
#include "stdafx.h"

class Ray
{
public:
	Ray();
	Ray(Vector3 pos, Vector3 dir); 
	~Ray();

	// ��ũ�� ��ġ
	static Ray RayAtWorldSpace(int x, int y);

	// �ٿ�� ���Ǿ� �浹
	bool CalcIntersectSphere(BoundingSphere* pSphere, float* distance = NULL);

	// �ﰢ���̶� �浹
	bool CalcIntersectTri(Vector3* pStart, float* distance);
	bool CalcIntersectTri(Vector3* v0, Vector3* v1, Vector3* v2, float* distance);

public:
	Vector3 m_pos;
	Vector3 m_dir;

private:
	static Device9 pDevice9;
};

