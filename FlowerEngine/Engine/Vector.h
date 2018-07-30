#pragma once
#include "stdafx.h"

class Vector
{
public:
	// �ش� ���͸� angle��ŭ ȸ�� ��ŵ�ϴ�.
	static Vector3 RotateX(Vector3 * v, float fAngle);
	
	// �ش� ���͸� angle��ŭ ȸ�� ��ŵ�ϴ�.
	static Vector3 RotateY(Vector3 * v, float fAngle);

	// �ش� ���͸� angle��ŭ ȸ�� ��ŵ�ϴ�.
	static Vector3 RotateZ(Vector3 * v, float fAngle);

	// �ش� ���͸� y���� �������� �� �� ȸ���ߴ��� ������ ���մϴ�.
	static float GetAngleY(Vector3 * v);
};

