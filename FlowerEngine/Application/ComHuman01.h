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

class ComHuman01 : public ComCharacter
{
public:
	ComHuman01(CString szName);
	~ComHuman01();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// UI 관련
	void UpdateHPBar() override;



	void SetAniEvent();

private:
	UIProgressBar * m_pHPBar;
};