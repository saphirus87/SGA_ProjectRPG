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

	HRESULT CreateD3DXTextMesh(CString szText, LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic);

	void SetText(CString szChrName);
	void SetText(CString szChrName, Color color, float fScale, bool bAlphaShade = false);

	CString GetText();
	void SetChrNamePos(Vector3& vChrNamePos);
	Vector3& GetChrNamePos();

	// �������� ������
	float fOffsetPosY;
	// �ؽ�Ʈ ���İ�
	float fAlpha;

private:
	// ũ�� ����
	Vector3 m_vScale;

	// ���İ��� ������ ������� ����
	bool m_bAlphaShade;
	
	Device9 pDevice9;
	Mesh m_pMesh3DText;
	Material m_mtrl;
	// ĳ���� �̸���, ��ġ���� �����ϴ� ����
	CString m_szText;
	Vector3 m_vChrNamePos;
	// �׻� ī�޶� �ٶ󺸰� �ϴ� Y�� �����
	Matrix4x4 m_matBillboard;
};

