#pragma once
class SkillInfo
{
public:
	SkillInfo();
	virtual ~SkillInfo();

	// ��ų ID
	unsigned int UID;

	// ��ų �̸�
	CString szName;
	
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