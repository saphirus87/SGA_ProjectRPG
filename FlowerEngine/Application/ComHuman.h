/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

설명 :
캐릭터중 인간입니다.
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

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetAniEvent();

	// UIButtonDelegate을(를) 통해 상속됨
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;
};