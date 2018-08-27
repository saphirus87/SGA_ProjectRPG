#include "stdafx.h"
#include "ComFollowTarget.h"


ComFollowTarget::ComFollowTarget(CString szName) :
	Component(szName),
	fMoveSpeed(0.02f),
	accellation(0.0f),
	m_fLerp(0.003f),
	IsFollowing(false),
	AbleAttack(false), 
	pTarget(NULL),
	fRange(20.0f)
{
}


ComFollowTarget::~ComFollowTarget()
{
}

void ComFollowTarget::Awake()
{
	
}

void ComFollowTarget::Update()
{
	if (pTarget == NULL)
	{
		// 초기화 
		AbleAttack = false;
		IsFollowing = false;
		return;
	}

	// Target 위치와 나의 위치가 특정 거리 이하이면
	float fDistance = ComTransform::Distance(gameObject, pTarget);
	
	// 초기화 
	AbleAttack = false;
	IsFollowing = false;

	if (fDistance < 1.0f)
		AbleAttack = true;
	else if (fDistance < fRange)
	{
		IsFollowing = true;
		// 플레이어를 바라보는 방향 벡터
		vDir = pTarget->transform->GetPosition() - gameObject->transform->GetPosition();
		D3DXVec3Normalize(&vDir, &vDir);

		// 일단은 Y축으로만 회전하자
		float angleY = Vector::GetAngleY(&vDir);
		gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

		Vector3 move;

		// 속도가 0.02f라면
		vDir *= fMoveSpeed;

		// 속도벡터 곱하는 방법
		gameObject->transform->Translate(vDir);
		
		// 내위치 ~ 상대위치, 선형보간 방법
		//D3DXVec3Lerp(&move, &gameObject->transform->GetPosition(), &pTarget->transform->GetPosition(), m_fLerp);
		//gameObject->transform->SetPosition(move);

		/*m_fLerp += accellation;

		if (m_fLerp >= 1.0f)
			m_fLerp = 0.0f;*/
	}
}

void ComFollowTarget::Render()
{
}
