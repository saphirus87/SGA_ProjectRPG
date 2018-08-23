/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

���� :
ĳ������ �ΰ��Դϴ�.
*/
#pragma once
#include "stdafx.h"
#include "ComCharacter.h"

class ComChrEquipment;

class ComHuman01 : public ComCharacter
{
public:
	ComHuman01(CString szName);
	~ComHuman01();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// �ٸ� �ݶ��̴��� �浹���� �� ȣ��Ǵ� �Լ�
	virtual void OnTriggerEnter(ComCollider &collider) override;

	void SetAniEvent();
};