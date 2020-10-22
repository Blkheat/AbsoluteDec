   #include "RockPCH.h"
#include "VFileSystem.h"

#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
////#include	"..\Network.H"
//#include	"..\Input_Manager.H"
#include	"quadlist.h"
#include	"Camera.H"
#include	"Pc.h"
#include	"RockClient.h"
#include	<EFFECT\\Particle.h>
#include	"Effect.h"
//#include	"ChatList.h"
////#include	"Menu_Interface.h"
//#include	"Weather_Manager.H"
#include	<Obj\\TextureManager.H>
#include	<SOUND\\Sound.h>
////#include	"list.h"
#include	<cassert>
#include	"profile\profile.h"
#include	"profile\custom_time.h"


#include "global.h"

#include "CRenderManager.h"
#include "DelayActList.h"

#include "RockInterface\\NonPlayerInfoWnd.h"


extern		bool	g_bBlockDebug;
extern		bool	g_bmp3Out;
extern		long	g_nowTime;


#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern		LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern		CRITICAL_SECTION	g_csMP3Flag;
extern		TEXTURE_MANAGER		g_TexManager;

void		SetupMaxMin(D3DXVECTOR3* max, D3DXVECTOR3* min);

#define		SELECTBOX_ANITIME			25f			// 0.025초
#define		SELECTBOX_ENDFRAME			10
#define		SELECTBOX_SCALESIZE			0.01f

#define		IMOTICON_SIZE				64			// 이모티콘사이즈
#define		IMOTICON_PLAYTIME			4000.0f		// 이모티콘 그리는 시간 4초
#define		IMOTICON_ANICOUNT			2			// 이모티콘 애니갯수
		
#define		SELECTMAP_POS				16
#define 	SELECTMAP_HEIGHT			2.1f
#define		MESSAGEBOX_YES				6

#define		OPERATOR_PASSWORD			1004		// 운영자 비밀번호


//-----------------------------------------------------------------------------
// Desc: 검기(잔상)생성
//-----------------------------------------------------------------------------
void	TextureTable::Create()
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 검기 메모리 할당
	char file_path[ 256 ] = {0,};
	strcpy( file_path, g_RockClient.GetRootDir());
	strcat( file_path, "\\Etc\\afterimage1.tex");
	
	g_RenderManager.CreateTextureFromFileEx( file_path , D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
											 D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
											 D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
											 0 , NULL, NULL, &pAfterImage1 );
	
	strcpy( file_path, g_RockClient.GetRootDir());
	strcat( file_path, "\\Etc\\afterimage2.tex");

	g_RenderManager.CreateTextureFromFileEx( file_path , D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
											 D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
											 D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
											 0 , NULL, NULL, &pAfterImage2 );

	strcpy( file_path, g_RockClient.GetRootDir());
	strcat( file_path, "\\Etc\\afterimage3.tex");
	
	g_RenderManager.CreateTextureFromFileEx( file_path , D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
											 D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
											 D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
											 0 , NULL, NULL, &pAfterImage3 );


	g_RenderManager.CreateTextureFromFile( file_path, &pAfterImage3 );

	// 그림자 텍스쳐 메모리 할당
	strcpy( file_path, g_RockClient.GetRootDir());
	strcat( file_path, "\\Etc\\shadow.tex");
	g_RenderManager.CreateTextureFromFileEx( file_path, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
											 D3DX_FILTER_NONE, 0, NULL, NULL, &pShadow );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

		// 검기 메모리 할당
	char file_path[ 256 ] = {0,};

	wsprintf( file_path, "%s" , "afterimage1.tex" );
	VFileHandle* pFH = g_VFEtc.OpenFile( file_path );
	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &pAfterImage1 );
		g_VFEtc.CloseFile( pFH );
	}
	
	wsprintf( file_path, "%s" , "afterimage2.tex" );
	pFH = g_VFEtc.OpenFile( file_path );
	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &pAfterImage2 );
		g_VFEtc.CloseFile( pFH );
	}

	wsprintf( file_path, "%s" , "afterimage3.tex" );	
	pFH = g_VFEtc.OpenFile( file_path );
	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &pAfterImage3 );
		g_VFEtc.CloseFile( pFH );
	}

	// 그림자 텍스쳐 메모리 할당
	wsprintf( file_path, "%s" , "shadow.tex" );
	pFH = g_VFEtc.OpenFile( file_path );
	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &pShadow );
		g_VFEtc.CloseFile( pFH );
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}


//-----------------------------------------------------------------------------
// Desc: 그림자 테이블
//-----------------------------------------------------------------------------
void ShadowTable::Clear()
{
	// 제일작다

	// 작다

	// 중간
	matMiddle._11 = 3.01883f;
	matMiddle._12 = 0.0f;
	matMiddle._13 = 0.0f;
	matMiddle._14 = 0.0f;
	matMiddle._21 = 0.0f;
	matMiddle._22 = 3.49848f;
	matMiddle._23 = 0.0f;
	matMiddle._24 = 0.0f;
	matMiddle._31 = 0.0f;
	matMiddle._32 = 0.0f;
	matMiddle._33 = 1.0f;
	matMiddle._34 = 1.0f;
	matMiddle._41 = 0.0f;
	matMiddle._42 = 0.0f;
	matMiddle._43 = -1.0f;
	matMiddle._44 = 0.0f;

	// 크다

	// 매우 크다
}


//-----------------------------------------------------------------------------
// Desc: CBravoPC 초기화
//-----------------------------------------------------------------------------
CBravoPC::CBravoPC()
{
	Init(0);
	InitTable();
	m_Select.Clear();
	m_TexTable.Clear();
	m_ShadowTable.Clear();
	InitMapInfo();

	m_bChatType	= 0;
	m_bOperator = false;
	memset( &m_bTempPlayer, false, sizeof(m_bTempPlayer) );

	// 이펙트텍스쳐
	m_pEffectTex = NULL;
}

CBravoPC::~CBravoPC()
{
	FinalCleanup();	
	m_Select.Destroy();
	m_TexTable.Destroy();
	
	SAFE_DELETE_ARRAY( m_pEffectTex );
}

HRESULT		CBravoPC::InvalidateDeviceObject()
{
	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		m_Player.m_SwordTail.InvalidateDeviceObject();		
	}
	int i = 0;
	for( i = 0; i < MAX_USER_INTABLE; ++i )
	{			
	    m_Pc[i].m_SwordTail.InvalidateDeviceObject();
	}

	for( i = 0; i <= MAX_USER_INTABLE; ++i )
	{		
		if( i == MAX_USER_INTABLE )
			m_Player.m_Shadow.Clear();
		else
		{
			m_Pc[i].m_Shadow.Clear();		
		}
	}

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CBravoPC::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT		CBravoPC::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	//m_SwordTail는 필요시 버퍼를 자동생성
	return S_OK;
}

bool		CBravoPC::Init(int size)
{
	m_PcTableCount	= 0;
	m_cSize	= size;
	m_cSize = 256;

	InitPC(m_MainPC);
	InitTable();

	m_nPickedUser		= -1;		// 현재 선택 유저
	lReceiveBlock		= -1;		// 서버에 받은 내위치
	m_DisplayQuest		= false;	// 퀘스트 그려?
	m_CurrentQuest		= 0;		// 현재 선택 퀘스트(QUEST_CLASS중에 하나)
	m_MakePlayerIndex	= -1;		// 캐릭터 생성 인덱스
	memset( &m_TempUnite, -1, sizeof(m_TempUnite) );
	return	true;
}

void		CBravoPC::FinalCleanup()
{
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		m_Pc[i].Destroy();
	}
}

/***********************************************************
	* COMMON
***********************************************************/
void		CBravoPC::InitPC(PCTABLE& pc)
{
	pc.Init();
}


/***********************************************************
	* MAIN PC
***********************************************************/

/***********************************************************
	* PC TABLE
***********************************************************/
void		CBravoPC::InitTable()
{
	//m_PcTableCount	= 0;
	for ( int i = 0; i < MAX_USER_INTABLE; i ++ )
		m_PcTable[i].Init();
}

int			CBravoPC::FindUser(long unique)
{
	for ( int i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if ( m_PcTable[i].lUnique == unique )
			return i;
	}
	//return -1;
	return	MAX_USER_INTABLE;
}

PCTABLE	* CBravoPC::GetNextPcTable(int & nindex)
{
	int nSindex = nindex + 1;
	
	for( int i = nSindex; i < MAX_USER_INTABLE; ++i )
	{
		if( m_Pc[i].m_nTableIndex != -1 )
		{
			nindex = i;
			return &m_PcTable[i];			
		}		
	}

	nindex = -1;
	return NULL;
}

// by wxywxy
PCTABLE	*CBravoPC::FindPcTable(Character *pSrc)
{
	// 플레이어 일때
	if( GetMainPCUniqe() == pSrc->GetUnique() )
	{
		return &m_MainPC;
	}
	// 다른 유저 일때
	else
	{
		for( int i = 0; i < MAX_USER_INTABLE; ++i )
		{
			if( m_PcTable[i].lUnique == pSrc->GetUnique() )
			{
				return &m_PcTable[i];
			}
		}
	}

	return NULL;
}


NonPlayer*		CBravoPC::GetUser(long unique)
{
	for( int i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			if( m_Pc[i].m_nTableIndex == -1 )
				return NULL;

			return &m_Pc[i];
		}
	}
	return NULL;
}

//by simwoosung
PCTABLE	*CBravoPC::FindPcTable(long unique)
{
   for( int i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			if( m_Pc[i].m_nTableIndex == -1 )
				return NULL;

			return &m_PcTable[i];
		}
	}
	return NULL;
}

