#include "stdafx.h"
#include "ComCharacter.h"
#include "ComChrControl.h"
#include "ComFollowTarget.h"
#include "ComChrEquipment.h"

ComCharacter::ComCharacter(CString szName) : 
	Component(szName),
	m_pAnimation(NULL),
	m_pChrEquipment(NULL),
	m_pAttackTarget(NULL),
	m_eType(eChrType_COUNT) // �ʱ�ȭ ������ ���
{
}


ComCharacter::~ComCharacter()
{
}

void ComCharacter::Awake()
{
	Init();
}

void ComCharacter::Update()
{
	
}

void ComCharacter::Render()
{
}

void ComCharacter::AttackTarget(ComCharacter * pTarget)
{
	m_pAttackTarget = pTarget;
	ComChrControl* pControl = (ComChrControl*)(gameObject->GetComponent("ComChrControl"));
	pControl->LookatTarget();

	// �� ���ݷ� ��� (�� ���ݷ� + ��� ���ݷ�)
	int equipmentDmg = 0;
	if (m_pChrEquipment)
		equipmentDmg = m_pChrEquipment->GetTotalATK_MIN();

	int dmg = Status.ATK_PHY + equipmentDmg;

	pTarget->Defence(dmg);
}

void ComCharacter::Defence(int dmg)
{
	// �� ���� ��� (�� ���� + ��� ����)
	int equipmentDef = 0;
	if (m_pChrEquipment)
		equipmentDef = m_pChrEquipment->GetTotalDEF_PHY();

	int def = Status.DEF_PHY + equipmentDef;

	dmg -= (def / 2);

	// HP ����
	Status.HP -= dmg;
	
	// UI ����
	UpdateHPBar();
}

bool ComCharacter::CheckDeath()
{
	if (Status.HP <= 0)
		return true;

	return false;
}

void ComCharacter::Init()
{
	// CPP ������
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pChrEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");
}

HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	// Ư�� �����ӿ��� ����
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// �׾ ������
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}