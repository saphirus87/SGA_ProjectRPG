#include "stdafx.h"
#include "ComSmallderon.h"
#include "ComObjMap.h"
#include "ComFollowTarget.h"
#include "IChrState.h"
#include "ComChrControl.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ChrStateAttack.h"

ComSmallderon::ComSmallderon(CString szName)
	:Component(szName), m_pMap(NULL), 
	m_pTarget(NULL), m_pCurrentState(NULL)
{
}

ComSmallderon::~ComSmallderon()
{
}

void ComSmallderon::Awake()
{
	//CPP 다형성
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pTarget = (ComFollowTarget*)gameObject->GetComponent("ComFollowTarget");
	GameObject* pObjMap = GameObject::Find("ObjMap");
	if (pObjMap != NULL)
		m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");
	m_pCurrentState = new ChrStateStand(m_pAnimation);
}

void ComSmallderon::Update()
{
	if (m_pTarget->IsFollowing)
	{
		Vector3 pos = gameObject->transform->GetPosition();
		float fHeight = 0.0f;

		if (m_pMap != NULL && m_pMap->GetHeight(fHeight, pos))
		{
			pos.y = fHeight;
			gameObject->transform->SetPosition(pos);
		}
	}
	if (m_pTarget->AbleAttack)
	{
		// 공격 가능 거리

	}

	//Test Code
	if (Input::KeyDown('1'))
	{
		m_pAnimation->PlayAnimation(eAni_Stand);
	}
	if (Input::KeyDown('2'))
	{
		m_pAnimation->PlayAnimation(eAni_Walk);
	}
}

void ComSmallderon::Render()
{
}

void ComSmallderon::Stand()
{
}

void ComSmallderon::Walk()
{
}

void ComSmallderon::Attack1()
{
}