PCTABLE	*CBravoPC::FindPcTable(TCHAR * strName)
{
   for( int i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if( Rstrcmp( m_PcTable[i].char_info.thePCName, strName ) == 0 )
		{
			if( m_Pc[i].m_nTableIndex == -1 )
				return NULL;

			return &m_PcTable[i];
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Desc: NPC 위치 리턴
//-----------------------------------------------------------------------------
long	CBravoPC::GetPcPos(long unique)
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		return m_MainPC.lDestBlock;
	}
	
	for ( int i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if ( m_PcTable[i].lUnique == unique )
		{
			return 	m_PcTable[i].lDestBlock;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------
// Desc: 유저 이름, 성별
//-----------------------------------------------------------------------------
char*	CBravoPC::GetPcName( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
		return m_MainPC.char_info.ID;
	for ( int i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if( m_PcTable[i].lUnique == unique )
			return m_PcTable[i].char_info.ID;
	}
	return NULL;
}

char*	CBravoPC::GetPcName( int index )
{
	if( index == -1 || index == MAX_USER_INTABLE )
		return m_MainPC.char_info.ID;
	else if( m_PcTable[index].char_info.ID != NULL )
		return m_PcTable[index].char_info.ID;

	return NULL;
}

long	CBravoPC::GetNameToUnique( char* szName )
{
	if( strcmp( szName, m_MainPC.char_info.ID ) == 0 )
		return m_MainPC.lUnique;
	for ( int i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if( strcmp( szName, m_PcTable[i].char_info.ID ) == 0 )
			return m_PcTable[i].lUnique;
	}
	return -1;
}


//-----------------------------------------------------------------------------
// Desc: 남성, 여성
//-----------------------------------------------------------------------------
int		CBravoPC::GetPcSex( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
		return m_MainPC.char_info.sex;

	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			return m_PcTable[i].char_info.sex;
		}
	}
	
	return -1;
}




D3DXMATRIX	CBravoPC::GetPosTM( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		return m_Player.GetPosTM();
	}
		
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			return m_Pc[i].GetPosTM();
		}
	}
	return m_Player.GetPosTM();
}

D3DXMATRIX	CBravoPC::GetPosEyeTM( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		return m_Player.GetPosEyeTM();
	}
		
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			return m_Pc[i].GetPosEyeTM();
		}
	}
	return m_Player.GetPosEyeTM();
}

D3DXMATRIX	CBravoPC::GetPosBoneCenterTM( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		return m_Player.GetPosCenterBoneTM();
	}
		
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			return m_Pc[i].GetPosCenterBoneTM();
		}
	}
	return m_Player.GetPosCenterBoneTM();
}

D3DXVECTOR3	CBravoPC::GetvPos( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		return D3DXVECTOR3( m_Player.GetPosTM()._41 , m_Player.GetPosTM()._42 , m_Player.GetPosTM()._43 );
	}
		
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			return D3DXVECTOR3( m_Pc[i].GetPosTM()._41 , m_Pc[i].GetPosTM()._42 , m_Pc[i].GetPosTM()._43 );
		}
	}
	
	return D3DXVECTOR3( m_Player.GetPosTM()._41 , m_Player.GetPosTM()._42 , m_Player.GetPosTM()._43 );
}

D3DXVECTOR3	CBravoPC::GetvBonePos( long unique , int BoneNum )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		return D3DXVECTOR3( m_Player.GetBoneRealTM( BoneNum )._41 , m_Player.GetBoneRealTM( BoneNum )._42 , m_Player.GetBoneRealTM( BoneNum )._43 );
	}
		
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			return D3DXVECTOR3( m_Pc[i].GetBoneRealTM( BoneNum )._41 , m_Pc[i].GetBoneRealTM( BoneNum )._42 , m_Pc[i].GetBoneRealTM( BoneNum )._43 );
		}
	}
	
	return D3DXVECTOR3( m_Player.GetBoneRealTM( BoneNum )._41 , m_Player.GetBoneRealTM( BoneNum )._42 , m_Player.GetBoneRealTM( BoneNum )._43 );
}


D3DXMATRIX	CBravoPC::GetHandTM( int type, long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		if( type == 0 )
			return m_Player.GetRHandTM();
		else
			return m_Player.GetLHandTM();
	}
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			if( type == 0 )
				return m_Pc[i].GetRHandTM();
			else
				return m_Pc[i].GetLHandTM();
		}
	}

	DebugPrint( "Not Find %d = CBravoPC::GetHandTM\n" , unique );
	
	D3DXMATRIX mat;

	D3DXMatrixIdentity( &mat );

	mat._41 = -1000;
	mat._42 = -1000;
	mat._43 = -1000;
	
	return mat;
}

//...............................................................................................................
// 이펙트 출력시 본 메트릭스 구하기 ( NPC 도 추가 )
//...............................................................................................................
int CBravoPC::GetBoneLinkTM(long Unique,int Index,int Num,D3DXMATRIX &mat)
{
	Character	*pSrc      = NULL;

	// NPC 
	if( -1 != Index )
	{
		int NpcIndex;
		
		if( ( NpcIndex = g_Map.FindNpc( Unique ) ) == MAX_NPC_INTABLE )
		{
			return false;
		}

		pSrc = ( Character * ) &g_Map.m_Npc[ NpcIndex ];
	}
	else
	{
		if( m_MainPC.lUnique == Unique || Unique == -1 )
		{
			pSrc	  = ( Character * ) &m_Player;
		}
		else
		{
			for( int i = 0; i < MAX_USER_INTABLE; ++i )
			{
				if( m_PcTable[i].lUnique == Unique )
				{
					pSrc = ( Character * ) &m_Pc[i];
				}
			}
		}
	}
	

	// 캐릭터를 못 찾았을때
	if( pSrc == NULL )
		return FALSE;

 	switch( Num )
	{
		//.......................................................................................................
		// 링크된 본 없음
		//.......................................................................................................
		case 0:
			
			D3DXMatrixIdentity( &mat );

			return TRUE;

		//.......................................................................................................
		// 머리
		//.......................................................................................................
		case 1:

			mat = pSrc->m_matRealHair;

			return TRUE;

		//.......................................................................................................
		// 가슴
		//.......................................................................................................
		case 2:

			mat = pSrc->m_matRealbreast;

			return TRUE;

		//.......................................................................................................
		// 낭심
		//.......................................................................................................
		case 3:

			mat = pSrc->m_matRealCenter;

			return TRUE;

		//.......................................................................................................
		// 왼손
		//.......................................................................................................
		case 4:

			mat = pSrc->m_matRealLHand;

			return TRUE;

		//.......................................................................................................
		// 오른손
		//.......................................................................................................
		case 5:

			mat = pSrc->m_matRealRHand;

			return TRUE;

		//.......................................................................................................
		// 왼발
		//.......................................................................................................
		case 6:

			mat = pSrc->m_matRealLFoot;

			return TRUE;

		//.......................................................................................................
		// 오른발
		//.......................................................................................................
		case 7:

			mat = pSrc->m_matRealRFoot;

			return TRUE;

		//.......................................................................................................
		// 칼끝
		//.......................................................................................................
		case 8:

			D3DXMatrixIdentity( &mat );

			if( pSrc->m_pUserInfo == NULL )
				return FALSE;

			mat._41 = pSrc->m_pUserInfo->vSwordAxis[ 0 ].x;
			mat._42 = pSrc->m_pUserInfo->vSwordAxis[ 0 ].y;
			mat._43 = pSrc->m_pUserInfo->vSwordAxis[ 0 ].z;

			return TRUE;

		//.......................................................................................................
		// 칼끝
		//.......................................................................................................
		case 9:

			D3DXMatrixIdentity( &mat );

			if( pSrc->m_pUserInfo == NULL )
				return FALSE;

			mat._41 = pSrc->m_pUserInfo->vSwordAxis[ 1 ].x;
			mat._42 = pSrc->m_pUserInfo->vSwordAxis[ 1 ].y;
			mat._43 = pSrc->m_pUserInfo->vSwordAxis[ 1 ].z;

			return TRUE;
	}


	return FALSE;
}


D3DXMATRIX	CBravoPC::GetHandRealTM( int type, long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		if( type == 0 )
			return m_Player.GetRealRHandTM();
		else
			return m_Player.GetRealLHandTM();
	}
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			if( type == 0 )
				return m_Pc[i].GetRealRHandTM();
			else
				return m_Pc[i].GetRealLHandTM();
		}
	}
	
	DebugPrint( "Not Find %d = CBravoPC::GetHandTM\n" , unique );

	D3DXMATRIX mat;

	D3DXMatrixIdentity( &mat );

	mat._41 = -1000;
	mat._42 = -1000;
	mat._43 = -1000;

	return mat;
}

int	CBravoPC::GetDestX()
{
	if(m_Player.m_nViewRefDestIndex != -1)
	{
		int nIndex = m_Player.m_nViewRefDestIndex;
		return m_PcTable[nIndex].lDestBlock%m_cSize;	
	}
	return m_MainPC.lDestBlock%m_cSize;
}
					
int	CBravoPC::GetDestY()
{
	if(m_Player.m_nViewRefDestIndex != -1)
	{
		int nIndex = m_Player.m_nViewRefDestIndex;
		return m_PcTable[nIndex].lDestBlock/m_cSize;	
	}
	return m_MainPC.lDestBlock/m_cSize;
}					

int	CBravoPC::GetDestX( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
		return GetDestX();
	int i = 0;
	for(  i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
			break;
	}
	return m_PcTable[i].lDestBlock%m_cSize;
}

int	CBravoPC::GetDestY( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
		return GetDestY();
	int i = 0;
	for(  i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
			break;
	}
	return m_PcTable[i].lDestBlock/m_cSize;
}

//-----------------------------------------------------------------------------
// Desc: 바운드박스값 리턴
// 0최대, 1최소, 2(최대-최소)/2
//-----------------------------------------------------------------------------
D3DXVECTOR3	CBravoPC::GetBoundMax( long unique, BYTE type )
{
	D3DXVECTOR3 center;
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		if( type == 0 )
			return m_Player.GetBoundMaxPos();
		else if( type == 1 )
			return m_Player.GetBoundMinPos();
		else if( type == 2 )
		{
			center = (m_Player.GetBoundMaxPos() - m_Player.GetBoundMinPos())/2.0f;
			return center;
		}
	}
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			if( type == 0 )
				return m_Pc[i].GetBoundMaxPos();
			else if( type == 1 )
				return m_Pc[i].GetBoundMinPos();
			else if( type == 2 )
			{
				center = (m_Pc[i].GetBoundMaxPos() - m_Pc[i].GetBoundMinPos())/2.0f;
				return center;
			}
		}
	}
	if( type == 0 )
		return m_Player.GetBoundMaxPos();
	return m_Player.GetBoundMinPos();
}

float	CBravoPC::GetBoundSize( long unique )
{
	D3DXVECTOR3 v;
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		v = (m_Player.GetBoundMaxPos() - m_Player.GetBoundMinPos())/2.0f;
		return (float)sqrt((double)(v.x*v.x + v.y*v.y + v.z*v.z));
	}

	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
		{
			v = (m_Pc[i].GetBoundMaxPos() - m_Pc[i].GetBoundMinPos())/2.0f;
			return (float)sqrt((double)(v.x*v.x + v.y*v.y + v.z*v.z));
		}
	}

	return 0.0f;
}

