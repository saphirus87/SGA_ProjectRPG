#include "stdafx.h"
#include "ComChrControl.h"
#include "ComTerrain.h"

ComChrControl::ComChrControl(CString szName)
	:Component(szName), 
	m_isMoving(false), 
	m_isRotating(false), 
	m_pos(NULL),
	m_pMap(NULL)
{
}

ComChrControl::~ComChrControl()
{
}

void ComChrControl::Awake()
{
	GameObject* pObjMap = GameObject::Find("ObjMap");
	if (pObjMap != NULL)
		m_pMap = (ComTerrain*)pObjMap->GetComponent("ComTerrain");

	m_pos = &gameObject->transform->GetPosition();
}

void ComChrControl::Update()
{
	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.f;
	if (m_pMap != NULL && m_pMap->GetHeight(fHeight, pos) == true)
	{
		pos.y = fHeight;
		gameObject->transform->SetPosition(pos);
	}

	Move();
}

void ComChrControl::Render()
{
}

void ComChrControl::Move()
{
	if (Input::KeyPress('A') || Input::KeyPress(VK_LEFT))
	{
		m_isRotating = true;
		gameObject->transform->RotateY(-0.1f);
	}
	if (Input::KeyPress('D') || Input::KeyPress(VK_RIGHT))
	{
		m_isRotating = true;
		gameObject->transform->RotateY(0.1f);
	}

	float fDeltaZ = 0;
	if (Input::KeyPress('W') || Input::KeyPress(VK_UP))
	{
		m_isMoving = true;
		fDeltaZ = 1;
	}
	if (Input::KeyPress('S') || Input::KeyPress(VK_DOWN))
	{
		m_isMoving = true;
		fDeltaZ = -1;
	}
	else
		m_isMoving = false;

	float fMoveSpeed = 0.5f;	//이동 속도
	gameObject->transform->GetForward(m_vecForward);
	Vector3 forward = fDeltaZ * m_vecForward * fMoveSpeed;
	gameObject->transform->Translate(forward);
}
