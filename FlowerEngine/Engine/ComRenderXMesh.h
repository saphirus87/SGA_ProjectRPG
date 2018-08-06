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

	void Load(CString szFolderPath, CString szFileName);

	Matrix4x4 matFrame;
	Matrix4x4 matParent;

private:
	Device9 pDevice9;
	Shader m_pEffect;
	Mesh m_pMesh;
	vector<MTLTEX> m_vecMtrl;
	// ���� ��
	DWORD m_iNumMaterials;			
};

