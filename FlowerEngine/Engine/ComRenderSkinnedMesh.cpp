#include "stdafx.h"
#include "ComRenderSkinnedMesh.h"


ComRenderSkinnedMesh::ComRenderSkinnedMesh(CString szName) :
	Component(szName),
	m_pEffect(NULL),
	m_pRootFrame(NULL),
	m_pAniControl(NULL),
	m_pSubRootFrame(NULL),
	m_pConnetFrame(NULL),
	m_pSubAniControl(NULL),
	m_iAniIndex(0),
	m_fBlendTime(0.3f),
	m_fPassedBlendTime(0),
	m_pBoneMatrices(NULL),
	m_pAllocateHierarchy(NULL),
	pDevice9(GetD3D9Device()),
	m_iReference(0)
{
}

ComRenderSkinnedMesh::~ComRenderSkinnedMesh()
{
	if (m_iReference < 1)
	{
		D3DXFrameDestroy(m_pRootFrame, m_pAllocateHierarchy);
		D3DXFrameDestroy(m_pSubRootFrame, m_pAllocateHierarchy);
		SAFE_DELETE_ARRAY(m_pBoneMatrices);
		m_mapBoneMatrices.clear();
		SAFE_DELETE(m_pAllocateHierarchy);
	}
	else
		--m_iReference;
}

void ComRenderSkinnedMesh::Load(CString szFolderPath, CString szFileName)
{
	// 셰이더 로드
	m_pEffect = Shaders::GetInstance()->GetShader(L"Resources/shader/skinnedmesh.fx");

	// 1. 뼈대 계층 구조 생성
	m_pAllocateHierarchy = new AllocateHierarchy(szFolderPath);

	// 2. x파일로 부터 애니메이션 매쉬 (주로 캐릭터)를 읽어오기
	D3DXLoadMeshHierarchyFromX(szFolderPath + szFileName, D3DXMESH_MANAGED, pDevice9, m_pAllocateHierarchy, NULL, &m_pRootFrame, &m_pAniControl);
	
	int numTrack = m_pAniControl->GetMaxNumTracks();
	for (int iTrack = 0; iTrack < numTrack; ++iTrack)
		m_pAniControl->SetTrackSpeed(iTrack, 0.5f);

	// 3. 루트 뼈대 부터 행렬 포인터들 설정
	SetupBoneMatrixPointers(m_pRootFrame);

	// 4. 뼈대 행렬들 메모리 할당
	m_pBoneMatrices = new Matrix4x4[m_pAllocateHierarchy->GetNumBoneMatricesMax()];
}

void ComRenderSkinnedMesh::Clone(ComRenderSkinnedMesh* pExist)
{
	// 존재하는 게임오브젝트의 렌더 스킨매쉬 구성요소(pExist) 맴버 변수를 이 렌더 스킨매쉬 구성요소로 복제
	m_pAllocateHierarchy = pExist->m_pAllocateHierarchy;
	m_pRootFrame = pExist->m_pRootFrame;
	m_pBoneMatrices = pExist->m_pBoneMatrices;
	m_mapBoneMatrices = pExist->m_mapBoneMatrices;

	m_pEffect = pExist->m_pEffect;

	// 존재하는 애니메이션 컨트롤러(pExist->m_pAniControl)를 이 컴포넌트의 애니메이션 컨트롤러(m_pAniControl)로 복제
	AnimationController pAC = pExist->m_pAniControl;
	pAC->CloneAnimationController(
		pAC->GetMaxNumAnimationOutputs(), 
		pAC->GetMaxNumAnimationSets(), 
		pAC->GetMaxNumTracks(),
		pAC->GetMaxNumEvents(),
		&m_pAniControl);

	++m_iReference;
}

void ComRenderSkinnedMesh::Awake()
{
	if (m_pAniControl != NULL)
	{
		// 애니메이션이 있다면 기본 애니메이션 재생
		PlayAnimation(m_pAniControl, 0, false);
		// 애니메이션 트랙 초기화
		m_pAniControl->SetTrackPosition(0, 0);
	}
}

void ComRenderSkinnedMesh::Update()
{
	// 애니메이션 업데이트와 전환시 부드럽게하기 위해 블랜딩(Blending)
	UpdateAnimation(m_pAniControl);

	// 뼈대의 움직임 적용을 위한 뼈대의 최종 월드 행렬(Combined) 계산
	UpdateFrameMatrices(m_pRootFrame, NULL);
}

