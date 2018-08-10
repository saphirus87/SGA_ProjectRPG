#include "stdafx.h"
#include "ComTerrain.h"
#include "QuadTree.h"

const int TOKEN_SIZE = 128;
// 임시
const float radius = 6.5f;

// 상수
const UINT mapSize = 129;


ComTerrain::ComTerrain(CString szName) :
	Component(szName),
	m_rayDistance(2.f),
	m_InverseUV(0.0f),
	m_pVB(NULL),
	m_pIB(NULL)
{
	pDevice9 = GetD3D9Device();
	m_pEffect = Shaders::GetInstance()->GetShader(SHADER_PATH + "/ShaderPT.fx");
}

ComTerrain::~ComTerrain()
{
	SAFE_DELETE(m_pQuadTree);

	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);

	for (auto & mtrl : m_mtltexList)
		SAFE_DELETE(mtrl.second);

	m_mtltexList.clear();

	m_vecVertices.clear();
	m_vecIndices.clear();
	m_testIndices.clear();
}

void ComTerrain::Awake()
{
	if (m_mapFilename.IsEmpty() == false)
	{
		m_InverseUV = 0.0f;
		LoadMap();

		m_pQuadTree = new QuadTree(mapSize, mapSize);

		return;
		for (int i = 0; i < mapSize; ++i)
		{
			for (int j = 0; j < mapSize; ++j)
			{
				if (i == 128)
				{
					int xIndex = 0;
					int zIndex = 0;

					//xIndex = 

					if (j == 0) m_vecQuadIdx.push_back(2 + ((i - 1) * (mapSize - 1)) * 12);
					else m_vecQuadIdx.push_back(8 + ((i - 1) * (mapSize - 1) + j - 1) * 12);
				}
				else
				{
					if (j == 0) m_vecQuadIdx.push_back(1 + (i * (mapSize - 1)) * 12);
					else m_vecQuadIdx.push_back(4 + (i * (mapSize - 1) + j - 1) * 12);
				}
			}
		}

		m_pQuadTree->SetMapVertex(&m_vecVertices);
		m_pQuadTree->SetIndex(&m_vecQuadIdx);
		m_pQuadTree->Build();
		m_pIndex = new DWORD;
	}
}

void ComTerrain::Update()
{
	/*if (Input::m_pKeyboard->KeyDown(VK_SPACE)) UpdateIndexBuffer();*/

	if (Input::m_pKeyboard->KeyDown(VK_SPACE))
	{
		UpdateIndexBuffer();
		//UpdateIndexBufferQuadTree();
	}
}

void ComTerrain::Render()
{
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);

	m_pEffect->SetMatrix("gWorldMatrix", &mat);
	m_pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());

	m_pEffect->Begin(0, 0);
	m_pEffect->BeginPass(0);

	m_pEffect->SetTexture("DiffuseMap_Tex", m_mtltexList.begin()->second->pTexture);
	m_pEffect->CommitChanges();

	pDevice9->SetFVF(VERTEX_PNT::FVF);
	pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PNT));
	pDevice9->SetIndices(m_pIB);

	pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vecVertices.size(), 0, m_vecIndices.size() / 3);

	m_pEffect->EndPass();
	m_pEffect->End();
}

bool ComTerrain::GetHeight(float & height, const D3DXVECTOR3 & pos)
{
	D3DXVECTOR3 rayPos(pos.x, pos.y + m_rayDistance, pos.z);
	D3DXVECTOR3 rayDir(0, -1, 0);
	float distance = 0.0f;

	for (int i = 0; i < m_vecIndices.size(); i += 3)
	{
		D3DXVECTOR3 v1 = m_vecVertices[m_vecIndices[i]].p;
		D3DXVECTOR3 v2 = m_vecVertices[m_vecIndices[i + 1]].p;
		D3DXVECTOR3 v3 = m_vecVertices[m_vecIndices[i + 2]].p;

		if (D3DXIntersectTri(&v1, &v2, &v3, &rayPos, &rayDir, NULL, NULL, &distance))
		{
			height = rayPos.y - distance;
			return true;
		}
	}

	return false;
}

bool ComTerrain::CalcPickedPosition(D3DXVECTOR3 & out, WORD screenX, WORD screenY)
{
	Ray ray = Ray::RayAtWorldSpace(screenX, screenY);
	float minDist = FLT_MAX;
	float dist = 0;
	bool bIntersect = false;

	for (size_t i = 0; i < m_surfaceVertices.size(); i += 3)
	{
		if (ray.CalcIntersectTri(&m_surfaceVertices[i], &dist))
		{
			if (dist < minDist)
			{
				bIntersect = true;
				minDist = dist;
				out = ray.m_pos + dist * ray.m_dir;
			}
		}
	}

	return bIntersect;
}

