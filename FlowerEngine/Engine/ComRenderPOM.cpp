#include "stdafx.h"
#include "ComRenderPOM.h"

//--------------------------------------------------------------------------------------
// Material properties parameters:
//--------------------------------------------------------------------------------------
D3DXCOLOR                   g_colorMtrlDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
D3DXCOLOR                   g_colorMtrlAmbient(0.35f, 0.35f, 0.35f, 0);
D3DXCOLOR                   g_colorMtrlSpecular(1.0f, 1.0f, 1.0f, 1.0f);


ComRenderPOM::ComRenderPOM(CString szName) : 
	ComRenderAse(szName),
	g_nCurrentTextureID(POM_STONES),
	g_nCurrentTechniqueID(POM),
	g_fHeightScale(0.02f),
	g_fLightScale(1.0f),
	g_pBaseTextures(NULL),
	g_pNMHTextures(NULL),
	g_fSpecularExponent(60.0f),
	g_fBaseTextureRepeat(1.0f),
	g_bVisualizeLOD(false),
	g_bVisualizeMipLevel(false),
	g_bDisplayShadows(true),
	g_bAddSpecular(true),
	g_bRenderPOM(true),
	g_nLODThreshold(3),
	g_nMinSamples(8),
	g_nMaxSamples(50),
	g_fShadowSoftening(0.58f)
{

	         
}


ComRenderPOM::~ComRenderPOM()
{
	SAFE_RELEASE(g_pMesh);
}

void ComRenderPOM::Awake()
{
	DXUtil::InitVertexBuffer(m_pVB, &m_vertices[0], VERTEX_PNT::FVF, m_vertices.size(), sizeof(VERTEX_PNT));

	m_pEffect = Shaders::GetInstance()->GetShader("Resources/shader/ParallaxOcclusionMapping.fx");

	// 하나의 정점이 가질 수 있는 정보 갯수의 최대치
	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	// 정점 선언 정보
	pDevice9->CreateVertexDeclaration(vertexDecl, &m_pVertexDecl);

	LoadMesh(pDevice9, L"Resources/shaderResource/Disc.x", &g_pMesh);

	D3DXVECTOR3* pData;
	D3DXVECTOR3 vCenter;
	FLOAT fObjectRadius;
	HRESULT hr;
	V(g_pMesh->LockVertexBuffer(0, (LPVOID*)&pData));
	V(D3DXComputeBoundingSphere(pData, g_pMesh->GetNumVertices(), s_iDECL_SIZE, &vCenter, &fObjectRadius));
	V(g_pMesh->UnlockVertexBuffer());


	SetPOMTextures(POM_STONES);
}

