            #include "RockPCH.h"
#include "VFileSystem.h"

#include	"..\\rocklibs\\RockInterface\SystemPrintWnd.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	"..\\quadlist.h"
#include	"..\\RockClient.h"
#include	"..\\Pc.h"
#include	"..\\Map.h"
#include	"..\\Camera.h"
//#include	"..\\Weather_Manager.H"
#include	"Sound.H"
#include	<Obj\\Object.h>
#include	<Obj\\TextureManager.H>
#include	<Map\\Field.h>
#include	<stdio.h>
#include	<assert.h>

#include "..\\global.h"

extern long g_nowTime;

#define		SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define		SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#define		SOUND_RANGE_TILE1		16		// �ڱ�ĳ���� �Ÿ�
#define		SOUND_MAXRANGE_TILE1	128		// �ڱ�ĳ���� �ƽ��Ÿ�

#define		SOUND_RANGE_TILE2		8		// ī�޶���� �Ÿ�
#define		SOUND_MAXRANGE_TILE2	80		// ī�޶���� �ƽ��Ÿ�

#define		SOUND_RANGE_CAMERA_Y1	60
#define		SOUND_RANGE_CAMERA_Y2	0.02f


//-----------------------------------------------------------------------------
// Desc: ������� ����
//-----------------------------------------------------------------------------
#define		LOADING_SONG		0
#define		MAIN_SONG			1
#define		ATTACK_SONG			2

//-----------------------------------------------------------------------------
// Desc: MP3
//-----------------------------------------------------------------------------
#define		MP3_PLAY			14
#define		MP3_STOP			1
#define		VOLUME_SILENCE		-10000
#define		VOLUME_FULL			0

//-----------------------------------------------------------------------------
// Desc: ȿ����
//-----------------------------------------------------------------------------
#define		SOUND_DUNGEON		19
#define		SOUND_PLAY_TIME		30000
#define		SOUND_RANGE			128.0f
#define		SOUND_SAME_COUNT	8
#define		SOUND_USE_TIME		20000

extern		long				g_nowTime;
extern		TEXTURE_MANAGER		g_TexManager;

HANDLE				g_MP3 = NULL;
bool				g_bmp3Out = false;
CRITICAL_SECTION	g_csMP3Flag;

extern LONG	g_EffectSoundVol;

//-----------------------------------------------------------------------------
// Desc: Sound_Manager Ŭ���� �ʱ�ȭ
//-----------------------------------------------------------------------------
void Sound_Manager::Clear()
{	
	m_p3DSound		= NULL;	
	m_pSoundTable	= NULL;	
	
	m_lPreTime		= g_nowTime;	// �ð�	
	m_bInit			= true;	
	m_SoundCount	= 0;
	m_GroundIndex	= -2;	

	m_ISNoPackLoad = FALSE;

	m_lBattlePrevTime =	g_nowTime;
	m_lCheckBattleTime = 6000;
}


//-----------------------------------------------------------------------------
// Desc: Sound_Manager Ŭ���� �ʱ�ȭ
//-----------------------------------------------------------------------------
void Sound_Manager::Destory()
{
	SAFE_DELETE_ARRAY( m_pSoundTable );
	SAFE_DELETE_ARRAY( m_p3DSound );
	
	Clear();
}


//-----------------------------------------------------------------------------
// Desc: DirectSound ����
//-----------------------------------------------------------------------------
void Sound_Manager::Create( HWND hwnd, char* file_pos )
{

	FILE  *fp = NULL;

	char  FileName[ 1024 ] = "";
	
	wsprintf( FileName , "%s\\UNPACKSOUND.dat" , g_RockClient.GetRootDir() );
	
	fp = fopen( FileName , "r+t" );

	if( fp != NULL )
	{
		m_ISNoPackLoad = TRUE;

		fclose( fp );
	}
	
	// ���̷�Ʈ ���� �ʱ�ȭ( �ڵ�, ���¼���, ä��, �츣��, ��Ʈ )
	HRESULT hr = m_SoundManager.Initialize( hwnd, DSSCL_PRIORITY, 1, 22050, 16 );
//	HRESULT hr = m_SoundManager.Initialize( hwnd, DSSCL_PRIORITY, 1, 44100, 16 );
	
																	 
	if( hr != S_OK )
	{
		m_bInit = false;
		return;
	}	

	// ���� ���� �б�	
	m_SoundCount = GetFileCount( file_pos, "\\Sound\\*.wav" );	
	
	// ���� ���� �غ�
	m_p3DSound		= SAFE_NEW_ARRAY( C3DSound , m_SoundCount );		
	m_pSoundTable	= SAFE_NEW_ARRAY( SOUND_TABLE , m_SoundCount );	

	// ���� �Ӽ� �����б� ( �޸𸮿� ���� �÷��� )	
	CreateSoundAttr( m_SoundCount, g_RockClient.GetRootDir() );

	
#ifdef APPLY_BATTLE_BGSOUND
	
	m_BackBattleSound.Init();
	m_BackBattleSound.Read( 2000 );

#endif

	// ���� ���� �޸𸮿� �ø���
	// CreateWaveFile( m_SoundCount, g_RockClient.GetRootDir() );		
}


int Sound_Manager::GetFileCount( char* file_pos, char* file_ext )
{
	int count	= 0;	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ���������� �ִ� ��� �����	
	char file_path[256] = {0,};
	strcpy( file_path, file_pos);
	strcat( file_path, file_ext);	
	
	// ���� ã��	
	WIN32_FIND_DATA	fild_data;
	HANDLE hFindFile = FindFirstFile( file_path, &fild_data );
	
	int Find	= -1;
	while( Find != 0 )
	{			
		Find = FindNextFile( hFindFile, &fild_data );
		count++;
	}					
	FindClose( hFindFile );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	if( m_ISNoPackLoad )
	{
		// ���������� �ִ� ��� �����	
		char file_path[256] = {0,};
		strcpy( file_path, file_pos);
		strcat( file_path, file_ext);	
		
		// ���� ã��	
		WIN32_FIND_DATA	fild_data;
		HANDLE hFindFile = FindFirstFile( file_path, &fild_data );
		
		int Find	= -1;
		while( Find != 0 )
		{			
			Find = FindNextFile( hFindFile, &fild_data );
			count++;
		}					
		FindClose( hFindFile );
	
		return count;
	}

	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	char buf[256] = {0,};
	int Index = 0;

	pFHL = g_VFSound.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, buf );

			if( lstrcmp( buf, "wav" ) == 0 )	
			{				
				count++;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


	return count;
}


