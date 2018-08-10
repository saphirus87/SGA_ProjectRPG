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
	// ���̴� �ε�
	m_pEffect = Shaders::GetInstance()->GetShader(L"Resources/shader/skinnedmesh.fx");

	// 1. ���� ���� ���� ����
	m_pAllocateHierarchy = new AllocateHierarchy(szFolderPath);

	// 2. x���Ϸ� ���� �ִϸ��̼� �Ž� (�ַ� ĳ����)�� �о����
	D3DXLoadMeshHierarchyFromX(szFolderPath + szFileName, D3DXMESH_MANAGED, pDevice9, m_pAllocateHierarchy, NULL, &m_pRootFrame, &m_pAniControl);
	
	int numTrack = m_pAniControl->GetMaxNumTracks();
	for (int iTrack = 0; iTrack < numTrack; ++iTrack)
		m_pAniControl->SetTrackSpeed(iTrack, 0.5f);

	// 3. ��Ʈ ���� ���� ��� �����͵� ����
	SetupBoneMatrixPointers(m_pRootFrame);

	// 4. ���� ��ĵ� �޸� �Ҵ�
	m_pBoneMatrices = new Matrix4x4[m_pAllocateHierarchy->GetNumBoneMatricesMax()];
}

void ComRenderSkinnedMesh::Clone(ComRenderSkinnedMesh* pExist)
{
	// �����ϴ� ���ӿ�����Ʈ�� ���� ��Ų�Ž� �������(pExist) �ɹ� ������ �� ���� ��Ų�Ž� ������ҷ� ����
	m_pAllocateHierarchy = pExist->m_pAllocateHierarchy;
	m_pRootFrame = pExist->m_pRootFrame;
	m_pBoneMatrices = pExist->m_pBoneMatrices;
	m_mapBoneMatrices = pExist->m_mapBoneMatrices;

	m_pEffect = pExist->m_pEffect;

	// �����ϴ� �ִϸ��̼� ��Ʈ�ѷ�(pExist->m_pAniControl)�� �� ������Ʈ�� �ִϸ��̼� ��Ʈ�ѷ�(m_pAniControl)�� ����
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
		// �ִϸ��̼��� �ִٸ� �⺻ �ִϸ��̼� ���
		PlayAnimation(m_pAniControl, 0, false);
		// �ִϸ��̼� Ʈ�� �ʱ�ȭ
		m_pAniControl->SetTrackPosition(0, 0);
	}
}

void ComRenderSkinnedMesh::Update()
{
	// �ִϸ��̼� ������Ʈ�� ��ȯ�� �ε巴���ϱ� ���� ����(Blending)
	UpdateAnimation(m_pAniControl);

	// ������ ������ ������ ���� ������ ���� ���� ���(Combined) ���
	UpdateFrameMatrices(m_pRootFrame, NULL);
}

