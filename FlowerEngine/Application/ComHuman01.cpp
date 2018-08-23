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
	m_eType = eChrType_Human;
}

ComHuman01::~ComHuman01()
{
}

void ComHuman01::Awake()
{
	Init();
}

void ComHuman01::SetAniEvent()
{
	// �ʴ� �߻��ϴ� �ִϸ��̼� Ű ������ ƽ�� ���� �����ɴϴ�.
	float fSrcTime = m_vecKeyFrameAnimSet[eAni_Attack_1]->GetSourceTicksPerSecond();	// 4800

	// eAni_Attack_1 �� ������ �� : 29
	// eAni_Attack_1 ������ �ִ� ������ Number : 12
	// ��ʽ� 12 : 29 = x : 4800(SrcTime)
	float x = fSrcTime * 12 / 29;
	
	// Ű �̺�Ʈ �ݹ�
	D3DXKEY_CALLBACK attackKey;
	attackKey.pCallbackData = this;
	attackKey.Time = x;

	// eAni �������� �߰��Ѵ�.
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Attack_3], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Attack_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(m_vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	m_vecKeyFrameAnimSet.clear();
}

void ComHuman01::Update()
{
	
}

void ComHuman01::Render()
{
}

void ComHuman01::OnTriggerEnter(ComCollider & collider)
{
	if (collider.gameObject->Tag == eTag_Item)
	{
		if (m_pChrEquipment != NULL)
		{
			ComEquipment* pEquip = (ComEquipment*)collider.gameObject->GetComponent("ComEquipment");
			m_pChrEquipment->Equip(pEquip->pItemInfo);
			collider.gameObject->SetActive(false);
		}
	}
}
