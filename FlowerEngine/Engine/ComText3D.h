#pragma once
#include "stdafx.h"

class ComText3D : public Component
{
public:
	ComText3D(CString szName);
	~ComText3D();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	HRESULT CreateD3DXTextMesh(LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic);

	void SetChrName(CString szChrName);
	CString GetChrName();

private:
	Device9 pDevice9;
	Mesh m_pMesh3DText;
	Material m_mtrl;

	CString m_szChrName;
	// �׻� ī�޶� �ٶ󺸰� �ϴ� Y�� �����
	Matrix4x4 m_matBillboard;
};

