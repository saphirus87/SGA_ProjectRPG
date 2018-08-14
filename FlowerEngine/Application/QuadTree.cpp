#include "stdafx.h"
#include "QuadTree.h"


int QuadTree::_GenTriIndex(int Triangles, vector<DWORD>& pIndex)
{
	if (m_Culled)
	{
		m_Culled = false;
		return Triangles;
	}

	int LODLevel = _GetLODLevel();

	// 현재 노드가 출력되어야 하는 경우 인덱스 추가
	if (_IsVisible(LODLevel))
	{
		// 와우 기준 하나의 사각형이 4개의 삼각형으로 구성됨
		// 2018. 08. 11. - 정점 최적화로 삼각형 2개로 사각형 1개 구성

		if (m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= 1)
		{
			DWORD v1Idx = (*m_pIndex)[m_Corner[CORNER_TR]];
			DWORD v2Idx = (*m_pIndex)[m_Corner[CORNER_BR]];
			DWORD v3Idx = (*m_pIndex)[m_Corner[CORNER_TL]];
			DWORD v4Idx = (*m_pIndex)[m_Corner[CORNER_BL]];

			pIndex[Triangles * 3] = v4Idx;
			pIndex[Triangles * 3 + 1] = v3Idx;
			pIndex[Triangles * 3 + 2] = v1Idx;
			++Triangles;

			pIndex[Triangles * 3] = v4Idx;
			pIndex[Triangles * 3 + 1] = v1Idx;
			pIndex[Triangles * 3 + 2] = v2Idx;
			++Triangles;

			return Triangles;
		}

		bool visible[4];

		if (m_pNeighbor[EDGE_UP]) visible[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible();
		if (m_pNeighbor[EDGE_DN]) visible[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible();
		if (m_pNeighbor[EDGE_RT]) visible[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible();
		if (m_pNeighbor[EDGE_LT]) visible[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible();

		// 4개의 이웃노드 모두 출력 가능하면 같은 LOD Level 이므로 분할할 필요 없음
		if (visible[EDGE_UP] && visible[EDGE_DN] && visible[EDGE_RT] && visible[EDGE_LT])
		{
			DWORD v1Idx = (*m_pIndex)[m_Corner[CORNER_TR]];
			DWORD v2Idx = (*m_pIndex)[m_Corner[CORNER_BR]];
			DWORD v3Idx = (*m_pIndex)[m_Corner[CORNER_TL]];
			DWORD v4Idx = (*m_pIndex)[m_Corner[CORNER_BL]];

			pIndex[Triangles * 3] = v4Idx;
			pIndex[Triangles * 3 + 1] = v3Idx;
			pIndex[Triangles * 3 + 2] = v1Idx;
			++Triangles;

			pIndex[Triangles * 3] = v4Idx;
			pIndex[Triangles * 3 + 1] = v1Idx;
			pIndex[Triangles * 3 + 2] = v2Idx;
			++Triangles;

			return Triangles;
		}

		int n = 0;

		if (!visible[EDGE_UP])
		{
			n = (m_Corner[CORNER_TL] + m_Corner[CORNER_TR]) / 2;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_TL]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[n];
			++Triangles;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[n];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_TR]];
			++Triangles;
		}
		else
		{
			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_TL]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_TR]];
			++Triangles;
		}

		if (!visible[EDGE_DN])
		{
			n = (m_Corner[CORNER_BL] + m_Corner[CORNER_BR]) / 2;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[n];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_BL]];
			++Triangles;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_BR]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[n];
			++Triangles;
		}
		else
		{
			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_BR]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_BL]];
			++Triangles;
		}

		if (!visible[EDGE_RT])
		{
			n = (m_Corner[CORNER_BR] + m_Corner[CORNER_TR]) / 2;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_TR]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[n];
			++Triangles;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[n];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_BR]];
			++Triangles;
		}
		else
		{
			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_TR]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_BR]];
			++Triangles;
		}

		if (!visible[EDGE_LT])
		{
			n = (m_Corner[CORNER_BL] + m_Corner[CORNER_TL]) / 2;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[n];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_TL]];
			++Triangles;

			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_BL]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[n];
			++Triangles;
		}
		else
		{
			pIndex[Triangles * 3] = (*m_pIndex)[m_Center];
			pIndex[Triangles * 3 + 1] = (*m_pIndex)[m_Corner[CORNER_BL]];
			pIndex[Triangles * 3 + 2] = (*m_pIndex)[m_Corner[CORNER_TL]];
			++Triangles;
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

void QuadTree::_AllInFrustum()
{
	m_Culled = false;
	if (!m_pChild[CORNER_TR]) return;
	
	m_pChild[CORNER_TR]->_AllInFrustum();
	m_pChild[CORNER_TL]->_AllInFrustum();
	m_pChild[CORNER_BR]->_AllInFrustum();
	m_pChild[CORNER_BL]->_AllInFrustum();
}

int QuadTree::_IsInFrustum()
{
	bool InPoint[4];
	bool InSphere;
	int nodeSize = m_Corner[CORNER_TL] - m_Corner[CORNER_TR];

	//int LODLevel = _GetLODLevel();

	if (nodeSize == 1)
	{
		Vector3 CenterPos = (*m_MapVertex)[(*m_pIndex)[m_Corner[CORNER_TR]]].p + (*m_MapVertex)[(*m_pIndex)[m_Corner[CORNER_BL]]].p;
		CenterPos /= 2.0f;
		InSphere = Camera::GetInstance()->FrustumCulling(&CenterPos, m_Radius);
	}
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
	int LODLevel = _GetLODLevel();

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

	//if (_IsVisible(LODLevel)) return;

	for (int i = 0; i < 4; ++i)
	{
		if (m_pChild[i] != NULL) m_pChild[i]->_FrustumCull();
	}
}

float QuadTree::_GetDistance(Vector3 & CenterPos)
{
	Vector3 v(CenterPos - Camera::GetInstance()->GetPosition());
	return D3DXVec3Length(&Vector3(v.x, 0.0f, v.z));
}

int QuadTree::_GetLODLevel()
{
	float distance = 0.0f;
	float LODRatio = 0.015f;	// 거리값에 따른 LOD 레벨 설정을 위한 변수

	distance = _GetDistance((*m_MapVertex)[(*m_pIndex)[m_Center]].p);

	return max((int)(distance * LODRatio), 1);
}

void QuadTree::_BuildNeighborNode(QuadTree * pRoot)
{
	int center = -1;
	int tr, tl, br, bl;

	for (int i = 0; i < 4; ++i)
	{
		tr = m_Corner[CORNER_TR];
		tl = m_Corner[CORNER_TL];
		br = m_Corner[CORNER_BR];
		bl = m_Corner[CORNER_BL];

		center = _GetNodeIndex(i, tr, tl, br, bl);
		if (center >= 0)
			m_pNeighbor[i] = pRoot->_FindNode(tr, tl, br, bl);
	}

	if (m_pChild[CORNER_TR])
	{
		m_pChild[CORNER_TR]->_BuildNeighborNode(pRoot);
		m_pChild[CORNER_TL]->_BuildNeighborNode(pRoot);
		m_pChild[CORNER_BR]->_BuildNeighborNode(pRoot);
		m_pChild[CORNER_BL]->_BuildNeighborNode(pRoot);
	}
}

bool QuadTree::_BuildQuadTree()
{
	// 우측상단과 좌측하단사이의 벡터 및 거리 계산
	D3DXVECTOR3 v;
	DWORD trIndex = (*m_pIndex)[m_Corner[CORNER_TR]];
	DWORD blIndex = (*m_pIndex)[m_Corner[CORNER_BL]];
	v = (*m_MapVertex)[trIndex].p - (*m_MapVertex)[blIndex].p;

	m_Radius = (D3DXVec3Length(&v) / 2.0f);
	if (_SubDivide())
	{
		m_pChild[CORNER_TR]->_BuildQuadTree();
		m_pChild[CORNER_TL]->_BuildQuadTree();
		m_pChild[CORNER_BR]->_BuildQuadTree();
		m_pChild[CORNER_BL]->_BuildQuadTree();
	}
	return true;
}

QuadTree* QuadTree::_FindNode(int tr, int tl, int br, int bl)
{
	QuadTree* p;

	if (m_Corner	[CORNER_TR] == tr && m_Corner[CORNER_TL] == tl
		&& m_Corner[CORNER_BR] == br && m_Corner[CORNER_BL] == bl) return this;

	if (m_pChild[0])
	{
		RECT rc;
		POINT pt;
		int n = (tr + tl + br + bl) / 4;

		// 
		pt.x = (int)(*m_MapVertex)[(*m_pIndex)[n]].p.x;
		pt.y = (int)(*m_MapVertex)[(*m_pIndex)[n]].p.z;

		// 자식노드 영역을 rc에 설정
		SetRect(&rc, (*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TR]->m_Corner[CORNER_BL]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TR]->m_Corner[CORNER_BL]]].p.z,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TR]->m_Corner[CORNER_TR]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TR]->m_Corner[CORNER_TR]]].p.z);
		// pt가 자식노드 안에 있으면 자식노드에서 찾음
		if (PtInRect(&rc, pt))
			return m_pChild[CORNER_TR]->_FindNode(tr, tl, br, bl);

		SetRect(&rc, (*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TL]->m_Corner[CORNER_BL]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TL]->m_Corner[CORNER_BL]]].p.z,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TL]->m_Corner[CORNER_TR]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_TL]->m_Corner[CORNER_TR]]].p.z);
		if (PtInRect(&rc, pt))
			return m_pChild[CORNER_TL]->_FindNode(tr, tl, br, bl);

		SetRect(&rc, (*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BR]->m_Corner[CORNER_BL]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BR]->m_Corner[CORNER_BL]]].p.z,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BR]->m_Corner[CORNER_TR]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BR]->m_Corner[CORNER_TR]]].p.z);
		if (PtInRect(&rc, pt))
			return m_pChild[CORNER_BR]->_FindNode(tr, tl, br, bl);

		SetRect(&rc, (*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BL]->m_Corner[CORNER_BL]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BL]->m_Corner[CORNER_BL]]].p.z,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BL]->m_Corner[CORNER_TR]]].p.x,
			(*m_MapVertex)[(*m_pIndex)[m_pChild[CORNER_BL]->m_Corner[CORNER_TR]]].p.z);
		if (PtInRect(&rc, pt))
			return m_pChild[CORNER_BL]->_FindNode(tr, tl, br, bl);
	}

	return NULL;
}

