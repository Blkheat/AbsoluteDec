  //-----------------------------------------------------------------
// File : Class 3DSound
//
// Desc : Use DirectX and DSUtil
//
// Copyright (c) 2001.10.29 By CJC
//-----------------------------------------------------------------

//#define STRICT
#include "RockPCH.h"

#include <dsound.h>
#include <mmsystem.h>

#include "C3DSound.h"
#include "..\\..\\VFileSystem.h"

//-----------------------------------------------------------------------------
C3DSound::C3DSound()
{
	m_bInit				= FALSE;
	m_pSoundManager		= NULL;

	m_MaxSound			= 0;
	m_NoSoundUse		= 0;
	m_ppSound			= NULL;
	m_ppDS3DBuffer		= NULL;
}

//-----------------------------------------------------------------------------
C3DSound::~C3DSound()
{
	Destroy();
}

void	C3DSound::Destroy()
{
	if( m_MaxSound > 0 )
	{
		for( DWORD i = 0; i < m_NoSoundUse; ++i )
		{
			m_ppSound[i]->Stop();
			SAFE_DELETE( m_ppSound[i] );
			SAFE_RELEASE( m_ppDS3DBuffer[i] );
		}
		SAFE_DELETE_ARRAY( m_ppSound );
		SAFE_DELETE_ARRAY( m_ppDS3DBuffer );
	}
	m_bInit			= FALSE;
	m_MaxSound		= 0;
	m_NoSoundUse	= 0;	
	m_ppSound		= NULL;
	m_pSoundManager = NULL;	
	m_ppDS3DBuffer	= NULL;
}

bool	C3DSound::SafeDestroy()
{
	// ���� ����ǰ� �ִ� ���尡 �ִ��� �˻��ϱ�
	if( m_MaxSound > 0 )
	{		
		for( DWORD i = 0; i < m_NoSoundUse; ++i )
		{
			if( IsPlaying( i ) == TRUE )
				return false;
		}
	}

	// ������ۻ���
	Destroy();
	return true;
}

//-----------------------------------------------------------------------------
HRESULT C3DSound::Initialize(CSoundManager *pSoundManager, DWORD dwMaxSound )
{
	// Set sound manager
	if(pSoundManager == NULL)
        return CO_E_NOTINITIALIZED;
	m_pSoundManager = pSoundManager;

	HRESULT hr;

    // Get the 3D listener, so we can control its params
    hr = m_pSoundManager->Get3DListenerInterface( &m_pDSListener );
    if( FAILED(hr) )
		return hr;

    // Get listener parameters
    m_dsListenerParams.dwSize = sizeof(DS3DLISTENER);
    m_pDSListener->GetAllParameters( &m_dsListenerParams );

	// �ٽ� �ʱ�ȭ �� ��� ������ ����ϵ�� ����
	if( m_MaxSound > 0 )
	{
		for( DWORD i = 0; i < m_NoSoundUse; ++i )
		{
			if( NULL == m_ppSound[i] )
				continue;

			m_ppSound[i]->Stop();
			SAFE_DELETE( m_ppSound[i] );
			SAFE_RELEASE( m_ppDS3DBuffer[i] );
		}
		SAFE_DELETE_ARRAY( m_ppSound );
		SAFE_DELETE_ARRAY( m_ppDS3DBuffer );
	}
	m_NoSoundUse = 0;

	// ���Ӱ� ���� ���� �غ�
	m_ppSound = SAFE_NEW_ARRAY( CSound* , dwMaxSound );	
	m_ppDS3DBuffer = SAFE_NEW_ARRAY( LPDIRECTSOUND3DBUFFER , dwMaxSound );	

	ZeroMemory(m_ppSound, sizeof(CSound*) * dwMaxSound );
	ZeroMemory(m_ppDS3DBuffer, sizeof(LPDIRECTSOUND3DBUFFER) * dwMaxSound );

	m_MaxSound = dwMaxSound;

	// set init done
	m_bInit = TRUE;
	return S_OK;
}

