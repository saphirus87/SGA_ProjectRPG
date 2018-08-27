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

	// 시간 원천을 설정합니다.
	SetTimeSource(timeSource);
	// 필터링을 설정합니다.
	SetFiltering(1);
	// 예상되는 장면의 시간 흐름을 얻어옵니다.
	FrameTime = GetPredictedFrameDuration(); 
}

// 시간 원천으로 부터 시간을 얻어와 원천의 시작, 끝 값을 설정합니다.
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
	// 정확한 마지막 장면의 흐름을 얻어옵니다.
	double exactLastFrameDuration = GetExactLastFrameDuration();

	// 장면 히스토리에 추가합니다.
	AddToFrameHistory(exactLastFrameDuration);
	
	// 장면 시간은
	FrameTime = GetPredictedFrameDuration();
	CurrentTime += FrameTime;
	++FrameNumber;

	// 옵저버들에게 통보합니다.
	for (VObIter it = Observers.begin(); it != Observers.end(); ++it)
		(*it)->Notify();
}

// 장면 필터링 윈도우를 설정하고 장면의 기본 시간을 설정합니다.
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

	// 시간 흐름 = 원천의 현재 시간 - 원천의 마지막 시간
	double frameDuration = sourceTime - SourceLastValue;
	if (frameDuration > 0.2)
		frameDuration = FrameDurationHistory.back(); // 마지막 원소의 참조를 반환합니다.

	SourceLastValue = sourceTime;
	return frameDuration;
}

void CClock::AddToFrameHistory(double exactFrameDuration)
{
	FrameDurationHistory.push_back(exactFrameDuration);
	if (FrameDurationHistory.size() > (unsigned int)FrameFilteringWindow)
		FrameDurationHistory.pop_front(); // 첫번째 원소를 삭제합니다.
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
