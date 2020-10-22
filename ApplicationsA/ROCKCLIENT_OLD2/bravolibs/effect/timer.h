
#ifndef			__TIMER_H__
#define			__TIMER_H__


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// include
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include		<vector>
#include		<windows.h>



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// class CTimer : 스톱 워치 클래스
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class CTimer
{
public:
	CTimer()
	{ 
		// 추가되는 시간 타이머를 벡터에 집어 넣는다.
		m_Timers.push_back(this);
		Stop();
	}


	virtual ~CTimer()
	{ 
		// 모든 시간 타이머들을 벡터와 함께 파괴 한다.
		for( std::vector<CTimer *>::iterator i = m_Timers.begin();	i != m_Timers.end(); i++)
		{
			if ( (*i) == this )
			{
				m_Timers.erase( i );
				break;
			}
		}
	}

	void	Start()								{ m_bIsRunning = true;  }
	void	Pause()								{ m_bIsRunning = false; }
	void	Stop()								{ Pause(); m_fTime = 0; }
	void	Begin()								{ Stop(); Start(); }			// 시작 하는 시간 타이머.
	void	BeginWithDelay(float fDelay)		{ m_fTime = -fDelay; Start(); }	// 딜레이를 줘서 시작 하는 시간 타이머.
 
	void	SetTime(float t)					{ m_fTime = t; }
	float	GetTime()							{ return m_fTime; }

	bool	IsRunning()							{ return m_bIsRunning; }

	void Update( float fElapsedTime )
	{
		if (m_bIsRunning)
			m_fTime += fElapsedTime;
	}

	// 모든 시간 타이머를 업데이트 시키자.
	static std::vector<CTimer *> m_Timers;
	static void UpdateAll(float fElapsedTime)
	{
		for( std::vector<CTimer *>::iterator i = m_Timers.begin(); i != m_Timers.end(); i++ )
		{
			(*i)->Update(fElapsedTime);
		}
	}
	

protected:
	bool			m_bIsRunning;				// 현재 살아있느냐?
	float			m_fTime;					// 시간.
};







#endif