//-----------------------------------------------------------------------------
// ��� ����� ��ġ�� �����Ѵ�.
//
// m_dsListenerParams ����ü ������ �ش��ϴ� ���� �ְ� ȣ���Ͽ� �����Ѵ�.
//
//	D3DVECTOR  vPosition;			// ��ǥ
//	D3DVECTOR  vVelocity;			// ����(�Ϲ������� ��ǥ���� �����ϰ� ���� - ���÷� ȿ�� ��꿡 ���ȴٰ� ��)
//	D3DVECTOR  vOrientFront;		// ��� ����� ���� ���� ��ǥ
//	D3DVECTOR  vOrientTop;			// ��� ����� �Ӹ� �� ��ǥ
//	D3DVALUE   flDistanceFactor;	// �Ÿ� ���� �� 
//	D3DVALUE   flRolloffFactor;		// ?
//	D3DVALUE   flDopplerFactor;		// ?
//
HRESULT C3DSound::SetListenerParams(DWORD dwApply)
{
	if( m_pDSListener == NULL )
        return CO_E_NOTINITIALIZED;

	return m_pDSListener->SetAllParameters( &m_dsListenerParams, dwApply );
}

//-----------------------------------------------------------------------------
// ����� �Ķ���Ͱ��� ���忡 ����
//
// dwApply = DS3D_DEFERRED �� ����ؼ� ������ �׸���� ������ ��Ű�� �Լ���.
// dwApply = DS3D_IMMEDIATE �� ����ؼ� ������ ��� ������� ������ �Ǿ��ٸ� �ʿ䰡 ����.
//
HRESULT C3DSound::CommitDeferredSettings()
{
	if( m_pDSListener == NULL )
        return CO_E_NOTINITIALIZED;

	return m_pDSListener->CommitDeferredSettings();
}
//-----------------------------------------------------------------------------
HRESULT	C3DSound::ReadWaveFile ( LPSTR strFileName, DWORD dwCreateBufferFlags, DWORD dwNumBuffers )
{
	if( m_bInit == FALSE )
        return CO_E_NOTINITIALIZED;

	HRESULT hr;

	// ��� ������ ���� �� Ȯ��.
	if( m_MaxSound <= m_NoSoundUse )
		return E_FAIL;

    CWaveFile waveFile;
    waveFile.Open( strFileName, NULL, WAVEFILE_READ );
	waveFile.Close();
    WAVEFORMATEX* pwfx = waveFile.GetFormat();
    if( pwfx == NULL )
        return E_FAIL;

	// 3D�� ����� �������� �ε��Ǵ� ����� MONO�� PCM������ �̾�� �Ѵ�.
	if( dwCreateBufferFlags & DSBCAPS_CTRL3D )
	{
		if( pwfx->nChannels > 1 )
			// Too many channels in wave.  Sound must be mono when using DSBCAPS_CTRL3D
			return E_FAIL;

		if( pwfx->wFormatTag != WAVE_FORMAT_PCM )
			// Sound must be PCM when using DSBCAPS_CTRL3D
			return E_FAIL;
	}

    // Load the wave file into a DirectSound buffer
    hr = m_pSoundManager->Create( &m_ppSound[m_NoSoundUse], 
								  strFileName, 
								  dwCreateBufferFlags, 
								  DS3DALG_HRTF_FULL, dwNumBuffers );
	
	if( hr == DS_NO_VIRTUALIZATION )
	{
		hr = m_pSoundManager->Create( &m_ppSound[m_NoSoundUse], 
									  strFileName, 
									  dwCreateBufferFlags, 
									  DS3DALG_NO_VIRTUALIZATION, dwNumBuffers );
	}
    
	if( FAILED( hr ) )
	{
		return hr;
	}

	// 3D ���� ���
	hr = m_ppSound[m_NoSoundUse]->Get3DBufferInterface( 0, &m_ppDS3DBuffer[m_NoSoundUse] );

	// ��� ���� ����
	m_NoSoundUse++;
	return hr;

}


