/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 서울게임 아카데미 김주안 선생님 수업시간에 배운 코드를 정리한 코드 입니다.

설명 : 뼈대(Frame)과 매쉬 컨테이너, 계층구조의 클래스들 입니다.
*/
#pragma once

struct Frame : public D3DXFRAME
{
	// root 부터 자신 프레임 까지의 누적된 행렬 정보
	Matrix4x4 CombinedTM;
};

struct MeshContainer : public D3DXMESHCONTAINER
{
	MeshContainer()
	{
		memset(this, 0, sizeof(MeshContainer));
	}

	// 재질과 텍스쳐 정보들
	vector<MTLTEX*> vecMtlTex;
	// 수정하지 않은 원본 매쉬(Update 마다 원본값을 참조해 Animation 연산)
	Mesh pOrigMesh;
	// 이 매쉬에 영향을 주는 뼈대들의 월드 행렬 포인터의 배열
	Matrix4x4** ppBoneMatrixPtrs;
	// 원본 매쉬를 각 뼈대의 로컬공간으로 보내는 행렬 배열
	Matrix4x4* pBoneOffsetMatrices;

	// 스킨매쉬 정보
	DWORD NumPaletteEntries;
	DWORD NumInfl;
	DWORD NumAttributeGroups;
	LPD3DXBUFFER pBoneCombinationBuf;
};

// 할당된 계층 자료구조
class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	AllocateHierarchy(CString szPath);
	~AllocateHierarchy();

	VARIATION(int, NumBoneMatricesMax, NumBoneMatricesMax);

	// ID3DXAllocateHierarchy을(를) 통해 상속됨

	// 프레임 생성 : 새로 메모리를 할당해서 프레임을 생성하고 ppNewFrame 인자에 연결
	// 생성한 프레임의 이름은 모델링 프로그램에서 정해준 프레임의 이름을 인자로 받아 지정
	// 프레임 전체를 트리구조로 꾸미기 위한 준비 작업
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame) override;
	
	// 매쉬 컨테이너 생성 : 새로 메모리를 할당해서 프레임을 생성하고 ppNewMeshContainer 인자에 연결
	// 매쉬의 이름, 재질, 텍스쳐를 지정
	// 생성한 매쉬 컨테이너에 pSkinInfo를 연결, Skinning을 위한 준비 작업
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
	
	// 생성했던 프레임을 해제
	// 생성시 동적 할당 해줬던 부분을 모두 해제
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

	// 생성했던 매쉬 컨테이너를 해제
	// 생성시 동적 할당 해줬던 부분을 모두 해제
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;

	// 매쉬 컨테이너를 통해 스키닝 매쉬를 만듭니다.
	void GenerateSkinnedMesh(MeshContainer* pMeshContainer);

protected:
	Device9 pDevice9;
	CString m_szFolderPath;
};

