#pragma once
#include "stdafx.h"

class ComTexture : public Component
{
public:
	ComTexture(CString szName);
	~ComTexture();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:

};

