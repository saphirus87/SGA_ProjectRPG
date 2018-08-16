#pragma once
#include "stdafx.h"

class ComCollider : public Component
{
public:
	ComCollider(CString szName);
	~ComCollider();

	Vector3 GetMinPos() { return Vector3(oriPos.x + minPos.x, oriPos.y + minPos.y, oriPos.z + minPos.z); }
	Vector3 GetMaxPos() { return Vector3(oriPos.x + maxPos.x, oriPos.y + maxPos.y, oriPos.z + maxPos.z); }

	bool IsIntersected(ComCollider &collider);
	bool IsPointInside(Vector3& p);

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	
	void Set(Vector3& scale, bool isRender);

	Vector3 minPos;
	Vector3 maxPos;
	Vector3 oriPos;

private:
	ComRenderCubePN * m_pCube;
};

