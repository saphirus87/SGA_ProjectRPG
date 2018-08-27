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
CRI_PER(0)
{
}


StatusInfo::~StatusInfo()
{
}
