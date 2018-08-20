#pragma once
#include "stdafx.h"
#include "ComChrControl.h"

class ComObjMap;
class ComFollowTarget;
class IChrState;

// Smallderon�� �ΰ�����(AI)
class ComSmallderon : public ComChrControl
{
public:
	ComSmallderon(CString szName);
	~ComSmallderon();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	//���� ���
	void Stand() override;
	void Walk(float fDeltaZ) override;
	void Attack1() override;
};

