/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 서울게임 아카데미 김주안 선생님 수업시간에 배운 코드를 설계를 수정하여 적용한 코드 입니다.

남길말 :
Ase 파일을 읽어와서 애니메이션 해 주는 게임오브젝트의 구성요소(Component)
*/

#pragma once
#include "stdafx.h"

class ComAseAnimation : public Component
{
public:
	ComAseAnimation(CString szName);
	~ComAseAnimation();

	// Component을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Update() override;
	virtual void Render() override;

	inline void SetScene(ASE_SCENE* pScene) { m_pScene = pScene; }

	vector<ComRenderAse*> vecRenderAse;

private:
	// Animation Info
	ASE_SCENE * m_pScene;
};

