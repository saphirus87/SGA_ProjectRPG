/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

설명 :
캐릭터 또는 아이템에서 사용되는 능력치 정보 입니다.
*/
#pragma once
#include "stdafx.h"

class StatusInfo
{
public:
	StatusInfo();
	virtual ~StatusInfo();

	// 캐릭터는 기본적으로 체력(Hit Point)이 있습니다. 이 체력이 전부 소진되면 캐릭터는 사망하게 됩니다.
	int HP;			
	// 캐릭터는 기본적으로 마나(Mana Point)가 있습니다. 이 마나가 전부 소진되면 캐릭터는 스킬 또는 마법을 사용할 수 없게 됩니다.
	int MP;			
	// 캐릭터는 기본적으로 물리 공격력이 있습니다.
	int ATK_PHY;	
	// 캐릭터는 기본적으로 마법 공격력이 있습니다.
	int ATK_MGR;	
	// 캐릭터는 기본적으로 물리 방어력이 있습니다.
	int DEF_PHY;
	// 캐릭터는 기본적으로 마법 방어력이 있습니다.
	int DEF_MGR;
	// 이동 속도
	float MOVE_SPEED;
	// 공격 속도
	float ATK_SPEED;
	// 치명타(Critical) 확률
	float CRI_PER;
};

