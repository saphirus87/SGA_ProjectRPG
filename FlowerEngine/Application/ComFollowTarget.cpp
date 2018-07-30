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
	// Target ��ġ�� ���� ��ġ�� Ư�� �Ÿ� �����̸�
	float fDistance = ComTransform::Distance(gameObject, pTarget);

	if (fDistance < 20.0f)
	{
		IsFollowing = true;
		// �÷��̾ �ٶ󺸴� ���� ����
		vForward = pTarget->transform->GetPosition() - gameObject->transform->GetPosition();
		D3DXVec3Normalize(&vForward, &vForward);

		// �ϴ��� Y�����θ� ȸ������
		float angleY = Vector::GetAngleY(&vForward);
		gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

		Vector3 move;

		// ����ġ ~ �����ġ, ��������
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