float CBravoPC::GetPcHeight( long unique )
{
	D3DXVECTOR3	vHeight;
	if( m_MainPC.lUnique == unique || unique == -1 )
	{
		vHeight = ( m_Player.GetBoundMaxVec() - m_Player.GetBoundMinVec() );
	}
	else
	{
		for( int i = 0; i < MAX_USER_INTABLE; ++i )
		{
			if( m_PcTable[i].lUnique == unique )
			{
				vHeight = ( m_Pc[i].GetBoundMaxVec() - m_Pc[i].GetBoundMinVec() );
			}
		}
	}

	return vHeight.y;
}

D3DXVECTOR3 CBravoPC::GetPcInnerPos( int index, long unique, float height )
{
	D3DXMATRIX mat;
	float npcheight;
	D3DXVECTOR3 returnpos;

	if( index == -1 )
	{
		mat	= GetPosTM( unique );
		npcheight	= GetPcHeight( unique );
		returnpos = D3DXVECTOR3( mat._41, mat._42 + npcheight * height, mat._43 );
	}
	else
	{
		mat	= g_Map.GetNpcMatrix( index, unique );
		npcheight	= g_Map.GetNpcBoundMax( index, unique ).y;
		returnpos = D3DXVECTOR3( mat._41, mat._42 + npcheight * height, mat._43 );
	}

	return returnpos;
}


D3DXVECTOR3 CBravoPC::GetPcCenterBoneInnerPos( int index, long unique, float height )
{
	D3DXMATRIX mat;
	float npcheight;
	D3DXVECTOR3 returnpos;

	if( index == -1 )
	{
		mat	= GetPosBoneCenterTM( unique );
		npcheight	= GetPcHeight( unique );
		returnpos = D3DXVECTOR3( mat._41, mat._42 + npcheight * height, mat._43 );
	}
	else
	{
		mat	= g_Map.GetNpcCenterBoneMatrix( index, unique );
		npcheight	= g_Map.GetNpcBoundMax( index, unique ).y;
		returnpos = D3DXVECTOR3( mat._41, mat._42 + npcheight * height, mat._43 );
	}

	return returnpos;
}

//-----------------------------------------------------------------------------
// Desc: 유저 보이는지 체크
//-----------------------------------------------------------------------------
bool CBravoPC::GetVisible( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
		return true;

	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
			return m_Pc[i].m_bIsVisible;
	}

	return false;
}

float CBravoPC::GetDirection( long unique )
{
	if( m_MainPC.lUnique == unique || unique == -1 )
		return m_Player.GetDirection();
	int i = 0;
	for(  i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == unique )
			break;
	}
	return m_Pc[i].GetDirection();
}


//-----------------------------------------------------------------------------
// Desc: 서버에 보내기
//-----------------------------------------------------------------------------
/*
void		CBravoPC::CM__WEATHER( int type )
{
}
*/

//-----------------------------------------------------------------------------
// Desc: 자기캐릭터 선택화면에서 사용할 캐릭터 생성하기
//-----------------------------------------------------------------------------
void	CBravoPC::CreateTempPlayer()
{
/*	// 비어있는 배열번호찾기
	int nIndex;
	for( int i = 0; i < MAX_CHARACTER_COUNT; ++i )
	{
		if( m_bTempPlayer[i] == false )
		{
			nIndex = i;
			m_bTempPlayer[i] = true;
			break;
		}
	}
	if( i >= MAX_CHARACTER_COUNT )
		return;

	// 생성창 캐릭터 없음
	g_Pc.m_MakePlayerIndex = -1;

	// 종족, 직업찾기
	int temp, sex, race, job;
//	temp = g_Net.m_SaveInfo[nIndex].m_Race;
//	sex  = (temp >> CHAR_SHIFT_KEY_SEX) & CHAR_KEY;
//	temp = g_Net.m_SaveInfo[nIndex].m_Race;	
//	race = (temp >> CHAR_SHIFT_KEY_RACE) & CHAR_KEY;
//	temp = g_Net.m_SaveInfo[nIndex].m_Race;
//	job  = temp & CHAR_KEY;	

	// 캐릭터 속성저장
	m_TempIndex[nIndex] = SetCharacterAttr( job, race, sex );

	// 캐릭터생성
	m_TempPlayer[nIndex].Init();
	m_TempPlayer[nIndex].m_nTableIndex = nIndex;
	m_TempPlayer[nIndex].CreatePlayer( 1 );

	// 머리색, 머리모양, 얼굴모양 셋팅
//	ChangePlayerStyle( g_Net.m_SaveInfo[nIndex].m_Style, nIndex );

	// 캐릭터위치
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );
	switch( g_Net.m_loadcount )
	{
	case 0:
		matWorld._41 = (matWorld._41+5)-SELECTMAP_POS;
		break;
	case 1:
		matWorld._41 += 5;
		break;
	case 2:
		matWorld._41 += (SELECTMAP_POS+5);
		break;
	}
	matWorld._42 = SELECTMAP_HEIGHT;
	matWorld._43 -= 2.0f;
	m_TempPlayer[nIndex].SetPosTM( matWorld );
	*/
}


// 선택화면 렌더링
void	CBravoPC::DisplayTempPlayer()
{
	// 캐릭터
	for( int i = 0; i < MAX_CHARACTER_COUNT; ++i )
	{
		if( m_bTempPlayer[i] == true )
		{
			m_TempPlayer[i].AniProcess();
			m_TempPlayer[i].AniRender();
			m_TempPlayer[i].Display( DISPLAY_SELECT );
		}
	}

	// NPC(타고 있는 말때문에 그린다)
	g_Map.AniProcessNpc();
	g_Map.DisplayNpc();
}


// 선택화면 캐릭터 삭제
void	CBravoPC::DeleteTempPlayer( bool bAll )
{
	// 전부삭제
	if( bAll == true )
	{
		for( int i = 0; i < MAX_CHARACTER_COUNT; ++i )
		{
			m_bTempPlayer[i] = false;
			m_TempPlayer[i].Destroy();
		}
	}
	// 한명만 삭제
	else
	{
		// 말있다면 말도 삭제
		// 삭제
	}
}


//-----------------------------------------------------------------------------
// Desc: 생성화면창에 나오는 캐릭터
//-----------------------------------------------------------------------------
void	CBravoPC::ChangePlayerStyle( UINT style, long lUnique )
{
	/*
	//--------------------//---------------------------------------------------------
	// 머리색깔, 머리모양, 얼굴모양
	//-----------------------------------------------------------------------------
	DWORD face_index = style & 31;
	style = style >> 5;	
	DWORD hair_index = style & 31;
	style = style >> 5;
	DWORD hair_color = style & 31;	

	//-----------------------------------------------------------------------------
	// 캐릭터 선택화면
	//-----------------------------------------------------------------------------
	if( g_RockClient.GetGameMode() == GMODE_SELECT )
	{
		m_TempPlayer[lUnique].SetMaterialColor( lUnique, HAIR_INDEX, hair_color );
		m_TempPlayer[lUnique].ChangeHair( lUnique, hair_index );
		m_TempPlayer[lUnique].ChangeFace( lUnique, face_index );
	}
	//-----------------------------------------------------------------------------
	// 게임중
	//-----------------------------------------------------------------------------
	else
	{
		if( lUnique == m_MainPC.lUnique )
		{
			m_Player.SetMaterialColor( MAX_USER_INTABLE, HAIR_INDEX, hair_color );
			m_Player.ChangeHair( MAX_USER_INTABLE, hair_index );
			m_Player.ChangeFace( MAX_USER_INTABLE, face_index );
		}
		else
		{
			int who = FindUser( lUnique );		
			if( who != MAX_USER_INTABLE )
			{
				m_Pc[who].SetMaterialColor( who, HAIR_INDEX, hair_color );
				m_Pc[who].ChangeHair( who, hair_index );
				m_Pc[who].ChangeFace( who, face_index );						
			}		
		}
	}
	*/
}


void	CBravoPC::ChangePlayerStyle( BYTE color, BYTE hair, BYTE face, long lUnique )
{	
/*
	//-----------------------------------------------------------------------------
	// 캐릭터 생성 창
	//-----------------------------------------------------------------------------
	if( lUnique == -1 )
	{
		m_MakePlayer.SetMaterialColor( m_MakePlayerIndex, HAIR_INDEX, color );
		m_MakePlayer.ChangeHair( m_MakePlayerIndex, hair );
		m_MakePlayer.ChangeFace( m_MakePlayerIndex, face );
	}
	//-----------------------------------------------------------------------------
	// 게임중
	//-----------------------------------------------------------------------------
	else if( lUnique == m_MainPC.lUnique )
	{
		m_Player.SetMaterialColor( MAX_USER_INTABLE, HAIR_INDEX, color );
		m_Player.ChangeHair( MAX_USER_INTABLE, hair );
		m_Player.ChangeFace( MAX_USER_INTABLE, face );
	}
	else
	{
		int who = FindUser( lUnique );		
		if( who != MAX_USER_INTABLE )
		{
			m_Pc[who].SetMaterialColor( who, HAIR_INDEX, color );
			m_Pc[who].ChangeHair( who, hair, true );
			m_Pc[who].ChangeFace( who, face );						
		}		
	}
*/
}


void	CBravoPC::CreateMakePlayer( int _nIndex )
{
/*
	if( g_RockClient.GetGameMode() == GMODE_SELECT && m_MakePlayerIndex != _nIndex )
	{
		// 인덱스저장
		m_MakePlayerIndex = _nIndex;
		if( m_MakePlayerIndex != -1 )
		{
			// 캐릭터생성
			m_MakePlayer.Init();
			m_MakePlayer.m_nTableIndex = MAX_USER_INTABLE;
			m_MakePlayer.CreatePlayer( 2 );
			
			// 머리색, 머리모양, 얼굴모양 셋팅
			m_MakePlayer.SetMaterialColor( _nIndex, HAIR_INDEX, 0 );
			m_MakePlayer.ChangeHair( _nIndex, 0 );
			m_MakePlayer.ChangeFace( _nIndex, 0 );
				
			// 캐릭터위치
			D3DXMATRIX	matWorld;
			D3DXMatrixIdentity( &matWorld );		
			matWorld._41 = 4.7f;
			matWorld._42 = 27.0f;
			matWorld._43 = -35.0f;
			m_MakePlayer.SetPosTM( matWorld );
		}
	}
*/
}


void	CBravoPC::DisplayMakePlayer()
{
	if( g_RockClient.GetGameMode() == GMODE_SELECT && m_MakePlayerIndex != -1 )
	{
		m_MakePlayer.AniProcess();
		m_MakePlayer.AniRender();
		m_MakePlayer.Display( DISPLAY_MAKE );
	}
}

//-----------------------------------------------------------------------------
// Desc: 데미지 이펙트 로딩
//-----------------------------------------------------------------------------
void CBravoPC::CreateDamageEffect(void)
{
	m_DamageEffect.Load( g_RockClient.GetRootDir() );
}


