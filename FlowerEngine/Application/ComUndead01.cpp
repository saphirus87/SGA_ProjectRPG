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
	
	GameObject* pUIBar = GameObject::Find("testUI");
	ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

	uiDialog->AddProgressBar(eUI_HPBar, "Resources/ui/6.tga");

	m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar);
	m_pHPBar->SetMaxValue(m_status.HPMAX);
	UpdateHPBar();
}

void ComUndead01::Update()
{
}

void ComUndead01::Render()
{
}

void ComUndead01::UpdateHPBar()
{
	m_pHPBar->SetCurValue(m_status.HP);
}
