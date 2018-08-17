#include "stdafx.h"
#include "ComChrControl.h"
#include "ComObjMap.h"
#include "ComTerrain.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ComChrControl::ComChrControl(CString szName)
	:Component(szName), m_pMap(NULL),
	m_pCurrentState(NULL)
{
}

ComChrControl::~ComChrControl()
{
	for (size_t i = 0; i < m_vecState.size(); ++i)
		SAFE_DELETE(m_vecState[i]);
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
		Walk(1);
	else if (Input::KeyUp('W') || Input::KeyUp(VK_UP))
		Stand();
	if (Input::KeyPress('S') || Input::KeyPress(VK_DOWN))
		Walk(-1);
	else if (Input::KeyUp('S') || Input::KeyUp(VK_DOWN))
		Stand();

	// 캐릭터 공격
	if (Input::KeyDown('F'))
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

void ComChrControl::Init()
{
	GameObject* pObjMap = GameObject::Find("ObjMap");
	if (pObjMap != NULL)
		m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
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
	float fMoveSpeed = 0.10f;	//이동 속도
	gameObject->transform->GetForward(m_vecForward);
	Vector3 forward = fDeltaZ * m_vecForward * fMoveSpeed;
	gameObject->transform->Translate(forward);
}

void ComChrControl::Attack1()
{
	// 현재 상태에서 Attack1로
	m_pCurrentState->Attack1(eAni_Attack_1);
}