//-----------------------------------------------------------------------------
// Desc: 이펙트 텍스쳐 로딩
//-----------------------------------------------------------------------------
void	CBravoPC::CreateEffectData()
{

}


//-----------------------------------------------------------------------------
// Desc: 인벤토리에 있는 아이템 검사
//-----------------------------------------------------------------------------
bool	CBravoPC::FindChracterItem( int Index, DWORD ItemIdx )
{	
	for( int i = 0; i < MAX_INVENTORY1; ++i )
	{
//		if( g_Net.m_SaveInfo[Index].m_Inventory[i].m_uItemType.m_dwItemType == ItemIdx )
//			return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Desc: 자기 캐릭터 생성(아이템구성)
//-----------------------------------------------------------------------------
void	CBravoPC::CreateChracterItem( Item_Info_Save* pInventory1, Item_Info_Save* pEquip )
{
/*	sooree network
	//-----------------------------------------------------------------------------
	// 현재 장착갑옷,무기등
	//-----------------------------------------------------------------------------
	for( int i = 0; i < MAX_EQUIPED; ++i )
	{
		m_Equip[i].index		= pEquip[i].m_uItemType.m_dwItemType;
		m_Equip[i].unique		= pEquip[i].m_dwItemUnique;
		m_Equip[i].life			= pEquip[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_LIFE];
		m_Equip[i].upgrade		= pEquip[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_UPGRADE];
		m_Equip[i].color		= pEquip[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_COLOR];
		m_Equip[i].loc			= pEquip[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_LOC];
	}	
	//-----------------------------------------------------------------------------
	// 장비들
	//-----------------------------------------------------------------------------
	for( i = 0; i < MAX_INVENTORY1; ++i )
	{
		// 인벤토리에서 들고나간 아이템
		if( pInventory1[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_LOC] == 255 )
		{
			m_Tool_Inventory1[i].index			= 0;
			m_Tool_Inventory1[i].unique			= 0;
			m_Tool_Inventory1[i].life			= 0;
			m_Tool_Inventory1[i].upgrade		= 0;
			m_Tool_Inventory1[i].color			= 0;
			m_Tool_Inventory1[i].loc			= 0;
		}
		else
		{
			m_Tool_Inventory1[i].index		= pInventory1[i].m_uItemType.m_dwItemType;
			m_Tool_Inventory1[i].unique		= pInventory1[i].m_dwItemUnique;
			m_Tool_Inventory1[i].life		= pInventory1[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_LIFE];
			m_Tool_Inventory1[i].upgrade	= pInventory1[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_UPGRADE];
			m_Tool_Inventory1[i].color		= pInventory1[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_COLOR];
			m_Tool_Inventory1[i].loc		= pInventory1[i].m_uItemInfo.m_byItemInfo[ITEM_KEY_LOC];
		}
	}
*/
}


//-----------------------------------------------------------------------------
// Desc: 직업에 따른 속성저장
//-----------------------------------------------------------------------------
int		CBravoPC::SetCharacterAttr( PCTABLE _Table )
{
	return SetCharacterAttr( _Table.char_info.job, _Table.char_info.race, _Table.char_info.sex );
}

int		CBravoPC::SetCharacterAttr( int _nJob, int _nRace, int _nSex )
{
	int nAttr = 0;
/*	switch( _nJob )
	{
	case FAIRY:		// 페어리
		nAttr = UNION_FAIRY;
		break;
	case ELIADEN:	// 엘리아덴
		nAttr = UNION_FAIRY;
		break;
	case ROIEL:		// 로이엘
		nAttr = UNION_FAIRY;
		break;
	default:
		{
			switch( _nRace )
			{
			case UNION:	// 인간
				{
					// 남자
					if( _nSex == 0 )
						nAttr = UNION_MAN;
					// 여자
					else
						nAttr = UNION_WOMAN;
				}
				break;
			default:
				{
				}
				break;
			}
		}
		break;
	} */
	return nAttr;
}


//-----------------------------------------------------------------------------
// Desc: 자기 캐릭터 로그인
//-----------------------------------------------------------------------------
void		CBravoPC::SetupMainPc( PCTABLE &pc )
{
	m_MainPC = pc;
	lReceiveBlock = m_MainPC.lDestBlock;

	//-----------------------------------------------------------------------------
	// 캐릭터 속성저장
	//-----------------------------------------------------------------------------
//	m_MainPC.nIndex = SetCharacterAttr( m_MainPC );
	m_MainPC.nIndex = pc.nIndex;	

	//-----------------------------------------------------------------------------
	// 캐릭터생성
	//-----------------------------------------------------------------------------
	m_Player.m_nTableIndex = MAX_USER_INTABLE;
	m_Player.CreatePlayer();

	//-----------------------------------------------------------------------------
	// 현재 위치 지정하기
	//-----------------------------------------------------------------------------
	int		nPosX  = m_MainPC.lDestBlock%m_cSize;
	int		nPosY  = m_MainPC.lDestBlock/m_cSize;
	m_Player.SetTilePos( nPosX, nPosY );	

	//-----------------------------------------------------------------------------
	// 머리색, 머리모양, 얼굴모양 셋팅
	//-----------------------------------------------------------------------------
	UINT style = 0;
//	ChangePlayerStyle( style, m_MainPC.lUnique );

	//-----------------------------------------------------------------------------
	// 퀘스트저장
	//-----------------------------------------------------------------------------
/*	Sinario_Quest	Quest = g_Net.m_SaveInfo[m_nPickedUser].s_Quest;

	// 시나리오인덱스의 퀘스트 생성하기
	if( Quest.m_SQIndex.m_byValue[0] != BYTE_FAIL )
	{
		SM__GET__QUEST( 0, QUEST_SCENARIO, Quest.m_SQIndex.m_byValue[0] );

		// 남은시간, 현재성공갯수 저장하기
		m_QuestInfo[QUEST_SCENARIO].table.req.Time				= Quest.m_QuestTime;
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[0]	= Quest.m_SQIndex.m_byValue[1];	
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[1]	= Quest.m_SQStatus.m_byValue[0];
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[2]	= Quest.m_SQStatus.m_byValue[1];
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[3]	= Quest.m_SQStatus.m_byValue[2];
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[4]	= Quest.m_SQStatus.m_byValue[3];
	}
*/
	//-----------------------------------------------------------------------------
	// 엑스터시
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// 퍼즐
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// 인벤토리
	//-----------------------------------------------------------------------------
	m_nPickedUser = -1;
}

//-----------------------------------------------------------------------------
// Desc: 말타고 있는지 리턴
//-----------------------------------------------------------------------------
bool		CBravoPC::GetHorseState( int index )
{
	return false;
}

void		CBravoPC::SetupDestBlock(long unique, int x, int y)
{
	int i = 0;
	for (  i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if ( unique == m_PcTable[i].lUnique )
			break;
	}

	m_PcTable[i].lDestBlock	= (y*m_cSize)+x;
}

//-----------------------------------------------------------------------------
// Desc: 다른 캐릭터와 충돌시.. 이동할 위치 보내기
//-----------------------------------------------------------------------------
void		CBravoPC::SetPlayerMove( DWORD pos )
{
	// 이동위치 큐에넣기
	EVENT_DATA_INFO	data;
	data.type = SM_MOVE_BROADCAST;
	data.x = pos%m_cSize;
	data.y = pos/m_cSize;
	m_Player.PushQueue( data );
}


//-----------------------------------------------------------------------------
// Desc: 워프한 맵의 자기위치 보내기
//-----------------------------------------------------------------------------
void		CBravoPC::SetWarpMove( PCTABLE& pc )
{
	m_MainPC = pc;

	// 내위치갱신
	lReceiveBlock = m_MainPC.lDestBlock;

	// 말위치도 갱신
/*	if( m_Player.m_State.bRide == true )
	{
		g_Map.m_NpcTable[m_Player.m_State.horse_index].lBlock     = m_MainPC.lDestBlock;
		g_Map.m_NpcTable[m_Player.m_State.horse_index].lDestBlock = m_MainPC.lDestBlock;
	}
	// 합체한 상태다
	if( m_Player.m_State.Unite_unique != -1 )
	{
	}
	// 내가 요정일 경우
	if( m_MainPC.nIndex == UNION_FAIRY )
	{
		// 합체 상태가 아닌데.. 스케일이 작다면.. 합체상태에서 죽어 워프한경우다..
		// 그럴경우 원래 크기값으로 돌려줘야 한다
		if( m_Player.m_State.Unite_unique == -1 && m_Player.m_vScale.x != 0.9f )
		{
			m_Player.m_vScale.x = 0.9f;
			m_Player.m_vScale.y = 0.9f;
			m_Player.m_vScale.z = 0.9f;
		}
	} */
	
	// 현재 위치 지정하기
	int		nPosX  = m_MainPC.lDestBlock%m_cSize;
	int		nPosY  = m_MainPC.lDestBlock/m_cSize;
	m_Player.SetTilePos( nPosX, nPosY );

	// 워프했기때문에 저장된 모든 행동 지우기
	m_Player.ClearEvent();
	m_Player.Clear();

	// 페어리일 경우, 페어리 이동 처리 생성
/*	if( m_MainPC.nIndex == UNION_FAIRY )
	{
		g_Particle.DisplayUpdate( 7, m_MainPC.lUnique, -1, -1 );	//
		g_Billboard.SetManager( 0.0f, 29, m_MainPC.lUnique, -1 );
		g_Billboard.SetManager( 0.0f, 30, m_MainPC.lUnique, -1 );

		// 합체가 아니고
		if( !GetPlayer()->m_State.bUnite )
		{
			// 차지 상태 라면
//			if( GetPlayer()->m_pUserInfo->bCharge ) 
//				g_Billboard.SetManager( 0.0f, 8, m_MainPC.lUnique, -1 );
		}
	}
	else if( m_MainPC.char_info.job == NOVICE || m_MainPC.char_info.job == KNIGHT )
	{
		// 합체냐
		if( GetPlayer()->m_State.bUnite )
		{
		}
	} */

	// 차지 상태 라면
//	if( GetPlayer()->m_pUserInfo->bCharge )
//		g_Billboard.SetManager( 0.0f, 8, m_MainPC.lUnique, -1 );

	///...sh
//	g_Billboard.SetManager( 1.0f, 16, m_MainPC.lUnique, -1 );
//	g_Billboard.SetManager( 1.0f, 17, m_MainPC.lUnique, -1 );
//	g_Billboard.SetManager( 1.0f, 18, m_MainPC.lUnique, -1 );
//	g_Billboard.SetManager( 1.0f, 19, m_MainPC.lUnique, -1 );
	
//	g_Particle.m_Cloak.ResetSimulation();

	for( int i = 0; i < LIGHTTALBE_COUNT; ++i )
	{
		if( g_Map.m_LightTable[i].Display == ROIEL_SKILL_150 )
		{
			g_Particle.SetEffSetCon( 0.25f,45,17,101.0f,m_MainPC.lUnique,-1,D3DXVECTOR3(0.0f,0.0f,0.0f),0.0f);
			g_Particle.SetEffSetCon( 0.25f,46,0,101.0f,m_MainPC.lUnique,-1,D3DXVECTOR3(0.0f,0.0f,0.0f),0.0f);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Desc: PC, NPC 데이타 처리 & 렌더링
//-----------------------------------------------------------------------------
void CBravoPC::AniProcess()
{
	//-----------------------------------------------------------------------------
	// 자기 캐릭터
	//-----------------------------------------------------------------------------
	m_Player.UpdateHPData();
	m_Player.QueueProcess();	
	m_Player.AniProcess();
	
	//-----------------------------------------------------------------------------
	// 다른 캐릭터
	//-----------------------------------------------------------------------------
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_Pc[i].m_bIsLoaded == true )
		{
			m_Pc[i].UpdateHPData();
			// 큐에 들어있는 메세지저장
			m_Pc[i].QueueProcess();

			int nRes = m_Pc[i].AniProcess();
		}
	}
	
	list< SDelayAppearPC * >::iterator ir = g_DelayActList.m_DelayAppearPCList.begin();
	
	for( ; ir != g_DelayActList.m_DelayAppearPCList.end(); ++ir )		
	{			
		if( ( *ir ) && ( SAFE_TIME_COMPARE( g_nowTime , > , ( *ir )->DelayTime ) ) )
		{
			g_RockClient.DelayProc_RpScAppearPC( &( *ir )->AppearPC );						
			
			SDelayAppearPC * pData = (*ir);
			SAFE_DELETE( pData );
			(*ir) = NULL;
			
			g_DelayActList.m_DelayAppearPCList.erase( ir++ );
		}			
	}		

	{
		list< SDelayAppearNPC * >::iterator i = g_DelayActList.m_DelayAppearNPCList.begin();
		
		for( ; i != g_DelayActList.m_DelayAppearNPCList.end() ; ++i )
		{			
			if( ( *i ) && ( SAFE_TIME_COMPARE( g_nowTime , > , ( *i )->DelayTime ) )  )
			{
				g_RockClient.Proc_RpScAppearHoly( &( (*i)->AppearNPC ) );
				
				SDelayAppearNPC * pData = (*i);
				SAFE_DELETE( pData );
				(*i) = NULL;

				g_DelayActList.m_DelayAppearNPCList.erase( i++ );
			}			
		}
	}

	{
		list< SDelayAppearPet * >::iterator i = g_DelayActList.m_DelayAppearPetList.begin();
		
		for( ; i != g_DelayActList.m_DelayAppearPetList.end() ; ++i )
		{			
			if( ( *i ) && ( SAFE_TIME_COMPARE( g_nowTime , > , ( *i )->DelayTime) ) )
			{
				g_RockClient.DelayProc_RpScAppearPet( &( (*i)->AppearPet ) );
				
				SDelayAppearPet * pData = (*i);
				SAFE_DELETE( pData );
				(*i) = NULL;

				g_DelayActList.m_DelayAppearPetList.erase( i++ );
			}			
		}
	}
	
	{
		list< SDelayDisappearPet * >::iterator i = g_DelayActList.m_DelayDisappearPetList.begin();
		
		for( ; i != g_DelayActList.m_DelayDisappearPetList.end() ; ++i )
		{			
			if( ( *i ) && ( SAFE_TIME_COMPARE( g_nowTime , > , ( *i )->DelayTime ) ) )
			{
				g_RockClient.DelayProc_RpScDisAppearPet( &( (*i)->DisappearPet ) );
				
				SDelayDisappearPet * pData = (*i);
				SAFE_DELETE( pData );
				(*i) = NULL;

				g_DelayActList.m_DelayDisappearPetList.erase( i++ );
			}			
		}
	}

	return;
}


// 렌더링
void CBravoPC::Display() 
{	
}


//-----------------------------------------------------------------------------
// Desc: 이모티콘 그리기
//-----------------------------------------------------------------------------
void CBravoPC::DisplayImoticon()
{	
	if( g_RockClient.GetGameMode() != GMODE_NORMAL )
		return;

//	g_RockClient.m_pSprCursor->Begin();

	RECT	rect;
	D3DXVECTOR2	vPos;
	vPos.x = 0;
	vPos.y = 0;

	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_ImoticonTable[i].type != -1 )
		{
			//-----------------------------------------------------------------------------
			// 저장된 시간지났다면 삭제하기
			//-----------------------------------------------------------------------------
			if( SAFE_TIME_COMPARE( m_ImoticonTable[i].time , <= , g_nowTime ) )			
				m_ImoticonTable[i].Clear();
			//-----------------------------------------------------------------------------
			// 렌더링
			//-----------------------------------------------------------------------------
			else
			{
				//-----------------------------------------------------------------------------
				// 유저위치 
				//-----------------------------------------------------------------------------
				if( m_ImoticonTable[i].index == -1 )
				{
					if( m_MainPC.lUnique == m_ImoticonTable[i].unique )
					{
//						if( g_ChatList.From3DTo2DUser( MAX_USER_INTABLE, vPos ) == false )
//							continue;
					}
					else
					{
						int nUser = FindUser( m_ImoticonTable[i].unique );
						if( nUser == MAX_USER_INTABLE )
							continue;
//						if( g_ChatList.From3DTo2DUser( nUser, vPos ) == false )
//							continue;
					}
					// 말탔다면 Y값 올리기
					if( GetHorseState( FindUser(m_ImoticonTable[i].unique) ) == true )
						vPos.y	-= 40;
				}
				//-----------------------------------------------------------------------------
				// NPC위치
				//-----------------------------------------------------------------------------
//				else				
//					vPos = g_ChatList.From3DTo2D2(g_Map.FindNpc(m_ImoticonTable[i].index, m_ImoticonTable[i].unique));
				vPos.x	-= (IMOTICON_SIZE/2);
				vPos.y	-= (IMOTICON_SIZE+5);	
				
				//-----------------------------------------------------------------------------
				// 현재 애니값 계산
				//-----------------------------------------------------------------------------
				int	time	= SAFE_TIME_SUB( IMOTICON_PLAYTIME , ( SAFE_TIME_SUB( m_ImoticonTable[i].time , g_nowTime ) ) );
				int ani		= (time/200)%2;

				// 이모티콘 타입에 따른 텍스쳐에서의 위치값
				// 첫번째줄
				if( m_ImoticonTable[i].type < 8 )
					SetRect( &rect, IMOTICON_SIZE*m_ImoticonTable[i].type, ani*IMOTICON_SIZE, IMOTICON_SIZE*(m_ImoticonTable[i].type+1), (ani+1)*IMOTICON_SIZE );
				// 두번째줄
				//else if( m_ImoticonTalbe[i].type < 16 )
				//	SetRect( &rect, IMOTICON_SIZE*m_ImoticonTable[i].type, IMOTICON_SIZE, IMOTICON_SIZE*(m_ImoticonTable[i].type+1), IMOTICON_SIZE );

				// 그리기
//				g_RockClient.m_pSprCursor->Draw( m_TexTable.pImoticon, &rect, NULL, NULL, 0.0f, &vPos, D3DCOLOR_ARGB(255,255,255,255) );
			}
		}
	}
//	g_RockClient.m_pSprCursor->End();
}


// 이모티콘 저장
void CBravoPC::SetImoticon( int index, DWORD unique, int type )
{
	// 현재 대상의 이모티콘이 있다면.. 삭제한다
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_ImoticonTable[i].index == index && m_ImoticonTable[i].unique == unique )
			m_ImoticonTable[i].Clear();		
	}

	// 이모티콘 생성한다
	for(int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_ImoticonTable[i].type == -1 )
		{
			m_ImoticonTable[i].index	= index;
			m_ImoticonTable[i].unique	= unique;
			m_ImoticonTable[i].type		= type;
			m_ImoticonTable[i].time		= SAFE_TIME_ADD( g_nowTime , IMOTICON_PLAYTIME );
			break;
		}
	}
}

