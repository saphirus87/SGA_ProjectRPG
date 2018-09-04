#pragma once
#include "stdafx.h"
#include "StatusInfo.h"

class AttackHandler : public ID3DXAnimationCallbackHandler
{
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};

class ComChrEquipment;

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
	
	// 방어를 합니다.
	void Defence(int dmg);
	
	// 죽었는지 여부를 반환합니다.
	bool IsDeath();

	// 시간에 따라 HP가 회복됩니다.
	void HPMPRecovery();

	// UI 관련
	void UpdateHPMPBar();

protected:
	void Init();

protected:
	// 캐릭터 타입
	eChrType m_eType;

	ComRenderSkinnedMesh* m_pAnimation;
	
	// 공격할 타겟
	ComCharacter* m_pAttackTarget;

	// 장비 장착
	ComChrEquipment* m_pChrEquipment;

public:
	// 능력치
	StatusInfo Status;

protected:
	UIProgressBar* m_pHPBar;
	UIProgressBar* m_pMPBar;

	CTimer* m_pTimerHPRec;
	CTimer* m_pTimerMPRec;
};