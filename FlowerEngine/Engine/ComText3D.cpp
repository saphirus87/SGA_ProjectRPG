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
	CreateD3DXTextMesh(&m_pMesh3DText, L"맑은 고딕", 5, FALSE, FALSE);

	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));
	m_mtrl.Diffuse.r = m_mtrl.Emissive.r = 1.0f;
	m_mtrl.Diffuse.g = m_mtrl.Emissive.g = 1.0f;
	m_mtrl.Diffuse.b = m_mtrl.Emissive.b = 0.0f;
	m_mtrl.Diffuse.a = m_mtrl.Emissive.a = 1.0f;
	
}

void ComText3D::Update()
{
	Matrix4x4 matView = Camera::GetInstance()->GetViewMatrix();
	Matrix4x4 matS;

	// 역행렬을 구해준 후
	D3DXMatrixInverse(&m_matBillboard, NULL, &matView);
	D3DXMatrixScaling(&matS, 0.20f, 0.20f, 0.20f);

	m_matBillboard *= matS;

	// 위치만 강제 셋팅
	Vector3 vPos = gameObject->transform->GetPosition();
	//Vector3 vPos = GetChrNamePos();
	m_matBillboard._41 = vPos.x;
	m_matBillboard._42 = vPos.y + 1.3f;;
	m_matBillboard._43 = vPos.z;
}

void ComText3D::Render()
{
	if (m_pMesh3DText != NULL)
	{
		pDevice9->SetRenderState(D3DRS_LIGHTING, true);
		pDevice9->SetMaterial(&m_mtrl);
		pDevice9->SetTransform(D3DTS_WORLD, &m_matBillboard);
		m_pMesh3DText->DrawSubset(0);
		pDevice9->SetRenderState(D3DRS_LIGHTING, false);
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

	hr = D3DXCreateText(pDevice9, hdc, GetChrName(), 0.001f, 0.001f, &pMeshNew, NULL, NULL);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	if (SUCCEEDED(hr))
		*ppMesh = pMeshNew;

	return hr;
}

void ComText3D::SetChrName(CString szChrName)
{
	m_szChrName = szChrName;
}

CString ComText3D::GetChrName()
{
	return m_szChrName;
}

void ComText3D::SetChrNamePos(Vector3 & vChrNamePos)
{
	m_vChrNamePos = vChrNamePos;
}

Vector3 & ComText3D::GetChrNamePos()
{
	return m_vChrNamePos;
}
