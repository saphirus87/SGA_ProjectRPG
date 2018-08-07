/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

���� :
ĳ���� �Ǵ� �����ۿ��� ���Ǵ� �ɷ�ġ ���� �Դϴ�.
*/
#pragma once
#include "stdafx.h"

class StatusInfo
{
public:
	StatusInfo();
	virtual ~StatusInfo();

	// ĳ���ʹ� �⺻������ ü��(Hit Point)�� �ֽ��ϴ�. �� ü���� ���� �����Ǹ� ĳ���ʹ� ����ϰ� �˴ϴ�.
	int HP;			
	// ĳ���ʹ� �⺻������ ����(Mana Point)�� �ֽ��ϴ�. �� ������ ���� �����Ǹ� ĳ���ʹ� ��ų �Ǵ� ������ ����� �� ���� �˴ϴ�.
	int MP;			
	// ĳ���ʹ� �⺻������ ���� ���ݷ��� �ֽ��ϴ�.
	int ATK_PHY;	
	// ĳ���ʹ� �⺻������ ���� ���ݷ��� �ֽ��ϴ�.
	int ATK_MGR;	
	// ĳ���ʹ� �⺻������ ���� ������ �ֽ��ϴ�.
	int DEF_PHY;
	// ĳ���ʹ� �⺻������ ���� ������ �ֽ��ϴ�.
	int DEF_MGR;
	// �̵� �ӵ�
	float MOVE_SPEED;
	// ���� �ӵ�
	float ATK_SPEED;
	// ġ��Ÿ(Critical) Ȯ��
	float CRI_PER;
};

