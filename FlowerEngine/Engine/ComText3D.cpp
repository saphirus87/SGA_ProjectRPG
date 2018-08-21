#include "stdafx.h"
#include "ComText3D.h"

ComText3D::ComText3D(CString szName)
	:Component(szName), 
	m_pMesh3DText(NULL)
{
	pDevice9 = GetD3D9Device();
	D3DXMatrixIdentity(&m_matBillboard);
}

ComText3D::~ComText3D()
{
	SAFE_RELEASE(m_pMesh3DText);
}

void ComText3D::Awake()
{
	CreateD3DXTextMesh(&m_pMesh3DText, L"ssssss", 10, FALSE, FALSE);

	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));
	m_mtrl.Diffuse.r = m_mtrl.Ambient.r = 0.0f;
	m_mtrl.Diffuse.g = m_mtrl.Ambient.g = 0.0f;
	m_mtrl.Diffuse.b = m_mtrl.Ambient.b = 1.0f;
	m_mtrl.Diffuse.a = m_mtrl.Ambient.a = 1.0f;

	GameObject* pGOChr = GameObject::Find("undead_01");
	Vector3 vPos = pGOChr->transform->GetPosition();
	vPos.y += 1.0f;

	gameObject->transform->SetPosition(vPos);
}

void ComText3D::Update()
{
	Matrix4x4 matView = Camera::GetInstance()->GetViewMatrix();

	// 역행렬을 구해준 후
	D3DXMatrixInverse(&m_matBillboard, NULL, &matView);

	// 위치만 강제 셋팅
	Vector3 vPos = gameObject->transform->GetPosition();
	m_matBillboard._41 = vPos.x;
	m_matBillboard._42 = vPos.y;
	m_matBillboard._43 = vPos.z;
}

void ComText3D::Render()
{
	if (m_pMesh3DText != NULL)
	{
		pDevice9->SetMaterial(&m_mtrl);
		pDevice9->SetTransform(D3DTS_WORLD, &m_matBillboard);
		
		m_pMesh3DText->DrawSubset(0);
	}
}

HRESULT ComText3D::CreateD3DXTextMesh(LPD3DXMESH * ppMesh, TCHAR * pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic)
{
	HRESULT hr;
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC(NULL);
	if (hdc == NULL)
		return E_OUTOFMEMORY;
	INT nHeight = -MulDiv(dwSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hFont;
	HFONT hFontOld;

	hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pstrFont);

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	hr = D3DXCreateText(pDevice9, hdc, L"This is calling D3DXCreateText",
		0.001f, 0.4f, &pMeshNew, NULL, NULL);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	if (SUCCEEDED(hr))
		*ppMesh = pMeshNew;

	return hr;
}