void ComRenderSkinnedMesh::Render()
{
	// 이 게임 오브젝트를 월드 공간으로 보내기 위해 정점 셰이더에 이 게임 오브젝트의 월드 행렬 설정
	m_pEffect->SetMatrix("mView", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("mProj", &Camera::GetInstance()->GetProjMatrix());

	// 루트 뼈대 부터 메쉬 컨테이너가 있다면 렌더링
	RenderFrame(m_pRootFrame);
}

void ComRenderSkinnedMesh::AddAnimationSet(AnimationController & pAniControlDest, AnimationController & pAniControlSrc, int index)
{
}

void ComRenderSkinnedMesh::SeparateFrame(XFrame pFrame, CString szName, AnimationController & pAniControlDest, AnimationController & pAniControlSrc)
{
}

void ComRenderSkinnedMesh::PlayAnimation(int iIndex, bool isBlend)
{
	PlayAnimation(m_pAniControl, iIndex, isBlend);
}

void ComRenderSkinnedMesh::PlayAnimation(AnimationController pAniControl, int iIndex, bool isBlend)
{
	// 애니메이션 세트 가져오기
	AnimationSet pNextAnimSet = NULL;
	pAniControl->GetAnimationSet(iIndex, &pNextAnimSet);

	// 블랜드로 애니메이션을 전환
	if (isBlend)
	{
		AnimationSet pPrevAnimSet = NULL;
		pAniControl->GetTrackAnimationSet(0, &pPrevAnimSet);
		pAniControl->SetTrackAnimationSet(1, pPrevAnimSet);

		D3DXTRACK_DESC trackDesc;
		pAniControl->GetTrackDesc(0, &trackDesc);
		pAniControl->SetTrackDesc(1, &trackDesc);
		pAniControl->SetTrackWeight(0, 0.0f);
		pAniControl->SetTrackWeight(1, 1.0f);
		SAFE_RELEASE(pPrevAnimSet);
		m_fPassedBlendTime = 0.0f;
	}

	pAniControl->SetTrackAnimationSet(0, pNextAnimSet);
	pAniControl->SetTrackPosition(0, 0);
	SAFE_RELEASE(pNextAnimSet);
}

Matrix4x4& ComRenderSkinnedMesh::GetMatrixByName(LPCSTR szName)
{
	Frame* pFrame = (Frame*)D3DXFrameFind(m_pRootFrame, szName);
	if (pFrame != NULL)
		return (pFrame)->CombinedTM;

	Matrix4x4 matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	return matIdentity;
}

void ComRenderSkinnedMesh::SetupBoneMatrixPointers(XFrame pFrame)
{
	if (pFrame->pMeshContainer != NULL)
		SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);

	// 뼈대의 (Sibling : 형재, 자매, 동기)가 있다면 재귀 호출(이 함수를 다시 호출)하여 SetupBoneMatrixPointersOnMesh 실행
	if (pFrame->pFrameSibling != NULL)
		SetupBoneMatrixPointers(pFrame->pFrameSibling);

	// 뼈대의 (FirstChild : 첫번째 자식 노드)가 있다면 재귀 호출(이 함수를 다시 호출)하여 SetupBoneMatrixPointersOnMesh 실행
	if (pFrame->pFrameFirstChild != NULL)
		SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
}

void ComRenderSkinnedMesh::SetupBoneMatrixPointersOnMesh(XMeshContainer pMeshContainerBase)
{
	Frame* pFrameInfluence;
	MeshContainer* pMeshContainer = (MeshContainer*)pMeshContainerBase;
	
	// 매쉬 컨테이너가 있고 스키닝 정보가 있다면
	if (pMeshContainer != NULL && pMeshContainer->pSkinInfo != NULL)
	{
		int numBones = pMeshContainer->pSkinInfo->GetNumBones();

		// 뼈대 수 만큼
		for (int i = 0; i < numBones; ++i)
		{
			// 1. 영향을 주는 뼈대를 매쉬컨테이너의 스킨정보에서 루트 뼈대부터 이름으로 찾는다. (Influence : 영향, 영향력, 영향을 주다)
			pFrameInfluence = (Frame*)D3DXFrameFind(m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i));
			CString strDebug(pFrameInfluence->Name);
			strDebug.Append(L"\r\n");
			OutputDebugString(strDebug);
			
			// 2. 설정할 매쉬 컨테이너의 뼈대 행렬 포인터에 최종 행렬을 설정한다.
			pMeshContainer->ppBoneMatrixPtrs[i] = &pFrameInfluence->CombinedTM;
		}
	}
}

