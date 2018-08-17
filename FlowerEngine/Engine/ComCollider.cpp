#include "stdafx.h"
#include "ComCollider.h"


ComCollider::ComCollider(CString szName) : 
	Component(szName), 
	minPos(Vector3(-1.0f, -1.0f, -1.0f)),
	maxPos(Vector3( 1.0f,  1.0f,  1.0f)),
	m_pCube(NULL)
{
}


ComCollider::~ComCollider()
{
}

bool ComCollider::IsIntersected(ComCollider & box)
{
	if (GetMinPos().x > box.GetMaxPos().x || GetMaxPos().x < box.GetMinPos().x) return false;
	if (GetMinPos().y > box.GetMaxPos().y || GetMaxPos().y < box.GetMinPos().y) return false;
	if (GetMinPos().z > box.GetMaxPos().z || GetMaxPos().z < box.GetMinPos().z) return false;
	return true;
}

bool ComCollider::IsPointInside(Vector3 & p)
{
	if (GetMinPos().x > p.x || GetMaxPos().x < p.x) return false;
	if (GetMinPos().y > p.y || GetMaxPos().y < p.y) return false;
	if (GetMinPos().z > p.z || GetMaxPos().z < p.z) return false;
	return true;
}

void ComCollider::Awake()
{
	oriPos = ((ComTransform*)gameObject->GetComponent("ComTransform"))->GetPosition();
	m_pCube = new ComRenderCubePN("ComRenderCubePN");
	gameObject->AddComponent(m_pCube);
}

void ComCollider::Update()
{
}

void ComCollider::Render()
{
}

void ComCollider::Set(Vector3& offsetPos, Vector3 & scale, bool isRender)
{
	if (m_pCube == NULL)
		return;

	this->offsetPos = offsetPos;

	minPos.x += offsetPos.x; minPos.y += offsetPos.y; minPos.z += offsetPos.z;
	maxPos.x += offsetPos.x; maxPos.y += offsetPos.y; maxPos.z += offsetPos.z;
	
	minPos.x *= scale.x; minPos.y *= scale.y; minPos.z *= scale.z;
	maxPos.x *= scale.x; maxPos.y *= scale.y; maxPos.z *= scale.z;

	m_pCube->SetLocalVertexScale(scale);
	m_pCube->IsRender = isRender;
	m_pCube->pCollider = this;
}
