#include "stdafx.h"
#include "StatusInfo.h"


StatusInfo::StatusInfo() :
HP(50),
HPMAX(50),
MP(10),
MPMAX(10),
ATK_PHY(3),
ATK_MGR(3),
DEF_PHY(1),
DEF_MGR(1),
MOVE_SPEED(0.02f),
ATK_SPEED(0),
CRI_PER(0),
REVTime_HP(3),
REV_HP(1),
REVTime_MP(3),
REV_MP(1),
LEVEL(1),
EXP(0)
{
	vecEXPNext.resize(99);

	for (int i = 0; i < 99; ++i)
		vecEXPNext[i] = i + 1;
}


StatusInfo::~StatusInfo()
{
}

bool StatusInfo::GetEXPAndCheckLevelUp()
{
	// 레벨 제한 99
	if (LEVEL <= 99)
	{
		// 경험치 증가
		++EXP;

		for (int i = LEVEL - 1; i < LEVEL; ++i)
		{
			// 레벨업
			if (EXP >= vecEXPNext[LEVEL - 1])
			{
				++LEVEL;
				return true;
			}
		}
	}

	return false;
}
