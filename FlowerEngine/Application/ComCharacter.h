#pragma once
#include "stdafx.h"
#include "StatusInfo.h"

class AttackHandler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

class Skill1Handler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

class ComChrEquipment;
class SkillInfo;

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
	
	// ��ų�� ����Ͽ� Ÿ���� �����մϴ�.
	virtual void AttackSkill1(ComCharacter* pTarget) {}
	virtual void AttackSkill2(ComCharacter* pTarget) {}
	virtual void AttackSkill3(ComCharacter* pTarget) {}

	// �� �մϴ�.
	void Defence(int dmg);
	
	// �׾����� ���θ� ��ȯ�մϴ�.
	bool IsDeath();

	// ������
	void LevelUp();

	// �ð��� ���� HP�� ȸ���˴ϴ�.
	void HPMPRecovery();

	// UI ����
	void UpdateUI();
	
protected:
	void Init();

protected:
	// ĳ���� Ÿ��
	eChrType m_eType;

	// ������ Ÿ��
	ComCharacter* m_pAttackTarget;

	// ��� ����
	ComChrEquipment* m_pChrEquipment;

	// ��ų ������
	vector<SkillInfo*> m_vecSkillInfo;

public:
	// �ɷ�ġ
	StatusInfo Status;

	ComRenderSkinnedMesh* m_pAnimation;
	AttackHandler* m_pAttackHandler;
	Skill1Handler* m_pSkill1Handler;

	// UI
protected:
	UIText* m_pUILevel;
	UIText* m_pUIEXP;
	UIProgressBar* m_pHPBar;
	UIProgressBar* m_pMPBar;
	UIImage* m_pFace;

	ComText3D* m_pComUIDamage;
	// ������ ǥ�� �ð�
	CTimer* m_pTimerDamage;

	CTimer* m_pTimerHPRec;
	CTimer* m_pTimerMPRec;
};