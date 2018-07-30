#include "stdafx.h"
#include "ComGrid.h"


ComGrid::ComGrid(CString szName) : 
	Component(szName)
{
	pDevice9 = GetD3D9Device();

	float interval = 1.0f;
	int numHalfLine = 20;
	float halfLength = interval * numHalfLine;
	D3DCOLOR c;
	float tmpInterval;
	for (int i = 1; i <= numHalfLine; ++i)
	{
		tmpInterval = interval * i;
		c = (i % 5 == 0) ? D3DCOLOR_XRGB(255, 255, 255) : D3DCOLOR_XRGB(128, 128, 128);
		m_vertices.push_back(VERTEX_PC(Vector3(-halfLength, 0.0f, tmpInterval), c));
		m_vertices.push_back(VERTEX_PC(Vector3(halfLength, 0.0f, tmpInterval), c));
		m_vertices.push_back(VERTEX_PC(Vector3(-halfLength, 0.0f, -tmpInterval), c));
		m_vertices.push_back(VERTEX_PC(Vector3(halfLength, 0.0f, -tmpInterval), c));
		m_vertices.push_back(VERTEX_PC(Vector3(tmpInterval, 0.0f, -halfLength), c));
		m_vertices.push_back(VERTEX_PC(Vector3(tmpInterval, 0.0f, halfLength), c));
		m_vertices.push_back(VERTEX_PC(Vector3(-tmpInterval, 0.0f, -halfLength), c));
		m_vertices.push_back(VERTEX_PC(Vector3(-tmpInterval, 0.0f, halfLength), c));
	}
	c = D3DCOLOR_XRGB(255, 0, 0);
	m_vertices.push_back(VERTEX_PC(Vector3(-halfLength, 0.0f, 0.0f), c));
	m_vertices.push_back(VERTEX_PC(Vector3(halfLength, 0.0f, 0.0f), c));

	c = D3DCOLOR_XRGB(0, 255, 0);
	m_vertices.push_back(VERTEX_PC(Vector3(0.0f, -halfLength, 0.0f), c));
	m_vertices.push_back(VERTEX_PC(Vector3(0.0f, halfLength, 0.0f), c));

	c = D3DCOLOR_XRGB(0, 0, 255);
	m_vertices.push_back(VERTEX_PC(Vector3(0.0f, 0.0f, -halfLength), c));
	m_vertices.push_back(VERTEX_PC(Vector3(0.0f, 0.0f, halfLength), c));
}

ComGrid::~ComGrid()
{
}

void ComGrid::Awake()
{
}

void ComGrid::Update()
{
}

void ComGrid::Render()
{
	// Grid´Â ¿¹¿Ü·Î LIGHTINGÀ» ²¨ÁÜ
	pDevice9->SetRenderState(D3DRS_LIGHTING, false);
	pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
	pDevice9->SetFVF(VERTEX_PC::FVF);
	pDevice9->DrawPrimitiveUP(D3DPT_LINELIST, m_vertices.size() / 2, &m_vertices[0], sizeof(VERTEX_PC));
	pDevice9->SetRenderState(D3DRS_LIGHTING, true);
}