#include "StdAfx.h"
#include "AppClock.h"
#include "TimeSource.h"
#include "DPObserver.h"

CClock* CClock::m_pInstance = NULL;

CClock::CClock(void) : 
	pTimeSource(NULL)
{
}

CClock::~CClock(void)
{
	delete pTimeSource;
	pTimeSource = NULL;

	for (auto & timer : Observers)
		SAFE_DELETE(timer);

	Observers.clear();
}

void CClock::Delete()
{
	SAFE_DELETE(m_pInstance);
}

void CClock::Initialize(const CTimeSource* timeSource)
{
	CurrentTime = 0;
	FrameTime = 0;
	FrameNumber = 0;
	SourceStartValue = 0;
	SourceLastValue = 0;

	// �ð� ��õ�� �����մϴ�.
	SetTimeSource(timeSource);
	// ���͸��� �����մϴ�.
	SetFiltering(1);
	// ����Ǵ� ����� �ð� �帧�� ���ɴϴ�.
	FrameTime = GetPredictedFrameDuration(); 
}

// �ð� ��õ���� ���� �ð��� ���� ��õ�� ����, �� ���� �����մϴ�.
void CClock::SetTimeSource(const CTimeSource* timeSource)
{
	SAFE_DELETE(pTimeSource);
	pTimeSource = timeSource;

	if (pTimeSource)
	{
		SourceStartValue = timeSource->GetTime();
		SourceLastValue = SourceStartValue;
	}
}

void CClock::FrameStep()
{
	// ��Ȯ�� ������ ����� �帧�� ���ɴϴ�.
	double exactLastFrameDuration = GetExactLastFrameDuration();

	// ��� �����丮�� �߰��մϴ�.
	AddToFrameHistory(exactLastFrameDuration);
	
	// ��� �ð���
	FrameTime = GetPredictedFrameDuration();
	CurrentTime += FrameTime;
	++FrameNumber;

	// �������鿡�� �뺸�մϴ�.
	for (VObIter it = Observers.begin(); it != Observers.end(); ++it)
		(*it)->Notify();
}

// ��� ���͸� �����츦 �����ϰ� ����� �⺻ �ð��� �����մϴ�.
void CClock::SetFiltering(int frameWindow, double frameDefault)
{
	FrameFilteringWindow = frameWindow > 1 ? frameWindow : 1;
	FrameDefaultTime = frameDefault;
	FrameDurationHistory.clear();
	FrameDurationHistory.push_back(FrameDefaultTime); // 0.060
}

double CClock::GetPredictedFrameDuration() const
{
	double totalFrameTime = 0;
	for (QDIter it = FrameDurationHistory.begin(); it != FrameDurationHistory.end(); ++it)
		totalFrameTime += *it;
	return totalFrameTime / FrameDurationHistory.size();
}

double CClock::GetExactLastFrameDuration()
{
	double sourceTime = 0;
	if (pTimeSource)
		sourceTime = pTimeSource->GetTime();

	// �ð� �帧 = ��õ�� ���� �ð� - ��õ�� ������ �ð�
	double frameDuration = sourceTime - SourceLastValue;
	if (frameDuration > 0.2)
		frameDuration = FrameDurationHistory.back(); // ������ ������ ������ ��ȯ�մϴ�.

	SourceLastValue = sourceTime;
	return frameDuration;
}

void CClock::AddToFrameHistory(double exactFrameDuration)
{
	FrameDurationHistory.push_back(exactFrameDuration);
	if (FrameDurationHistory.size() > (unsigned int)FrameFilteringWindow)
		FrameDurationHistory.pop_front(); // ù��° ���Ҹ� �����մϴ�.
}

float CClock::GetFrameRate() const
{
	return 1.0f / (float)FrameTime;	
}

void CClock::AddObserver(IObserver* pObserver)
{
	if (pObserver)
		Observers.push_back(pObserver);
}

void CClock::RemoveObserver(IObserver* pObserver)
{
	/*VObIter iter;
	for (iter = Observers.begin(); iter != Observers.end(); ++iter)
	{
		if (*iter == pObserver)
		{
			SAFE_DELETE(*iter);
			iter = Observers.erase(iter);
		}
	}*/
		
	Observers.erase(
		std::remove(Observers.begin(), Observers.end(), pObserver), 
		Observers.end());
}
