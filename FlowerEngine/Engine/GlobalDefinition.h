/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 서울게임 아카데미 김주안 선생님 수업시간에 배운 코드에 추가하거나 수정하였습니다.

설명 :
공통적으로 사용하는 것들의 선언, 정의, 함수들
*/
#pragma once
#include "stdafx.h"

typedef D3DXVECTOR4						Vector4;
typedef D3DXVECTOR3						Vector3;
typedef D3DXVECTOR2						Vector2;
typedef D3DXMATRIXA16					Matrix4x4;
typedef LPDIRECT3DTEXTURE9				Texture;
typedef LPD3DXSPRITE					Sprite;
typedef LPDIRECT3DSURFACE9				Surface;
typedef LPD3DXEFFECT					Shader;
typedef D3DXCOLOR						Color;
typedef D3DMATERIAL9					Material;
typedef LPDIRECT3DVERTEXBUFFER9			VertexBuffer;
typedef LPDIRECT3DINDEXBUFFER9			IndexBuffer;
typedef LPDIRECT3DVERTEXDECLARATION9	VertexDecl;
typedef LPDIRECT3DDEVICE9				Device9;
typedef LPD3DXFONT						Font;
typedef LPD3DXMESH						Mesh;
typedef LPD3DXANIMATIONCONTROLLER		AnimationController;
typedef LPD3DXANIMATIONSET				AnimationSet;
typedef LPD3DXMESHCONTAINER				XMeshContainer;
typedef LPD3DXFRAME						XFrame;
typedef D3DXIMAGE_INFO					ImgInfo;

extern CString ASSET_PATH;
extern CString SHADER_PATH;

extern vector<Vector3> g_vecCubeVertex;
extern vector<WORD> g_vecCubeIndex;

extern vector<Vector3> g_vecQuadVertex;
extern vector<WORD> g_vecQuadIndex;
extern vector<Vector2> g_vecQuadUV;

#define WHITE	Color(1.0f, 1.0f, 1.0f, 1.0f)
#define BLACK	Color(0.0f, 0.0f, 0.0f, 1.0f)
#define RED		Color(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN	Color(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE	Color(0.0f, 0.0f, 1.0f, 1.0f)
#define YELLOW	Color(1.0f, 1.0f, 0.0f, 1.0f)

namespace DXUtil
{
	D3DMATERIAL9 InitMtrl(Color a, Color d, Color s, Color e, float p);

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 8.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 8.0f);

	void ComputeNormal(Vector3* out, Vector3* p0, Vector3* p1, Vector3* p2);

	D3DLIGHT9 InitDirectional(Vector3* dir, Color* c);
	D3DLIGHT9 InitPoint(Vector3* pos, Color* c);
	D3DLIGHT9 InitSpot(Vector3* dir, Vector3* pos, Color* c);

	void InitVertexBuffer(VertexBuffer &pVB, LPVOID vertices, DWORD fvf, UINT vtxCnt, UINT stride);
	void InitIndexBuffer(IndexBuffer &pIB, LPVOID indices, UINT idxCnt, UINT stride);
}

#define SINGLETON(class_name)\
private: \
	class_name(); \
	~class_name(); \
public:\
	static class_name* GetInstance()\
	{\
		static class_name instance;\
		return &instance;\
	}


#define SAFE_UPDATE(p) if(p) { (p)->Update(); }
#define SAFE_RENDER(p) if(p) { (p)->Render(); }
#define SAFE_WNDPROC(p) if (p) {(p)->WndProc(hWnd, message, wParam, lParam);}
#define SAFE_RELEASE(p) if(p) {(p)->Release(); (p) = NULL;}
#define SAFE_DELETE(p) if(p) {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p) {delete [] (p); (p) = NULL;}
#define SAFE_ADDREF(p) if(p) { (p)->AddRef(); }

#define VARIATION(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

enum SCENE_INDEX {
	SCENE_SELECT,
	SCENE_GRID,
	SCENE_GAME_BOXMOVE,
	SCENE_GAME_AIRCRAFT,
	SCENE_OBJ,
	SCENE_HEIGHTMAP,
	SCENE_TEST,
};

enum TAG_DISPLAYOBJECT {
	TAG_PLAYER,
	TAG_MAP
};

enum DIR
{
	eDir_Up,	// 북
	eDir_Right,	// 동
	eDir_Down,	// 남
	eDir_Left,	// 서
};

enum eCellType
{
	eCellType_None,
	eCellType_Cube,		// 큐브가 놓여있는 상태
	eCellType_Wall,		// 벽이라서 갈 수 없음(예외범위 지정용)
	eCellType_Goal,		// 목표 지점
	eCellType_CubeGoal	// 목표 지점에 도착한 큐브
};

enum eAni
{
	eAni_Skill_2,
	eAni_Skill_1,
	eAni_Attack_1,
	eAni_Walk,
	eAni_Stand,			// Export 시 거꾸로 됨
	eAni_COUNT
};

