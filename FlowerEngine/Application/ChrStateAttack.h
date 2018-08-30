#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateAttack1 : public IChrState
{
private:
	ComChrControl * m_pControl;

public:
	ChrStateAttack1(ComChrControl* pControl);
	~ChrStateAttack1();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};

class ChrStateSkill1 : public IChrState
{
private:
	ComChrControl * m_pControl;

	// 애니메이션 끝남 체크용
	CTimer* m_pTimerAnim;
	
	// 쿨타임인지 여부
	bool m_IsCoolTime;
public:
	// 쿨타임 외부에서 스킬 정보 셋팅 가능
	float CoolTime;
	// 이 스킬 쿨타임
	CTimer* m_pTimerCool;

public:
	ChrStateSkill1(ComChrControl* pControl);
	~ChrStateSkill1();

	void Update() override;

	// IChrState을(를) 통해 상속됨
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};

class ChrStateSkill2 : public IChrState
{
private:
	ComChrControl * m_pControl;

public:
	ChrStateSkill2(ComChrControl* pControl);
	~ChrStateSkill2();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};