#include "stdafx.h"
#include "ComUIPanelDialog.h"
#include "ComAircraft.h"

ComUIPanelDialog::ComUIPanelDialog(CString szName) : 
	Component(szName),
	m_iCurrentDlgIndex(0),
	m_iCurrentDlgGroupIndex(0)
{
}


ComUIPanelDialog::~ComUIPanelDialog()
{
	vector<DialogInfo*>::iterator iter;
	for (int i = 0; i < m_vecDialog.size(); ++i)
	{
		for (iter = m_vecDialog[i].begin(); iter != m_vecDialog[i].end(); ++iter)
		{
			SAFE_DELETE(*iter);
		}

		m_vecDialog[i].clear();
	}
}

void ComUIPanelDialog::Awake()
{
	FactoryGameObject factory;

	// ��� �̹���
	GameObject* pGoUIImgBG = factory.CreateUIImage(gameObject, "UIImage", "Resources/ui/blackBG.jpg");
	ComUIImage* pUIImgBG = (ComUIImage*)pGoUIImgBG->GetComponent("ComUIImage");
	pUIImgBG->SetFullScreen();
	pUIImgBG->color = Color(1.0f, 1.0f, 1.0f, 0.8f);
		
	// �ϴ� UI �̹���
	GameObject* pGoUIImage = factory.CreateUIImage(gameObject, "UIImage", "Resources/ui/panel-info.png", 0, 640);
	pGoUIImage->transform->SetScale(3.0f, 0.5f, 1.0f);
	GameObject* pGoUIText = factory.CreateUIText(pGoUIImage, "UIText", "", 60.f, 50.f, 800.f, 200.f);
	m_pUIText = (ComUIText*)pGoUIText->GetComponent("ComUIText");

	// NEXT ��ư
	RECT rc = DXUTGetWindowClientRect();
	GameObject* pGoUIBtnClose = factory.CreateUIButton(gameObject, "UIImageButton", "Resources/ui/arrow-btn-r-down.png", rc.right - 300, rc.bottom - 150);
	ComUIButton* pButton = (ComUIButton*)pGoUIBtnClose->GetComponent("ComUIButton");
	pButton->m_pDelegate = this;

	// ���� �̹���
	m_pImgMan = factory.CreateUIImage(gameObject, "UIImage", "Resources/aircraft/ch1.png");
	m_pImgMan->SetActive(false);
	m_pImgMan->transform->SetScale(1.5f, 1.5f, 1.f);

	// ���� �̹���
	m_pImgGirl = factory.CreateUIImage(gameObject, "UIImage", "Resources/aircraft/ch2.png");
	m_pImgGirl->transform->SetScale(1.4f, 1.6f, 1.f);

	// ������ ����
	AddDialog(eChr_Girl, 0, "������ �ڰ� �ִ°ž�. �� �Ͼ�� ����! �������� ������ �ĵ��Ծ�.\r\n�ϳ� ���̸� �� �ϳ� �����°� ������ Ư���̾�.");
	AddDialog(eChr_Man, 0, "�����~ =_=a �� ���̴µ�? ��~ �ǰ��� �װھ�.\r\n������ �� ���� �ȵɱ�?");
	AddDialog(eChr_Girl, 0, "�ȵ�. ���� ��, �׸��� ���� ���� ������!\r\n�� ���� �θ��� �Ÿ��� ��~");
	AddDialog(eChr_Man, 0, "��, �˾Ҿ�. ^_^ �׷��� �� ����� ��� �����ϴ°ž�?");
	AddDialog(eChr_Girl, 0, "W,S,A,D Ű�� ������ �� �־�.\r\nSPACE BAR Ű�� �̻����� ��� ���� ���� ��ų �� ���� �ž�.");

	// ������ (2) ����
	AddDialog(eChr_Girl, 1, "������ �߳�? ������ ^_^\r\n����� ������ ���� ���� �̻��� ������ �����ϰ� �̵� �ӵ��� �����Ұž�.\r\n��, ����� ������ �����.");
	AddDialog(eChr_Man, 1, "��, ����⿡ �Ӹ��� ���屺.");
	AddDialog(eChr_Girl, 1, "����� ����1������ �� 45�� �������� �̻��� �ϳ��� �� �߻��.");

	// ������ (3) ����
	AddDialog(eChr_Girl, 2, "��! ���� ����3�� ������ ����");
	AddDialog(eChr_Man, 2, "��, �̹����� ����⿡ ������ ���屺.");

	// ������ (4) ����
	AddDialog(eChr_Girl, 3, "��! ����4�� ������. �������� ������ ���� ��������.");
	AddDialog(eChr_Man, 3, "��, �̹����� ����⿡ ���� ������ ���屺.");

	// ������ (5) ����
	AddDialog(eChr_Girl, 4, "��! ����5�� ������.\r\n�̻����� �Ʒ� 45�� �������� �ϳ��� �߻�ɰž�.\r\n�׸��� 0.2�� �������� ���������� 30���� �����ϸ�\r\n�ڵ����� �̻����� �߻�ɰž�.");

	// ������ (6) ����
	AddDialog(eChr_Girl, 5, "��! ����6�� ������.\r\n���������� �߻�Ǵ� �̻����� 30������ 15���� �پ�����.");

	// ������ (7) ����
	AddDialog(eChr_Girl, 6, "��! ����7�� ������.\r\n���������� �߻�Ǵ� �̻�����\r\n0.2�� ���ݿ��� 0.1�� �������� �پ�����.");

	// ������ (8) ����
	AddDialog(eChr_Girl, 7, "��! ����8�� ������.\r\n�̻����� �������ε� ȸ���Ǹ� �߻� �ɰž�.");

	// ������ (9) ����
	AddDialog(eChr_Girl, 8, "��! ����9�� ������.\r\n�̻����� �������ε� ȸ���Ǹ� �߻� �ɰž�.");

	// ������ (10) ����
	AddDialog(eChr_Girl, 9, "��! ����10�� ������.\r\n���� �ʻ�⸦ ����� �� �ִµ�\r\n'F' Ű�� ������ ������ ������� �ðž�.\r\n���� ���ܼ� ������ �ε����� �ı� ��ų �� �־�.");

	// ������ (10) ����
	AddDialog(eChr_Girl, 100, "�;ƾƾ�~ ������. ������ ������ ��� �����߾�.\r\n���ְ� ����� ����. ^_^");
	AddDialog(eChr_Girl, 100, "�������� ������ ���� �������ž�. ��� �ؾߵ�~");

	OnClick(NULL);
}

