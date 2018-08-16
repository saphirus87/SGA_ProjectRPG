#include "stdafx.h"
#include "ComChrControl.h"
#include "ComObjMap.h"
#include "ComTerrain.h"
#include "IChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ComChrControl::ComChrControl(CString szName)
	:Component(szName), 
	m_pMap(NULL),
	m_pCurrentState(NULL)
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

	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pCurrentState = new ChrStateStand(m_pAnimation);
}

void ComChrControl::Update()
{
	if (Input::KeyPress('A') || Input::KeyPress(VK_LEFT))
	{
		gameObject->transform->RotateY(-0.1f);
	}
	if (Input::KeyPress('D') || Input::KeyPress(VK_RIGHT))
	{
		gameObject->transform->RotateY(0.1f);
	}

	if (Input::KeyPress('W') || Input::KeyPress(VK_UP))
	{
		Walk(1);
	}
	else if (Input::KeyUp('W') || Input::KeyUp(VK_UP))
	{
		Stand();
	}
	if (Input::KeyPress('S') || Input::KeyPress(VK_DOWN))
	{
		Walk(-1);
	}
	else if (Input::KeyUp('S') || Input::KeyUp(VK_DOWN))
	{
		Stand();
	}

	if (Input::KeyDown('F'))
	{
		Attack();
	}


}

void ComChrControl::Render()
{
}

void ComChrControl::GetHeight()
{
	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.f;
	if (m_pMap != NULL && m_pMap->GetHeight(fHeight, pos) == true)
	{
		pos.y = fHeight;
		gameObject->transform->SetPosition(pos);
	}
}

void ComChrControl::Move()
{
	
}

void ComChrControl::SetState(IChrState * pChrState)
{
	SAFE_DELETE(m_pCurrentState);
	m_pCurrentState = pChrState;
}

void ComChrControl::Stand()
{
	m_pCurrentState->Stand(this);
}

void ComChrControl::Walk(float fDeltaZ)
{
	m_pCurrentState->Walk(this);
	GetHeight();
	float fMoveSpeed = 0.10f;	//이동 속도
	gameObject->transform->GetForward(m_vecForward);
	Vector3 forward = fDeltaZ * m_vecForward * fMoveSpeed;
	gameObject->transform->Translate(forward);
}

void ComChrControl::Attack()
{
	m_pCurrentState->Attack(this);
}

