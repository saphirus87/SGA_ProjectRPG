#include "stdafx.h"
#include "ComChrControl.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "ComTerrain.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"
#include "ComCharacter.h"

ComChrControl::ComChrControl(CString szName)
	:Component(szName), m_pMap(NULL),
	m_pCurrentState(NULL),
	IsPicking(false),
	m_pTarget(NULL),
	IsMoveToPoint(false),
	vMoveToPoint(0, 0, 0),
	hAttack(NULL),
	pAttackTarget(NULL)
{
}

ComChrControl::~ComChrControl()
{
	for (size_t i = 0; i < m_vecState.size(); ++i)
		SAFE_DELETE(m_vecState[i]);
}

void ComChrControl::Init()
{
	GameObject* pObjMap = GameObject::Find("ObjMap");
	if (pObjMap != NULL)
		m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");
	pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");

	m_pTarget = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");
	m_pCharacter = (ComCharacter*)gameObject->GetComponent("ComCharacter");
}

void ComChrControl::Awake()
{
	Init();

	m_vecState.resize(eAni_COUNT);
	m_vecState[eAni_Stand] = new ChrStateStand(this);
	m_vecState[eAni_Walk] = new ChrStateWalk(this);
	m_vecState[eAni_Attack_1] = new ChrStateAttack1(this);
	m_vecState[eAni_Attack_2] = new ChrStateAttack1(this);
	m_vecState[eAni_Attack_3] = new ChrStateAttack1(this);

	m_pCurrentState = m_vecState[eAni_Stand];
	Stand();
}

void ComChrControl::Update()
{
	// ĳ���� ȸ��
	if (Input::KeyPress('A') || Input::KeyPress(VK_LEFT))
		gameObject->transform->RotateY(-0.1f);
	if (Input::KeyPress('D') || Input::KeyPress(VK_RIGHT))
		gameObject->transform->RotateY(0.1f);

	// ĳ���� �̵�
	if (Input::KeyPress('W') || Input::KeyPress(VK_UP))
	{
		CancleAttackTarget();
		Walk(1);
	}
	else if (Input::KeyUp('W') || Input::KeyUp(VK_UP))
	{
		CancleAttackTarget();
		Stand();
	}
	if (Input::KeyPress('S') || Input::KeyPress(VK_DOWN))
	{
		CancleAttackTarget();
		Walk(-1);
	}
	else if (Input::KeyUp('S') || Input::KeyUp(VK_DOWN))
	{
		CancleAttackTarget();
		Stand();
	}

	// ĳ���� ����
	if (Input::KeyDown('F'))
		Attack1();

	if (Input::ButtonDown(VK_LBUTTON))
		CheckPickingChr();

	if (Input::ButtonDown(VK_RBUTTON))
	{
		CancleAttackTarget();
		CheckPickingMon();
		CheckPickingMap();
	}

	if (m_pTarget != NULL && m_pTarget->IsFollowing)
	{
		m_pCurrentState->Walk(eAni_Walk);
		GetHeight();
	}
	else if (m_pTarget != NULL && m_pTarget->AbleAttack)
		Attack1();

	MoveToPoint();
}

void ComChrControl::Render()
{
}

void ComChrControl::GetHeight()
{
	m_pMap->UpdateIndexBufferQuadTree();
	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.f;
	if (m_pMap != NULL && m_pMap->GetHeight(fHeight, pos) == true)
	{
		pos.y = fHeight;
		gameObject->transform->SetPosition(pos);
	}
}

void ComChrControl::CancleAttackTarget()
{
	m_pTarget->pTarget = NULL;
	m_pTarget->AbleAttack = false;
}

