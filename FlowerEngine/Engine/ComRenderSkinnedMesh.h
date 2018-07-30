/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 서울게임 아카데미 김주안 선생님 수업시간에 배운 코드를 설계를 수정하여 적용한 코드 입니다.

설명 : 
1. 뼈대 계층구조(AllocateHierarchy)를 통하여 .x파일을 읽어옵니다.(Load)
2. Load시 기존에 있는 .x파일이면 복제(Clone)를 합니다.
3. DirectX의 AnimationController를 통하여 애니메이션을 제어합니다. 
   더 자세한 애니메이션은 DirectX ID3DXAnimationController 인터페이스를 참고하세요.

   정점 계산과 렌더링
4. 정점들은 skinnedmesh.fx 셰이더 파일을 통하여 WorldViewProj 행렬이 계산 됩니다.
5. 픽셸들은 skinnedmesh.fx 셰이더 파일을 통하여 계산됩니다. 해당 파일을 참고하세요.
*/
#pragma once
#include "stdafx.h"

class ComRenderSkinnedMesh : public Component
{
public:
	ComRenderSkinnedMesh(CString szName);
	~ComRenderSkinnedMesh();

	// 애니메이션이 포함된 x파일을 읽어옵니다.
	void Load(CString szPath, CString szFileName);

	// 이 게임 오브젝트를 복제합니다.
	void Clone(ComRenderSkinnedMesh* pExist);

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
	
	// 설명 작성
	void AddAnimationSet(AnimationController& pAniControlDest, AnimationController& pAniControlSrc, int index);
	
	// 애니메이션을 분리합니다.
	void SeparateFrame(XFrame pFrame, // 뼈대
		CString szName,	// 이름
		AnimationController& pAniControlDest, // 목표
		AnimationController& pAniControlSrc); // 원본

	// 애니메이션을 실행합니다.
	void PlayAnimation(AnimationController pAniControl, int iIndex, bool isBlend);

private:
	// 루트 뼈대 부터 행렬 포인터들 지정 (재귀호출)
	void SetupBoneMatrixPointers(XFrame pFrame);
	
	// 매쉬 컨테이너로부터 최종 뼈대 행렬을 설정합니다.
	void SetupBoneMatrixPointersOnMesh(XMeshContainer pMeshContainerBase);
	
	// 애니메이션을 업데이트 합니다.
	void UpdateAnimation(AnimationController pAniControl);
	
	// 뼈대의 최종 행렬 계산
	void UpdateFrameMatrices(XFrame pFrame, XFrame pParent);

	// 해당 프레임부터 계층구조를 탐색하며 FrameSibling, FrameFirstChild를 순회하며 렌더링 합니다.
	void RenderFrame(XFrame pFrame);

	// 매쉬 컨테이너를 렌더링 합니다.
	void RenderMeshContainer(MeshContainer* pMeshContainer);

private:
	// DirectX 디바이스
	Device9 pDevice9;
	
	// 셰이더
	Shader m_pEffect;

	// 뼈대 계층구조 할당
	AllocateHierarchy* m_pAllocateHierarchy;

	// 루트 뼈대
	XFrame m_pRootFrame;
	// 분리된 뼈대(Separate)를 만들기 위한 보조 루트 뼈대 - 뼈대를 상/하단으로 나눔
	XFrame m_pSubRootFrame;
	XFrame m_pConnetFrame;

	// 기본 애니메이션 제어
	AnimationController m_pAniControl;
	// 보조 애니메이션 제어 - 위는 총쏘기 아래는 달리기 애니메이션을 적용하기 위한
	AnimationController m_pSubAniControl;

	// 애니메이션 식별 번호(Index)
	int m_iAniIndex;

	// 애니메이션을 부드럽게 전환하는(Blend) 시간
	float m_fBlendTime;
	
	// 애니메이션을 부드럽게 전환하는 지나간 시간
	float m_fPassedBlendTime;

	// 뼈대 행렬들
	Matrix4x4* m_pBoneMatrices;

	// 메모리를 한 번만 Clear하면 되기 때문에 사용하는 변수
	static bool IsCleared;
};

