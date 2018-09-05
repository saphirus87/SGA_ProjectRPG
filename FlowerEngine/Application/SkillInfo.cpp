#include "stdafx.h"
#include "SkillInfo.h"


SkillInfo::SkillInfo() : 
	UID(0),
	iUseMP(3),
	fCoolTime(3)
{
}


SkillInfo::~SkillInfo()
{
}

HumanSkill1::HumanSkill1() : 
	iAddSkillDmg(1),
	fRange(2.0f)
{
}

HumanSkill1::~HumanSkill1()
{
}