void CBravoPC::DelImoticon( int index, DWORD unique )
{
	// 삭제한다
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_ImoticonTable[i].index == index && m_ImoticonTable[i].unique == unique )
			m_ImoticonTable[i].Clear();		
	}
}

//-----------------------------------------------------------------------------
// Desc: 퀘스트처리
//-----------------------------------------------------------------------------
void CBravoPC::ViewQuest()
{
	m_DisplayQuest	= !m_DisplayQuest;
	m_CurrentQuest	= 0;
}

void CBravoPC::SetQuestData( int Class, int index )
{
//	m_QuestInfo[Class].SetQuestData(index);
}

void CBravoPC::DelQuestData( int Class )
{	
	// 초기화
	m_QuestInfo[Class].bInit = false;
	m_QuestInfo[Class].table.req.Init();
	m_QuestInfo[Class].table.target.Init();

	// 문자배열 메모리해제 
	int i = 0;
	for(  i = 0; i < m_QuestInfo[Class].text.SubjectMaxLine; ++i )
		m_QuestInfo[Class].text.Subject[i].Destroy();
	for( i = 0; i < m_QuestInfo[Class].text.ObjectMaxLine; ++i )
		m_QuestInfo[Class].text.Object[i].Destroy();
	for( i = 0; i < m_QuestInfo[Class].text.GiftMaxLine; ++i )
		m_QuestInfo[Class].text.Gift[i].Destroy();
}

// 퀘스트 상태업데이트
void CBravoPC::UpdateQuest( int index, BYTE type )
{	
}

// 퀘스트창에서 그리기
void CBravoPC::DisplayQuest()
{	
}
//-----------------------------------------------------------------------------
// Desc: 맵정보(이름) 
//-----------------------------------------------------------------------------
void CBravoPC::InitMapInfo()
{
}

void CBravoPC::DisplayMapInfo( int map_id, BYTE type )
{
}

char* CBravoPC::GetMapName( int map_id )
{
	return m_cMapName;
}

