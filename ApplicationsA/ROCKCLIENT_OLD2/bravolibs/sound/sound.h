
#ifndef _SOUND_H_
#define _SOUND_H_


#include	<windows.h>
#include	<mmsystem.h>
#include	<dshow.h>
#include	"Sound_Manager.H"
#include	"C3DSound.H"
#include    "..\\..\\CSceneManager.h"

// 사운드 인덱스
#define		SOUND_BIRD				0		// 새
#define		SOUND_RAIN				20		// 비
#define		SOUND_LEVELUP			24		// 레벨업
#define		SOUND_OWL				37		// 부엉이

#define		SOUND_DEFENSE			44		// 디펜스모드
#define		SOUND_OPENWINDOW		45		// 창열기
#define		SOUND_DROPITEM			46		// 아이템버리기
#define		SOUND_CLICKITEM			47		// 아이템선택
#define		SOUND_EQUIPITEM			48		// 아이템장착
#define		SOUND_PICKUP_COIN		71		// 동전줍기


extern CRITICAL_SECTION	g_csMP3Flag;

//-----------------------------------------------------------------------------
// Desc: MP3 매니저
//-----------------------------------------------------------------------------
class CMsound
{
public:
	IGraphBuilder*	pGraph;
	IMediaControl*	pMediaControl;
	IMediaEvent*	pEvent;
	IMediaSeeking*	pSeek;

	// 재생중이니?
	bool			m_bPlay;	
	bool			m_bStop;
	bool			m_bPause;
public:
	CMsound()	{ 
					InitializeCriticalSection(&m_csMP3Stop);
					InitializeCriticalSection(&m_csMP3Play);
					Clear(); 
				}
	~CMsound()	{ 
					DeleteCriticalSection(&m_csMP3Stop);
					DeleteCriticalSection(&m_csMP3Play);
//					Destroy(); 
				}

	// 초기화, 파괴자
	void	Clear();
	void	Destroy();	
	void	Read( int index );

	// mp3 읽기, 재생
	void	Init();
	int		Play();
	HRESULT SetVolume( long lVolume );

	CRITICAL_SECTION	m_csMP3Stop;
	CRITICAL_SECTION	m_csMP3Play;
	void	SetStop(bool flag)  
	{
		EnterCriticalSection(&m_csMP3Stop);
		m_bStop = flag;		
		LeaveCriticalSection(&m_csMP3Stop);
	}

	bool    GetStop()
	{
		bool stop;
		EnterCriticalSection(&m_csMP3Stop);
		stop = m_bStop;		
		LeaveCriticalSection(&m_csMP3Stop);
		return stop;
	}

	void	SetPause(bool flag)  
	{
		EnterCriticalSection(&m_csMP3Stop);
		m_bPause = flag;		
		LeaveCriticalSection(&m_csMP3Stop);
	}

	bool    GetPause()
	{
		bool pause;
		EnterCriticalSection(&m_csMP3Stop);
		pause = m_bPause;		
		LeaveCriticalSection(&m_csMP3Stop);
		return pause;
	}


	
	// mp3 정지
	void	Stop();
	void	Pause();
};


//-----------------------------------------------------------------------------
// Desc: WAV  매니저
//-----------------------------------------------------------------------------
struct SOUND_TABLE
{
	bool	bLoop;		// 루프
	BYTE	byCount;	// 갯수
	BYTE	byTime;		// 시간
	BYTE	byWeat;		// 날씨
	BYTE	byMap;		// 맵
	BYTE	byCurt;		// 현재인덱스
	BYTE	byAttr;		// 속성(중요도)

	bool	bIsLoaded;	// 로딩했니?
	bool	bIsLoading;	// 로딩중?
	long	lPreTime;	// 사용시간

	SOUND_TABLE()
	{
		bLoop	= false; 
		byCount = 0; 
		byTime	= 0; 
		byWeat	= 0; 
		byMap	= 0; 
		byCurt	= 0; 
		byAttr	= 0;

		bIsLoaded	= false;
		bIsLoading	= false;
		lPreTime	= 0;
	}
};


class Sound_Manager
{
	CSoundManager	m_SoundManager;		// 사운드매니저
	C3DSound*		m_p3DSound;			// 사운드
	SOUND_TABLE*	m_pSoundTable;		// 사운드정보

public:	

