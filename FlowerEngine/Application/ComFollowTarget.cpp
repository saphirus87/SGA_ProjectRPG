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
		// �ʱ�ȭ 
		AbleAttack = false;
		IsFollowing = false;
		return;
	}

	// Target ��ġ�� ���� ��ġ�� Ư�� �Ÿ� �����̸�
	float fDistance = ComTransform::Distance(gameObject, pTarget);
	
	// �ʱ�ȭ 
	AbleAttack = false;
	IsFollowing = false;

	if (fDistance < 1.0f)
		AbleAttack = true;
	else if (fDistance < fRange)
	{
		IsFollowing = true;
		// �÷��̾ �ٶ󺸴� ���� ����
		vDir = pTarget->transform->GetPosition() - gameObject->transform->GetPosition();
		D3DXVec3Normalize(&vDir, &vDir);

		// �ϴ��� Y�����θ� ȸ������
		float angleY = Vector::GetAngleY(&vDir);
		gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

		Vector3 move;

		// �ӵ��� 0.02f���
		vDir *= fMoveSpeed;

		// �ӵ����� ���ϴ� ���
		gameObject->transform->Translate(vDir);
		
		// ����ġ ~ �����ġ, �������� ���
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
