#include "stdafx.h"
#include "AseLoader.h"
#include "Asciitok.h"
#include "ComRenderAse.h"
#include "ComAseAnimation.h"

bool IsEqualString(const char* str1, const char* str2)
{
	// 같으면 0 반환
	return strcmp(str1, str2) == 0;
}

AseLoader::AseLoader() : 
	m_pScene(NULL)
{
}


AseLoader::~AseLoader()
{
}

GameObject * AseLoader::Load(CString szFolerPath, CString filename)
{
	m_szFolderPath = szFolerPath;

	// 파일 열기
	m_fs.open(m_szFolderPath + filename);

	if (m_fs.is_open() == false)
		MessageBox(NULL, L"파일이 없거나 경로가 잘못 되었습니다.", L"에러", MB_OK);

	GameObject* m_pRootGO = NULL;

	// 파일 끝까지 읽기
	while (m_fs.eof() == false)
	{
		// 토큰 읽기
		m_fs >> m_szToken;
		
		if (IsEqualString(m_szToken, ID_SCENE))
		{
			ReadScene();
		}
		else if (IsEqualString(m_szToken, ID_MATERIAL_LIST))
		{
			ReadMaterialList();			
		}
		else if (IsEqualString(m_szToken, ID_GEOMETRY))
		{
			GameObject* pGO = CreateGEOMOBJECT();
			if (m_pRootGO == NULL)
			{
				m_pRootGO = pGO;
			}
		}
	}

	m_fs.close();

	return m_pRootGO;
}

int AseLoader::GetTokenAsInt()
{
	// 한 단락을 읽어서 m_szToken에 저장
	m_fs >> m_szToken;
	return atoi(m_szToken);
}

float AseLoader::GetTokenAsFloat()
{
	m_fs >> m_szToken;
	return atof(m_szToken);
}

char * AseLoader::GetTokenNoQuotation()
{
	// 한 라인을 읽어서 m_szToken에 저장
	m_fs.getline(m_szToken, TOKEN_SIZE, '\"');
	m_fs.getline(m_szToken, TOKEN_SIZE, '\"');

	return m_szToken;
}

void AseLoader::ReadScene()
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_FIRSTFRAME))
		{
			m_pScene = new ASE_SCENE();
			m_pScene->numFirstFrame = GetTokenAsInt();
		}
		else if (IsEqualString(m_szToken, ID_LASTFRAME))
		{
			m_pScene->numLastFrame = GetTokenAsInt();
		}
		else if (IsEqualString(m_szToken, ID_FRAMESPEED))
		{
			m_pScene->FrameSpeed = GetTokenAsInt();
		}
		else if (IsEqualString(m_szToken, ID_TICKSPERFRAME))
		{
			m_pScene->TicksPerFrame = GetTokenAsInt();
		}
	} while (nLevel > 0);
}

void AseLoader::ReadMaterialList()
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		// 재질 갯수
		else if (IsEqualString(m_szToken, ID_MATERIAL_COUNT))
		{
			int size = GetTokenAsInt();
			m_mtlList.resize(size);
			m_diffuseMapNameList.resize(size);
		}
		else if (IsEqualString(m_szToken, ID_MATERIAL))
		{
			CreateMaterial(GetTokenAsInt());
		}
	} while (nLevel > 0);
}

void AseLoader::CreateMaterial(int iIndex)
{
	Material* mtl = &m_mtlList[iIndex];
	ZeroMemory(mtl, sizeof(Material));
	
	// specular value
	mtl->Power = 20.0f;

	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		// 재질 갯수
		else if (IsEqualString(m_szToken, ID_AMBIENT))
		{
			mtl->Ambient.r = GetTokenAsFloat();
			mtl->Ambient.g = GetTokenAsFloat();
			mtl->Ambient.b = GetTokenAsFloat();
			mtl->Ambient.a = 1.0f;
		}
		else if (IsEqualString(m_szToken, ID_DIFFUSE))
		{
			mtl->Diffuse.r = GetTokenAsFloat();
			mtl->Diffuse.g = GetTokenAsFloat();
			mtl->Diffuse.b = GetTokenAsFloat();
			mtl->Diffuse.a = 1.0f;
		}
		else if (IsEqualString(m_szToken, ID_SPECULAR))
		{
			mtl->Specular.r = GetTokenAsFloat();
			mtl->Specular.g = GetTokenAsFloat();
			mtl->Specular.b = GetTokenAsFloat();
			mtl->Specular.a = 1.0f;
		}
		else if (IsEqualString(m_szToken, ID_MAP_DIFFUSE))
		{
			SetDiffuseBitmap(iIndex);
		}
	} while (nLevel > 0);
}