void ComChrControl::MoveToPoint()
{
	if (IsMoveToPoint == true)
	{
		// Ư�� ��ġ�� �̵�
		float fDistToPoint = ComTransform::Distance(gameObject, &vMoveToPoint);

		if (fDistToPoint < 1.0f)
		{
			IsMoveToPoint = false;
			Stand();
		}
		else
		{
			m_pCurrentState->Walk(eAni_Walk);
			GetHeight();

			// �÷��̾ �ٶ󺸴� ���� ����
			Vector3 vDir = vMoveToPoint - gameObject->transform->GetPosition();
			D3DXVec3Normalize(&vDir, &vDir);

			// �ϴ��� Y�����θ� ȸ������
			float angleY = Vector::GetAngleY(&vDir);
			gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

			Vector3 move;

			// �ӵ��� 0.02f���
			vDir *= 0.02f;

			// �ӵ����� ���ϴ� ���
			gameObject->transform->Translate(vDir);
		}
	}
}

void ComChrControl::SetState(int iIndex)
{
	m_pCurrentState = m_vecState[iIndex];
}

void ComChrControl::Stand()
{
	// ���� ���¿��� Stand��
	m_pCurrentState->Stand(eAni_Stand);
}

void ComChrControl::Walk(float fDeltaZ)
{
	// ���� ���¿��� Walk��
	m_pCurrentState->Walk(eAni_Walk);
	GetHeight();
	float fMoveSpeed = 0.10f;	//�̵� �ӵ�
	gameObject->transform->GetForward(m_vecForward);
	Vector3 forward = fDeltaZ * m_vecForward * fMoveSpeed;
	gameObject->transform->Translate(forward);
}

void ComChrControl::Attack1()
{
	// ���� ���¿��� Attack1��
	m_pCurrentState->Attack1(eAni_Attack_1);
	

}

void ComChrControl::CheckPickingChr()
{
	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();
	
	ComRenderCubePN* pCube = (ComRenderCubePN*)gameObject->GetComponent("ComRenderCubePN");

	Ray ray = Ray::RayAtWorldSpace(mousePos.x, mousePos.y);

	vector<Vector3>& vertices = pCube->GetVector();
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		float dist = 0;
		IsPicking = ray.CalcIntersectTri(&vertices[i], &dist);
		if (IsPicking == true)
		{
			// �ڱ� ��ġ�� �ʱ�ȭ
			vMoveToPoint = gameObject->transform->GetPosition();
			break;
		}
	}
}

void ComChrControl::CheckPickingMon()
{
	if (m_pTarget == NULL)
		return;

	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

	list<GameObject*> listMonster = GameObject::FindAll("Monster");

	for (auto & o : listMonster)
	{
		ComRenderCubePN* pCube = (ComRenderCubePN*)o->GetComponent("ComRenderCubePN");

		Ray ray = Ray::RayAtWorldSpace(mousePos.x, mousePos.y);

		vector<Vector3>& vertices = pCube->GetVector();
		for (size_t i = 0; i < vertices.size(); i += 3)
		{
			float dist = 0;
			bool pickMon = ray.CalcIntersectTri(&vertices[i], &dist);

			if (pickMon == true)
			{
				// ���͸� ���󰣴�.
				m_pTarget->pTarget = o;
				pAttackTarget = (ComCharacter*)o->GetComponent("ComCharacter");
			}
		}
	}
}

void ComChrControl::CheckPickingMap()
{
	// �̹� Ŭ���Ǿ� �ִٸ� IsMoveToPoint�� false�� �Ͽ� �� MoveToPoint���� ������ �Ѵ�.
	if (IsPicking == true)
		IsMoveToPoint = false;

	// Ŭ���ߴµ� ���� �̵����̶�� MoveToPoint���� �� �� ���� �д�.
	if (IsMoveToPoint == true)
		return;

	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

	// Picking�� ��ü�� MoveToPoint�� ����ϰ� �ش� ��ġ�� �̵��Ѵ�.
	if (m_pMap->CalcPickedPosition(vMoveToPoint, mousePos.x, mousePos.y) && IsPicking == true)
		IsMoveToPoint = true;
}

