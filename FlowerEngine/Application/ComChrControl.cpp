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
	m_pTarget(NULL)
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
		m_pTarget->pTarget = NULL;
		m_pTarget->AbleAttack = false;
		Walk(1);
	}
	else if (Input::KeyUp('W') || Input::KeyUp(VK_UP))
	{
		m_pTarget->pTarget = NULL;
		m_pTarget->AbleAttack = false;
		Stand();
	}
	if (Input::KeyPress('S') || Input::KeyPress(VK_DOWN))
	{
		m_pTarget->pTarget = NULL;
		m_pTarget->AbleAttack = false;
		Walk(-1);
	}
	else if (Input::KeyUp('S') || Input::KeyUp(VK_DOWN))
	{
		m_pTarget->pTarget = NULL;
		m_pTarget->AbleAttack = false;
		Stand();
	}

	// ĳ���� ����
	if (Input::KeyDown('F'))
		Attack1();

	if (Input::ButtonDown(VK_LBUTTON))
		CheckPickingChr();

	if (Input::ButtonDown(VK_RBUTTON))
	{
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
	
	// Ư�� �����ӿ��� ����
	//ComCharacter* pChr = (ComCharacter*)m_pTarget->gameObject->GetComponent("ComCharacter");
	//m_pCharacter->AttackTarget(pChr);
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
			}
		}
	}
}

void ComChrControl::CheckPickingMap()
{
	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

	Vector3 vOut(0, 0, 0);
	//m_pMap->CalcPickedPosition(vOut, mousePos.x, mousePos.y);
}