void AseLoader::SetDiffuseBitmap(int iIndex)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		// 재질 갯수
		else if (IsEqualString(m_szToken, ID_BITMAP))
		{
			CString readFileName = CString(GetTokenNoQuotation());
			int index = readFileName.ReverseFind('\\'); // 거꾸로 찾은다음
			CString szFileName = readFileName.Right(readFileName.GetLength() - index - 1); // 파일이름 길이에서 찾은 위치 빼고 '\\'가 붙어 있어서 1 더 뺌

			m_diffuseMapNameList[iIndex] = m_szFolderPath + szFileName;
		}
	} while (nLevel > 0);
}

GameObject * AseLoader::CreateGEOMOBJECT()
{
	GameObject* pGO = NULL;

	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

			 if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_NODE_NAME))
		{
			pGO = new GameObject(GetTokenNoQuotation());
		}
		else if (IsEqualString(m_szToken, ID_NODE_PARENT))
		{
			// multimap find 성능 OLog(N)
			// 이 오브젝트의 부모(Parent)를 설정
			pGO->SetParent(GameObject::Find(GetTokenNoQuotation()));
		}
		else if (IsEqualString(m_szToken, ID_NODE_TM))
		{
			// 이 오브젝트의 월드 행렬 읽어서 설정
			SetWorldMatrix(pGO);
		}
		else if (IsEqualString(m_szToken, ID_MESH))
		{
			// 이 오브젝트의 정점들 읽음
			ReadMesh(pGO);
		}
		else if (IsEqualString(m_szToken, ID_TM_ANIMATION))
		{
			ReadAnimation(pGO);
			ComAseAnimation* comAseAnimation = static_cast<ComAseAnimation*>(pGO->GetComponent("ComAseAnimation"));

			if (comAseAnimation == NULL)
			{
				comAseAnimation = new ComAseAnimation("ComAseAnimation");
				comAseAnimation->SetScene(m_pScene);
				pGO->AddComponent(comAseAnimation);
			}

			comAseAnimation->vecRenderAse.push_back((ComRenderAse*)(pGO->GetComponent("ComRenderAse")));
		}
		else if (IsEqualString(m_szToken, ID_MATERIAL_REF))
		{
			int index = GetTokenAsInt();
			ComRenderAse* renderAse = static_cast<ComRenderAse*>(pGO->GetComponent("ComRenderAse"));
			renderAse->SetMtlTex(&MTLTEX(m_mtlList[index], Assets::GetTexture(m_diffuseMapNameList[index])));
		}
		

	} while (nLevel > 0);

	return pGO;
}

