/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

설명 :
캐릭터중 인간입니다.
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

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetAniEvent();

	// UIButtonDelegate을(를) 통해 상속됨
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;

	void Skill1();
	void Skill2();
	void Skill3();

	// 스킬을 사용하여 타겟을 공격합니다.
	void AttackSkill1(ComCharacter* pTarget) override;
	void AttackSkill2(ComCharacter* pTarget) override;
	void AttackSkill3(ComCharacter* pTarget) override;

private:
	// Skill1 쿨타임 텍스트
	UIText * uiTextCoolTimeSkill1;

	// 스킬 정보들
	vector<SkillInfo*> m_vecSkillInfo;
};