/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 DirectX Sample 코드를 가져와 사용한 것입니다.

설명 :
후처리(Post Processing)을 담당합니다.
여기에서 모든 후처리는 픽셀 셰이더입니다.
*/
#pragma once
#include "stdafx.h"

// NUM_PARAMS is the maximum number of changeable parameters supported
// in an effect.
#define NUM_PARAMS 2

// RT_COUNT is the number of simultaneous render targets used in the sample.
#define RT_COUNT 3

struct PPVERT;
//--------------------------------------------------------------------------------------
// This is the vertex format used for the skybox.
struct SKYBOXVERT;

//--------------------------------------------------------------------------------------
// struct CPProcInstance
// A class that represents an instance of a post-process to be applied
// to the scene.
//--------------------------------------------------------------------------------------
struct CPProcInstance
{
	D3DXVECTOR4 m_avParam[NUM_PARAMS];
	int m_nFxIndex;

public:
	CPProcInstance() : m_nFxIndex(-1)
	{
		ZeroMemory(m_avParam, sizeof(m_avParam));
	}
};

// An CRenderTargetSet object dictates what render targets
// to use in a pass of scene rendering.
struct CRenderTargetSet
{
	IDirect3DSurface9* pRT[RT_COUNT];
};

struct CRenderTargetChain
{
	int m_nNext;
	bool m_bFirstRender;
	LPDIRECT3DTEXTURE9  m_pRenderTarget[2];

public:
	CRenderTargetChain() : m_nNext(0),
		m_bFirstRender(true)
	{
		ZeroMemory(m_pRenderTarget, sizeof(m_pRenderTarget));
	}

	~CRenderTargetChain()
	{
		Cleanup();
	}

	void                Init(LPDIRECT3DTEXTURE9* pRT)
	{
		for (int i = 0; i < 2; ++i)
		{
			m_pRenderTarget[i] = pRT[i];
			m_pRenderTarget[i]->AddRef();
		}
	}

	void                Cleanup()
	{
		SAFE_RELEASE(m_pRenderTarget[0]);
		SAFE_RELEASE(m_pRenderTarget[1]);
	}

	void                Flip()
	{
		m_nNext = 1 - m_nNext;
	};

	LPDIRECT3DTEXTURE9  GetPrevTarget()
	{
		return m_pRenderTarget[1 - m_nNext];
	}
	LPDIRECT3DTEXTURE9  GetPrevSource()
	{
		return m_pRenderTarget[m_nNext];
	}
	LPDIRECT3DTEXTURE9  GetNextTarget()
	{
		return m_pRenderTarget[m_nNext];
	}
	LPDIRECT3DTEXTURE9  GetNextSource()
	{
		return m_pRenderTarget[1 - m_nNext];
	}
};