void ComTerrain::LoadMap()
{
	vector<Vector3> vecP;
	vector<DWORD> vecAttbuf;
	CString mtlName;
	char token[TOKEN_SIZE];

	std::ifstream fin;

	fin.open(m_filePath + m_mapFilename);

	if (fin.is_open() == false)
		assert(false && "fin.is_open() == false");

	float fPosXMax = 0.0f;
	float fPosZMax = 0.0f;

	while (fin.eof() == false)
	{
		fin >> token;

		if (CompareStr(token, "mtllib"))
		{
			fin >> token;
			LoadMtlLib(m_filePath + token);
		}
		else if (CompareStr(token, "v"))
		{
			float x, y, z;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %f", &x, &y, &z);
			if (x > fPosXMax)
				fPosXMax = x;
			if (z > fPosZMax)
				fPosZMax = z;
			vecP.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (CompareStr(token, "usemtl"))
		{
			fin >> token;
			mtlName = token;
		}
		else if (CompareStr(token, "f"))
		{
			int aIndex[3] = { 0, 0, 0 };

			fin.getline(token, TOKEN_SIZE);

			sscanf_s(token, "%d %d %d", &aIndex[0], &aIndex[1], &aIndex[2]);

			// 삼각형 하나
			for (int i = 0; i < 3; ++i)
			{
				// OBJ 파일에는 INDEX가 1부터 들어가는데 배열 0부터 집어 넣으려고 -1
				m_vecIndices.push_back((DWORD)aIndex[i]-1);
			}

			vecAttbuf.push_back(m_mtltexList[mtlName]->id);
		}
	}
	fin.close();

	// Hyuns Test START
	//m_vertices.clear();
	////m_mtltexList.clear();

	//VERTEX_PNT vertex;

	//float fTileSize = 100.0f;
	//// 0
	//vertex.p = Vector3(0, 0, 0);
	//vertex.t = Vector2(0, 1);
	//vertex.n = Vector3(0, 1, 0);
	//m_vertices.push_back(vertex);
	//m_surfaceVertices.push_back(vertex.p);

	//// 1
	//vertex.p = Vector3(0, 0, fTileSize);
	//vertex.t = Vector2(0, 0);
	//vertex.n = Vector3(0, 1, 0);
	//m_vertices.push_back(vertex);
	//m_surfaceVertices.push_back(vertex.p);

	//// 2
	//vertex.p = Vector3(fTileSize, 10, fTileSize);
	//vertex.t = Vector2(1, 0);
	//vertex.n = Vector3(0, 1, 0);
	//m_vertices.push_back(vertex);
	//m_surfaceVertices.push_back(vertex.p);

	//// 3
	//vertex.p = Vector3(fTileSize, 0, 0);
	//vertex.t = Vector2(1, 1);
	//vertex.n = Vector3(0, 1, 0);
	//m_vertices.push_back(vertex);
	//m_surfaceVertices.push_back(vertex.p);

	//m_surfaceIndices.push_back(0);
	//m_surfaceIndices.push_back(1);
	//m_surfaceIndices.push_back(2);

	//m_surfaceIndices.push_back(0);
	//m_surfaceIndices.push_back(2);
	//m_surfaceIndices.push_back(3);

	// Hyuns Test END
	
	for (size_t i = 0; i < vecP.size(); ++i)
	{
		VERTEX_PNT pnt;
		pnt.p = vecP[i];

		Vector2 vT(pnt.p.x, pnt.p.z);
		
		// 원본 코드
		//vT.x /= fPosXMax; // 533.613770 - 상수로 표현하면
		//vT.y = 1.0f - (vT.y / fPosZMax); // 533.236572 - 상수로 표현하면
		
		vT.x /= 533.613770;
		vT.y = 1.0f - (vT.y / 533.236572);

		pnt.t = vT;
		pnt.n = Vector3(0, 1, 0);
		D3DXVec3Normalize(&pnt.n, &pnt.n);
		m_vecVertices.push_back(pnt);
	}

	// buffer 설정 부분
	pDevice9->CreateVertexBuffer(m_vecVertices.size() * sizeof(VERTEX_PNT), NULL, VERTEX_PNT::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	VERTEX_PNT* pVertex;
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &m_vecVertices[0], m_vecVertices.size() * sizeof(VERTEX_PNT));
	m_pVB->Unlock();

	pDevice9->CreateIndexBuffer(m_vecIndices.size() * sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, NULL);

	DWORD* pIndex;
	m_pIB->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &m_vecIndices[0], m_vecIndices.size() * sizeof(DWORD));
	m_pIB->Unlock();
}

void ComTerrain::LoadMtlLib(LPCTSTR fullPath)
{
	CString mtlName;
	DWORD mtltexID = 0;
	char token[TOKEN_SIZE];
	std::ifstream fin;

	fin.open(fullPath);

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

			CString readFileName = CString(token);
			int index = readFileName.ReverseFind('\\'); // 거꾸로 찾은다음
			CString szFileName = readFileName.Right(readFileName.GetLength() - index - 1); // 파일이름 길이에서 찾은 위치 빼고 '\\'가 붙어 있어서 1 더 뺌

			m_mtltexList[mtlName]->pTexture = Assets::GetTexture(m_filePath + szFileName);
		}
	}

	fin.close();
}

void ComTerrain::UpdateIndexBuffer()
{
	if (!m_vecIndices.empty()) m_vecIndices.clear();

	const int num = 12;
	for (int i = 0; i < m_vecVertices.size(); i += num)
	{
		if (Camera::GetInstance()->FrustumCulling(&m_vecVertices[i].p, radius))
		{
			for (int j = 0; j < num; j++)
			{
				m_vecIndices.push_back(i + j);
			}
		}
	}

	if (m_pIB != NULL) SAFE_RELEASE(m_pIB);

	if (m_vecIndices.size() <= 0) return;

	pDevice9->CreateIndexBuffer(m_vecIndices.size() * sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL);

	DWORD* pIndex;
	m_pIB->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &m_vecIndices[0], m_vecIndices.size() * sizeof(DWORD));
	m_pIB->Unlock();
}

void ComTerrain::UpdateIndexBufferQuadTree()
{
	if (!m_testIndices.empty()) m_testIndices.clear();

	m_TriangleNum = m_pQuadTree->GenerateIndex(m_testIndices);

	if (m_pIB != NULL) SAFE_RELEASE(m_pIB);

	if (m_TriangleNum <= 0) return;

	pDevice9->CreateIndexBuffer(m_testIndices.size() * sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL);

	DWORD* pIndex;
	m_pIB->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &m_testIndices[0], m_testIndices.size() * sizeof(DWORD));
	m_pIB->Unlock();
}
