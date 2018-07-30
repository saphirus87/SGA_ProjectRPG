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

	// 배경 이미지
	GameObject* pGoUIImgBG = factory.CreateUIImage(gameObject, "UIImage", "Resources/ui/blackBG.jpg");
	ComUIImage* pUIImgBG = (ComUIImage*)pGoUIImgBG->GetComponent("ComUIImage");
	pUIImgBG->SetFullScreen();
	pUIImgBG->color = Color(1.0f, 1.0f, 1.0f, 0.8f);
		
	// 하단 UI 이미지
	GameObject* pGoUIImage = factory.CreateUIImage(gameObject, "UIImage", "Resources/ui/panel-info.png", 0, 640);
	pGoUIImage->transform->SetScale(3.0f, 0.5f, 1.0f);
	GameObject* pGoUIText = factory.CreateUIText(pGoUIImage, "UIText", "", 60.f, 50.f, 800.f, 200.f);
	m_pUIText = (ComUIText*)pGoUIText->GetComponent("ComUIText");

	// NEXT 버튼
	RECT rc = DXUTGetWindowClientRect();
	GameObject* pGoUIBtnClose = factory.CreateUIButton(gameObject, "UIImageButton", "Resources/ui/arrow-btn-r-down.png", rc.right - 300, rc.bottom - 150);
	ComUIButton* pButton = (ComUIButton*)pGoUIBtnClose->GetComponent("ComUIButton");
	pButton->m_pDelegate = this;

	// 남자 이미지
	m_pImgMan = factory.CreateUIImage(gameObject, "UIImage", "Resources/aircraft/ch1.png");
	m_pImgMan->SetActive(false);
	m_pImgMan->transform->SetScale(1.5f, 1.5f, 1.f);

	// 여자 이미지
	m_pImgGirl = factory.CreateUIImage(gameObject, "UIImage", "Resources/aircraft/ch2.png");
	m_pImgGirl->transform->SetScale(1.4f, 1.6f, 1.f);

	// 움직임 설명
	AddDialog(eChr_Girl, 0, "아직도 자고 있는거야. 안 일어나고 뭐해! 물달팽이 군단이 쳐들어왔어.\r\n하나 죽이면 또 하나 나오는게 저들의 특성이야.");
	AddDialog(eChr_Man, 0, "어디어디~ =_=a 안 보이는데? 아~ 피곤해 죽겠어.\r\n오늘은 좀 쉬면 안될까?");
	AddDialog(eChr_Girl, 0, "안돼. ㅋㅋ 아, 그리고 눈을 떠야 보이지!\r\n눈 감고 두리번 거리지 마~");
	AddDialog(eChr_Man, 0, "아, 알았어. ^_^ 그런데 이 비행기 어떻게 조종하는거야?");
	AddDialog(eChr_Girl, 0, "W,S,A,D 키로 움직일 수 있어.\r\nSPACE BAR 키로 미사일을 쏘면 적을 격추 시킬 수 있을 거야.");

	// 레벨업 (2) 축하
	AddDialog(eChr_Girl, 1, "레벨업 했네? 축하해 ^_^\r\n비행기 레벨이 오를 수록 미사일 범위가 증가하고 이동 속도도 증가할거야.\r\n또, 비행기 외형도 변경되.");
	AddDialog(eChr_Man, 1, "아, 비행기에 머리가 생겼군.");
	AddDialog(eChr_Girl, 1, "비행기 레벨1에서는 위 45도 방향으로 미사일 하나가 더 발사되.");

	// 레벨업 (3) 축하
	AddDialog(eChr_Girl, 2, "어! 벌써 레벨3네 축하해 ㅋㅋ");
	AddDialog(eChr_Man, 2, "아, 이번에는 비행기에 날개가 생겼군.");

	// 레벨업 (4) 축하
	AddDialog(eChr_Girl, 3, "어! 레벨4네 축하해. 물달팽이 군단이 많이 없어졌어.");
	AddDialog(eChr_Man, 3, "아, 이번에는 비행기에 꼬리 날개가 생겼군.");

	// 레벨업 (5) 축하
	AddDialog(eChr_Girl, 4, "어! 레벨5네 축하해.\r\n미사일이 아래 45도 방향으로 하나더 발사될거야.\r\n그리고 0.2초 간격으로 오른쪽으로 30도씩 증가하며\r\n자동으로 미사일이 발사될거야.");

	// 레벨업 (6) 축하
	AddDialog(eChr_Girl, 5, "어! 레벨6네 축하해.\r\n오른쪽으로 발사되는 미사일이 30도에서 15도로 줄어들었어.");

	// 레벨업 (7) 축하
	AddDialog(eChr_Girl, 6, "어! 레벨7네 축하해.\r\n오른쪽으로 발사되는 미사일이\r\n0.2초 간격에서 0.1초 간격으로 줄어들었어.");

	// 레벨업 (8) 축하
	AddDialog(eChr_Girl, 7, "어! 레벨8네 축하해.\r\n미사일이 왼쪽으로도 회전되며 발사 될거야.");

	// 레벨업 (9) 축하
	AddDialog(eChr_Girl, 8, "어! 레벨9네 축하해.\r\n미사일이 윗쪽으로도 회전되며 발사 될거야.");

	// 레벨업 (10) 축하
	AddDialog(eChr_Girl, 9, "어! 레벨10네 축하해.\r\n이제 필살기를 사용할 수 있는데\r\n'F' 키를 누르면 적들이 빨려들어 올거야.\r\n방어막이 생겨서 적들을 부딪혀서 파괴 시킬 수 있어.");

	// 레벨업 (10) 축하
	AddDialog(eChr_Girl, 100, "와아아아~ 축하해. 달팽이 군단을 모두 섬멸했어.\r\n우주가 깔끔해 졌네. ^_^");
	AddDialog(eChr_Girl, 100, "물달팽이 군단이 더욱 강해질거야. 대비를 해야되~");

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
	// 대화가 없으면 함수 종료
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
