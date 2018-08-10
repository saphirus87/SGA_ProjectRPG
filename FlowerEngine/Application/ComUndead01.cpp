#include "stdafx.h"
#include "ComUndead01.h"
#include "ChrState.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"

ComUndead01::ComUndead01(CString szName)
	:Component(szName)
{
}

ComUndead01::~ComUndead01()
{
	SAFE_DELETE(m_pChrState);
}

void ComUndead01::Awake()
{
	//CPP 다형성
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pChrState = new ChrState(m_pAnimation);

	m_pChrState->SetState(new ChrStateStand(m_pAnimation));
	m_pChrState->Stand();
}

void ComUndead01::Update()
{
	if (Input::KeyDown('W'))
	{
		m_pChrState->SetState(new ChrStateWalk(m_pAnimation));
		m_pChrState->Walk();
	}
	//Test Code
	/*if (Input::KeyDown('1'))
	{
		m_pAnimation->PlayAnimation(eAni_Stand);
	}
	if (Input::KeyDown('2'))
	{
		m_pAnimation->PlayAnimation(eAni_Walk);
	}*/
	//애니메이션 추가 코드
	/*if (Input::KeyDown('3'))
	{
		m_pAnimation->PlayAnimation(eAni_WalkBackwards);
	}
	if (Input::KeyDown('4'))
	{
		m_pAnimation->PlayAnimation(eAni_Run);
	}
	if (Input::KeyDown('5'))
	{
		m_pAnimation->PlayAnimation(eAni_RotateLeft);
	}
	if (Input::KeyDown('6'))
	{
		m_pAnimation->PlayAnimation(eAni_RotateRight);
	}*/
}

void ComUndead01::Render()
{
}