void ComUIPanelDialog::OnClick(ComUIButton * pBtn)
{
//	m_pOwner->SetActive(false);

	if (m_vecDialog[m_iCurrentDlgGroupIndex].size() <= m_iCurrentDlgIndex)
	{
		gameObject->SetActive(false);
		return;
	}

	DialogInfo* dlg = m_vecDialog[m_iCurrentDlgGroupIndex][m_iCurrentDlgIndex];

	switch (dlg->eChr)
	{
	case eChr_Man:
		m_pImgMan->SetActive(true);
		m_pImgGirl->SetActive(false);
		break;
	case eChr_Girl:
		m_pImgMan->SetActive(false);
		m_pImgGirl->SetActive(true);
		break;
	}

	m_pUIText->text = dlg->text;

	++m_iCurrentDlgIndex;
}

void ComUIPanelDialog::Update()
{
	
}

void ComUIPanelDialog::Render()
{
}

void ComUIPanelDialog::AddDialog(eChr chr, int iGroupIndex, CString text)
{
	DialogInfo* dlg = new DialogInfo;
	dlg->eChr = chr;
	dlg->text = text;
	m_vecDialog.resize(iGroupIndex + 1);
	m_vecDialog[iGroupIndex].push_back(dlg);
}

void ComUIPanelDialog::OpenDialog(int DlgGroupIndex)
{
	// ��ȭ�� ������ �Լ� ����
	if (m_vecDialog.size() <= DlgGroupIndex)
		return;

	m_iCurrentDlgGroupIndex = DlgGroupIndex;
	m_iCurrentDlgIndex = 0;
	gameObject->SetActive(true);

	OnClick(NULL);
}

DialogInfo::DialogInfo() : 
	eChr(eChr_Man)
{
}

DialogInfo::~DialogInfo()
{
}
