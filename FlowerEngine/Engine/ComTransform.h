/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ���� �����ϰ� �ۼ��� �ڵ��Դϴ�.

���� :
��� ���� ������Ʈ�� ��ġ, ȸ��, ũ�Ⱚ�� ������ �ֽ��ϴ�.
��ġ, ȸ��, ũ���� ������ ��ķ� ����ϹǷ� �ش� ��ĵ��� �ֽ��ϴ�.

�� �������(Component)�� ���� ��ü(GameObject)�� ��ġ, ȸ��, ũ�⸦ ���� �����մϴ�.
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

	// �� ������Ʈ�� �Ÿ��� ��ȯ�մϴ�.
	static float Distance(GameObject* pGO1, GameObject* pGO2);
	static float Distance(GameObject* pGO1, Vector3* pVec);

	// ���� ����� �����մϴ�.
	inline void SetWorldMatrix(Matrix4x4* pMatrixWorld) { m_matWorld = *pMatrixWorld; }

	// ���� ����� ��ȯ�մϴ�.
	Matrix4x4& GetWorldMatrix();

	// ���� ����� �����մϴ�.
	inline void SetLocalMatrix(Matrix4x4* pMatrixLocal) { m_matLocal = *pMatrixLocal; }

	// ���� ��ɸ� ��ȯ�մϴ�.
	inline Matrix4x4& GetLocalMatrix() { return m_matLocal; }

private:
	Vector3 m_position;	// ��ġ		x, y, z
	Vector3 m_rotAngle;	// ȸ������	x, y, z
	Vector3 m_scale;	// ũ��	
	
	Matrix4x4 m_matLocal;	// �ڱ� �ڽ��� ���
	Matrix4x4 m_matWorld;	// �θ� ����� ���� �� ���
	Matrix4x4 matRotX;
	Matrix4x4 matRotY;
	Matrix4x4 matRotZ;
	Matrix4x4 matT;
	Matrix4x4 matS;

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;
};