void AseLoader::SetWorldMatrix(OUT GameObject* pGO)
{
	Matrix4x4 matWorld;
	D3DXMatrixIdentity(&matWorld);

	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

			 if (IsEqualString(m_szToken, "{")) { ++nLevel; }
		else if (IsEqualString(m_szToken, "}")) { --nLevel; }
		else if (IsEqualString(m_szToken, ID_TM_ROW0))
		{
			matWorld._11 = GetTokenAsFloat();
			// MAX랑 DirectX랑 축이 다르기 때문에 13을 먼저 설정
			matWorld._13 = GetTokenAsFloat();
			matWorld._12 = GetTokenAsFloat();
			//matWorld._14 = 0.0f;
		}
		else if (IsEqualString(m_szToken, ID_TM_ROW1))
		{
			matWorld._31 = GetTokenAsFloat();
			matWorld._33 = GetTokenAsFloat();
			matWorld._32 = GetTokenAsFloat();
			//matWorld._34 = 0.0f;
		}
		else if (IsEqualString(m_szToken, ID_TM_ROW2))
		{
			matWorld._21 = GetTokenAsFloat();
			matWorld._23 = GetTokenAsFloat();
			matWorld._22 = GetTokenAsFloat();
			//matWorld._24 = 0.0f;
		}
		else if (IsEqualString(m_szToken, ID_TM_ROW3))
		{
			matWorld._41 = GetTokenAsFloat();
			matWorld._43 = GetTokenAsFloat();
			matWorld._42 = GetTokenAsFloat();
			//matWorld._44 = 1.0f;
		}
	} while (nLevel > 0);

	pGO->transform->SetWorldMatrix(&matWorld);
}

void AseLoader::ReadMesh(OUT GameObject * pGO)
{
	vector<Vector3> vecP;
	vector<Vector2> vecT;
	vector<VERTEX_PNT> vecVertex;

	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) { ++nLevel; }
		else if (IsEqualString(m_szToken, "}")) { --nLevel; }
		// 정점의 총 갯수 (Cube경우 8개)
		else if (IsEqualString(m_szToken, ID_MESH_NUMVERTEX)) { vecP.resize(GetTokenAsInt()); }
		// 삼각형(폴리곤)의 총 갯수 (Cube경우 6 x 2 = 12개)
		else if (IsEqualString(m_szToken, ID_MESH_NUMFACES)) { vecVertex.resize(3 * GetTokenAsInt()); }
		else if (IsEqualString(m_szToken, ID_MESH_NUMTVERTEX)) { vecT.resize(GetTokenAsInt()); }
		else if (IsEqualString(m_szToken, ID_MESH_VERTEX_LIST)) { ReadMeshVertexList(vecP); }
		else if (IsEqualString(m_szToken, ID_MESH_FACE_LIST)) { ReadMeshFaceList(vecP, vecVertex); }
		else if (IsEqualString(m_szToken, ID_MESH_TVERTLIST)) { ReadMeshTexVertexList(vecT); }
		//else if (IsEqualString(m_szToken, ID_MESH_NUMTVFACES)) {  }
		else if (IsEqualString(m_szToken, ID_MESH_TFACELIST)) { ReadMeshTexFaceList(vecT, vecVertex); }
		else if (IsEqualString(m_szToken, ID_MESH_NORMALS)) { ReadMeshNormals(vecVertex); }


	} while (nLevel > 0);

	ComRenderAse* comRenderAse = new ComRenderAse("ComRenderAse");
	comRenderAse->m_vertices = vecVertex;
	pGO->AddComponent(comRenderAse);
}

void AseLoader::ReadMeshVertexList(OUT vector<Vector3>& vecV)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;
		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_MESH_VERTEX))
		{
			int index = GetTokenAsInt();
			vecV[index].x = GetTokenAsFloat();
			// MAX랑 DX랑 축이 다르므로 바꿔서 넣어줍니다.
			vecV[index].z = GetTokenAsFloat();
			vecV[index].y = GetTokenAsFloat();
		}

	} while (nLevel > 0);
}

void AseLoader::ReadMeshFaceList(IN vector<Vector3>& vecV, OUT vector<VERTEX_PNT>& vecVertex)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;
		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_MESH_FACE))
		{
			int faceIndex = GetTokenAsInt();
			
			m_fs >> m_szToken; // A
			int nA = GetTokenAsInt();
			m_fs >> m_szToken; // B
			int nB = GetTokenAsInt();
			m_fs >> m_szToken; // C
			int nC = GetTokenAsInt();

			// MAX랑 DX랑 축이 다르므로
			vecVertex[faceIndex * 3 + 0].p = vecV[nA];
			vecVertex[faceIndex * 3 + 1].p = vecV[nC];
			vecVertex[faceIndex * 3 + 2].p = vecV[nB];
		}
	} while (nLevel > 0);
}

