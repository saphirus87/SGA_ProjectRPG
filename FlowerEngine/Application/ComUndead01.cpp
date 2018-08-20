#include "stdafx.h"
#include "ComUndead01.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ComCharacter.h"

ComUndead01::ComUndead01(CString szName)
	:ComCharacter(szName)
{
}

ComUndead01::~ComUndead01()
{
}

void ComUndead01::Awake()
{
	Init();
}

void ComUndead01::Update()
{
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