int QuadTree::_GetNodeIndex(int edge, int & tr, int & tl, int & br, int & bl)
{
	int center = 0;
	int _orgTr, _orgTl, _orgBr, _orgBl;
	_orgTr = tr;
	_orgTl = tl;
	_orgBr = br;
	_orgBl = bl;

	int gap = tl - tr;

	switch (edge)
	{
	case QuadTree::EDGE_UP:
		tr = _orgTr - gap * m_TotalSizeX;
		tl = _orgTl - gap * m_TotalSizeX;
		br = _orgTr;
		bl = _orgTl;
		break;
	case QuadTree::EDGE_DN:
		tr = _orgBr;
		tl = _orgBl;
		br = _orgBr + gap * m_TotalSizeX;
		bl = _orgBl + gap * m_TotalSizeX;
		break;
	case QuadTree::EDGE_RT:
		tr = _orgTr - gap;
		tl = _orgTr;
		br = _orgBr - gap;
		bl = _orgBr;
		if ((tr / m_TotalSizeX) != (_orgTr / m_TotalSizeX)) return -1;
		if ((tl / m_TotalSizeX) != (_orgTl / m_TotalSizeX)) return -1;
		break;
	case QuadTree::EDGE_LT:
		tr = _orgTl;
		tl = _orgTl + gap;
		br = _orgBl;
		bl = _orgBl + gap;
		if ((tr / m_TotalSizeX) != (_orgTr / m_TotalSizeX)) return -1;
		if ((tl / m_TotalSizeX) != (_orgTl / m_TotalSizeX)) return -1;
		break;
	default:
		break;
	}

	// 이웃 노드의 중점
	center = (tr + tl + br + bl) / 4;
	// 중점이 범위 밖으로 벗어나면 -1 리턴
	if (center < 0 || center >= (m_TotalSizeX * m_TotalSizeX)) return -1;

	return center;
}

