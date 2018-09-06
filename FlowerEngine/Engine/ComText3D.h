#pragma once
#include "stdafx.h"

class ComText3D : public Component
{
public:
	ComText3D(CString szName);
	~ComText3D();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	HRESULT CreateD3DXTextMesh(CString szText, LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic);

	void SetText(CString szChrName);
	void SetText(CString szChrName, Color color, float fScale, bool bAlphaShade = false);

	CString GetText();
	void SetChrNamePos(Vector3& vChrNamePos);
	Vector3& GetChrNamePos();

	// 위쪽으로 보정값
	float fOffsetPosY;
	// 텍스트 알파값
	float fAlpha;

private:
	// 크기 배율
	Vector3 m_vScale;

	// 알파값이 서서히 사라지는 여부
	bool m_bAlphaShade;
	
	Device9 pDevice9;
	Mesh m_pMesh3DText;
	Material m_mtrl;
	// 캐릭터 이름값, 위치값을 저장하는 변수
	CString m_szText;
	Vector3 m_vChrNamePos;
	// 항상 카메라를 바라보게 하는 Y축 역행렬
	Matrix4x4 m_matBillboard;
};

