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
	eItem_None = -1,
	eItem_Shoulder,
	eItem_Helmet,
	eItem_Shield,
	eItem_WeaponR,
	eItem_WeaponL,
	// 예제
	eItem_Potion,	// 포션류
	eItem_Quest,	// 퀘스트류 잡템
	// 추가
	eItem_COUNT
};

// 아이템 정보
class ItemInfo
{
public:
	ItemInfo();
	virtual ~ItemInfo();

	// 아이템 아이디 
	unsigned int UID;
	// 아이템 타입
	eItemType Type;
	// 장착 캐릭터 타입
	eChrType ChrType;
	// 아이템 이름
	CString Name;
	// 폴더 경로
	CString FolderPath;
	// 아이템 리소스 이름
	CString XFileName;
	// 변경된 텍스쳐 이름, 텍스쳐 이름이 없으면 원본 사용
	CString TextureName;
	// 아이콘 이름
	CString IconName;

	int Reference;
};

class ItemPotion : public ItemInfo
{
public:
	ItemPotion();
	~ItemPotion();

	// 수량
	UINT Count;			// 서버 DB에서 날라오는 값으로 설정
	// 최대 수량
	UINT CountMax;		// 파일에서 읽어서 설정
};

//class ItemQuest : public ItemInfo
//{
//public:
//	ItemQuest();
//	~ItemQuest();
//
//	// 수량
//	UINT Count;
//	// 최대 수량
//	UINT CountMax;
//};

// 어깨 방어구 장비 아이템
class EquipmentShoulder : public ItemInfo
{
public:
	EquipmentShoulder(CString szName, CString szXFileName, CString szIconName);
	~EquipmentShoulder();

	void Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType);

	// TODO : 능력치 정보 기획에 맞게 수정 가능
	// 캐릭터의 능력치에 체력(Hit Point) 증가
	int HP;
	// 캐릭터의 능력치에 마나(Mana Point) 증가
	int MP;
	// 캐릭터의 능력치에 물리 방어력 증가
	int DEF_PHY;
	// 캐릭터의 능력치에 마법 방어력 증가
	int DEF_MGR;
	// 능력치 추가
};

// 핼멧 방어구 장비 아이템
class EquipmentHelmet : public ItemInfo
{
public:
	EquipmentHelmet(CString szName, CString szXFileName, CString szIconName);
	~EquipmentHelmet();

	void Set(int iHP, int iMP, int iDEF_PHY, int iDEF_MGR, eChrType chrType);

	// TODO : 능력치 정보 기획에 맞게 수정 가능
	// 캐릭터의 능력치에 체력(Hit Point) 증가
	int HP;
	// 캐릭터의 능력치에 마나(Mana Point) 증가
	int MP;
	// 캐릭터의 능력치에 물리 방어력 증가
	int DEF_PHY;
	// 캐릭터의 능력치에 마법 방어력 증가
	int DEF_MGR;
	// 능력치 추가
};

// 방패 방어구 장비 아이템
class EquipmentShield : public ItemInfo
{
public:
	EquipmentShield(CString szName, CString szXFileName, CString szIconName);
	~EquipmentShield();

	void Set(int iBLOCK_PER, eChrType chrType);

	// TODO : 능력치 정보 기획에 맞게 수정 가능
	// 캐릭터의 능력치에 방패 막기 확률 증가
	float BLOCK_PER;
};

// 검 무기 아이템
class EquipmentWeapon : public ItemInfo
{
public:
	EquipmentWeapon(CString szName, CString szXFileName, CString szIconName);
	~EquipmentWeapon();

	void Set(int iATK_MIN, int iATK_MAX, eChrType chrType);

	// TODO : 능력치 정보 기획에 맞게 수정 가능
	// 캐릭터의 능력치에 최소 공격력 증가
	int ATK_MIN;
	// 캐릭터의 능력치에 최대 공격력 증가
	int ATK_MAX;
};

// 장비 클래스 추가
//class EquipmentStaff : public ItemInfo