void QuadTree::_Destroy()
{
	for (int i = 0; i < 4; ++i) SAFE_DELETE(m_pChild[i]);
}

QuadTree::QuadTree(int x, int y)
{
	Init();

	m_pParent = NULL;

	m_Corner[CORNER_TR] = 0;
	m_Corner[CORNER_TL] = x - 1;
	m_Corner[CORNER_BR] = x * (y - 1);
	m_Corner[CORNER_BL] = x * y - 1;

	m_Center = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL] + m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 4;

	m_TotalSizeX = x;
}

QuadTree::QuadTree(QuadTree* pParent)
{
	Init();

	m_pParent = pParent;
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
		m_pNeighbor[i] = NULL;
		m_Corner[i] = 0;
	}

	m_Center = 0;
	m_Culled = false;
	m_Radius = 0.0f;
}

bool QuadTree::Build()
{
	_BuildQuadTree();
	_BuildNeighborNode(this);

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

	m_Center = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL] + m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 4;

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

	TopEdgeCenter = (m_Corner[CORNER_TR] + m_Corner[CORNER_TL]) / 2;
	BottomEdgeCenter = (m_Corner[CORNER_BR] + m_Corner[CORNER_BL]) / 2;
	RightEdgeCenter = (m_Corner[CORNER_BR] + m_Corner[CORNER_TR]) / 2;
	LeftEdgeCenter = (m_Corner[CORNER_BL] + m_Corner[CORNER_TL]) / 2;
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
