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

// ĳ������ �������Դϴ�.
class ComCharacter : public Component
{
public:
	ComCharacter(CString szName);
	virtual ~ComCharacter();

protected: 
	void Init();
	void InitPlayer();

public:
	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// �ٸ� �ݶ��̴��� �浹���� �� ȣ��Ǵ� �Լ�
	virtual void OnTriggerEnter(ComCollider &collider) override;

	/// ���� ����
	// Ÿ���� �����մϴ�.
	void AttackTarget(ComCharacter* pTarget);
	// �� �մϴ�.
	void Defence(int Damage, bool bCritical);
	// ��ų�� ����Ͽ� Ÿ���� �����մϴ�.
	virtual void AttackSkill1(ComCharacter* pTarget) {}
	virtual void AttackSkill2(ComCharacter* pTarget) {}
	virtual void AttackSkill3(ComCharacter* pTarget) {}
	// ���� ����� �ٶ󺾴ϴ�.
	void LookatTarget();
	// ���� ����� ����մϴ�.
	void CancleAttackTarget();
	/// ���� ����
	// �׾����� ���θ� ��ȯ�մϴ�.
	bool IsDeath();
	// ���� ����� �׾��� �� ó���� �մϴ�.
	void CheckMonDeath();
	// �� ��ü�� ��ŷ�Ǿ����� ���θ� �˻��մϴ�. (ĳ����, ����, ��)
	bool CheckPickingMon();
	// �ð��� ���� HP�� ȸ���˴ϴ�.
	void HPMPRecovery();

	/// �� ����
	// �ʿ��� ���̸� ���ɴϴ�.
public:	void GetHeight();
protected:
	ComObjMap * m_pMap;
	// �ʱ� ĳ���� ���ý� �� ���� �ִ��� ����
	bool IsGroud;

protected:
	// ĳ���� Ÿ��
	eChrType m_eType;

	// ������ Ÿ��
public: ComCharacter* pAttackTarget;

protected:
	// Ÿ������ ����
	ComFollowTarget * m_pFollow;

	// ��� ����
	ComChrEquipment* m_pChrEquipment;

	// ��ų ������
	vector<SkillInfo*> m_vecSkillInfo;

public:
	// �ɷ�ġ
	StatusInfo* Status;

	ComRenderSkinnedMesh* m_pAnimation;
	AttackHandler* m_pAttackHandler;
	Skill1Handler* m_pSkill1Handler;

	/// ���±�� ����
	// ���µ�
	vector<IChrState*> m_vecState;
	// ���� ����
	IChrState * m_pCurrentState;
	void SetState(int iIndex);
	virtual void Stand();
	virtual void Walk(float fDeltaZ);
	virtual void Attack1();
	virtual void Death();

	/// UI ����
protected:
	void UpdateUI();

protected:
	/// ĳ���� ����
	// ĳ���� ���� �ؽ�Ʈ ǥ��
	UIText* m_pUILevel;
	// ĳ���� ����ġ �ؽ�Ʈ ǥ��
	UIText* m_pUIEXP;
	// ĳ���� �� �̹���
	UIImage* m_pFace;
	/// ������ ǥ��
	// �Ӹ����� ������ ǥ��
	ComText3D* m_pComUIDamage;
	ComText3D* m_pComUICritical;
	// ������ ǥ�� �ð�
	CTimer* m_pTimerDamage;
	CTimer* m_pTimerCritical;
	/// HP/MP ����
	// HP/MP��
	UIProgressBar* m_pHPBar;
	UIProgressBar* m_pMPBar;
	// HP/MP ȸ���ð� üũ�ϱ� ����
	CTimer* m_pTimerHPRec;
	CTimer* m_pTimerMPRec;
};