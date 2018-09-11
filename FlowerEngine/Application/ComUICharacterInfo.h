#pragma once
#include "stdafx.h"

class ComChrEquipment;

class ComUICharacterInfo : public Component, public UIButtonDelegate
{
public:
	ComUICharacterInfo(CString szName);
	~ComUICharacterInfo();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// UIButtonDelegate��(��) ���� ��ӵ�
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;

	//
	void UpdateIcons();
	void ClearEquipIcons();
	void SetChrEquip(ComChrEquipment* pChrEquip) { m_pChrEquip = pChrEquip; }

private:
	ComChrEquipment* m_pChrEquip;
	ComDialog* m_pDialog;
};