//-----------------------------------------------------------------------------
// Desc: 무기에 붙은 이펙트렌더링
//-----------------------------------------------------------------------------
void CBravoPC::EffectDisplay()
{	
	// 이펙트
	m_Player.DisplayEffect();			
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{			
		if( m_Pc[i].m_bIsVisible == true && m_Pc[i].m_bIsLoaded == true  )
			m_Pc[i].DisplayEffect();		
	}	
}

 
//-----------------------------------------------------------------------------
// Desc: 그림자
//-----------------------------------------------------------------------------
void CBravoPC::ShadowMake()
{		
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	// 다른 캐릭터		
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{			
		if( m_Pc[i].m_bIsVisible == true && m_Pc[i].m_bIsLoaded == true && m_PcTable[i].bRender == true && 
			m_Pc[i].m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
			m_Pc[i].ShadowMake();
	}

	// 자기캐릭터
	if( m_MainPC.bRender == true && m_Player.m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
		m_Player.ShadowMake();	
}

void CBravoPC::ShadowDisplay()
{	
	if( g_RockClient.m_SceneManager.GetOtherPlayerDisplay() )
	{
		// 다른 캐릭터
		for( int i = 0; i < MAX_USER_INTABLE; ++i )
		{
			if( m_Pc[i].m_bIsVisible == true && m_Pc[i].m_bIsLoaded == true && m_PcTable[i].bRender == true &&
				m_Pc[i].m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
				m_Pc[i].ShadowDisplay();
		}
	}

	if( g_RockClient.m_SceneManager.GetPlayerDisplay() &&
		g_Pc.GetPlayer()->IsMyPlayerRender )
	{
		// 자기 캐릭터
		if( m_MainPC.bRender == true && m_Player.m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
			m_Player.ShadowDisplay();
	}
}


//-----------------------------------------------------------------------------
// Desc: 마우스로 픽킹한 타일값 보내기
//-----------------------------------------------------------------------------
bool CBravoPC::SetTile()
{
	EVENT_DATA_INFO	data;
	data.x = g_Map.GetPickedX();
	data.y = g_Map.GetPickedY();
	data.type	= SM_MOVE_BROADCAST;
	m_Player.PushQueue( data );	

	// 임시 이동 테스트 ( 다른 유저와 NPC )
//	if( m_Pc[0].m_bIsVisible == true && m_Pc[0].m_bIsLoaded == true )
//		m_Pc[0].SetQueue( data );
//	if( g_Map.m_Npc[0].m_bIsVisible == true && g_Map.m_Npc[0].m_bIsLoaded == true )
//		g_Map.m_Npc[0].SetQueue( data );

	return	true;
}

//-----------------------------------------------------------------------------
// 유저클릭 
//-----------------------------------------------------------------------------
bool CBravoPC::SetPC()
{
	return true;
}

//-----------------------------------------------------------------------------
// Desc: NPC클릭 
//-----------------------------------------------------------------------------
bool CBravoPC::SetNPC()
{
	//-----------------------------------------------------------------------------
	// 메뉴창 아이콘을 클릭했다면 리턴
	//-----------------------------------------------------------------------------

	return true;
}


bool	CBravoPC::MerchantProcess( int merchant, int select )
{
	return false;
}


bool	CBravoPC::SetMouse( bool bFindNpc )
{

	return SetTile();
}

#ifdef HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)
//-----------------------------------------------------------------------------
// Desc: 캐릭터선택화면에서 픽킹
//-----------------------------------------------------------------------------
short	CBravoPC::PickCharSelectUser()
{
	short sSortIndex = PickUser();
	DebugPrint("Index = %d\n", sSortIndex);
	return m_PcTable[sSortIndex].lUnique;

/*
	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);
	ptCursor.x	+= 2;
	ptCursor.y	+= 10;

	D3DXVECTOR3	vPickRayDir;
	D3DXVECTOR3	vPickRayOrig;

	D3DXMATRIX		matProj;
	matProj	= g_Camera.m_matProj;
	
	GetCursorPos(&ptCursor);
	ptCursor.x	+= 2;
	ptCursor.y	+= 10;
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	D3DXVECTOR3	v;
	v.x	= (((2.0f*ptCursor.x)/g_RockClient.GetDisplayWidth())-1) / matProj._11;
	v.y	= -(((2.0f*ptCursor.y)/g_RockClient.GetDisplayHeight())-1) / matProj._22;
	v.z	= 1.0f;
	
	D3DXMATRIX		matView, m;
	matView	= g_Camera.GetView();
	D3DXMatrixInverse(&m, NULL, &matView);

	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

	FLOAT	fBary1, fBary2;
	FLOAT	fDist;

	D3DXVECTOR3		v1[3];	// 1st face
	D3DXVECTOR3		v2[3];	// 2nd face
	D3DXMATRIX		matpos;

	for ( short i = 0; i < MAX_CHARACTER_COUNT; i ++ )
	{
		if( m_bTempPlayer == false )
			continue;

		D3DXMatrixIdentity(&matpos);
		matpos._41	= m_TempPlayer[i].GetPosTM()._41;
		matpos._42	= m_TempPlayer[i].GetPosTM()._42;
		matpos._43	= m_TempPlayer[i].GetPosTM()._43;

		//D3DXVECTOR3	min	= D3DXVECTOR3(m_TempPlayer[i].m_Bound.m_min_vertex.x, m_TempPlayer[i].m_Bound.m_min_vertex.y, m_TempPlayer[i].m_Bound.m_min_vertex.z);
		//D3DXVECTOR3	max	= D3DXVECTOR3(m_TempPlayer[i].m_Bound.m_max_vertex.x ,m_TempPlayer[i].m_Bound.m_max_vertex.y, m_TempPlayer[i].m_Bound.m_max_vertex.z);
		D3DXVECTOR3	min	= g_Pc_Manager.GetBoundMinPos( &m_TempPlayer[i] );
		D3DXVECTOR3	max	= g_Pc_Manager.GetBoundMaxPos( &m_TempPlayer[i] );
		D3DXVec3TransformCoord(&min, &min, &matpos);
		D3DXVec3TransformCoord(&max, &max, &matpos);		

		for ( short j = 0; j < 4; j ++ )
		{
			switch ( j )
			{
			case	0:		// frond face of baounding box
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= min.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
			case	1:		// rear face 
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= max.y;
				v2[1].z	= min.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= max.z;
				break;
			case	2:	// left
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= min.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= min.x;
				v2[1].y	= min.y;
				v2[1].z	= min.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= max.z;
				break;
			case	3:	// right
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= max.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= min.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= max.z;
				break;
			}
			if (g_Map.IntersectTriangle(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2) 
			||	g_Map.IntersectTriangle(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2) )
			{
				// 메뉴에 저장

				return i;
			}
		}
	}

	return	m_nPickedUser = -1;
*/
}

#endif// #define HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)

//-----------------------------------------------------------------------------
// Desc: 캐릭터 픽킹
//-----------------------------------------------------------------------------
short	CBravoPC::PickUser()
{
	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);
//	ptCursor.x	+= 2;
//	ptCursor.y	+= 10;


	D3DXVECTOR3	vPickRayDir;
	D3DXVECTOR3	vPickRayOrig;

	D3DXMATRIX		matProj;

	matProj	= g_Camera.m_matProj;

	GetCursorPos(&ptCursor);
//	ptCursor.x	+= 2;
//	ptCursor.y	+= 10;
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	D3DXVECTOR3	v;
	v.x	= (((2.0f*ptCursor.x)/g_RockClient.GetDisplayWidth())-1) / matProj._11;
	v.y	= -(((2.0f*ptCursor.y)/g_RockClient.GetDisplayHeight())-1) / matProj._22;
	v.z	= 1.0f;

	D3DXMATRIX		matView, m;
	matView	= g_Camera.m_matView;
	D3DXMatrixInverse(&m, NULL, &matView);

	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;
	
	if( !g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke) )
	{
		m_nPickedUser = -1;
	}
	
	FLOAT	fBary1, fBary2;
	FLOAT	fDist;
	
	D3DXVECTOR3		v1[3];	// 1st face
	D3DXVECTOR3		v2[3];	// 2nd face
	D3DXMATRIX		matpos;
	D3DXMATRIX		matrot;
	D3DXVECTOR3		min, max;

	int nIndex		= -1;

	g_Map.m_SortTable.StartPCPointer();

	while( ( nIndex = g_Map.m_SortTable.GetPC() ) != -1 )
	{
/*		
		if( nIndex == -1 )
		{
			nIndex = MAX_USER_INTABLE;
			continue;						// 자기자신은 픽킹에서 제외시킨다.
		}
		else
		{
			nIndex = g_Pc_Manager.m_pSortTable[i].Index;
			if( g_Pc_Manager.m_pSortTable[i].byType != SORT_PC )
				continue;
		}
*/		
		D3DXMatrixIdentity( &matpos );		
		D3DXMatrixIdentity( &matrot );		

		Character* pChr = NULL;
/*
		if( nIndex == MAX_USER_INTABLE )
		{
			matpos._41	= m_Player.GetPosTM()._41;
			matpos._42	= m_Player.GetPosTM()._42;
			matpos._43	= m_Player.GetPosTM()._43;
//			min = g_Pc_Manager.GetBoundMinPos( &m_Player );
//			max = g_Pc_Manager.GetBoundMaxPos( &m_Player );
			min = g_Pc_Manager.GetMinBoundVec( &m_Player );
			max = g_Pc_Manager.GetMaxBoundVec( &m_Player ); 
			pChr = (Character*) g_Pc.GetPlayer();
		}
		else*/
		{
			// 로딩안되어있거나, 보이지 않다면
			if( m_Pc[nIndex].m_bIsLoaded == false || m_Pc[nIndex].m_bIsVisible == false )
				continue;			
			if( g_Pc.m_Pc[nIndex].m_CharAlpha.State == CHAR_NONE_DISPLAY )
				continue;
			
			/*if( g_Pc.m_Pc[nIndex].m_curt_event.type == SM_STATE_DIE )
				continue;*/

			if(!m_Pc[nIndex].m_State.bRide)
			{
				matpos._41	= m_Pc[nIndex].GetPosTM()._41;
				matpos._42	= m_Pc[nIndex].GetPosTM()._42;
				matpos._43	= m_Pc[nIndex].GetPosTM()._43;
			}
			else
			{
				matpos._41	= m_Pc[nIndex].m_matRideChr._41;
				matpos._42	= m_Pc[nIndex].m_matRideChr._42;
				matpos._43	= m_Pc[nIndex].m_matRideChr._43;
			}

//			min = g_Pc_Manager.GetBoundMinPos( &m_Pc[nIndex] );
//			max = g_Pc_Manager.GetBoundMaxPos( &m_Pc[nIndex] );			
			
			if(m_Pc[nIndex].m_IsAllLTexture)
			{
				min = g_Pc_Manager.GetMinBoundVec( &m_Pc[nIndex] );
				max = g_Pc_Manager.GetMaxBoundVec( &m_Pc[nIndex] );
			}
			else
			{
				min = g_Pc_Manager.GetBoundMinPos( &m_Pc[nIndex] );
				max = g_Pc_Manager.GetBoundMaxPos( &m_Pc[nIndex] );	
			}
			
			pChr = (Character*) &m_Pc[nIndex];
		}
		
		for( int j = 0; j < 6; j ++ )
		{
			switch( j )
			{
				// frond side
			case 0:				
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;
				
				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= min.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break; 
				// right side
			case 1:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= max.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;
				
				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
				// back side
			case 2:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;
				
				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= max.z;
				break;
				// left side
			case 3:		
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= min.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;
				
				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= min.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
				// top side
			case 4:					
				v1[0].x	= min.x;
				v1[0].y	= max.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;
				
				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= max.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= max.y;
				v2[2].z	= min.z;
				break;
				// bottom side
			case 5:						
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= min.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= min.y;
				v1[2].z	= min.z;
				
				v2[0].x	= min.x;
				v2[0].y	= min.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
			}

			D3DXMatrixRotationYawPitchRoll( &matrot, pChr->m_Mov.now_Yrad, pChr->m_Mov.now_Xrad, 0.0f );
			D3DXVec3TransformCoord( &v1[0], &v1[0], &matrot );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matrot );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matrot );
			D3DXVec3TransformCoord( &v2[0], &v2[0], &matrot );
			D3DXVec3TransformCoord( &v2[1], &v2[1], &matrot );
			D3DXVec3TransformCoord( &v2[2], &v2[2], &matrot );

			D3DXVec3TransformCoord( &v1[0], &v1[0], &matpos );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matpos );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matpos );
			D3DXVec3TransformCoord( &v2[0], &v2[0], &matpos );
			D3DXVec3TransformCoord( &v2[1], &v2[1], &matpos );
			D3DXVec3TransformCoord( &v2[2], &v2[2], &matpos );
			
			if (g_Map.IntersectTriangle(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2) 
			||	g_Map.IntersectTriangle(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2) )
			{
//				if( nIndex == MAX_USER_INTABLE )
//					g_RockClient.m_byCurCursor = BC_PERSON;
//				else
//				{
//---------------------------------------------------------------------------------------------------------------
					// 죽어있거나, 현재 클릭한 대상이라면.. 무시
//					if( m_Pc[nIndex].m_curt_event.type == SM_STATE_DIE )
//						return -1;
//---------------------------------------------------------------------------------------------------------------


					//if( g_Pc.m_Select.bPC == true && g_Pc.m_Select.index == i )
					//	return -1;

					// 마우스타입저장
//					if( m_Player.m_pUserInfo->bPkMode == true || m_Pc[nIndex].m_pUserInfo->bPkMode == true )
//						g_RockClient.m_byCurCursor = BC_ATTACKENABLE;
//					else
//						g_RockClient.m_byCurCursor = BC_PERSON;
//				}
				
				// 부가효과 (강제타겟) 처리 
				if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke) )
				{
					if( m_nPickedUser == NonPlayerInfoWnd.thePickedUserID )
					{
						return m_nPickedUser;
					}
					else
					{
						return  -1;
					}
				}					
				
				// 클릭한 유저 저장
				m_nPickedUser = nIndex;
				
				return nIndex;
			}
		}
	}

	return	-1;
}

