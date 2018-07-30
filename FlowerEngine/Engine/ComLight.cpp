#include "stdafx.h"
#include "ComLight.h"

int ComLight::m_iLightIndex = 0;

ComLight::ComLight(CString szName) : 
	Component(szName),
	m_iIndex(0)
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
	pDevice9 = GetD3D9Device();
}

ComLight::~ComLight()
{
	--m_iLightIndex;
}

void ComLight::InitDirectional(float fAmbient, float fDiffuse)
{
	// еб╬Г╨Ш
	Vector3 dir(1, -1, 1);
	D3DXVec3Normalize(&dir, &dir);
	m_Color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Light.Type = D3DLIGHT_DIRECTIONAL;
	m_Light.Ambient = m_Color * fAmbient;
	m_Light.Diffuse = m_Color * fDiffuse;
	m_Light.Specular = m_Color;
	m_Light.Direction = dir;

	InitLight();
}

void ComLight::InitPoint(float fPosX, float fPosY, float fPosZ, Color* c)
{
	m_Color = *c;
	m_Light.Type = D3DLIGHT_POINT;
	m_Light.Ambient = *c * 0.4f;
	m_Light.Diffuse = *c;
	m_Light.Specular = *c * 0.6f;
	m_Light.Position = Vector3(fPosX, fPosY, fPosZ);
	m_Light.Range = 20.0f;
	m_Light.Falloff = 1.0f;
	m_Light.Attenuation0 = 0.005f;
	m_Light.Attenuation1 = 0.005f;
	m_Light.Attenuation2 = 0.005f;

	InitLight();
}

void ComLight::UpdateLight(float fAmbient, float fDiffuse)
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));

	Vector3 dir(1, -1, 1);
	D3DXVec3Normalize(&dir, &dir);
	m_Color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	m_Light.Type = D3DLIGHT_DIRECTIONAL;
	m_Light.Ambient = m_Color * fAmbient;
	m_Light.Diffuse = m_Color * fDiffuse;
	m_Light.Specular = m_Color;
	m_Light.Direction = dir;
	pDevice9->SetLight(m_iIndex, &m_Light);
}

void ComLight::InitLight()
{
	pDevice9->SetLight(m_iLightIndex, &m_Light);
	pDevice9->LightEnable(m_iLightIndex, true);
	m_iIndex = m_iLightIndex;
	++m_iLightIndex;
}

void ComLight::Awake()
{
}

void ComLight::Update()
{
}

void ComLight::Render()
{
}
