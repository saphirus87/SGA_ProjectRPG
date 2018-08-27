#include "stdafx.h"
#include "ComTroll01.h"
#include "ComObjMap.h"
#include "ComCharacter.h"

ComTroll01::ComTroll01(CString szName)
	:ComCharacter(szName)
{
	m_eType = eChrType_Troll;
}

ComTroll01::~ComTroll01()
{
}

void ComTroll01::Awake()
{
	Init();

	GameObject* pUIBar = GameObject::Find("testUI");
	ComDialog* uiDialog = (ComDialog*)pUIBar->GetComponent("ComDialog");

	uiDialog->AddProgressBar(eUI_HPBar_Troll, "Resources/ui/6.tga");

	m_pHPBar = uiDialog->GetProgressBar(eUI_HPBar_Troll);
	m_pHPBar->SetPosition(Vector3(800, 0, 0));
	m_pHPBar->SetMaxValue(Status.HPMAX);
	UpdateHPBar();

	SetAniEvent();
}

void ComTroll01::Update()
{
}

void ComTroll01::Render()
{
}

void ComTroll01::SetAniEvent()
{
	// �ִϸ��̼� Ű�����Ӽ�
	vector<LPD3DXKEYFRAMEDANIMATIONSET> vecKeyFrameAnimSet;

	vecKeyFrameAnimSet.resize(eAni_COUNT);

	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->GetAnimationSet(i, (LPD3DXANIMATIONSET*)&vecKeyFrameAnimSet[i]);

	// Register �ϴ� �������� Animation Index�� �����Ǳ� ������ �̸� ��� Unregister �Ѵ�.
	for (int i = eAni_Attack_3; i < eAni_COUNT; ++i)
		m_pAnimation->m_pAniControl->UnregisterAnimationSet(vecKeyFrameAnimSet[i]);

	// �ʴ� �߻��ϴ� �ִϸ��̼� Ű ������ ƽ�� ���� �����ɴϴ�.
	float fSrcTime = vecKeyFrameAnimSet[eAni_Attack_1]->GetSourceTicksPerSecond();	// 4800

	// eAni_Attack_1 �� ������ �� : 29
	// eAni_Attack_1 ������ �ִ� ������ Number : 12
	// ��ʽ� 12 : 29 = x : 4800(SrcTime)
	float x = fSrcTime * 12 / 29;
	
	// Ű �̺�Ʈ �ݹ�
	D3DXKEY_CALLBACK attackKey;
	attackKey.pCallbackData = this;
	attackKey.Time = x;

	// eAni �������� �߰��Ѵ�.
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_3], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_2], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Attack_1], 1, &attackKey, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Walk], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);
	m_pAnimation->AddCallbackKeysAndCompress(vecKeyFrameAnimSet[eAni_Stand], 0, NULL, D3DXCOMPRESS_DEFAULT, 1.0f);

	vecKeyFrameAnimSet.clear();
}
