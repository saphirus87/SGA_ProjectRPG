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

// �޸� ��ų1
class HumanSkill1 : public SkillInfo
{
public:
	HumanSkill1();
	~HumanSkill1();

	// ��ų �߰�������
	int iAddSkillDmg;

	// ��ų ���� �Ÿ�
	float fRange;
};