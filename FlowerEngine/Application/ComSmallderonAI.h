#pragma once
#include "stdafx.h"
#include "ComCharacter.h"
#include "StatusInfo.h"

class ComObjMap;
class ComFollowTarget;
class IChrState;

// Smallderon�� �ΰ�����(AI)
class ComSmallderonAI : public ComCharacter
{
public:
	ComSmallderonAI(CString szName);
	~ComSmallderonAI();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	//���� ���
	void Stand() override;
	void Walk(float fDeltaZ) override;
	void Attack1() override;
	void Death() override;

	// ������ ����� ã���ϴ�.
	void FindAttackTarget();
	// ĳ���Ͱ� �׾����� Ȯ���ϰ� ó���մϴ�.
	void CheckPlayerDeath();

private:
	CTimer* m_pTimerAttack;
};