struct CPostProcess
{
	LPD3DXEFFECT m_pEffect;              // Effect object for this technique
	D3DXHANDLE m_hTPostProcess;        // PostProcess technique handle
	int m_nRenderTarget;        // Render target channel this PP outputs
	D3DXHANDLE  m_hTexSource[4];        // Handle to the post-process source textures
	D3DXHANDLE  m_hTexScene[4];         // Handle to the saved scene texture
	bool        m_bWrite[4];            // Indicates whether the post-process technique
										//   outputs data for this render target.
	WCHAR       m_awszParamName
		[NUM_PARAMS][MAX_PATH]; // Names of changeable parameters
	WCHAR       m_awszParamDesc
		[NUM_PARAMS][MAX_PATH]; // Description of parameters
	D3DXHANDLE  m_ahParam[NUM_PARAMS];  // Handles to the changeable parameters
	int         m_anParamSize[NUM_PARAMS];// Size of the parameter. Indicates
										  // how many components of float4
										  // are used.
	D3DXVECTOR4 m_avParamDef[NUM_PARAMS]; // Parameter default

public:
	CPostProcess() : m_pEffect(NULL),
		m_hTPostProcess(NULL),
		m_nRenderTarget(0)
	{
		ZeroMemory(m_hTexSource, sizeof(m_hTexSource));
		ZeroMemory(m_hTexScene, sizeof(m_hTexScene));
		ZeroMemory(m_bWrite, sizeof(m_bWrite));
		ZeroMemory(m_ahParam, sizeof(m_ahParam));
		ZeroMemory(m_awszParamName, sizeof(m_awszParamName));
		ZeroMemory(m_awszParamDesc, sizeof(m_awszParamDesc));
		ZeroMemory(m_anParamSize, sizeof(m_anParamSize));
		ZeroMemory(m_avParamDef, sizeof(m_avParamDef));
	}
	~CPostProcess()
	{
		Cleanup();
	}
	HRESULT     Init(LPDIRECT3DDEVICE9 pDev, DWORD dwShaderFlags, LPCWSTR wszName)
	{
		HRESULT hr;
		WCHAR wszPath[MAX_PATH];

		// 파일이 없으면 return
		//if (FAILED(hr = DXUTFindDXSDKMediaFileCch(wszPath, MAX_PATH, wszName)))
		//	return hr;

		hr = D3DXCreateEffectFromFile(pDev,
			wszName,
			NULL,
			NULL,
			dwShaderFlags,
			NULL,
			&m_pEffect,
			NULL);
		if (FAILED(hr))
			return hr;

		// Get the PostProcess technique handle
		m_hTPostProcess = m_pEffect->GetTechniqueByName("PostProcess");

		// Obtain the handles to all texture objects in the effect
		m_hTexScene[0] = m_pEffect->GetParameterByName(NULL, "g_txSceneColor");
		m_hTexScene[1] = m_pEffect->GetParameterByName(NULL, "g_txSceneNormal");
		m_hTexScene[2] = m_pEffect->GetParameterByName(NULL, "g_txScenePosition");
		m_hTexScene[3] = m_pEffect->GetParameterByName(NULL, "g_txSceneVelocity");
		m_hTexSource[0] = m_pEffect->GetParameterByName(NULL, "g_txSrcColor");
		m_hTexSource[1] = m_pEffect->GetParameterByName(NULL, "g_txSrcNormal");
		m_hTexSource[2] = m_pEffect->GetParameterByName(NULL, "g_txSrcPosition");
		m_hTexSource[3] = m_pEffect->GetParameterByName(NULL, "g_txSrcVelocity");

		// Find out what render targets the technique writes to.
		D3DXTECHNIQUE_DESC techdesc;
		if (FAILED(m_pEffect->GetTechniqueDesc(m_hTPostProcess, &techdesc)))
			return D3DERR_INVALIDCALL;

		for (DWORD i = 0; i < techdesc.Passes; ++i)
		{
			D3DXPASS_DESC passdesc;
			if (SUCCEEDED(m_pEffect->GetPassDesc(m_pEffect->GetPass(m_hTPostProcess, i), &passdesc)))
			{
				D3DXSEMANTIC aSem[MAXD3DDECLLENGTH];
				UINT uCount;
				if (SUCCEEDED(D3DXGetShaderOutputSemantics(passdesc.pPixelShaderFunction, aSem, &uCount)))
				{
					// Semantics received. Now examine the content and
					// find out which render target this technique
					// writes to.
					while (uCount--)
					{
						if (D3DDECLUSAGE_COLOR == aSem[uCount].Usage &&
							RT_COUNT > aSem[uCount].UsageIndex)
							m_bWrite[uCount] = true;
					}
				}
			}
		}

		// Obtain the render target channel
		D3DXHANDLE hAnno;
		hAnno = m_pEffect->GetAnnotationByName(m_hTPostProcess, "nRenderTarget");
		if (hAnno)
			m_pEffect->GetInt(hAnno, &m_nRenderTarget);

		// Obtain the handles to the changeable parameters, if any.
		for (int i = 0; i < NUM_PARAMS; ++i)
		{
			char szName[32];

			sprintf_s(szName, 32, "Parameter%d", i);
			hAnno = m_pEffect->GetAnnotationByName(m_hTPostProcess, szName);
			LPCSTR szParamName;
			if (hAnno &&
				SUCCEEDED(m_pEffect->GetString(hAnno, &szParamName)))
			{
				m_ahParam[i] = m_pEffect->GetParameterByName(NULL, szParamName);
				MultiByteToWideChar(CP_ACP, 0, szParamName, -1, m_awszParamName[i], MAX_PATH);
			}

			// Get the parameter description
			sprintf_s(szName, 32, "Parameter%dDesc", i);
			hAnno = m_pEffect->GetAnnotationByName(m_hTPostProcess, szName);
			if (hAnno &&
				SUCCEEDED(m_pEffect->GetString(hAnno, &szParamName)))
			{
				MultiByteToWideChar(CP_ACP, 0, szParamName, -1, m_awszParamDesc[i], MAX_PATH);
			}

			// Get the parameter size
			sprintf_s(szName, 32, "Parameter%dSize", i);
			hAnno = m_pEffect->GetAnnotationByName(m_hTPostProcess, szName);
			if (hAnno)
				m_pEffect->GetInt(hAnno, &m_anParamSize[i]);

			// Get the parameter default
			sprintf_s(szName, 32, "Parameter%dDef", i);
			hAnno = m_pEffect->GetAnnotationByName(m_hTPostProcess, szName);
			if (hAnno)
				m_pEffect->GetVector(hAnno, &m_avParamDef[i]);
		}

		return S_OK;
	}
	void        Cleanup()
	{
		SAFE_RELEASE(m_pEffect);
	}
	HRESULT     OnLostDevice()
	{
		assert(m_pEffect);
		m_pEffect->OnLostDevice();
		return S_OK;
	}
	HRESULT     OnResetDevice(DWORD dwWidth, DWORD dwHeight)
	{
		assert(m_pEffect);
		m_pEffect->OnResetDevice();

		// If one or more kernel exists, convert kernel from
		// pixel space to texel space.

		// First check for kernels.  Kernels are identified by
		// having a string annotation of name "ConvertPixelsToTexels"
		D3DXHANDLE hParamToConvert;
		D3DXHANDLE hAnnotation;
		UINT uParamIndex = 0;
		// If a top-level parameter has the "ConvertPixelsToTexels" annotation,
		// do the conversion.
		while (NULL != (hParamToConvert = m_pEffect->GetParameter(NULL, uParamIndex++)))
		{
			if (NULL != (hAnnotation = m_pEffect->GetAnnotationByName(hParamToConvert, "ConvertPixelsToTexels")))
			{
				LPCSTR szSource;
				m_pEffect->GetString(hAnnotation, &szSource);
				D3DXHANDLE hConvertSource = m_pEffect->GetParameterByName(NULL, szSource);

				if (hConvertSource)
				{
					// Kernel source exists. Proceed.
					// Retrieve the kernel size
					D3DXPARAMETER_DESC desc;
					m_pEffect->GetParameterDesc(hConvertSource, &desc);
					// Each element has 2 floats
					DWORD cKernel = desc.Bytes / (2 * sizeof(float));
					D3DXVECTOR4* pvKernel = new D3DXVECTOR4[cKernel];
					if (!pvKernel)
						return E_OUTOFMEMORY;
					m_pEffect->GetVectorArray(hConvertSource, pvKernel, cKernel);
					// Convert
					for (DWORD i = 0; i < cKernel; ++i)
					{
						pvKernel[i].x = pvKernel[i].x / dwWidth;
						pvKernel[i].y = pvKernel[i].y / dwHeight;
					}
					// Copy back
					m_pEffect->SetVectorArray(hParamToConvert, pvKernel, cKernel);

					delete[] pvKernel;
				}
			}
		}

		return S_OK;
	}
};

