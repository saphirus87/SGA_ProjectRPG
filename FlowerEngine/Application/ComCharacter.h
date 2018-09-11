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
class ComObjMap;
class ComFollowTarget;
class IChrState;

// 캐릭터의 공통요소입니다.
class ComCharacter : public Component
{
public:
	ComCharacter(CString szName);
	virtual ~ComCharacter();

protected: 
	void Init();
	void InitPlayer();

public:
	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 다른 콜라이더에 충돌했을 때 호출되는 함수
	virtual void OnTriggerEnter(ComCollider &collider) override;

	/// 전투 관련
	// 타겟을 공격합니다.
	void AttackTarget(ComCharacter* pTarget);
	// 방어를 합니다.
	void Defence(int Damage, bool bCritical);
	// 스킬을 사용하여 타겟을 공격합니다.
	virtual void AttackSkill1(ComCharacter* pTarget) {}
	virtual void AttackSkill2(ComCharacter* pTarget) {}
	virtual void AttackSkill3(ComCharacter* pTarget) {}
	// 공격 대상을 바라봅니다.
	void LookatTarget();
	// 공격 대상을 취소합니다.
	void CancleAttackTarget();
	/// 죽음 관련
	// 죽었는지 여부를 반환합니다.
	bool IsDeath();
	// 공격 대상이 죽었을 때 처리를 합니다.
	void CheckMonDeath();
	// 이 객체가 픽킹되었는지 여부를 검사합니다. (캐릭터, 몬스터, 맵)
	bool CheckPickingMon();
	// 시간에 따라 HP가 회복됩니다.
	void HPMPRecovery();

	/// 맵 관련
	// 맵에서 높이를 얻어옵니다.
public:	void GetHeight();
protected:
	ComObjMap * m_pMap;
	// 초기 캐릭터 셋팅시 땅 위에 있는지 여부
	bool IsGroud;

protected:
	// 캐릭터 타입
	eChrType m_eType;

	// 공격할 타겟
public: ComCharacter* pAttackTarget;

protected:
	// 타겟으로 따라감
	ComFollowTarget * m_pFollow;

	// 장비 장착
	ComChrEquipment* m_pChrEquipment;

	// 스킬 정보들
	vector<SkillInfo*> m_vecSkillInfo;

public:
	// 능력치
	StatusInfo* Status;

	ComRenderSkinnedMesh* m_pAnimation;
	AttackHandler* m_pAttackHandler;
	Skill1Handler* m_pSkill1Handler;

	/// 상태기계 관련
	// 상태들
	vector<IChrState*> m_vecState;
	// 현재 상태
	IChrState * m_pCurrentState;
	void SetState(int iIndex);
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();
	virtual void Death();

	/// UI 관련
protected:
	void UpdateUI();

protected:
	/// 캐릭터 정보
	// 캐릭터 레벨 텍스트 표시
	UIText* m_pUILevel;
	// 캐릭터 경험치 텍스트 표시
	UIText* m_pUIEXP;
	// 캐릭터 얼굴 이미지
	UIImage* m_pFace;
	/// 데미지 표시
	// 머리위에 데미지 표시
	ComText3D* m_pComUIDamage;
	ComText3D* m_pComUICritical;
	// 데미지 표시 시간
	CTimer* m_pTimerDamage;
	CTimer* m_pTimerCritical;
	/// HP/MP 관련
	// HP/MP바
	UIProgressBar* m_pHPBar;
	UIProgressBar* m_pMPBar;
	// HP/MP 회복시간 체크하기 위한
	CTimer* m_pTimerHPRec;
	CTimer* m_pTimerMPRec;
};