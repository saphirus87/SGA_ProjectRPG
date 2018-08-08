#include "stdafx.h"
#include "ComUndead01.h"

ComUndead01::ComUndead01(CString szName)
	:Component(szName)
{
}

ComUndead01::~ComUndead01()
{
}

void ComUndead01::Awake()
{
	//CPP ´ÙÇü¼º
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}

void ComUndead01::Update()
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

void ComUndead01::Render()
{
}
