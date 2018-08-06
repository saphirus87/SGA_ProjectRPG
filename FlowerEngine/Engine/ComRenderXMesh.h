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

	Matrix4x4 matWorld;

private:
	Device9 pDevice9;
	Mesh m_pMesh;
	vector<MTLTEX> m_vecMtrl;
	// ���� ��
	DWORD m_iNumMaterials;			
};

