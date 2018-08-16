#include "stdafx.h"
#include "ComPostProcess.h"
#include "SDKmesh.h"

CDXUTXFileMesh                  g_Skybox;                // Skybox mesh

struct PPVERT
{
	float x, y, z, rhw;
	float tu, tv;       // Texcoord for post-process source
	float tu2, tv2;     // Texcoord for the original scene

	const static D3DVERTEXELEMENT9 Decl[4];
};

// Vertex declaration for post-processing
const D3DVERTEXELEMENT9 PPVERT::Decl[4] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1 },
D3DDECL_END()
};

struct SKYBOXVERT
{
	float x, y, z;      // Position
	D3DXVECTOR3 tex;    // Texcoord

	const static D3DVERTEXELEMENT9 Decl[3];
};

const D3DVERTEXELEMENT9 SKYBOXVERT::Decl[] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
D3DDECL_END()
};

ComPostProcess::ComPostProcess(CString szName) : 
	Component(szName),
	g_nRtUsed(3),
	m_pGrid(NULL)
{
	pDevice9 = GetD3D9Device();
}

ComPostProcess::~ComPostProcess()
{
	SAFE_RELEASE(m_pVertexDecl);
	SAFE_RELEASE(g_pSkyBoxDecl);
	SAFE_RELEASE(g_pEnvTex);
	
	g_Skybox.Destroy();

	// Release the scene save and render target textures
	for (int i = 0; i < RT_COUNT; ++i)
	{
		SAFE_RELEASE(g_pSceneSave[i]);
		g_RTChain[i].Cleanup();
	}

	// Release the RT table's references
	for (int p = 0; p < RT_COUNT; ++p)
		for (int rt = 0; rt < RT_COUNT; ++rt)
			SAFE_RELEASE(g_aRtTable[p].pRT[rt]);

	for (auto & o : m_aPostProcess)
		o.Cleanup();
}

