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

class ComHuman : public ComCharacter, public UIButtonDelegate
{
public:
	ComHuman(CString szName);
	~ComHuman();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetAniEvent();

	// UIButtonDelegate��(��) ���� ��ӵ�
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;
};