//-----------------------------------------------------------------------------
HRESULT C3DSound::ReadWaveFileFromHandle ( HANDLE hFile, DWORD dwCreateBufferFlags, DWORD dwNumBuffers)
{
	if( m_bInit == FALSE )
        return CO_E_NOTINITIALIZED;

	HRESULT hr;

	// ��� ������ ���� �� Ȯ��.
	if( m_MaxSound <= m_NoSoundUse )
		return E_FAIL;

    CWaveFile waveFile;
    waveFile.OpenFromHandle( hFile, NULL, WAVEFILE_READ );
	waveFile.Close();	// ����� ������ ȭ�� ����Ʈ�� �� ��ġ�� �ű� - CreateFromHandle���� �����.

    WAVEFORMATEX* pwfx = waveFile.GetFormat();
    if( pwfx == NULL )
        return E_FAIL;

	// 3D�� ����� �������� �ε��Ǵ� ����� MONO�� PCM������ �̾�� �Ѵ�.
	if( dwCreateBufferFlags & DSBCAPS_CTRL3D )
	{
		if( pwfx->nChannels > 1 )
			// Too many channels in wave.  Sound must be mono when using DSBCAPS_CTRL3D
			return E_FAIL;

		if( pwfx->wFormatTag != WAVE_FORMAT_PCM )
			// Sound must be PCM when using DSBCAPS_CTRL3D
			return E_FAIL;
	}

    // Load the wave file into a DirectSound buffer
    hr = m_pSoundManager->CreateFromHandle( &m_ppSound[m_NoSoundUse], 
								  hFile, 
								  dwCreateBufferFlags | DSBCAPS_CTRL3D, 
								  DS3DALG_HRTF_FULL, dwNumBuffers );

	if( hr == DS_NO_VIRTUALIZATION )
	{
		hr = m_pSoundManager->CreateFromHandle( &m_ppSound[m_NoSoundUse], 
									  hFile, 
									  dwCreateBufferFlags | DSBCAPS_CTRL3D, 
									  DS3DALG_NO_VIRTUALIZATION, dwNumBuffers );
	}
    if( FAILED( hr ) )
	{
		return hr;
	}

	// 3D ���� ���
	hr = m_ppSound[m_NoSoundUse]->Get3DBufferInterface( 0, &m_ppDS3DBuffer[m_NoSoundUse] );

	// ��� ���� ����
	m_NoSoundUse++;
	return hr;

}

// struct WAVE_HEADER {
//  char riff_id[4];	/* "RIFF" */
//  long riff_size;	/* riff size */
//  char wave_id[4];	/* "WAVE" */
//  char format_id[4];	/* "fmt " */
//  long format_size;	/* format size */
//  short format_type;	/* 1 for PCM */
//  short ChannelNo;	/* 1 for mono, 2 for stereo */
//  long SamplesPerSec;	/* 11025 for 11KHz */
//  long AvgBytesPerSec;
//  short BytesPerSample;
//  short BitsPerSample;	/* 8 for 8bits, 16 for 16bits */
//  char data_id[4];	/* "data" */
//  long data_size;	/* data size */
//};


struct WAVE_HEADER 
{
  char  ChunkID[ 4 ];
  long  ChunkSize;
  char  Format[ 4 ];
  
  char	SubChunk1ID[ 4 ];
  long	SubChunk1Size;
  short AudioFormat;
  short NumChannels;
  long  SampleRate;
  long  ByteRate;
  short BlockAlign;
  short BitsPerSample;

  char  Subchunk2ID[ 4 ];
  long  Subchunk2Size;
};


