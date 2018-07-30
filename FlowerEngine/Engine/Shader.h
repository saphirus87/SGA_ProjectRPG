#pragma once
#include "stdafx.h"

class Shaders
{
public:
	static Shaders* GetInstance();
	static void Delete();
	Shader GetShader(CString filepath);

private:
	Shaders();
	~Shaders();

	Shader LoadShader(CString filepath);

private:
	Device9 pDevice9;
	static Shaders* m_pInstance;
	map<CString, LPD3DXEFFECT> m_mapEffect;
};

