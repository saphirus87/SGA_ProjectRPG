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
	//CPP ´ÙÇü¼º
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
	else if (Input::KeyUp('W'))
	{
		m_pChrState->Stand();
	}
	
	//Test Code
	if (Input::KeyDown('1'))
		m_pAnimation->PlayAnimation(eAni_Stand);

	if (Input::KeyDown('2'))
		m_pAnimation->PlayAnimation(eAni_Walk);

	if (Input::KeyDown('3'))
		m_pAnimation->PlayAnimation(eAni_Attack_1);

	if (Input::KeyDown('4'))
		m_pAnimation->PlayAnimation(eAni_Attack_2);

	if (Input::KeyDown('5'))
		m_pAnimation->PlayAnimation(eAni_Attack_3);
}

void ComUndead01::Render()
{
}
