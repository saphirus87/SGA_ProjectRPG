#include "stdafx.h"
#include "GlobalDefinition.h"

CString ASSET_PATH = _T("./Resources/");
CString SHADER_PATH = _T("./Resources/shader");


vector<Vector3> g_vecCubeVertex = {
	Vector3(-1.0f, -1.0f, -1.0f),
	Vector3(-1.0f,  1.0f, -1.0f),
	Vector3( 1.0f,  1.0f, -1.0f),
	Vector3( 1.0f, -1.0f, -1.0f),
	Vector3(-1.0f, -1.0f,  1.0f),
	Vector3(-1.0f,  1.0f,  1.0f),
	Vector3( 1.0f,  1.0f,  1.0f),
	Vector3( 1.0f, -1.0f,  1.0f)
};

vector<WORD> g_vecCubeIndex = {
	0, 1, 2, 0, 2, 3,	// 근
	7, 6, 5, 7, 5, 4,	// 원
	4, 5, 1, 4, 1, 0,	// 좌
	3, 2, 6, 3, 6, 7,	// 우
	1, 5, 6, 1, 6, 2,	// 상
	4, 0, 3, 4, 3, 7	// 하
};

vector<Vector3> g_vecQuadVertex = {
	Vector3(-1, -1, 0),
	Vector3(-1, 1, 0),
	Vector3(1, 1, 0),
	Vector3(1, -1, 0)
};

vector<WORD> g_vecQuadIndex = {
	0, 1, 2, 0, 2, 3
};

vector<Vector2> g_vecQuadUV = {
	Vector2(0, 1),
	Vector2(0, 0),
	Vector2(1, 0),
	Vector2(1, 1)
};

DWORD FtoDw(float f) { return *((DWORD*)&f); }

float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}

bool CompareStr(const char* str1, const char* str2)
{
	return strcmp(str1, str2) == 0;
}

float EaseTo(float* out, float start, float end, float speed)
{
	// 제한값 필요, 값 너무 클 때
	*out += (end - start) * speed;
	return *out;
}

Vector3 EaseTo(Vector3* out, Vector3& start, Vector3& end, float speed)
{
	// 제한값 필요
	*out += (end - start) * speed;
	return *out;
}

namespace DXUtil
{

D3DMATERIAL9 InitMtrl(Color a, Color d, Color s, Color e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;

	return mtrl;
}

void ComputeNormal(Vector3* out, Vector3* p0, Vector3* p1, Vector3* p2)
{
	Vector3 v01 = *p1 - *p0;
	Vector3 v02 = *p2 - *p0;

	D3DXVec3Cross(out, &v01, &v02);
	D3DXVec3Normalize(out, out);
}

D3DLIGHT9 InitDirectional(Vector3* dir, Color* c)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *c * 0.4f;
	light.Diffuse = *c;
	light.Specular = *c * 0.6f;
	light.Direction = *dir;

	return light;
}

D3DLIGHT9 InitPoint(Vector3* pos, Color* c)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *c * 0.4f;
	light.Diffuse = *c;
	light.Specular = *c * 0.6f;
	light.Position = *pos;
	light.Range = 20.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 0.005f;
	light.Attenuation1 = 0.005f;
	light.Attenuation2 = 0.005f;

	return light;
}

D3DLIGHT9 InitSpot(Vector3* dir, Vector3* pos, Color* c)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *c * 0.4f;
	light.Diffuse = *c;
	light.Specular = *c * 0.6f;
	light.Direction = *dir;
	light.Position = *pos;
	light.Range = 50.0f;
	light.Falloff = 1.0f;
	light.Theta = 0.0f;
	light.Phi = D3DX_PI / 6;
	light.Attenuation0 = 0.005f;
	light.Attenuation1 = 0.005f;
	light.Attenuation2 = 0.005f;

	return light;
}

void InitVertexBuffer(VertexBuffer & pVB, LPVOID vertices, DWORD fvf, UINT vtxCnt, UINT stride)
{
	GetD3D9Device()->CreateVertexBuffer(vtxCnt * stride, NULL, fvf, D3DPOOL_MANAGED, &pVB, NULL);

	LPVOID pVertex;
	pVB->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, vertices, vtxCnt * stride);
	pVB->Unlock();
}

void InitIndexBuffer(IndexBuffer & pIB, LPVOID indices, UINT idxCnt, UINT stride)
{
	D3DFORMAT format;
	if (stride == sizeof(WORD)) 
		format = D3DFMT_INDEX16;
	else if (stride == sizeof(DWORD))
		format = D3DFMT_INDEX32;

	GetD3D9Device()->CreateIndexBuffer(idxCnt * stride, NULL, format, D3DPOOL_MANAGED, &pIB, NULL);
	LPVOID pIndex;
	pIB->Lock(0, 0, (LPVOID*)&pIndex, 0);
	memcpy(pIndex, indices, idxCnt * stride);
	pIB->Unlock();
}

}