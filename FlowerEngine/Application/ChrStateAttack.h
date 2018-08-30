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
	ComChrControl * m_pControl;

	// �ִϸ��̼� ���� üũ��
	CTimer* m_pTimerAnim;
	
	// ��Ÿ������ ����
	bool m_IsCoolTime;
public:
	// ��Ÿ�� �ܺο��� ��ų ���� ���� ����
	float CoolTime;
	// �� ��ų ��Ÿ��
	CTimer* m_pTimerCool;

public:
	ChrStateSkill1(ComChrControl* pControl);
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
	ComChrControl * m_pControl;

public:
	ChrStateSkill2(ComChrControl* pControl);
	~ChrStateSkill2();

	// IChrState��(��) ���� ��ӵ�
	virtual void Stand(int iIndex) override;
	virtual void Walk(int iIndex) override;
	virtual void Attack1(int iIndex) override;
	virtual void Skill1(int iIndex) override;
	virtual void Skill2(int iIndex) override;
	virtual void Death(int iIndex) override;
};