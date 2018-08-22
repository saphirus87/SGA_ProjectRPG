#include "stdafx.h"
#include "ComTroll01.h"
#include "ComObjMap.h"
#include "ComCharacter.h"

ComTroll01::ComTroll01(CString szName)
	:ComCharacter(szName)
{
}

ComTroll01::~ComTroll01()
{
}

void ComTroll01::Awake()
{
	Init();
}

void ComTroll01::Update()
{
}

void ComTroll01::Render()
{
}
