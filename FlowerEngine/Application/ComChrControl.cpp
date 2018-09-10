#include "stdafx.h"
#include "ComChrControl.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "ComTerrain.h"
#include "ComCharacter.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ComChrControl::ComChrControl(CString szName)
	:Component(szName), m_pMap(NULL),
	IsPicking(false),
	m_pFollow(NULL),
	IsMoveToPoint(false),
	vMoveToPoint(0, 0, 0)
{
}

ComChrControl::~ComChrControl()
{
}

void ComChrControl::Awake()
{
	GameObject* pObjMap = GameObject::Find("ObjMap");
	if (pObjMap != NULL)
		m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");

	m_pFollow = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");
	m_pCharacter = (ComCharacter*)gameObject->GetComponent("ComCharacter");
}

void ComChrControl::Update()
{
	if (m_pCharacter->m_pCurrentState != m_pCharacter->m_vecState[eAni_Skill_1])
	{
		// ĳ���� ȸ��
		if (Input::KeyPress('A') || Input::KeyPress(VK_LEFT))
			gameObject->transform->RotateY(-0.1f);
		if (Input::KeyPress('D') || Input::KeyPress(VK_RIGHT))
			gameObject->transform->RotateY(0.1f);

		// ĳ���� �̵�
		if (Input::KeyPress('W') || Input::KeyPress(VK_UP))
		{
			m_pCharacter->CancleAttackTarget();
			m_pCharacter->Walk(1);
			// �ɾ ���� ���콺 �̵� ���� ���� (���콺 �̵��� Ű���� �̵��ϸ� �ӵ� 2��Ǵ� ���� ����)
			IsMoveToPoint = false;
		}
		else if (Input::KeyUp('W') || Input::KeyUp(VK_UP))
		{
			m_pCharacter->CancleAttackTarget();
			m_pCharacter->Stand();
		}
		if (Input::KeyPress('S') || Input::KeyPress(VK_DOWN))
		{
			m_pCharacter->CancleAttackTarget();
			m_pCharacter->Walk(-1);
			// �ɾ ���� ���콺 �̵� ���� ���� (���콺 �̵��� Ű���� �̵��ϸ� �ӵ� 2��Ǵ� ���� ����)
			IsMoveToPoint = false;
		}
		else if (Input::KeyUp('S') || Input::KeyUp(VK_DOWN))
		{
			m_pCharacter->CancleAttackTarget();
			m_pCharacter->Stand();
		}
	}
	
	if (Input::ButtonDown(VK_LBUTTON))
		CheckPickingChr();

	if (Input::ButtonDown(VK_RBUTTON))
	{
		m_pCharacter->CancleAttackTarget();
		// ���Ͱ� Picking���� ������ Map���� �̵� (�ߺ� ��ŷ ����)
		if (m_pCharacter->CheckPickingMon() == false)
			CheckPickingMap();
	}

	if (m_pFollow != NULL && m_pFollow->IsFollowing)
	{
		m_pCharacter->m_pCurrentState->Walk(eAni_Walk);
		m_pCharacter->GetHeight();
	}
	else if (m_pFollow != NULL && m_pFollow->AbleAttack)
		m_pCharacter->Attack1();

	MoveToPoint();
	m_pCharacter->CheckMonDeath();
	m_pCharacter->m_pCurrentState->Update();
}

void ComChrControl::Render()
{
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
			m_pCharacter->Stand();
		}
		else
		{
			m_pCharacter->m_pCurrentState->Walk(eAni_Walk);
			m_pCharacter->GetHeight();

			// �÷��̾ �ٶ󺸴� ���� ����
			Vector3 vDir = vMoveToPoint - gameObject->transform->GetPosition();
			D3DXVec3Normalize(&vDir, &vDir);

			// �ϴ��� Y�����θ� ȸ������
			float angleY = Vector::GetAngleY(&vDir);
			gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

			// �̵� �ӵ�
			vDir *= m_pCharacter->Status->MOVE_SPEED;

			// �ӵ����� ���ϴ� ���
			gameObject->transform->Translate(vDir);
		}
	}
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
	if (m_pMap && m_pMap->CalcPickedPosition(vMoveToPoint, mousePos.x, mousePos.y) && IsPicking == true)
		IsMoveToPoint = true;
}

