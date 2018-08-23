#include "stdafx.h"
#include "ComCharacter.h"
#include "ComChrControl.h"
#include "ComFollowTarget.h"

ComCharacter::ComCharacter(CString szName) : 
	Component(szName),
	m_pAnimation(NULL),
	m_pAttackTarget(NULL),
	m_eType(eChrType_COUNT) // 초기화 값으로 사용
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

	// Register 하는 순서데로 Animation Index가 설정되기 때문에 미리 모두 Unregister 한다.
	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->UnregisterAnimationSet(m_vecKeyFrameAnimSet[i]);
}

void ComCharacter::AttackTarget(ComCharacter * pTarget)
{
	m_pAttackTarget = pTarget;
	ComChrControl* pControl = (ComChrControl*)(gameObject->GetComponent("ComChrControl"));
	pControl->LookatTarget();

	// 총 공격력 계산 (내 공격력 + 장비 공격력)
	int dmg = m_status.ATK_PHY;

	pTarget->Defence(m_status.ATK_PHY);
}

void ComCharacter::Defence(int dmg)
{
	// 총 방어력 계산 (내 방어력 + 장비 방어력)
	int def = m_status.DEF_PHY;

	dmg -= def;

	// HP 차감
	m_status.HP -= dmg;
	
	// UI 갱신
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
	// CPP 다형성
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pChrEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");
	
	AnimationCompress();
	SetAniEvent();
}

HRESULT AttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	// 특정 프레임에서 공격
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");

	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d %s\r\n", Track, pChr->gameObject->Name());
	OutputDebugString(szDebug);

	// 죽어서 없으면
	if (pControl->pAttackTarget == NULL)
		return S_OK;

	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}