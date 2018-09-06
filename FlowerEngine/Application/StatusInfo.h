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
	int HPMAX;
	// ĳ���ʹ� �⺻������ ����(Mana Point)�� �ֽ��ϴ�. �� ������ ���� �����Ǹ� ĳ���ʹ� ��ų �Ǵ� ������ ����� �� ���� �˴ϴ�.
	int MP;			
	int MPMAX;
	// ĳ���ʹ� �⺻������ ���� ���ݷ��� �ֽ��ϴ�.
	int ATK_PHY;	
	// ĳ���ʹ� �⺻������ ���� ���ݷ��� �ֽ��ϴ�.
	int ATK_MGR;	
	// ĳ���ʹ� �⺻������ ���� ������ �ֽ��ϴ�.
	int DEF_PHY;
	// ĳ���ʹ� �⺻������ ���� ������ �ֽ��ϴ�.
	int DEF_MGR;
	
	// HP ȸ�� �ð�
	float REVTime_HP;
	// HP ȸ����
	int REV_HP;
	// MP ȸ�� �ð�
	float REVTime_MP;
	// MP ȸ����
	int REV_MP;

	// �̵� �ӵ�
	float MOVE_SPEED;
	// ���� �ӵ�
	float ATK_SPEED;
	// ġ��Ÿ(Critical) Ȯ��
	float CRI_PER;

	// LEVEL ����
	int LEVEL;
	// ���� ����ġ
	int EXP;
	// �ʿ� ����ġ
	int NextEXP() { return vecEXPNext[LEVEL - 1]; }
	vector<int> vecEXPNext;
	// ������ ���θ� Ȯ���մϴ�
	bool CheckLevelUp();
};

