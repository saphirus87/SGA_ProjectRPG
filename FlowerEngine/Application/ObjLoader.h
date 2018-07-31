#pragma once
#include "stdafx.h"

class ObjLoader
{
private:
	Device9 pDevice9;

public:
	CString m_filePath;
	map<CString, MTLTEX*> m_mtltexList;

	ObjLoader();
	~ObjLoader();

	LPD3DXMESH LoadMesh(LPCTSTR filepath, LPCTSTR filename, D3DXMATRIXA16* pMat, vector<MTLTEX*> &vecMtltex, float InverseUV = 0.0f);
	void LoadSurface(LPCTSTR fullpath, D3DXMATRIXA16* pMat, OUT vector<D3DXVECTOR3>& vertices);
	void LoadMtlLib(LPCTSTR fullpath);
};