//-----------------------------------------------------------------------------
// Desc: 유저 테이블 갱신(보이니?)
//-----------------------------------------------------------------------------
bool	CBravoPC::UpdateUserTable()
{
	m_nUserCount = 0;
	D3DXMATRIX	matpos;
	D3DXMATRIX	matChar	= GetPosTM(-1);

	for ( short i = 0; i < MAX_USER_INTABLE; i ++ )
	{
		if( m_Pc[i].m_bIsLoaded == true )
		{
			D3DXMatrixIdentity(&matpos);
			matpos._41	= m_Pc[i].GetPosTM()._41;
			matpos._42	= m_Pc[i].GetPosTM()._42;
			matpos._43	= m_Pc[i].GetPosTM()._43;
			
			D3DXVECTOR3		min = g_Pc_Manager.GetBoundMinPos( &m_Pc[i] );
			D3DXVECTOR3		max = g_Pc_Manager.GetBoundMaxPos( &m_Pc[i] );

			D3DXVec3TransformCoord(&min, &min, &matpos);
			D3DXVec3TransformCoord(&max, &max, &matpos);
			SetupMaxMin(&max, &min);

			short	sizex	= max.x - min.x;
			short	sizey	= max.y - min.y;
			short	sizez	= max.z	- min.z;

			if( true == (m_Pc[i].m_bIsVisible = g_Camera.CubeInFrustum(min.x, min.y, min.z, sizex, sizey, sizez) ))
			{
				m_nUserCount++;	

				// 게임 선택화면에서는 주 플레이어가 없기 때문에 거리는 모두 0 으로 초기화
				if( g_RockClient.GetGameMode() == GMODE_SELECT )
				{
					m_Pc[i].m_fDistance = 0.0f;
				}
				else
				{
					const float	_x  = (matChar._41-matpos._41)*(matChar._41-matpos._41);
					const float	_y 	= (matChar._43-matpos._43)*(matChar._43-matpos._43);
					m_Pc[i].m_fDistance = sqrt(_x+_y);
				}
			}
		}
		else if( m_Pc[i].m_bIsLoaded == false && m_Pc[i].m_nTableIndex != -1 )
		{

		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// Desc: 유저 테이블 갱신(보이니?)
//-----------------------------------------------------------------------------
bool	CBravoPC::UpdateUser( int who )
{
	D3DXMATRIX	matpos;
	D3DXMatrixIdentity(&matpos);
	matpos._41	= m_Pc[who].GetPosTM()._41;
	matpos._42	= m_Pc[who].GetPosTM()._42;
	matpos._43	= m_Pc[who].GetPosTM()._43;	
	D3DXVECTOR3	min = g_Pc_Manager.GetBoundMinPos( &m_Pc[who] );
	D3DXVECTOR3	max = g_Pc_Manager.GetBoundMaxPos( &m_Pc[who] );
	
	D3DXVec3TransformCoord(&min, &min, &matpos);
	D3DXVec3TransformCoord(&max, &max, &matpos);
	SetupMaxMin(&max, &min);	

	short	sizex	= max.x - min.x;
	short	sizey	= max.y - min.y;
	short	sizez	= max.z	- min.z;

	if ( true == (m_Pc[who].m_bIsVisible = g_Camera.CubeInFrustum(min.x, min.y, min.z, sizex, sizey, sizez) ))
		m_nUserCount++;
	return true;
}

//-----------------------------------------------------------------------------
// Desc: 내가 보는 방향
//-----------------------------------------------------------------------------
D3DXVECTOR3	CBravoPC::GetPlayerSightDir( long unique, float SightLength, float DeltaRad )
{
	float rad  = GetDirection(unique);
	float Vecx = GetPosTM(unique)._41;
	float Vecz = GetPosTM(unique)._43;
	float Vecy = GetPosTM(unique)._42;


	//v는 캐릭터 중간 좌표 높이는 당높이 + 바운드 박스 중간 
	D3DXVECTOR3 v = D3DXVECTOR3( Vecx, Vecy, Vecz );

	D3DXMATRIX mat;
	D3DXMATRIX mattemp;

	// 여기서 (0.0f 0.0f -SightLength) 보는 방향 으로 부터 SightLength 떨어 진거리 측정을 위해 
	D3DXMatrixTranslation( &mat, 0.0f, 0.0f, (0.0f - SightLength) );

	//각도 만큼 벡터를 회전 시킨 후에 
	D3DXMatrixRotationY( &mattemp, rad + DeltaRad );
	D3DXMatrixMultiply( &mat, &mat, &mattemp );

	//w라는 회전된 벡터를 구해 낸다 
	D3DXVECTOR3 w = D3DXVECTOR3( mat._41, mat._42, mat._43 );

	//캐릭터 좌표와 + 회전된 벡터를 구하면 캐릭터 반경내의 처다보는 값이 나온다 
	v += w;

	return v;
}

void CBravoPC::SM__CHANGE__ITEM( long unique, DWORD item_id, DWORD item_unique, DWORD item_pos, BYTE pos )
{
	// 자기 캐릭터
	if( unique == m_MainPC.lUnique )
	{
		if( m_Player.SM__CHANGE__ITEM( item_id, item_unique ) == 1 )
		{
			m_Player.m_AttackType = 0;
			m_Player.m_AttackStep = 1;			
		}
	}
	else
	{
		int table_index = g_Pc.FindUser(unique);

		g_Pc.m_Pc[table_index].SM__CHANGE__ITEM( item_id, item_unique );
	}
}


//-----------------------------------------------------------------------------
// Desc: 아이템벗기
//-----------------------------------------------------------------------------
void CBravoPC::SM__TAKEOFF__ITEM( long unique, DWORD item_id, DWORD item_unique )
{
	// 자기 캐릭터
	if( unique == m_MainPC.lUnique )
	{
		// 아이템 벗기
		if( m_Player.SM__TAKEOFF__ITEM( item_id, item_unique ) == 1 )
		{
			m_Player.m_AttackType = 0;
			m_Player.m_AttackStep = 1;
		}
	}
	else
	{
		int table_index = g_Pc.FindUser(unique);

		g_Pc.m_Pc[table_index].SM__TAKEOFF__ITEM( item_id, item_unique );
	}
}


//-----------------------------------------------------------------------------
// Desc: 플레이어 생성
//-----------------------------------------------------------------------------
void CBravoPC::SM__APPEAR( long unique, PCTABLE& pc )
{
	m_PcTable[m_PcTableCount] = pc;
	m_PcTable[m_PcTableCount].lUnique = unique;
	m_Pc[m_PcTableCount].m_nTableIndex = m_PcTableCount;
	m_Pc[m_PcTableCount].m_bPC = true;
	m_Pc[m_PcTableCount].CreateNonPlayer();	
	m_Pc[m_PcTableCount].m_State.bWalk = true;			
	m_Pc[m_PcTableCount].m_bIsLoaded = true;
	m_Pc[m_PcTableCount].m_bIsVisible = true;	

	int		nPosX  = m_MainPC.lDestBlock%m_cSize;
	int		nPosY  = m_MainPC.lDestBlock/m_cSize;
	m_Pc[m_PcTableCount].SetTilePos( nPosX, nPosY );

	g_Map.SetMapData( PC_BLOCK, m_PcTableCount, m_MainPC.lDestBlock, true );
	
	m_PcTableCount++;

	return;
}

/// sooree network
int CBravoPC::AppearPC( long unique, PCTABLE& pc )
{
	int i = 0;
	for(  i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_PcTable[i].lUnique == -1 )	/// 빈 위치 
		{
			m_PcTable[i].Init();			// 카메라 워크관련 By wxywxy
			m_PcTable[i] = pc;
			m_PcTable[i].lUnique = unique;			
			m_Pc[i].m_nTableIndex = i;
			m_Pc[i].m_bPC = true;
			m_Pc[i].CreateNonPlayer();	
			m_Pc[i].m_State.bWalk = false;	/// 일단은 모두 뛰게 설정합니다. 
			m_Pc[i].m_bIsLoaded = true;
			m_Pc[i].m_bIsVisible = true;				

			int		nPosX, nPosY;
			nPosX = pc.lBlock % m_cSize;
			nPosY = pc.lBlock / m_cSize;

			m_Pc[i].SetTilePos( nPosX, nPosY );

			break;
		}
	}

	return i;
}

/// sooree network
int CBravoPC::DisappearPC( DWORD aPCID )	
{
	int i = 0;
	for(  i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( ( m_PcTable[i].lUnique == aPCID ) && ( m_Pc[i].m_bPC == true ) )
		{
			// 이펙트 종료 루틴 By wxywxy
//			g_Particle.StopDisplay( m_PcTable[i].lUnique, m_PcTable[i].nIndex );
//			g_Billboard.DelManager( m_PcTable[i].lUnique, m_PcTable[i].nIndex ); 

			///-- 큐에 남아있던 공격적용루틴을 일괄처리한다.
			m_Pc[i].AllActQueue();
			
			g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Pc[i] );

			g_Particle.DelEffSetCon( m_PcTable[i].lUnique , m_Pc[i].GetIndex() );
			g_Particle.StopDisplay(  m_PcTable[i].lUnique , m_Pc[i].GetIndex() );
			g_Billboard.DelManager(  m_PcTable[i].lUnique , m_Pc[i].GetIndex() ); 

			m_PcTable[i].Init();	/// lUnique = -1 로 세팅 
			m_Pc[i].Destroy();

			m_Pc[i].m_bIsLoaded = false;
			m_Pc[i].m_bIsVisible = false;	
			break;
		}
	}
	return i;
}

int CBravoPC::GetAttrIndex(BYTE aCitizen,BYTE aGender)
{
	if( aCitizen == MILLENA )
	{
		if( aGender == MALE )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else 
	{
		if( aGender == MALE )
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}

	return 0;
}

//...............................................................................................................
// AppearNPC 메세지 받은후 신수이면 처리
//...............................................................................................................
int CBravoPC::ISCheckHoly(SRpScAppearNPC *aMsg)
{
	SNPCBaseInfo* npc_Info;

	BYTE * pBuff = NULL;
	pBuff = (BYTE *)aMsg;

	int nPackSize = 0;
	
#ifdef PACKET_RESIZE
	
	nPackSize += sizeof( aMsg->theMsgHeader ) + sizeof( DWORD ) + sizeof( WORD ) + sizeof( DWORD ) + sizeof( DWORD );
	nPackSize += sizeof( BYTE ) + sizeof( WORD ) + sizeof( BYTE );	
	
	DWORD aNPCID = aMsg->theNPCID;
	WORD  aNPCType = aMsg->theNPCType;
	
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCType ); 

	if( npc_Info == NULL )
		return FALSE;

	if( npc_Info->theRace != NPC_RACE_HOLY )
		return FALSE;

	//...........................................................................................................
	// 신수 처리
	//...........................................................................................................
	list< SDelayAppearNPC * >::iterator i = g_DelayActList.m_DelayAppearNPCList.begin();
		
	for( ; i != g_DelayActList.m_DelayAppearNPCList.end() ; ++i )
	{			
		if( ( *i ) && ( *i )->AppearNPC.theNPCID == aNPCID )
		{
			SDelayAppearNPC * pData = (*i);
			SAFE_DELETE( pData );
			(*i) = NULL;

			g_DelayActList.m_DelayAppearNPCList.erase( i++ );

			RLG1( "------------------- Delete Holy List %d------------------- \n" , aNPCID );
		}				
	}
	
	float fAngle = 0.0f;
	BYTE  aPriorityType = n_NONEIDType;
	DWORD aPriorityID = 0;	

	if( aMsg->theFrontierFlag && !aMsg->thePriorityFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);		
	}

	if( aMsg->thePriorityFlag && !aMsg->theFrontierFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);
	}

	//...........................................................................................................
	// 신수 처리
	//...........................................................................................................
	if( fAngle ) // 소환시
	{
		SDelayAppearNPC* pDelayAppearNPC = NULL;
		pDelayAppearNPC = SAFE_NEW( SDelayAppearNPC );				
		pDelayAppearNPC->DelayTime = SAFE_TIME_ADD( g_nowTime , 800 );
		pDelayAppearNPC->AppearNPC = *( aMsg );
		g_DelayActList.m_DelayAppearNPCList.push_back( pDelayAppearNPC );
		
		int pX = (int)(aMsg->theCellIndex%g_Pc.m_cSize);
		int pZ = (int)(aMsg->theCellIndex/g_Pc.m_cSize);
		
		float fX = pX * (TILE_SIZE/2) + (TILE_SIZE/4);
		float fZ = pZ * (TILE_SIZE/2) + (TILE_SIZE/4);
		float fY = g_Map.Get__MapHeightF( fX , fZ ) + + 15.0f;
		
		int index = g_Pc_Manager.m_HolyManager.GetCreateEffect( aNPCType ,
			aPriorityType );

		g_Particle.SetEffSetCon( 0.0f , index , ESLT_POS , 8.1f , -1 , -1 , 
			D3DXVECTOR3( fX , fY , fZ  ) );

		g_Sound.Play( 294 , fX , fY , fZ );

		RLG1( "------------------- Insert Holy List %d------------------- \n" , aNPCID );
	}
	else
	{
		g_RockClient.Proc_RpScAppearHoly( aMsg );
	}