enum eAniMon
{
	eAniMon_Death,
	eAniMon_Attack_1,
	eAniMon_Walk,
	eAniMon_Stand,
	eAniMon_COUNT
};

enum eChrType
{
	eChrType_Human,
	eChrType_Undead,
	eChrType_Troll,
	eChrType_COUNT
};

enum eTag
{
	eTag_Chracter,
	eTag_Monster,
	eTag_Item,		// 맵에 떨궈져 있는 아이템
};

enum eUI
{
	eUI_Loading,
	eUI_LoadingBar,
	eUI_LoadingBarFrame,
	eUI_StartBtn,
	eUI_StartText,
	eUI_HPBar_Human,
	eUI_MPBar_Human,
	eUI_HPBar_Undead,
	eUI_HPBar_Troll,
	eUI_SkillBtn1_Human,
	eUI_SkillBtn1_Human_TextCoolTime,
	eUI_SkillBtn2_Human,
	eUI_SkillBtn3_Human,
	eUI_Human_Img_Face,
	eUI_Human_Text_LV,
	eUI_Human_Text_EXP,

	eUI_SkillBtn1_Undead,
	eUI_SkillBtn2_Undead,
	eUI_SkillBtn3_Undead,

	eUI_SkillBtn1_Troll,
	eUI_SkillBtn2_Troll,
	eUI_SkillBtn3_Troll,
};

enum eInvenUI
{
	eInvenUI_CopperCoinImg = 101,
	eInvenUI_SilverCoinImg,
	eInvenUI_GoldCoinImg,
	eInvenUI_CopperCoinText = 111,
	eInvenUI_SilverCoinText,
	eInvenUI_GoldCoinText,
	eInvenUI_Slot1Btn = 1001,
	eInvenUI_Slot2Btn,
	eInvenUI_Slot3Btn,
	eInvenUI_Slot4Btn,
	eInvenUI_Slot5Btn,
	eInvenUI_Slot6Btn,
	eInvenUI_Slot7Btn,
	eInvenUI_Slot8Btn,
	eInvenUI_Slot9Btn,
	eInvenUI_Slot10Btn,
	eInvenUI_Slot11Btn,
	eInvenUI_Slot12Btn,
	eInvenUI_Slot13Btn,
	eInvenUI_Slot14Btn,
	eInvenUI_Slot15Btn,
	eInvenUI_Slot16Btn,
	eInvenUI_Slot17Btn,
	eInvenUI_Slot18Btn,
	eInvenUI_Slot19Btn,
	eInvenUI_Slot20Btn,
	eInvenUI_Slot1NumText = 1101,
	eInvenUI_Slot2NumText,
	eInvenUI_Slot3NumText,
	eInvenUI_Slot4NumText,
	eInvenUI_Slot5NumText,
	eInvenUI_Slot6NumText,
	eInvenUI_Slot7NumText,
	eInvenUI_Slot8NumText,
	eInvenUI_Slot9NumText,
	eInvenUI_Slot10NumText,
	eInvenUI_Slot11NumText,
	eInvenUI_Slot12NumText,
	eInvenUI_Slot13NumText,
	eInvenUI_Slot14NumText,
	eInvenUI_Slot15NumText,
	eInvenUI_Slot16NumText,
	eInvenUI_Slot17NumText,
	eInvenUI_Slot18NumText,
	eInvenUI_Slot19NumText,
	eInvenUI_Slot20NumText,
	eInvenUI_PickedItemImg = 2001,
	eInvenUI_PickedItemNumText = 2101
};

enum eEquipUI
{
	eEquipUI_HelmetBtn = 1001,
	eEquipUI_NecklaceBtn,
	eEquipUI_ShoulderBtn,
	eEquipUI_CloakBtn,
	eEquipUI_ChestBtn,
	eEquipUI_ShirtBtn,
	eEquipUI_TabardBtn,
	eEquipUI_WristBtn,
	eEquipUI_HandBtn,
	eEquipUI_WaistBtn,
	eEquipUI_LegBtn,
	eEquipUI_FootBtn,
	eEquipUI_Finger1Btn,
	eEquipUI_Finger2Btn,
	eEquipUI_Trinket1Btn,
	eEquipUI_Trinket2Btn,
	eEquipUI_MainHandBtn,
	eEquipUI_SecondaryHandBtn,
	eEquipUI_RelicBtn,
};

enum eItemInfoUI
{
	eItemInfoUI_Border,
	eItemInfoUI_BorderTopLeft,
	eItemInfoUI_BorderTop,
	eItemInfoUI_BorderTopRight,
	eItemInfoUI_BorderLeft,
	eItemInfoUI_BorderRight,
	eItemInfoUI_BorderBottomLeft,
	eItemInfoUI_BorderBottom,
	eItemInfoUI_BorderBottomRight,
	eItemInfoUI_ItemNameText,
};

struct KEYBOARD_STATE
{
	Vector3	deltaPos;
	Vector3	deltaRot;
	bool		bJump;
};


