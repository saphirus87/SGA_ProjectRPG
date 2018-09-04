#pragma once
#include "stdafx.h"
#include "StatusInfo.h"

class AttackHandler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

class ComChrEquipment;

// ĳ������ �������Դϴ�.
class ComCharacter : public Component
{
public:
	ComCharacter(CString szName);
	virtual ~ComCharacter();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// �ٸ� �ݶ��̴��� �浹���� �� ȣ��Ǵ� �Լ�
	virtual void OnTriggerEnter(ComCollider &collider) override;

	// Ÿ���� �����մϴ�.
	void AttackTarget(ComCharacter* pTarget);
	
	// �� �մϴ�.
	void Defence(int dmg);
	
	// �׾����� ���θ� ��ȯ�մϴ�.
	bool IsDeath();

	// �ð��� ���� HP�� ȸ���˴ϴ�.
	void HPMPRecovery();

	// UI ����
	void UpdateHPMPBar();

protected:
	void Init();

protected:
	// ĳ���� Ÿ��
	eChrType m_eType;

	ComRenderSkinnedMesh* m_pAnimation;
	
	// ������ Ÿ��
	ComCharacter* m_pAttackTarget;

	// ��� ����
	ComChrEquipment* m_pChrEquipment;

public:
	// �ɷ�ġ
	StatusInfo Status;

protected:
	UIProgressBar* m_pHPBar;
	UIProgressBar* m_pMPBar;

	CTimer* m_pTimerHPRec;
	CTimer* m_pTimerMPRec;
};