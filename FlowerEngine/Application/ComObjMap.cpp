#include "stdafx.h"
#include "ComObjMap.h"
#include "QuadTree.h"

const int TOKEN_SIZE = 128;

ComObjMap::ComObjMap(CString szName) :
	Component(szName),
	m_TriangleNum(0),
	m_rayDistance(2.f),
	m_InverseUV(0.0f),
	m_pVB(NULL),
	m_pIB(NULL)
{
	pDevice9 = GetD3D9Device();
	m_pEffect = Shaders::GetInstance()->GetShader(SHADER_PATH + "/ShaderLightingTexture.fx");
}

ComObjMap::~ComObjMap()
{
	SAFE_DELETE(m_pQuadTree);

	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);

	for (auto & mtrl : m_mtltexList)
		SAFE_DELETE(mtrl.second);

	m_mtltexList.clear();

	m_vertices.clear();
	m_surfaceIndices.clear();
	m_OptVertices.clear();
	m_vecQuadIdx.clear();
}

void ComObjMap::Awake()
{
	if (m_mapFilename.IsEmpty() == false)
	{
		m_InverseUV = 5.0f;
		LoadMap();

		VertexOptimization();

		m_pQuadTree = new QuadTree(mapSize, mapSize);
		m_pQuadTree->SetMapVertex(&m_OptVertices);
		m_pQuadTree->SetIndex(&m_vecQuadIdx);
 		m_pQuadTree->Build();
	}
}

void ComObjMap::Update()
{
	//if (Input::KeyDown(VK_SPACE))
		//UpdateIndexBufferQuadTree();
}

void ComObjMap::Render()
{
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);

	m_pEffect->SetMatrix("gWorldMatrix", &mat);
	m_pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());

	m_pEffect->Begin(0, 0);
	m_pEffect->BeginPass(0);

	m_pEffect->SetTexture("DiffuseMap_Tex", m_mtltexList.begin()->second->pTexture);

	pDevice9->SetFVF(VERTEX_PNT::FVF);
	pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PNT));
	pDevice9->SetIndices(m_pIB);

	pDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertices.size(), 0, m_TriangleNum);

	m_pEffect->EndPass();
	m_pEffect->End();
}

bool ComObjMap::GetHeight(float & height, const D3DXVECTOR3 & pos)
{
	//return false;
	D3DXVECTOR3 rayPos(pos.x, pos.y + m_rayDistance, pos.z);
	D3DXVECTOR3 rayDir(0, -1, 0);
	float distance = 0.0f;

	for (int i = 0; i < m_TriangleNum * 3; i += 3)
	{
		D3DXVECTOR3 TargetPos(pos.x, 0, pos.z);
		D3DXVECTOR3 TerrainPos(m_OptVertices[m_surfaceIndices[i]].p.x, 0, m_OptVertices[m_surfaceIndices[i]].p.z);
		if (D3DXVec3Length(&(TargetPos - TerrainPos)) >= 5.0f) continue;


		D3DXVECTOR3 v1 = m_OptVertices[m_surfaceIndices[i]].p;
		D3DXVECTOR3 v2 = m_OptVertices[m_surfaceIndices[i + 1]].p;
		D3DXVECTOR3 v3 = m_OptVertices[m_surfaceIndices[i + 2]].p;

		if (D3DXIntersectTri(&v1, &v2, &v3, &rayPos, &rayDir, NULL, NULL, &distance))
		{
			height = rayPos.y - distance;
			return true;
		}
	}

	return false;
}

bool ComObjMap::CalcPickedPosition(D3DXVECTOR3 & out, WORD screenX, WORD screenY)
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