void ComPostProcess::Awake()
{
	// Create vertex declaration for post-process
	pDevice9->CreateVertexDeclaration(PPVERT::Decl, &m_pVertexDecl);
	pDevice9->CreateVertexDeclaration(SKYBOXVERT::Decl, &g_pSkyBoxDecl);
	
	CString szFolder = "Resources/shader/PostEffect/";
	g_aszFxFile.push_back(szFolder + "PP_ColorMonochrome.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorInverse.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorGBlurH.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorGBlurV.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorBloomH.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorBloomV.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorBrightPass.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorToneMap.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorEdgeDetect.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorDownFilter4.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorUpFilter4.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorCombine.fx");
	g_aszFxFile.push_back(szFolder + "PP_ColorCombine4.fx");
	g_aszFxFile.push_back(szFolder + "PP_NormalEdgeDetect.fx");
	g_aszFxFile.push_back(szFolder + "PP_DofCombine.fx");
	g_aszFxFile.push_back(szFolder + "PP_NormalMap.fx");
	g_aszFxFile.push_back(szFolder + "PP_PositionMap.fx");

	m_pEffect = Shaders::GetInstance()->GetShader("Resources/shader/PostEffect/Scene.fx");
	
	HRESULT hr;
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc = DXUTGetD3D9BackBufferSurfaceDesc();

	m_aPostProcess.resize(g_aszFxFile.size());
	// Initialize the PostProcess objects
	for (int i = 0; i < g_aszFxFile.size(); ++i)
	{
		m_aPostProcess[i].Init(pDevice9, D3DXSHADER_DEBUG, g_aszFxFile[i]);
		m_aPostProcess[i].OnResetDevice(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	}

	// Create scene save texture
	for (int i = 0; i < RT_COUNT; ++i)
	{
		pDevice9->CreateTexture(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&g_pSceneSave[i],
			NULL);

		// Create the textures for this render target chains
		IDirect3DTexture9* pRT[2];
		ZeroMemory(pRT, sizeof(pRT));
		for (int t = 0; t < 2; ++t)
		{
			pDevice9->CreateTexture(pBackBufferSurfaceDesc->Width,
				pBackBufferSurfaceDesc->Height,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&pRT[t],
				NULL);
		}
		g_RTChain[i].Init(pRT);
		SAFE_RELEASE(pRT[0]);
		SAFE_RELEASE(pRT[1]);
	}

	// Initialize the render target table based on how many simultaneous RTs
	// the card can support.
	IDirect3DSurface9* pSurf;
	
	g_pSceneSave[0]->GetSurfaceLevel(0, &pSurf);
	g_aRtTable[0].pRT[0] = pSurf;
	g_pSceneSave[1]->GetSurfaceLevel(0, &pSurf);
	g_aRtTable[0].pRT[1] = pSurf;
	g_pSceneSave[2]->GetSurfaceLevel(0, &pSurf);
	g_aRtTable[0].pRT[2] = pSurf;
	// Passes 1 and 2 are not used
	g_aRtTable[1].pRT[0] = NULL;
	g_aRtTable[1].pRT[1] = NULL;
	g_aRtTable[1].pRT[2] = NULL;
	g_aRtTable[2].pRT[0] = NULL;
	g_aRtTable[2].pRT[1] = NULL;
	g_aRtTable[2].pRT[2] = NULL;

	g_Skybox.Create(pDevice9, L"Resources/shaderResource/alley_skybox.x");
	g_Skybox.SetVertexDecl(pDevice9, SKYBOXVERT::Decl);
	// Create the environment mapping texture
	WCHAR str[MAX_PATH];
	D3DXCreateCubeTextureFromFile(pDevice9, L"Resources/shaderResource/Snow_ENV_SkyBox.dds", &g_pEnvTex);

	m_listProcInstance.clear();
	
	// IDC_PREBLUR
	// Insert effects
	/*InsertEffect(9);
	InsertEffect(2);
	InsertEffect(3);
	InsertEffect(2);
	InsertEffect(3);
	InsertEffect(10);*/

	// IDC_PREBLOOM
	// Insert effects
	/*InsertEffect(9);
	InsertEffect(9);
	InsertEffect(6);
	InsertEffect(4);
	InsertEffect(5);
	InsertEffect(4);
	InsertEffect(5);
	InsertEffect(10);
	InsertEffect(12); */

	// IDC_PREDOF
	// Insert effects
	//InsertEffect(9);
	//InsertEffect(2);
	//InsertEffect(3);
	//InsertEffect(2);
	//InsertEffect(3);
	//InsertEffect(10);
	//InsertEffect(14); //DOF Combine

	// IDC_PREEDGE
	// EDGE DETECTION
	//InsertEffect(13);

	// IDC_PREEDGE
	// Insert effects
	/*InsertEffect(13);
	InsertEffect(9);
	InsertEffect(4);
	InsertEffect(5);
	InsertEffect(12);*/

	// MONOCHROME
	//InsertEffect(0);

	// INVERSION
	//InsertEffect(1);

	// BRIGHT PATH
	//InsertEffect(6);

	// TONE MAPPING
	//InsertEffect(7);

	// POSITION MAP
	//InsertEffect(16);
}

void ComPostProcess::Update()
{
}

void ComPostProcess::Render()
{
	pDevice9->EndScene();

	// Save render target 0 so we can restore it later
	IDirect3DSurface9* pOldRT;
	pDevice9->GetRenderTarget(0, &pOldRT);
	
	if (SUCCEEDED(pDevice9->BeginScene()))
	{
		for (int rt = 0; rt < g_nRtUsed; ++rt)
		{
			pDevice9->SetRenderTarget(rt, g_aRtTable[0].pRT[rt]);
		}

		// Clear the render target and the zbuffer 
		pDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(1, 64, 64, 64), 1.0f, 0);
		
		if (m_pGrid == NULL)
		{
			list<GameObject*>::iterator iter;
			for (iter = m_listRenderObject.begin(); iter != m_listRenderObject.end(); ++iter)
			{
				if ((*iter)->Name().Find(L"Grid") >= 0)
				{
					m_pGrid = (*iter);
					m_listRenderObject.erase(iter);
					break;
				}
			}
		}
		if (m_pGrid)
			m_pGrid->Render();

		/*m_listRenderObject.sort(CompareZ);
		for (auto & o : m_listRenderObject)
			o->Render();*/
		// Render the skybox as if the camera is at center
		D3DXMATRIXA16 mWorldView;
		D3DXMatrixIdentity(&mWorldView);
		m_pEffect->SetTechnique("RenderSkyBox");
		pDevice9->SetVertexDeclaration(g_pSkyBoxDecl);

		D3DXMATRIXA16 mView = Camera::GetInstance()->GetViewMatrix();
		mView._41 = mView._42 = mView._43 = 0.0f;

		D3DXMatrixScaling(&mWorldView, 100.0f, 100.0f, 100.0f);
		D3DXMatrixMultiply(&mWorldView, &mWorldView, &mView);
		m_pEffect->SetMatrix("g_mWorldView", &mWorldView);
		Camera::GetInstance()->SetProj(1000);
		Matrix4x4 matProj = Camera::GetInstance()->GetProjMatrix();
		Camera::GetInstance()->SetProj(100);
		m_pEffect->SetMatrix("g_mProj", &matProj);
		m_pEffect->SetTexture("g_txScene", g_pEnvTex);

		UINT cPass;
		LPD3DXMESH pMeshObj = g_Skybox.GetMesh();
		m_pEffect->Begin(&cPass, 0);
		for (int p = 0; p < cPass; ++p)
		{
			// Set the render target(s) for this pass
			for (int rt = 0; rt < g_nRtUsed; ++rt)
				pDevice9->SetRenderTarget(rt, g_aRtTable[p].pRT[rt]);

			m_pEffect->BeginPass(p);

			// Iterate through each subset and render with its texture
			for (DWORD m = 0; m < g_Skybox.m_dwNumMaterials; ++m)
			{
				m_pEffect->SetTexture("g_txScene", g_Skybox.m_pTextures[m]);
				m_pEffect->CommitChanges();
				pMeshObj->DrawSubset(m);
			}

			m_pEffect->EndPass();
		}
		m_pEffect->End();

		m_listRenderObject.sort(CompareZ);
		for (auto & o : m_listRenderObject)
			o->Render();

		pDevice9->EndScene();
	}

	// È¯°æ¸Ê ·»´õ¸µ


	//
    // Swap the chains
    //
	int i;
	for (i = 0; i < RT_COUNT; ++i)
		g_RTChain[i].Flip();

	// Reset all render targets used besides RT 0
	for (i = 1; i < g_nRtUsed; ++i)
		pDevice9->SetRenderTarget(i, NULL);

	//
	// Perform post-processes
	//
	bool bPerformPostProcess = !m_listProcInstance.empty();

	if (bPerformPostProcess)
		PerformPostProcess(pDevice9);

	// Restore old render target 0 (back buffer)
	pDevice9->SetRenderTarget(0, pOldRT);
	SAFE_RELEASE(pOldRT);

	//
	// Get the final result image onto the backbuffer
	//
	const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetD3D9BackBufferSurfaceDesc();
	if (SUCCEEDED(pDevice9->BeginScene()))
	{
		// Render a screen-sized quad
		PPVERT quad[4] =
		{
			{ -0.5f,                          -0.5f,                           0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			{ pd3dsdBackBuffer->Width - 0.5f, -0.5f,                           0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
			{ -0.5f,                          pd3dsdBackBuffer->Height - 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
			{ pd3dsdBackBuffer->Width - 0.5f, pd3dsdBackBuffer->Height - 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f }
		};
		IDirect3DTexture9* pPrevTarget;
		pPrevTarget = (bPerformPostProcess) ? g_RTChain[0].GetPrevTarget() : g_pSceneSave[0];

		pDevice9->SetVertexDeclaration(m_pVertexDecl);
		m_pEffect->SetTechnique("RenderNoLight");
		m_pEffect->SetTexture("g_txScene", pPrevTarget);
		UINT cPasses;
		m_pEffect->Begin(&cPasses, 0);
		for (int p = 0; p < cPasses; ++p)
		{
			m_pEffect->BeginPass(p);
			pDevice9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(PPVERT));
			m_pEffect->EndPass();
		}
		m_pEffect->End();

		pDevice9->EndScene();
	}

	pDevice9->BeginScene();
}

HRESULT ComPostProcess::PerformPostProcess(Device9 pd3dDevice)
{
	HRESULT hr;

	//
	// Extents are used to control how much of the rendertarget is rendered
	// during PostProcess. For example, with the extent of 0.5 and 0.5, only
	// the upper left quarter of the rendertarget will be rendered during
	// PostProcess.
	//
	float fExtentX = 1.0f, fExtentY = 1.0f;
	const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetD3D9BackBufferSurfaceDesc();

	//
	// Set up our quad
	//
	PPVERT Quad[4] =
	{
		{ -0.5f,							-0.5f,								1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ pd3dsdBackBuffer->Width - 0.5f,	-0.5,								1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ -0.5,								pd3dsdBackBuffer->Height - 0.5f,	1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
		{ pd3dsdBackBuffer->Width - 0.5f,	pd3dsdBackBuffer->Height - 0.5f,	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
	};

	//
	// Create a vertex buffer out of the quad
	//
	IDirect3DVertexBuffer9* pVB;
	hr = pd3dDevice->CreateVertexBuffer(sizeof(PPVERT) * 4,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		0,
		D3DPOOL_DEFAULT,
		&pVB,
		NULL);
	if (FAILED(hr))
		return DXUT_ERR(L"CreateVertexBuffer", hr);

	// Fill in the vertex buffer
	LPVOID pVBData;
	if (SUCCEEDED(pVB->Lock(0, 0, &pVBData, D3DLOCK_DISCARD)))
	{
		CopyMemory(pVBData, Quad, sizeof(Quad));
		pVB->Unlock();
	}

	// Clear first-time render flags
	for (int i = 0; i < RT_COUNT; ++i)
		g_RTChain[i].m_bFirstRender = true;
	
	for (auto & pInstance : m_listProcInstance)
	{
		PerformSinglePostProcess(pd3dDevice,
			m_aPostProcess[pInstance->m_nFxIndex],
			*pInstance,
			pVB,
			Quad,
			fExtentX,
			fExtentY);
	}

	// Release the vertex buffer
	pVB->Release();

	return S_OK;
}

HRESULT ComPostProcess::PerformSinglePostProcess(IDirect3DDevice9 * pd3dDevice, CPostProcess & PP, CPProcInstance & Inst, IDirect3DVertexBuffer9 * pVB, PPVERT * aQuad, float & fExtentX, float & fExtentY)
{
	HRESULT hr;
	//
	// The post-process effect may require that a copy of the
	// originally rendered scene be available for use, so
	// we initialize them here.
	//
	for (int i = 0; i < RT_COUNT; ++i)
		PP.m_pEffect->SetTexture(PP.m_hTexScene[i], g_pSceneSave[i]);

	/*PP.m_pEffect->SetTexture("g_txSrcColor", g_pSceneSave[0]);
	PP.m_pEffect->SetTexture("g_txSrcNormal", g_pSceneSave[1]);
	PP.m_pEffect->SetTexture("g_txSrcPosition", g_pSceneSave[2]);*/

	// DOF
	/*if (Inst.m_nFxIndex == 14)
		Inst.m_avParam[0] = Camera::GetInstance()->GetDOF();*/

	//
	// If there are any parameters, initialize them here.
	//
	for (int i = 0; i < NUM_PARAMS; ++i)
		if (PP.m_ahParam[i])
			PP.m_pEffect->SetVector(PP.m_ahParam[i], &Inst.m_avParam[i]);

	// Render the quad
	if (SUCCEEDED(pd3dDevice->BeginScene()))
	{
		PP.m_pEffect->SetTechnique("PostProcess");

		// Set the vertex declaration
		pd3dDevice->SetVertexDeclaration(m_pVertexDecl);

		// Draw the quad
		UINT cPasses, p;
		PP.m_pEffect->Begin(&cPasses, 0);
		for (p = 0; p < cPasses; ++p)
		{
			bool bUpdateVB = false;  // Inidicates whether the vertex buffer
									 // needs update for this pass.

									 //
									 // If the extents has been modified, the texture coordinates
									 // in the quad need to be updated.
									 //

			if (aQuad[1].tu != fExtentX)
			{
				aQuad[1].tu = aQuad[3].tu = fExtentX;
				bUpdateVB = true;
			}
			if (aQuad[2].tv != fExtentY)
			{
				aQuad[2].tv = aQuad[3].tv = fExtentY;
				bUpdateVB = true;
			}

			//
			// Check if the pass has annotation for extent info.  Update
			// fScaleX and fScaleY if it does.  Otherwise, default to 1.0f.
			//

			float fScaleX = 1.0f, fScaleY = 1.0f;
			D3DXHANDLE hPass = PP.m_pEffect->GetPass(PP.m_hTPostProcess, p);
			D3DXHANDLE hExtentScaleX = PP.m_pEffect->GetAnnotationByName(hPass, "fScaleX");
			if (hExtentScaleX)
				PP.m_pEffect->GetFloat(hExtentScaleX, &fScaleX);
			D3DXHANDLE hExtentScaleY = PP.m_pEffect->GetAnnotationByName(hPass, "fScaleY");
			if (hExtentScaleY)
				PP.m_pEffect->GetFloat(hExtentScaleY, &fScaleY);

			//
			// Now modify the quad according to the scaling values specified for
			// this pass
			//
			if (fScaleX != 1.0f)
			{
				aQuad[1].x = (aQuad[1].x + 0.5f) * fScaleX - 0.5f;
				aQuad[3].x = (aQuad[3].x + 0.5f) * fScaleX - 0.5f;
				bUpdateVB = true;
			}
			if (fScaleY != 1.0f)
			{
				aQuad[2].y = (aQuad[2].y + 0.5f) * fScaleY - 0.5f;
				aQuad[3].y = (aQuad[3].y + 0.5f) * fScaleY - 0.5f;
				bUpdateVB = true;
			}

			if (bUpdateVB)
			{
				LPVOID pVBData;
				// Scaling requires updating the vertex buffer.
				if (SUCCEEDED(pVB->Lock(0, 0, &pVBData, D3DLOCK_DISCARD)))
				{
					CopyMemory(pVBData, aQuad, 4 * sizeof(PPVERT));
					pVB->Unlock();
				}
			}
			fExtentX *= fScaleX;
			fExtentY *= fScaleY;

			// Set up the textures and the render target
			//
			for (int i = 0; i < RT_COUNT; ++i)
			{
				// If this is the very first post-process rendering,
				// obtain the source textures from the scene.
				// Otherwise, initialize the post-process source texture to
				// the previous render target.
				//
				if (g_RTChain[i].m_bFirstRender)
					PP.m_pEffect->SetTexture(PP.m_hTexSource[i], g_pSceneSave[i]);
				else
					PP.m_pEffect->SetTexture(PP.m_hTexSource[i], g_RTChain[i].GetNextSource());
			}

			//
			// Set up the new render target
			//
			IDirect3DTexture9* pTarget = g_RTChain[PP.m_nRenderTarget].GetNextTarget();
			IDirect3DSurface9* pTexSurf;
			hr = pTarget->GetSurfaceLevel(0, &pTexSurf);
			if (FAILED(hr))
				return DXUT_ERR(L"GetSurfaceLevel", hr);
			pd3dDevice->SetRenderTarget(0, pTexSurf);
			pTexSurf->Release();
			// We have output to this render target. Flag it.
			g_RTChain[PP.m_nRenderTarget].m_bFirstRender = false;

			//
			// Clear the render target
			//
			pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L);
			
			//
			// Render
			//
			PP.m_pEffect->BeginPass(p);
			pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(PPVERT));
			pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			PP.m_pEffect->EndPass();

			// Update next rendertarget index
			g_RTChain[PP.m_nRenderTarget].Flip();
		}
		PP.m_pEffect->End();

		// End scene
		pd3dDevice->EndScene();
	}

	return S_OK;
}

void ComPostProcess::InsertEffect(int nEffectIndex)
{
	// Create a new CPProcInstance object and set it as the data field of the
	// newly inserted item.
	CPProcInstance* pNewInst = new CPProcInstance;

	if (pNewInst)
	{
		pNewInst->m_nFxIndex = nEffectIndex;
		ZeroMemory(pNewInst->m_avParam, sizeof(pNewInst->m_avParam));
		for (int p = 0; p < NUM_PARAMS; ++p)
			pNewInst->m_avParam[p] = m_aPostProcess[pNewInst->m_nFxIndex].m_avParamDef[p];

		m_listProcInstance.push_back(pNewInst);
	}
}