#else

	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->theNPCType ); 

	if( npc_Info == NULL )
		return FALSE;

	if( npc_Info->theRace != NPC_RACE_HOLY )
		return FALSE;

	//...........................................................................................................
	// 신수 처리
	//...........................................................................................................
	list< SDelayAppearNPC * >::iterator i = g_DelayActList.m_DelayAppearNPCList.begin();
		
	for( ; i != g_DelayActList.m_DelayAppearNPCList.end() ; ++i )
	{			
		if( ( *i ) && ( *i )->AppearNPC.theNPCID == aMsg->theNPCID )
		{
			SDelayAppearNPC * pData = (*i);
			SAFE_DELETE( pData );
			(*i) = NULL;

			g_DelayActList.m_DelayAppearNPCList.erase( i++ );

			RLG1( "------------------- Delete Holy List %d------------------- \n" , aMsg->theNPCID );
		}				
	}		

	//...........................................................................................................
	// 신수 처리
	//...........................................................................................................
	if( aMsg->theAngle ) // 소환시
	{
		SDelayAppearNPC* pDelayAppearNPC = NULL;
		pDelayAppearNPC = SAFE_NEW( SDelayAppearNPC );				
		pDelayAppearNPC->DelayTime = SAFE_TIME_ADD( g_nowTime , 800 );
		pDelayAppearNPC->AppearNPC = *( aMsg );
		g_DelayActList.m_DelayAppearNPCList.push_back( pDelayAppearNPC );
		
		int pX = (int)(aMsg->theCellIndex%g_Pc.m_cSize);
		int pZ = (int)(aMsg->theCellIndex/g_Pc.m_cSize);
		
		float fX = pX * (TILE_SIZE/2) + (TILE_SIZE/4);
		float fZ = pZ * (TILE_SIZE/2) + (TILE_SIZE/4);
		float fY = g_Map.Get__MapHeightF( fX , fZ ) + + 15.0f;
		
		int index = g_Pc_Manager.m_HolyManager.GetCreateEffect( aMsg->theNPCType ,
			aMsg->thePriorityType );

		g_Particle.SetEffSetCon( 0.0f , index , ESLT_POS , 8.1f , -1 , -1 , 
			D3DXVECTOR3( fX , fY , fZ  ) );

		g_Sound.Play( 294 , fX , fY , fZ );

		RLG1( "------------------- Insert Holy List %d------------------- \n" , aMsg->theNPCID );
	}
	else
	{
		g_RockClient.Proc_RpScAppearHoly( aMsg );
	}

#endif

	return TRUE;
}	

//...............................................................................................................
// DisAppearNPC 메세지 받은후 신수이면 처리
//...............................................................................................................
int CBravoPC::ISCheckDisappearHoly(SRpScDisappearNPC *aMsg)
{
	//...........................................................................................................
	// 신수 처리
	//...........................................................................................................
	list< SDelayAppearNPC * >::iterator i = g_DelayActList.m_DelayAppearNPCList.begin();
		
	for( ; i != g_DelayActList.m_DelayAppearNPCList.end() ; ++i )
	{			
		if( ( *i ) && ( *i )->AppearNPC.theNPCID == aMsg->theNPCID )
		{
			SDelayAppearNPC * pData = (*i);
			SAFE_DELETE( pData );
			(*i) = NULL;
			
			g_DelayActList.m_DelayAppearNPCList.erase( i++ );

			RLG1( "------------------- Delete Holy List %d------------------- \n" , aMsg->theNPCID );
	
			return TRUE;	
		}				
	}		

	return FALSE;
}


int	CBravoPC::GetDanceMotion( PCTABLE * pTable )
{
	if( pTable->char_info.theCitizen == MILLENA )
	{
		if( pTable->char_info.theMainClass == Knight )
		{
			return n_Dance_Night;
		}
		
		if( pTable->char_info.theMainClass == Mercenary )
		{
			return n_Dance_Mercenary;
		}

		if( pTable->char_info.theMainClass == Sniper )
		{
			return n_Dance_Sniper;
		}

		if( pTable->char_info.theMainClass == Slayer )
		{
			return n_Dance_Slayer;
		}	
	}
	else
	{
		if( pTable->char_info.theMainClass == Psyche )
		{
			return n_Dance_Psyche;
		}
		
		if( pTable->char_info.theMainClass == Prominas )
		{
			return n_Dance_Prominas;
		}

		if( pTable->char_info.theMainClass == Priest )
		{
			return n_Dance_Priest;
		}

		if( pTable->char_info.theMainClass == Holy_Avenger )
		{
			return n_Dance_Holy_Avenger;
		}
	}

	return -1;
}