//-----------------------------------------------------------------------------
// Desc: WAV ���� �б�
//-----------------------------------------------------------------------------
void Sound_Manager::CreateWaveFile( int SoundCount, char* szFilePos )
{ 	
	// wav����
	char	szFilePath[256] = {0,};
	for( int i = 0; i < SoundCount; ++i )
	{
		switch( m_pSoundTable[i].byAttr )
		{
		case DATA_NON_DELETE:
			{
				// ����ε��÷��׺���
				m_pSoundTable[i].bIsLoaded = true;

				// 3DCound ���ۻ���
				m_p3DSound[i].Initialize( &m_SoundManager, (DWORD)m_pSoundTable[i].byCount );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
				// ����� ���� ���� ����
				sprintf( szFilePath, "%s\\Sound\\%04d.wav", szFilePos, i );
				for( int j = 0; j < (int)m_pSoundTable[i].byCount; ++j )
				{
					if( E_FAIL == m_p3DSound[i].ReadWaveFile(szFilePath, DSBCAPS_CTRL3D, 1) )
					{
//						#ifdef GAMETYPE_TEST
//							MessageBox( NULL, "CreateWaveFile Fail", "", MB_OK );
//						#endif
						return;
					}
				}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

				if( m_ISNoPackLoad )
				{
					// ����� ���� ���� ����
					sprintf( szFilePath, "%s\\Sound\\%04d.wav", szFilePos, i );
					for( int j = 0; j < (int)m_pSoundTable[i].byCount; ++j )
					{
						if( E_FAIL == m_p3DSound[i].ReadWaveFile(szFilePath, DSBCAPS_CTRL3D, 1) )
						{
							return;
						}
					}

					continue;
				}

				// ����� ���� ���� ����
				sprintf( szFilePath, "%04d.wav", i );
				for( int j = 0; j < (int)m_pSoundTable[i].byCount; ++j )
				{
					if( E_FAIL == m_p3DSound[i].ReadWaveFileFromMemory( szFilePath, DSBCAPS_CTRL3D, 1 ) )
					{
						return;
					}
				}
				
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

			}
			break;
		default:
			continue;
			break;
		}
	}
}


void Sound_Manager::CreateSoundAttr( int count, char* file_pos )
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	char	file_path[256] = {0,};
	strcpy( file_path, file_pos);
	strcat( file_path, "\\Sound\\ATTR.dat", 256 ); 

	FILE*	file;
	file = fopen( file_path, "rt" );
	assert( file != NULL && "!����Ӽ� �����б� ����" );
	{
		char* token;				// ��ū
		char string [256];			// ���� �ٴ��� ���� ����
		bool bRead  = true;			// ���� �� �о����� üũ		
		int	 number	= 0;			// �ټ�		
		char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
				
		// �� ������ �б�
		if( fgets( string, 256, file ) == NULL )				
				return;				
		do 
		{								
			// �ּ��� �ִٸ� ���� �� ����
			while( string[0] == '/' && string[1] == '/' )	
			{				
				// �� �̻� ������ ���ٸ� ���� ����
				if( fgets( string, 256 , file ) == NULL )		
				{
					bRead = false;
					break;
				}
			}
			
			///-- Critical Error Check : Array Index - DEV
			assert( number < m_SoundCount );
			///-- Critical Error Message : Alpha Tester
			if( number >= m_SoundCount )
			{
				MessageBox( NULL, "Sound Count Error", 0, 0 );
			}

			// �������� ��ȣ
			token = strtok( string, splitter );	
			if( token == NULL )			
				break;	

			// ���尹��
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byCount = (unsigned int)atoi(token);
			
			// ����?
			token = strtok( NULL, splitter );
			atoi(token) == 0 ? m_pSoundTable[number].bLoop = false : m_pSoundTable[number].bLoop = true;

			// ����
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byWeat = (unsigned int)atoi(token);

			// �ð�
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byTime = (unsigned int)atoi(token);

			// ��
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byMap = (unsigned int)atoi(token);

			// ����Ӽ�
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byAttr = (unsigned int)atoi(token);			
			
			// ���� ȭ�� �б�
			//g_Sound.UpdateSound( number );								
			
			number++;


			// ���� �� �б�, ������ ���ٸ� string�� 0����									
			if( fgets( string, 256 , file ) == NULL )		
				bRead = false;	
			
		} 
		while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			
	}
	fclose( file );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFSound.OpenFile( "ATTR.dat" );

	assert( pFH != NULL && "!����Ӽ� �����б� ����" );
	{
		char* token;				// ��ū
		char string [256];			// ���� �ٴ��� ���� ����
		bool bRead  = true;			// ���� �� �о����� üũ		
		int	 number	= 0;			// �ټ�		
		char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
				
		// �� ������ �б�
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
		{
			g_VFSound.CloseFile( pFH );
			return; 
		}				
		do 
		{								
			// �ּ��� �ִٸ� ���� �� ����
			while( string[0] == '/' && string[1] == '/' )	
			{				
				// �� �̻� ������ ���ٸ� ���� ����
				if( pFH->Gets( string, sizeof( string ) ) == 0 )
				{
					bRead = false;
					break;
				}	
			}
			
			// �������� ��ȣ
			token = strtok( string, splitter );	
			if( token == NULL )			
				break;	

			///-- Critical Error Check : Array Index
			assert( number < m_SoundCount );
			///-- Critical Error Message : Alpha Tester
			if( number >= m_SoundCount )
			{
				MessageBox( NULL, "Sound Count Error", 0, 0 );
			}

			// ���尹��
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byCount = (unsigned int)atoi(token);
			
			// ����?
			token = strtok( NULL, splitter );
			atoi(token) == 0 ? m_pSoundTable[number].bLoop = false : m_pSoundTable[number].bLoop = true;

			// ����
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byWeat = (unsigned int)atoi(token);

			// �ð�
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byTime = (unsigned int)atoi(token);

			// ��
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byMap = (unsigned int)atoi(token);

			// ����Ӽ�
			token = strtok( NULL, splitter );
			m_pSoundTable[number].byAttr = (unsigned int)atoi(token);
						
			// ���� ȭ�� �б�
			//g_Sound.UpdateSound( number );							
			
			number++;

			// ���� �� �б�, ������ ���ٸ� string�� 0����									
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
				bRead = false;		
			
		} 
		while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			
	}
	g_VFSound.CloseFile( pFH );
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}

void Sound_Manager::CheckBattleMode( BOOL bForceRefresh )
{

#ifdef APPLY_BATTLE_BGSOUND
	
	DWORD dNowType = g_Pc.GetPlayer()->m_curt_event.type;

	BOOL IsBattleMode = FALSE;
	if( dNowType == SM_ATTACK_NPC || dNowType == SM_ATTACK_DELAY )
	{
		IsBattleMode = TRUE;
	}

	if( dNowType == SM_USE_SKILL )
	{
		DWORD dSkillCode = g_Pc.GetPlayer()->m_NowAInfo.dSkillCode;
		SSkillBaseInfo* skill_Info;
		skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( dSkillCode ); 
		if( skill_Info )
		{
			if( ( skill_Info->theSkillType2 == n_DeathBlow ||
				  skill_Info->theSkillType2 == n_AttackMagic || 
				  skill_Info->theSkillType2 == n_NoFrameDeathBlow ) &&
				 ( skill_Info->theActTarget != n_AT_SELF ) )
			{
				IsBattleMode = TRUE;
			}
		}
	}

	if( bForceRefresh )
	{
		IsBattleMode = TRUE;
	}

	if( IsBattleMode )
	{
		m_lBattlePrevTime = g_nowTime;
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_lBattlePrevTime ) , < , m_lCheckBattleTime ) )
	{
		if( !m_IsNowBattleSound )
		{
			PauseBGSound();
			PlayBattleSound();
			m_IsNowBattleSound = TRUE;
		}
	}
	else
	{
		if( m_IsNowBattleSound )
		{
			PauseBattleSound();
			PlayBGSound();
			m_IsNowBattleSound = FALSE;
		}
	}

