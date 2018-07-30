#pragma once
/* 
������ 
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ������� ��ī���� ���־� ������ �����ð��� ��� �ڵ带 ���踦 �����Ͽ� ������ �ڵ� �Դϴ�.

���渻 :
Ase Animation�� ���� �ϼ����� �ʾ���. ������ �ڵ� ���� ������ �˰� �ڵ� �ۼ��ϸ� �� ����.
*/
class AseLoader
{
public:
	AseLoader();
	~AseLoader();

	GameObject* Load(CString szFolerPath, CString filename);

	// ��ū�� �о �� �ڷ������� ��ȯ�մϴ�.
	int			GetTokenAsInt();
	float		GetTokenAsFloat();
	char*		GetTokenNoQuotation();

	// 1. SCENE ���� (Frame)�� �о�ɴϴ�.
	void ReadScene();

	// 2. ���� ������ �о�ɴϴ�.
	void ReadMaterialList();
	// ���� ������ �о�ͼ� ������ ����ϴ�.
	void CreateMaterial(int iIndex);
	void SetDiffuseBitmap(int iIndex);

	// 3. ���� ������ �о�ͼ� ������Ʈ�� ����ϴ�.
	GameObject* CreateGEOMOBJECT();
	// ���� ����� �о�ͼ� �����մϴ�.
	void SetWorldMatrix(OUT GameObject* pGO);
	// �޽��� �����մϴ�.
	void ReadMesh(OUT GameObject* pGO);
	// �޽� ���� ������ �о�ɴϴ�.
	void ReadMeshVertexList(OUT vector<Vector3>& vecV);
	// �޽� ���̽� ����Ʈ�� �о�ɴϴ�.
	void ReadMeshFaceList(IN vector<Vector3>& vecV, OUT vector<VERTEX_PNT>& vecVertex);
	// �޽� �ؽ��� ���� ������ �о�ɴϴ�.
	void ReadMeshTexVertexList(OUT vector<Vector2>& vecVT);
	// �޽� �ؽ��� ���̽� ����Ʈ�� �о�ɴϴ�.
	void ReadMeshTexFaceList(IN vector<Vector2>& vecVT, OUT vector<VERTEX_PNT>& vecVertex);
	// �޽� ��� ������ �о�ɴϴ�.
	void ReadMeshNormals(OUT vector<VERTEX_PNT>& vecVertex);

	// �ִϸ��̼� ���� ���� (�и�)
	void ReadAnimation(GameObject* pGO);
	void ReadPosTrack(GameObject* pGO);
	void ReadRotTrack(GameObject* pGO);

private:
	// ���� ������ ���� ���
	CString m_szFolderPath;

	// ���� ��Ʈ��
	ifstream			m_fs;
	// ��ū ũ��
	static const int	TOKEN_SIZE = 256; 
	// ���� ���ڿ�
	char				m_szToken[TOKEN_SIZE];

	// ���� SCENE
	ASE_SCENE*			m_pScene;

	// ���� ������
	vector<Material>	m_mtlList;
	vector<CString>		m_diffuseMapNameList;
};

