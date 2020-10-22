#if !defined(__GTIMER_H_INCLUDED__)
#define __GTIMER_H_INCLUDED__

class CGTimer
{

protected: 
	bool	isGoodTimer;                 //QPF의 사용여부
	bool    isStop;                      //타이머의 정지 여부                 
	
	//QPF의 사용가능시 적용 변수	
	LARGE_INTEGER	m_startTime;
	LARGE_INTEGER	m_ticksPerSecond;    //초당 ticks

	int		framesPerSecond;             //초당 프레임 수 
	float	lastTime;                    //fps를 계산하기 위한 변수
	
	LONGLONG m_llLastElapsedTime;        //최근 경과시간
    LONGLONG m_llBaseTime;               //베이스 시간

	
	//QPF의 사용불가시 적용 변수
	int		b_framesPerSecond;
	float	b_startTime;
	float	b_lastTime;


    double  m_dLastElapsedTime;          //최근 경과시간
	double  m_dBaseTime;                 //베이스 시간
	
	float	m_fFrameElapsedSecTime;		 //경과시간

public:
	bool	InitTimer();
	void	DInit() { };

	void	UpdateFrameElapsedTime(); 
	float	GetFrameElapsedSecTime();

public:
	CGTimer();
	virtual ~CGTimer();
};

extern CGTimer g_GTimer;

#endif	__GTIMER_H_INCLUDED__