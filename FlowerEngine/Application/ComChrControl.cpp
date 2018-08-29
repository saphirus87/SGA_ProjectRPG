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
#include "SceneRPG.h"

ComChrControl::ComChrControl(CString szName)
	:Component(szName), m_pMap(NULL),
	m_pCurrentState(NULL),
	IsPicking(false),
	m_pFollow(NULL),
	IsMoveToPoint(false),
	vMoveToPoint(0, 0, 0),
	pAttackTarget(NULL),
	IsGroud(false)
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

	m_pFollow = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");
	m_pCharacter = (ComCharacter*)gameObject->GetComponent("ComCharacter");
}

void ComChrControl::Awake()
{
	Init();

	m_vecState.resize(eAni_COUNT);
	m_vecState[eAni_Stand] = new ChrStateStand(this);
	m_vecState[eAni_Walk] = new ChrStateWalk(this);
	m_vecState[eAni_Attack_1] = new ChrStateAttack1(this);
	m_vecState[eAni_Skill_1] = new ChrStateSkill1(this);
	m_vecState[eAni_Skill_2] = new ChrStateSkill2(this);

	m_pCurrentState = m_vecState[eAni_Stand];
	Stand();

	m_pMap->UpdateIndexBufferQuadTree();
	GetHeight();
}

void ComChrControl::Update()
{
	// 캐릭터 회전
	if (Input::KeyPress('A') || Input::KeyPress(VK_LEFT))
		gameObject->transform->RotateY(-0.1f);
	if (Input::KeyPress('D') || Input::KeyPress(VK_RIGHT))
		gameObject->transform->RotateY(0.1f);

	// 캐릭터 이동
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
	
	if (Input::ButtonDown(VK_LBUTTON))
		CheckPickingChr();

	if (Input::ButtonDown(VK_RBUTTON))
	{
		CancleAttackTarget();
		CheckPickingMon();
		CheckPickingMap();
	}

	if (m_pFollow != NULL && m_pFollow->IsFollowing)
	{
		m_pCurrentState->Walk(eAni_Walk);
		GetHeight();
	}
	else if (m_pFollow != NULL && m_pFollow->AbleAttack)
		Attack1();

	MoveToPoint();
	CheckAttackTargetDeath();
}

void ComChrControl::Render()
{
}

void ComChrControl::GetHeight()
{
	if (m_pMap == NULL)
		return;

	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.f;
	if (m_pMap->GetHeight(fHeight, pos) == true)
	{
		pos.y = fHeight;
		gameObject->transform->SetPosition(pos);
		IsGroud = true;
	}
}

void ComChrControl::CancleAttackTarget()
{
	pAttackTarget = NULL;
	m_pFollow->IsFollowing = false;
	m_pFollow->AbleAttack = false;
	m_pFollow->pTarget = NULL;
}

void ComChrControl::LookatTarget()
{
	if (pAttackTarget == NULL)
		return;

	// 플레이어를 바라보는 방향 벡터
	Vector3 vDir = pAttackTarget->gameObject->transform->GetPosition() - gameObject->transform->GetPosition();
	D3DXVec3Normalize(&vDir, &vDir);

	// 일단은 Y축으로만 회전하자
	float angleY = Vector::GetAngleY(&vDir);
	gameObject->transform->SetRotation(0.0f, angleY, 0.0f);
}

void ComChrControl::CheckAttackTargetDeath()
{
	// 공격 상대가 죽었으면
	if (pAttackTarget && pAttackTarget->IsDeath() == true)
	{
		if (pAttackTarget->gameObject->Tag == eTag_Chracter)
			pAttackTarget->gameObject->SetActive(false);
		else
		{
			ComChrControl* pControl = (ComChrControl*)(pAttackTarget->gameObject->GetComponent("ComChrControl"));
			pControl->Death();
		}
		CancleAttackTarget();
		Stand();
	}
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
			Stand();
		}
		else
		{
			m_pCurrentState->Walk(eAni_Walk);
			GetHeight();

			// 플레이어를 바라보는 방향 벡터
			Vector3 vDir = vMoveToPoint - gameObject->transform->GetPosition();
			D3DXVec3Normalize(&vDir, &vDir);

			// 일단은 Y축으로만 회전하자
			float angleY = Vector::GetAngleY(&vDir);
			gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

			// 이동 속도
			vDir *= m_pCharacter->Status.MOVE_SPEED;

			// 속도벡터 곱하는 방법
			gameObject->transform->Translate(vDir);
		}
	}
}

void ComChrControl::FindAttackTarget()
{
	SceneRPG* sceneRPG = (SceneRPG*)SceneManager::GetInstance()->GetCurrentScene();
	
	// 게임 종료라면
	if (sceneRPG->IsGameEnd)
		return;

	list<GameObject*> listGO = GameObject::FindAll(eTag_Chracter);

	size_t chrCnt = listGO.size();
	int deathCnt = 0;
	for (auto & chr : listGO)
	{
		ComCharacter* comChr = (ComCharacter*)chr->GetComponent("ComCharacter");

		if (comChr->IsDeath() == false)
		{
			pAttackTarget = comChr;
			m_pFollow->pTarget = comChr->gameObject;
			break;
		}
		else
			++deathCnt;
	}

	// 캐릭터 모두 사망시 게임 종료
	if (deathCnt >= chrCnt)
		sceneRPG->IsGameEnd = true;
}

void ComChrControl::SetState(int iIndex)
{
	m_pCurrentState = m_vecState[iIndex];
}

void ComChrControl::Stand()
{
	// 현재 상태에서 Stand로
	m_pCurrentState->Stand(eAni_Stand);
}

void ComChrControl::Walk(float fDeltaZ)
{
	// 현재 상태에서 Walk로
	m_pCurrentState->Walk(eAni_Walk);
	GetHeight();

	Vector3 vecForward;			
	gameObject->transform->GetForward(vecForward);
	Vector3 forward = fDeltaZ * vecForward * m_pCharacter->Status.MOVE_SPEED;
	gameObject->transform->Translate(forward);
}

void ComChrControl::Attack1()
{
	// 현재 상태에서 Attack1로
	m_pCurrentState->Attack1(eAni_Attack_1);
}

void ComChrControl::Death()
{
}

void ComChrControl::Skill1()
{
	m_pCurrentState->Skill1(eAni_Skill_1);
}

void ComChrControl::Skill2()
{
	m_pCurrentState->Skill2(eAni_Skill_2);
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

void ComChrControl::CheckPickingMon()
{
	if (m_pFollow == NULL)
		return;

	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

	list<GameObject*> listMonster = GameObject::FindAll("Monster");

	for (auto & o : listMonster)
	{
		ComRenderCubePN* pCube = (ComRenderCubePN*)o->GetComponent("ComRenderCubePN");

		// 죽었을 때는 몬스터 픽킹을 하지 않는다
		if (pCube->Enable == false)
			continue;

		Ray ray = Ray::RayAtWorldSpace(mousePos.x, mousePos.y);

		vector<Vector3>& vertices = pCube->GetVector();
		for (size_t i = 0; i < vertices.size(); i += 3)
		{
			float dist = 0;
			bool pickMon = ray.CalcIntersectTri(&vertices[i], &dist);

			if (pickMon == true)
			{
				// 몬스터를 따라간다.
				m_pFollow->pTarget = o;
				pAttackTarget = (ComCharacter*)o->GetComponent("ComCharacter");
			}
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

