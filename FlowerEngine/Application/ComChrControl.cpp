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
}

void ComChrControl::Update()
{
	if (m_pCurrentState != m_vecState[eAni_Skill_1])
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
	}
	
	if (Input::ButtonDown(VK_LBUTTON))
		CheckPickingChr();

	if (Input::ButtonDown(VK_RBUTTON))
	{
		CancleAttackTarget();
		// ���Ͱ� Picking���� ������ Map���� �̵� (�ߺ� ��ŷ ����)
		if (CheckPickingMon() == false)
			CheckPickingMap();
	}

	if (m_pFollow != NULL && m_pFollow->IsFollowing)
	{
		m_pCurrentState->Walk(eAni_Walk);
		m_pCharacter->GetHeight();
	}
	else if (m_pFollow != NULL && m_pFollow->AbleAttack)
		Attack1();

	MoveToPoint();
	CheckAttackTargetDeath();

	m_pCurrentState->Update();
}

void ComChrControl::Render()
{
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

	// �÷��̾ �ٶ󺸴� ���� ����
	Vector3 vDir = pAttackTarget->gameObject->transform->GetPosition() - gameObject->transform->GetPosition();
	D3DXVec3Normalize(&vDir, &vDir);

	// �ϴ��� Y�����θ� ȸ������
	float angleY = Vector::GetAngleY(&vDir);
	gameObject->transform->SetRotation(0.0f, angleY, 0.0f);
}

void ComChrControl::CheckAttackTargetDeath()
{
	// ���� ��밡 �׾�����
	if (pAttackTarget && pAttackTarget->IsDeath() == true)
	{
		// ĳ���� ���� ó��
		if (pAttackTarget->gameObject->Tag == eTag_Chracter)
			pAttackTarget->gameObject->SetActive(false);
		else // ���� ���� ó��
		{
			ComChrControl* pControl = (ComChrControl*)(pAttackTarget->gameObject->GetComponent("ComChrControl"));
			pControl->Death();

			// ĳ���� ������ ó��
			if (m_pCharacter->Status.GetEXPAndCheckLevelUp())
				m_pCharacter->LevelUp();
		}
		CancleAttackTarget();
		Stand();
	}
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
			m_pCharacter->GetHeight();

			// �÷��̾ �ٶ󺸴� ���� ����
			Vector3 vDir = vMoveToPoint - gameObject->transform->GetPosition();
			D3DXVec3Normalize(&vDir, &vDir);

			// �ϴ��� Y�����θ� ȸ������
			float angleY = Vector::GetAngleY(&vDir);
			gameObject->transform->SetRotation(0.0f, angleY, 0.0f);

			// �̵� �ӵ�
			vDir *= m_pCharacter->Status.MOVE_SPEED;

			// �ӵ����� ���ϴ� ���
			gameObject->transform->Translate(vDir);
		}
	}
}

void ComChrControl::FindAttackTarget()
{
	SceneRPG* sceneRPG = (SceneRPG*)SceneManager::GetInstance()->GetCurrentScene();
	
	// ���� ������
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

	// ĳ���� ��� ����� ���� ����
	if (deathCnt >= chrCnt)
		sceneRPG->IsGameEnd = true;
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
	// �ɾ ���� ���콺 �̵� ���� ���� (���콺 �̵��� Ű���� �̵��ϸ� �ӵ� 2��Ǵ� ���� ����)
	IsMoveToPoint = false;

	// ���� ���¿��� Walk��
	m_pCurrentState->Walk(eAni_Walk);
	m_pCharacter->GetHeight();

	Vector3 vecForward;			
	gameObject->transform->GetForward(vecForward);
	Vector3 forward = fDeltaZ * vecForward * m_pCharacter->Status.MOVE_SPEED;
	gameObject->transform->Translate(forward);
}

void ComChrControl::Attack1()
{
	// ���� ���¿��� Attack1��
	m_pCurrentState->Attack1(eAni_Attack_1);
}

void ComChrControl::Death()
{
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

bool ComChrControl::CheckPickingMon()
{
	if (m_pFollow == NULL)
		return false;

	Mouse* pMouse = Input::GetInstance()->m_pMouse;
	Vector3 mousePos = Input::GetInstance()->m_pMouse->GetPosition();

	list<GameObject*> listMonster = GameObject::FindAll(eTag_Monster);

	for (auto & o : listMonster)
	{
		ComRenderCubePN* pCube = (ComRenderCubePN*)o->GetComponent("ComRenderCubePN");

		// �׾��� ���� ���� ��ŷ�� ���� �ʴ´�
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
				// ���͸� ���󰣴�.
				m_pFollow->pTarget = o;
				m_pFollow->fMoveSpeed = m_pCharacter->Status.MOVE_SPEED;
				pAttackTarget = (ComCharacter*)o->GetComponent("ComCharacter");
				return true;
			}
		}
	}

	return false;
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