#endif

}

//-----------------------------------------------------------------------------
// Desc: WAV ���� ���
//-----------------------------------------------------------------------------
void Sound_Manager::Play( int _nIndex, float fX, float fY, float fZ ,e_SCENE_MODE SceneMode, BOOL IsDirectPlay )
{
	if( !IsDirectPlay )
	{
	  if( !m_ISWavePlay )
		return;
	}

	if( m_SoundCount <= _nIndex ) return;

	D3DVECTOR vPos;
	vPos.x = fX; vPos.y = fY; vPos.z = fZ;
	Play( _nIndex, &vPos , SceneMode, IsDirectPlay );
}

void Sound_Manager::Play( int _nIndex, D3DVECTOR* pPosition ,e_SCENE_MODE SceneMode, BOOL IsDirectPlay )
{			
	//....................................................................................................
	switch( g_RockClient.m_SceneManager.GetSoundPlayMode() )
	{
		case SCENE_NORMAL:
			break;

		case SCENE_SQUENCE:
			
			if( SceneMode != g_RockClient.m_SceneManager.GetSoundPlayMode() )
				return;

			break;
	}
	
	
	if( !IsDirectPlay )
	{
	  if( !m_ISWavePlay )
		return;
	}

	if( m_SoundCount <= _nIndex ) return;


//	if( !m_pSoundTable[_nIndex].bIsLoaded )
	//-----------------------------------------------------------------------------
	// �ε��Ǿ����� �ʰ�, �ε��ϴ� ���� �ƴ϶�� �ش� ���� �����ϱ�
	//-----------------------------------------------------------------------------
	if( false == m_pSoundTable[_nIndex].bIsLoaded && false == m_pSoundTable[_nIndex].bIsLoading )
	{
//		LeaveCriticalSection( &g_TexManager.m_csSoundFlag );
		g_Sound.UpdateSound( _nIndex );							
	}

	// �ʱ�ȭ����
	if( m_bInit == false )
		return;	
	// ȿ���� ��� ����? By wxywxy
	else if( !IsDirectPlay )
	{
		if( ISWaveEnable() == false )
		{
			return;
		}
	}
/*
	// �������϶��� ó���Ѵ�
	else if( g_RockClient.GetGameMode() != GMODE_NORMAL )
		return;
*/
	// 2D ���常 ���� ( wxywxy )
	//pPosition = NULL;
	//-----------------------------------------------------------------------------
	// ���尡 �ε��Ǿ����� üũ
	//-----------------------------------------------------------------------------	
//	EnterCriticalSection( &g_TexManager.m_csSoundFlag );

	//-----------------------------------------------------------------------------
	// ���� �ε��Ǿ��ִٸ� ����ϱ�
	//-----------------------------------------------------------------------------
	if( true == m_pSoundTable[_nIndex].bIsLoaded && false == m_pSoundTable[_nIndex].bIsLoading )
	{		
//		LeaveCriticalSection( &g_TexManager.m_csSoundFlag );
		
		if( !GetCurrentIndex( _nIndex ) )
		{
			RLGS( "------------------ GetCurrentIndex ( Buffer FULL ) --------------------------" );
			
			return;
		}

		//-----------------------------------------------------------------------------
		// Desc: �Ÿ��� ���� ������ġ�� ���
		//
		// 1. �ڱ�ĳ���� SOUND_CHECK_RANGE Ÿ���̳��� 0���� �Ͽ��� ����Ѵ�.
		// 2. �׿��� ��Ȳ�� ī�޶���� �Ÿ��� ����Ͽ��� ����Ѵ�.
		//
		// Copyright (c) 2003.11.27 By KStaR79
		//-----------------------------------------------------------------------------
		D3DXVECTOR3		vPos(0.0f,0.0f,0.0f);
		
		if( pPosition != NULL )
		{		
			//-----------------------------------------------------------------------------
			// �ڱ�ĳ���Ϳ��� �Ÿ�
			//-----------------------------------------------------------------------------
			D3DXVECTOR3 vDistance;

			vDistance.x = pPosition->x-g_Pc.GetPlayer()->GetPosTM()._41;
			vDistance.y = 0.0f;//pPosition->y-g_Pc.GetPlayer()->GetPosTM()._42;
			vDistance.z = pPosition->z-g_Pc.GetPlayer()->GetPosTM()._43;

			vPos.x = vDistance.x;
			vPos.y = 0.0f;
			vPos.z = vDistance.z;
			
			
			if( D3DXVec3Length( &vDistance ) <= 50.0f )
			{
				pPosition = NULL;
/*				
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientFront.x = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientFront.y = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientFront.z = 1.0f;
				
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientTop.x = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientTop.y = 1.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientTop.z = 0.0f;
				
				m_p3DSound[_nIndex].m_dsListenerParams.vPosition.x = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vPosition.y = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vPosition.z = 0.0f;

				m_p3DSound[_nIndex].m_pDSListener->SetAllParameters( &m_p3DSound[_nIndex].m_dsListenerParams , DS3D_IMMEDIATE );
*/				
			}
			else
			{
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientFront.x = g_Camera.m_vDir.x;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientFront.y = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientFront.z = g_Camera.m_vDir.z;
				
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientTop.x = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientTop.y = 1.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vOrientTop.z = 0.0f;
				
				m_p3DSound[_nIndex].m_dsListenerParams.vPosition.x = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vPosition.y = 0.0f;
				m_p3DSound[_nIndex].m_dsListenerParams.vPosition.z = 0.0f;
				
				m_p3DSound[_nIndex].m_pDSListener->SetAllParameters( &m_p3DSound[_nIndex].m_dsListenerParams , DS3D_IMMEDIATE );
			}

			//-----------------------------------------------------------------------------
			// SOUND_CHECK_RANGE �⺻��ġ�ȿ� �ִٸ� ����� �鸰��
			//-----------------------------------------------------------------------------
//			if( abs(vTempPos.x) <= SOUND_RANGE_TILE1 && abs(vTempPos.y) <= SOUND_RANGE_TILE1 && abs(vTempPos.z) <= SOUND_RANGE_TILE1 )		
//				pPosition = NULL;
			//-----------------------------------------------------------------------------
			// ī�޶���� ��ġ�� ���� �����ġ ����
			//-----------------------------------------------------------------------------
//			else
/*			{

				// ���� By wxywxy
				D3DXVECTOR3		vDistance;
				D3DXVECTOR3		vNearPlane;
				D3DXVECTOR3		vCameraPos;
				D3DXVECTOR3		vDir;
				D3DXVECTOR3		vPos = *pPosition;
				float			fDistance = 0.0f;

				vDir = ( g_Camera.m_vEye - g_Camera.m_vLookat );
					
				D3DXVec3Normalize( &vNearPlane , &vDir ); 

				vNearPlane *= 5.0f;

//				vCameraPos.x	= g_Pc.GetPlayer()->GetPosTM()._41 + vDir.x - vNearPlane.x;
//				vCameraPos.y	= g_Pc.GetPlayer()->GetPosTM()._42 + vDir.y - vNearPlane.y;
//				vCameraPos.z	= g_Pc.GetPlayer()->GetPosTM()._43 + vDir.z - vNearPlane.z; 
				vCameraPos.x	= g_Pc.GetPlayer()->GetPosTM()._41 + vNearPlane.x;
				vCameraPos.y	= g_Pc.GetPlayer()->GetPosTM()._42 + vNearPlane.y;
				vCameraPos.z	= g_Pc.GetPlayer()->GetPosTM()._43 + vNearPlane.z; 
				
//				vCameraPos.x	= g_Pc.GetPlayer()->GetPosTM()._41+g_Camera.m_vEye.y-g_Camera.m_vLookat.x;
//				vCameraPos.y	= g_Pc.GetPlayer()->GetPosTM()._42+g_Camera.m_vEye.y-g_Camera.m_vLookat.y;
//				vCameraPos.z	= g_Pc.GetPlayer()->GetPosTM()._43+g_Camera.m_vEye.z-g_Camera.m_vLookat.z;			

//				vCameraPos.x	= g_Pc.GetPlayer()->GetPosTM()._41;
//				vCameraPos.y	= 0.0f;//g_Pc.GetPlayer()->GetPosTM()._42+g_Camera.m_vEye.y-g_Camera.m_vLookat.y;
//				vCameraPos.z	= g_Pc.GetPlayer()->GetPosTM()._43;			

//				vPos.y			= 0.0f;
				vDistance		= vPos - vCameraPos;

				fDistance = D3DXVec3Length( &vDistance );
					
//				DebugPrint( "\nDistance : %f \n" , fDistance );
				//-----------------------------------------------------------------------------
				// ī�޶��ó��� ����� �鸰��
				//-----------------------------------------------------------------------------

/*
				if( fDistance <= 100.0f )
				{
					pPosition = NULL;

					// DebugPrint( "\nNear\n" );
				}
				else*/
				//-----------------------------------------------------------------------------
				// ī�޶󿡼� �ʹ� �ָ� �ִٸ� ���� �����Ѵ�
				//-----------------------------------------------------------------------------
/*
				if( fDistance >= 1000.0f )
				{
					// DebugPrint( "\nFar\n" );

					return;
				}
				//-----------------------------------------------------------------------------
				// ī�޶���� �Ÿ��� ���� ó��
				//-----------------------------------------------------------------------------
				else
				{
					// DebugPrint( "\nRange\n" );

					pPosition->x = vDistance.x;
					pPosition->y = vDistance.y;
					pPosition->z = vDistance.z;
				}
*/
/*
				
				//-----------------------------------------------------------------------------
				// ī�޶�� ��ü���� �Ÿ�
				//-----------------------------------------------------------------------------
				D3DVECTOR		vCameraPos;
				vCameraPos.x	= g_Pc.GetPlayer()->GetPosTM()._41+g_Camera.m_vEye.x-g_Camera.m_vLookat.x;
				vCameraPos.y	= g_Pc.GetPlayer()->GetPosTM()._42+g_Camera.m_vEye.y-g_Camera.m_vLookat.y;
				vCameraPos.z	= g_Pc.GetPlayer()->GetPosTM()._43+g_Camera.m_vEye.z-g_Camera.m_vLookat.z;			
				float	_x		= (vCameraPos.x-pPosition->x)*(vCameraPos.x-pPosition->x);
				float	_y 		= (vCameraPos.y-pPosition->y)*(vCameraPos.y-pPosition->y);
				float	_z 		= (vCameraPos.z-pPosition->z)*(vCameraPos.z-pPosition->z);
				float	fRange	= sqrt(_x+_y+_z)/200.0f;

				//-----------------------------------------------------------------------------
				// ī�޶��ó��� ����� �鸰��
				//-----------------------------------------------------------------------------
				if( fRange <= 0.5f )
					pPosition = NULL;
				//-----------------------------------------------------------------------------
				// ī�޶󿡼� �ʹ� �ָ� �ִٸ� ���� �����Ѵ�
				//-----------------------------------------------------------------------------
				else if( fRange >= 3.0f )
					return;
				//-----------------------------------------------------------------------------
				// ī�޶���� �Ÿ��� ���� ó��
				//-----------------------------------------------------------------------------
				else
				{
					// �⺻�Ÿ��� ����
					fRange -= 0.5f;

					// ī�޶�� ��ü���� ����
					_x		= pPosition->x-vCameraPos.x;
					_y 		= pPosition->z-vCameraPos.z;
					int	Rot	= (int)(atan2(_y,_x)/ANGLE);

					// ���
					if( Rot == 0 )
						pPosition = NULL;
					// ���� ����ó��
					else if( Rot < 0 && Rot >= -20 )
						pPosition = NULL;
					else if( Rot <= -160 )
						pPosition = NULL;
					// ������ ����ó��
					else if( Rot > 0 && Rot <=  20 )
						pPosition = NULL;
					else if( Rot >=  160 )
						pPosition = NULL;
					// �����̱� ������ ������ ���ش�
					else if( Rot < 0 )
						fRange *= -1.0f;
					
					//-----------------------------------------------------------------------------
					// ���� ��ġ������ �������ϱ�	
					//-----------------------------------------------------------------------------
					if( pPosition != NULL )
					{
						pPosition->x = fRange;
						pPosition->y = fRange;
						pPosition->z = fRange;
					}
				}
*/
//			}		
		}

		static D3DXVECTOR3 vZero( 0.0f , 0.0f , 0.0f );
		//-----------------------------------------------------------------------------
		// �������
		//-----------------------------------------------------------------------------		
		if( pPosition == NULL )
		{
			m_p3DSound[_nIndex].SetPositionVelocity( m_pSoundTable[_nIndex].byCurt, &vZero, &vZero );
			//m_p3DSound[_nIndex].m_pDSListener->SetRolloffFactor( 0.0f  , DS3D_IMMEDIATE );
			m_p3DSound[_nIndex].m_pDSListener->SetRolloffFactor( 0.01f  , DS3D_IMMEDIATE );
		}
		else
		{
			m_p3DSound[_nIndex].SetPositionVelocity( m_pSoundTable[_nIndex].byCurt, &vZero, &vZero );
			//m_p3DSound[_nIndex].m_pDSListener->SetRolloffFactor( 0.01f  , DS3D_IMMEDIATE );
			m_p3DSound[_nIndex].m_pDSListener->SetRolloffFactor( 0.01f  , DS3D_IMMEDIATE );
			//�������� - �ּҰŸ��� �������� �Ÿ��� �ι辿 ����� ������ �ݾ� �ٿ��ش�.
		}

		//m_p3DSound->SetDistances( _nIndex , DS3D_MINDISTANCEFACTOR  , 10000.0f );

		// ����. ^ ^;; DuplicateSoundBuffer()
		//-----------------------------------------------------------------------------
		// �����ε�������
		//-----------------------------------------------------------------------------
		//for( int i = 0 ; i < )
		m_p3DSound[_nIndex].Stop( m_pSoundTable[_nIndex].byCurt );
		m_p3DSound[_nIndex].PlayWithPos( m_pSoundTable[_nIndex].byCurt, &vPos, 0, m_pSoundTable[_nIndex].bLoop );		
		
		if( m_ISNoPackLoad == TRUE )
		{
			if( g_RockClient.GetGameMode() == GMODE_NORMAL )
			{
				static BOOL ISDisplay = FALSE;
				
				if( pPosition != NULL )
				{
					static D3DXVECTOR3 vLen;
					
					vLen.x = pPosition->x - g_Pc.GetPlayer()->GetPosTM()._41;
					vLen.y = 0.0f;
					vLen.z = pPosition->z - g_Pc.GetPlayer()->GetPosTM()._43;
					
					if( D3DXVec3Length( &vLen ) > 100.0f )
					{
						ISDisplay = FALSE;
					}
					else
					{
						ISDisplay = TRUE;
					}
				}
				else
				{
					ISDisplay = TRUE;
				}
				
				if( ISDisplay )
				{
					switch( _nIndex )
					{
					case 0:
					case 1:
					case 2:
					case 54:
					case 77:
					case 128:
					case 129:
						break;
						
					default:
						{
							char TempStr[ 1024 ] = "";
							wsprintf( TempStr , "Play Sound Num : %d" , _nIndex );
							g_SystemPrintWnd.InsertSystemMsg((TCHAR *)TempStr, 0 );
						}
						break;
					}
				}
			}
			
		}
		
		// ���� �ð�����
		m_pSoundTable[_nIndex].lPreTime = g_nowTime;
		return;
	}
//	LeaveCriticalSection( &g_TexManager.m_csSoundFlag );
}

