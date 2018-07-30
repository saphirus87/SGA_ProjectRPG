#include "stdafx.h"
#include "Assets.h"


map<CString, LPDIRECT3DTEXTURE9> Assets::m_pTextureList;
map<Assets::FontType, LPD3DXFONT> Assets::m_pFontList;
map<CString, D3DXIMAGE_INFO> Assets::m_imageInfoList;

void Assets::Delete()
{
	for (auto p : m_pTextureList)
	{
		SAFE_RELEASE(p.second);
	}
	m_pTextureList.clear();

	for (auto p : m_pFontList)
	{
		SAFE_RELEASE(p.second);
	}
	m_pFontList.clear();
}


LPDIRECT3DTEXTURE9 Assets::GetTexture(LPCTSTR filePath, D3DXIMAGE_INFO* info)
{
	Device9 pDevice9 = GetD3D9Device();

	if (lstrlen(filePath) == 0) return NULL;

	if (info == NULL)
	{
		if (m_pTextureList.find(filePath) == m_pTextureList.end())
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice9, filePath, &m_pTextureList[filePath])))
				assert(false && "FAILED(D3DXCreateTextureFromFile)");
		}
	}
	else
	{
		if (m_pTextureList.find(filePath) == m_pTextureList.end())
		{
			if (FAILED(D3DXCreateTextureFromFileEx(pDevice9, filePath, 0, 0, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &m_imageInfoList[filePath], NULL, &m_pTextureList[filePath])))
				assert(false && "FAILED(D3DXCreateTextureFromFileEx)");
		}
		*info = m_imageInfoList[filePath];
	}

	return m_pTextureList[filePath];
}


LPD3DXFONT Assets::GetFont(FontType type)
{
	Device9 pDevice9 = GetD3D9Device();

	if (m_pFontList.find(type) == m_pFontList.end())
	{
		if (type == FontType_NORMAL)
		{
			if (FAILED(D3DXCreateFont(pDevice9, 24, 12, FW_NORMAL, 1, false, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("umberto"), &m_pFontList[type])))
				assert(false && "FAILED(D3DXCreateFont)");
		}
		else if (type == FontType_QUEST)
		{
			AddFontResource(_T("Fonts/umberto.ttf"));
			if (FAILED(D3DXCreateFont(pDevice9, 24, 12, FW_NORMAL, 1, false, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, _T("±Ã¼­Ã¼"), &m_pFontList[type])))
				assert(false && "FAILED(D3DXCreateFont)");
		}
	}

	return m_pFontList[type];
}
