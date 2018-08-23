#include "stdafx.h"
#include "ComCharacter.h"
#include "ComChrControl.h"
#include "ComFollowTarget.h"

ComCharacter::ComCharacter(CString szName) : 
	Component(szName),
	m_pAnimation(NULL),
	m_pAttackTarget(NULL),
	m_eType(eChrType_COUNT) // �ʱ�ȭ ������ ���
{
}


ComCharacter::~ComCharacter()
{
	m_vecKeyFrameAnimSet.clear();
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

void ComCharacter::AnimationCompress()
{
	m_vecKeyFrameAnimSet.resize(eAni_COUNT);

	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&m_vecKeyFrameAnimSet[i]);

	// Register �ϴ� �������� Animation Index�� �����Ǳ� ������ �̸� ��� Unregister �Ѵ�.
	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->UnregisterAnimationSet(m_vecKeyFrameAnimSet[i]);
}

void ComCharacter::AttackTarget(ComCharacter * pTarget)
{
	m_pAttackTarget = pTarget;
	ComChrControl* pControl = (ComChrControl*)(gameObject->GetComponent("ComChrControl"));
	pControl->LookatTarget();

	// �� ���ݷ� ��� (�� ���ݷ� + ��� ���ݷ�)
	int dmg = m_status.ATK_PHY;

	pTarget->Defence(m_status.ATK_PHY);
}

void ComCharacter::Defence(int dmg)
{
	// �� ���� ��� (�� ���� + ��� ����)
	int def = m_status.DEF_PHY;

	dmg -= def;

	// HP ����
	m_status.HP -= dmg;
	
	// UI ����
	UpdateHPBar();
}

bool ComCharacter::CheckDeath()
{
	if (m_status.HP <= 0)
		return true;

	return false;
}

void ComCharacter::Init()
{
	// CPP ������
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pChrEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");
	
	AnimationCompress();
	SetAniEvent();
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