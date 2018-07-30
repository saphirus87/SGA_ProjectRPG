/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ������� ��ī���� ���־� ������ �����ð��� ��� �ڵ带 ���踦 �����Ͽ� ������ �ڵ� �Դϴ�.

���渻 :
Ase ������ �о�ͼ� �ִϸ��̼� �� �ִ� ���ӿ�����Ʈ�� �������(Component)
*/

#pragma once
#include "stdafx.h"

class ComAseAnimation : public Component
{
public:
	ComAseAnimation(CString szName);
	~ComAseAnimation();

	// Component��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	inline void SetScene(ASE_SCENE* pScene) { m_pScene = pScene; }

	vector<ComRenderAse*> vecRenderAse;

private:
	// Animation Info
	ASE_SCENE * m_pScene;
};

