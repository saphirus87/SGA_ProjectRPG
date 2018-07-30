#include "stdafx.h"
#include "ComAircraft.h"
#include "ComAircraftControl.h"
#include "ComUIPanelDialog.h"

ComAircraft::ComAircraft(CString szName) : 
	Component(szName),
	m_iLevel(1),
	m_iEXP(0),
	m_fMoveSpeed(0.1f),
	m_iEnermyCount(0),
	m_fAttackRange(2.0f)
{
	for (int i = 1; i < 100; ++i)
		m_vecEXPNext.push_back(i);
}

ComAircraft::~ComAircraft()
{
}

void ComAircraft::Awake()
{
	FactoryGameObject factory;

	// ��Ʈ�� �������
	m_pAircraftControl = (ComAircraftControl*)gameObject->GetComponent("ComAircraftControl");
	
	// LEVEL Text �������
	GameObject* pGoUITextLV = factory.CreateUIText(NULL, "UI1Text", "", 0.f, 0.f, 200.f, 100.f);
	m_pTextInfo = (ComUIText*)pGoUITextLV->GetComponent("ComUIText");

	GameObject* pGO = GameObject::Find("UIPanelDialog");
	if (pGO != NULL)
		m_pPanelUI = (ComUIPanelDialog*)pGO->GetComponent("ComUIPanelDialog");
}

void ComAircraft::Update()
{
	CString szLevel;
	szLevel.Format(L"LV. %d\r\nEXP %d/%d\r\n", m_iLevel, CurrentEXP(), NextEXP());
	m_pTextInfo->text = szLevel;

	CString szDebug;
	szDebug.Format(L"EnermyCount : %d", m_iEnermyCount);
	Debug::Get()->EndLine();
	Debug::Get()->AddText(szDebug);
}

void ComAircraft::Render()
{
}

float ComAircraft::AttackRange()
{
	return 10 + m_iLevel * 2.0f;
}

void ComAircraft::CheckGameEnd()
{
	m_iEnermyCount = 0;

	// ���� ���� üũ
	list<GameObject*> listEnermy = GameObject::FindAll("Enermy");

	for (auto & go : listEnermy)
	{
		if (go->IsActive() == true)
			++m_iEnermyCount;
	}

	//m_iEnermyCount = listEnermy.size();
	if (m_iEnermyCount == 1) // ������ ���� �� 1 (�� �ı��� ������Ʈ)
		m_pPanelUI->OpenDialog(100);
}

bool ComAircraft::CheckLevelUp()
{
	// ���� ���� 99
	if (m_iLevel <= 99)
	{
		++m_iEXP;

		for (int i = m_iLevel - 1; i < m_iLevel; ++i)
		{
			if (m_iEXP >= m_vecEXPNext[m_iLevel - 1])
			{
				LevelUp();
				return true;
			}
		}
	}

	return false;
}

void ComAircraft::LevelUp()
{
	// �̵��ӵ� ����
	m_fMoveSpeed += 0.05f;
	
	// ���� EXP
	m_iEXP = 0;

	// ��ȭâ
 	m_pPanelUI->OpenDialog(m_iLevel++);
	
	FactoryGameObject factory;
	// ���� ��ȭ
	factory.CreateAircraft("Aircraft", m_iLevel);
}

