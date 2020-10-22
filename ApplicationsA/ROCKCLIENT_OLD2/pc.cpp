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

#define		SELECTBOX_ANITIME			25f			// 0.025��
#define		SELECTBOX_ENDFRAME			10
#define		SELECTBOX_SCALESIZE			0.01f

#define		IMOTICON_SIZE				64			// �̸�Ƽ�ܻ�����
#define		IMOTICON_PLAYTIME			4000.0f		// �̸�Ƽ�� �׸��� �ð� 4��
#define		IMOTICON_ANICOUNT			2			// �̸�Ƽ�� �ִϰ���
		
#define		SELECTMAP_POS				16
#define 	SELECTMAP_HEIGHT			2.1f
#define		MESSAGEBOX_YES				6

#define		OPERATOR_PASSWORD			1004		// ��� ��й�ȣ


//-----------------------------------------------------------------------------
// Desc: �˱�(�ܻ�)����
//-----------------------------------------------------------------------------
void	TextureTable::Create()
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �˱� �޸� �Ҵ�
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

	// �׸��� �ؽ��� �޸� �Ҵ�
	strcpy( file_path, g_RockClient.GetRootDir());
	strcat( file_path, "\\Etc\\shadow.tex");
	g_RenderManager.CreateTextureFromFileEx( file_path, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
											 D3DX_FILTER_NONE, 0, NULL, NULL, &pShadow );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

		// �˱� �޸� �Ҵ�
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

	// �׸��� �ؽ��� �޸� �Ҵ�
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
// Desc: �׸��� ���̺�
//-----------------------------------------------------------------------------
void ShadowTable::Clear()
{
	// �����۴�

	// �۴�

	// �߰�
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

	// ũ��

	// �ſ� ũ��
}


//-----------------------------------------------------------------------------
// Desc: CBravoPC �ʱ�ȭ
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

	// ����Ʈ�ؽ���
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
	//m_SwordTail�� �ʿ�� ���۸� �ڵ�����
	return S_OK;
}

