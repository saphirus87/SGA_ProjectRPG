#include "stdafx.h"
#include "Vector.h"
//
//D3DXMatrixRotationYawPitchRoll(
//
//	D3DXMATRIX * pOut,                //회전이 되어 나오는 행렬
//	FLOAT Yaw,                                 //y축으로 radian 값으로 회전한다.
//	FLOAT Pitch,                                //x축으로 radian 값으로 회전한다.
//	FLOAT Roll                                   //z축으로 radian 값으로 회전한다.

Vector3 Vector::RotateX(Vector3 * v, float fAngle)
{
	Vector3 rV = *v;
	D3DXVec3Normalize(&rV, &rV);
	Matrix4x4 matRotX, matRotY;
	// x축으로 먼저 회전 시킨다음, y축으로 회전
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(fAngle));
	
	// x가 양수면 z축 방향을 바라봄, x가 음수면 -z축 방향을 바라봄
	Vector3 vAxisZ(0, 0, 1);
	D3DXMatrixRotationY(&matRotY, GetAngleY(&rV));
	D3DXVec3TransformNormal(&rV, &vAxisZ, &(matRotX * matRotY));
	
	return rV;
}

Vector3 Vector::RotateY(Vector3 * v, float fAngle)
{
	Vector3 rV = *v;
	D3DXVec3Normalize(&rV, &rV);
	Matrix4x4 matRot, matRotY;
	// x축으로 먼저 회전 시킨다음, y축으로 회전
	D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));

	// x가 양수면 z축 방향을 바라봄, x가 음수면 -z축 방향을 바라봄
	Vector3 vAxisZ(0, 0, 1);
	D3DXMatrixRotationY(&matRotY, GetAngleY(&rV));
	D3DXVec3TransformNormal(&rV, &vAxisZ, &(matRot * matRotY));

	return rV;
}

Vector3 Vector::RotateZ(Vector3 * v, float fAngle)
{
	Vector3 rV = *v;
	D3DXVec3Normalize(&rV, &rV);
	Matrix4x4 matRotZ, matRotY;
	// x축으로 먼저 회전 시킨다음, y축으로 회전
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(fAngle));

	// x가 양수면 z축 방향을 바라봄, x가 음수면 -z축 방향을 바라봄
	Vector3 vAxisZ(0, 0, 1);
	D3DXMatrixRotationY(&matRotY, GetAngleY(&rV));
	D3DXVec3TransformNormal(&rV, &vAxisZ, &(matRotZ * matRotY));

	return rV;
}

float Vector::GetAngleY(Vector3 * v)
{
	Vector3 nV1 = *v;
	Vector3 nV2 = Vector3(0, 0, 1);

	D3DXVec3Normalize(&nV1, &nV1);
	D3DXVec3Normalize(&nV2, &nV2);

	float r = acosf(D3DXVec3Dot(&nV1, &nV2));

	//외적의 y성분으로 방향이 결정된다.
	r = (nV1.x * nV2.z - nV1.z * nV2.x > 0.0f) ? r : -r;
	
	return r;
}