HRESULT C3DSound::ReadWaveFileFromMemory ( LPSTR strFileName, DWORD dwCreateBufferFlags, DWORD dwNumBuffers )
{
	if( m_bInit == FALSE )
        return CO_E_NOTINITIALIZED;

	HRESULT hr;

	// ��� ������ ���� �� Ȯ��.
	if( m_MaxSound <= m_NoSoundUse )
		return E_FAIL;
	
	WAVEFORMATEX pwfx;
	ZeroMemory( &pwfx, sizeof(WAVEFORMATEX) ); 
/*
	pwfx.wFormatTag      = WAVE_FORMAT_PCM; 
	pwfx.nChannels       = 1; 
//	pwfx.nSamplesPerSec  = 22050; 
	pwfx.nSamplesPerSec  = 44100; 
	pwfx.wBitsPerSample  = 16; 
	pwfx.nBlockAlign     = (WORD) (pwfx.wBitsPerSample / 8 * pwfx.nChannels);
	pwfx.nAvgBytesPerSec = pwfx.nSamplesPerSec * pwfx.nBlockAlign;
*/
	VFileHandle* pFH = NULL;
	
	pFH = g_VFSound.OpenFile( strFileName );	
	
	if( pFH == NULL ) return E_FAIL;
	
	WAVE_HEADER	WaveHeader;
	
	pFH->Read( &WaveHeader, sizeof(WAVE_HEADER) );

	pwfx.wFormatTag      = WaveHeader.AudioFormat; 
	pwfx.nChannels       = WaveHeader.NumChannels; 
	pwfx.nSamplesPerSec  = WaveHeader.SampleRate; 
	pwfx.wBitsPerSample  = WaveHeader.BitsPerSample; 
//	pwfx.nBlockAlign     = (WORD) (pwfx.wBitsPerSample / 8 * pwfx.nChannels);
	pwfx.nBlockAlign     = WaveHeader.BlockAlign;
	pwfx.nAvgBytesPerSec = WaveHeader.ByteRate;

	g_VFSound.CloseFile( pFH );	

	// 3D�� ����� �������� �ε��Ǵ� ����� MONO�� PCM������ �̾�� �Ѵ�.
	if( dwCreateBufferFlags & DSBCAPS_CTRL3D )
	{
		if( pwfx.nChannels > 1 )
			// Too many channels in wave.  Sound must be mono when using DSBCAPS_CTRL3D
			return E_FAIL;

		if( pwfx.wFormatTag != WAVE_FORMAT_PCM )
			// Sound must be PCM when using DSBCAPS_CTRL3D
			return E_FAIL;
	}

//	VFileHandle* pFH = NULL;
	pFH = g_VFSound.OpenFile( strFileName );	
	if( pFH == NULL ) return E_FAIL;

	ULONG Size   = (ULONG)( WaveHeader.Subchunk2Size );
	BYTE  *pData = (BYTE*)pFH->GetData();
	
	pData = pData + sizeof(WAVE_HEADER);

	//CWaveFile waveFile;
	//waveFile.OpenFromMemory( (BYTE*)pFH->GetData(), pFH->GetSize(), &pwfx, WAVEFILE_READ );
    //waveFile.OpenFromMemory( (BYTE*)pFH->GetData(), Size , &pwfx, WAVEFILE_READ );
	
    // Load the wave file into a DirectSound buffer
    hr = m_pSoundManager->CreateFromMemory( &m_ppSound[m_NoSoundUse],
											pData , Size , 
											&pwfx, dwCreateBufferFlags | DSBCAPS_CTRL3D, 
//											DS3DALG_DEFAULT, dwNumBuffers );
//											DS3DALG_HRTF_FULL , dwNumBuffers );
											DS3DALG_HRTF_LIGHT , dwNumBuffers );
	
    if( FAILED( hr ) )
	{
		g_VFSound.CloseFile( pFH );	

		return hr;
	}
	
	if( hr == DS_NO_VIRTUALIZATION )
	{
		hr = m_pSoundManager->CreateFromMemory( &m_ppSound[m_NoSoundUse],
											pData , Size , 
											&pwfx, dwCreateBufferFlags | DSBCAPS_CTRL3D, 
											DS3DALG_NO_VIRTUALIZATION, dwNumBuffers );
	}

	g_VFSound.CloseFile( pFH );	

    if( FAILED( hr ) )
	{
		return hr;
	}

	// 3D ���� ���
	hr = m_ppSound[m_NoSoundUse]->Get3DBufferInterface( 0, &m_ppDS3DBuffer[m_NoSoundUse] );

	// ��� ���� ����
	m_NoSoundUse++;
	return hr;
}


//-----------------------------------------------------------------------------
HRESULT C3DSound::SetDistances( DWORD dwWaveIndex, D3DVALUE Min, D3DVALUE Max, DWORD dwApply )
{
	if(dwWaveIndex >= m_NoSoundUse)
		return E_FAIL;

	HRESULT hr;
	hr  = m_ppDS3DBuffer[dwWaveIndex]->SetMinDistance( Min, dwApply );
	hr |= m_ppDS3DBuffer[dwWaveIndex]->SetMaxDistance( Max, dwApply );

	return hr;
}

