#pragma once
#include "stdafx.h"

class ComUIItemInfo : public Component, public UIButtonDelegate
{
public:
	ComUIItemInfo(CString szName);
	~ComUIItemInfo();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// UIButtonDelegate��(��) ���� ��ӵ�
	virtual void OnClick(UIButton * pSender) override;
	virtual void OnPress(UIButton * pSender) override;

	void UpdateDialog(Vector2 BorderSize);

	void SetItemInfo(ItemInfo* pItemInfo);
	void SetPosition(Vector3 pos) { gameObject->transform->SetPosition(pos); }
	void SetIsVisible(bool IsVisible) { m_pDialog->SetIsVisible(IsVisible); }

private:
	ComDialog* m_pDialog;
	ItemInfo* m_pItemInfo;
};