//............................................................................................................
// �÷��� �ϱ� ���� ���� ���� ���� �ϱ�
//............................................................................................................
int Sound_Manager::GetCurrentIndex(int index)
{
	if( NULL == ( m_p3DSound + index ) )
	{
		return FALSE;
	}	
	
	// ������� ���ۿ��� �÷��� ���� �ƴѰ� ������ ����
	for( int i = 0 ; i < m_p3DSound[ index ].m_NoSoundUse ; ++i )
	{
		if( !m_p3DSound[ index ].m_ppSound[ i ]->IsSoundPlaying() )
		{
			m_pSoundTable[ index ].byCurt = i;

			//DebugPrint("��� �ȵ� ���� �÷��� %d\n" , i );

			return TRUE;
		}
	}
	
	// ��� ���۰� �÷������϶� ( ��ü������ ������ �÷��� ���ؿ� )
	if( m_p3DSound[ index ].m_NoSoundUse >= m_pSoundTable[ index ].byCount )
		return FALSE;

	// ���� ���� �����

//	try
	{
		m_pSoundTable[ index ].byCurt = m_p3DSound[ index ].m_NoSoundUse;
		
		if( NULL == m_p3DSound[ index ].m_ppSound[ m_pSoundTable[ index ].byCurt ] )
		{
			m_p3DSound[ index ].m_ppSound[ m_pSoundTable[ index ].byCurt ] = SAFE_NEW( CSound );			
			
			m_p3DSound[ index ].m_ppSound[ m_p3DSound[ index ].m_NoSoundUse ]->m_apDSBuffer 
				= SAFE_NEW( LPDIRECTSOUNDBUFFER );				
			
			if( FAILED( 
				m_SoundManager.GetDirectSound()->DuplicateSoundBuffer(
				*m_p3DSound[ index ].m_ppSound[ 0 ]->m_apDSBuffer ,
				m_p3DSound[ index ].m_ppSound[ m_p3DSound[ index ].m_NoSoundUse ]->m_apDSBuffer ) ) )
			{ 

#ifdef _DEBUG

			DebugPrint("-- Sound_Manager::GetCurrentIndex(int index)  DuplicateSoundBuffer --\n" );
			
//			throw;
#endif
			
				SAFE_DELETE(m_p3DSound[ index ].m_ppSound[ m_pSoundTable[ index ].byCurt ]);

				return FALSE;
			}
			
			if( (*(m_p3DSound[ index ].m_ppSound[ m_p3DSound[ index ].m_NoSoundUse ]->m_apDSBuffer))->QueryInterface( 
				IID_IDirectSound3DBuffer , (VOID**)&m_p3DSound[ index ].m_ppDS3DBuffer[m_p3DSound[ index ].m_NoSoundUse] ) != DS_OK )
			{
#ifdef _DEBUG

			DebugPrint("-- Sound_Manager::GetCurrentIndex(int index)  QueryInterface --\n" );
			
//			throw;
#endif
				
				SAFE_DELETE(m_p3DSound[ index ].m_ppSound[ m_pSoundTable[ index ].byCurt ]);

				return FALSE;
			}
			else
			{
				DS3DBUFFER Ds3DBuffer;
				
				Ds3DBuffer.dwSize = sizeof(DS3DBUFFER);
				m_p3DSound[ index ].m_ppDS3DBuffer[m_p3DSound[ index ].m_NoSoundUse]->GetAllParameters(&Ds3DBuffer);
				Ds3DBuffer.dwMode = DS3DMODE_NORMAL;
				Ds3DBuffer.flMinDistance = 0.0f;	
				//���尡 �۾����� �����ϴ� �Ÿ��� ��Ÿ����.
				//Ds3DBuffer.flMaxDistance = DS3D_DEFAULTMAXDISTANCE;
				Ds3DBuffer.flMaxDistance = 10000.0f;
				//�� �̻� �Ҹ��� �۾������ʴ� ������ �ǹ��Ѵ�.

				m_p3DSound[ index ].m_ppDS3DBuffer[m_p3DSound[ index ].m_NoSoundUse]->SetAllParameters(&Ds3DBuffer, DS3D_IMMEDIATE);
			}
			
			m_p3DSound[ index ].m_ppSound[ m_p3DSound[ index ].m_NoSoundUse ]->m_dwNumBuffers = 1;
			
			//DebugPrint("���� ���� �÷��� %d\n" , m_p3DSound[ index ].m_NoSoundUse );

			m_p3DSound[ index ].m_NoSoundUse++;

			return TRUE;
		}
		else
		{

#ifdef _DEBUG

			DebugPrint("-- Sound_Manager::GetCurrentIndex(int index)  NULL != Buffer --\n" );
			
			throw;
#endif
		}
	}

//	catch(...)
//	{
//		return FALSE;
//	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Desc: ���� ���μ���
//-----------------------------------------------------------------------------
void Sound_Manager::Update()
{	
	// �ʱ�ȭ���ж�� ����ϸ�ȵȴ�
	if( m_bInit == false )
		return;
	// ȿ�����÷��� �����ִٸ�
	else if( ISWaveEnable() == false )
		return;					
	// �������϶��� ó���Ѵ�
	else if( g_RockClient.GetGameMode() != GMODE_NORMAL )
		return;	// ����������
	else if( g_RockClient.m_bGameEnd == true )
		return;

	CheckBattleMode();
}

//-----------------------------------------------------------------------------
// Desc: WAV ���� ��� ����
//-----------------------------------------------------------------------------
void Sound_Manager::Stop( int _nIndex )
{				
    m_p3DSound[_nIndex].StopAll();
}


void Sound_Manager::AllStop()
{		
	for(int i = 0; i < m_SoundCount; ++i )
		m_p3DSound[i].StopAll();	
}


//-----------------------------------------------------------------------------
// Desc: ���� �����ϱ�
//
// Copyright (c) 2004.1.2 By KStaR79
//-----------------------------------------------------------------------------
void	Sound_Manager::UpdateSound( int _nIndex )
{		
//	EnterCriticalSection( &g_TexManager.m_csSoundFlag );
/*	
	if( g_Sound.m_SoundCount <= _nIndex )
	{
#ifdef _DEBUG
		MessageBox( NULL , "UpdateSound More Num" , "Notice" , MB_OK );
#endif
	}
*/
	
	if( m_pSoundTable == NULL )
	{
		return;
	}
	
	if( m_pSoundTable[_nIndex].bIsLoaded == false && m_pSoundTable[_nIndex].bIsLoading == false )
	{		
		// �ε��� �÷��� ����
		m_pSoundTable[_nIndex].bIsLoading = true;
//		LeaveCriticalSection( &g_TexManager.m_csSoundFlag );

		// 3DCound ����
		m_p3DSound[_nIndex].Initialize( &m_SoundManager, (DWORD)m_pSoundTable[_nIndex].byCount );
		
		// ����� ���� ���� ����
		char	szFilePath[256] = {0,};

		BOOL	ISLoad = TRUE;
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		sprintf( szFilePath, "%s\\Sound\\%04d.wav", g_RockClient.GetRootDir(), _nIndex ); 

		//....................................................................................................
		// �� ��ƾ
		//....................................................................................................
/*
		for( int j = 0; j < (int)m_pSoundTable[_nIndex].byCount; ++j )
			m_p3DSound[_nIndex].ReadWaveFile( szFilePath, DSBCAPS_CTRL3D, 1 );
*/

		m_p3DSound[_nIndex].ReadWaveFile( szFilePath, DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME , 1 );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

		if( m_ISNoPackLoad )
		{
			sprintf( szFilePath, "%s\\Sound\\%04d.wav", g_RockClient.GetRootDir(), _nIndex ); 
			
			m_p3DSound[_nIndex].ReadWaveFile( szFilePath, DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME , 1 );

			if( ISLoad )
			{
				m_pSoundTable[_nIndex].bIsLoading	= false;
				m_pSoundTable[_nIndex].bIsLoaded	= true;
				m_pSoundTable[_nIndex].lPreTime		= g_nowTime;
			}
			else
			{
				m_pSoundTable[_nIndex].bIsLoading	= false;
				m_pSoundTable[_nIndex].bIsLoaded	= false;
				m_pSoundTable[_nIndex].lPreTime		= g_nowTime;
			}		

			return;
		}

		sprintf( szFilePath, "%04d.wav", _nIndex ); 

//		if( FAILED( m_p3DSound[_nIndex].ReadWaveFileFromMemory( szFilePath, DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME , 1 ) ) )
		if( FAILED( m_p3DSound[_nIndex].ReadWaveFileFromMemory( szFilePath, 
		    DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME , m_pSoundTable[_nIndex].byCount ) ) )
		{
			ISLoad = FALSE;
		}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

		
		// �÷���, �ð�����
//		EnterCriticalSection( &g_TexManager.m_csSoundFlag );

		if( ISLoad )
		{
			m_pSoundTable[_nIndex].bIsLoading	= false;
			m_pSoundTable[_nIndex].bIsLoaded	= true;
			m_pSoundTable[_nIndex].lPreTime		= g_nowTime;
		}
		else
		{
			m_pSoundTable[_nIndex].bIsLoading	= false;
			m_pSoundTable[_nIndex].bIsLoaded	= false;
			m_pSoundTable[_nIndex].lPreTime		= g_nowTime;
		}

//		LeaveCriticalSection( &g_TexManager.m_csSoundFlag );		
		return;
	}
//	LeaveCriticalSection( &g_TexManager.m_csSoundFlag );
}


//-----------------------------------------------------------------------------
// Desc: ���� ����ϴ��� �˻�
//
// Copyright (c) 2004.1.2 By KStaR79
//-----------------------------------------------------------------------------
void	Sound_Manager::UpdateSoundUse()
{
	for( int i = 0; i < m_SoundCount; ++i )
	{		
		// ����ð� �˻�
		long	lCheckTime;		
//		EnterCriticalSection( &g_TexManager.m_csSoundFlag );
		lCheckTime = SAFE_TIME_SUB( g_nowTime , m_lPreTime );
//		LeaveCriticalSection( &g_TexManager.m_csSoundFlag );

		// �Ӽ��� ���� ó��
		switch( m_pSoundTable[i].byAttr )
		{
		case DATA_DELETE0:		// ��������
			{
				if( lCheckTime >= SOUND_USE_TIME )
					continue;
			}
			break;
		case DATA_DELETE1:		// ��������
			{
				if( lCheckTime >= SOUND_USE_TIME*2 )
					continue;
			}
			break;
		default:
			continue;
		}

//		EnterCriticalSection( &g_TexManager.m_csSoundFlag );

		if( m_p3DSound[i].SafeDestroy() == true )
		{
			m_pSoundTable[i].bIsLoaded	= false;
			m_pSoundTable[i].bIsLoading	= false;
		}

//		LeaveCriticalSection( &g_TexManager.m_csSoundFlag );
	}
}


//-----------------------------------------------------------------------------
// Desc: ���� ���ð� �ʱ�ȭ
//
// Copyright (c) 2004.1.2 By KStaR79
//-----------------------------------------------------------------------------
void	Sound_Manager::InitSoundTime()
{
	for( int i = 0; i < m_SoundCount; ++i )
		m_pSoundTable[i].lPreTime = g_nowTime;
}


//-----------------------------------------------------------------------------
// Desc: mp3 ������ �����ϱ�
//-----------------------------------------------------------------------------
unsigned int __stdcall Msound( LPVOID p );

void Sound_Manager::SetMP3Volume(LONG Value)	
{ 
	m_MP3Volume     = Value; 
	m_MP3RealVolume = -50 * ( 100 - m_MP3Volume );
	
	m_BackSound.SetVolume( m_MP3RealVolume );
}

void Sound_Manager::SetWaveVolume(LONG Value)	
{ 
	m_WaveVolume = Value; 
	m_WaveRealVolume = -25 * ( 100 - m_WaveVolume );
	//m_WaveRealVolume = 50 * ( m_WaveVolume - 100 );

	//by simwoosung
	for(int i = 0; i < m_SoundCount; ++i )
	{
	   m_p3DSound[i].SetWaveVolume(m_WaveRealVolume);
	}
}


void Sound_Manager::PlayMP3( bool bForce , int _nMapIndex )
{	
	
	
#ifdef APPLY_BATTLE_BGSOUND
	
	if( m_IsNowBattleSound )
	{
		m_BackBattleSound.SetPause(false);
		StopBattleSound();	
		m_IsNowBattleSound = FALSE;
	}

#endif
	
	//-----------------------------------------------------------------------------
	// ��������÷��� �����ִٸ� ����
	//-----------------------------------------------------------------------------
	if( ISMP3Enable() == false )
		return;

	//-----------------------------------------------------------------------------
	// ���ε����� ���� ó��
	//-----------------------------------------------------------------------------
	int nIndex;

	LPENV_DATA pEnvSet = g_Map.m_SetEnv.GetSet();

	if( pEnvSet == NULL )
	{
		m_BackSound.Stop();
		m_BackSound.Destroy();

		return;
	}
	else
	{
		nIndex = pEnvSet->BGM;

		if( -1 == nIndex )
		{
			m_BackSound.Stop();
			m_BackSound.Destroy();

			return;
		}
	}

	// ���� �ε������ ����
	if( bForce == false && nIndex == m_GroundIndex )
	{
		return;
	}
	// �̹� ������ �ִ� ���� ������
	else
	{
		// ��°����� �迭ã��		
		m_BackSound.Stop();
		m_BackSound.Destroy();
	} 

	//-----------------------------------------------------------------------------
	// ���ε��� ����
	//-----------------------------------------------------------------------------
	m_GroundIndex = nIndex;

	//-----------------------------------------------------------------------------
	// �ش� ������� �ε��ϱ�		
	//-----------------------------------------------------------------------------
	m_BackSound.Init();
	m_BackSound.Read( nIndex );

	// �÷��� ����
	m_BackSound.m_bPlay = true;
	
	//-----------------------------------------------------------------------------
	// ���������		
	//-----------------------------------------------------------------------------	
	// by simwoosung
	//_beginthreadex�� ����Ҷ����� ���������  CloseHandle API�� �̿��Ͽ� 
	//�ݾ� �־�� ���ҽ����� �߻����� �ʽ��ϴ�.
	if(g_MP3)
	{
		CloseHandle(g_MP3);
		g_MP3 = NULL;
	}	

	Sleep( 100 );

	DWORD	dwThreadID;
	g_MP3 = ( HANDLE ) _beginthreadex( NULL, NULL, Msound, &m_BackSound, NULL, (unsigned*)&dwThreadID );
	return;
}

void Sound_Manager::PauseBattleSound()
{
	if( ISMP3Enable() == false )
	  return;

	m_BackBattleSound.Pause();
}

void Sound_Manager::StopBattleSound()
{
	if( ISMP3Enable() == false )
	  return;

	m_BackBattleSound.Stop();
}

void Sound_Manager::PlayBattleSound()
{
	if( ISMP3Enable() == false )
	  return;

	m_BackBattleSound.SetPause(false);

	// �÷��� ����
	m_BackBattleSound.m_bPlay = true;	
	//-----------------------------------------------------------------------------
	// ���������		
	//-----------------------------------------------------------------------------	
	// by simwoosung
	//_beginthreadex�� ����Ҷ����� ���������  CloseHandle API�� �̿��Ͽ� 
	//�ݾ� �־�� ���ҽ����� �߻����� �ʽ��ϴ�.
	if(g_MP3)
	{
		CloseHandle(g_MP3);
		g_MP3 = NULL;
	}

	DWORD	dwThreadID;
	g_MP3 = ( HANDLE ) _beginthreadex( NULL, NULL, Msound, &m_BackBattleSound, NULL, (unsigned*)&dwThreadID );
}

void Sound_Manager::PauseBGSound()
{
	if( ISMP3Enable() == false )
	  return;

	m_BackSound.Pause();
}

void Sound_Manager::StopBGSound()
{
	if( ISMP3Enable() == false )
	  return;

	m_BackSound.Stop();
}

void Sound_Manager::PlayBGSound()
{
	if( ISMP3Enable() == false )
	  return;

	m_BackSound.SetPause(false);
	
	// �÷��� ����
	m_BackSound.m_bPlay = true;	
	//-----------------------------------------------------------------------------
	// ���������		
	//-----------------------------------------------------------------------------	
	// by simwoosung
	//_beginthreadex�� ����Ҷ����� ���������  CloseHandle API�� �̿��Ͽ� 
	//�ݾ� �־�� ���ҽ����� �߻����� �ʽ��ϴ�.
	if(g_MP3)
	{
		CloseHandle(g_MP3);
		g_MP3 = NULL;
	}

	DWORD	dwThreadID;
	g_MP3 = ( HANDLE ) _beginthreadex( NULL, NULL, Msound, &m_BackSound, NULL, (unsigned*)&dwThreadID );
}

//-----------------------------------------------------------------------------
// Desc: Ŭ���� �ʱ�ȭ
//-----------------------------------------------------------------------------
void	CMsound::Clear()
{
	pGraph	= NULL;	
	pEvent	= NULL;
	pSeek	= NULL;
	pMediaControl = NULL;
	m_bPlay	= false;
	SetStop( true );
}

void	CMsound::Destroy()
{	
	// �޸𸮻���
	EnterCriticalSection( &m_csMP3Play );

	CoUninitialize();
	Clear();

	LeaveCriticalSection( &m_csMP3Play );
}


//-----------------------------------------------------------------------------
// Desc: MP3 ����
//-----------------------------------------------------------------------------
void	CMsound::Stop()
{
	SetStop( true );
	EnterCriticalSection( &m_csMP3Stop );
	if( pMediaControl != NULL )	
		pMediaControl->Stop();	
	LeaveCriticalSection( &m_csMP3Stop );
}

//-----------------------------------------------------------------------------
// Desc: MP3 ����
//-----------------------------------------------------------------------------
void	CMsound::Pause()
{
	SetPause( true );
	EnterCriticalSection( &m_csMP3Stop );
	if( pMediaControl != NULL )	
		pMediaControl->Pause();	
	LeaveCriticalSection( &m_csMP3Stop );
}

//-----------------------------------------------------------------------------
// Desc: MP3 ���� �ʱ�ȭ
//-----------------------------------------------------------------------------
void	CMsound::Init()
{	
	CoInitialize(NULL);
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **) &pGraph);
	pGraph->QueryInterface(IID_IMediaControl, (void **) &pMediaControl);		
	pGraph->QueryInterface(IID_IMediaEvent, (void **) &pEvent);	
	pGraph->QueryInterface(IID_IMediaSeeking, (void **) &pSeek);		
}

