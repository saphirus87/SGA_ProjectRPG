#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComCharacter;

class ChrStateAttack1 : public IChrState
{
private:
	ComCharacter * m_pControl;

public:
	ChrStateAttack1(ComCharacter* pControl);
	~ChrStateAttack1();

	// IChrState��(��) ���� ��ӵ�
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
	ComCharacter * m_pControl;
	
public:
	// �ִϸ��̼� ���� üũ��
	CTimer * m_pTimerAnim;
	// �� ��ų ��Ÿ��
	CTimer* m_pTimerCool;
	// ��Ÿ������ ����
	bool IsCoolTime;
	
public:
	ChrStateSkill1(ComCharacter* pControl);
	~ChrStateSkill1();

	void Update() override;

	// IChrState��(��) ���� ��ӵ�
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
	ComCharacter * m_pControl;

public:
	ChrStateSkill2(ComCharacter* pControl);
	~ChrStateSkill2();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};