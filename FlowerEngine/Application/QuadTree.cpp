#include "stdafx.h"
#include "QuadTree.h"


int QuadTree::_GenTriIndex(int Triangles, vector<DWORD>& pIndex)
{
	if (m_Culled)
	{
		m_Culled = false;
		return Triangles;
	}

	// 현재 노드가 출력되어야 하는 경우 인덱스 추가
	if (_IsVisible())
	{
		// 와우 기준 하나의 사각형이 4개의 삼각형으로 구성됨

		for (int i = 0; i < 12; i++)
		{
			if (i % 3 == 0) ++Triangles;
			pIndex[Triangles * 3 + i % 3] = m_Center + i;
			//pIndex.push_back(m_Center + i);
			
		}

		return Triangles;
	}

	// 자식 노드 검색
	for (int i = 0; i < 4; ++i)
	{
		if (m_pChild[i] != NULL) Triangles = m_pChild[i]->_GenTriIndex(Triangles, pIndex);
	}

	return Triangles;
}

int QuadTree::_IsInFrustum()
{
	bool InPoint[4];
	bool InSphere;

	if (_IsVisible())
		InSphere = Camera::GetInstance()->FrustumCulling(&(*m_MapVertex)[m_Center].p, m_Radius);
	else
		InSphere = Camera::GetInstance()->FrustumCulling(&(*m_MapVertex)[(*m_pIndex)[m_Center] + 7].p, m_Radius);
	// 절두체 안에 구가 포함되지 않으면 바로 함수 종료(포인트별 체크 불필요)
	if (!InSphere) return FRUSTUM_OUT;

	for (int i = 0; i < 4; ++i)
	{
		InPoint[i] = Camera::GetInstance()->FrustumCulling(&(*m_MapVertex)[(*m_pIndex)[m_Corner[i]]].p);
	}

	// 4개 정점이 모두 절두체 안에 포함
	if ((InPoint[0] + InPoint[1] + InPoint[2] + InPoint[3]) == 4) return FRUSTUM_COMPLETELY_IN;

	// 4개 정점 중 일부만 절두체 안에 포함
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::_FrustumCull()
{
	int result = _IsInFrustum();

	switch (result)
	{
	case QuadTree::FRUSTUM_OUT:	// 절두체에 포함되지 않음(하위 노드 탐색 불필요)
		m_Culled = true;
		return;
	case QuadTree::FRUSTUM_PARTIALLY_IN:	// 절두체에 일부분이 포함(하위 노드 탐색 필요)
		m_Culled = false;
		break;
	case QuadTree::FRUSTUM_COMPLETELY_IN:	// 절두체에 전체 포함(하위 노드 탐색 불필요)
		m_Culled = false;
		return;
	case QuadTree::FRUSTUM_UNKNOWN:
		break;
	default:
		break;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (m_pChild[i] != NULL) m_pChild[i]->_FrustumCull();
	}
}

void QuadTree::_Destroy()
{
	for (int i = 0; i < 4; ++i) SAFE_DELETE(m_pChild[i]);
}

QuadTree::QuadTree(int x, int y)
{
	Init();

	m_Corner[CORNER_TR] = 0;
	m_Corner[CORNER_TL] = x - 1;
	m_Corner[CORNER_BR] = x * (y - 1);
	m_Corner[CORNER_BL] = x * y - 1;

	m_Center = (x / 2) - 1 + ((y / 2) - 1) * x;

	m_TotalSizeX = x;
}

QuadTree::QuadTree(QuadTree* pParent)
{
	Init();
}

QuadTree::~QuadTree()
{
	_Destroy();
}

void QuadTree::Init()
{
	for (int i = 0; i < 4; ++i)
	{
		m_pChild[i] = NULL;
		m_Corner[i] = 0;
	}

	m_Center = 0;
	m_Culled = false;
	m_Radius = 0.0f;
}

bool QuadTree::Build()
{
	// 우측상단과 좌측하단사이의 벡터 및 거리 계산
	D3DXVECTOR3 v;
	v = (*m_MapVertex)[(*m_pIndex)[m_Corner[CORNER_TR]]].p - (*m_MapVertex)[(*m_pIndex)[m_Corner[CORNER_BL]] + 7].p;

	m_Radius = (D3DXVec3Length(&v) / 2.0f) * 2.5f;
	if (_SubDivide())
	{
		m_pChild[CORNER_TR]->Build();
		m_pChild[CORNER_TL]->Build();
		m_pChild[CORNER_BR]->Build();
		m_pChild[CORNER_BL]->Build();
	}
	return true;
}

int QuadTree::GenerateIndex(vector<DWORD>& pIndex)
{
	_FrustumCull();
	return _GenTriIndex(0, pIndex);
}

QuadTree* QuadTree::_AddChild(int CornerTR, int CornerTL, int CornerBR, int CornerBL)
{
	QuadTree* pChild;

	pChild = new QuadTree(this);
	pChild->m_TotalSizeX = m_TotalSizeX;
	pChild->SetMapVertex(m_MapVertex);
	pChild->SetIndex(m_pIndex);
	pChild->_SetCorners(CornerTR, CornerTL, CornerBR, CornerBL);

	return pChild;
}

bool QuadTree::_SetCorners(int CornerTR, int CornerTL, int CornerBR, int CornerBL)
{
	m_Corner[CORNER_TR] = CornerTR;
	m_Corner[CORNER_TL] = CornerTL;
	m_Corner[CORNER_BR] = CornerBR;
	m_Corner[CORNER_BL] = CornerBL;

	UINT SizeX = m_Corner[CORNER_TL] - m_Corner[CORNER_TR];

	if (CornerTL - CornerTR != 0)
	{
		m_Center = m_Corner[CORNER_TR] + (SizeX / 2) + (SizeX / 2) * m_TotalSizeX;
		//m_Center = ((m_Corner[CORNER_BR] - SizeX) / 2 + m_Corner[CORNER_BL] / 2) / 2;
		//m_Center = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL] + m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 4;
	}
	else
		m_Center = (*m_pIndex)[m_Corner[CORNER_TR]] - 1;

	return true;
}

bool QuadTree::_SubDivide()
{
	UINT TopEdgeCenter;
	UINT BottomEdgeCenter;
	UINT RightEdgeCenter;
	UINT LeftEdgeCenter;
	UINT CentralPoint;
	UINT SizeX = m_Corner[CORNER_TL] - m_Corner[CORNER_TR] + 1;

	if (SizeX > 2)
	{
		TopEdgeCenter = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL]) / 2;
		BottomEdgeCenter = (m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 2;
		RightEdgeCenter = (m_Corner[CORNER_BR] - (m_TotalSizeX * (SizeX / 2)));
		LeftEdgeCenter = RightEdgeCenter + SizeX - 1;
		CentralPoint = (RightEdgeCenter + LeftEdgeCenter) / 2;
	}

	// 더 이상 분할 할 수 없는 경우 종료
	if (m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= 0) return false;

	// Child Node 추가
	if (SizeX > 2)
	{
		m_pChild[CORNER_TR] = _AddChild(m_Corner[CORNER_TR], TopEdgeCenter, RightEdgeCenter, CentralPoint);
		m_pChild[CORNER_TL] = _AddChild(TopEdgeCenter + 1, m_Corner[CORNER_TL], CentralPoint + 1, LeftEdgeCenter);
		m_pChild[CORNER_BR] = _AddChild(RightEdgeCenter + m_TotalSizeX, CentralPoint + m_TotalSizeX, m_Corner[CORNER_BR], BottomEdgeCenter);
		m_pChild[CORNER_BL] = _AddChild(CentralPoint + m_TotalSizeX + 1, LeftEdgeCenter + m_TotalSizeX, BottomEdgeCenter + 1, m_Corner[CORNER_BL]);
	}
	else
	{
		m_pChild[CORNER_TR] = _AddChild(m_Corner[CORNER_TR], m_Corner[CORNER_TR], m_Corner[CORNER_TR], m_Corner[CORNER_TR]);
		m_pChild[CORNER_TL] = _AddChild(m_Corner[CORNER_TL], m_Corner[CORNER_TL], m_Corner[CORNER_TL], m_Corner[CORNER_TL]);
		m_pChild[CORNER_BR] = _AddChild(m_Corner[CORNER_BR], m_Corner[CORNER_BR], m_Corner[CORNER_BR], m_Corner[CORNER_BR]);
		m_pChild[CORNER_BL] = _AddChild(m_Corner[CORNER_BL], m_Corner[CORNER_BL], m_Corner[CORNER_BL], m_Corner[CORNER_BL]);
	}
	return true;
}
