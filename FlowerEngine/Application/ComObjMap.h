#pragma once
#include "stdafx.h"

class ComObjMap : public Component
{
private:
	Device9 pDevice9;

	float m_rayDistance;

	CString m_filePath;
	CString m_mapFilename;
	CString m_surfaceFilename;

	LPD3DXMESH m_pMesh;
	vector<MTLTEX*> m_mtltexList;

	vector<D3DXVECTOR3> m_vertices;
	vector<D3DXVECTOR3> m_surfaceVertices;

	size_t m_dimension;
	D3DXVECTOR3 m_size;

	LPD3DXEFFECT m_pEffect;

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

	void SetFilename(CString szFolderPath, CString szMapFilename, CString szSurfaceFileName)
	{
		m_filePath = szFolderPath; m_mapFilename = szMapFilename; m_surfaceFilename = szSurfaceFileName;
	}
	void SetDimension(size_t dimension) { m_dimension = dimension; }
};