bool		CBravoPC::Init(int size)
{
	m_PcTableCount	= 0;
	m_cSize	= size;
	m_cSize = 256;

	InitPC(m_MainPC);
	InitTable();

	m_nPickedUser		= -1;		// ���� ���� ����
	lReceiveBlock		= -1;		// ������ ���� ����ġ
	m_DisplayQuest		= false;	// ����Ʈ �׷�?
	m_CurrentQuest		= 0;		// ���� ���� ����Ʈ(QUEST_CLASS�߿� �ϳ�)
	m_MakePlayerIndex	= -1;		// ĳ���� ���� �ε���
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
	// �÷��̾� �϶�
	if( GetMainPCUniqe() == pSrc->GetUnique() )
	{
		return &m_MainPC;
	}
	// �ٸ� ���� �϶�
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
// Desc: NPC ��ġ ����
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
// Desc: ���� �̸�, ����
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
// Desc: ����, ����
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
// ����Ʈ ��½� �� ��Ʈ���� ���ϱ� ( NPC �� �߰� )
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
	

	// ĳ���͸� �� ã������
	if( pSrc == NULL )
		return FALSE;

 	switch( Num )
	{
		//.......................................................................................................
		// ��ũ�� �� ����
		//.......................................................................................................
		case 0:
			
			D3DXMatrixIdentity( &mat );

			return TRUE;

		//.......................................................................................................
		// �Ӹ�
		//.......................................................................................................
		case 1:

			mat = pSrc->m_matRealHair;

			return TRUE;

		//.......................................................................................................
		// ����
		//.......................................................................................................
		case 2:

			mat = pSrc->m_matRealbreast;

			return TRUE;

		//.......................................................................................................
		// ����
		//.......................................................................................................
		case 3:

			mat = pSrc->m_matRealCenter;

			return TRUE;

		//.......................................................................................................
		// �޼�
		//.......................................................................................................
		case 4:

			mat = pSrc->m_matRealLHand;

			return TRUE;

		//.......................................................................................................
		// ������
		//.......................................................................................................
		case 5:

			mat = pSrc->m_matRealRHand;

			return TRUE;

		//.......................................................................................................
		// �޹�
		//.......................................................................................................
		case 6:

			mat = pSrc->m_matRealLFoot;

			return TRUE;

		//.......................................................................................................
		// ������
		//.......................................................................................................
		case 7:

			mat = pSrc->m_matRealRFoot;

			return TRUE;

		//.......................................................................................................
		// Į��
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
		// Į��
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
// Desc: �ٿ��ڽ��� ����
// 0�ִ�, 1�ּ�, 2(�ִ�-�ּ�)/2
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
// Desc: ���� ���̴��� üũ
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
// Desc: ������ ������
//-----------------------------------------------------------------------------
/*
void		CBravoPC::CM__WEATHER( int type )
{
}
*/

//-----------------------------------------------------------------------------
// Desc: �ڱ�ĳ���� ����ȭ�鿡�� ����� ĳ���� �����ϱ�
//-----------------------------------------------------------------------------
void	CBravoPC::CreateTempPlayer()
{
/*	// ����ִ� �迭��ȣã��
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

	// ����â ĳ���� ����
	g_Pc.m_MakePlayerIndex = -1;

	// ����, ����ã��
	int temp, sex, race, job;
//	temp = g_Net.m_SaveInfo[nIndex].m_Race;
//	sex  = (temp >> CHAR_SHIFT_KEY_SEX) & CHAR_KEY;
//	temp = g_Net.m_SaveInfo[nIndex].m_Race;	
//	race = (temp >> CHAR_SHIFT_KEY_RACE) & CHAR_KEY;
//	temp = g_Net.m_SaveInfo[nIndex].m_Race;
//	job  = temp & CHAR_KEY;	

	// ĳ���� �Ӽ�����
	m_TempIndex[nIndex] = SetCharacterAttr( job, race, sex );

	// ĳ���ͻ���
	m_TempPlayer[nIndex].Init();
	m_TempPlayer[nIndex].m_nTableIndex = nIndex;
	m_TempPlayer[nIndex].CreatePlayer( 1 );

	// �Ӹ���, �Ӹ����, �󱼸�� ����
//	ChangePlayerStyle( g_Net.m_SaveInfo[nIndex].m_Style, nIndex );

	// ĳ������ġ
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


// ����ȭ�� ������
void	CBravoPC::DisplayTempPlayer()
{
	// ĳ����
	for( int i = 0; i < MAX_CHARACTER_COUNT; ++i )
	{
		if( m_bTempPlayer[i] == true )
		{
			m_TempPlayer[i].AniProcess();
			m_TempPlayer[i].AniRender();
			m_TempPlayer[i].Display( DISPLAY_SELECT );
		}
	}

	// NPC(Ÿ�� �ִ� �������� �׸���)
	g_Map.AniProcessNpc();
	g_Map.DisplayNpc();
}


// ����ȭ�� ĳ���� ����
void	CBravoPC::DeleteTempPlayer( bool bAll )
{
	// ���λ���
	if( bAll == true )
	{
		for( int i = 0; i < MAX_CHARACTER_COUNT; ++i )
		{
			m_bTempPlayer[i] = false;
			m_TempPlayer[i].Destroy();
		}
	}
	// �Ѹ� ����
	else
	{
		// ���ִٸ� ���� ����
		// ����
	}
}


//-----------------------------------------------------------------------------
// Desc: ����ȭ��â�� ������ ĳ����
//-----------------------------------------------------------------------------
void	CBravoPC::ChangePlayerStyle( UINT style, long lUnique )
{
	/*
	//--------------------//---------------------------------------------------------
	// �Ӹ�����, �Ӹ����, �󱼸��
	//-----------------------------------------------------------------------------
	DWORD face_index = style & 31;
	style = style >> 5;	
	DWORD hair_index = style & 31;
	style = style >> 5;
	DWORD hair_color = style & 31;	

	//-----------------------------------------------------------------------------
	// ĳ���� ����ȭ��
	//-----------------------------------------------------------------------------
	if( g_RockClient.GetGameMode() == GMODE_SELECT )
	{
		m_TempPlayer[lUnique].SetMaterialColor( lUnique, HAIR_INDEX, hair_color );
		m_TempPlayer[lUnique].ChangeHair( lUnique, hair_index );
		m_TempPlayer[lUnique].ChangeFace( lUnique, face_index );
	}
	//-----------------------------------------------------------------------------
	// ������
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
	// ĳ���� ���� â
	//-----------------------------------------------------------------------------
	if( lUnique == -1 )
	{
		m_MakePlayer.SetMaterialColor( m_MakePlayerIndex, HAIR_INDEX, color );
		m_MakePlayer.ChangeHair( m_MakePlayerIndex, hair );
		m_MakePlayer.ChangeFace( m_MakePlayerIndex, face );
	}
	//-----------------------------------------------------------------------------
	// ������
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
		// �ε�������
		m_MakePlayerIndex = _nIndex;
		if( m_MakePlayerIndex != -1 )
		{
			// ĳ���ͻ���
			m_MakePlayer.Init();
			m_MakePlayer.m_nTableIndex = MAX_USER_INTABLE;
			m_MakePlayer.CreatePlayer( 2 );
			
			// �Ӹ���, �Ӹ����, �󱼸�� ����
			m_MakePlayer.SetMaterialColor( _nIndex, HAIR_INDEX, 0 );
			m_MakePlayer.ChangeHair( _nIndex, 0 );
			m_MakePlayer.ChangeFace( _nIndex, 0 );
				
			// ĳ������ġ
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
// Desc: ������ ����Ʈ �ε�
//-----------------------------------------------------------------------------
void CBravoPC::CreateDamageEffect(void)
{
	m_DamageEffect.Load( g_RockClient.GetRootDir() );
}


//-----------------------------------------------------------------------------
// Desc: ����Ʈ �ؽ��� �ε�
//-----------------------------------------------------------------------------
void	CBravoPC::CreateEffectData()
{

}


//-----------------------------------------------------------------------------
// Desc: �κ��丮�� �ִ� ������ �˻�
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
// Desc: �ڱ� ĳ���� ����(�����۱���)
//-----------------------------------------------------------------------------
void	CBravoPC::CreateChracterItem( Item_Info_Save* pInventory1, Item_Info_Save* pEquip )
{
/*	sooree network
	//-----------------------------------------------------------------------------
	// ���� ��������,�����
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
	// ����
	//-----------------------------------------------------------------------------
	for( i = 0; i < MAX_INVENTORY1; ++i )
	{
		// �κ��丮���� ����� ������
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
// Desc: ������ ���� �Ӽ�����
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
	case FAIRY:		// ��
		nAttr = UNION_FAIRY;
		break;
	case ELIADEN:	// �����Ƶ�
		nAttr = UNION_FAIRY;
		break;
	case ROIEL:		// ���̿�
		nAttr = UNION_FAIRY;
		break;
	default:
		{
			switch( _nRace )
			{
			case UNION:	// �ΰ�
				{
					// ����
					if( _nSex == 0 )
						nAttr = UNION_MAN;
					// ����
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
// Desc: �ڱ� ĳ���� �α���
//-----------------------------------------------------------------------------
void		CBravoPC::SetupMainPc( PCTABLE &pc )
{
	m_MainPC = pc;
	lReceiveBlock = m_MainPC.lDestBlock;

	//-----------------------------------------------------------------------------
	// ĳ���� �Ӽ�����
	//-----------------------------------------------------------------------------
//	m_MainPC.nIndex = SetCharacterAttr( m_MainPC );
	m_MainPC.nIndex = pc.nIndex;	

	//-----------------------------------------------------------------------------
	// ĳ���ͻ���
	//-----------------------------------------------------------------------------
	m_Player.m_nTableIndex = MAX_USER_INTABLE;
	m_Player.CreatePlayer();

	//-----------------------------------------------------------------------------
	// ���� ��ġ �����ϱ�
	//-----------------------------------------------------------------------------
	int		nPosX  = m_MainPC.lDestBlock%m_cSize;
	int		nPosY  = m_MainPC.lDestBlock/m_cSize;
	m_Player.SetTilePos( nPosX, nPosY );	

	//-----------------------------------------------------------------------------
	// �Ӹ���, �Ӹ����, �󱼸�� ����
	//-----------------------------------------------------------------------------
	UINT style = 0;
//	ChangePlayerStyle( style, m_MainPC.lUnique );

	//-----------------------------------------------------------------------------
	// ����Ʈ����
	//-----------------------------------------------------------------------------
/*	Sinario_Quest	Quest = g_Net.m_SaveInfo[m_nPickedUser].s_Quest;

	// �ó������ε����� ����Ʈ �����ϱ�
	if( Quest.m_SQIndex.m_byValue[0] != BYTE_FAIL )
	{
		SM__GET__QUEST( 0, QUEST_SCENARIO, Quest.m_SQIndex.m_byValue[0] );

		// �����ð�, ���缺������ �����ϱ�
		m_QuestInfo[QUEST_SCENARIO].table.req.Time				= Quest.m_QuestTime;
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[0]	= Quest.m_SQIndex.m_byValue[1];	
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[1]	= Quest.m_SQStatus.m_byValue[0];
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[2]	= Quest.m_SQStatus.m_byValue[1];
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[3]	= Quest.m_SQStatus.m_byValue[2];
		m_QuestInfo[QUEST_SCENARIO].table.target.CurCount[4]	= Quest.m_SQStatus.m_byValue[3];
	}
*/
	//-----------------------------------------------------------------------------
	// �����ͽ�
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// �κ��丮
	//-----------------------------------------------------------------------------
	m_nPickedUser = -1;
}

//-----------------------------------------------------------------------------
// Desc: ��Ÿ�� �ִ��� ����
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
// Desc: �ٸ� ĳ���Ϳ� �浹��.. �̵��� ��ġ ������
//-----------------------------------------------------------------------------
void		CBravoPC::SetPlayerMove( DWORD pos )
{
	// �̵���ġ ť���ֱ�
	EVENT_DATA_INFO	data;
	data.type = SM_MOVE_BROADCAST;
	data.x = pos%m_cSize;
	data.y = pos/m_cSize;
	m_Player.PushQueue( data );
}


//-----------------------------------------------------------------------------
// Desc: ������ ���� �ڱ���ġ ������
//-----------------------------------------------------------------------------
void		CBravoPC::SetWarpMove( PCTABLE& pc )
{
	m_MainPC = pc;

	// ����ġ����
	lReceiveBlock = m_MainPC.lDestBlock;

	// ����ġ�� ����
/*	if( m_Player.m_State.bRide == true )
	{
		g_Map.m_NpcTable[m_Player.m_State.horse_index].lBlock     = m_MainPC.lDestBlock;
		g_Map.m_NpcTable[m_Player.m_State.horse_index].lDestBlock = m_MainPC.lDestBlock;
	}
	// ��ü�� ���´�
	if( m_Player.m_State.Unite_unique != -1 )
	{
	}
	// ���� ������ ���
	if( m_MainPC.nIndex == UNION_FAIRY )
	{
		// ��ü ���°� �ƴѵ�.. �������� �۴ٸ�.. ��ü���¿��� �׾� �����Ѱ���..
		// �׷���� ���� ũ�Ⱚ���� ������� �Ѵ�
		if( m_Player.m_State.Unite_unique == -1 && m_Player.m_vScale.x != 0.9f )
		{
			m_Player.m_vScale.x = 0.9f;
			m_Player.m_vScale.y = 0.9f;
			m_Player.m_vScale.z = 0.9f;
		}
	} */
	
	// ���� ��ġ �����ϱ�
	int		nPosX  = m_MainPC.lDestBlock%m_cSize;
	int		nPosY  = m_MainPC.lDestBlock/m_cSize;
	m_Player.SetTilePos( nPosX, nPosY );

	// �����߱⶧���� ����� ��� �ൿ �����
	m_Player.ClearEvent();
	m_Player.Clear();

	// ���� ���, �� �̵� ó�� ����
/*	if( m_MainPC.nIndex == UNION_FAIRY )
	{
		g_Particle.DisplayUpdate( 7, m_MainPC.lUnique, -1, -1 );	//
		g_Billboard.SetManager( 0.0f, 29, m_MainPC.lUnique, -1 );
		g_Billboard.SetManager( 0.0f, 30, m_MainPC.lUnique, -1 );

		// ��ü�� �ƴϰ�
		if( !GetPlayer()->m_State.bUnite )
		{
			// ���� ���� ���
//			if( GetPlayer()->m_pUserInfo->bCharge ) 
//				g_Billboard.SetManager( 0.0f, 8, m_MainPC.lUnique, -1 );
		}
	}
	else if( m_MainPC.char_info.job == NOVICE || m_MainPC.char_info.job == KNIGHT )
	{
		// ��ü��
		if( GetPlayer()->m_State.bUnite )
		{
		}
	} */

	// ���� ���� ���
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
// Desc: PC, NPC ����Ÿ ó�� & ������
//-----------------------------------------------------------------------------
void CBravoPC::AniProcess()
{
	//-----------------------------------------------------------------------------
	// �ڱ� ĳ����
	//-----------------------------------------------------------------------------
	m_Player.UpdateHPData();
	m_Player.QueueProcess();	
	m_Player.AniProcess();
	
	//-----------------------------------------------------------------------------
	// �ٸ� ĳ����
	//-----------------------------------------------------------------------------
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_Pc[i].m_bIsLoaded == true )
		{
			m_Pc[i].UpdateHPData();
			// ť�� ����ִ� �޼�������
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


// ������
void CBravoPC::Display() 
{	
}


//-----------------------------------------------------------------------------
// Desc: �̸�Ƽ�� �׸���
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
			// ����� �ð������ٸ� �����ϱ�
			//-----------------------------------------------------------------------------
			if( SAFE_TIME_COMPARE( m_ImoticonTable[i].time , <= , g_nowTime ) )			
				m_ImoticonTable[i].Clear();
			//-----------------------------------------------------------------------------
			// ������
			//-----------------------------------------------------------------------------
			else
			{
				//-----------------------------------------------------------------------------
				// ������ġ 
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
					// �����ٸ� Y�� �ø���
					if( GetHorseState( FindUser(m_ImoticonTable[i].unique) ) == true )
						vPos.y	-= 40;
				}
				//-----------------------------------------------------------------------------
				// NPC��ġ
				//-----------------------------------------------------------------------------
//				else				
//					vPos = g_ChatList.From3DTo2D2(g_Map.FindNpc(m_ImoticonTable[i].index, m_ImoticonTable[i].unique));
				vPos.x	-= (IMOTICON_SIZE/2);
				vPos.y	-= (IMOTICON_SIZE+5);	
				
				//-----------------------------------------------------------------------------
				// ���� �ִϰ� ���
				//-----------------------------------------------------------------------------
				int	time	= SAFE_TIME_SUB( IMOTICON_PLAYTIME , ( SAFE_TIME_SUB( m_ImoticonTable[i].time , g_nowTime ) ) );
				int ani		= (time/200)%2;

				// �̸�Ƽ�� Ÿ�Կ� ���� �ؽ��Ŀ����� ��ġ��
				// ù��°��
				if( m_ImoticonTable[i].type < 8 )
					SetRect( &rect, IMOTICON_SIZE*m_ImoticonTable[i].type, ani*IMOTICON_SIZE, IMOTICON_SIZE*(m_ImoticonTable[i].type+1), (ani+1)*IMOTICON_SIZE );
				// �ι�°��
				//else if( m_ImoticonTalbe[i].type < 16 )
				//	SetRect( &rect, IMOTICON_SIZE*m_ImoticonTable[i].type, IMOTICON_SIZE, IMOTICON_SIZE*(m_ImoticonTable[i].type+1), IMOTICON_SIZE );

				// �׸���
//				g_RockClient.m_pSprCursor->Draw( m_TexTable.pImoticon, &rect, NULL, NULL, 0.0f, &vPos, D3DCOLOR_ARGB(255,255,255,255) );
			}
		}
	}
//	g_RockClient.m_pSprCursor->End();
}


