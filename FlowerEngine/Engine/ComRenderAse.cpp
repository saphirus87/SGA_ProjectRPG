#include "stdafx.h"
#include "ComRenderAse.h"


ComRenderAse::ComRenderAse(CString szName) :
	Component(szName),
	IsRenderBumpmap(true), // shader로 렌더링 해줘야 postprocessing edge 검출됨
	m_pTexSpecular(NULL),
	m_pTexBump(NULL)
{
	pDevice9 = GetD3D9Device();
}

ComRenderAse::~ComRenderAse()
{
	SAFE_RELEASE(m_pVertexDecl);
	SAFE_RELEASE(m_pVB);
}

void ComRenderAse::Awake()
{
	DXUtil::InitVertexBuffer(m_pVB, &m_vertices[0], VERTEX_PNT::FVF, m_vertices.size(), sizeof(VERTEX_PNT));

	m_pEffect = Shaders::GetInstance()->GetShader("Resources/shader/BumpSpecMappingSGA.fx");
	
	// 하나의 정점이 가질 수 있는 정보 갯수의 최대치
	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	// 정점 선언 정보
	pDevice9->CreateVertexDeclaration(vertexDecl, &m_pVertexDecl);
}

void ComRenderAse::Update()
{
}

void ComRenderAse::Render()
{
	// 일반 렌더링 (셰이더 X)
	if (IsRenderBumpmap == false)
	{
		if (m_mtlTex.pTexture)
		{
			pDevice9->SetTransform(D3DTS_WORLD, &gameObject->transform->GetWorldMatrix());
			pDevice9->SetTexture(0, m_mtlTex.pTexture);
			pDevice9->SetMaterial(&m_mtlTex.material);
			pDevice9->SetFVF(VERTEX_PNT::FVF);
			pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PNT));
			pDevice9->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vertices.size() / 3);
			pDevice9->SetTexture(0, NULL);
		}
	}
	
	//else if (m_pTexSpecular)
	//{
	//	m_pEffect = Shaders::GetInstance()->GetShader("Resources/shader/BumpSpecMappingSGA.fx");

	//	m_pEffect->SetMatrix("gWorldMatrix", &gameObject->transform->GetWorldMatrix());

	//	Matrix4x4 matWVP = gameObject->transform->GetWorldMatrix() * Camera::GetInstance()->GetVPmatrix();
	//	m_pEffect->SetMatrix("gWVPMatrix", &matWVP);

	//	Vector4 lightPos = Vector4(-500, 500, -500, 1);
	//	Vector3 camPos = Camera::GetInstance()->GetPosition();
	//	Vector4 camPos4 = Vector4(camPos.x, camPos.y, camPos.z, 1);
	//	Vector4 lightColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	m_pEffect->SetVector("gWorldLightPosition", &lightPos);
	//	m_pEffect->SetVector("gWorldCameraPosition", &camPos4);
	//	m_pEffect->SetVector("gLightColor", &lightColor);

	//	pDevice9->SetVertexDeclaration(m_pVertexDecl);

	//	// 그림자 같은거 할 때도 pass를 추가해주어야 함
	//	UINT numPasses = 0;
	//	m_pEffect->Begin(&numPasses, NULL);
	//	m_pEffect->SetTexture("DiffuseMap_Tex", m_mtlTex.pTexture);
	//	if (m_pTexBump)
	//		m_pEffect->SetTexture("NormalMap_Tex", m_pTexBump);
	//	if (m_pTexSpecular)
	//		m_pEffect->SetTexture("SpecularMap_Tex", m_pTexSpecular);
	//	for (int i = 0; i < numPasses; ++i)
	//	{
	//		m_pEffect->BeginPass(i);

	//		pDevice9->SetMaterial(&m_mtlTex.material);
	//		pDevice9->SetFVF(VERTEX_PNT::FVF);
	//		pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PNT));
	//		pDevice9->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vertices.size() / 3);

	//		m_pEffect->EndPass();
	//	}

	//	m_pEffect->End();
	//}
	else if (m_mtlTex.pTexture)
	{
		m_pEffect = Shaders::GetInstance()->GetShader("Resources/shader/BumpMappingSGA.fx");

		m_pEffect->SetMatrix("gWorldMatrix", &gameObject->transform->GetWorldMatrix());
		m_pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());

		Matrix4x4 matWVP = gameObject->transform->GetWorldMatrix() * Camera::GetInstance()->GetVPmatrix();
		m_pEffect->SetMatrix("gWVPMatrix", &matWVP);

		D3DXVECTOR4 lightPos = D3DXVECTOR4(-500, 500, -500, 1);
		D3DXVECTOR3 camPos = Camera::GetInstance()->GetPosition();
		D3DXVECTOR4 camPos4 = D3DXVECTOR4(camPos.x, camPos.y, camPos.z, 1);
		Vector4 lightColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pEffect->SetVector("gWorldLightPosition", &lightPos);
		m_pEffect->SetVector("gWorldCameraPosition", &camPos4);
		m_pEffect->SetVector("gLightColor", &lightColor);

		pDevice9->SetVertexDeclaration(m_pVertexDecl);

		// 그림자 같은거 할 때도 pass를 추가해주어야 함
		UINT numPasses = 0;
		m_pEffect->Begin(&numPasses, NULL);
		m_pEffect->SetTexture("DiffuseMap_Tex", m_mtlTex.pTexture);
		if (m_pTexBump)
			m_pEffect->SetTexture("NormalMap_Tex", m_pTexBump);
		for (int i = 0; i < numPasses; ++i)
		{
			m_pEffect->BeginPass(i);

			pDevice9->SetMaterial(&m_mtlTex.material);
			pDevice9->SetFVF(VERTEX_PNT::FVF);
			pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PNT));
			pDevice9->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vertices.size() / 3);

			m_pEffect->EndPass();
		}

		m_pEffect->End();
	}
}

