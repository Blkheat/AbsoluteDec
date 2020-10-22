#if !defined(__GTIMER_H_INCLUDED__)
#define __GTIMER_H_INCLUDED__

class CGTimer
{

protected: 
	bool	isGoodTimer;                 //QPF�� ��뿩��
	bool    isStop;                      //Ÿ�̸��� ���� ����                 
	
	//QPF�� ��밡�ɽ� ���� ����	
	LARGE_INTEGER	m_startTime;
	LARGE_INTEGER	m_ticksPerSecond;    //�ʴ� ticks

	int		framesPerSecond;             //�ʴ� ������ �� 
	float	lastTime;                    //fps�� ����ϱ� ���� ����
	
	LONGLONG m_llLastElapsedTime;        //�ֱ� ����ð�
    LONGLONG m_llBaseTime;               //���̽� �ð�

	
	//QPF�� ���Ұ��� ���� ����
	int		b_framesPerSecond;
	float	b_startTime;
	float	b_lastTime;


    double  m_dLastElapsedTime;          //�ֱ� ����ð�
	double  m_dBaseTime;                 //���̽� �ð�
	
	float	m_fFrameElapsedSecTime;		 //����ð�

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