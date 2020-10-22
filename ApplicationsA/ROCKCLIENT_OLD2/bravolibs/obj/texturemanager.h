
#ifndef		_TEXTURN_MANAGER_H_
#define		_TEXTURN_MANAGER_H_

#include <windows.h>
#include <list>


#define		DATA_UPDATA_TIME		30000	// 데이타 검색시간(30초)
#define		TEXTURE_INPUT_COUNT		32		// 텍스쳐 생성갯수
#define		SOUND_INPUT_COUNT		16		// 사운드 생성갯수


//-----------------------------------------------------------------------------
// Desc: 텍스쳐 관리자
//
// 1. 해당 클래스의 목적은 게임에서 사용되는 텍스쳐들을 검사하여서,
// 사용빈도수가 낮은 텍스쳐는 메모리에서 삭제시키는 일을 한다.
// 
// 2. 캐릭터(유저, NPC) 생성시 타겟이 사용하는 텍스쳐가 있는지 검사하여서,
// 텍스쳐가 없다면 실시간으로 로딩해준다.
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
struct	DATA_CREATE_ATTR 
{
	int		Index;
	BYTE	byType;

	DATA_CREATE_ATTR()	{ Clear(); }
	void	Clear()	{ Index = -1; byType = 255; }
};


class TEXTURE_MANAGER
{
public:	
	BYTE		m_byType;			// 검색타입( 0:CHR 1:NPC 2:OBJ )
	long		m_lPreCreTexTime;	// 이전 텍스쳐 생성시간 	
	long		m_lPreTime;			// 이전검색시간		

	WORD		theChrDeleteCnt;
	WORD		theNpcDeleteCnt;
	WORD		theObjDeleteCnt;

	std::list < DATA_CREATE_ATTR * > theCreateTex;

	// 텍스쳐
	DATA_CREATE_ATTR	m_CreateTex[TEXTURE_INPUT_COUNT];
	CRITICAL_SECTION	m_csTextureFlag;

	// 사운드 
	DATA_CREATE_ATTR	m_CreateSound[SOUND_INPUT_COUNT];

public:
	TEXTURE_MANAGER()	
	{ 
		Clear();		
		InitializeCriticalSection( &m_csTextureFlag );
	}
	~TEXTURE_MANAGER()
	{ 
		Destroy();	
		DeleteCriticalSection( &m_csTextureFlag );
	}

	void	Clear();										// 변수초기화
	void	Destroy();										// 변수초기화
	void	UpdateDataUse();								// 데이타사용검사
	void	CreateUseTex();									// 써야될 텍스쳐 생성
	void	DeleteNotUsedTex();								// 쓰지 않는 텍스쳐 지우기 

	// 텍스쳐
	void	InitTextrueTime();								// 텍스쳐 사용시간 초기화	
	bool	SetTextureIndex( BYTE byType, int _nIndex );	// 텍스쳐 생성하기 위해서 인덱스 저장하기
	void	UpdateTextureCreate();							// 텍스쳐 생성하기

	void    ClearTheCreateTexList();						///-- theCreateTex에 저장된 데이타 지우기

	// 사운드
	void	InitSoundTime();								// 텍스쳐 사용시간 초기화
	void	UpdateSoundUse();								// 텍스쳐 검사하기
	bool	SetSoundIndex( int _nIndex );					// 사운드 생성하기 위해서 인덱스 저장하기
	void	UpdateSoundCreate();							// 텍스쳐 생성하기
	
	void	DeleteAllChrAndNpcTex();
};

extern TEXTURE_MANAGER		g_TexManager;

#endif