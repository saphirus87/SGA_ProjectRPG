#pragma once
#include "stdafx.h"

class QuadTree
{
	enum CornerType { CORNER_TR, CORNER_TL, CORNER_BR, CORNER_BL };
	enum Edge { EDGE_UP, EDGE_DN, EDGE_RT, EDGE_LT };
	enum QuadLocation { FRUSTUM_OUT, FRUSTUM_PARTIALLY_IN, FRUSTUM_COMPLETELY_IN, FRUSTUM_UNKNOWN = -1 };
private:
	QuadTree* m_pChild[4];
	QuadTree* m_pParent;
	QuadTree* m_pNeighbor[4];

	int m_Center;
	int m_Corner[4];

	bool m_Culled;
	float m_Radius;

private:
	vector<VERTEX_PNT>* m_MapVertex;
	vector<DWORD>* m_pIndex;

	UINT m_TotalSizeX;

private:
	// �ڽ� ��� �߰�
	QuadTree * _AddChild(int CornerTR, int CornerTL, int CornerBR, int CornerBL);

	// 4���� �ڳʰ� ����
	bool _SetCorners(int CornerTR, int CornerTL, int CornerBR, int CornerBL);

	// ���� Ʈ���� 4���� ���� Ʈ���� ����
	bool _SubDivide();

	// LOD Level�� ���� ���� ���� �׸��� �����ϱ� ���� �Լ�
	bool _IsVisible(int LODLevel) { return (m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= LODLevel); }

	// ����� �������� �ε��� ����
	int _GenTriIndex(int Triangles, vector<DWORD>& pIndex);

	// ��� �ڽĳ���� m_pCulled ���� false�� �ʱ�ȭ
	void _AllInFrustum();
	// ����ü �ȿ� ���� ��尡 ���� �Ǿ����� ���� Ȯ��
	int _IsInFrustum();
	// ����ü �ø�
	void _FrustumCull();

	// LOD(Level Of Detail)
	float _GetDistance(Vector3& CenterPos);
	int _GetLODLevel();

	// �̿���� ����
	void _BuildNeighborNode(QuadTree* pRoot);
	// ����Ʈ�� ����
	bool _BuildQuadTree();
	// ����Ʈ���� �˻��ؼ� 4���� �ڳʰ��� ��ġ�ϴ� ��带 Ž��
	QuadTree* _FindNode(int tr, int tl, int br, int bl);
	// �����¿� �̿������ �ε����� ���Ѵ�.
	int _GetNodeIndex(int edge, int& tr, int& tl, int& br, int& bl);


	// ���� Ʈ�� ����
	void _Destroy();
public:
	QuadTree(int x, int y);
	QuadTree(QuadTree* pParent);
	~QuadTree();

	// ����Ʈ�� �ʱ�ȭ
	void Init();

	// ���� Ʈ�� ����
	bool Build();

	// �ﰢ���� �ε����� �����, ����� �ﰢ���� ���� ��ȯ
	int GenerateIndex(vector<DWORD>& pIndex);

	void SetMapVertex(vector<VERTEX_PNT>* vertex) { m_MapVertex = vertex; }
	void SetIndex(vector<DWORD>* index) { m_pIndex = index; }

	// ���� ��尡 �ø��� �������
	bool IsCulled() { return m_Culled; }
	// ���� ��尡 LOD Level�� �� �� �׷����� �ϴ� �������
	bool IsVisible() { return m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= _GetLODLevel(); };
	// ���� ����� �ڳʰ��� ��� �´�.
	void GetCorner(int& tr, int& tl, int& br, int& bl)
	{
		tr = m_Corner[CORNER_TR]; tl = m_Corner[CORNER_TL];
		br = m_Corner[CORNER_BR]; bl = m_Corner[CORNER_BL];
	}
};