// �̸�Ƽ�� ����
void CBravoPC::SetImoticon( int index, DWORD unique, int type )
{
	// ���� ����� �̸�Ƽ���� �ִٸ�.. �����Ѵ�
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_ImoticonTable[i].index == index && m_ImoticonTable[i].unique == unique )
			m_ImoticonTable[i].Clear();		
	}

	// �̸�Ƽ�� �����Ѵ�
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
	// �����Ѵ�
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( m_ImoticonTable[i].index == index && m_ImoticonTable[i].unique == unique )
			m_ImoticonTable[i].Clear();		
	}
}

//-----------------------------------------------------------------------------
// Desc: ����Ʈó��
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
	// �ʱ�ȭ
	m_QuestInfo[Class].bInit = false;
	m_QuestInfo[Class].table.req.Init();
	m_QuestInfo[Class].table.target.Init();

	// ���ڹ迭 �޸����� 
	int i = 0;
	for(  i = 0; i < m_QuestInfo[Class].text.SubjectMaxLine; ++i )
		m_QuestInfo[Class].text.Subject[i].Destroy();
	for( i = 0; i < m_QuestInfo[Class].text.ObjectMaxLine; ++i )
		m_QuestInfo[Class].text.Object[i].Destroy();
	for( i = 0; i < m_QuestInfo[Class].text.GiftMaxLine; ++i )
		m_QuestInfo[Class].text.Gift[i].Destroy();
}

