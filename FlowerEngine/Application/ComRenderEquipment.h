#pragma once
#include "stdafx.h"

class ComRenderEquipment : public Component
{
public:
	ComRenderEquipment(CString szName);
	~ComRenderEquipment();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// �Ž��� �ε��մϴ�.
	void Load(CString szFolderPath, CString szFileName);

	// �Ž��� ����(Clone)�մϴ�.
	void Clone(ComRenderEquipment* pComRenderEquipment);
	
	// ���� ��İ� �θ������ �����Ͽ� ������ �մϴ�.
	// Render�Լ��� �Ἥ ������ �� ��� ���� ������ ������ �ʾ ĳ���� �̵��� ��� �з��� ������ �Ǵ� ������ �߻��ϹǷ� �� �Լ��� ����մϴ�.
	void Render(Matrix4x4* pMatFrame, Matrix4x4* pMatParent);

	// ����(Mirrored, Flip) ����
	bool IsMirrored;

	// ������ �ؽ��ĸ� �����մϴ�.
	void ChangeTexture(int iIndex, CString szTextureName);

	// ���� �ȵ��� ���� ���� �� ������ �ϱ� ���� ����
	bool IsEquiped;

	// �ٴڿ� ���������� GetHeight�� ����
	bool IsDropped;

	// ������ ����
	ItemInfo * pItemInfo;
	
private:
	void RenderShader();

private:
	Device9 pDevice9;
	Shader m_pEffect;
	Mesh m_pMesh;
	vector<MTLTEX> m_vecMtrl;
	
	// ���� ��
	DWORD m_iNumMaterials;			

	Matrix4x4 m_matFinal;

	// ��� ����
	Matrix4x4 m_matFrame;
	Matrix4x4 m_matParent;
};