// ���⼭ MP3 ȭ�� �����ض�~ ��~
void	CMsound::Read( int _nIndex )
{
	// ���ϰ�θ� �����ڵ�� ���� 
	TCHAR	szFileName[1024] = "";
	
	wsprintf( szFileName , "%s\\%d.mp3" , g_RockClient.GetRootDir() , _nIndex );
	
	WCHAR	wFileName[256];	
 	MultiByteToWideChar( CP_ACP, 0, szFileName, -1, wFileName, 256 );

	// �����б�
	pGraph->RenderFile( wFileName, NULL );
}


//-----------------------------------------------------------------------------
// Desc: MP3 �÷���
//-----------------------------------------------------------------------------
int		CMsound::Play()
{	
	//-----------------------------------------------------------------------------
	// ��������
	//-----------------------------------------------------------------------------
	SetVolume( g_Sound.m_MP3RealVolume );

	//-----------------------------------------------------------------------------
	// �����÷��� ����
	//-----------------------------------------------------------------------------
	HRESULT hr = pMediaControl->Run();	
	if( hr != S_OK )
	{
		return 0;	
	}

	long a,b,c;
	for(;;)
	{				
		//-----------------------------------------------------------------------------
		// ��������� ����Ǿ����� �˻�
		//-----------------------------------------------------------------------------
		if( GetStop() == true )
		{
			return -1;
		}

		if( GetPause() == true )
		{
			return -2;
		}

		//-----------------------------------------------------------------------------
		// �����̺�Ʈ �ޱ�
		//-----------------------------------------------------------------------------
		if( pEvent != NULL )
			pEvent->GetEvent( &a, &b, &c, 100);
			
		//-----------------------------------------------------------------------------
		// ��������� ����Ǿ����� �˻�
		//-----------------------------------------------------------------------------
		if( GetStop() == true )
		{
			return -1;
		}

		if( GetPause() == true )
		{
			return -2;
		}

		//-----------------------------------------------------------------------------
		// ���ǿ��ְ� ������.. 		
		// �ٽ� Ʋ��߱� ������.. 1�� �����ؼ� �����͸� ��ó������ ������
		//-----------------------------------------------------------------------------
		if( a == MP3_STOP )			
			return 1;

		//-----------------------------------------------------------------------------
		// ��������� ����Ǿ����� �˻�
		//-----------------------------------------------------------------------------
		if( GetStop() == true )
		{
			return -1;
		}

		if( GetPause() == true )
		{
			return -2;
		}

		//-----------------------------------------------------------------------------
		// ��������Ǿ����� �˻�
		//-----------------------------------------------------------------------------
		if( g_RockClient.m_bGameEnd == true && m_bPlay == true )				
			Stop();
		
		//-----------------------------------------------------------------------------
		// ��������� ����Ǿ����� �˻�
		//-----------------------------------------------------------------------------
		if( GetStop() == true )
		{
			return -1;
		}

		if( GetPause() == true )
		{
			return -2;
		}

		//-----------------------------------------------------------------------------
		// �̺�Ʈ���ٸ� �����Ű��
		//-----------------------------------------------------------------------------
		if( pEvent != NULL )
		{
			pEvent->FreeEventParams( a, b, c );
		}

		Sleep( 1 );
	}
		
	//LeaveCriticalSection( &m_csMP3Play );
	return 0;
}