void AseLoader::ReadMeshTexVertexList(OUT vector<Vector2>& vecVT)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;
		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_MESH_TVERT))
		{
			int index = GetTokenAsInt();
			vecVT[index].x = GetTokenAsFloat();
			vecVT[index].y = 1.0f - GetTokenAsFloat();
		}
	} while (nLevel > 0);
}

void AseLoader::ReadMeshTexFaceList(IN vector<Vector2>& vecVT, OUT vector<VERTEX_PNT>& vecVertex)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;
		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_MESH_TFACE))
		{
			int faceIndex = GetTokenAsInt();
			int nA = GetTokenAsInt();
			int nB = GetTokenAsInt();
			int nC = GetTokenAsInt();

			vecVertex[faceIndex * 3].t = vecVT[nA];
			vecVertex[faceIndex * 3 + 1].t = vecVT[nC];
			vecVertex[faceIndex * 3 + 2].t = vecVT[nB];
		}
	} while (nLevel > 0);
}

void AseLoader::ReadMeshNormals(OUT vector<VERTEX_PNT>& vecVertex)
{
	int faceIndex = 0;
	int vertexCount = 0;
	int aVertexIndex[] = { 0, 2, 1 };

	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;
		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_MESH_FACENORMAL))
		{
			faceIndex = GetTokenAsInt();
			vertexCount = 0;
		}
		else if (IsEqualString(m_szToken, ID_MESH_FACENORMAL))
		{
			m_fs >> m_szToken;
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.x = GetTokenAsFloat();
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.z = GetTokenAsFloat();
			vecVertex[faceIndex * 3 + aVertexIndex[vertexCount]].n.y = GetTokenAsFloat();
			++vertexCount;
		}
	} while (nLevel > 0);
}

void AseLoader::ReadAnimation(GameObject* pGO)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_POS_TRACK))
		{
			ReadPosTrack(pGO);
		}
		else if (IsEqualString(m_szToken, ID_ROT_TRACK))
		{
			ReadRotTrack(pGO);
		}
	} while (nLevel > 0);
}

void AseLoader::ReadPosTrack(GameObject* pGO)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_POS_SAMPLE))
		{
			ASE_POS_SAMPLE posSample;
			posSample.tick = GetTokenAsInt();
			posSample.position.x = GetTokenAsFloat();
			posSample.position.z = GetTokenAsFloat();
			posSample.position.y = GetTokenAsFloat();

			ComRenderAse* renderAse = static_cast<ComRenderAse*>(pGO->GetComponent("ComRenderAse"));
			
			renderAse->AddPosSample(&posSample);
		}
	} while (nLevel > 0);
}

void AseLoader::ReadRotTrack(GameObject* pGO)
{
	int nLevel = 0;
	do
	{
		m_fs >> m_szToken;

		if (IsEqualString(m_szToken, "{")) ++nLevel;
		else if (IsEqualString(m_szToken, "}")) --nLevel;
		else if (IsEqualString(m_szToken, ID_ROT_TRACK))
		{
			ASE_ROT_SAMPLE rotSample;
			rotSample.tick = GetTokenAsInt();
			rotSample.quaternion.x = GetTokenAsFloat();
			rotSample.quaternion.z = GetTokenAsFloat();
			rotSample.quaternion.y = GetTokenAsFloat();
			rotSample.quaternion.w = GetTokenAsFloat();

			float w = sinf(rotSample.quaternion.w / 2.0f);
			rotSample.quaternion.x *= sinf(w);
			rotSample.quaternion.z *= sinf(w);
			rotSample.quaternion.y *= sinf(w);
			rotSample.quaternion.w = cosf(rotSample.quaternion.w / 2.0f);

			ComRenderAse* renderAse = static_cast<ComRenderAse*>(pGO->GetComponent("ComRenderAse"));
			if (renderAse->GetRotTrack()->empty() == false)
				rotSample.quaternion = renderAse->GetRotTrack()->back().quaternion * rotSample.quaternion;

			renderAse->AddRotSample(&rotSample);
		}
	} while (nLevel > 0);
}
