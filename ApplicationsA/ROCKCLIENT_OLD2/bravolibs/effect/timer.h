
#ifndef			__TIMER_H__
#define			__TIMER_H__


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// include
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include		<vector>
#include		<windows.h>



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// class CTimer : ���� ��ġ Ŭ����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class CTimer
{
public:
	CTimer()
	{ 
		// �߰��Ǵ� �ð� Ÿ�̸Ӹ� ���Ϳ� ���� �ִ´�.
		m_Timers.push_back(this);
		Stop();
	}


	virtual ~CTimer()
	{ 
		// ��� �ð� Ÿ�̸ӵ��� ���Ϳ� �Բ� �ı� �Ѵ�.
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
	void	Begin()								{ Stop(); Start(); }			// ���� �ϴ� �ð� Ÿ�̸�.
	void	BeginWithDelay(float fDelay)		{ m_fTime = -fDelay; Start(); }	// �����̸� �༭ ���� �ϴ� �ð� Ÿ�̸�.
 
	void	SetTime(float t)					{ m_fTime = t; }
	float	GetTime()							{ return m_fTime; }

	bool	IsRunning()							{ return m_bIsRunning; }

	void Update( float fElapsedTime )
	{
		if (m_bIsRunning)
			m_fTime += fElapsedTime;
	}

	// ��� �ð� Ÿ�̸Ӹ� ������Ʈ ��Ű��.
	static std::vector<CTimer *> m_Timers;
	static void UpdateAll(float fElapsedTime)
	{
		for( std::vector<CTimer *>::iterator i = m_Timers.begin(); i != m_Timers.end(); i++ )
		{
			(*i)->Update(fElapsedTime);
		}
	}
	

protected:
	bool			m_bIsRunning;				// ���� ����ִ���?
	float			m_fTime;					// �ð�.
};







#endif