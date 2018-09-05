/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

���� :
ĳ������ �ΰ��Դϴ�.
*/
#pragma once
#include "stdafx.h"
#include "ComCharacter.h"

class ComChrEquipment;
class SkillInfo;

class ComHuman : public ComCharacter, public UIButtonDelegate
{
public:
	ComHuman(CString szName);
	~ComHuman();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetAniEvent();

	// UIButtonDelegate��(��) ���� ��ӵ�
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;

	void Skill1();
	void Skill2();
	void Skill3();

	// ��ų�� ����Ͽ� Ÿ���� �����մϴ�.
	void AttackSkill1(ComCharacter* pTarget) override;
	void AttackSkill2(ComCharacter* pTarget) override;
	void AttackSkill3(ComCharacter* pTarget) override;

private:
	// Skill1 ��Ÿ�� �ؽ�Ʈ
	UIText * uiTextCoolTimeSkill1;

	// ��ų ������
	vector<SkillInfo*> m_vecSkillInfo;
};