// ����Ʈ ���¾�����Ʈ
void CBravoPC::UpdateQuest( int index, BYTE type )
{	
}

// ����Ʈâ���� �׸���
void CBravoPC::DisplayQuest()
{	
}
//-----------------------------------------------------------------------------
// Desc: ������(�̸�) 
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
// Desc: ���⿡ ���� ����Ʈ������
//-----------------------------------------------------------------------------
void CBravoPC::EffectDisplay()
{	
	// ����Ʈ
	m_Player.DisplayEffect();			
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{			
		if( m_Pc[i].m_bIsVisible == true && m_Pc[i].m_bIsLoaded == true  )
			m_Pc[i].DisplayEffect();		
	}	
}

 
//-----------------------------------------------------------------------------
// Desc: �׸���
//-----------------------------------------------------------------------------
void CBravoPC::ShadowMake()
{		
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	// �ٸ� ĳ����		
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{			
		if( m_Pc[i].m_bIsVisible == true && m_Pc[i].m_bIsLoaded == true && m_PcTable[i].bRender == true && 
			m_Pc[i].m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
			m_Pc[i].ShadowMake();
	}

	// �ڱ�ĳ����
	if( m_MainPC.bRender == true && m_Player.m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
		m_Player.ShadowMake();	
}

void CBravoPC::ShadowDisplay()
{	
	if( g_RockClient.m_SceneManager.GetOtherPlayerDisplay() )
	{
		// �ٸ� ĳ����
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
		// �ڱ� ĳ����
		if( m_MainPC.bRender == true && m_Player.m_CharAlpha.GetState() != CHAR_NONE_DISPLAY )
			m_Player.ShadowDisplay();
	}
}


//-----------------------------------------------------------------------------
// Desc: ���콺�� ��ŷ�� Ÿ�ϰ� ������
//-----------------------------------------------------------------------------
bool CBravoPC::SetTile()
{
	EVENT_DATA_INFO	data;
	data.x = g_Map.GetPickedX();
	data.y = g_Map.GetPickedY();
	data.type	= SM_MOVE_BROADCAST;
	m_Player.PushQueue( data );	

	// �ӽ� �̵� �׽�Ʈ ( �ٸ� ������ NPC )
//	if( m_Pc[0].m_bIsVisible == true && m_Pc[0].m_bIsLoaded == true )
//		m_Pc[0].SetQueue( data );
//	if( g_Map.m_Npc[0].m_bIsVisible == true && g_Map.m_Npc[0].m_bIsLoaded == true )
//		g_Map.m_Npc[0].SetQueue( data );

	return	true;
}

//-----------------------------------------------------------------------------
// ����Ŭ�� 
//-----------------------------------------------------------------------------
bool CBravoPC::SetPC()
{
	return true;
}

//-----------------------------------------------------------------------------
// Desc: NPCŬ�� 
//-----------------------------------------------------------------------------
bool CBravoPC::SetNPC()
{
	//-----------------------------------------------------------------------------
	// �޴�â �������� Ŭ���ߴٸ� ����
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

#ifdef HHM_CHARACTER_SELECT_3D_PICK// ĳ���� ����â���� ��ư���� �ϴ� ĳ���� ������ 3D ��ŷ���� �ٲ�(2009/06/12)
//-----------------------------------------------------------------------------
// Desc: ĳ���ͼ���ȭ�鿡�� ��ŷ
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
				// �޴��� ����

				return i;
			}
		}
	}

	return	m_nPickedUser = -1;
