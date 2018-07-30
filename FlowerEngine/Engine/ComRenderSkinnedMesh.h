/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ������� ��ī���� ���־� ������ �����ð��� ��� �ڵ带 ���踦 �����Ͽ� ������ �ڵ� �Դϴ�.

���� : 
1. ���� ��������(AllocateHierarchy)�� ���Ͽ� .x������ �о�ɴϴ�.(Load)
2. Load�� ������ �ִ� .x�����̸� ����(Clone)�� �մϴ�.
3. DirectX�� AnimationController�� ���Ͽ� �ִϸ��̼��� �����մϴ�. 
   �� �ڼ��� �ִϸ��̼��� DirectX ID3DXAnimationController �������̽��� �����ϼ���.

   ���� ���� ������
4. �������� skinnedmesh.fx ���̴� ������ ���Ͽ� WorldViewProj ����� ��� �˴ϴ�.
5. �ȼе��� skinnedmesh.fx ���̴� ������ ���Ͽ� ���˴ϴ�. �ش� ������ �����ϼ���.
*/
#pragma once
#include "stdafx.h"

class ComRenderSkinnedMesh : public Component
{
public:
	ComRenderSkinnedMesh(CString szName);
	~ComRenderSkinnedMesh();

	// �ִϸ��̼��� ���Ե� x������ �о�ɴϴ�.
	void Load(CString szPath, CString szFileName);

	// �� ���� ������Ʈ�� �����մϴ�.
	void Clone(ComRenderSkinnedMesh* pExist);

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
	
	// ���� �ۼ�
	void AddAnimationSet(AnimationController& pAniControlDest, AnimationController& pAniControlSrc, int index);
	
	// �ִϸ��̼��� �и��մϴ�.
	void SeparateFrame(XFrame pFrame, // ����
		CString szName,	// �̸�
		AnimationController& pAniControlDest, // ��ǥ
		AnimationController& pAniControlSrc); // ����

	// �ִϸ��̼��� �����մϴ�.
	void PlayAnimation(AnimationController pAniControl, int iIndex, bool isBlend);

private:
	// ��Ʈ ���� ���� ��� �����͵� ���� (���ȣ��)
	void SetupBoneMatrixPointers(XFrame pFrame);
	
	// �Ž� �����̳ʷκ��� ���� ���� ����� �����մϴ�.
	void SetupBoneMatrixPointersOnMesh(XMeshContainer pMeshContainerBase);
	
	// �ִϸ��̼��� ������Ʈ �մϴ�.
	void UpdateAnimation(AnimationController pAniControl);
	
	// ������ ���� ��� ���
	void UpdateFrameMatrices(XFrame pFrame, XFrame pParent);

	// �ش� �����Ӻ��� ���������� Ž���ϸ� FrameSibling, FrameFirstChild�� ��ȸ�ϸ� ������ �մϴ�.
	void RenderFrame(XFrame pFrame);

	// �Ž� �����̳ʸ� ������ �մϴ�.
	void RenderMeshContainer(MeshContainer* pMeshContainer);

private:
	// DirectX ����̽�
	Device9 pDevice9;
	
	// ���̴�
	Shader m_pEffect;

	// ���� �������� �Ҵ�
	AllocateHierarchy* m_pAllocateHierarchy;

	// ��Ʈ ����
	XFrame m_pRootFrame;
	// �и��� ����(Separate)�� ����� ���� ���� ��Ʈ ���� - ���븦 ��/�ϴ����� ����
	XFrame m_pSubRootFrame;
	XFrame m_pConnetFrame;

	// �⺻ �ִϸ��̼� ����
	AnimationController m_pAniControl;
	// ���� �ִϸ��̼� ���� - ���� �ѽ�� �Ʒ��� �޸��� �ִϸ��̼��� �����ϱ� ����
	AnimationController m_pSubAniControl;

	// �ִϸ��̼� �ĺ� ��ȣ(Index)
	int m_iAniIndex;

	// �ִϸ��̼��� �ε巴�� ��ȯ�ϴ�(Blend) �ð�
	float m_fBlendTime;
	
	// �ִϸ��̼��� �ε巴�� ��ȯ�ϴ� ������ �ð�
	float m_fPassedBlendTime;

	// ���� ��ĵ�
	Matrix4x4* m_pBoneMatrices;

	// �޸𸮸� �� ���� Clear�ϸ� �Ǳ� ������ ����ϴ� ����
	static bool IsCleared;
};

