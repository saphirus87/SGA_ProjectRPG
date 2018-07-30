#pragma once
#include "stdafx.h"

class ComRenderAse : public Component
{
public:
	ComRenderAse(CString szName);
	virtual ~ComRenderAse();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	inline void SetMtlTex(MTLTEX* pMtlTex) { m_mtlTex = *pMtlTex; }
		
	vector<VERTEX_PNT> m_vertices;
	bool IsRenderBumpmap;

	MTLTEX m_mtlTex;
	Texture m_pTexSpecular;
	Texture m_pTexBump;

	// Animation
	void AddPosSample(ASE_POS_SAMPLE* pAsePosSample);
	void AddRotSample(ASE_ROT_SAMPLE* pAseRotSample);
	inline vector<ASE_POS_SAMPLE>* GetPosTrack() { return &m_posTrack; }
	inline vector<ASE_ROT_SAMPLE>* GetRotTrack() { return &m_rotTrack; }

	void UpdateAniMatrices(int tickCount, Matrix4x4* pMatParent);
	void CalcLocalTranslationMatrix(int tickCount, Matrix4x4& mat);
	void CalcLocalRotationMatrix(int tickCount, Matrix4x4& mat);

protected:
	Device9 pDevice9;
	VertexDecl m_pVertexDecl;

	// 왜 Index Buffer 없지?ㅋ
	// Index Buffer 사용이유 알기와 만들기
	VertexBuffer m_pVB;

	// Bumpmap Shader
	Shader m_pEffect;

	vector<ASE_POS_SAMPLE> m_posTrack;
	vector<ASE_ROT_SAMPLE> m_rotTrack;
};

