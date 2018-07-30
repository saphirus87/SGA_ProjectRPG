#include "stdafx.h"
#include "ComFollowTarget.h"


ComFollowTarget::ComFollowTarget(CString szName) :
	Component(szName),
	accellation(0.0f),
	m_fLerp(0.003f),
	IsFollowing(false)
{
}


ComFollowTarget::~ComFollowTarget()
{
}

void ComFollowTarget::Awake()
{
	pTarget = GameObject::Find("Aircraft");
}

void ComFollowTarget::Update()
{
	// Target 위치와 나의 위치가 특정 거리 이하이면
	float fDistance = ComTransform::Distance(gameObject, pTarget);

	if (fDistance < 20.0f)
	{
		IsFollowing = true;
		// 플레이어를 바라보는 방향 벡터
		vForward = pTarget->transform->GetPosition() - gameObject->transform->GetPosition();
		D3DXVec3Normalize(&vForward, &vForward);

		// 일단은 Y축으로만 회전하자
		float angleY = Vector::GetAngleY(&vForward);
		gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

		Vector3 move;

		// 내위치 ~ 상대위치, 선형보간
		D3DXVec3Lerp(&move, &gameObject->transform->GetPosition(), &pTarget->transform->GetPosition(), m_fLerp);
		gameObject->transform->SetPosition(move);

		m_fLerp += accellation;

		if (m_fLerp >= 1.0f)
			m_fLerp = 0.0f;
	}
	else
		IsFollowing = false;
}

void ComFollowTarget::Render()
{
}
