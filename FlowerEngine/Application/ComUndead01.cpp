#include "stdafx.h"
#include "ComUndead01.h"
#include "ChrStateStand.h"
#include "ChrStateWalk.h"
#include "ComCharacter.h"

ComUndead01::ComUndead01(CString szName)
	:ComCharacter(szName)
{
}

ComUndead01::~ComUndead01()
{
}

void ComUndead01::Awake()
{
	Init();
}

void ComUndead01::Update()
{
}

void ComUndead01::Render()
{
}