void ComRenderAse::AddPosSample(ASE_POS_SAMPLE * pAsePosSample)
{
	m_posTrack.push_back(*pAsePosSample);
}

void ComRenderAse::AddRotSample(ASE_ROT_SAMPLE * pAseRotSample)
{
	m_rotTrack.push_back(*pAseRotSample);
}

void ComRenderAse::UpdateAniMatrices(int tickCount, Matrix4x4* pMatParent)
{
	Matrix4x4 matWorld, matLocal, matT, matR;
	
	CalcLocalTranslationMatrix(tickCount, matT);
	CalcLocalRotationMatrix(tickCount, matR);

	matLocal = matR * matT;

	if (pMatParent)
	{
		matWorld = matLocal * (*pMatParent);
		gameObject->transform->SetWorldMatrix(&matWorld);
	}
	else
	{
		matWorld = matLocal;
		gameObject->transform->SetWorldMatrix(&matWorld);
	}
}

void ComRenderAse::CalcLocalTranslationMatrix(int tickCount, Matrix4x4 & mat)
{
	D3DXMatrixIdentity(&mat);
	if (m_posTrack.empty())
	{
		Matrix4x4& matLocal = gameObject->transform->GetWorldMatrix();
		mat._41 = matLocal._41;
		mat._42 = matLocal._42;
		mat._43 = matLocal._43;
	}
	// 트랙의 첫번째와 마지막 행렬을 구해서 정점들을 선형보간 해준다.
	else if (tickCount <= m_posTrack.front().tick)
	{
		mat._41 = m_posTrack.front().position.x;
		mat._42 = m_posTrack.front().position.y;
		mat._43 = m_posTrack.front().position.z;
	}
	else if (tickCount >= m_posTrack.back().tick)
	{
		mat._41 = m_posTrack.back().position.x;
		mat._42 = m_posTrack.back().position.y;
		mat._43 = m_posTrack.back().position.z;
	}
	else
	{
		int next = -1;
		for (size_t i = 0; i < m_posTrack.size(); ++i)
		{
			if (m_posTrack[i].tick > tickCount)
			{
				// 다음 tick에 대한 Index 구하기
				next = i;
				break;
			}
		}

		int prev = next - 1;

		// 진행된 값 / 전체 
		float t = (tickCount - m_posTrack[prev].tick) / (float)(m_posTrack[next].tick - m_posTrack[prev].tick);

		D3DXVECTOR3 v;
		D3DXVec3Lerp(&v, &m_posTrack[prev].position, &m_posTrack[next].position, t);

		mat._41 = v.x;
		mat._42 = v.y;
		mat._43 = v.z;
	}
}

void ComRenderAse::CalcLocalRotationMatrix(int tickCount, Matrix4x4 & mat)
{
	D3DXMatrixIdentity(&mat);

	if (m_rotTrack.empty())
	{
		mat = gameObject->transform->GetWorldMatrix();
		// 회전값이기 때문에
		mat._41 = 0;
		mat._42 = 0;
		mat._43 = 0;
	}
	else if (tickCount <= m_rotTrack.front().tick)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_rotTrack.front().quaternion);
	}
	else if (tickCount >= m_rotTrack.back().tick)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_rotTrack.back().quaternion);
	}
	else
	{
		int next = -1;
		for (size_t i = 0; i < m_rotTrack.size(); ++i)
		{
			if (m_rotTrack[i].tick > tickCount)
			{
				// 다음 tick에 대한 Index 구하기
				next = i;
				break;
			}
		}

		int prev = next - 1;

		// 진행된 값 / 전체 
		float t = (tickCount - m_rotTrack[prev].tick) / (float)(m_rotTrack[next].tick - m_rotTrack[prev].tick);

		D3DXQUATERNION q;
		// 구형 보간
		D3DXQuaternionSlerp(&q, &m_rotTrack[prev].quaternion, &m_rotTrack[next].quaternion, t);
		D3DXMatrixRotationQuaternion(&mat, &q);
	}
}
