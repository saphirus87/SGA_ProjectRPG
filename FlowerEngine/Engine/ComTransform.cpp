#include "stdafx.h"
#include "GameObject.h"
#include "ComTransform.h"

ComTransform::ComTransform(CString szName) : 
	Component(szName),
	m_position(Vector3(0, 0, 0)),
	m_scale(Vector3(1, 1, 1)),
	m_rotAngle(Vector3(0, 0, 0)),
	IsAutoUpdate(true)
{
	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matWorld);
}

ComTransform::~ComTransform()
{
}

void ComTransform::SetPosition(float fPositionX, float fPositionY, float fPositionZ)
{
	m_position.x = fPositionX;
	m_position.y = fPositionY;
	m_position.z = fPositionZ;
}

void ComTransform::SetRotation(float fAngleX, float fAngleY, float fAngleZ)
{
	m_rotAngle.x = fAngleX;
	m_rotAngle.y = fAngleY;
	m_rotAngle.z = fAngleZ;
}

void ComTransform::SetScale(float fScaleX, float fScaleY, float fScaleZ)
{
	m_scale.x = fScaleX;
	m_scale.y = fScaleY;
	m_scale.z = fScaleZ;
}

void ComTransform::RotateX(float fAngleX)
{
	m_rotAngle.x += fAngleX;
}

void ComTransform::RotateY(float fAngleY)
{
	m_rotAngle.y += fAngleY;
}

void ComTransform::RotateZ(float fAngleZ)
{
	m_rotAngle.z += fAngleZ;
}

void ComTransform::Translate(Vector3 & vecMove)
{
	m_position += vecMove;
}

void ComTransform::GetForward(Vector3& forward)
{
	D3DXVec3TransformNormal(&forward, &Vector3(0.0f, 0.0f, 1.0f), &matRotY);
}

float ComTransform::Distance(GameObject * pGO1, GameObject * pGO2)
{
	Vector3 vDist = pGO1->transform->GetPosition() - pGO2->transform->GetPosition();
	return D3DXVec3Length(&vDist);
}

float ComTransform::Distance(GameObject * pGO1, Vector3 * pVec)
{
	Vector3 vDist = pGO1->transform->GetPosition() - *pVec;
	return D3DXVec3Length(&vDist);
}

Matrix4x4 & ComTransform::GetWorldMatrix()
{
	return m_matWorld;
}

void ComTransform::Update()
{
	if (IsAutoUpdate == false)
		return;

	D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationX(&matRotX, m_rotAngle.x);
	D3DXMatrixRotationY(&matRotY, m_rotAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_rotAngle.z);
	D3DXMatrixScaling(&matS, m_scale.x, m_scale.y, m_scale.z);
	m_matWorld = m_matLocal = matS * matRotX * matRotY * matRotZ * matT;
	if (gameObject->GetParent())
		m_matWorld = m_matLocal * gameObject->GetParent()->transform->GetWorldMatrix();
}

void ComTransform::Render()
{
	
}

void ComTransform::Awake()
{
}
