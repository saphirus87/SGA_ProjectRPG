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
	// 자식 노드 추가
	QuadTree * _AddChild(int CornerTR, int CornerTL, int CornerBR, int CornerBL);

	// 4개의 코너값 설정
	bool _SetCorners(int CornerTR, int CornerTL, int CornerBR, int CornerBL);

	// 쿼드 트리를 4개의 하위 트리로 분할
	bool _SubDivide();

	// LOD Level에 따라 분할 할지 그릴지 결정하기 위한 함수
	bool _IsVisible(int LODLevel) { return (m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= LODLevel); }

	// 출력할 폴리곤의 인덱스 생성
	int _GenTriIndex(int Triangles, vector<DWORD>& pIndex);

	// 모든 자식노드의 m_pCulled 값을 false로 초기화
	void _AllInFrustum();
	// 절두체 안에 현재 노드가 포함 되었는지 여부 확인
	int _IsInFrustum();
	// 절두체 컬링
	void _FrustumCull();

	// LOD(Level Of Detail)
	float _GetDistance(Vector3& CenterPos);
	int _GetLODLevel();

	// 이웃노드 생성
	void _BuildNeighborNode(QuadTree* pRoot);
	// 쿼드트리 생성
	bool _BuildQuadTree();
	// 쿼드트리를 검색해서 4개의 코너값이 일치하는 노드를 탐색
	QuadTree* _FindNode(int tr, int tl, int br, int bl);
	// 상하좌우 이웃노드의 인덱스를 구한다.
	int _GetNodeIndex(int edge, int& tr, int& tl, int& br, int& bl);


	// 쿼드 트리 삭제
	void _Destroy();
public:
	QuadTree(int x, int y);
	QuadTree(QuadTree* pParent);
	~QuadTree();

	// 쿼드트리 초기화
	void Init();

	// 쿼드 트리 구축
	bool Build();

	// 삼각형의 인덱스를 만들고, 출력할 삼각형의 갯수 반환
	int GenerateIndex(vector<DWORD>& pIndex);

	void SetMapVertex(vector<VERTEX_PNT>* vertex) { m_MapVertex = vertex; }
	void SetIndex(vector<DWORD>* index) { m_pIndex = index; }

	// 현재 노드가 컬링된 노드인지
	bool IsCulled() { return m_Culled; }
	// 현재 노드가 LOD Level로 볼 때 그려져야 하는 노드인지
	bool IsVisible() { return m_Corner[CORNER_TL] - m_Corner[CORNER_TR] <= _GetLODLevel(); };
	// 현재 노드의 코너값을 얻어 온다.
	void GetCorner(int& tr, int& tl, int& br, int& bl)
	{
		tr = m_Corner[CORNER_TR]; tl = m_Corner[CORNER_TL];
		br = m_Corner[CORNER_BR]; bl = m_Corner[CORNER_BL];
	}
};

