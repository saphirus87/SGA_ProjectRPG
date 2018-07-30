#include "stdafx.h"
#include "Vector.h"
//
//D3DXMatrixRotationYawPitchRoll(
//
//	D3DXMATRIX * pOut,                //ȸ���� �Ǿ� ������ ���
//	FLOAT Yaw,                                 //y������ radian ������ ȸ���Ѵ�.
//	FLOAT Pitch,                                //x������ radian ������ ȸ���Ѵ�.
//	FLOAT Roll                                   //z������ radian ������ ȸ���Ѵ�.

Vector3 Vector::RotateX(Vector3 * v, float fAngle)
{
	Vector3 rV = *v;
	D3DXVec3Normalize(&rV, &rV);
	Matrix4x4 matRotX, matRotY;
	// x������ ���� ȸ�� ��Ų����, y������ ȸ��
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(fAngle));
	
	// x�� ����� z�� ������ �ٶ�, x�� ������ -z�� ������ �ٶ�
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
	// x������ ���� ȸ�� ��Ų����, y������ ȸ��
	D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));

	// x�� ����� z�� ������ �ٶ�, x�� ������ -z�� ������ �ٶ�
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
	// x������ ���� ȸ�� ��Ų����, y������ ȸ��
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(fAngle));

	// x�� ����� z�� ������ �ٶ�, x�� ������ -z�� ������ �ٶ�
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

	//������ y�������� ������ �����ȴ�.
	r = (nV1.x * nV2.z - nV1.z * nV2.x > 0.0f) ? r : -r;
	
	return r;
}