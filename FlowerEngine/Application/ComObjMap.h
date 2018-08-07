#pragma once
#include "stdafx.h"

// 임시
const float radius = 2.5f;

class ComObjMap : public Component
{
private:
	vector<D3DXVECTOR3> m_surfaceVertices;

	Device9 pDevice9;
	float m_rayDistance;

	CString m_filePath;
	CString m_mapFilename;

	vector<VERTEX_PNT> m_vertices;
	vector<DWORD> m_surfaceIndices;
	map<CString, MTLTEX*> m_mtltexList;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	LPD3DXEFFECT m_pEffect;
	float m_InverseUV;

public:
	ComObjMap(CString szName);
	~ComObjMap();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	bool GetHeight(float & height, const D3DXVECTOR3 & pos);
	bool CalcPickedPosition(D3DXVECTOR3 &out, WORD screenX, WORD screenY);
	void SetRayDistance(float rayDistance) { m_rayDistance = rayDistance; }
	void SetFilename(CString szFolderPath, CString szMapFilename)
	{
		m_filePath = szFolderPath; m_mapFilename = szMapFilename;
	}

	// Map Load
	void LoadMap();
	void LoadMtlLib(LPCTSTR fullPath);

	// frustum culling
	void UpdateIndexBuffer();
};

