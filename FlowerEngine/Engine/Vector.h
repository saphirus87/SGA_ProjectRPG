#pragma once
#include "stdafx.h"

class Vector
{
public:
	// 해당 벡터를 angle만큼 회전 시킵니다.
	static Vector3 RotateX(Vector3 * v, float fAngle);
	
	// 해당 벡터를 angle만큼 회전 시킵니다.
	static Vector3 RotateY(Vector3 * v, float fAngle);

	// 해당 벡터를 angle만큼 회전 시킵니다.
	static Vector3 RotateZ(Vector3 * v, float fAngle);

	// 해당 벡터를 y축을 기준으로 몇 도 회전했는지 각도를 구합니다.
	static float GetAngleY(Vector3 * v);
};

