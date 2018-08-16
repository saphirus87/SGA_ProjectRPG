#pragma once
#include "stdafx.h"
#include "IChrState.h"

class IChrState;
class ComChrControl;

class ChrStateAttack1 : public IChrState
{
private:
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateAttack1(ComRenderSkinnedMesh* pAnimation);
	~ChrStateAttack1();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(ComChrControl * pChrState) override;
	virtual void Walk(ComChrControl * pChrState) override;
	virtual void Attack1(ComChrControl * pChrState) override;
	virtual void Attack2(ComChrControl * pChrState) override;
	virtual void Attack3(ComChrControl * pChrState) override;
};

class ChrStateAttack2 : public IChrState
{
private:
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateAttack2(ComRenderSkinnedMesh* pAnimation);
	~ChrStateAttack2();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(ComChrControl * pChrState) override;
	virtual void Walk(ComChrControl * pChrState) override;
	virtual void Attack1(ComChrControl * pChrState) override;
	virtual void Attack2(ComChrControl * pChrState) override;
	virtual void Attack3(ComChrControl * pChrState) override;
};

class ChrStateAttack3 : public IChrState
{
private:
	ComRenderSkinnedMesh * m_pAnimation;

public:
	ChrStateAttack3(ComRenderSkinnedMesh* pAnimation);
	~ChrStateAttack3();

	// IChrState을(를) 통해 상속됨
	virtual void Stand(ComChrControl * pChrState) override;
	virtual void Walk(ComChrControl * pChrState) override;
	virtual void Attack1(ComChrControl * pChrState) override;
	virtual void Attack2(ComChrControl * pChrState) override;
	virtual void Attack3(ComChrControl * pChrState) override;
};
