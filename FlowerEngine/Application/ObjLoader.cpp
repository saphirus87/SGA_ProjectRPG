#include "stdafx.h"
#include "ObjLoader.h"

const int TOKEN_SIZE = 128;

ObjLoader::ObjLoader()
{
	pDevice9 = GetD3D9Device();
}

ObjLoader::~ObjLoader()
{
}

// 와우 맵 로드시 UV값중 V값 반전 필요, 함수에 bool 변수 추가 해서 true 일경우 반전 시키는 내용 추가
LPD3DXMESH ObjLoader::LoadMesh(LPCTSTR filepath, LPCTSTR filename, D3DXMATRIXA16 * pMat, vector<MTLTEX*>& vecMtltex, float InverseUV)
{
	vector<D3DXVECTOR3> vecP;
	vector<D3DXVECTOR2> vecT;
	vector<D3DXVECTOR3> vecN;
	vector<VERTEX_PNT> vecPNT;
	vector<DWORD> vecAttbuf;
	CString mtlName;
	char token[TOKEN_SIZE];

	m_filePath = filepath;

	std::ifstream fin;

	fin.open(m_filePath + filename);

	if (fin.is_open() == false)
		assert(false && "fin.is_open() == false");

	while (fin.eof() == false)
	{
		fin >> token;

		if (CompareStr(token, "mtllib"))
		{
			fin >> token;
			LoadMtlLib(m_filePath + token);

			vecMtltex.resize(m_mtltexList.size());

			for (auto p : m_mtltexList)
			{
				vecMtltex[p.second->id] = p.second;
			}
		}
		else if (CompareStr(token, "v"))
		{
			float x, y, z;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %f", &x, &y, &z);
			vecP.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (CompareStr(token, "vt"))
		{
			float x, y;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %*f", &x, &y);
			if (InverseUV > 0.1f) vecT.push_back(D3DXVECTOR2(x, InverseUV - y));
			else vecT.push_back(D3DXVECTOR2(x, y));
		}
		else if (CompareStr(token, "vn"))
		{
			float x, y, z;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %f", &x, &y, &z);
			vecN.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (CompareStr(token, "usemtl"))
		{
			fin >> token;
			mtlName = token;
		}
		else if (CompareStr(token, "f"))
		{
			int aIndex[3][3];

			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

			for (int i = 0; i < 3; i++)
			{
				VERTEX_PNT pnt;
				pnt.p = vecP[aIndex[i][0] - 1];
				pnt.t = vecT[aIndex[i][1] - 1];
				pnt.n = vecN[aIndex[i][2] - 1];
				//vecPNT.push_back(VERTEX_PNT(vecP[aIndex[i][0] - 1], vecN[aIndex[i][2] - 1], vecT[aIndex[i][1] - 1]));

				if (pMat)
				{
					D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
					D3DXVec3TransformCoord(&pnt.n, &pnt.n, pMat);
				}
				D3DXVec3Normalize(&pnt.n, &pnt.n);
				vecPNT.push_back(pnt);
			}
			vecAttbuf.push_back(m_mtltexList[mtlName]->id);
		}
	}

	m_mtltexList.clear();
	fin.close();

	LPD3DXMESH pMesh = NULL;
	D3DXCreateMeshFVF(vecPNT.size() / 3, vecPNT.size(), D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_PNT::FVF, pDevice9, &pMesh);

	VERTEX_PNT* pV = NULL;
	DWORD flags = 0;
	pMesh->LockVertexBuffer(flags, (LPVOID*)&pV);
	memcpy(pV, &vecPNT[0], vecPNT.size() * sizeof(VERTEX_PNT));
	pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	pMesh->LockIndexBuffer(flags, (LPVOID*)&pI);
	for (size_t i = 0; i < vecPNT.size(); i++)
	{
		pI[i] = i;
	}
	pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	pMesh->LockAttributeBuffer(flags, &pA);
	memcpy(pA, &vecAttbuf[0], vecAttbuf.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdjacency(pMesh->GetNumFaces() * 3);
	pMesh->GenerateAdjacency(FLT_EPSILON, &vecAdjacency[0]);
	pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, &vecAdjacency[0], NULL, NULL, NULL);

	return pMesh;
}

void ObjLoader::LoadSurface(LPCTSTR fullpath, D3DXMATRIXA16 * pMat, OUT vector<D3DXVECTOR3>& vertices)
{
	vector<D3DXVECTOR3> vecP;
	char token[TOKEN_SIZE];

	std::ifstream fin;
	fin.open(m_filePath + fullpath);

	if (fin.is_open() == false)
		assert(false && "fin.is_open() == false");

	while (fin.eof() == false)
	{
		fin >> token;

		if (CompareStr(token, "v"))
		{
			float x, y, z;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %f", &x, &y, &z);
			vecP.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (CompareStr(token, "f"))
		{
			int aIndex[3];

			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
				&aIndex[0], &aIndex[1], &aIndex[2]);

			for (int i = 0; i < 3; i++)
			{
				D3DXVECTOR3 p;
				p = vecP[aIndex[i] - 1];


				if (pMat)
				{
					D3DXVec3TransformCoord(&p, &p, pMat);
				}
				vertices.push_back(p);
			}
		}
	}

	fin.close();
}

void ObjLoader::LoadMtlLib(LPCTSTR fullpath)
{
	CString mtlName;
	DWORD mtltexID = 0;
	char token[TOKEN_SIZE];
	std::ifstream fin;

	fin.open(fullpath);

	if (fin.is_open() == false)
		assert(false && "fin.is_open() == false");

	while (fin.eof() == false)
	{
		fin >> token;

		if (CompareStr(token, "newmtl"))
		{
			fin >> token;
			mtlName = token;
			if (m_mtltexList.find(mtlName) == m_mtltexList.end())
			{
				m_mtltexList[mtlName] = new MTLTEX;
				m_mtltexList[mtlName]->id = mtltexID++;

			}
		}
		else if (CompareStr(token, "Ka"))
		{
			D3DCOLORVALUE c;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %f", &c.r, &c.g, &c.b);
			c.a = 1.0f;
			m_mtltexList[mtlName]->material.Ambient = c;
		}
		else if (CompareStr(token, "Kd"))
		{
			D3DCOLORVALUE c;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %f", &c.r, &c.g, &c.b);
			c.a = 1.0f;
			m_mtltexList[mtlName]->material.Diffuse = c;
		}
		else if (CompareStr(token, "Ks"))
		{
			D3DCOLORVALUE c;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %f", &c.r, &c.g, &c.b);
			c.a = 1.0f;
			m_mtltexList[mtlName]->material.Specular = c;
		}
		else if (CompareStr(token, "map_Kd"))
		{
			fin >> token;
			m_mtltexList[mtlName]->pTexture = Assets::GetTexture(m_filePath + token);
		}
	}

	fin.close();
}
