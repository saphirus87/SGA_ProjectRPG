/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 직접 설계하고 작성한 코드입니다.

설명 :
모든 게임 오브젝트는 위치, 회전, 크기값을 가지고 있습니다.
위치, 회전, 크기의 적용은 행렬로 계산하므로 해당 행렬들이 있습니다.

이 구성요소(Component)는 게임 객체(GameObject)의 위치, 회전, 크기를 위해 존재합니다.
*/
#pragma once
#include "stdafx.h"

class ComTransform : public Component
{
public:
	ComTransform(CString szName);
	~ComTransform();

	inline Vector3& GetPosition() { return m_position; }
	inline Vector3& GetRotation() { return m_rotAngle; }
	inline Vector3& GetScale() { return m_scale; }

	void SetPosition(float fX, float fY, float fZ);
	inline void SetPosition(Vector3& vPos) { m_position = vPos; }
	void SetRotation(float fAngleX, float fAngleY, float fAngleZ);
	inline void SetRotation(Vector3& vRot) { m_rotAngle = vRot; }
	void SetScale(float fScaleX, float fScaleY, float fScaleZ);

	void RotateX(float fAngleX);
	void RotateY(float fAngleY);
	void RotateZ(float fAngleZ);

	void Translate(Vector3& vecMove);
	void GetForward(Vector3& forward);

	// 두 오브젝트의 거리를 반환합니다.
	static float Distance(GameObject* pGO1, GameObject* pGO2);
	static float Distance(GameObject* pGO1, Vector3* pVec);

	// 월드 행렬을 설정합니다.
	inline void SetWorldMatrix(Matrix4x4* pMatrixWorld) { m_matWorld = *pMatrixWorld; }

	// 월드 행렬을 반환합니다.
	Matrix4x4& GetWorldMatrix();

	// 로컬 행렬을 설정합니다.
	inline void SetLocalMatrix(Matrix4x4* pMatrixLocal) { m_matLocal = *pMatrixLocal; }

	// 로컬 행령를 반환합니다.
	inline Matrix4x4& GetLocalMatrix() { return m_matLocal; }

private:
	Vector3 m_position;	// 위치		x, y, z
	Vector3 m_rotAngle;	// 회전각도	x, y, z
	Vector3 m_scale;	// 크기	
	
	Matrix4x4 m_matLocal;	// 자기 자신의 행렬
	Matrix4x4 m_matWorld;	// 부모 행렬을 곱한 총 행렬
	Matrix4x4 matRotX;
	Matrix4x4 matRotY;
	Matrix4x4 matRotZ;
	Matrix4x4 matT;
	Matrix4x4 matS;

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

