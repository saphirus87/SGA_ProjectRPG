#pragma once
#include "Component.h"
class ComRenderXMesh : public Component
{
public:
	ComRenderXMesh(CString szName);
	~ComRenderXMesh();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 매쉬를 로딩합니다.
	void Load(CString szFolderPath, CString szFileName);

	// 매쉬를 복제(Clone)합니다.
	void Clone(ComRenderXMesh* pComRenderXMesh);
	
	// 뼈대 행렬과 부모행렬을 설정합니다.
	void SetFrameMatrix(Matrix4x4* pMatFrame, Matrix4x4* pMatParent);
private:
	Device9 pDevice9;
	Shader m_pEffect;
	Mesh m_pMesh;
	vector<MTLTEX> m_vecMtrl;
	
	// 재질 수
	DWORD m_iNumMaterials;			

	// 행렬 정보
	Matrix4x4 m_matFrame;
	Matrix4x4 m_matParent;
};