//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also 
// optimizes the mesh for the graphics card's vertex cache, which improves 
// performance by organizing the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
HRESULT ComRenderPOM::LoadMesh(IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh)
{
	ID3DXMesh* pMesh = NULL;
	WCHAR str[MAX_PATH];
	HRESULT hr;

	//====================================================================//
	// Load the mesh with D3DX and get back a ID3DXMesh*.  For this       //
	// sample we'll ignore the X file's embedded materials since we know  //
	// exactly the model we're loading.  See the mesh samples such as     //
	// "OptimizedMesh" for a more generic mesh loading example.           //
	//====================================================================//
	//V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, strFileName));
	//str = strFileName;
	V_RETURN(D3DXLoadMeshFromX(strFileName, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh));

	// Create a new vertex declaration to hold all the required data
	const D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
	{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
	{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
	D3DDECL_END()
	};

	LPD3DXMESH pTempMesh = NULL;

	// Clone mesh to match the specified declaration: 
	if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), vertexDecl, pd3dDevice, &pTempMesh)))
	{
		SAFE_RELEASE(pTempMesh);
		return E_FAIL;
	}

	//====================================================================//
	// Check if the old declaration contains normals, tangents, binormals //
	//====================================================================//
	bool bHadNormal = false;
	bool bHadTangent = false;
	bool bHadBinormal = false;

	D3DVERTEXELEMENT9 vertexOldDecl[MAX_FVF_DECL_SIZE];

	if (pMesh && SUCCEEDED(pMesh->GetDeclaration(vertexOldDecl)))
	{
		// Go through the declaration and look for the right channels, hoping for a match:
		for (UINT iChannelIndex = 0; iChannelIndex < D3DXGetDeclLength(vertexOldDecl); iChannelIndex++)
		{
			if (vertexOldDecl[iChannelIndex].Usage == D3DDECLUSAGE_NORMAL)
			{
				bHadNormal = true;
			}

			if (vertexOldDecl[iChannelIndex].Usage == D3DDECLUSAGE_TANGENT)
			{
				bHadTangent = true;
			}

			if (vertexOldDecl[iChannelIndex].Usage == D3DDECLUSAGE_BINORMAL)
			{
				bHadBinormal = true;
			}
		}
	}

	if (pTempMesh == NULL && (bHadNormal == false || bHadTangent == false || bHadBinormal == false))
	{
		// We failed to clone the mesh and we need the tangent space for our effect:
		return E_FAIL;
	}

	//==============================================================//
	// Generate normals / tangents / binormals if they were missing //
	//==============================================================//
	SAFE_RELEASE(pMesh);
	pMesh = pTempMesh;

	if (!bHadNormal)
	{
		// Compute normals in case the meshes have them
		D3DXComputeNormals(pMesh, NULL);
	}

	DWORD* rgdwAdjacency = NULL;
	rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];

	if (rgdwAdjacency == NULL)
	{
		return E_OUTOFMEMORY;
	}
	V(pMesh->GenerateAdjacency(1e-6f, rgdwAdjacency));

	// Optimize the mesh for this graphics card's vertex cache 
	// so when rendering the mesh's triangle list the vertices will 
	// cache hit more often so it won't have to re-execute the vertex shader 
	// on those vertices so it will improve perf.     
	V(pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL));

	if (!bHadTangent || !bHadBinormal)
	{
		ID3DXMesh* pNewMesh;

		// Compute tangents, which are required for normal mapping
		if (FAILED(D3DXComputeTangentFrameEx(pMesh, D3DDECLUSAGE_TEXCOORD, 0, D3DDECLUSAGE_TANGENT, 0,
			D3DDECLUSAGE_BINORMAL, 0,
			D3DDECLUSAGE_NORMAL, 0, 0, rgdwAdjacency, -1.01f,
			-0.01f, -1.01f, &pNewMesh, NULL)))
		{
			return E_FAIL;
		}

		SAFE_RELEASE(pMesh);
		pMesh = pNewMesh;
	}

	SAFE_DELETE_ARRAY(rgdwAdjacency);

	*ppMesh = pMesh;

	return S_OK;

}

