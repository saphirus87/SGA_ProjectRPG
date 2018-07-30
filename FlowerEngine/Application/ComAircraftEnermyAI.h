#pragma once
#include "stdafx.h"

class ComShooting;
class ComFollowTarget;

class ComAircraftEnermyAI : public Component
{
public:
	ComAircraftEnermyAI(CString szName);
	~ComAircraftEnermyAI();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
	
private:
	ComShooting * m_pShooting;
	ComFollowTarget * m_pFollowTarget;
	CTimer * m_pTimer;
};

