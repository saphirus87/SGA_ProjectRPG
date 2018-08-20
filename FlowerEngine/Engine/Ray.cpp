#include "stdafx.h"
#include "Ray.h"

Device9 Ray::pDevice9 = NULL;

Ray::Ray() : 
	m_pos(0, 0, 0),
	m_dir(0, 0, 1)
{
}

Ray::Ray(Vector3 pos, Vector3 dir) : m_pos(pos), m_dir(dir)
{
}

Ray::~Ray()
{
}

Ray Ray::RayAtWorldSpace(int x, int y)
{
	if (pDevice9 == NULL)
		pDevice9 = GetD3D9Device();

	D3DVIEWPORT9 vp;
	pDevice9->GetViewport(&vp);
	Matrix4x4 matProj;
	pDevice9->GetTransform(D3DTS_PROJECTION, &matProj);

	Ray ray;
	// PROJ TO VIEW SPACE
	ray.m_dir.x = ((x * 2.0f) / vp.Width - 1.0f) / matProj._11; // view space에서의 방향까지 보낼 수 있음
	// -1 ~ 1
	ray.m_dir.y = ((-y * 2.0f) / vp.Height + 1.0f) / matProj._22;

	// TO WORLD SPACE
	Matrix4x4 matView, matInvView;
	pDevice9->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, NULL, &matView); // 역행렬

	D3DXVec3TransformCoord(&ray.m_pos, &ray.m_pos, &matInvView);
	D3DXVec3TransformNormal(&ray.m_dir, &ray.m_dir, &matInvView); // 방향값은 Normal
	D3DXVec3Normalize(&ray.m_dir, &ray.m_dir);

	return ray;
}

bool Ray::CalcIntersectSphere(BoundingSphere * pSphere, float* distance)
{
	Vector3 localPos = m_pos - pSphere->center;

	// 길이
	if (distance)
		(*distance) = D3DXVec3Length(&localPos);

	// 공식
	float qdotv = D3DXVec3Dot(&localPos, &m_dir);
	float qdotq = D3DXVec3Dot(&localPos, &localPos);
	float rr = pSphere->radius * pSphere->radius;

	return qdotv * qdotv - (qdotq - rr) >= 0; // 0 이상이면 교차 한 것
}

bool Ray::CalcIntersectTri(Vector3 * pStart, float * distance)
{
	return D3DXIntersectTri(pStart, pStart + 1, pStart + 2, &m_pos, &m_dir, NULL, NULL, distance);
}

bool Ray::CalcIntersectTri(Vector3 v0, Vector3 v1, Vector3 v2, float * distance)
{
	return false;
}
