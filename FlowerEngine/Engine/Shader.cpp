#include "stdafx.h"
#include "Shader.h"

Shaders* Shaders::m_pInstance = NULL;

Shaders * Shaders::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new Shaders();

	return m_pInstance;
}

void Shaders::Delete()
{
	// �Ҹ��ڰ� ȣ����� �����Ƿ�
	SAFE_DELETE(m_pInstance);
}

Shaders::Shaders()
{
	pDevice9 = GetD3D9Device();
}

Shaders::~Shaders()
{
	for (auto p : m_mapEffect)
		SAFE_RELEASE(p.second);
}

LPD3DXEFFECT Shaders::GetShader(CString szFilepath)
{
	if (m_mapEffect.find(szFilepath) == m_mapEffect.end())
	{
		m_mapEffect[szFilepath] = LoadShader(szFilepath);
	}
	return m_mapEffect[szFilepath];
}

LPD3DXEFFECT Shaders::LoadShader(CString szFilepath)
{
	LPD3DXEFFECT pEffect = NULL;
	LPD3DXBUFFER pError = NULL;

	// ������ �������� Ȯ�ΰ���
	D3DXCreateEffectFromFile(pDevice9, szFilepath, NULL, NULL, D3DXSHADER_DEBUG, NULL, &pEffect, &pError);

	if (!pEffect && pError)
	{
		int size = pError->GetBufferSize();
		LPVOID errorMsg = pError->GetBufferPointer();

		if (errorMsg)
		{
			char* str = new char[size];
			sprintf_s(str, size, (const char*)errorMsg, size);

//			OutputDebugStringA(str);
			MessageBoxA(NULL, str, 0, 0);
			delete[] str;
		}
	}

	return pEffect;
}
