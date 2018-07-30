#pragma once
#include "stdafx.h"
#include "DXUTcamera.h"
//--------------------------------------------------------------------------------------
// Texture pair ids for use for this sample
//--------------------------------------------------------------------------------------
enum POM_TEXTURE_IDS
{
	POM_STONES = 0,
	POM_ROCKS,
	POM_WALL,
	POM_FOUR_BUMPS,
	POM_BUMPS,
	POM_DENTS,
	POM_SAINT
};

//--------------------------------------------------------------------------------------
// Id numbers for different rendering techniques used
//--------------------------------------------------------------------------------------
enum TECHNIQUE_IDS
{
	POM,           // Parallax occlusion mapping
	BUMPMAP,       // Bump mapping
	PM             // Parallax mapping with offset limiting
};

const int                   s_iDECL_SIZE = 56;



class ComRenderPOM : public ComRenderAse
{
public:
	ComRenderPOM(CString szName);
	~ComRenderPOM();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	HRESULT LoadMesh(IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh);
	void SetPOMTextures(int iTextureIndex);

	IDirect3DTexture9*         g_pBaseTextures;    // Array of base map texture surfaces
	IDirect3DTexture9*         g_pNMHTextures;    // Array of normal / height map texture surfaces

	int g_nCurrentTechniqueID;
	ID3DXMesh*                  g_pMesh;            // Mesh object

private:
	int g_nCurrentTextureID;
	
	float g_fLightScale;
	float g_fSpecularExponent;        // Material's specular exponent
	float g_fBaseTextureRepeat;        // The tiling factor for base and normal map textures

	bool g_bVisualizeLOD;            // Toggles visualization of level of detail colors
	bool g_bVisualizeMipLevel;       // Toggles visualization of mip level
	bool g_bDisplayShadows;           // Toggles display of self-occlusion based shadows
	bool g_bAddSpecular;              // Toggles rendering with specular or without
	bool g_bRenderPOM;

	int  g_nLODThreshold;                // The mip level id for transitioning between the full computation
																   // for parallax occlusion mapping and the bump mapping computation
	int g_nMinSamples;                  // The minimum number of samples for sampling the height field profile
	int g_nMaxSamples;                 // The maximum number of samples for sampling the height field profile
	float g_fShadowSoftening;         // Blurring factor for the soft shadows computation

	float g_fHeightScale;                    // This parameter controls the height map range for displacement mapping

	int                         g_iHeightScaleSliderMin;
	int                         g_iHeightScaleSliderMax;
	float                       g_fHeightScaleUIScale;

};