*/
}

#endif// #define HHM_CHARACTER_SELECT_3D_PICK// ĳ���� ����â���� ��ư���� �ϴ� ĳ���� ������ 3D ��ŷ���� �ٲ�(2009/06/12)

//-----------------------------------------------------------------------------
// Desc: ĳ���� ��ŷ
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
			continue;						// �ڱ��ڽ��� ��ŷ���� ���ܽ�Ų��.
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
			// �ε��ȵǾ��ְų�, ������ �ʴٸ�
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
					// �׾��ְų�, ���� Ŭ���� ����̶��.. ����
//					if( m_Pc[nIndex].m_curt_event.type == SM_STATE_DIE )
//						return -1;
//---------------------------------------------------------------------------------------------------------------


					//if( g_Pc.m_Select.bPC == true && g_Pc.m_Select.index == i )
					//	return -1;

					// ���콺Ÿ������
//					if( m_Player.m_pUserInfo->bPkMode == true || m_Pc[nIndex].m_pUserInfo->bPkMode == true )
//						g_RockClient.m_byCurCursor = BC_ATTACKENABLE;
//					else
//						g_RockClient.m_byCurCursor = BC_PERSON;
//				}
				
				// �ΰ�ȿ�� (����Ÿ��) ó�� 
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
				
				// Ŭ���� ���� ����
				m_nPickedUser = nIndex;
				
				return nIndex;
			}
		}
	}

	return	-1;
}