//--------------------------------------------------------------------------------------
// struct CPostProcess
// A struct that encapsulates aspects of a render target PostProcess
// technique.
//--------------------------------------------------------------------------------------
struct CPostProcess;

class ComPostProcess : public Component
{
public:
	ComPostProcess(CString szName);
	~ComPostProcess();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	//--------------------------------------------------------------------------------------
	// PerformPostProcess()
	// Perform all active post-processes in order.
	HRESULT PerformPostProcess(Device9 pd3dDevice);

	//--------------------------------------------------------------------------------------
	// Name: PerformSinglePostProcess()
	// Desc: Perform post-process by setting the previous render target as a
	//       source texture and rendering a quad with the post-process technique
	//       set.
	//       This method changes render target without saving any. The caller
	//       should ensure that the default render target is saved before calling
	//       this.
	//       When this method is invoked, m_dwNextTarget is the index of the
	//       rendertarget of this post-process.  1 - m_dwNextTarget is the index
	//       of the source of this post-process.
	HRESULT PerformSinglePostProcess(IDirect3DDevice9* pd3dDevice,
		CPostProcess& PP,
		CPProcInstance& Inst,
		IDirect3DVertexBuffer9* pVB,
		PPVERT* aQuad,
		float& fExtentX,
		float& fExtentY);

	//--------------------------------------------------------------------------------------
	// Inserts the PostProcess effect identified by the index nEffectIndex into the
	// active list.
	void InsertEffect(int nEffectIndex);

	list<GameObject*> m_listRenderObject;
	
private:
	GameObject * m_pGrid;

	Device9 pDevice9;

	Shader m_pEffect;
	VertexDecl m_pVertexDecl;
	VertexDecl g_pSkyBoxDecl;
	int g_nRtUsed;
	CRenderTargetChain g_RTChain[RT_COUNT];
	Texture g_pSceneSave[RT_COUNT];
	IDirect3DCubeTexture9* g_pEnvTex;               // Texture for environment mapping
	CRenderTargetSet g_aRtTable[RT_COUNT];    // Table of which RT to use for all passes
	list<CPProcInstance*> m_listProcInstance;
	vector<CPostProcess> m_aPostProcess;  // Effect object for PostProcesses
	vector<CString> g_aszFxFile;
};

