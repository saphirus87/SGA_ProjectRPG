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
	void SetChrNamePos(Vector3& vChrNamePos);
	Vector3& GetChrNamePos();

private:
	Device9 pDevice9;
	Mesh m_pMesh3DText;
	Material m_mtrl;
	// ĳ���� �̸���, ��ġ���� �����ϴ� ����
	CString m_szChrName;
	Vector3 m_vChrNamePos;
	// �׻� ī�޶� �ٶ󺸰� �ϴ� Y�� �����
	Matrix4x4 m_matBillboard;
};

