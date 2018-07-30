#include "stdafx.h"
#include "ComCollider.h"


ComCollider::ComCollider(CString szName) : 
	Component(szName), 
	minPos(Vector3(-1.0f, -1.0f, -1.0f)),
	maxPos(Vector3( 1.0f,  1.0f,  1.0f))
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
}

void ComCollider::Update()
{
}

void ComCollider::Render()
{
}
