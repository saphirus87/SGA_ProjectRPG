#include "stdafx.h"
#include "ComText3D.h"

ComText3D::ComText3D(CString szName)
	:Component(szName), 
	m_pMesh3DText(NULL),
	fOffsetPosY(1.3f),
	m_vScale(0.2f, 0.2f, 0.2f),
	fAlpha(1.0f),
	m_bAlphaShade(false)
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

}

void ComText3D::Update()
{
	Matrix4x4 matView = Camera::GetInstance()->GetViewMatrix();
	Matrix4x4 matS;

	// 역행렬을 구해준 후
	D3DXMatrixInverse(&m_matBillboard, NULL, &matView);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);

	m_matBillboard *= matS;

	// 위치만 강제 셋팅
	Vector3 vPos = gameObject->transform->GetPosition();
	//Vector3 vPos = GetChrNamePos();
	m_matBillboard._41 = vPos.x;
	m_matBillboard._42 = vPos.y + fOffsetPosY;;
	m_matBillboard._43 = vPos.z;
}

void ComText3D::Render()
{
	float fDeltaTime = GetElapsedTime();

	if (m_pMesh3DText != NULL)
	{
		//pDevice9->SetRenderState(D3DRS_LIGHTING, false);
		
		if (m_bAlphaShade)
		{
			fAlpha -= 0.1f * fDeltaTime;
			m_mtrl.Diffuse.a = m_mtrl.Ambient.a = m_mtrl.Specular.a = m_mtrl.Emissive.a = 0.1f;
		}

		pDevice9->SetMaterial(&m_mtrl);
		pDevice9->SetTransform(D3DTS_WORLD, &m_matBillboard);
		m_pMesh3DText->DrawSubset(0);
		//pDevice9->SetRenderState(D3DRS_LIGHTING, true);
	}
}

HRESULT ComText3D::CreateD3DXTextMesh(CString szText, LPD3DXMESH * ppMesh, TCHAR * pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic)
{
	HRESULT hr;
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC(NULL);
	if (hdc == NULL)
		return E_OUTOFMEMORY;

	// MulDiv(int nNumber, int nNumerator, int nDenominator)
	/*
		nNumber * nNumerator / nDenominator 을 계산해 성공할 경우 계산결과를 
		반환, 오버플로우가 발생하거나 나누는 값이 0인 경우는 -1을 반환한다.
	*/
	INT nHeight = -MulDiv(dwSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HFONT hFont;
	HFONT hFontOld;

	// CreateFont()
	/*
		(int 문자 폭, int 문자 높이, int 문자 기울기, int 문자 방향, int 문자 굵기, 
		DWORD 기울기, DWORD 밑줄, DWORD 취소선, DWORD 문자셋, DWORD 출력 정확도, 
		DWORD 클리핑 정확도, DWORD 출력의 질, 자간, LPCWSTR 폰트 이름)
	*/
	hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pstrFont);

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	// D3DXCreateText()
	/*
		(디바이스 포인터, DC핸들, 생성할 문자열, 트루타입 폰트 외곽선부터의 최대 편차, 텍스트의 깊이값, 
		텍스트를 저장할 메쉬 포인터, 인접정보를 저장할 버퍼의 포인터, 폰트의 정보를 저장할 구조체 포인터(NULL))
	*/
	hr = D3DXCreateText(pDevice9, hdc, szText, 0.001f, 0.001f, &pMeshNew, NULL, NULL);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);

	if (SUCCEEDED(hr))
		*ppMesh = pMeshNew;

	return hr;
}

void ComText3D::SetText(CString szChrName)
{
	m_szText = szChrName;

	if (m_pMesh3DText)
		m_pMesh3DText->Release();

	CreateD3DXTextMesh(m_szText, &m_pMesh3DText, L"맑은 고딕", 5, FALSE, FALSE);

	// 기본 색상값
	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));
	m_mtrl.Diffuse.r = m_mtrl.Emissive.r = 1.0f;
	m_mtrl.Diffuse.g = m_mtrl.Emissive.g = 1.0f;
	m_mtrl.Diffuse.b = m_mtrl.Emissive.b = 0.0f;
	m_mtrl.Diffuse.a = m_mtrl.Emissive.a = 1.0f;
}

void ComText3D::SetText(CString szChrName, Color color, float fScale, bool bAlphaShade)
{
	m_szText = szChrName;

	if (m_pMesh3DText)
		m_pMesh3DText->Release();

	CreateD3DXTextMesh(m_szText, &m_pMesh3DText, L"맑은 고딕", 5, TRUE, FALSE);
	
	m_bAlphaShade = bAlphaShade;
	fAlpha = color.a;

	m_vScale.x = m_vScale.y = m_vScale.z = fScale;

	ZeroMemory(&m_mtrl, sizeof(D3DMATERIAL9));
	m_mtrl.Diffuse.r = m_mtrl.Emissive.r = color.r;
	m_mtrl.Diffuse.g = m_mtrl.Emissive.g = color.g;
	m_mtrl.Diffuse.b = m_mtrl.Emissive.b = color.b;
	m_mtrl.Diffuse.a = m_mtrl.Emissive.a = color.a;
}

CString ComText3D::GetText()
{
	return m_szText;
}

void ComText3D::SetChrNamePos(Vector3 & vChrNamePos)
{
	m_vChrNamePos = vChrNamePos;
}

Vector3 & ComText3D::GetChrNamePos()
{
	return m_vChrNamePos;
}
