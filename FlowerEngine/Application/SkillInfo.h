#pragma once
class SkillInfo
{
public:
	SkillInfo();
	virtual ~SkillInfo();

	// 스킬 이름
	CString szName;

	// 스킬 ID
	unsigned int UID;

	// 사용 MP
	int iUseMP;

	// 쿨타임
	float fCoolTime;
};

// 휴먼 스킬1
class HumanSkill1 : public SkillInfo
{
public:
	HumanSkill1();
	~HumanSkill1();

	// 스킬 추가데미지
	int iAddSkillDmg;

	// 스킬 공격 거리
	float fRange;
};