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
		// 캐릭터 회전
		if (Input::KeyPress('A') || Input::KeyPress(VK_LEFT))
			gameObject->transform->RotateY(-0.1f);
		if (Input::KeyPress('D') || Input::KeyPress(VK_RIGHT))
			gameObject->transform->RotateY(0.1f);

		// 캐릭터 이동
		if (Input::KeyPress('W') || Input::KeyPress(VK_UP))
		{
			m_pCharacter->CancleAttackTarget();
			m_pCharacter->Walk(1);
			// 걸어갈 때는 마우스 이동 하지 않음 (마우스 이동시 키보드 이동하면 속도 2배되는 버그 방지)
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
			// 걸어갈 때는 마우스 이동 하지 않음 (마우스 이동시 키보드 이동하면 속도 2배되는 버그 방지)
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
		// 몬스터가 Picking되지 않으면 Map으로 이동 (중복 픽킹 방지)
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
		// 특정 위치로 이동
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

			// 플레이어를 바라보는 방향 벡터
			Vector3 vDir = vMoveToPoint - gameObject->transform->GetPosition();
			D3DXVec3Normalize(&vDir, &vDir);

			// 일단은 Y축으로만 회전하자
			float angleY = Vector::GetAngleY(&vDir);
			gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

			// 이동 속도
			vDir *= m_pCharacter->Status->MOVE_SPEED;

			// 속도벡터 곱하는 방법
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
			// 자기 위치로 초기화
			vMoveToPoint = gameObject->transform->GetPosition();
			break;
		}
	}
}

void ComChrControl::CheckPickingMap()
{
	// 이미 클릭되어 있다면 IsMoveToPoint를 false로 하여 새 MoveToPoint까지 가도록 한다.
	if (IsPicking == true)
		IsMoveToPoint = false;

	// 클릭했는데 아직 이동중이라면 MoveToPoint까지 갈 때 까지 둔다.
	if (IsMoveToPoint == true)
		return;

	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

	// Picking된 객체만 MoveToPoint를 계산하고 해당 위치로 이동한다.
	if (m_pMap && m_pMap->CalcPickedPosition(vMoveToPoint, mousePos.x, mousePos.y) && IsPicking == true)
		IsMoveToPoint = true;
}

