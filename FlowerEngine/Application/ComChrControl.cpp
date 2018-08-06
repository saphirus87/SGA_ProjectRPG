#include "stdafx.h"
#include "ComChrControl.h"

ComChrControl::ComChrControl(CString szName)
	:Component(szName), 
	m_isMoving(false), 
	m_pos(NULL)
{
}

ComChrControl::~ComChrControl()
{
}

void ComChrControl::Awake()
{
	m_pos = &gameObject->transform->GetPosition();
	m_pTimer = new CTimer(CClock::GetInstance());
	m_pTimer->Start();
}

void ComChrControl::Update()
{
	Move();

}

void ComChrControl::Render()
{
}

void ComChrControl::Move()
{
	if (Input::KeyPress('A') || Input::KeyPress(VK_LEFT))
	{
		gameObject->transform->RotateY(-0.1f);
	}
	if (Input::KeyPress('D') || Input::KeyPress(VK_RIGHT))
	{
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
