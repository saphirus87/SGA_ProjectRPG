/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

설명 :
클래스 이름 그데로 아이템 정보입니다. 보통 EXCEL이나 ITEM TOOL에서 데이터를 읽어서 사용합니다.
*/
#pragma once
#include "stdafx.h"

enum eItemType
{
	eItem_None,		// 타입 없음
	eItem_Shoulder,	// 어깨 방어구
	eItem_COUNT
};

// 아이템 정보
class ItemInfo
{
public:
	ItemInfo();
	virtual ~ItemInfo();

	// 아이템 아이디 
	unsigned int ID;
	eItemType ItemType;
};

// 장비 아이템
class EquipmentShoulder : public ItemInfo
{
public:
	EquipmentShoulder();
	~EquipmentShoulder();

	// 캐릭터의 능력치에 체력(Hit Point) 증가
	int HP;
	// 캐릭터의 능력치에 마나(Mana Point) 증가
	int MP;
	// 캐릭터의 능력치에 물리 방어력 증가
	int DEF_PHY;
	// 캐릭터의 능력치에 마법 방어력 증가
	int DEF_MGR;
};