#pragma once
class SkillInfo
{
public:
	SkillInfo();
	virtual ~SkillInfo();

	// ��ų �̸�
	CString szName;

	// ��ų ID
	unsigned int UID;

	// ��� MP
	int iUseMP;

	// ��Ÿ��
	float fCoolTime;
};

