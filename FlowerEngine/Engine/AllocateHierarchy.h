/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ������� ��ī���� ���־� ������ �����ð��� ��� �ڵ带 ������ �ڵ� �Դϴ�.

���� : ����(Frame)�� �Ž� �����̳�, ���������� Ŭ������ �Դϴ�.
*/
#pragma once

struct Frame : public D3DXFRAME
{
	// root ���� �ڽ� ������ ������ ������ ��� ����
	Matrix4x4 CombinedTM;
};

struct MeshContainer : public D3DXMESHCONTAINER
{
	MeshContainer()
	{
		memset(this, 0, sizeof(MeshContainer));
	}

	// ������ �ؽ��� ������
	vector<MTLTEX*> vecMtlTex;
	// �������� ���� ���� �Ž�(Update ���� �������� ������ Animation ����)
	Mesh pOrigMesh;
	// �� �Ž��� ������ �ִ� ������� ���� ��� �������� �迭
	Matrix4x4** ppBoneMatrixPtrs;
	// ���� �Ž��� �� ������ ���ð������� ������ ��� �迭
	Matrix4x4* pBoneOffsetMatrices;

	// ��Ų�Ž� ����
	DWORD NumPaletteEntries;
	DWORD NumInfl;
	DWORD NumAttributeGroups;
	LPD3DXBUFFER pBoneCombinationBuf;
};

// �Ҵ�� ���� �ڷᱸ��
class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	AllocateHierarchy(CString szPath);
	~AllocateHierarchy();

	VARIATION(int, NumBoneMatricesMax, NumBoneMatricesMax);

	// ID3DXAllocateHierarchy��(��) ���� ��ӵ�

	// ������ ���� : ���� �޸𸮸� �Ҵ��ؼ� �������� �����ϰ� ppNewFrame ���ڿ� ����
	// ������ �������� �̸��� �𵨸� ���α׷����� ������ �������� �̸��� ���ڷ� �޾� ����
	// ������ ��ü�� Ʈ�������� �ٹ̱� ���� �غ� �۾�
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame) override;
	
	// �Ž� �����̳� ���� : ���� �޸𸮸� �Ҵ��ؼ� �������� �����ϰ� ppNewMeshContainer ���ڿ� ����
	// �Ž��� �̸�, ����, �ؽ��ĸ� ����
	// ������ �Ž� �����̳ʿ� pSkinInfo�� ����, Skinning�� ���� �غ� �۾�
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
	
	// �����ߴ� �������� ����
	// ������ ���� �Ҵ� ����� �κ��� ��� ����
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

	// �����ߴ� �Ž� �����̳ʸ� ����
	// ������ ���� �Ҵ� ����� �κ��� ��� ����
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;

	// �Ž� �����̳ʸ� ���� ��Ű�� �Ž��� ����ϴ�.
	void GenerateSkinnedMesh(MeshContainer* pMeshContainer);

protected:
	Device9 pDevice9;
	CString m_szFolderPath;
};

