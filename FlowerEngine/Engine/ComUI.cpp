#include "stdafx.h"
#include "ComUI.h"


ComUI::ComUI(CString szName) : 
	Component(szName),
	color(WHITE),
	size(Vector2(100, 100)),
	IsDrawBorder(true),
	m_pivot(Vector3(0, 0, 0))

{
	pDevice9 = GetD3D9Device();
	D3DXCreateSprite(pDevice9, &m_pSprite);
}

ComUI::~ComUI()
{
	SAFE_RELEASE(m_pSprite);
}

void ComUI::DrawBorder()
{
	if (IsDrawBorder)
	{
		Matrix4x4 matW = gameObject->transform->GetWorldMatrix();
		Matrix4x4 matL = gameObject->transform->GetLocalMatrix();

		// 41 = transform matrix value
		// 11 = scale matrix value
		int left = matW._41;
		int top = matW._42;
		int right = left + size.x * matL._11;
		int bottom = top + size.y * matL._22;

		SetRect(&m_screenRect, left, top, right, bottom);
		
		vector<VERTEX_RHWC> vecVertex;
		D3DCOLOR c = WHITE;
		vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(m_screenRect.left, m_screenRect.top, 0, 1), c));
		vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(m_screenRect.right, m_screenRect.top, 0, 1), c));
		vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(m_screenRect.right, m_screenRect.bottom, 0, 1), c));
		vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(m_screenRect.left, m_screenRect.bottom, 0, 1), c));
		vecVertex.push_back(VERTEX_RHWC(D3DXVECTOR4(m_screenRect.left, m_screenRect.top, 0, 1), c));

		pDevice9->SetRenderState(D3DRS_LIGHTING, false);
		pDevice9->SetFVF(VERTEX_RHWC::FVF);
		pDevice9->DrawPrimitiveUP(D3DPT_LINESTRIP, vecVertex.size() - 1, &vecVertex[0], sizeof(VERTEX_RHWC));
		pDevice9->SetRenderState(D3DRS_LIGHTING, true);
	}
}