//-----------------------------------------------------------------------------
HRESULT C3DSound::SetPositionVelocity( DWORD dwWaveIndex, D3DVECTOR *lpPosition,  D3DVECTOR *lpVelocity, DWORD dwApply )
{
	if(dwWaveIndex >= m_NoSoundUse)
		return E_FAIL;

	HRESULT hr;
	hr  = m_ppDS3DBuffer[dwWaveIndex]->SetPosition( lpPosition->x, lpPosition->y, lpPosition->z, dwApply );
	hr |= m_ppDS3DBuffer[dwWaveIndex]->SetVelocity( lpVelocity->x, lpVelocity->y, lpVelocity->z, dwApply );

	return hr;
}

//-----------------------------------------------------------------------------
HRESULT C3DSound::GetPositionVelocity( DWORD dwWaveIndex, D3DVECTOR *lpPosition, D3DVECTOR *lpVelocity )
{
	if( dwWaveIndex >= m_NoSoundUse )
		return E_FAIL;

	HRESULT hr;
	hr  = m_ppDS3DBuffer[dwWaveIndex]->GetPosition( lpPosition );
	hr |= m_ppDS3DBuffer[dwWaveIndex]->GetVelocity( lpVelocity );
	return hr;
}

//-----------------------------------------------------------------------------
HRESULT C3DSound::Play( DWORD dwWaveIndex, DWORD dwPriority, DWORD dwFlags, BOOL bUse2D )
{
	if( dwWaveIndex >= m_NoSoundUse )
		return E_FAIL;	
/**/
	// 2D ���� ��� ��� Ȯ��
	if( bUse2D == TRUE )
		m_ppDS3DBuffer[dwWaveIndex]->SetMode( DS3DMODE_DISABLE, DS3D_IMMEDIATE );
	else
		m_ppDS3DBuffer[dwWaveIndex]->SetMode( DS3DMODE_NORMAL, DS3D_IMMEDIATE );
/**/
//	m_ppDS3DBuffer[dwWaveIndex]->SetMode( DS3DMODE_DISABLE, DS3D_IMMEDIATE );

	return m_ppSound[dwWaveIndex]->Play( dwPriority, dwFlags );
}

//-----------------------------------------------------------------------------
HRESULT C3DSound::PlayWithPos( DWORD dwWaveIndex, D3DVECTOR *lpPosition, DWORD dwPriority, DWORD dwFlags, DWORD dwApply )
{
	if( dwWaveIndex >= m_NoSoundUse )
		return E_FAIL;

	// �ӵ��� ���� By wxywxy
	static D3DXVECTOR3 vVelocity(0.0f,0.0f,0.0f);
	
	HRESULT hr	= DS_OK;	
	if( lpPosition != NULL )
//		hr  = SetPositionVelocity ( dwWaveIndex, lpPosition, lpPosition, dwApply );
		hr  = SetPositionVelocity ( dwWaveIndex, lpPosition, &vVelocity, dwApply );

	hr |= Play( dwWaveIndex, dwPriority, dwFlags );
	return hr;
}

//-----------------------------------------------------------------------------
BOOL C3DSound::IsPlaying( DWORD dwWaveIndex )
{
	if( dwWaveIndex >= m_NoSoundUse )
		return E_FAIL;

	return m_ppSound[dwWaveIndex]->IsSoundPlaying();
}


//-----------------------------------------------------------------------------
HRESULT C3DSound::Stop( DWORD dwWaveIndex)
{
	if(dwWaveIndex >= m_NoSoundUse)
		return E_FAIL;

	return ( m_ppSound[dwWaveIndex]->Stop() );
}

//-----------------------------------------------------------------------------
HRESULT C3DSound::StopAll()
{
	HRESULT hr = S_OK;

	for(DWORD i=0; i<m_NoSoundUse; i++)
	{
		hr |= m_ppSound[i]->Stop();
	}

	return hr;
}

void C3DSound::SetWaveVolume(LONG WaveRealVolume)
{
	for(DWORD i=0; i < m_NoSoundUse; i++)
	{
		m_ppSound[i]->m_apDSBuffer[0]->SetVolume( WaveRealVolume );
	}
}



// end of file