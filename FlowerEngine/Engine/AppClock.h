#pragma once

class CTimeSource;
class IObserver;

typedef std::deque <double> QDouble;
typedef std::deque <double>::const_iterator QDIter;
typedef std::list <IObserver*> VObserver;
typedef std::list <IObserver*>::iterator VObIter;

class CClock
{
private:
	//CClock(const CTimeSource* timeSource = NULL);
	CClock(void);
	~CClock(void);
	static CClock* m_pInstance;

public:
	static CClock* GetInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new CClock();
			m_pInstance->Initialize(new CTimeSourceWin32);
		}
		return m_pInstance;
	}

	void Delete();
	void Initialize(const CTimeSource* timeSource = NULL);

	// 기능 입니다.
	void FrameStep();

	// 접근자 입니다.
	void SetTimeSource(const CTimeSource* timeSource);
	void SetFiltering(int frameWindow, double frameDefault = 0.030);
	void AddObserver(IObserver* pObserver);
	void RemoveObserver(IObserver* pObserver);

	double GetTime() const { return CurrentTime; }
	double GetFrameDuration() const { return FrameTime; } // == GetDeltaTime
	int GetFrameNumber() const { return FrameNumber; }
	float GetFrameRate() const;

private:
	double GetPredictedFrameDuration() const;
	double GetExactLastFrameDuration();
	void AddToFrameHistory(double exactFrameDuration);

	const CTimeSource* pTimeSource;
	double CurrentTime;
	double FrameTime;
	int FrameNumber;

	double SourceStartValue;
	double SourceLastValue;

	int FrameFilteringWindow;
	double FrameDefaultTime;

	QDouble FrameDurationHistory;
	VObserver Observers;
};