//-----------------------------------------------------------------------------
// Desc: ���� ���̺� ����(���̴�?)
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

				// ���� ����ȭ�鿡���� �� �÷��̾ ���� ������ �Ÿ��� ��� 0 ���� �ʱ�ȭ
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
// Desc: ���� ���̺� ����(���̴�?)
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
// Desc: ���� ���� ����
//-----------------------------------------------------------------------------
D3DXVECTOR3	CBravoPC::GetPlayerSightDir( long unique, float SightLength, float DeltaRad )
{
	float rad  = GetDirection(unique);
	float Vecx = GetPosTM(unique)._41;
	float Vecz = GetPosTM(unique)._43;
	float Vecy = GetPosTM(unique)._42;


	//v�� ĳ���� �߰� ��ǥ ���̴� ����� + �ٿ�� �ڽ� �߰� 
	D3DXVECTOR3 v = D3DXVECTOR3( Vecx, Vecy, Vecz );

	D3DXMATRIX mat;
	D3DXMATRIX mattemp;

	// ���⼭ (0.0f 0.0f -SightLength) ���� ���� ���� ���� SightLength ���� ���Ÿ� ������ ���� 
	D3DXMatrixTranslation( &mat, 0.0f, 0.0f, (0.0f - SightLength) );

	//���� ��ŭ ���͸� ȸ�� ��Ų �Ŀ� 
	D3DXMatrixRotationY( &mattemp, rad + DeltaRad );
	D3DXMatrixMultiply( &mat, &mat, &mattemp );

	//w��� ȸ���� ���͸� ���� ���� 
	D3DXVECTOR3 w = D3DXVECTOR3( mat._41, mat._42, mat._43 );

	//ĳ���� ��ǥ�� + ȸ���� ���͸� ���ϸ� ĳ���� �ݰ泻�� ó�ٺ��� ���� ���´� 
	v += w;

	return v;
}