//--------------------------------------------------------------------------------------
// Select a pair of base and normal map / height map textures to use and
// setup related height map range parameters, given a texture index
// Note: all texture surfaces in g_pBaseTextures and g_pNMHTextures MUST
// be created prior to calling this function.
//--------------------------------------------------------------------------------------
void ComRenderPOM::SetPOMTextures(int iTextureIndex)
{
	g_nCurrentTextureID = iTextureIndex;

	// Bind the new active textures to the correct texture slots in the shaders:
	if (pDevice9 && m_pEffect)
	{
		HRESULT hr;

		/*g_pBaseTextures = Assets::GetTexture(L"Resources/shaderResource/stones.bmp");
		g_pNMHTextures = Assets::GetTexture(L"Resources/shaderResource/stones_NM_height.tga");*/
		g_pBaseTextures = Assets::GetTexture(L"Resources/shaderResource/rocks.jpg");
		g_pNMHTextures = Assets::GetTexture(L"Resources/shaderResource/rocks_NM_height.tga");
		V(m_pEffect->SetTexture("g_baseTexture", g_pBaseTextures));
		V(m_pEffect->SetTexture("g_nmhTexture", g_pNMHTextures));

	}

	// Setup height map range slider parameters (need to be setup per-texture, as very height-map specific:
	switch (iTextureIndex)
	{
	case POM_STONES:
	{
		// Stones texture pair:
		g_iHeightScaleSliderMin = 0;
		g_iHeightScaleSliderMax = 10;
		g_fHeightScaleUIScale = 100.0f;
		g_fHeightScale = 0.02f;

		g_fSpecularExponent = 60.0f;
		g_fBaseTextureRepeat = 1.0f;

		g_nMinSamples = 8;
		g_nMaxSamples = 50;
		break;
	}

	case POM_ROCKS:
	{
		g_iHeightScaleSliderMin = 0;
		g_iHeightScaleSliderMax = 10;
		g_fHeightScaleUIScale = 100.0f;
		g_fHeightScale = 0.1f;

		g_fSpecularExponent = 100.0f;
		g_fBaseTextureRepeat = 1.0f;

		g_nMinSamples = 8;
		g_nMaxSamples = 100;
		break;
	}

	case POM_WALL:
	{
		g_iHeightScaleSliderMin = 0;
		g_iHeightScaleSliderMax = 10;
		g_fHeightScaleUIScale = 100.0f;
		g_fHeightScale = 0.06f;

		g_fSpecularExponent = 60.0f;
		g_fBaseTextureRepeat = 1.0f;

		g_nMinSamples = 8;
		g_nMaxSamples = 50;
		break;
	}

	case POM_FOUR_BUMPS:
	{
		g_iHeightScaleSliderMin = 0;
		g_iHeightScaleSliderMax = 10;
		g_fHeightScaleUIScale = 10.0f;
		g_fHeightScale = 0.2f;

		g_fSpecularExponent = 100.0f;
		g_fBaseTextureRepeat = 1.0f;
		g_nMinSamples = 12;
		g_nMaxSamples = 100;
		break;
	}

	case POM_BUMPS:
	{
		g_iHeightScaleSliderMin = 0;
		g_iHeightScaleSliderMax = 10;
		g_fHeightScaleUIScale = 10.0f;
		g_fHeightScale = 0.2f;

		g_fSpecularExponent = 100.0f;
		g_fBaseTextureRepeat = 4.0f;
		g_nMinSamples = 12;
		g_nMaxSamples = 100;
		break;
	}

	case POM_DENTS:
	{
		g_iHeightScaleSliderMin = 0;
		g_iHeightScaleSliderMax = 10;
		g_fHeightScaleUIScale = 10.0f;
		g_fHeightScale = 0.2f;

		g_fSpecularExponent = 100.0f;
		g_fBaseTextureRepeat = 4.0f;
		g_nMinSamples = 12;
		g_nMaxSamples = 100;
		break;
	}

	case POM_SAINT:
	{
		g_iHeightScaleSliderMin = 0;
		g_iHeightScaleSliderMax = 10;
		g_fHeightScaleUIScale = 100.0f;
		g_fHeightScale = 0.08f;

		g_fSpecularExponent = 100.0f;
		g_fBaseTextureRepeat = 1.0f;
		g_nMinSamples = 12;
		g_nMaxSamples = 100;
		break;
	}
	}
}

void ComRenderPOM::Update()
{
}