void ComRenderSkinnedMesh::UpdateAnimation(AnimationController pAniControl)
{
	float fDeltaTime = GetElapsedTime();

	pAniControl->AdvanceTime(fDeltaTime, NULL);

	// 애니메이션을 부드럽게 전환(Blend)
	if (m_fPassedBlendTime <= m_fBlendTime)
	{
		m_fPassedBlendTime += fDeltaTime;

		if (m_fPassedBlendTime < m_fBlendTime)
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			pAniControl->SetTrackWeight(0, fWeight);
			pAniControl->SetTrackWeight(1, 1.0f - fWeight);
		}
		else
		{
			pAniControl->SetTrackWeight(0, 1);
			pAniControl->SetTrackWeight(1, 0);
			pAniControl->SetTrackEnable(1, false);
		}
	}
}

void ComRenderSkinnedMesh::UpdateFrameMatrices(XFrame pFrame, XFrame pParent)
{
	Frame* frame = (Frame*)pFrame;

	// 부모가 있다면
	if (pParent != NULL)
		frame->CombinedTM = frame->TransformationMatrix * ((Frame*)pParent)->CombinedTM;
	else
		frame->CombinedTM = frame->TransformationMatrix;

	if (frame->pFrameSibling != NULL)
		UpdateFrameMatrices(frame->pFrameSibling, pParent);

	if (frame->pFrameFirstChild != NULL)
		UpdateFrameMatrices(frame->pFrameFirstChild, frame);
}

void ComRenderSkinnedMesh::RenderFrame(XFrame pFrame)
{
	XMeshContainer pMeshContainer = pFrame->pMeshContainer;

	while (pMeshContainer != NULL)
	{
		RenderMeshContainer((MeshContainer*)pMeshContainer);
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	
	if (pFrame->pFrameSibling != NULL)
		RenderFrame(pFrame->pFrameSibling);

	if (pFrame->pFrameFirstChild != NULL)
		RenderFrame(pFrame->pFrameFirstChild);
}

void ComRenderSkinnedMesh::RenderMeshContainer(MeshContainer* pMeshContainer)
{
	// 스키닝 정보가 없다면 렌더링 하지 않는다.
	if (pMeshContainer->pSkinInfo == NULL)
		return;

	LPD3DXBONECOMBINATION pBoneComb = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneCombinationBuf->GetBufferPointer();

	UINT iBoneID = 0;
	// 매쉬 컨테이너의 속성(attribute) 수만큼
	for (int iAttribute = 0; iAttribute < pMeshContainer->NumAttributeGroups; ++iAttribute)
	{
		/// I. 정점 셰이더를 위한
		// 1. 스킨드 매쉬 월드 행렬 준비 (first calculate all the world matrices)
		for (int iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
		{
			iBoneID = pBoneComb[iAttribute].BoneId[iPaletteEntry];
			if (iBoneID != UINT_MAX)
			{
				Matrix4x4 matTemp = pMeshContainer->pBoneOffsetMatrices[iBoneID] * *pMeshContainer->ppBoneMatrixPtrs[iBoneID] * gameObject->transform->GetWorldMatrix();
				m_pBoneMatrices[iPaletteEntry] = matTemp;
			}
		}

		// 2. 정점 셰이더를 위한 뼈대 수 설정 (Set CurNumBones to select the correct vertex shader for the number of bones)
		m_pEffect->SetInt("CurNumBones", pMeshContainer->NumInfl - 1);
	
		// 3. 정점 셰이더에 월드 행렬 설정
		m_pEffect->SetMatrixArray("mWorldMatrixArray", m_pBoneMatrices, pMeshContainer->NumPaletteEntries);

		/// II. 픽셀 셰이더를 재질, 텍스쳐 설정
		// 1. 물체의 빛에 의한 색상 설정
		MTLTEX* mtrl = pMeshContainer->vecMtlTex[pBoneComb[iAttribute].AttribId];

		// 2. 물체의 텍스쳐 설정
		m_pEffect->SetTexture("DiffuseMap_Tex", mtrl->pTexture);

		// 3. 셰이더 값 업데이트
		m_pEffect->CommitChanges();

		UINT numPasses;
		/// III. 셰이더 시작
		m_pEffect->Begin(&numPasses, NULL);
		m_pEffect->BeginPass(0);
		pMeshContainer->MeshData.pMesh->DrawSubset(iAttribute);
		m_pEffect->EndPass();
		m_pEffect->End();
	}
}