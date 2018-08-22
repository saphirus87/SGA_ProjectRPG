#include "stdafx.h"
#include "ComHuman01.h"
#include "ComChrEquipment.h"
#include "ComEquipment.h"
#include "ItemInfo.h"
#include "ComCharacter.h"
#include "ComChrControl.h"

ComHuman01::ComHuman01(CString szName) : 
	ComCharacter(szName)
{
}

ComHuman01::~ComHuman01()
{
}

void ComHuman01::Awake()
{
	Init();

	vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;
	vecKeyFrameAnimSet.resize(eAni_COUNT);

	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	// Register 하는 순서데로 Animation Index가 설정되기 때문에 미리 모두 Unregister 한다.
	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

	float fPeriod = vecKeyFrameAnimSet[eAni_Attack_1]->GetPeriod();
	float fSrcTime = vecKeyFrameAnimSet[eAni_Attack_1]->GetSourceTicksPerSecond();	// 4800

	D3DXKEY_CALLBACK attackKey;
	attackKey.pCallbackData = this;
	attackKey.Time = float(fPeriod / fSrcTime);

	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_3], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}

void ComHuman01::Update()
{
	
}

void ComHuman01::Render()
{
}

void ComHuman01::OnTriggerEnter(ComCollider & collider)
{
	if (collider.gameObject->Name().Find(L"Equipment_shoulder") >= 0)
	{
		ComChrEquipment* pEquipment = (ComChrEquipment*)gameObject->GetComponent("ComChrEquipment");
		if (pEquipment != NULL)
		{
			ComEquipment* pEquip = (ComEquipment*)collider.gameObject->GetComponent("ComEquipment");
			pEquipment->Equip(pEquip->pItemInfo);
			collider.gameObject->SetActive(false);
		}
	}
}

HRESULT HumanAttackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	CString szDebug;
	szDebug.Format(L"EventCallback Track : %d\r\n", Track);
	OutputDebugString(szDebug);

	// 특정 프레임에서 공격
	ComCharacter* pChr = (ComCharacter*)pCallbackData;
	ComChrControl* pControl = (ComChrControl*)pChr->gameObject->GetComponent("ComChrControl");
	pChr->AttackTarget(pControl->pAttackTarget);

	return S_OK;
}