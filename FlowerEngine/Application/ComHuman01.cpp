#include "stdafx.h"
#include "ComHuman01.h"


ComHuman01::ComHuman01(CString szName) : 
	Component(szName)
{
}


ComHuman01::~ComHuman01()
{
}

void ComHuman01::Awake()
{
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
}

void ComHuman01::Update()
{
	if (Input::KeyDown('1'))
		m_pAnimation->PlayAnimation(0);
	
	if (Input::KeyDown('2'))
		m_pAnimation->PlayAnimation(1);
}

void ComHuman01::Render()
{
}
