#include "stdafx.h"
#include "ComTroll01.h"
#include "ComObjMap.h"

ComTroll01::ComTroll01(CString szName)
	:Component(szName)
{
}

ComTroll01::~ComTroll01()
{
}

void ComTroll01::Awake()
{
	//CPP ´ÙÇü¼º
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}

void ComTroll01::Update()
{
	//Test Code
	if (Input::KeyDown('1'))
	{
		m_pAnimation->PlayAnimation(eAni_Stand);
	}
	if (Input::KeyDown('2'))
	{
		m_pAnimation->PlayAnimation(eAni_Walk);
	}
}

void ComTroll01::Render()
{
}
