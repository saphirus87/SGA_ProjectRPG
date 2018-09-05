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

