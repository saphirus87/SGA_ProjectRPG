#include "stdafx.h"
#include "ComObjMap.h"
#include "ObjLoader.h"

const int TOKEN_SIZE = 128;

ComObjMap::ComObjMap(CString szName) :
	Component(szName),
	m_rayDistance(2.f)
{
	pDevice9 = GetD3D9Device();
	m_pEffect = Shaders::GetInstance()->GetShader(SHADER_PATH + "/ShaderLightingTexture.fx");
}

ComObjMap::~ComObjMap()
{
	SAFE_RELEASE(m_pMesh);
}

void ComObjMap::Awake()
{
	ObjLoader loader;

	if (m_mapFilename.IsEmpty() == false)
	{
		m_pMesh = loader.LoadMesh(m_filePath, m_mapFilename, NULL, m_mtltexList, 5.0f);
	}

	if (m_surfaceFilename.IsEmpty() == false)
	{
		loader.LoadSurface(m_surfaceFilename, NULL, m_surfaceVertices);
	}
}

void ComObjMap::Update()
{
}

void ComObjMap::Render()
{
	m_pEffect->SetMatrix("gWorldMatrix", &gameObject->transform->GetWorldMatrix());
	m_pEffect->SetMatrix("gViewMatrix", &Camera::GetInstance()->GetViewMatrix());
	m_pEffect->SetMatrix("gProjMatrix", &Camera::GetInstance()->GetProjMatrix());

	m_pEffect->Begin(0, 0);
	m_pEffect->BeginPass(0);

	for (int i = 0; i < m_mtltexList.size(); i++)
	{
		m_pEffect->SetTexture("DiffuseMap_Tex", m_mtltexList[i]->pTexture);
		m_pMesh->DrawSubset(i);
	}

	m_pEffect->EndPass();
	m_pEffect->End();
}

bool ComObjMap::GetHeight(float & height, const D3DXVECTOR3 & pos)
{
	//m_surfaceVertices

	D3DXVECTOR3 rayPos(pos.x, pos.y + m_rayDistance, pos.z);
	D3DXVECTOR3 rayDir(0, -1, 0);
	float distance = 0.0f;

	for (int i = 0; i < m_surfaceVertices.size(); i += 12)
	{
		D3DXVECTOR3 v1 = m_surfaceVertices[i + 10];
		D3DXVECTOR3 v2 = m_surfaceVertices[i + 4];
		D3DXVECTOR3 v3 = m_surfaceVertices[i + 11];

		D3DXVECTOR3 v4 = m_surfaceVertices[i + 1];
		D3DXVECTOR3 v5 = m_surfaceVertices[i + 2];
		D3DXVECTOR3 v6 = m_surfaceVertices[i + 4];

		if (D3DXIntersectTri(&v1, &v2, &v3, &rayPos, &rayDir, NULL, NULL, &distance))
		{
			height = rayPos.y - distance;
			return true;
		}

		if (D3DXIntersectTri(&v4, &v5, &v6, &rayPos, &rayDir, NULL, NULL, &distance))
		{
			height = rayPos.y - distance;
			return true;
		}
	}

	return false;
}

bool ComObjMap::CalcPickedPosition(D3DXVECTOR3 & out, WORD screenX, WORD screenY)
{
	Ray ray = Ray::RayAtWorldSpace(screenX, screenY);
	float minDist = FLT_MAX;
	float dist = 0;
	bool bIntersect = false;

	for (size_t i = 0; i < m_surfaceVertices.size(); i += 3)
	{
		if (ray.CalcIntersectTri(&m_surfaceVertices[i], &dist))
		{
			if (dist < minDist)
			{
				bIntersect = true;
				minDist = dist;
				out = ray.m_pos + dist * ray.m_dir;
			}
		}
	}

	return bIntersect;
}