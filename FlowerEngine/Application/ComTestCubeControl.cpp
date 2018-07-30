#include "stdafx.h"
#include "ComTestCubeControl.h"

ComTestCubeControl::ComTestCubeControl(CString szName) :
	Component(szName)
{
}


ComTestCubeControl::~ComTestCubeControl()
{
}

void ComTestCubeControl::Update()
{
	float y = gameObject->transform->GetRotation().y;
	gameObject->transform->RotateY(D3DXToRadian(1.0f));
}

void ComTestCubeControl::Render()
{
}

void ComTestCubeControl::Awake()
{
}
