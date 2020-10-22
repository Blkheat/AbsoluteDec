    //-----------------------------------------------------------------
// File : Class 3DSound
//
// Desc : Use DirectX and DSUtil
//
// Copyright (c) 2001.10.29 By CJC
//-----------------------------------------------------------------


#ifndef __C3DSOUND_H__
#define __C3DSOUND_H__

#include "Sound_Manager.H"

//-----------------------------------------------------------------------------
// Name: class C3DSound
// Desc: Encapsulates 3DSound
//-----------------------------------------------------------------------------
class C3DSound
{
public:
	BOOL					m_bInit;
	CSoundManager*			m_pSoundManager;				// Sound manager

	// 3D Sound listener
	LPDIRECTSOUND3DLISTENER m_pDSListener;					// 3D listener object
	DS3DLISTENER            m_dsListenerParams;             // Listener properties(듣는 사람 현재 정보)

	// 3D Sound buffer
	DWORD					m_MaxSound;
	DWORD					m_NoSoundUse;
	CSound**				m_ppSound;						// 3D sound array
	LPDIRECTSOUND3DBUFFER*  m_ppDS3DBuffer;					// 3D sound buffer array
	
 
public:
    C3DSound();
    ~C3DSound();

	HRESULT Initialize(CSoundManager *pSoundManager, DWORD dwMaxSound = 32 );

	HRESULT SetListenerParams(DWORD dwApply = DS3D_IMMEDIATE );	// 듣는 사람 정보 세팅
	HRESULT CommitDeferredSettings();

	HRESULT	ReadWaveFile ( LPSTR strFileName, DWORD dwCreateBufferFlags = DSBCAPS_CTRL3D, DWORD dwNumBuffers = 1 );
	HRESULT ReadWaveFileFromHandle ( HANDLE hFile, DWORD dwCreateBufferFlags = 0, DWORD dwNumBuffers = 1 );
	HRESULT ReadWaveFileFromMemory ( LPSTR strFileName, DWORD dwCreateBufferFlags = DSBCAPS_CTRL3D, DWORD dwNumBuffers = 1  );

	HRESULT SetDistances( DWORD dwWaveIndex, D3DVALUE Min, D3DVALUE Max, DWORD dwApply = DS3D_IMMEDIATE );
	HRESULT SetPositionVelocity( DWORD dwWaveIndex, D3DVECTOR *lpPosition,  D3DVECTOR *lpVelocity, DWORD dwApply = DS3D_IMMEDIATE );
	HRESULT GetPositionVelocity( DWORD dwWaveIndex, D3DVECTOR *lpPosition, D3DVECTOR *lpVelocity );

	HRESULT Play( DWORD dwWaveIndex = 0, DWORD dwPriority = 0, DWORD dwFlags = 0, BOOL bUse2D = FALSE );
	HRESULT PlayWithPos( DWORD dwWaveIndex, D3DVECTOR *lpPosition = NULL, DWORD dwPriority = 0, DWORD dwFlags = 0, DWORD dwApply = DS3D_IMMEDIATE );

	BOOL	IsPlaying( DWORD dwWaveIndex );	
	void	Destroy();
	bool	SafeDestroy();

	HRESULT Stop( DWORD dwWaveIndex );
	HRESULT StopAll();
	
	void SetWaveVolume(LONG WaveRealVolume);
};

#endif