void CBravoPC::SM__CHANGE__ITEM( long unique, DWORD item_id, DWORD item_unique, DWORD item_pos, BYTE pos )
{
	// �ڱ� ĳ����
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
// Desc: �����۹���
//-----------------------------------------------------------------------------
void CBravoPC::SM__TAKEOFF__ITEM( long unique, DWORD item_id, DWORD item_unique )
{
	// �ڱ� ĳ����
	if( unique == m_MainPC.lUnique )
	{
		// ������ ����
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
// Desc: �÷��̾� ����
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
		if( m_PcTable[i].lUnique == -1 )	/// �� ��ġ 
		{
			m_PcTable[i].Init();			// ī�޶� ��ũ���� By wxywxy
			m_PcTable[i] = pc;
			m_PcTable[i].lUnique = unique;			
			m_Pc[i].m_nTableIndex = i;
			m_Pc[i].m_bPC = true;
			m_Pc[i].CreateNonPlayer();	
			m_Pc[i].m_State.bWalk = false;	/// �ϴ��� ��� �ٰ� �����մϴ�. 
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
			// ����Ʈ ���� ��ƾ By wxywxy
//			g_Particle.StopDisplay( m_PcTable[i].lUnique, m_PcTable[i].nIndex );
//			g_Billboard.DelManager( m_PcTable[i].lUnique, m_PcTable[i].nIndex ); 

			///-- ť�� �����ִ� ���������ƾ�� �ϰ�ó���Ѵ�.
			m_Pc[i].AllActQueue();
			
			g_Particle.m_EffectManager.DeleteNow( ( Character * ) &m_Pc[i] );

			g_Particle.DelEffSetCon( m_PcTable[i].lUnique , m_Pc[i].GetIndex() );
			g_Particle.StopDisplay(  m_PcTable[i].lUnique , m_Pc[i].GetIndex() );
			g_Billboard.DelManager(  m_PcTable[i].lUnique , m_Pc[i].GetIndex() ); 

			m_PcTable[i].Init();	/// lUnique = -1 �� ���� 
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
// AppearNPC �޼��� ������ �ż��̸� ó��
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
	// �ż� ó��
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
	// �ż� ó��
	//...........................................................................................................
	if( fAngle ) // ��ȯ��
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
	// �ż� ó��
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
	// �ż� ó��
	//...........................................................................................................
	if( aMsg->theAngle ) // ��ȯ��
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
// DisAppearNPC �޼��� ������ �ż��̸� ó��
//...............................................................................................................
int CBravoPC::ISCheckDisappearHoly(SRpScDisappearNPC *aMsg)
{
	//...........................................................................................................
	// �ż� ó��
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