void ComRenderSkinnedMesh::Render()
{
	// �� ���� ������Ʈ�� ���� �������� ������ ���� ���� ���̴��� �� ���� ������Ʈ�� ���� ��� ����
	m_pEffect->SetMatrix("mView", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("mProj", &Camera::GetInstance()->GetProjMatrix());

	// ��Ʈ ���� ���� �޽� �����̳ʰ� �ִٸ� ������
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
	// �ִϸ��̼� ��Ʈ ��������
	AnimationSet pNextAnimSet = NULL;
	pAniControl->GetAnimationSet(iIndex, &pNextAnimSet);

	// ����� �ִϸ��̼��� ��ȯ
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

	// ������ (Sibling : ����, �ڸ�, ����)�� �ִٸ� ��� ȣ��(�� �Լ��� �ٽ� ȣ��)�Ͽ� SetupBoneMatrixPointersOnMesh ����
	if (pFrame->pFrameSibling != NULL)
		SetupBoneMatrixPointers(pFrame->pFrameSibling);

	// ������ (FirstChild : ù��° �ڽ� ���)�� �ִٸ� ��� ȣ��(�� �Լ��� �ٽ� ȣ��)�Ͽ� SetupBoneMatrixPointersOnMesh ����
	if (pFrame->pFrameFirstChild != NULL)
		SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
}

void ComRenderSkinnedMesh::SetupBoneMatrixPointersOnMesh(XMeshContainer pMeshContainerBase)
{
	Frame* pFrameInfluence;
	MeshContainer* pMeshContainer = (MeshContainer*)pMeshContainerBase;
	
	// �Ž� �����̳ʰ� �ְ� ��Ű�� ������ �ִٸ�
	if (pMeshContainer != NULL && pMeshContainer->pSkinInfo != NULL)
	{
		int numBones = pMeshContainer->pSkinInfo->GetNumBones();

		// ���� �� ��ŭ
		for (int i = 0; i < numBones; ++i)
		{
			// 1. ������ �ִ� ���븦 �Ž������̳��� ��Ų�������� ��Ʈ ������� �̸����� ã�´�. (Influence : ����, �����, ������ �ִ�)
			pFrameInfluence = (Frame*)D3DXFrameFind(m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i));
			CString strDebug(pFrameInfluence->Name);
			strDebug.Append(L"\r\n");
			OutputDebugString(strDebug);
			
			// 2. ������ �Ž� �����̳��� ���� ��� �����Ϳ� ���� ����� �����Ѵ�.
			pMeshContainer->ppBoneMatrixPtrs[i] = &pFrameInfluence->CombinedTM;
		}
	}
}

void ComRenderSkinnedMesh::UpdateAnimation(AnimationController pAniControl)
{
	float fDeltaTime = GetElapsedTime();

	pAniControl->AdvanceTime(fDeltaTime, NULL);

	// �ִϸ��̼��� �ε巴�� ��ȯ(Blend)
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

	// �θ� �ִٸ�
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
	// ��Ű�� ������ ���ٸ� ������ ���� �ʴ´�.
	if (pMeshContainer->pSkinInfo == NULL)
		return;

	LPD3DXBONECOMBINATION pBoneComb = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneCombinationBuf->GetBufferPointer();

	UINT iBoneID = 0;
	// �Ž� �����̳��� �Ӽ�(attribute) ����ŭ
	for (int iAttribute = 0; iAttribute < pMeshContainer->NumAttributeGroups; ++iAttribute)
	{
		/// I. ���� ���̴��� ����
		// 1. ��Ų�� �Ž� ���� ��� �غ� (first calculate all the world matrices)
		for (int iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
		{
			iBoneID = pBoneComb[iAttribute].BoneId[iPaletteEntry];
			if (iBoneID != UINT_MAX)
			{
				Matrix4x4 matTemp = pMeshContainer->pBoneOffsetMatrices[iBoneID] * *pMeshContainer->ppBoneMatrixPtrs[iBoneID] * gameObject->transform->GetWorldMatrix();
				m_pBoneMatrices[iPaletteEntry] = matTemp;
			}
		}

		// 2. ���� ���̴��� ���� ���� �� ���� (Set CurNumBones to select the correct vertex shader for the number of bones)
		m_pEffect->SetInt("CurNumBones", pMeshContainer->NumInfl - 1);
	
		// 3. ���� ���̴��� ���� ��� ����
		m_pEffect->SetMatrixArray("mWorldMatrixArray", m_pBoneMatrices, pMeshContainer->NumPaletteEntries);

		/// II. �ȼ� ���̴��� ����, �ؽ��� ����
		// 1. ��ü�� ���� ���� ���� ����
		MTLTEX* mtrl = pMeshContainer->vecMtlTex[pBoneComb[iAttribute].AttribId];

		// 2. ��ü�� �ؽ��� ����
		m_pEffect->SetTexture("DiffuseMap_Tex", mtrl->pTexture);

		// 3. ���̴� �� ������Ʈ
		m_pEffect->CommitChanges();

		UINT numPasses;
		/// III. ���̴� ����
		m_pEffect->Begin(&numPasses, NULL);
		m_pEffect->BeginPass(0);
		pMeshContainer->MeshData.pMesh->DrawSubset(iAttribute);
		m_pEffect->EndPass();
		m_pEffect->End();
	}
}