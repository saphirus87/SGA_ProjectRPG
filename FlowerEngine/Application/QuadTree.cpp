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
		/*LPDWORD p = ((LPDWORD)pIndex) + Triangles * 3;

		// 와우 기준 하나의 사각형이 4개의 삼각형으로 구성됨
		 
		// 우측 삼각형
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TR]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BR]];
		++Triangles;

		// 상단 삼각형
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TL]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TR]];
		++Triangles;

		// 좌측 삼각형
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TL]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BL]];
		++Triangles;

		// 하단 삼각형
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BL]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BR]];
		++Triangles;*/

		// 와우 기준 하나의 사각형이 4개의 삼각형으로 구성됨

		// 우측 삼각형
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TR]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BR]]);
		++Triangles;

		// 상단 삼각형
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TL]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TR]]);
		++Triangles;

		// 좌측 삼각형
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TL]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BL]]);
		++Triangles;

		// 하단 삼각형
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BL]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BR]]);
		++Triangles;

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
		InSphere = Camera::GetInstance()->FrustumCulling(&(*m_MapVertex)[(*m_pIndex)[m_Center]].p, m_Radius);
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

	m_Center = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL] + m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 4;
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
	v = (*m_MapVertex)[(*m_pIndex)[m_Corner[CORNER_TR]]].p - (*m_MapVertex)[(*m_pIndex)[m_Corner[CORNER_BL]]].p;

	m_Radius = D3DXVec3Length(&v) / 2.0f;
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

	if (CornerTL - CornerTR != 1)
		m_Center = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL] + m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 4;
	else
		m_Center = (*m_pIndex)[m_Corner[CORNER_TL]] - 4;

	return true;
}

bool QuadTree::_SubDivide()
{
	UINT TopEdgeCenter;
	UINT BottomEdgeCenter;
	UINT RightEdgeCenter;
	UINT LeftEdgeCenter;
	UINT CentralPoint;

	TopEdgeCenter = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL]) / 2;
	BottomEdgeCenter = (m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 2;
	RightEdgeCenter = (m_Corner[CORNER_TR] + m_Corner[CORNER_BR]) / 2;
	LeftEdgeCenter = (m_Corner[CORNER_TL] + m_Corner[CORNER_BL]) / 2;
	CentralPoint = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL] + m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 4;

	// 더 이상 분할 할 수 없는 경우 종료
	if (m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= 1) return false;

	// Child Node 추가
	m_pChild[CORNER_TR] = _AddChild(m_Corner[CORNER_TR], TopEdgeCenter, RightEdgeCenter, CentralPoint);
	m_pChild[CORNER_TL] = _AddChild(TopEdgeCenter, m_Corner[CORNER_TL], CentralPoint, LeftEdgeCenter);
	m_pChild[CORNER_BR] = _AddChild(RightEdgeCenter, CentralPoint, m_Corner[CORNER_BR], BottomEdgeCenter);
	m_pChild[CORNER_BL] = _AddChild(CentralPoint, LeftEdgeCenter, BottomEdgeCenter, m_Corner[CORNER_BL]);

	return true;
}
