#pragma once
/* 
송현국 
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 서울게임 아카데미 김주안 선생님 수업시간에 배운 코드를 설계를 수정하여 적용한 코드 입니다.

남길말 :
Ase Animation이 아직 완성되지 않았음. 선생님 코드 보고 원리를 알고 코드 작성하면 될 것임.
*/
class AseLoader
{
public:
	AseLoader();
	~AseLoader();

	GameObject* Load(CString szFolerPath, CString filename);

	// 토큰을 읽어서 각 자료형으로 변환합니다.
	int			GetTokenAsInt();
	float		GetTokenAsFloat();
	char*		GetTokenNoQuotation();

	// 1. SCENE 정보 (Frame)를 읽어옵니다.
	void ReadScene();

	// 2. 재질 정보를 읽어옵니다.
	void ReadMaterialList();
	// 재질 정보를 읽어와서 재질을 만듭니다.
	void CreateMaterial(int iIndex);
	void SetDiffuseBitmap(int iIndex);

	// 3. 정점 정보를 읽어와서 오브젝트를 만듭니다.
	GameObject* CreateGEOMOBJECT();
	// 월드 행렬을 읽어와서 셋팅합니다.
	void SetWorldMatrix(OUT GameObject* pGO);
	// 메쉬를 셋팅합니다.
	void ReadMesh(OUT GameObject* pGO);
	// 메쉬 정점 정보를 읽어옵니다.
	void ReadMeshVertexList(OUT vector<Vector3>& vecV);
	// 메쉬 페이스 리스트를 읽어옵니다.
	void ReadMeshFaceList(IN vector<Vector3>& vecV, OUT vector<VERTEX_PNT>& vecVertex);
	// 메쉬 텍스쳐 정점 정보를 읽어옵니다.
	void ReadMeshTexVertexList(OUT vector<Vector2>& vecVT);
	// 메쉬 텍스쳐 페이스 리스트를 읽어옵니다.
	void ReadMeshTexFaceList(IN vector<Vector2>& vecVT, OUT vector<VERTEX_PNT>& vecVertex);
	// 메쉬 노멀 정보를 읽어옵니다.
	void ReadMeshNormals(OUT vector<VERTEX_PNT>& vecVertex);

	// 애니메이션 관련 정보 (분리)
	void ReadAnimation(GameObject* pGO);
	void ReadPosTrack(GameObject* pGO);
	void ReadRotTrack(GameObject* pGO);

private:
	// 읽을 파일의 폴더 경로
	CString m_szFolderPath;

	// 파일 스트림
	ifstream			m_fs;
	// 토큰 크기
	static const int	TOKEN_SIZE = 256; 
	// 읽을 문자열
	char				m_szToken[TOKEN_SIZE];

	// 읽을 SCENE
	ASE_SCENE*			m_pScene;

	// 읽을 재질들
	vector<Material>	m_mtlList;
	vector<CString>		m_diffuseMapNameList;
};

