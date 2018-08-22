#pragma once
#include "stdafx.h"

class ComTexture : public Component
{
public:
	ComTexture(CString szName);
	~ComTexture();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:

};