void ComRenderPOM::Render()
{
	HRESULT hr;
	D3DXMATRIXA16 mWorldViewProjection;
	D3DXVECTOR3 vLightDir;
	// sin(360 - 30), -cos(360 - 30)
	vLightDir = D3DXVECTOR3(sinf(D3DX_PI * 2 - D3DX_PI / 6), 20, -cosf(D3DX_PI * 2 - D3DX_PI / 6));
	
	D3DXCOLOR vLightDiffuse;
	
	vLightDiffuse = g_fLightScale * D3DXCOLOR(1, 1, 1, 1);
	UINT iPass, cPasses;

	D3DXMATRIXA16 mWorld = gameObject->transform->GetWorldMatrix();
	D3DXMATRIXA16 mView = Camera::GetInstance()->GetViewMatrix();;
	D3DXMATRIXA16 mProjection = Camera::GetInstance()->GetProjMatrix();
	mWorldViewProjection = mWorld * mView * mProjection;

	// Get camera position:
	D3DXVECTOR4 vEye = Vector4(Camera::GetInstance()->GetPosition(), 1);

	V(m_pEffect->SetValue("g_LightDir", &vLightDir, sizeof(D3DXVECTOR3)));
	V(m_pEffect->SetValue("g_LightDiffuse", &vLightDiffuse, sizeof(D3DXVECTOR4)));

	// Update the effect's variables.  Instead of using strings, it would 
	// be more efficient to cache a handle to the parameter by calling 
	// ID3DXEffect::GetParameterByName
	V(m_pEffect->SetMatrix("g_mWorldViewProjection", &mWorldViewProjection));
	V(m_pEffect->SetMatrix("g_mWorld", &mWorld));
	V(m_pEffect->SetMatrix("g_mView", &mView));
	V(m_pEffect->SetVector("g_vEye", &vEye));
	V(m_pEffect->SetValue("g_fHeightMapScale", &g_fHeightScale, sizeof(float)));

	D3DXCOLOR vWhite = D3DXCOLOR(1, 1, 1, 1);
	V(m_pEffect->SetValue("g_materialDiffuseColor", &vWhite, sizeof(D3DXCOLOR)));

	V(m_pEffect->SetValue("g_materialAmbientColor", &g_colorMtrlAmbient, sizeof(D3DXCOLOR)));
	V(m_pEffect->SetValue("g_materialDiffuseColor", &g_colorMtrlDiffuse, sizeof(D3DXCOLOR)));
	V(m_pEffect->SetValue("g_materialSpecularColor", &g_colorMtrlSpecular, sizeof(D3DXCOLOR)));


	V(m_pEffect->SetValue("g_fSpecularExponent", &g_fSpecularExponent, sizeof(float)));
	V(m_pEffect->SetValue("g_fBaseTextureRepeat", &g_fBaseTextureRepeat, sizeof(float)));
	V(m_pEffect->SetValue("g_nLODThreshold", &g_nLODThreshold, sizeof(int)));
	V(m_pEffect->SetValue("g_nMinSamples", &g_nMinSamples, sizeof(int)));
	V(m_pEffect->SetValue("g_nMaxSamples", &g_nMaxSamples, sizeof(int)));
	V(m_pEffect->SetValue("g_fShadowSoftening", &g_fShadowSoftening, sizeof(float)));

	V(m_pEffect->SetBool("g_bVisualizeLOD", (BOOL)g_bVisualizeLOD));
	V(m_pEffect->SetBool("g_bVisualizeMipLevel", (BOOL)g_bVisualizeMipLevel));
	V(m_pEffect->SetBool("g_bDisplayShadows", (BOOL)g_bDisplayShadows));
	V(m_pEffect->SetBool("g_bAddSpecular", (BOOL)g_bAddSpecular));

	// Render the scene:
	switch (g_nCurrentTechniqueID)
	{
	case POM:
		V(m_pEffect->SetTechnique("RenderSceneWithPOM"));
		break;

	case BUMPMAP:
		V(m_pEffect->SetTechnique("RenderSceneWithBumpMap"));
		break;

	case PM:
		V(m_pEffect->SetTechnique("RenderSceneWithPM"));
		break;
	}

	V(m_pEffect->Begin(&cPasses, 0));
	for (iPass = 0; iPass < cPasses; iPass++)
	{
		V(m_pEffect->BeginPass(iPass));

//		pDevice9->SetMaterial(&m_mtlTex.material);
		pDevice9->SetFVF(VERTEX_PNT::FVF);
		pDevice9->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PNT));
		pDevice9->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vertices.size() / 3);

		//g_pMesh->DrawSubset(0);

		V(m_pEffect->EndPass());
	}
	V(m_pEffect->End());
}
