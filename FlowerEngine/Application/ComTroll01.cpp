#include "stdafx.h"
#include "ComTroll01.h"
#include "ComObjMap.h"
#include "ComCharacter.h"

ComTroll01::ComTroll01(CString szName)
	:ComCharacter(szName)
{
}

ComTroll01::~ComTroll01()
{
}

void ComTroll01::Awake()
{
	Init();
}

void ComTroll01::Update()
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

void ComTroll01::Render()
{
}
