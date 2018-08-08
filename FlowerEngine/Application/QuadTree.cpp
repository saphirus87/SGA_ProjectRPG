#include "stdafx.h"
#include "QuadTree.h"


int QuadTree::_GenTriIndex(int Triangles, vector<DWORD>& pIndex)
{
	if (m_Culled)
	{
		m_Culled = false;
		return Triangles;
	}

	// ���� ��尡 ��µǾ�� �ϴ� ��� �ε��� �߰�
	if (_IsVisible())
	{
		/*LPDWORD p = ((LPDWORD)pIndex) + Triangles * 3;

		// �Ϳ� ���� �ϳ��� �簢���� 4���� �ﰢ������ ������
		 
		// ���� �ﰢ��
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TR]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BR]];
		++Triangles;

		// ��� �ﰢ��
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TL]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TR]];
		++Triangles;

		// ���� �ﰢ��
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_TL]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BL]];
		++Triangles;

		// �ϴ� �ﰢ��
		*p++ = (*m_pIndex)[m_Center];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BL]];
		*p++ = (*m_pIndex)[m_Corner[CORNER_BR]];
		++Triangles;*/

		// �Ϳ� ���� �ϳ��� �簢���� 4���� �ﰢ������ ������

		// ���� �ﰢ��
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TR]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BR]]);
		++Triangles;

		// ��� �ﰢ��
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TL]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TR]]);
		++Triangles;

		// ���� �ﰢ��
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_TL]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BL]]);
		++Triangles;

		// �ϴ� �ﰢ��
		pIndex.push_back(m_Center);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BL]]);
		pIndex.push_back((*m_pIndex)[m_Corner[CORNER_BR]]);
		++Triangles;

		return Triangles;
	}

	// �ڽ� ��� �˻�
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
	// ����ü �ȿ� ���� ���Ե��� ������ �ٷ� �Լ� ����(����Ʈ�� üũ ���ʿ�)
	if (!InSphere) return FRUSTUM_OUT;

	for (int i = 0; i < 4; ++i)
	{
		InPoint[i] = Camera::GetInstance()->FrustumCulling(&(*m_MapVertex)[(*m_pIndex)[m_Corner[i]]].p);
	}

	// 4�� ������ ��� ����ü �ȿ� ����
	if ((InPoint[0] + InPoint[1] + InPoint[2] + InPoint[3]) == 4) return FRUSTUM_COMPLETELY_IN;

	// 4�� ���� �� �Ϻθ� ����ü �ȿ� ����
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::_FrustumCull()
{
	int result = _IsInFrustum();

	switch (result)
	{
	case QuadTree::FRUSTUM_OUT:	// ����ü�� ���Ե��� ����(���� ��� Ž�� ���ʿ�)
		m_Culled = true;
		return;
	case QuadTree::FRUSTUM_PARTIALLY_IN:	// ����ü�� �Ϻκ��� ����(���� ��� Ž�� �ʿ�)
		m_Culled = false;
		break;
	case QuadTree::FRUSTUM_COMPLETELY_IN:	// ����ü�� ��ü ����(���� ��� Ž�� ���ʿ�)
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
	// ������ܰ� �����ϴܻ����� ���� �� �Ÿ� ���
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

	// �� �̻� ���� �� �� ���� ��� ����
	if (m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= 1) return false;

	// Child Node �߰�
	m_pChild[CORNER_TR] = _AddChild(m_Corner[CORNER_TR], TopEdgeCenter, RightEdgeCenter, CentralPoint);
	m_pChild[CORNER_TL] = _AddChild(TopEdgeCenter, m_Corner[CORNER_TL], CentralPoint, LeftEdgeCenter);
	m_pChild[CORNER_BR] = _AddChild(RightEdgeCenter, CentralPoint, m_Corner[CORNER_BR], BottomEdgeCenter);
	m_pChild[CORNER_BL] = _AddChild(CentralPoint, LeftEdgeCenter, BottomEdgeCenter, m_Corner[CORNER_BL]);

	return true;
}