unsigned int __stdcall Msound(LPVOID sound)
{
	//-----------------------------------------------------------------------------
	// ������� ������ �ޱ�
	//-----------------------------------------------------------------------------
	CMsound* mp = (CMsound*)sound;	
	mp->SetStop( false );
	EnterCriticalSection( &mp->m_csMP3Play );

	//-----------------------------------------------------------------------------
	// �뷡ó������ ������
	//-----------------------------------------------------------------------------
	LONGLONG cur = 0;	
	mp->pSeek->SetPositions( &cur, AM_SEEKING_AbsolutePositioning, &cur, AM_SEEKING_SeekToKeyFrame );
	
	int Res = -1; 
	
	//-----------------------------------------------------------------------------
	// mp3 ���
	//-----------------------------------------------------------------------------
	for(;;)
	{		
		Res = mp->Play();

		// ���������� ����
		if( Res == -1 )		
		{
			break;
		}
		
		if( Res == -2 )
		{
			break;
		}

		// �뷡�� ������ ����Ǽ�.. �ٽ� ó������ ���;� �Ѵ�
		else if( Res == 1 )
		{
			mp->pSeek->SetPositions( &cur, AM_SEEKING_AbsolutePositioning, &cur, AM_SEEKING_SeekToKeyFrame );
		}
		
		Sleep( 1 );
	}

	//-----------------------------------------------------------------------------
	// mp3 ����
	//-----------------------------------------------------------------------------	
	
	mp->m_bPlay = false;
	
	if( Res == -1 )
	{
		mp->SetStop( true );
	}
	
	LeaveCriticalSection( &mp->m_csMP3Play );

	return 0;
}