	bool			m_bInit;			// 초기화했니?
	unsigned int	m_SoundCount;		// 사운드파일갯수
	long			m_lPreTime;			// 이전시간	

	int				m_ISWavePlay;		// 효과음 플레이 여부 By wxywxy
	int				m_ISMP3Play;		// 효과음 플레이 여부 By wxywxy

	LONG			m_MP3Volume;		// 배경음 볼륨
	LONG			m_WaveVolume;		// 웨이브 볼륨

	LONG			m_MP3RealVolume;
	LONG			m_WaveRealVolume;

public:
	 // 초기화 By wxywxy
	Sound_Manager():
	m_ISWavePlay( FALSE ) ,
	m_ISMP3Play( FALSE )
	{ 
		m_SoundCount = 0;

		Clear(); 
	}

	~Sound_Manager()	{ Destory(); }

	//-----------------------------------------------------------------------------
	// 3D사운드 초기화하기
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destory();	
	void	Create( HWND hwnd, char* file_pos );
	void	CreateWaveFile( int count, char* file_pos );
	void	CreateSoundAttr( int count, char* file_pos );

	//-----------------------------------------------------------------------------
	// 사운드업데이트
	//-----------------------------------------------------------------------------
	void	UpdateSound( int _nIndex );
	void	UpdateSoundUse();	
	void	InitSoundTime();

	//-----------------------------------------------------------------------------
	// 플레이 하기 전에 사운드 버퍼 선택 하기
	//-----------------------------------------------------------------------------
	int Sound_Manager::GetCurrentIndex(int index);
	
	//-----------------------------------------------------------------------------
	// wav 실행함수
	//-----------------------------------------------------------------------------
	void	Update();
	void	Play( int Index, D3DVECTOR*	pPosition = NULL ,e_SCENE_MODE SceneMode = SCENE_NORMAL, BOOL IsDirectPlay = FALSE );		// wav파일 재생	
	void	Play( int Index, float x, float y, float z ,e_SCENE_MODE SceneMode = SCENE_NORMAL, BOOL IsDirectPlay = FALSE );		// wav파일 재생	
	void	Stop( int num_sound );														// wav파일 재생 정지	
	void	AllStop();																	// wav파일 모두 정징
	int		GetFileCount( char* file_pos, char* file_ext );								// 사운드파일 갯수리턴
	
	//.............................................................................
	// 효과음 배경음악 플레이 여부
	//.............................................................................
	int		ISWaveEnable(void)			{ return m_ISWavePlay; }
	int		ISMP3Enable(void)			{ return m_ISMP3Play;  }

	LONG	GetWaveVolume(void)			{ return m_WaveVolume; }
	LONG	GetMP3Volume(void)			{ return m_MP3Volume;  }

	//.............................................................................
	// 효과음 배경음악 플레이 여부 
	//.............................................................................
	void    SetWaveEnable(int ISAble)	{ m_ISWavePlay = ISAble; }
	void    SetMP3Enable(int ISAble)	{ m_ISMP3Play  = ISAble; }
	
	void	SetMP3Volume(LONG Value);
	void	SetWaveVolume(LONG Value);
	void	CheckBattleMode( BOOL bForceRefresh = FALSE );
	
	BOOL	m_IsNowBattleSound;	
	long	m_lBattlePrevTime;
	long	m_lCheckBattleTime;
	
//-----------------------------------------------------------------------------
// Desc : MP3
//-----------------------------------------------------------------------------
public:

	CMsound		m_BackSound;			// 배경음악
	CMsound		m_BackBattleSound;		// 전투음악
	int			m_GroundIndex;		// 현재맵인덱스	
	void		PlayMP3( bool bForce = false , int index = -1 );
	void		PauseBattleSound();
	void		StopBattleSound();
	void		PlayBattleSound();
	void		PauseBGSound();
	void		StopBGSound();
	void		PlayBGSound();
	
	BOOL		m_ISNoPackLoad;		// 업체 줄때 ( 루트 디렉토리에 UNPACKSOUND.dat 있음 언팩 버전으로 읽어 Boa 요 )
};


extern	Sound_Manager	g_Sound;
extern  HANDLE	g_MP3;

#endif