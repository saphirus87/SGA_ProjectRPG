#pragma once
#include "stdafx.h"

class ComMeshSphere : public Component
{
public:
	ComMeshSphere(CString szName);
	~ComMeshSphere();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	BoundingSphere m_boundingSphere;
	Material m_material; // ���� ��ȯ��

private:
	Device9 pDevice9;
	Mesh m_pMesh;

	Shader m_pEffect;
	Texture m_pTex;
};

