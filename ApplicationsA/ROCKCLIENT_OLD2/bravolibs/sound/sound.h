
#ifndef _SOUND_H_
#define _SOUND_H_


#include	<windows.h>
#include	<mmsystem.h>
#include	<dshow.h>
#include	"Sound_Manager.H"
#include	"C3DSound.H"
#include    "..\\..\\CSceneManager.h"

// ���� �ε���
#define		SOUND_BIRD				0		// ��
#define		SOUND_RAIN				20		// ��
#define		SOUND_LEVELUP			24		// ������
#define		SOUND_OWL				37		// �ξ���

#define		SOUND_DEFENSE			44		// ���潺���
#define		SOUND_OPENWINDOW		45		// â����
#define		SOUND_DROPITEM			46		// �����۹�����
#define		SOUND_CLICKITEM			47		// �����ۼ���
#define		SOUND_EQUIPITEM			48		// ����������
#define		SOUND_PICKUP_COIN		71		// �����ݱ�


extern CRITICAL_SECTION	g_csMP3Flag;

//-----------------------------------------------------------------------------
// Desc: MP3 �Ŵ���
//-----------------------------------------------------------------------------
class CMsound
{
public:
	IGraphBuilder*	pGraph;
	IMediaControl*	pMediaControl;
	IMediaEvent*	pEvent;
	IMediaSeeking*	pSeek;

	// ������̴�?
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

	// �ʱ�ȭ, �ı���
	void	Clear();
	void	Destroy();	
	void	Read( int index );

	// mp3 �б�, ���
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


	
	// mp3 ����
	void	Stop();
	void	Pause();
};


//-----------------------------------------------------------------------------
// Desc: WAV  �Ŵ���
//-----------------------------------------------------------------------------
struct SOUND_TABLE
{
	bool	bLoop;		// ����
	BYTE	byCount;	// ����
	BYTE	byTime;		// �ð�
	BYTE	byWeat;		// ����
	BYTE	byMap;		// ��
	BYTE	byCurt;		// �����ε���
	BYTE	byAttr;		// �Ӽ�(�߿䵵)

	bool	bIsLoaded;	// �ε��ߴ�?
	bool	bIsLoading;	// �ε���?
	long	lPreTime;	// ���ð�

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
	CSoundManager	m_SoundManager;		// ����Ŵ���
	C3DSound*		m_p3DSound;			// ����
	SOUND_TABLE*	m_pSoundTable;		// ��������

public:	

	bool			m_bInit;			// �ʱ�ȭ�ߴ�?
	unsigned int	m_SoundCount;		// �������ϰ���
	long			m_lPreTime;			// �����ð�	

	int				m_ISWavePlay;		// ȿ���� �÷��� ���� By wxywxy
	int				m_ISMP3Play;		// ȿ���� �÷��� ���� By wxywxy

	LONG			m_MP3Volume;		// ����� ����
	LONG			m_WaveVolume;		// ���̺� ����

	LONG			m_MP3RealVolume;
	LONG			m_WaveRealVolume;

public:
	 // �ʱ�ȭ By wxywxy
	Sound_Manager():
	m_ISWavePlay( FALSE ) ,
	m_ISMP3Play( FALSE )
	{ 
		m_SoundCount = 0;

		Clear(); 
	}

	~Sound_Manager()	{ Destory(); }

	//-----------------------------------------------------------------------------
	// 3D���� �ʱ�ȭ�ϱ�
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destory();	
	void	Create( HWND hwnd, char* file_pos );
	void	CreateWaveFile( int count, char* file_pos );
	void	CreateSoundAttr( int count, char* file_pos );

	//-----------------------------------------------------------------------------
	// ���������Ʈ
	//-----------------------------------------------------------------------------
	void	UpdateSound( int _nIndex );
	void	UpdateSoundUse();	
	void	InitSoundTime();

	//-----------------------------------------------------------------------------
	// �÷��� �ϱ� ���� ���� ���� ���� �ϱ�
	//-----------------------------------------------------------------------------
	int Sound_Manager::GetCurrentIndex(int index);
	
	//-----------------------------------------------------------------------------
	// wav �����Լ�
	//-----------------------------------------------------------------------------
	void	Update();
	void	Play( int Index, D3DVECTOR*	pPosition = NULL ,e_SCENE_MODE SceneMode = SCENE_NORMAL, BOOL IsDirectPlay = FALSE );		// wav���� ���	
	void	Play( int Index, float x, float y, float z ,e_SCENE_MODE SceneMode = SCENE_NORMAL, BOOL IsDirectPlay = FALSE );		// wav���� ���	
	void	Stop( int num_sound );														// wav���� ��� ����	
	void	AllStop();																	// wav���� ��� ��¡
	int		GetFileCount( char* file_pos, char* file_ext );								// �������� ��������
	
	//.............................................................................
	// ȿ���� ������� �÷��� ����
	//.............................................................................
	int		ISWaveEnable(void)			{ return m_ISWavePlay; }
	int		ISMP3Enable(void)			{ return m_ISMP3Play;  }

	LONG	GetWaveVolume(void)			{ return m_WaveVolume; }
	LONG	GetMP3Volume(void)			{ return m_MP3Volume;  }

	//.............................................................................
	// ȿ���� ������� �÷��� ���� 
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

	CMsound		m_BackSound;			// �������
	CMsound		m_BackBattleSound;		// ��������
	int			m_GroundIndex;		// ������ε���	
	void		PlayMP3( bool bForce = false , int index = -1 );
	void		PauseBattleSound();
	void		StopBattleSound();
	void		PlayBattleSound();
	void		PauseBGSound();
	void		StopBGSound();
	void		PlayBGSound();
	
	BOOL		m_ISNoPackLoad;		// ��ü �ٶ� ( ��Ʈ ���丮�� UNPACKSOUND.dat ���� ���� �������� �о� Boa �� )
};


extern	Sound_Manager	g_Sound;
extern  HANDLE	g_MP3;

#endif