#pragma once
#include "stdafx.h"

class ComGrid : public Component
{
public:
	ComGrid(CString szName);
	~ComGrid();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	Device9 pDevice9;
	vector<VERTEX_PC> m_vertices;
};