void ComObjMap::LoadMap()
{
	vector<D3DXVECTOR3> vecP;
	vector<D3DXVECTOR2> vecT;
	vector<D3DXVECTOR3> vecN;
	vector<DWORD> vecAttbuf;
	CString mtlName;
	char token[TOKEN_SIZE];

	std::ifstream fin;

	fin.open(m_filePath + m_mapFilename);

	if (fin.is_open() == false)
		assert(false && "fin.is_open() == false");

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
			vecP.push_back(D3DXVECTOR3(x, y, z));
		}
		else if (CompareStr(token, "vt"))
		{
			float x, y;
			fin.getline(token, TOKEN_SIZE);
			sscanf_s(token, "%f %f %*f", &x, &y);
			if (m_InverseUV > 0.1f) vecT.push_back(D3DXVECTOR2(x, m_InverseUV - y));
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

				// 적용시킬 Matrix가 있는 경우(일단은 무시)
				/*if (pMat)
				{
				D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
				D3DXVec3TransformCoord(&pnt.n, &pnt.n, pMat);
				}*/
				D3DXVec3Normalize(&pnt.n, &pnt.n);
				m_vertices.push_back(pnt);
			}
			vecAttbuf.push_back(m_mtltexList[mtlName]->id);
		}
	}

	//m_mtltexList.clear();
	fin.close();

	// buffer 설정 부분
	pDevice9->CreateVertexBuffer(m_vertices.size() * sizeof(VERTEX_PNT), NULL, VERTEX_PNT::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	VERTEX_PNT* pVertex;
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &m_vertices[0], m_vertices.size() * sizeof(VERTEX_PNT));
	m_pVB->Unlock();
}

void ComObjMap::LoadMtlLib(LPCTSTR fullPath)
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
			m_mtltexList[mtlName]->pTexture = Assets::GetTexture(m_filePath + token);
		}
	}

	fin.close();
}

void ComObjMap::UpdateIndexBufferQuadTree()
{
	m_TriangleNum = m_pQuadTree->GenerateIndex(m_surfaceIndices);

	if (m_TriangleNum <= 0) return;

	DWORD* pIndex;
	m_pIB->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, &m_surfaceIndices[0], m_TriangleNum * 3 * sizeof(DWORD));
	m_pIB->Unlock();
}

void ComObjMap::VertexOptimization()
{
	for (int i = 0; i < m_vertices.size(); i += 12)
	{
		m_OptVertices.push_back(m_vertices[i + 7]);
		m_OptVertices.push_back(m_vertices[i + 8]);
		m_OptVertices.push_back(m_vertices[i + 1]);

		m_OptVertices.push_back(m_vertices[i + 7]);
		m_OptVertices.push_back(m_vertices[i + 1]);
		m_OptVertices.push_back(m_vertices[i + 2]);
	}

	if (m_pVB != NULL) SAFE_RELEASE(m_pVB);

	pDevice9->CreateVertexBuffer(m_OptVertices.size() * sizeof(VERTEX_PNT), NULL, VERTEX_PNT::FVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	VERTEX_PNT* pVertex;
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, &m_OptVertices[0], m_OptVertices.size() * sizeof(VERTEX_PNT));
	m_pVB->Unlock();

	m_surfaceIndices.resize(m_OptVertices.size());
	pDevice9->CreateIndexBuffer(m_surfaceIndices.size() * sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL);

	// QuadTree Culling에 사용할 인덱스 값 생성
	for (int i = 0; i < (mapSize - 1) * (mapSize - 1); ++i)
	{
		// test idx
		int TotalIndexX = i % 128;
		int TotalIndexZ = i / 128;

		// 8X8 타일의 인덱스 값
		int sqIndexX = TotalIndexX / 8;
		int sqIndexZ = TotalIndexZ / 8;
		int sqIndex = sqIndexX + sqIndexZ * 16;

		// 8X8 타일 내 인덱스 값
		int IndexX = TotalIndexX % 8;
		int IndexZ = TotalIndexZ % 8;
		int Index = IndexX + IndexZ * 8;

		// 최종 인덱스
		int result = sqIndex * 64 + IndexX + IndexZ * 8;

		// 기본적으로 사각형에 좌측 하단 좌표를 삽입
		// 그럴 경우 최상단 1줄의 좌표가 없게 되므로 처음에 1줄을 삽입
		// 각 줄의 맨 오른쪽 좌표 누락되었기 때문에 그 부분 추가
		// 마지막으로 사각형에서 원하는 정점을 넣기 위한 부분 추가

		if (i == 0)
		{
			for (int j = 0; j < mapSize - 1; ++j)
			{
				if (j == 0) m_vecQuadIdx.push_back(2);
				m_vecQuadIdx.push_back((j % 8 + (j / 8) * 64) * 6 + 1);
			}
		}

		if (i % (mapSize - 1) == 0) m_vecQuadIdx.push_back(result * 6 + 5);
		m_vecQuadIdx.push_back(result * 6);
	}
}