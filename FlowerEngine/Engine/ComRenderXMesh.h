#pragma once
#include "Component.h"
class ComRenderXMesh : public Component
{
public:
	ComRenderXMesh(CString szName);
	~ComRenderXMesh();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// �Ž��� �ε��մϴ�.
	void Load(CString szFolderPath, CString szFileName);

	// �Ž��� ����(Clone)�մϴ�.
	void Clone(ComRenderXMesh* pComRenderXMesh);
	
	// ���� ��İ� �θ������ �����մϴ�.
	void SetFrameMatrix(Matrix4x4* pMatFrame, Matrix4x4* pMatParent);
private:
	Device9 pDevice9;
	Shader m_pEffect;
	Mesh m_pMesh;
	vector<MTLTEX> m_vecMtrl;
	
	// ���� ��
	DWORD m_iNumMaterials;			

	// ��� ����
	Matrix4x4 m_matFrame;
	Matrix4x4 m_matParent;
};