struct VERTEX_PC
{
	Vector3 p;
	D3DCOLOR	c;

	VERTEX_PC() {}
	VERTEX_PC(Vector3 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
};

struct VERTEX_PT
{
	Vector3 p;
	Vector2 t;

	VERTEX_PT() {}
	VERTEX_PT(Vector3 _p, Vector2 _t) : p(_p), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1, };
};

struct VERTEX_PCT
{
	Vector3 p;
	D3DCOLOR	c;
	Vector2 t;
	Vector2 t1;

	VERTEX_PCT() {}
	VERTEX_PCT(Vector3 _p, D3DCOLOR _c, Vector2 _t) : p(_p), c(_c), t(_t), t1(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2, };
};

struct VERTEX_RHWC
{
	VERTEX_RHWC() {}
	VERTEX_RHWC(D3DXVECTOR4 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	D3DXVECTOR4 p;
	D3DCOLOR	c;
	
	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE, };
};

struct VERTEX_PN
{
	Vector3 p;
	Vector3	n;

	VERTEX_PN() {}
	VERTEX_PN(Vector3 _p, Vector3 _n) : p(_p), n(_n) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct VERTEX_PNT
{
	Vector3 p;
	Vector3	n;
	Vector2	t;

	VERTEX_PNT() {}
	VERTEX_PNT(Vector3 _p, Vector3 _n, Vector2 _t) : p(_p), n(_n), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, };
};

struct VERTEX_BLENDING
{
	Vector3 p;
	float		weights[3];
	DWORD		index;
	//Color	c;
	Vector2	t;

	VERTEX_BLENDING() {}
	VERTEX_BLENDING(Vector3 _p, float* _weights, DWORD _i, Color _c, Vector2 _t)
		: p(_p), index(_i), t(_t) {
		weights[0] = _weights[0];
		weights[1] = _weights[1];
		weights[2] = _weights[2];
	}

	enum { FVF = D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_TEX1 };
};


struct MTLTEX
{
	D3DMATERIAL9		material;
	LPDIRECT3DTEXTURE9	pTexture;
	int					id;

	MTLTEX()
	{
		ZeroMemory(&material, sizeof(D3DMATERIAL9)); pTexture = NULL;
		material = DXUtil::WHITE_MTRL;
		pTexture = NULL;
	}
	MTLTEX(D3DMATERIAL9& _mtl, LPDIRECT3DTEXTURE9 _pTex) : material(_mtl), pTexture(_pTex) {}
};

struct ASE_POS_SAMPLE
{
	int				tick;
	Vector3		position;
};

struct ASE_ROT_SAMPLE
{
	int				tick;
	D3DXQUATERNION	quaternion;
};

struct ASE_SCENE
{
	int numFirstFrame;
	int numLastFrame;
	int FrameSpeed;
	int TicksPerFrame;
};


struct BoundingSphere
{
	Vector3 center;
	float		radius;
	bool		isPicked;

	BoundingSphere():isPicked(false) {}
	BoundingSphere(Vector3 _center, float _radius):center(_center), radius(_radius), isPicked(false) {}
};

class Cube;
struct BoundingBox
{
	Cube* pCube; // 이 바운딩 박스의 큐브
	
	BoundingBox():minPos(Vector3(-1, -1, -1)), maxPos(Vector3(1, 1, 1)){ }
	BoundingBox(Vector3 min, Vector3 max):minPos(min), maxPos(max) {}
	Vector3 minPos;
	Vector3 maxPos;
	Vector3 oriPos;

	Vector3 GetMinPos() { return Vector3(oriPos.x + minPos.x, oriPos.y + minPos.y, oriPos.z + minPos.z); }
	Vector3 GetMaxPos() { return Vector3(oriPos.x + maxPos.x, oriPos.y + maxPos.y, oriPos.z + maxPos.z); }

	bool IsIntersected(BoundingBox &box)
	{
		if (GetMinPos().x > box.GetMaxPos().x || GetMaxPos().x < box.GetMinPos().x) return false;
		if (GetMinPos().y > box.GetMaxPos().y || GetMaxPos().y < box.GetMinPos().y) return false;
		if (GetMinPos().z > box.GetMaxPos().z || GetMaxPos().z < box.GetMinPos().z) return false;
		return true;
	}

	bool IsPointInside(Vector3& p)
	{
		if (GetMinPos().x > p.x || GetMaxPos().x < p.x) return false;
		if (GetMinPos().y > p.y || GetMaxPos().y < p.y) return false;
		if (GetMinPos().z > p.z || GetMaxPos().z < p.z) return false;
		return true;
	}
};

DWORD FtoDw(float f);

float GetRandomFloat(float lowBound, float highBound);
bool CompareStr(const char* str1, const char* str2);

// 보간
float EaseTo(float* out, float start, float end, float speed);
Vector3 EaseTo(Vector3* out, Vector3& start, Vector3& end, float speed);