//-----------------------------------------------------------------------------
// Desc: ������� ��������
//-----------------------------------------------------------------------------
HRESULT CMsound::SetVolume( long lVolume )
{
	if( g_Sound.ISMP3Enable() == FALSE )
		return S_OK;

    HRESULT hr = S_OK;
    IBasicAudio *pBasicAudio = NULL;

    long lVolumeSrc;
    if ( lVolume < VOLUME_SILENCE )
        lVolume = VOLUME_SILENCE;
    else if ( lVolume > VOLUME_FULL )
        lVolume = VOLUME_FULL;  

    if ( !pGraph )
        return S_OK;
    hr =  pGraph->QueryInterface( IID_IBasicAudio, (void **)&pBasicAudio );
    if ( FAILED(hr) )
        return S_OK;

    // Read current volume
    hr = pBasicAudio->get_Volume( &lVolumeSrc );
    if ( hr == E_NOTIMPL )
    {
        // Fail quietly if this is a video-only media file
        pBasicAudio->Release();
        return hr;
    }
    else if ( FAILED( hr ) )
    {
		pBasicAudio->Release();
        return hr;
    }

    // Set new volume
    hr = pBasicAudio->put_Volume( lVolume );
    if ( FAILED(hr) )
    {
//        RetailOutput(TEXT("Failed in pBasicAudio->put_Volume!  hr=0x%x\r\n"), hr);
    }
    pBasicAudio->Release();
    return hr;
}