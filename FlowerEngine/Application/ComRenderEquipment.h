#pragma once
#include "stdafx.h"

class ComRenderEquipment : public Component
{
public:
	ComRenderEquipment(CString szName);
	~ComRenderEquipment();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	// 매쉬를 로딩합니다.
	void Load(CString szFolderPath, CString szFileName);

	// 매쉬를 복제(Clone)합니다.
	void Clone(ComRenderEquipment* pComRenderEquipment);
	
	// 뼈대 행렬과 부모행렬을 설정하여 렌더링 합니다.
	// Render함수를 써서 렌더링 할 경우 뼈대 프레임 설정이 늦어서 캐릭터 이동시 장비가 밀려서 렌더링 되는 현상이 발생하므로 이 함수를 사용합니다.
	void Render(Matrix4x4* pMatFrame, Matrix4x4* pMatParent);

	// 반전(Mirrored, Flip) 여부
	bool IsMirrored;

	// 렌더링 텍스쳐를 변경합니다.
	void ChangeTexture(int iIndex, CString szTextureName);

	// 장착 안됐을 때는 지형 위 렌더링 하기 위한 변수
	bool IsEquiped;

	// 바닥에 떨어졌으면 GetHeight를 안함
	bool IsDropped;

	// 아이템 정보
	ItemInfo * pItemInfo;
	
private:
	void RenderShader();

private:
	Device9 pDevice9;
	Shader m_pEffect;
	Mesh m_pMesh;
	vector<MTLTEX> m_vecMtrl;
	
	// 재질 수
	DWORD m_iNumMaterials;			

	Matrix4x4 m_matFinal;

	// 행렬 정보
	Matrix4x4 m_matFrame;
	Matrix4x4 m_matParent;
};

