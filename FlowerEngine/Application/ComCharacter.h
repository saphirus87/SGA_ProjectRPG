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

// 캐릭터의 공통요소입니다.
class ComCharacter : public Component
{
public:
	ComCharacter(CString szName);
	virtual ~ComCharacter();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 다른 콜라이더에 충돌했을 때 호출되는 함수
	virtual void OnTriggerEnter(ComCollider &collider) override;

	// 타겟을 공격합니다.
	void AttackTarget(ComCharacter* pTarget);
	
	// 스킬을 사용하여 타겟을 공격합니다.
	virtual void AttackSkill1(ComCharacter* pTarget) {}
	virtual void AttackSkill2(ComCharacter* pTarget) {}
	virtual void AttackSkill3(ComCharacter* pTarget) {}

	// 방어를 합니다.
	void Defence(int dmg);
	
	// 죽었는지 여부를 반환합니다.
	bool IsDeath();

	// 레벨업
	void LevelUp();

	// 시간에 따라 HP가 회복됩니다.
	void HPMPRecovery();

	// UI 관련
	void UpdateUI();
	
protected:
	void Init();

protected:
	// 캐릭터 타입
	eChrType m_eType;

	// 공격할 타겟
	ComCharacter* m_pAttackTarget;

	// 장비 장착
	ComChrEquipment* m_pChrEquipment;

	// 스킬 정보들
	vector<SkillInfo*> m_vecSkillInfo;

public:
	// 능력치
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
	// 데미지 표시 시간
	CTimer* m_pTimerDamage;

	CTimer* m_pTimerHPRec;
	CTimer* m_pTimerMPRec;
};