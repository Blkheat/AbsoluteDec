// NetworkMgr.cpp: implementation of the CNetworkMgr class.
//////////////////////////////////////////////////////////////////////

#include "RockPCH.h"

//#include "pc.h"

#include "NetworkMgr.h"

#include "RockClient.h"
#include "RockInterface\\Rui.h"
#include "RockInterface\\UiUtil.h"
#include "RockInterface\\MessageWndProc.h"
#include "RockInterface\\CWebShopWnd.h"
#include "stringmanager.h"
#include "PacketTransLogDlg.h"
#include "CSlangFilter.h"
 
#include "quadlist.h"
#include "Bravolibs\\network\\Protocol.h"
#include "Pc.h"

#include "Bravolibs\\Obj\\SkillSystem.h"
#include "RockInterface\\CUserStoreSellWnd.h"
#include "RockInterface\\CUserStoreBuyWnd.h"
#include "md5.h"
#include "RockInterface\\ServerListWndProc.h"

#include "CNetProcThread.h"

extern int				g_Version;
extern long				g_nowTime;


#define USE_ROCKCRYPTO 

CNetworkMgr::CNetworkMgr()
{
	theLoginServerIP[0] = NULL;
	theLoginServerPort = 0;
	
	thePCID = 0xFFFFFFFF;
	
	theConnectState = nNoConnect;
	theLoginSocketPtr = NULL;
	theGameSocketPtr = NULL;

	theCharacterNum = 0;
	theSelectCharIndex = 0;
	
	theWaitingSvrMsgFlag = FALSE;
	memset( theWhisperMessage , NULL , sizeof(theWhisperMessage) );
	
	//by simwoosung
	theActiveCsStartMoveFlag = FALSE;
	theScStartMoveWatingTime = 3.0f;
	theScWatingCumulateTime = 0.0f;
}

CNetworkMgr::~CNetworkMgr()
{
	CleanUpSocketAll();
}

BOOL	CNetworkMgr::WinSockStart()
{
	WSADATA		wsaData;
	if( WSAStartup( MAKEWORD(2,2), &wsaData ) != 0 )
	{
		return FALSE;
	}
	
	return TRUE;
}

void	CNetworkMgr::WinSockClose()
{
	WSACleanup();
}

void	CNetworkMgr::CleanUpSocketAll()
{
	CleanUpSocket( theLoginSocketPtr );
	CleanUpSocket( theGameSocketPtr );
}

void	CNetworkMgr::CleanUpSocket( CNetConnectSocket*& aSocketPtrRef )
{
	if( aSocketPtrRef )
	{
		aSocketPtrRef->Close();		
		SAFE_DELETE( aSocketPtrRef );
	}
}


/// sooree net remodel 
int		CNetworkMgr::NetToLogin( void* aMsgPtr, int aSize )
{
	//by simwoosung
    CheckProtocol((void *) aMsgPtr, FALSE);

#ifdef USE_ROCKCRYPTO
	SRpHeaderPtr pMsgHeader = ( SRpHeaderPtr )aMsgPtr;
	
	pMsgHeader->theVar1 = 5 + BYTE( GetTickCount() & 0xF );	/// 5 ~ 20 Byte Random Padding 
	pMsgHeader->theVar2 = BYTE( rand() & 0xFF );
	pMsgHeader->theSeq = theNetPack2Login.GetNextSeq();

	WORD newSize = ( WORD )aSize + pMsgHeader->theVar1;	
	
	memcpy( theNetPackBuf, aMsgPtr, aSize ); /// ���� �޽��� ������� ����. 
	theNetPack2Login.Encrypt( theNetPackBuf, newSize ); /// padding size �� ��ȣȭ 


	/// ��ȣȭ���� ���� ������ ���� �ϳ��� ���ۿ� ���� �ְ� 	
	theLoginSocketPtr->PushSendQ( &newSize, sizeof( newSize ) );
	theLoginSocketPtr->PushSendQ( theNetPackBuf, newSize );	/// padding size °�� ������. 
#else
	
	theLoginSocketPtr->PushSendQ( aMsgPtr, aSize );

#endif

	return( theLoginSocketPtr->NbSend() );
}

/// sooree net remodel 
int		CNetworkMgr::NetToGame( void* aMsgPtr, int aSize )
{
	//by simwoosung
    CheckProtocol((void *) aMsgPtr, FALSE);
	
	if( NULL == theGameSocketPtr )
	{
		return FALSE;
	}
	
#ifdef USE_ROCKCRYPTO

	SRpHeaderPtr pMsgHeader = ( SRpHeaderPtr )aMsgPtr;

	pMsgHeader->theVar1 = 5 + BYTE( GetTickCount() & 0xF );	/// 5 ~ 20 Byte Random Padding 
	pMsgHeader->theVar2 = BYTE( rand() & 0xFF );
	pMsgHeader->theSeq = theNetPack2Game.GetNextSeq();

	WORD newSize = ( WORD )aSize + pMsgHeader->theVar1;	


	memcpy( theNetPackBuf, aMsgPtr, aSize ); /// ���� �޽��� ������� ����. 
	theNetPack2Game.Encrypt( theNetPackBuf, newSize ); /// padding size �� ��ȣȭ 


	/// ��ȣȭ���� ���� ������ ���� �ϳ��� ���ۿ� ���� �ְ� 	
	theGameSocketPtr->PushSendQ( &newSize, sizeof( newSize ) );
	theGameSocketPtr->PushSendQ( theNetPackBuf, newSize );	/// padding size °�� ������. 
#else
	theGameSocketPtr->PushSendQ( aMsgPtr, aSize );
#endif

	return( theGameSocketPtr->NbSend() );
}

BOOL	CNetworkMgr::ConnectToLoginServer( char* aLoginServerIP, int aLoginServerPort )
{
	/// sooree net remodel 
	int i = 0;
	for( ; i < sizeof( theNetPackBuf ); ++i )
	{
		theNetPackBuf[i] = rand() & 0xFF;
	}

	strcpy( theLoginServerIP, aLoginServerIP );
	theLoginServerPort = aLoginServerPort;
	
	if( CreateAndConnect( theLoginSocketPtr, theLoginServerIP, theLoginServerPort ) == TRUE )
	{
		theNetPack2Login.InitializeIdx();

		theConnectState = nInLoginServer;

		return TRUE;
	}
	
	return FALSE;
}
///---------------------------------------------------------------------------
///-- ConnectToGameServer
///---------------------------------------------------------------------------
///-- ������ Channel �� ������ �õ��Ѵ�.
///-- ���� ������� �ʴ� �Լ�
BOOL	CNetworkMgr::ConnectToGameServer( int aListIndex, char* aGameServerName )
{
	SUPERASSERT( aListIndex >= 0 || aListIndex < theServerSetNum );
	
	if( lstrcmp(theServerSetData[aListIndex].theName, aGameServerName) != 0 )
	{
		RLG3("%dth Name is Invalid : %s asked... but %s", aListIndex, aGameServerName, theServerSetData[aListIndex].theName );
		return FALSE;
	}
	
	if( CreateAndConnect( theGameSocketPtr, theServerSetData[aListIndex].theIP, 
		theServerSetData[aListIndex].thePort ) == TRUE )
	{
		theNetPack2Game.InitializeIdx();
		
		theConnectState = nInGameServer;
		
		Send_RpCsGameLogin();	/// GameServer�� �������ڸ��� C2S_GameLogin ���� 
		
		return TRUE;
	}
	
	return FALSE;
}
///---------------------------------------------------------------------------
///-- ConnectToGameServer
///---------------------------------------------------------------------------
///-- ������ Channel �� ������ �õ��Ѵ�.
BOOL	CNetworkMgr::ConnectToGameServer( int aListIndex )
{
	
	SUPERASSERT( aListIndex >= 0 || aListIndex < theServerSetNum );
	
	if( CreateAndConnect(	theGameSocketPtr, theServerSetData[aListIndex].theIP, 
		theServerSetData[aListIndex].thePort ) == TRUE )
	{
		theNetPack2Game.InitializeIdx();

		theConnectState = nInGameServer;
	
		Send_RpCsGameLogin();	/// GameServer�� �������ڸ��� C2S_GameLogin ���� 
		
		return TRUE;
	}
	
	return FALSE;
}

/// �̰� ���� Loop���� �� frame ���� ȣ���մϴ�.
void	CNetworkMgr::NetworkProc()
{
	//	DWORD error; 
	int ret;
	
	if( theLoginSocketPtr )
	{
		ret = theLoginSocketPtr->NbRecv( 0 );
		
		if( ret == SOCKET_ERROR )
		{
			/*	
			char buf[ 1024 ] = "";
			int  ErrorCode = WSAGetLastError();
			
			wsprintf( buf, "Error CNetworkMgr::NetworkProc : code=%d", ErrorCode );
		  
			RLGS( buf );
			*/			
			OnLoginSocketClose();
		}
		else
		{
			MsgProcLS( theLoginSocketPtr );
		}
	}
	
	if( theGameSocketPtr )
	{
		ret = theGameSocketPtr->NbRecv( 0 );
		if( ret == SOCKET_ERROR )
		{
			PRINT_DLG_LOG( "���� SOCKET_ERROR �߻�" );		
			OnGameSocketClose();		
		}
		else
		{			
			MsgProcGS( theGameSocketPtr );
		}
	}
}

void	CNetworkMgr::OnLoginSocketClose()
{
	CleanUpSocket( theLoginSocketPtr );
	
	if( theConnectState == nInGameServer )		/// ���� 
	{
		PRINT_DLG_LOG( "!�α��μ����� ���� ����(����)" );
		RLGS( "!�α��μ����� ���� ����(����)" );	
		UCHAR uMsg[128]="";


	}
	else	/// ����!
	{
		PRINT_DLG_LOG( "!�α��μ����� ���� ����!!!!!!!!!" );
		RLGS( "!�α��μ����� ���� ����!!!!!!!!!" );		
		CleanUpSocket( theGameSocketPtr );	/// Game Socket�� ���δ�. 
		theConnectState = nNoConnect;
		
		Proc_OnSocketClose();		
	}
}

void	CNetworkMgr::OnGameSocketClose()
{
	PRINT_DLG_LOG( "!���Ӽ����� ���� ����!!!!!!!!!" );
	RLGS( "!���Ӽ����� ���� ����!!!!!!!!!" );	
	CleanUpSocket( theLoginSocketPtr );
	CleanUpSocket( theGameSocketPtr );
	theConnectState = nNoConnect;
	//ServerListWnd
#ifdef _DEBUG
	
	if( ServerListWnd.mb_server == false ) 
	{
		g_RockClient.LogOff();
	}
	
#endif
	Proc_OnSocketClose();	
}

BOOL	CNetworkMgr::CreateAndConnect( CNetConnectSocket*& aSocketPtrRef, char* aIP, int aPort )
{
	CleanUpSocket( aSocketPtrRef );
	
	aSocketPtrRef = SAFE_NEW( CNetConnectSocket );

	if( aSocketPtrRef == NULL )
	{
		RLGS( "Alloc Memory CNetConnectSocket Fail" );	
		return FALSE;
	}
	
	if( aSocketPtrRef->Connect( aIP, aPort ) == FALSE )
	{
		RLG2( " aSocketPtrRef->Connect Fail : %s %d", aIP, aPort );
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_CONNECT2 ) , 2000, NULL );
		
		SAFE_DELETE( aSocketPtrRef );
		
		return FALSE;
	}
	
	aSocketPtrRef->Create( MAX_PACKET_SIZE*2*4, MAX_PACKET_SIZE*2, MAX_PACKET_SIZE );
	//	aSocketPtrRef->SetSockTCPNoDelay(); 		///  client���� default�� ����. 
	aSocketPtrRef->SetSockBufRCV( MAX_PACKET_SIZE*4 );
	aSocketPtrRef->SetSockBufSND( MAX_PACKET_SIZE*4 );
	aSocketPtrRef->SetNetEvent();
	
	return TRUE;
}

/// Login Server Message Dispatch
void	CNetworkMgr::MsgProcLS( CNetConnectSocket* aSocketPtr )
{
	void*			msg;
	int				size;
	SRpHeader*		header;	
	bool			result = true;
	
	WORD			packetSize = 0;	/// ��ȣȭ���� ���� ��Ŷ ������ ������ 2 Byte ũ�� 

	for(;;)
	{
		msg = aSocketPtr->theMsgQRecv.GetBuffer();
		size = aSocketPtr->theMsgQRecv.GetSize();
		//size = 0;
		
#ifdef USE_ROCKCRYPTO
		if( size == 0) 
		{
			break;				/// ���ۿ� 2 Byte�� �ȵ���ֳ� 
		}
		
		if( size < sizeof( WORD ) )
		{
			break;				/// ���ۿ� 2 Byte�� �ȵ���ֳ� 
		}
		
 

		memcpy( &packetSize, msg, sizeof( WORD ) );	/// ��ȣȭ���� ���� size ������ �а�...
		
		if( packetSize > MAX_PACKET_SIZE * 2 )	/// ������ ���� ���� üũ 
		{
			///--SEND_DEBUG_LOG("MsgProcLS Invalid Packet Header : -> size info is too big(%d)", packetSize );
			
			RLG1( "Invalid Packet Header : -> size info is too big(%d)", packetSize );
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}

		if( size < int( packetSize + sizeof( WORD ) ) ) 
		{
			break;	/// ���� �޽��� ũ�� + sizeof( size ���� ) 
		}

		/// ����� ū ũ�Ⱑ �������Ƿ� ��ȣȭ �۾� 
		/// ���� sizeof( size ���� ) pop 
		aSocketPtr->PopRecvQ( sizeof( packetSize ) );

		/// ���� �޽��� ������ ���ͼ� 
		msg = aSocketPtr->theMsgQRecv.GetBuffer();

		/// ��ȣȭ 
		theNetPack2Login.Decrypt( msg, packetSize );

		/// ��� ĳ���� : ���ο� CsHeader 
		header = static_cast< SRpHeaderPtr > ( msg );

		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// ������ ���� ���� üũ 
		{
			///-- SEND_DEBUG_LOG("MsgProcLS Invalid Packet Header : -> size info is too big(%d)", packetSize );
			
			RLG2( "Invalid Packet Header : %d -> size info is too big(%d)", header->theProtocol , header->theSize );
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}

		/// sooree net remodel 
		if( packetSize != ( header->theSize + header->theVar2 ) )
		{
			ClientClose( g_RockClient.GetApp() , NULL );
			return;
		}

#else
		/// ��� Ȯ�� 
		if ( size < sizeof( SRpHeader ) ) break;
		header = ( SRpHeader* ) msg;
		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// ������ ���� ���� üũ 
		{
			RLG2( "Invalid Packet Header : %d -> size info is too big(%d)", header->theProtocol , header->theSize );
			
			///-- SEND_DEBUG_LOG( "MsgProcLS Invalid Packet Header : %d -> size info is too big(%d)" , header->theProtocol , header->theSize );
			
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}
		
		/// ��� �����ߴ°�?
		if( size < header->theSize ) break;
#endif		
		
		//by simwoosung
        CheckProtocol((void *) msg, TRUE);
		
		switch( header->theProtocol )
		{
            //-----------------------------------------------------------------
			// Framework Related
			//-----------------------------------------------------------------
		case n_RpScKeepAliveCheck:
			Send_RpCsKeepAliveCheck( (SRpScKeepAliveCheck*) msg ); 				break;
			
		case n_RpScErrorFramework:
			Proc_RpScErrorFramework( (SRpScErrorFramework*) msg );				
			return;
			
		//case n_RpScWorldList:
		//	Proc_RpScWorldList( ( SRpScWorldList* ) msg );break;

		//case n_RpScChannelList:
			//Proc_RpScChannelList( ( SRpScChannelList* ) msg );break;
			
		case n_RpScServerSetList:
			Proc_RpScServerSetList( (SRpScServerSetList*) msg );				break;

		case n_RpScChangeAccount:
			Proc_RpScChangeAccount( (SRpScChangeAccount*) msg );	break;
		
		case n_RpScHackShield:
			Proc_RpScHackShield( (SRpScHackShield*) msg );	break;
			
		case n_RpScCharacterList:
///			Proc_RpScCharacterList( (SRpScCharacterList*) msg );				
			break;
			
		case n_RpScPrepareLogin:
///			result = Proc_RpScPrepareLogin( (SRpScPrepareLogin*) msg );			
			break;
			
		case n_RpScPCLogin:
///			Proc_RpScPCLogin( (SRpScPCLogin*) msg );							
			break;
			
			//-----------------------------------------------------------------
			// Other Game Protocols : ���� ���������� ��ӹ��� Ŭ�������� ó�� 
			//-----------------------------------------------------------------
		default :
///			Proc_RpScProtocols( msg );											
			break;
		}
		
		/// sooree net remodel 
		aSocketPtr->PopRecvQ( packetSize );
		
	} // for( size )
	
	// Proc_RpScPrepareLogin �ÿ� ������ ���� ���ؼ�...
	if( result == false )
	{
		CleanUpSocketAll();
	}
	
	return;
}

/// Game Server Message Dispatch
void	CNetworkMgr::MsgProcGS( CNetConnectSocket* aSocketPtr )
{
	void*			msg;
	int				size;
	SRpHeader*		header;	
	bool			result = true;
	
	WORD			packetSize = 0;	/// ��ȣȭ���� ���� ��Ŷ ������ ������ 2 Byte ũ�� 

#ifdef _DEBUG

	//int nProcessPCount = 0;
	//long lPrevTime = timeGetTime();
	
#endif

	for(;;)
	{
		msg = aSocketPtr->theMsgQRecv.GetBuffer();
		size = aSocketPtr->theMsgQRecv.GetSize();
		
#ifdef USE_ROCKCRYPTO
		if( size < sizeof( WORD ) ) 
		{
			break;				/// ���ۿ� 2 Byte�� �ȵ���ֳ� 
		}
		
		memcpy( &packetSize, msg, sizeof( WORD ) );	/// ��ȣȭ���� ���� size ������ �а�...
		
		if( packetSize > MAX_PACKET_SIZE * 2 )	/// ������ ���� ���� üũ 
		{
			RLG1( "Invalid Packet Header : -> size info is too big(%d)", packetSize );
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}

		if( size < int( packetSize + sizeof( WORD ) ) ) 
		{
			break;	/// ���� �޽��� ũ�� + sizeof( size ���� ) 
		}

		/// ����� ū ũ�Ⱑ �������Ƿ� ��ȣȭ �۾� 
		/// ���� sizeof( size ���� ) pop 
		aSocketPtr->PopRecvQ( sizeof( packetSize ) );

		/// ���� �޽��� ������ ���ͼ� 
		msg = aSocketPtr->theMsgQRecv.GetBuffer();

		/// ��ȣȭ 
		theNetPack2Game.Decrypt( msg, packetSize );

		/// ��� ĳ���� : ���ο� CsHeader 
		header = static_cast< SRpHeaderPtr > ( msg );

		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// ������ ���� ���� üũ 
		{
			RLG2( "Invalid Packet Header : %d -> size info is too big(%d)", header->theProtocol , header->theSize );
///			SEND_DEBUG_LOG(" MsgProcGS Invalid Packet Header : %d -> size info is too big(%d)", header->theProtocol , header->theSize );
			
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}

		/// sooree net remodel 
		if( packetSize != ( header->theSize + header->theVar2 ) )
		{
			/// invalid packet 
		
			return;
		}

#else
		/// ��� Ȯ�� 
		if ( size < sizeof( SRpHeader ) ) break;
		header = ( SRpHeader* ) msg;
		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// ������ ���� ���� üũ 
		{
			RLG2( "Invalid Packet Header : %d -> size info is too big(%d)", header->theProtocol , header->theSize );
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}
		
		/// ��� �����ߴ°�?
		if( size < header->theSize ) break;
#endif	
		
		//by simwoosung
        CheckProtocol((void *) msg, TRUE);
		
		switch( header->theProtocol )
		{
            //-----------------------------------------------------------------
			// Framework Related
			//-----------------------------------------------------------------
		case n_RpScKeepAliveCheck:
			{
				Send_RpCsKeepAliveCheck( (SRpScKeepAliveCheck*) msg );
			}
			break;			
		case n_RpScErrorFramework:
			Proc_RpScErrorFramework( (SRpScErrorFramework*) msg );				
			return;
			
		///case n_RpScWorldList:
		///			Proc_RpScWorldList( ( SRpScWorldList* ) msg );
		///	break;

		///case n_RpScChannelList:
			///Proc_RpScChannelList( ( SRpScChannelList* ) msg );
		///	break;	


		case n_RpScServerSetList:
			Proc_RpScServerSetList( (SRpScServerSetList*) msg );				break;

		case n_RpScHackShield:
			result = Proc_RpScHackShield( (SRpScHackShield*) msg );						break;

		case n_RpScGameGuard:
			result = Proc_RpScGameGuard( (SRpScGameGuard*) msg );						break;
		case n_RpScCharacterList:
			Proc_RpScCharacterList( (SRpScCharacterList*) msg );				break;
			
		case n_RpScPrepareLogin:
			result = Proc_RpScPrepareLogin( (SRpScPrepareLogin*) msg );			break;
			
		case n_RpScPCLogin:
			Proc_RpScPCLogin( (SRpScPCLogin*) msg );							break;

		case n_RpScChangeAccount:
			Proc_RpScChangeAccount( (SRpScChangeAccount*) msg );				break;
			//-----------------------------------------------------------------
			// Other Game Protocols : ���� ���������� ��ӹ��� Ŭ�������� ó�� 
			//-----------------------------------------------------------------
		default :
			Proc_RpScProtocols( msg );											break;
		}
		
		/// sooree net remodel 
		aSocketPtr->PopRecvQ( packetSize );

		///nProcessPCount++;
		
	} // for( size )

#ifdef _DEBUG	
	
	//if( nProcessPCount > 10 )
	{
		
		//float fDelTime = (float)(lDelTime * 0.001f);
		//PRINT_DLG_LOG( "ó�� ��Ŷ�� - %d ó���ð�  - %f��", nProcessPCount, 
		//	fDelTime );
	}

#endif
	
	// Proc_RpScPrepareLogin �ÿ� ������ ���� ���ؼ�...
	if( result == false )
	{
		CleanUpSocketAll();
	}
	
	return;
}

void	CNetworkMgr::Proc_RpScErrorFramework( SRpScErrorFramework* aMsg )
{
	RLG2( "S2C_ErrorFramework : code=%d str=%s", aMsg->theErrorCode, aMsg->theErrorStr );
}

void CNetworkMgr::Proc_RpScChangeAccount( SRpScChangeAccount* aMsg )
{
	//PigWing �� ������������ ����.  
	memset(theAccountName ,0,sizeof(theAccountName));
	memset(theChangeAccountName, 0, sizeof(theChangeAccountName));

	SAFE_STR_CPY( theAccountName, aMsg->theAccountID, 65 );	/// ����ؾ� �մϴ�.
	SAFE_STR_CPY( theChangeAccountName , aMsg->theChangedAccountID, 65 );
	
	if( n_Kr_Pigwing == g_RockClient.m_GameTypeInfo.theFrom  || 
		n_Kr_MyClub == g_RockClient.m_GameTypeInfo.theFrom )
	{
		memset(g_RockClient.m_UserSysID , 0 , sizeof(g_RockClient.m_UserSysID));
		SAFE_STR_CPY( g_RockClient.m_UserSysID, theChangeAccountName, 64 );	/// ����ؾ� �մϴ�.
	}
}

void	CNetworkMgr::Proc_RpScServerSetList( SRpScServerSetList* aMsg )
{
	char userName[128] = "";
	
	memset( theServerSetData , 0 , sizeof( SServerSetData ) * 64 );
	memset(g_RockClient.m_SysID , 0 , sizeof(g_RockClient.m_SysID));	
	
	thePCID = aMsg->theMsgHeader.thePCID;	/// ���⼭ �� ����ؾ� �Ѵ�. 
	SAFE_STR_CPY( theAccountName, aMsg->theAccountID, 65 );	/// ����ؾ� �մϴ�.
	SAFE_STR_CPY( g_RockClient.m_SysID , aMsg->theChollianSysID, 33 );


	
	if( g_RockClient.GetLanguage() == eLang_Korea ) 
	{
		if( g_RockClient.m_SysID != NULL && n_Kr_Chollian == g_RockClient.m_GameTypeInfo.theFrom )
		{
			memset(g_WebShopWnd.m_md5 , 0 , sizeof(g_WebShopWnd.m_md5));
			md5_encodeU( g_RockClient.m_SysID , g_WebShopWnd.m_md5 );
		}
		else if( (n_Kr_DreamLine == g_RockClient.m_GameTypeInfo.theFrom) ||
				(n_Kr_LuluGame == g_RockClient.m_GameTypeInfo.theFrom))
		{
			///-- �帲������ check������ ��ȣȭ�� �������̵� �����ش�.
			memset(g_WebShopWnd.m_md5 , 0 , sizeof(g_WebShopWnd.m_md5));			
			SAFE_STR_CPY( userName, g_RockClient.m_UserName , 33 );
			md5_encodeU( userName , g_WebShopWnd.m_md5 );
		}
	}

	if( n_JapanNetMarble == g_RockClient.m_GameTypeInfo.theFrom && 
		g_RockClient.GetLanguage() != eLang_Korea ) 
	{
		md5_encodeU( theAccountName , g_WebShopWnd.m_md5 );
		SAFE_STR_CPY( g_RockClient.m_UserName, theAccountName, 1024 );
	}

#ifdef  TAIWAN_VERSION
	memset( g_WebShopWnd.m_md5 , NULL , sizeof(g_WebShopWnd.m_md5) );
	memcpy( g_WebShopWnd.m_md5	, aMsg->theCheckSum , sizeof( aMsg->theCheckSum) );
#endif 

	theServerSetNum = aMsg->theServerSetNum;
	ServerListWnd.m_bRunServerListName = false;  
	memcpy( theServerSetData, aMsg->theServerSetList, sizeof( SServerSetData ) * theServerSetNum );
}

bool CNetworkMgr::Proc_RpScHackShield(SRpScHackShield * aMsg)
{
	//CRockClient::Proc_RpScHackShield(aMsg);
	int a= 0 ;

	return true;
}

bool CNetworkMgr::Proc_RpScGameGuard(SRpScGameGuard * aMsg)
{
	return true; 
}

///---------------------------------------------------------------------------
///-- Proc_RpScWorldList
///---------------------------------------------------------------------------
///-- �������� �����Ѵ�.
/*
void	CNetworkMgr::Proc_RpScWorldList( SRpScWorldList* aMsg )
{
	memset( theWorldData , 0 , sizeof( SWorldData ) * 64 );

	thePCID = aMsg->theMsgHeader.thePCID;	/// ���⼭ �� ����ؾ� �Ѵ�. 
	theWorldNum = aMsg->theWorldNum;
	memcpy( theWorldData, aMsg->theWorldList, sizeof( SWorldData ) * theWorldNum );	
}*/
///---------------------------------------------------------------------------
///-- Proc_RpScChannelList
///---------------------------------------------------------------------------
///-- ä���� �����Ѵ�.
/*
void	CNetworkMgr::Proc_RpScChannelList( SRpScChannelList * aMsg )
{
	memset( theChannelData , 0 , sizeof( SChannelData ) * 64 );

	thePCID = aMsg->theMsgHeader.thePCID;	/// ���⼭ �� ����ؾ� �Ѵ�. 
	theChannelNum = aMsg->theChannelNum;
	memcpy( theChannelData, aMsg->theChannelList, sizeof( SChannelData ) * theChannelNum );
}
*/
///---------------------------------------------------------------------------
///-- Proc_RpScCharacterList
///---------------------------------------------------------------------------
void	CNetworkMgr::Proc_RpScCharacterList( SRpScCharacterList* aMsg )
{
	theCharacterNum = aMsg->theCharacterNum;	
	

	memcpy( theCharacterList, aMsg->theCharacterList, sizeof( SRpdCharacterDataWithCore ) * theCharacterNum );
}

bool	CNetworkMgr::Proc_RpScPrepareLogin( SRpScPrepareLogin* aMsg )
{
	/// ���� �̰� ������ Ŭ���̾�Ʈ�� �ε� �� ���� ���� ������ ���� ���� �غ� �������ؾ� �Ѵ�.
	/// �غ� �Ϸ� ��, C2S_PCLogin ���� 
	return true;
}

void	CNetworkMgr::Proc_RpScPCLogin( SRpScPCLogin* aMsg )
{
	/// �α��� �Ϸ�!
}

/////////////////////////////////////////// * msg procedure * ///////////////////////////////////////////


BOOL	CNetworkMgr::SelectCharacter( int aListIndex, char* aCharacterName )
{
	SUPERASSERT( aListIndex >= 0 || aListIndex < theCharacterNum );
	
	if( lstrcmp( theCharacterList[aListIndex].theData.thePCName, aCharacterName) != 0 )
	{
		RLG3("%dth Name is Invalid : %s asked... but %s", aListIndex, aCharacterName,theCharacterList[aListIndex].theData.thePCName );
		return FALSE;
	}	
	
	Send_RpCsSelectCharacter( aCharacterName );
	
	theSelectCharIndex = aListIndex;
	
	return TRUE;
}

BOOL	CNetworkMgr::SelectCharacter( int aListIndex )
{
	ASSERT( aListIndex >= 0 || aListIndex < theCharacterNum );
	
	Send_RpCsSelectCharacter( theCharacterList[aListIndex].theData.thePCName );
	
	theSelectCharIndex = aListIndex;
	
	return TRUE;
}

/////////////////////////////////////////// * send protocol * ///////////////////////////////////////////

int		CNetworkMgr::SendMsg2LS( void* aMsgPtr, int aSize )
{
	//by simwoosung
    CheckProtocol((void *) aMsgPtr, FALSE);
	
	theLoginSocketPtr->PushSendQ( aMsgPtr, aSize );

	return( theLoginSocketPtr->NbSend() );
}

int		CNetworkMgr::SendMsg2GS( void* aMsgPtr, int aSize )
{
	//by simwoosung
    CheckProtocol((void *) aMsgPtr, FALSE);
	
	if( NULL == theGameSocketPtr )
	{
		return FALSE;
	}

	theGameSocketPtr->PushSendQ( aMsgPtr, aSize );

	return( theGameSocketPtr->NbSend() );
}

//-----------------------------------------------------------
// Framework Related
//-----------------------------------------------------------

void	CNetworkMgr::Send_RpCsKeepAliveCheck( SRpScKeepAliveCheck* aMsg )
{
	SRpCsKeepAliveCheck	msg_CsKeepAliveCheck;
	msg_CsKeepAliveCheck.theMsgHeader.thePCID = thePCID;
	msg_CsKeepAliveCheck.theServerAskTick = aMsg->theServerAskTick;
	msg_CsKeepAliveCheck.theClientTick = timeGetTime();	

	NetToGame( &msg_CsKeepAliveCheck, sizeof( msg_CsKeepAliveCheck ) );
}

void	CNetworkMgr::Send_RpCsAccountLogin( char* aAccountName, char* aPasswd, UINT uForm )
{
	  
	/// �ѱ� ���� �߿� ���� ����Ʈ�� ���ӿ��� ����Ʈ���� �� ���� �빮��ȭ���� �ʰ� ������.
	_strupr( aAccountName );
	strcpy( theAccountName, aAccountName );	/// ����ؾ� �մϴ�.

#ifdef C_GAME_GUARD
	g_RockClient.pNpgl->Send(aAccountName);
#endif 

	SRpCsAccountLogin msg_CsAccountLogin;
	 
	memset( msg_CsAccountLogin.theAccountID , 0 , sizeof(msg_CsAccountLogin.theAccountID) );
	memset( msg_CsAccountLogin.thePasswd    , 0 , sizeof(msg_CsAccountLogin.thePasswd) );

	memcpy( msg_CsAccountLogin.theAccountID , aAccountName , sizeof(msg_CsAccountLogin.theAccountID) );
	msg_CsAccountLogin.theAccountID[sizeof(msg_CsAccountLogin.theAccountID) - 1] = '\0';

	memcpy( msg_CsAccountLogin.thePasswd , aPasswd , sizeof(msg_CsAccountLogin.thePasswd) );
	msg_CsAccountLogin.thePasswd[sizeof(msg_CsAccountLogin.thePasswd) - 1] = '\0';

	int nVersion = GetGameVersion();
	msg_CsAccountLogin.theVersion = nVersion;
		
	if( !nVersion )
	{
		SEND_DEBUG_LOG("%s",msg_CsAccountLogin.theAccountID);
	}	

	msg_CsAccountLogin.theFrom = uForm;


#ifdef CHECK_GMCLIENT

	msg_CsAccountLogin.theUserType = 1;		
	#ifdef GM_CLIENT
		msg_CsAccountLogin.theUserType = 2;
	#endif

#endif
	
	NetToLogin( &msg_CsAccountLogin, sizeof( msg_CsAccountLogin ) );
	
	RLG3( "CsAccountLogin : %s %s %d", aAccountName, aPasswd, nVersion );	
}

///---------------------------------------------------------------------------
///-- Send_RpCsGameLogin
///---------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsGameLogin()
{
	OutputDebugString( "GameLogin\n" );
	
	SRpCsGameLogin msg_CsGameLogin;
	msg_CsGameLogin.theMsgHeader.thePCID = thePCID;
	
// �Ǳ����� �α��� �������� SysID ���ͼ�  ä�ηα��� �ҽ� theAccountName ���� SysID �γ��ش�.
// 
	SAFE_STR_CPY( msg_CsGameLogin.theAccountID, theAccountName, RP_MAX_ACCOUNT_ID );
	
	NetToGame( &msg_CsGameLogin, sizeof( msg_CsGameLogin ) );
	
	RLG2( "CsGameLogin : id=%d account=%s", thePCID, theAccountName );
}

void	CNetworkMgr::Send_RpCsSelectCharacter( char* aPCName )
{
	OutputDebugString( "SelectCharacter\n" );
	
	SRpCsSelectCharacter msg_CsSelectCharacter;
	msg_CsSelectCharacter.theMsgHeader.thePCID = thePCID;
	SAFE_STR_CPY( msg_CsSelectCharacter.thePCName, aPCName, RP_MAX_PC_NAME );
	
	NetToGame( &msg_CsSelectCharacter, sizeof( msg_CsSelectCharacter ) );
	
	RLG1( "CsSelectCharacter : %s", aPCName);
}

void	CNetworkMgr::Send_RpCsPCLogin()
{
	SRpCsPCLogin msg_CsPCLogin;
	msg_CsPCLogin.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msg_CsPCLogin, sizeof( msg_CsPCLogin ) );
	
	
	RLGS( "CsPCLogin");
}

void	CNetworkMgr::Send_RpCsPCLogout( int aLogoutType )
{
	SRpCsPCLogout msg_CsPCLogout;
	msg_CsPCLogout.theMsgHeader.thePCID = thePCID;
	msg_CsPCLogout.theLogoutType = aLogoutType;
	
	NetToGame( &msg_CsPCLogout, sizeof( msg_CsPCLogout ) );
		
	RLG1( "CsPCLogout : %d", aLogoutType );
}

//-----------------------------------------------------------
// Other Game Protocols
//-----------------------------------------------------------
void	CNetworkMgr::Send_RpCsStartMove( DWORD aMoveID, DWORD aDestiCell, BYTE aMoveSpeed )
{
	SRpCsStartMove msg_CsStartMove;
	msg_CsStartMove.theMsgHeader.thePCID = aMoveID;
	msg_CsStartMove.theDestiCell = aDestiCell;
	msg_CsStartMove.theMoveSpeed = aMoveSpeed;
	
	NetToGame( &msg_CsStartMove, sizeof( msg_CsStartMove ) );	
	
	if( thePCID == aMoveID )
	{
		SetWaitingSvrMsg();
	 
		//by simwoosung
		theActiveCsStartMoveFlag = TRUE;	
		theScWatingCumulateTime = 0.0f;

		RLGS( "theActiveCsStartMoveFlag = TRUE;" );
	}

	return;
}


void	CNetworkMgr::Send_RpCsMove( DWORD aMoveID, DWORD aCell, float aAngle )
{
	if( aMoveID == g_Pc.m_MainPC.lUnique )
	{
		DWORD PosX = aCell % g_Pc.m_cSize;
		DWORD PosY = aCell / g_Pc.m_cSize;
		
		///--PRINT_DLG_LOG("Send_RpCsMove DestCell = %u PosX = %u PosY = %u", aCell, PosX, PosY );
	}
	
	SRpCsMove msg_CsMove;

	msg_CsMove.theMsgHeader.thePCID = aMoveID;
	msg_CsMove.theCell = aCell;
	msg_CsMove.theAngle = aAngle;

	NetToGame( &msg_CsMove, sizeof( msg_CsMove ) );


}

///--JAPAN_BUG_MODIFY
void	CNetworkMgr::Send_RpCsStop( DWORD aCell, float aAngle, DWORD dPCID)
{
	SRpCsStop msg_CsStop;
	
	if( dPCID )
	{
		msg_CsStop.theMsgHeader.thePCID = dPCID;
	}
	else
	{
		msg_CsStop.theMsgHeader.thePCID = thePCID;
	}

	msg_CsStop.theCell = aCell;
	msg_CsStop.theAngle = aAngle;
	
	NetToGame( &msg_CsStop, sizeof( msg_CsStop ) );
	
	
	//	SetWaitingSvrMsg();
}


void	CNetworkMgr::Send_RpCsAttack( DWORD aAttackID, DWORD aAttackedID, BYTE aAttackedType, WORD aMotion, BYTE aComboType, BYTE aComboCount, BYTE aComboStatus, BYTE aComboSeq )
{
	SRpCsAttack msg_CsAttack;
	msg_CsAttack.theMsgHeader.thePCID = aAttackID;	
	msg_CsAttack.theAttackedID = aAttackedID;
	msg_CsAttack.theAttackedType = aAttackedType;
	msg_CsAttack.theMotion = aMotion;
	msg_CsAttack.theComboType = aComboType;				// �޺� Ÿ�� - �Ϲݰ��� : 0  ex> c_ComboXKey1, ....
	msg_CsAttack.theComboCount = aComboCount;			// �޺� Ƚ�� - �Ϲݰ��� : 0
	msg_CsAttack.theComboStatus	= aComboStatus;			// �޺� ���� - �ϼ�, ���� ��...
	msg_CsAttack.theComboSeq = aComboSeq;
	
	NetToGame( &msg_CsAttack, sizeof( msg_CsAttack ) );
	
	// �޺��� �ƴ� ��Ÿ�� �����ؼ� �����°��� ���� �÷��̷� ����!!! 
	if( aComboCount == 0 )
	{
		g_RockClient.m_PickAttack_DelayTime = g_nowTime;
	}	
	else
	{
		if( g_Pc.m_MainPC.lUnique == aAttackID )
		{
			SPcDataParam *PcParam = nRui->GetPcParam();

			if( PcParam->MainClass == Archer || 
				PcParam->MainClass == Sniper )
			{
				g_RockClient.m_PickAttack_DelayTime = g_nowTime;
			}
		}
	}

//	RLG3( "CsAttack() theAttackedID = %u, theAttackedType = %d, theMotion = %u", aAttackedID, aAttackedType, aMotion );
}

void CNetworkMgr::Send_RpCsRequestWarp( int aGateIndex )
{
	SRpCsRequestWarp msg_CsRequestWarp;
	msg_CsRequestWarp.theMsgHeader.thePCID = thePCID;
	
	msg_CsRequestWarp.theGateIndex = aGateIndex;
	
	NetToGame( &msg_CsRequestWarp, sizeof( msg_CsRequestWarp ) );
		
	return;
}

void CNetworkMgr::Send_RpCsReadyWarp()
{
	SRpCsReadyWarp msg_CsReadyWarp;
	msg_CsReadyWarp.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msg_CsReadyWarp, sizeof( msg_CsReadyWarp ) );
		
	return;
}

// ��ų ��� ��û
void CNetworkMgr::Send_RpCsSkill( DWORD aSkillID, DWORD aTarget, BYTE aTargetType, WORD aSkillCode )
{
	// ���� ������ ��ų ����
	if( aSkillCode == c_SkillCode_UserSellStore )
	{
		if( g_Pc.GetPlayer()->m_CharAlpha.GetState() == CHAR_HIDE )
		{
			SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_USERSTORE_SELL_NOT ) );

			return;
		}

	}	
		
	///--PRINT_DLG_LOG("��ų ���� %d", aSkillCode);
	
	SRpCsSkill msg_CsSkill;
	msg_CsSkill.theMsgHeader.thePCID = aSkillID;
	
	msg_CsSkill.theTarget = aTarget;			// target�� ������ �ɼ��� �ְ�.. ĳ���Ͱ� �ɼ��� �ִ�.
	msg_CsSkill.theTargetType = aTargetType;	// n_PC, n_NPC, n_Cell
	msg_CsSkill.theSkillCode = aSkillCode;

	// ��ų ����� �ٷ� �Ϲ� ������ �����ؼ� �����°��� ���� �÷��̷� ����!!! 
	g_RockClient.m_PickAttack_DelayTime = g_nowTime;

	NetToGame( &msg_CsSkill, sizeof( msg_CsSkill ) );
	return;
} 

void CNetworkMgr::Send_RpCsSpelling( BYTE aChantCount )
{
	SRpCsSpelling msg_CsSpelling;
	msg_CsSpelling.theMsgHeader.thePCID = thePCID;
	
	msg_CsSpelling.theSpellingCount = aChantCount;			// ���� ��â ��
	
	NetToGame( &msg_CsSpelling, sizeof( msg_CsSpelling ) );
	
	
	return;
}

// ��ó �غ� ���� : target -> Cell or PC or NPC | n_PC, n_NPC, n_Cell
void CNetworkMgr::Send_RpCsArcherReady(DWORD aTarget,BYTE aTargetType)
{
	//	RLGS("Send_RpCsArcherReady\n");
	
	SRpCsArcherReady msg_CSArcherReady;
	
	msg_CSArcherReady.theMsgHeader.thePCID = thePCID;
	
	msg_CSArcherReady.theTarget			   = aTarget;
	
	msg_CSArcherReady.theTargetType		   = aTargetType;
	
	NetToGame( &msg_CSArcherReady, sizeof( msg_CSArcherReady ) );
	
    return;
}

//-----------------------------------------------------------------------------
////////////////////// * Chatting protocol * //////////////////////////////////
//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsSay( TCHAR* aSayMessage )
{
	//�̰� ����ƽ���� ��ƾ��� - ����޽��������� ���������� SRpCsSay ����������
	//������� ���� ä��ó�� �� �����͸� ó���ؾ��� ���
	static SRpCsSay	msgCsSay;
	ZeroMemory(&msgCsSay.theMessage , sizeof(msgCsSay.theMessage));

	 msgCsSay.theMsgHeader.thePCID = thePCID;
 
#ifdef UNICODE_ROCK
	BYTE	bMessageSize = (Rstrlen(aSayMessage)<<1)+2;
#else
	BYTE	bMessageSize = Rstrlen(aSayMessage)+1; 
#endif

	msgCsSay.theMsgHeader.theSize = sizeof( _SRpCsSay ) - sizeof( msgCsSay.theMessage );
	msgCsSay.theMsgHeader.theSize += bMessageSize;
	msgCsSay.theMessageSize = bMessageSize;

	SAFE_STR_CPY( msgCsSay.theMessage, aSayMessage, RP_MAX_CHAT_SIZE );
	NetToGame( &msgCsSay, msgCsSay.theMsgHeader.theSize );

}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsWhisper( TCHAR* aReceiver, TCHAR* aSayMessage )
{
	//�̰� ����ƽ���� ��ƾ���
	static SRpCsWhisper	msgCsWhisper;
	
	msgCsWhisper.theMsgHeader.thePCID = thePCID;
	ZeroMemory(msgCsWhisper.theMessage , sizeof(msgCsWhisper.theMessage));

#ifdef UNICODE_ROCK
	BYTE	bMessageSize = (Rstrlen(aSayMessage)<<1)+2 ;
#else
	BYTE	bMessageSize = Rstrlen(aSayMessage)+1 ; 
#endif
	
	msgCsWhisper.theMsgHeader.theSize = sizeof( _SRpCsWhisper ) - sizeof( msgCsWhisper.theMessage );
	msgCsWhisper.theMsgHeader.theSize += bMessageSize;
	msgCsWhisper.theMessageSize = bMessageSize;

#ifdef WHISPER_GM

//GM�̶� ���ڰ� ������...
	if( g_RockClient.m_SlangFilter.IsFilterGmName(aReceiver) )
	{
		msgCsWhisper.theUsertype = n_GM_Lv3;//n_GM_Lv3;	
	}
	else
	{
		msgCsWhisper.theUsertype = n_Default;
	}

	#ifdef ROCKCLIENT_DEV
	msgCsWhisper.theUsertype = n_GM_Lv3;//n_GM_Lv3;	
	#endif

#endif
		
	SAFE_STR_CPY( msgCsWhisper.theMessage, aSayMessage, RP_MAX_CHAT_SIZE );
	SAFE_STR_CPY( msgCsWhisper.theReceiver, aReceiver, RP_MAX_PC_NAME );
	SAFE_STR_CPY( theWhisperMessage, aSayMessage, MAX_SIZE_256 );

	NetToGame( &msgCsWhisper, msgCsWhisper.theMsgHeader.theSize );

}

//-----------------------------------------------------------------------------
//////////////////////// * Item protocol * ////////////////////////////////////
//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsPickup( DWORD aItemUID )
{
	SRpCsPickup msgCsPickup;
	msgCsPickup.theMsgHeader.thePCID = thePCID;
	
	msgCsPickup.theItemID	= aItemUID;
	
	NetToGame( &msgCsPickup, sizeof( msgCsPickup ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsDrop( DWORD aItemUID, BYTE aBundle, BYTE aItemTab )
{

	///--PRINT_DLG_LOG("CsDrop aItemUID %d aBundle %d aItemTab %d ",aItemUID , aBundle , aItemTab );

	if( aBundle == 0 )
	{
		return;
	}
	
	SRpCsDrop msgCsDrop;
	msgCsDrop.theMsgHeader.thePCID = thePCID;
	
	msgCsDrop.theItemID		= aItemUID;
	msgCsDrop.theBundle		= aBundle;
	msgCsDrop.theItemTab	= aItemTab;
	
	NetToGame( &msgCsDrop, sizeof( msgCsDrop ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsDropEld( DWORD aEld )
{
	if( aEld == 0 )
	{
		return;
	}
	
	SRpCsDropEld msgCsDropEld;
	msgCsDropEld.theMsgHeader.thePCID = thePCID;
	
	msgCsDropEld.theEld = aEld;
	
	NetToGame( &msgCsDropEld, sizeof( msgCsDropEld ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsUse( DWORD aItemUID, BYTE aColonyPos )
{
	SRpCsUse msgCsUse;
	msgCsUse.theMsgHeader.thePCID = thePCID;
	
	msgCsUse.theItemID = aItemUID;

#ifdef COLONY_UPDATE
	msgCsUse.theColonyPos = aColonyPos;
#endif	
	
	NetToGame( &msgCsUse, sizeof( msgCsUse ) );	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsEquip( DWORD aItemUID, BYTE aPosition )
{
	///--PRINT_DLG_LOG("CsEquip aItemUID %d aPosition %d " ,aItemUID , aPosition );
	SRpCsEquip msgCsEquip;
	msgCsEquip.theMsgHeader.thePCID = thePCID;
	
	msgCsEquip.theItemID	= aItemUID;
	msgCsEquip.thePosition	= aPosition;
	
	
	NetToGame( &msgCsEquip, sizeof( msgCsEquip ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsTakeoff( DWORD aItemUID, BYTE aPosition )
{
	///--PRINT_DLG_LOG("CsTakeoff aItemUID %d aPosition %d " ,aItemUID , aPosition );

	SRpCsTakeoff msgCsTakeoff;
	msgCsTakeoff.theMsgHeader.thePCID = thePCID;
	
	msgCsTakeoff.theItemID		= aItemUID;
	msgCsTakeoff.thePosition	= aPosition;
	
	
	NetToGame( &msgCsTakeoff, sizeof( msgCsTakeoff ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsBuy( DWORD aNPCID, WORD aItemCode, BYTE aBundle, BYTE aPosition )
{
	SRpCsBuy msgCsBuy;
	msgCsBuy.theMsgHeader.thePCID = thePCID;
	
	msgCsBuy.theNPCID		= aNPCID;// UID
	msgCsBuy.theItemCode	= aItemCode;
	msgCsBuy.theBundle		= aBundle;
	msgCsBuy.thePosition	= aPosition;
	
	//--PRINT_DLG_LOG("CsBuy aNPCID %d aItemCode %d aBundle %d aPosition %d " ,aNPCID , aItemCode ,aBundle , aPosition );
	NetToGame( &msgCsBuy, sizeof( msgCsBuy ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsSell( DWORD aNPCID, DWORD aItemUID, BYTE aBundle, BYTE aItemTab )
{
	SRpCsSell msgCsSell;
	msgCsSell.theMsgHeader.thePCID = thePCID;
	
	msgCsSell.theNPCID = aNPCID;
	msgCsSell.theItemID	= aItemUID;
	msgCsSell.theBundle	= aBundle;
	msgCsSell.theItemTab= aItemTab;
	
	
	NetToGame( &msgCsSell, sizeof( msgCsSell ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsCombine( DWORD aItemUID1, DWORD aItemUID2, BYTE aItemTab )
{
	nRui->m_KeyLock.Inventory.SetLock(g_nowTime);
	SRpCsCombine msgCsCombine;
	msgCsCombine.theMsgHeader.thePCID = thePCID;
	
	msgCsCombine.theItemID1 = aItemUID1;
	msgCsCombine.theItemID2 = aItemUID2;
	msgCsCombine.theItemTab = aItemTab;
	
	
	NetToGame( &msgCsCombine, sizeof( msgCsCombine ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsChangeSlot( DWORD aItemUID1, DWORD aItemUID2, BYTE aSlot, BYTE aItemTab )
{
	//PRINT_DLG_LOG("CsChangeSlot aItemUID1 %d , aItemUID2 %d, aSlot %d aItemTab %d" , aItemUID1 ,aItemUID2 , aSlot , aItemTab);
	nRui->m_KeyLock.Inventory.SetLock(g_nowTime);
	
	SRpCsChangeSlot msgCsChangeSlot;
	msgCsChangeSlot.theMsgHeader.thePCID = thePCID;
	
	msgCsChangeSlot.theItemID1	= aItemUID1;
	msgCsChangeSlot.theItemID2	= aItemUID2;
	msgCsChangeSlot.theSlot		= aSlot;
	msgCsChangeSlot.theItemTab	= aItemTab;
	
	
	NetToGame( &msgCsChangeSlot, sizeof( msgCsChangeSlot ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsLinkQuickSlot( SRpdQuickSlot aQslot )
{
	
	//nRui->m_KeyLock.QuickSlotKey.bflag = true;
	//nRui->m_KeyLock.QuickSlotKey.dTime = g_nowTime;
	nRui->m_KeyLock.QuickSlotKey.SetLock(g_nowTime);
	SRpCsLinkQuickSlot msgCsLinkQuickSlot;
	msgCsLinkQuickSlot.theMsgHeader.thePCID = thePCID;
	
//	msgCsLinkQuickSlot.theID		= aQslot.theID;
//	msgCsLinkQuickSlot.theType		= aQslot.theType;
//	msgCsLinkQuickSlot.thePosition	= aQslot.thePosition;
	
	msgCsLinkQuickSlot.theQuickSlot = aQslot;	
	
	NetToGame( &msgCsLinkQuickSlot, sizeof( msgCsLinkQuickSlot ) );
	
}

//-----------------------------------------------------------------------------
void	CNetworkMgr::Send_RpCsUnLinkQuickSlot( BYTE aPosition )
{
	SRpCsUnlinkQuickSlot msgCsUnlinkQuickSlot;
	msgCsUnlinkQuickSlot.theMsgHeader.thePCID = thePCID;
	
	msgCsUnlinkQuickSlot.thePosition = aPosition;
	
	NetToGame( &msgCsUnlinkQuickSlot, sizeof( msgCsUnlinkQuickSlot ) );
	
}


void CNetworkMgr::Send_RpCsAlive(DWORD thePayItem)
{
	SRpCsAlive msgCsAlive;
	msgCsAlive.theMsgHeader.thePCID = thePCID;

	msgCsAlive.thePayItem = thePayItem;

	NetToGame( &msgCsAlive, sizeof( msgCsAlive ) );	
}


void CNetworkMgr::Send_RpCsPoint( BYTE aPower, BYTE aVital, BYTE aSympathy, BYTE aInt, BYTE aStamina, BYTE aDex )
{
	SRpCsPoint msgCsPoint;
	msgCsPoint.theMsgHeader.thePCID = thePCID;
	
	msgCsPoint.thePower		= aPower;		// �Ŀ�
	msgCsPoint.theVital		= aVital;		// ü��	
	msgCsPoint.theSympathy	= aSympathy;	// ����
	msgCsPoint.theInt		= aInt;			// ����	
	msgCsPoint.theStamina	= aStamina;		// ���¹̳� 	
	msgCsPoint.theDex		= aDex;			// ��ø 		
	
	NetToGame( &msgCsPoint, sizeof( msgCsPoint ) );
	
}


void CNetworkMgr::Send_RpCsScriptStart( DWORD NPCID, WORD ScriptCode )
{
	SRpCsScriptStart msgCsScriptStart;
	msgCsScriptStart.theMsgHeader.thePCID = thePCID;
	
	msgCsScriptStart.theNPCID = NPCID;
	msgCsScriptStart.theScriptCode = ScriptCode;
	
	NetToGame( &msgCsScriptStart, sizeof( msgCsScriptStart ) );
	
}

void CNetworkMgr::Send_RpCsScriptEnd( WORD aNpcCode )
{
	SRpCsScriptEnd msgCsScriptEnd;
	msgCsScriptEnd.theMsgHeader.thePCID = thePCID;
	
	msgCsScriptEnd.theScriptCode = aNpcCode;		
	
	NetToGame( &msgCsScriptEnd, sizeof( msgCsScriptEnd ) );
	
}


void CNetworkMgr::Send_RpCsAnswerScript( BYTE aAnswer )
{
	SRpCsAnswerScript msgCsAnswerScript;
	msgCsAnswerScript.theMsgHeader.thePCID = thePCID;
	
	msgCsAnswerScript.theAnswer = aAnswer;		
	
	NetToGame( &msgCsAnswerScript, sizeof( msgCsAnswerScript ) );
	
}

#ifdef DECO_RENEWAL_MJH
void CNetworkMgr::Send_RpCsCreateCharacter(int SelectNum,DWORD Class ,char *Name,BYTE Citizen,BYTE Gender,DWORD Hair,DWORD Face)
#else
void CNetworkMgr::Send_RpCsCreateCharacter(int SelectNum,char *Name,BYTE Citizen,BYTE Gender,DWORD Hair,DWORD Face) 
#endif // DECO_RENEWAL_MJH

{
	SRpCsCreateCharacter msgCsCreateCharacter;
	
	msgCsCreateCharacter.theMsgHeader.thePCID    = thePCID;
	msgCsCreateCharacter.theAttribute.theCitizen = Citizen;
	msgCsCreateCharacter.theAttribute.theGender  = Gender;
	msgCsCreateCharacter.theAttribute.theHair    = Hair;
	msgCsCreateCharacter.theAttribute.theFace    = Face;
#ifdef DECO_RENEWAL_MJH //��������
	msgCsCreateCharacter.theMainClass    = Class;
#endif //DECO_RENEWAL_MJH
	
	memset( msgCsCreateCharacter.thePCName , 0    , RP_MAX_PC_NAME );
	SAFE_STR_CPY(msgCsCreateCharacter.thePCName , Name ,  RP_MAX_PC_NAME);
	msgCsCreateCharacter.theCore = theCharacterList[ SelectNum ].theCore;
	
	NetToGame( &msgCsCreateCharacter, sizeof( msgCsCreateCharacter ) );
	
}

void CNetworkMgr::Send_RpCsDeleteCharacter(char *Name)
{
	SRpCsDeleteCharacter msg;
	
	msg.theMsgHeader.thePCID = thePCID;
	Rsprintf( RWCHAR(msg.thePCName) , _RT("%s") , RWCHAR(Name) );
	
	NetToGame( &msg, sizeof( msg ) );
	
}

/*

  // ĳ���� ���� �����Ⱓ���� ���� ��� ��û �޽���
  typedef struct _SRpCsDeleteCancel
  {
  _SRpCsDeleteCancel()
  {
		theMsgHeader.theSize = sizeof( _SRpCsDeleteCancel );
		theMsgHeader.theProtocolType = 0;
		theMsgHeader.theProtocol = n_RpCsDeleteCancel;
		theMsgHeader.thePCID = 0;
		}
		
		  SRpHeader		theMsgHeader;
		  char			thePCName[RP_MAX_PC_NAME];
		  }SRpCsDeleteCancel, * SRpCsDeleteCancelPtr;
		  
*/



void CNetworkMgr::Send_RpCsHotKey( BYTE aKeyType, WORD aSkillCode )
{
	SRpCsHotKey msgCsHotKey;
	msgCsHotKey.theMsgHeader.thePCID = thePCID;
	
	msgCsHotKey.theKeyType = aKeyType;		// 0: Ctrl, 1: Space
	msgCsHotKey.theSkillCode = aSkillCode;
	
	NetToGame( &msgCsHotKey, sizeof( msgCsHotKey ) );
	
}



void CNetworkMgr::Send_RpCsReqCharInfo( DWORD aChrID, BYTE aChrType )
{
	SRpCsReqCharInfo msgCsReqCharInfo;
	msgCsReqCharInfo.theMsgHeader.thePCID = thePCID;
	
	msgCsReqCharInfo.theID = aChrID;		
	msgCsReqCharInfo.theType = aChrType;	// n_PC, n_NPC
	
	NetToGame( &msgCsReqCharInfo, sizeof( msgCsReqCharInfo ) );
	
}


// .., ��� ��..  ���ӽ�ų ���ᰡ ������� ���� ��ų. Ŭ���̾�Ʈ�� ���Ḧ ��û��.
void CNetworkMgr::Send_RpCsKeepUpSkillEnd( WORD aSkillCode )
{
	SRpCsKeepUpSkillEnd msgCsKeepUpSkillEnd;
	msgCsKeepUpSkillEnd.theMsgHeader.thePCID = thePCID;
	
	msgCsKeepUpSkillEnd.theSkillCode = aSkillCode;		 // ��ų�ڵ� USHRT_MAX : ����� ��� ��ų ���� theEffectType ����
	
	NetToGame( &msgCsKeepUpSkillEnd, sizeof( msgCsKeepUpSkillEnd ) );
	
}

// ����Ʈ ���� ��������
void CNetworkMgr::Send_RpCsGiveupQuest( WORD aQuestCode )
{
	SRpCsGiveupQuest msgCsGiveUpQuest;
	msgCsGiveUpQuest.theMsgHeader.thePCID = thePCID;
	
	msgCsGiveUpQuest.theQuestCode = aQuestCode;
	
	NetToGame( &msgCsGiveUpQuest, sizeof( msgCsGiveUpQuest ) );
	
}

void CNetworkMgr::Send_RpCsGMCmd( DWORD aCommand, int* aNParamList, char* aSParam )
{
	///--JAPAN_BUG_MODIFY
#ifdef	GM_CLIENT	
	SRpCsGMCmd msgCsGMCmd; 
	
	ZeroMemory(&msgCsGMCmd.theSParam , sizeof(msgCsGMCmd.theSParam));

	msgCsGMCmd.theMsgHeader.thePCID = thePCID;
	msgCsGMCmd.theCommand = aCommand;
	for( int i = 0; i < 4; ++i )
	{
		msgCsGMCmd.theNParam[i] = aNParamList[i];
	}
	
	memcpy(msgCsGMCmd.theSParam, aSParam, sizeof(RTCHAR)*63 );

//	strncpy( msgCsGMCmd.theSParam, aSParam, 63 );
//	msgCsGMCmd.theSParam[63] = '\0';
	
	NetToGame( &msgCsGMCmd, sizeof( msgCsGMCmd ) );
#endif

}


void CNetworkMgr::Send_RpCsGetSkill( WORD aSkillCode )
{
	SRpCsGetSkill msgCsGetSkill;
	msgCsGetSkill.theMsgHeader.thePCID = thePCID;
	
	msgCsGetSkill.theSkillCode = aSkillCode;
	
	NetToGame( &msgCsGetSkill, sizeof( msgCsGetSkill ) );
}


//-----------------------------------------------------------------------------
///////////////////////// * send protocol * ///////////////////////////////////
//-----------------------------------------------------------------------------
void CNetworkMgr::SetPlayerMessage( TCHAR* str )
{
}

//-----------------------------------------------------------------------------
void CNetworkMgr::SetNonPlayerMessage( DWORD pcid, TCHAR* str )
{
}

//-----------------------------------------------------------------------------

//by simwoosung - ��������, �α׿���, ��ȯ �ϱ����� �����̵����� ������ �˷���
void CNetworkMgr::Send_RpCs10SecDelay( BYTE BDelayReason )
{
	SRpCs10SecDelay msgCs10SecDelay;
	msgCs10SecDelay.theMsgHeader.thePCID = thePCID;
	
    msgCs10SecDelay.theDelayReason = BDelayReason;
	
	NetToGame( &msgCs10SecDelay, sizeof( msgCs10SecDelay ) );
}

//-----------------------------------------------------------
// Party Protocols
//-----------------------------------------------------------
void CNetworkMgr::Send_RpCsPartyAskJoin( TCHAR * aPCName, BYTE PartyType )
{
    TCHAR tstrMessage[256] = _T("");
	Rsprintf( RWCHAR(tstrMessage), G_STRING_CHAR( IDS_SYSPR_INVITED_USER ) , RWCHAR(aPCName) );
	ChatMessagePrint(tstrMessage , D3DCOLOR_XRGB( 0, 215, 5 ), TRUE);	
	
	SRpCsPartyAskJoin msgCsPartyAskJoin;
	msgCsPartyAskJoin.theMsgHeader.thePCID = thePCID;
	
	SAFE_STR_CPY( msgCsPartyAskJoin.thePCName, aPCName, RP_MAX_PC_NAME );

	msgCsPartyAskJoin.thePartyType = PartyType;
	
	NetToGame( &msgCsPartyAskJoin, sizeof( msgCsPartyAskJoin ) );	
}

void CNetworkMgr::Send_RpCsPartyAnsJoin( TCHAR * aLeaderName, BYTE aAnswer)
{
    SRpCsPartyAnsJoin msgCsPartyAnsJoin;
	msgCsPartyAnsJoin.theMsgHeader.thePCID = thePCID;
	
    SAFE_STR_CPY( msgCsPartyAnsJoin.theLeaderName, aLeaderName, RP_MAX_PC_NAME );
	msgCsPartyAnsJoin.theAnswer = aAnswer;
	
	NetToGame( &msgCsPartyAnsJoin, sizeof( msgCsPartyAnsJoin ) );
}

void CNetworkMgr::Send_RpCsPartyLeave( TCHAR * aPCName )
{
    SRpCsPartyLeave  msgCsPartyLeave;
	msgCsPartyLeave.theMsgHeader.thePCID = thePCID;
	
	SAFE_STR_CPY( msgCsPartyLeave.thePCName, aPCName, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsPartyLeave, sizeof( msgCsPartyLeave ) );
}

void CNetworkMgr::Send_RpCsPartyChangeName( TCHAR * aPartyName)
{
    SRpCsPartyChangeName msgCsPartyChangeName;
    msgCsPartyChangeName.theMsgHeader.thePCID = thePCID;
	
    SAFE_STR_CPY( msgCsPartyChangeName.thePartyName, aPartyName, 21 );
	
    NetToGame( &msgCsPartyChangeName, sizeof( msgCsPartyChangeName ) );
}

void CNetworkMgr::Send_RpCsPartyChat(TCHAR * aString)
{ 
	static SRpCsPartyChat msgCsPartyChat;
	msgCsPartyChat.theMsgHeader.thePCID = thePCID;
	ZeroMemory(msgCsPartyChat.theString , sizeof(msgCsPartyChat.theString));

#ifdef UNICODE_ROCK
	BYTE	bMessageSize = (Rstrlen(aString)<<1)+2 ;
#else
	BYTE	bMessageSize = Rstrlen(aString)+1; 
#endif
	
	msgCsPartyChat.theMsgHeader.theSize = sizeof( _SRpCsPartyChat ) - sizeof( msgCsPartyChat.theString );
	msgCsPartyChat.theMsgHeader.theSize += bMessageSize;
	msgCsPartyChat.theStringSize = bMessageSize;
	SAFE_STR_CPY( msgCsPartyChat.theString, aString, RP_MAX_CHAT_SIZE );
	NetToGame( &msgCsPartyChat, msgCsPartyChat.theMsgHeader.theSize );


}

void CNetworkMgr::Send_RpCsChangeLeader(TCHAR * aGiveLeader)
{
	SRpCsPartyChangeLeader msgCsPartyChangeLeader;
	msgCsPartyChangeLeader.theMsgHeader.thePCID = thePCID;
	
	SAFE_STR_CPY( msgCsPartyChangeLeader.theNewLeaderName, aGiveLeader, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsPartyChangeLeader, sizeof( msgCsPartyChangeLeader ) );
}

void CNetworkMgr::Send_RpCsPartyItemMgr(BYTE aHowRoot)
{
	SRpCsPartyItemMgr msgCsPartyItemMgr;
	msgCsPartyItemMgr.theMsgHeader.thePCID = thePCID;
	
	msgCsPartyItemMgr.theItemMgr = aHowRoot;
	
	NetToGame( &msgCsPartyItemMgr, sizeof( msgCsPartyItemMgr ) );
}

void CNetworkMgr::Send_RpCsPartyGiveGage(DWORD aPCID, BYTE aGage)
{
	SRpCsPartyGiveGage msgCsPartyGiveGage;
	msgCsPartyGiveGage.theMsgHeader.thePCID = thePCID;
	
	msgCsPartyGiveGage.thePCID = aPCID;
	msgCsPartyGiveGage.theGage = aGage;
	
	NetToGame( &msgCsPartyGiveGage, sizeof( msgCsPartyGiveGage ) );
}

void CNetworkMgr::Send_RpCsDirection( float aAngle )
{
	SRpCsDirection msgCsDirection;
	msgCsDirection.theMsgHeader.thePCID = thePCID;
	msgCsDirection.theAngle = aAngle;

//	RLG1( "Send_RpCsDirection : aAngle = %3.3f", aAngle );			
	
	NetToGame( &msgCsDirection, sizeof( msgCsDirection ) );
	
	return;
}
//============================================================================================================
//============================================================================================================
// ��ȯ �ý��� ���� �������� 
//============================================================================================================
//============================================================================================================
// ��ȯ ��� �ѱ�, ���� 
void CNetworkMgr::Send_RpCsExchange( BYTE aIsOpen ) 
{
	SRpCsExchange msgCsExchange;
	msgCsExchange.theMsgHeader.thePCID = thePCID;
	msgCsExchange.theExchange = aIsOpen;
	
	NetToGame( &msgCsExchange, sizeof( msgCsExchange ) );
	
	return;
}

// A�� ��ȯ ��û�� �޽��� 
void CNetworkMgr::Send_RpCsAskExchange( DWORD aPCID ) 
{
	SRpCsAskExchange msgCsAskExchange;
	msgCsAskExchange.theMsgHeader.thePCID = thePCID;
	msgCsAskExchange.thePCID = aPCID;		// ��ȯ�� �ϰ��� �ϴ� ���ID
	
	NetToGame( &msgCsAskExchange, sizeof( msgCsAskExchange ) );
	
	return;
}


// B�� ���� �޽���
void CNetworkMgr::Send_RpCsAnsExchange( BYTE aAnswer ) 
{
	SRpCsAnsExchange msgCsAnsExchange;
	msgCsAnsExchange.theMsgHeader.thePCID = thePCID;
	msgCsAnsExchange.theAnswer = aAnswer;	
	
	NetToGame( &msgCsAnsExchange, sizeof( msgCsAnsExchange ) );
	
	return;
}

//struct _SExItem
//{
//	DWORD	theItemUID;
//	DWORD	theBundle;
//};
// A,B�� �ø��� �� ��� ������ ����Ʈ
void CNetworkMgr::Send_RpCsSuggestExItem( DWORD aEld, BYTE aItemNum, _SExItem aItemList[10] ) 
{
	SRpCsSuggestExItem msgCsSuggestExItem;   
	msgCsSuggestExItem.theMsgHeader.thePCID = thePCID;

	int protocol_size = 0;
	protocol_size = sizeof( SRpHeader ) + sizeof( DWORD ) + sizeof( BYTE ) + ( sizeof( _SExItem ) * aItemNum );
	
	msgCsSuggestExItem.theMsgHeader.theSize = protocol_size;
		
	msgCsSuggestExItem.theEld = aEld;
	msgCsSuggestExItem.theItemNum = aItemNum;
	
	int list_num = 0;
	int i = 0 , j = 0 ; 
	for( i = 0; i < 10; ++i )
	{
		if( aItemList[i].theItemUID )
		{
			msgCsSuggestExItem.theItemList[list_num] = aItemList[i];
			++list_num;
		}	   
	} 
	    
	DWORD	TempdItemUID = 0 ; 
	int Num = 0 ; 

	//������ ���������� �ٽ��ѹ� �˻��Ѵ�..
	for(i = 0 ; i < list_num ; ++i)
	{
		if(msgCsSuggestExItem.theItemList[i].theItemUID)		
		{
			TempdItemUID = msgCsSuggestExItem.theItemList[i].theItemUID;
			Num = 0;
		
			for( j = 0;  j <list_num ; ++j)
			{
				//2���̻��̸� ������..
				if( TempdItemUID == msgCsSuggestExItem.theItemList[j].theItemUID)
				{
					++Num;
				}
				
			}
			if( Num >= 2 )
			{
				SEND_DEBUG_LOG("CsSuggestExItem Item UID HackUser");
				return;
			}

		}

	}
	




	
//	NetToGame( &msgCsSuggestExItem, sizeof( msgCsSuggestExItem ) );
	NetToGame( &msgCsSuggestExItem, protocol_size );
	
	return;
}

// ��ȯ�¶� ��ư Ŭ���� ����
void CNetworkMgr::Send_RpCsAgreeExchange() 
{
	SRpCsAgreeExchange msgCsAgreeExchange;
	msgCsAgreeExchange.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msgCsAgreeExchange, sizeof( msgCsAgreeExchange ) );
	
	return;
}

// ��ȯ��� ��ư Ŭ���� ����
void CNetworkMgr::Send_RpCsCancelExchange() 
{
	SRpCsCancelExchange msgCsCancelExchange;
	msgCsCancelExchange.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msgCsCancelExchange, sizeof( msgCsCancelExchange ) );
	
	return;
}

//============================================================================================================
//============================================================================================================
// ���� �ý��� ���� �������� 
//============================================================================================================
//============================================================================================================
// ����â�� ������ ������ ���� (DB�� �����Ѵ�.)
void CNetworkMgr::Send_RpCsCloseBank() 
{
	SRpCsCloseBank msgCsCloseBank;
	msgCsCloseBank.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msgCsCloseBank, sizeof( msgCsCloseBank ) );
	
	return;
}

// �������� �����Ѵ�. 
void CNetworkMgr::Send_RpCsDeposit( DWORD aItemID, BYTE aBundle, BYTE aBankSlot ) 
{
	//	DWORD			theItemID;
	//	BYTE			theBundle;			// �Ҹ�ǰ�� ���
	//	BYTE			theBankSlot;		// �������� ���࿡ ��ġ�� ���� ��ȣ(0 ~ 124)
	//PRINT_DLG_LOG("CsDeposit aItemID %d aBundle %d aBankSlot %d ",aItemID , aBundle , aBankSlot );

	if( aBundle == 0)
		return;
	
	SRpCsDeposit msgCsDeposit;
	msgCsDeposit.theMsgHeader.thePCID = thePCID;
	msgCsDeposit.theItemID = aItemID;
	msgCsDeposit.theBundle = aBundle;
	msgCsDeposit.theBankSlot = aBankSlot;
	
	NetToGame( &msgCsDeposit, sizeof( msgCsDeposit ) );
	
	return;
}

// �������� ã�´�.
void CNetworkMgr::Send_RpCsDraw( DWORD aBankItemID, BYTE aBundle ) 
{
	//	BYTE			theBankItemID;		// ã�� �������� ID
	//	BYTE			theBundle;			// �Ҹ�ǰ�� ��� ã�� ����� 
	//PRINT_DLG_LOG("Send_RpCsDraw aBankItemID %d aBundle %d",aBankItemID , aBundle );
	

	if( aBundle == 0)
		return;

	
	SRpCsDraw msgCsDraw;
	msgCsDraw.theMsgHeader.thePCID = thePCID;
	msgCsDraw.theBankItemID = aBankItemID;
	msgCsDraw.theBundle = aBundle;
	
	NetToGame( &msgCsDraw, sizeof( msgCsDraw ) );
	
	return;
}

// Eld�� ���࿡ �ñ��.
void CNetworkMgr::Send_RpCsDepositEld( DWORD aEld ) 
{
	//	DWORD			theEld;
	
	SRpCsDepositEld msgCsDepositEld;
	msgCsDepositEld.theMsgHeader.thePCID = thePCID;
	msgCsDepositEld.theEld = aEld;
	
	NetToGame( &msgCsDepositEld, sizeof( msgCsDepositEld ) );
	
	return;
}

// �ٵ带 ã�´�. 
void CNetworkMgr::Send_RpCsDrawEld( DWORD aEld ) 
{
	//	DWORD			theEld;
	
	SRpCsDrawEld msgCsDrawEld;
	msgCsDrawEld.theMsgHeader.thePCID = thePCID;
	msgCsDrawEld.theEld = aEld;
	
	NetToGame( &msgCsDrawEld, sizeof( msgCsDrawEld ) );
	
	return;
}

// ����â������ ������ �̵� 
void CNetworkMgr::Send_RpCsWHChangeSlot( DWORD aBankItemID, BYTE aSlot ) 
{
	//	BYTE			theBankItemID;		// �ű���� �ϴ� �������� ���� ���Թ�ȣ 
	//	BYTE			theSlot;			// �ű� ��ġ 
	//--PRINT_DLG_LOG("Send_RpCsWHChangeSlota BankItemID %d aSlot %d " , aBankItemID , aSlot );

	nRui->m_KeyLock.Warehouse.SetLock(g_nowTime);
	SRpCsWHChangeSlot msgCsWHChangeSlot;
	msgCsWHChangeSlot.theMsgHeader.thePCID = thePCID;
	msgCsWHChangeSlot.theBankItemID = aBankItemID;
	msgCsWHChangeSlot.theSlot = aSlot;
	
	NetToGame( &msgCsWHChangeSlot, sizeof( msgCsWHChangeSlot ) );
	
	return;
}
#ifdef DECO_RENEWAL_GUILD_MJH

//============================================================================================================
//============================================================================================================
// ���â�� �ý��� ���� �������� 
//============================================================================================================
//============================================================================================================
// �������� �����Ѵ�. 
void CNetworkMgr::Send_RpCsCloseFrontierBank( TCHAR* aPcName )
{
	SRpCsFrontierBankClose msgCsCloseFrontierBank;
	msgCsCloseFrontierBank.theMsgHeader.thePCID = thePCID;
	ZeroMemory(msgCsCloseFrontierBank.thePCName , RP_MAX_PC_NAME );
	SAFE_STR_CPY( msgCsCloseFrontierBank.thePCName, aPcName, RP_MAX_PC_NAME );


	NetToGame( &msgCsCloseFrontierBank, sizeof( msgCsCloseFrontierBank ) );
	
	return;
}
void CNetworkMgr::Send_RpCsDepositFrontier( DWORD aItemID, BYTE aBundle, BYTE aBankSlot ) 
{
	//	DWORD			theItemID;
	//	BYTE			theBundle;			// �Ҹ�ǰ�� ���
	//	BYTE			theBankSlot;		// �������� ���࿡ ��ġ�� ���� ��ȣ(0 ~ 124)
	//PRINT_DLG_LOG("CsDepositFrontier aItemID %d aBundle %d aBankSlot %d ",aItemID , aBundle , aBankSlot );

	if( aBundle == 0)
		return;
	
	SRpCsDepositFrontierBank msgCsDepositFrontier;
	msgCsDepositFrontier.theMsgHeader.thePCID = thePCID;
	msgCsDepositFrontier.theItemID = aItemID;
	msgCsDepositFrontier.theBundle = aBundle;
	msgCsDepositFrontier.theBankSlot = aBankSlot;
	
	NetToGame( &msgCsDepositFrontier, sizeof( msgCsDepositFrontier ) );
	
	return;
}

// ���â���� �������� ã�´�.
void CNetworkMgr::Send_RpCsDrawFrontier( DWORD aBankItemID, BYTE aBundle ) 
{
	//	BYTE			theBankItemID;		// ã�� �������� ID
	//	BYTE			theBundle;			// �Ҹ�ǰ�� ��� ã�� ����� 
	//PRINT_DLG_LOG("Send_RpCsDrawFrontier aBankItemID %d aBundle %d",aBankItemID , aBundle );
	

	if( aBundle == 0)
		return;

	
	SRpCsDrawFrontierBank msgCsDrawFrontier;
	msgCsDrawFrontier.theMsgHeader.thePCID = thePCID;
	msgCsDrawFrontier.theBankItemID = aBankItemID;
	msgCsDrawFrontier.theBundle = aBundle;
	
	NetToGame( &msgCsDrawFrontier, sizeof( msgCsDrawFrontier ) );
	
	return;
}

// ���â������ ������ �̵� 
void CNetworkMgr::Send_RpCsChangeFrontierSlot( DWORD aBankItemID, BYTE aSlot ) 
{
	//	BYTE			theBankItemID;		// �ű���� �ϴ� �������� ���� ���Թ�ȣ 
	//	BYTE			theSlot;			// �ű� ��ġ 
	//--PRINT_DLG_LOG("Send_RpCsChangeFrontierSlot BankItemID %d aSlot %d " , aBankItemID , aSlot );

	nRui->m_KeyLock.GuildInven.SetLock(g_nowTime);
	SRpCsChangeSlotFrontierBank msgCsChangeSlotFrontier;
	msgCsChangeSlotFrontier.theMsgHeader.thePCID = thePCID;
	msgCsChangeSlotFrontier.theBankItemID = aBankItemID;
	msgCsChangeSlotFrontier.theSlot = aSlot;
	
	NetToGame( &msgCsChangeSlotFrontier, sizeof( msgCsChangeSlotFrontier ) );
	
	return;
}
#endif //DECO_RENEWAL_GUILD_MJH

///---------------------------------------------------------------------------
///-- Send_CsSetStone
///---------------------------------------------------------------------------
///-- ������ ���� �� �����۰� ���� ID ����
void CNetworkMgr::Send_CsSetStone( DWORD aItemID, DWORD aEldaID, DWORD aPayItem[3] )
{
	SRpCsSetStone msgCsSetStone;
	msgCsSetStone.theMsgHeader.thePCID = thePCID;
	msgCsSetStone.theItemID = aItemID;
	msgCsSetStone.theStoneID = aEldaID;

#if defined( PAYITEM_6 )

	for( int i = 0 ; i < 3; ++i )
	{
		msgCsSetStone.thePayItem[i] = aPayItem[i];
	}

#endif
	
	NetToGame( &msgCsSetStone, sizeof( msgCsSetStone ) );
}
///---------------------------------------------------------------------------
///-- Send_CsEldacraftGame
///---------------------------------------------------------------------------
///-- Game ����� �� Column �ϼ��� ���� ����
void CNetworkMgr::Send_CsEldacraftGame( DWORD aMiss100, DWORD aMiss50, DWORD aCombo, DWORD aScore, BYTE aEnd, SEldacraftGameCell *theCellList )
{
	int i;
	
	SRpCsEldacraftGame msgEldacraftGame;
	msgEldacraftGame.theMsgHeader.thePCID = thePCID;
	msgEldacraftGame.theMissCount_100 = (BYTE)aMiss100;
	msgEldacraftGame.theMissCount_50= (BYTE)aMiss50;
	msgEldacraftGame.theCombo = (BYTE)aCombo;
	msgEldacraftGame.theScore = aScore;
	msgEldacraftGame.theEndingFlag = aEnd;

	for( i = 0 ; i < 5; ++i )	
	{
		msgEldacraftGame.theCellList[i] = theCellList[i];
		/*
		msgEldacraftGame.theCellList[i].theColor = theCellList[i].theColor;
		msgEldacraftGame.theCellList[i].theSubColor= theCellList[i].theSubColor;
		msgEldacraftGame.theCellList[i].theType= theCellList[i].theType*/;
	}
	
	NetToGame( &msgEldacraftGame, sizeof( msgEldacraftGame ) );
	
}

///---------------------------------------------------------------------------
///-- Send_CsMakeItem
///---------------------------------------------------------------------------
///-- ������ ����
void CNetworkMgr::Send_CsMakeItem( SBundleItem atheItem[6], BYTE aB_item_type  )
{
	int i;
	SRpCsMakeItem msgmakeitem;
	msgmakeitem.theMsgHeader.thePCID = thePCID;	
	msgmakeitem.theType = aB_item_type;
	
	for( i = 0 ; i < 6; ++i )
	{
		msgmakeitem.theItem[i].theItemID = atheItem[i].theItemID;
		msgmakeitem.theItem[i].theBundle = atheItem[i].theBundle;
	}
	
	NetToGame( &msgmakeitem, sizeof( msgmakeitem) );
}

void CNetworkMgr::Send_CsMakeItem( SBundleItem aBundleItem[6], BYTE aB_item_type, DWORD aPayItem[3] )
{
	int i;
	SRpCsMakeItem msgmakeitem;
	msgmakeitem.theMsgHeader.thePCID = thePCID;	
	msgmakeitem.theType = aB_item_type;
	ZeroMemory(&msgmakeitem.thePayItem,sizeof(msgmakeitem.thePayItem));

	for( i = 0 ; i < 6; ++i )
	{
		msgmakeitem.theItem[i].theItemID = aBundleItem[i].theItemID;
		msgmakeitem.theItem[i].theBundle = aBundleItem[i].theBundle;
	}

	for( i = 0 ; i < 3; ++i )
	{
		msgmakeitem.thePayItem[i] = aPayItem[i];
	}
	
	NetToGame( &msgmakeitem, sizeof( msgmakeitem) );
}

//============================================================================================================
//============================================================================================================
// ... ���� �������� 
//============================================================================================================
//============================================================================================================

void CNetworkMgr::Send_CsSkillUp( DWORD aSkillCode ) 
{
	SRpCsSkillUp msgSkillUp;
	msgSkillUp.theMsgHeader.thePCID = thePCID;
	msgSkillUp.theSkillCode = aSkillCode;
	
	NetToGame( &msgSkillUp, sizeof( msgSkillUp ) );

	return;
}

///---------------------------------------------------------------------------
///-- Send_CsRequstRepair
///---------------------------------------------------------------------------
void CNetworkMgr::Send_CsRequstRepair( SRpCsRequstRepair * aMsg )
{
	aMsg->theMsgHeader.thePCID = thePCID;
	NetToGame( aMsg, sizeof( SRpCsRequstRepair ) );
}

///-----------------------------------------------------------------------
///-- community Protocols 
///-----------------------------------------------------------------------
void CNetworkMgr::Send_CsMsngrMemberInsert( TCHAR * aPCName )
{
	SRpCsMsngrMemberInsert msgCsMsngrMemberInsert;
	msgCsMsngrMemberInsert.theMsgHeader.thePCID = thePCID;
	ZeroMemory(msgCsMsngrMemberInsert.thePCName , RP_MAX_PC_NAME );
	SAFE_STR_CPY( msgCsMsngrMemberInsert.thePCName, aPCName, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsMsngrMemberInsert, sizeof( msgCsMsngrMemberInsert ) );
}

void CNetworkMgr::Send_CsMsngrMemberInsertAns( bool bFlag, TCHAR * aPCName )
{
	SRpCsMsngrMemberInsertAns msgCsMsngrMemberInsertAns;
	msgCsMsngrMemberInsertAns.theMsgHeader.thePCID = thePCID;

	msgCsMsngrMemberInsertAns.theYesNoFlag = bFlag;
	ZeroMemory(msgCsMsngrMemberInsertAns.thePCName , RP_MAX_PC_NAME );
	SAFE_STR_CPY( msgCsMsngrMemberInsertAns.thePCName, aPCName, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsMsngrMemberInsertAns, sizeof( msgCsMsngrMemberInsertAns ) );
}

void CNetworkMgr::Send_CsMsngrMemberDelete( TCHAR * aPCName )
{
	SRpCsMsngrMemberDelete msgCsMsngrMemberDelete;
	msgCsMsngrMemberDelete.theMsgHeader.thePCID = thePCID;
	ZeroMemory(msgCsMsngrMemberDelete.thePCName , RP_MAX_PC_NAME );
    SAFE_STR_CPY( msgCsMsngrMemberDelete.thePCName, aPCName, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsMsngrMemberDelete, sizeof( msgCsMsngrMemberDelete ) );
}

void CNetworkMgr::Send_CsMsngrMemberBlock( BYTE bProckType, TCHAR * aPCName )
{
	SRpCsMsngrMemberBlock msgCsMsngrMemberBlock;
	msgCsMsngrMemberBlock.theMsgHeader.thePCID = thePCID;
	
	msgCsMsngrMemberBlock.theProcType = bProckType;
	ZeroMemory(msgCsMsngrMemberBlock.thePCName , RP_MAX_PC_NAME );
    SAFE_STR_CPY( msgCsMsngrMemberBlock.thePCName, aPCName, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsMsngrMemberBlock, sizeof( msgCsMsngrMemberBlock ) );
}

void CNetworkMgr::Send_CsMsngrChangeOpt( SRpCsMsngrChangeOpt * aMsg )
{
	aMsg->theMsgHeader.thePCID = thePCID;
	NetToGame( aMsg, sizeof( SRpCsMsngrChangeOpt ) );
}

void CNetworkMgr::Send_CsMsngrSay( TCHAR * aPCName, TCHAR* aSayMessage )
{
	static SRpCsMsngrSay msgCsMsngrSay;
	msgCsMsngrSay.theMsgHeader.thePCID = thePCID;

	ZeroMemory(msgCsMsngrSay.theToPCName , RP_MAX_PC_NAME);
	ZeroMemory(msgCsMsngrSay.theStr , sizeof(msgCsMsngrSay.theStr));
	SAFE_STR_CPY( msgCsMsngrSay.theToPCName , aPCName, RP_MAX_PC_NAME );
	SAFE_STR_CPY( msgCsMsngrSay.theStr , aSayMessage, 128 );

	NetToGame( &msgCsMsngrSay, sizeof( SRpCsMsngrSay ) );
}

void CNetworkMgr::Send_CsMsngrChatCreateRoom()
{
	SRpCsMsngrChatCreateRoom msgCsMsngrChatCreateRoom;
	msgCsMsngrChatCreateRoom.theMsgHeader.thePCID = thePCID;

	NetToGame( &msgCsMsngrChatCreateRoom, sizeof(msgCsMsngrChatCreateRoom) );
}

void CNetworkMgr::Send_CsMsngrChatExitRoom()
{
	SRpCsMsngrChatExitRoom msgCsMsngrChatExitRoom;
	msgCsMsngrChatExitRoom.theMsgHeader.thePCID = thePCID;

	NetToGame( &msgCsMsngrChatExitRoom, sizeof(msgCsMsngrChatExitRoom) );
}

void CNetworkMgr::Send_CsMsngrChatInviteAsk( TCHAR * aPCName )
{
	SRpCsMsngrChatInviteAsk msgCsMsngrChatInviteAsk;
	msgCsMsngrChatInviteAsk.theMsgHeader.thePCID = thePCID;
	ZeroMemory(msgCsMsngrChatInviteAsk.theToPCName , sizeof(msgCsMsngrChatInviteAsk.theToPCName ) );
	SAFE_STR_CPY( msgCsMsngrChatInviteAsk.theToPCName , aPCName, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsMsngrChatInviteAsk, sizeof( msgCsMsngrChatInviteAsk ) );
}

void CNetworkMgr::Send_CsMsngrChatInviteAns( bool YesNoFlag, TCHAR * aFromName, TCHAR * aOwnerName)
{
	SRpCsMsngrChatInviteAns msgCsMsngrChatInviteAns;
	msgCsMsngrChatInviteAns.theMsgHeader.thePCID = thePCID;

	msgCsMsngrChatInviteAns.theYesNoFlag = YesNoFlag;
	ZeroMemory(msgCsMsngrChatInviteAns.theFromPCName, sizeof(msgCsMsngrChatInviteAns.theFromPCName ) );
	ZeroMemory(msgCsMsngrChatInviteAns.theOwnerPCName, sizeof(msgCsMsngrChatInviteAns.theOwnerPCName ) );

	SAFE_STR_CPY( msgCsMsngrChatInviteAns.theFromPCName , aFromName, RP_MAX_PC_NAME );
	SAFE_STR_CPY( msgCsMsngrChatInviteAns.theOwnerPCName , aOwnerName, RP_MAX_PC_NAME );
	
	NetToGame( &msgCsMsngrChatInviteAns, sizeof( msgCsMsngrChatInviteAns ) );
}

void CNetworkMgr::Send_CsMsngrChatSay( TCHAR* aSayMessage )
{
	static SRpCsMsngrChatSay msgCsMsngrChatSay;
	msgCsMsngrChatSay.theMsgHeader.thePCID = thePCID;

	ZeroMemory(msgCsMsngrChatSay.theStr , sizeof(msgCsMsngrChatSay.theStr) );
	SAFE_STR_CPY( msgCsMsngrChatSay.theStr , aSayMessage, 128 );
	
	NetToGame( &msgCsMsngrChatSay, sizeof( msgCsMsngrChatSay ) );
}

///---------------------------------------------------------------------------
///-- Send_CsOnInactivated
///---------------------------------------------------------------------------
void CNetworkMgr::Send_CsOnInactivated()
{	
	if( !theGameSocketPtr) return;
	if( theConnectState == nInGameServer )
	{
		SRpCsOnInactivated msgCsInactivate;
		msgCsInactivate.theMsgHeader.thePCID = thePCID;
		NetToGame( &msgCsInactivate, sizeof( msgCsInactivate ) );	
	}
}


///---------------------------------------------------------------------------
///-- Send_CsOnRestoreOK
///---------------------------------------------------------------------------
void CNetworkMgr::Send_CsOnRestoreOK()
{	
	if( !theGameSocketPtr) return;
	if( theConnectState == nInGameServer )
	{
		SRpCsOnRestoreOK msgCsRestoreMsg;
		msgCsRestoreMsg.theMsgHeader.thePCID = thePCID;
		NetToGame( &msgCsRestoreMsg, sizeof( msgCsRestoreMsg ) );
	}
}

///void CNetworkMgr::Send_CsRestoreDeviceObjectsEnd()
// � ���� ������ �������� ����...
//void Send_CsRestoreDeviceObjectsStart();
//void Send_CsRestoreDeviceObjectsEnd();


/*
void CNetworkMgr::Send_CsRestoreDeviceObjectsStart()
{
	SRpCsRestoreDelayStart msgCsRestoreMsg;
	msgCsRestoreMsg.theMsgHeader.thePCID = thePCID;

  NetToGame( &msgCsRestoreMsg, sizeof( msgCsRestoreMsg ) );
}

void CNetworkMgr::Send_CsRestoreDeviceObjectsEnd()
{
	SRpCsRestoreDelayEnd msgCsRestoreMsg;
	msgCsRestoreMsg.theMsgHeader.thePCID = thePCID;

  NetToGame( &msgCsRestoreMsg, sizeof( msgCsRestoreMsg ) );
}
*/

//............................................................................................................
// �米 ���� ����
//............................................................................................................
void CNetworkMgr::Send_CsExpression(DWORD aPCID,BYTE Index)
{
	SRpCsExpression	TempMsg;

	TempMsg.theMsgHeader.thePCID = aPCID;
	TempMsg.theExpression		 = Index;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

//============================================================================================================
//============================================================================================================
// PVP �ý��� ���� 
//============================================================================================================
//============================================================================================================

// 
void CNetworkMgr::Send_CsReadyFightSystem()
{
	SRpCsReadyFightSystem	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}


// ��� Ÿ�� ����
void CNetworkMgr::Send_CsSelectFightGameType( BYTE aGameType )
{
	SRpCsSelectFightGameType	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFightGameType = aGameType;			// n_ReguralGame (������), n_IrreguralGame (�������) 

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}
/*
// ��� ����
void CNetworkMgr::Send_CsSelectFightGrade( BYTE aGradeType )
{	
	SRpCsSelectFightGrade	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFightGrade = aGradeType;				// 0, 1... ���ʷ�...

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
	
}
*/

// �游��� ( ������ ���� )
void CNetworkMgr::Send_CsMakeFightRoom( _SFightRoomInfo aRoomInfo )
{
	SRpCsMakeFightRoom	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theInfo = aRoomInfo;			

	NetToGame( &TempMsg , sizeof( TempMsg ) );
	
	return;
}

// ���� �����ϱ�
void CNetworkMgr::Send_CsJoinFightRoom( WORD aRoomID, void* paPassWord, BYTE	theWatcher)
{
	char* aPassWord = (char*)paPassWord;
	SRpCsJoinFightRoom	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theRoomID = aRoomID;			

	Rsprintf( RWCHAR(TempMsg.thePassword) , _RT("%s"), RWCHAR(aPassWord) );

#ifdef JUST_WATCH
	TempMsg.theWatcher = theWatcher;
#endif
	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// ���ӹ濡�� ������ ( ������ �� ���� )
void CNetworkMgr::Send_CsOutFightRoom( DWORD aKickID )
{
	SRpCsOutFightRoom	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.thePCID = aKickID;				

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// �������� ����( ���常 ����)
void CNetworkMgr::Send_CsChangeFightInfo( BYTE aMapCode, BYTE aTimeOut, BYTE aItemFlag )
{
	SRpCsChangeFightInfo	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID; 
	TempMsg.theMapCode = aMapCode;
	TempMsg.theTimeOut = aTimeOut;
	TempMsg.theItemFlag = aItemFlag;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// �� ���� 
void CNetworkMgr::Send_CsSelectTeam( BYTE aSelectTeam )
{
	SRpCsSelectTeam	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theTeam = aSelectTeam;				

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// ���� �غ� ��ư Ŭ��!! or ���!! 
void CNetworkMgr::Send_CsFightReady( BYTE aFlag )
{
	SRpCsFightReady	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFlag = aFlag;				// n_Success : �غ�   n_Fail : ��� 	

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// ���� ���� 
void CNetworkMgr::Send_CsFightStart()
{
	SRpCsFightStart	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// ������ �ε� �Ϸ�
void CNetworkMgr::Send_CsFightReadyWarp()
{
	SRpCsFightReadyWarp	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// �������� ������ ( �������� ���ư��� )
void CNetworkMgr::Send_CsOutWaitingRoom()
{
	SRpCsOutWaitingRoom	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

//..........................................................................................................
// ���� ���� ����
//..........................................................................................................

// ���� ���� ��û
void CNetworkMgr::Send_CsOpenPCShopWindow()
{
	SRpCsOpenPCShopWindow TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// ���� ���� ����
void CNetworkMgr::Send_CsOpenPCShop()
{
	SRpCsOpenPCShop TempMsg;
	
	TempMsg.theMsgHeader.thePCID = thePCID;
	
	g_UserStoreSellWnd.m_pStoreNameText->GetText( TempMsg.theShopName , 41 );
	g_UserStoreSellWnd.m_pStoreContextText->GetText( TempMsg.theShopDesc , 41 );

	BYTE Count = 0;

	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( g_UserStoreSellWnd.m_WarehousePcItem[ i ].Amount )
		{
			TempMsg.theShopItemList[ Count ].theItemUID = ( DWORD ) g_UserStoreSellWnd.m_WarehousePcItem[ i ].ItemUID;
			TempMsg.theShopItemList[ Count ].thePrice   = ( DWORD ) g_UserStoreSellWnd.m_PriceList[ i ];
			TempMsg.theShopItemList[ Count ].theBundle  = ( BYTE  ) g_UserStoreSellWnd.m_WarehousePcItem[ i ].Amount;

			Count++;
		}
	}

	TempMsg.theShopItemNum = Count;
	
	//......................................................................................................
	// ��Ŷ ���� ������ ���
	//......................................................................................................
	WORD DataSize = sizeof( SRpCsOpenPCShop );

	DataSize = DataSize - ( sizeof( _ShopInfo ) * ( 50 - Count ) );

	TempMsg.theMsgHeader.theSize = DataSize;

	NetToGame( &TempMsg , DataSize );
}

// ���� ���� �ݱ�
void CNetworkMgr::Send_CsClosePCShop()
{
	SRpCsClosePCShop TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	EVENT_DATA_INFO event_store;
	
	event_store.type   = SM_USE_SKILL;
	event_store.status = SM_SELLSKILL_STANDUP;
	event_store.motion = 0;			
	
	
	g_UserStoreSellWnd.Close();

	g_Pc.GetPlayer()->PushQueue( event_store, TRUE );
	nRui->DeleteDlgBalloon( enPLAYER , thePCID );
}

// ���� ���� ����
void CNetworkMgr::Send_CsInPCShop(DWORD ID)
{
	int Index = 0;
	
	Index = g_Pc.FindUser( ID );
	
	if( Index == MAX_USER_INTABLE )
		return;

	SRpCsInPCShop TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.thePCID = ID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	g_UserStoreBuyWnd.SetShopPCID( ID , ( TCHAR * )g_Pc.m_PcTable[ Index ].char_info.thePCName );
}



// ���� ���� ���
void CNetworkMgr::Send_CsBuyInPCShop(DWORD ShopPCID,DWORD ItemUID,BYTE Count)
{
	SRpCsBuyInPCShop TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.thePCID              = ShopPCID;
	TempMsg.theItemUID           = ItemUID;
	TempMsg.theBundle            = Count;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}


// ������ ��������� ����Ʈ ���� 
void CNetworkMgr::Send_CsOpenPayShop()
{
	SRpCsOpenPayShop TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// ������ ��������� �κ����� �ű�� 
void CNetworkMgr::Send_CsGetPayItem( DWORD aPayItemID )
{
	SRpCsGetPayItem TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.thePayItemID = aPayItemID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}


// â�� ��û 
#ifdef DECO_RENEWAL_GUILD_MJH
void CNetworkMgr::Send_CsReqMakeFrontier( TCHAR* aFrontierName )
{
//	char		theFrontierName[RP_MAX_FRONTIER_NAME];	// ������ ����Ƽ���

	SRpCsReqMakeFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	SAFE_STR_CPY( TempMsg.theFrontierName, aFrontierName, RP_MAX_FRONTIER_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}
// �������� �ۼ�
void CNetworkMgr::Send_CsChangeFrontierNotice( TCHAR* aFrontierNotice , DWORD aFrontierID)
{
//  char       theNewNotice  // ��� ��������   

	SRpCsChangeFrontierNotice TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFrontierID = aFrontierID;

	SAFE_STR_CPY( TempMsg.theNewNotice, aFrontierNotice, 50 );

	NetToGame( &TempMsg , sizeof( TempMsg ) );	

}
void CNetworkMgr::Send_CsChangeFrontierMemberIntro( TCHAR* aMemberIntro )
{
//  char       theNewNotice  // ��� �ڱ�Ұ�   

	SRpCsChangeFrontierMemberIntro TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	SAFE_STR_CPY( TempMsg.theNewIntroduce, aMemberIntro, 30  );

	NetToGame( &TempMsg , sizeof( TempMsg ) );	

}
//����� ����
void CNetworkMgr::Send_CsChangeFrontierMaster( TCHAR* aNewMaster )
{
//  DWORD       aFrontierID      // ����Ƽ����̵�
//  char        aNewMaster       // ���ο� �����͸�

	SRpCsChangeFrontierMaster TempMsg;
	
	TempMsg.theMsgHeader.thePCID = thePCID;
	SAFE_STR_CPY( TempMsg.theNewFrontierMasterName, aNewMaster, RP_MAX_PC_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsMsngrProposeJoinFrontier( DWORD aReceiverID , DWORD aEld , _SExItem aItem )
{
//	DWORD		theReceiverID;		// ���ȹ޴� ���� ID

	SRpCsMsngrProposeJoinFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theReceiverID = aReceiverID;
	TempMsg.theEld = aEld;
	TempMsg.theItem = aItem;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

#else 

void CNetworkMgr::Send_CsReqMakeFrontier( TCHAR* aFrontierName, TCHAR* aMemberName0, TCHAR* aMemberName1, TCHAR* aMemberName2 )
{
//	char		theFrontierName[RP_MAX_FRONTIER_NAME];	// ������ ����Ƽ���
//	char		theMemberName[3][RP_MAX_PC_NAME];		// â�ܸɹ� 3��

	SRpCsReqMakeFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	SAFE_STR_CPY( TempMsg.theFrontierName, aFrontierName, RP_MAX_FRONTIER_NAME );
	SAFE_STR_CPY( TempMsg.theMemberName[0], aMemberName0, RP_MAX_PC_NAME );
	SAFE_STR_CPY( TempMsg.theMemberName[1], aMemberName1, RP_MAX_PC_NAME );
	SAFE_STR_CPY( TempMsg.theMemberName[2], aMemberName2, RP_MAX_PC_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}

void CNetworkMgr::Send_CsMsngrProposeJoinFrontier( DWORD aReceiverID )
{
//	DWORD		theReceiverID;		// ���ȹ޴� ���� ID

	SRpCsMsngrProposeJoinFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theReceiverID = aReceiverID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
#endif // DECO_RENEWAL_GUILD_MJH

// �������ȿ� ���� �亯 ���� 
void CNetworkMgr::Send_CsMsngrAnswerJoinFrontier( BYTE aAnswer )
{
//	DWORD		theReceiverID;		// ���ȹ޴� ���� ID

	SRpCsMsngrAnswerJoinFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theAnswer = aAnswer;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// Ż�� 
// ������ ���� Ż�𰡴�, Major �̻�� ���� ���� 
void CNetworkMgr::Send_CsMsngrLeaveFrontier( TCHAR* aPCName )
{
//	char		thePCName[RP_MAX_PC_NAME];	// Ż�����ڸ� (�α׾ƿ����� ������ Ż���ų���� �ֱ� ����)

	SRpCsMsngrLeaveFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	SAFE_STR_CPY( TempMsg.thePCName, aPCName, RP_MAX_PC_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}


// ��ü����â���� �ɹ��� ȣĪ�� ��� ���� ��û 
void CNetworkMgr::Send_CsMsngrFrontierMemberInfo( TCHAR* aPCName, TCHAR* aNickName, BYTE aGrade )
{
//	char		thePCName[RP_MAX_PC_NAME];
//	char		theNickName[RP_MAX_FRONTIER_NICK];
//	BYTE		theGrade;

	SRpCsMsngrFrontierMemberInfo TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	SAFE_STR_CPY( TempMsg.thePCName, aPCName, RP_MAX_PC_NAME );
	SAFE_STR_CPY( TempMsg.theNickName, aNickName, RP_MAX_FRONTIER_NICK );

	TempMsg.theGrade = aGrade; 

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// ������ Appear�� ���������� Ŭ���̾�Ʈ�� �˷��ְ�
// Ŭ���̾�Ʈ�� �ش� ����Ƽ���� ������ ������ ���� ��� ������ �� ������ ��û�ϰ� �޾Ƽ� �����Ѵ�. 
// ���� ���� 

#ifdef DECO_RENEWAL_GUILD_MJH

void CNetworkMgr::Send_CsMsngrUploadEmblem( BYTE aEmblem )
{
	SRpCsMsngrUploadEmblem TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theVersion = aEmblem; 
	

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

#else

void CNetworkMgr::Send_CsMsngrUploadEmblem( WORD aEmblemSize, char* aEmblem )
{

	SRpCsMsngrUploadEmblem TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theMsgHeader.theSize += aEmblemSize;
	
	TempMsg.theEmblemSize = aEmblemSize; 
	
	memcpy(TempMsg.theEmblem, aEmblem, aEmblemSize);

	NetToGame( &TempMsg , TempMsg.theMsgHeader.theSize );
}

#endif // DECO_RENEWAL_GUILD_MJH 

// ���� ��û (Ŭ���̾�Ʈ�� ���� ������ ���)
void CNetworkMgr::Send_CsMsngrEmblem( DWORD aFrontierID )
{
//	DWORD		theFrontierID;

	SRpCsMsngrEmblem TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.theFrontierID = aFrontierID; 

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// ����Ƽ�� ���� ����
void CNetworkMgr::Send_CsPayDues( DWORD aDues )
{
//	DWORD		theDues;		// ����Ƽ�� �ݾ� ������ �й��� �ݾ� 

	SRpCsPayDues TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.theDues = aDues; 

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// ����Ƽ�� ���� �й� 
void CNetworkMgr::Send_CsMsngrDivideDues( DWORD aDues )
{
//	DWORD		theDues;		// ����Ƽ�� �ݾ� ������ �й��� �ݾ� 

	SRpCsMsngrDivideDues TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.theDues = aDues; 

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsMsngrFrontierSay( TCHAR* aStr )
{
//	char		theStr[128]; 			/// ��ȭ ���� 

	static SRpCsMsngrFrontierSay TempMsg;	
	TempMsg.theMsgHeader.thePCID = thePCID;
	ZeroMemory(TempMsg.theStr , sizeof(TempMsg.theStr));

#ifdef UNICODE_ROCK
	BYTE	bMessageSize = (Rstrlen(aStr)<<1)+2 ;
#else
	BYTE	bMessageSize = Rstrlen(aStr)+1; 
#endif
		
	TempMsg.theMsgHeader.theSize = sizeof( _SRpCsMsngrFrontierSay ) - sizeof( TempMsg.theStr );
	TempMsg.theMsgHeader.theSize += bMessageSize;
	TempMsg.theStrSize = bMessageSize;
	SAFE_STR_CPY( TempMsg.theStr, aStr, RP_MAX_CHAT_SIZE );
	NetToGame( &TempMsg, TempMsg.theMsgHeader.theSize );


}

void CNetworkMgr::Send_CsMsngrAddFrontierMember(TCHAR* aPCName , DWORD aFrontierID)
{

	SRpCsMsngrAddFrontierMember TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theFrontierID = aFrontierID;
	TempMsg.thePCID = thePCID ; 
	
	SAFE_STR_CPY( TempMsg.thePCName, aPCName, RP_MAX_PC_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );

}

// �ż�����ó�� ��û 
void CNetworkMgr::Send_CsCreateFrontierPet( WORD aItemCode, TCHAR* aPetName )
{
//	WORD				theItemCode;
//	char				thePetName[20];

	SRpCsCreateFrontierPet TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theItemCode = aItemCode;

	SAFE_STR_CPY( TempMsg.thePetName, aPetName, RP_MAX_FRONTIER_PET_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// �ż� ��ȯ ��û 
void CNetworkMgr::Send_CsMsngrCallFrontierPet()
{
	SRpCsMsngrCallFrontierPet TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// ����Ƽ�� ���� ��û (�����ͱ���)
void CNetworkMgr::Send_CsMsngrReqCombine(DWORD aProposedPCID)
{
	SRpCsMsngrReqCombine TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theProposedPCID = aProposedPCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

//��������Ƽ�� ��û�� ���� �亯
void CNetworkMgr::Send_CsMsngrAnsCombine(DWORD aProposerPCID, BYTE aAnswer)
{
	SRpCsMsngrAnsCombine TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theProposerPCID = aProposerPCID;
	TempMsg.theAnswer = aAnswer;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// ���� ����Ƽ�� ���� ��û
void CNetworkMgr::Send_CsMsngrSeperateF()
{
	SRpCsMsngrSeperateF TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
#ifdef DECO_RENEWAL_CORPORATION_MJH
// ���� ���� ��û
void CNetworkMgr::Send_CsMsngrCorpJoin( BYTE aCorps )
{
	SRpCsJoinCorps TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theCorps = aCorps;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
// ���� Ż�� ��û
void CNetworkMgr::Send_CsMsngrCorpLeave()
{
	SRpCsLeaveCorps TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
#endif // DECO_RENEWAL_CORPORATION_MJH
// �ż� ��ȯ ���� ��û 
void CNetworkMgr::Send_CsMsngrCancelCallFrontierPet()
{
	SRpCsMsngrCancelCallFrontierPet TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// �ް��� ��������ۿ� 
void CNetworkMgr::Send_CsShout(DWORD theItemID, TCHAR *theMessage)
{
	static SRpCsShout	msgCsShout;
	msgCsShout.theMsgHeader.thePCID = thePCID;
	ZeroMemory(msgCsShout.theMessage , sizeof(msgCsShout.theMessage));

#ifdef UNICODE_ROCK
	BYTE	bMessageSize = (Rstrlen(theMessage)<<1)+2;	
#else
	BYTE	bMessageSize = Rstrlen(theMessage)+1;	
#endif
	
	msgCsShout.theMsgHeader.theSize = sizeof( _SRpCsShout ) - sizeof( msgCsShout.theMessage );
	msgCsShout.theMsgHeader.theSize += bMessageSize;
	msgCsShout.theMessageSize = bMessageSize;
	SAFE_STR_CPY( msgCsShout.theMessage, theMessage, RP_MAX_CHAT_SIZE );

	msgCsShout.theItemID = theItemID;

	NetToGame( &msgCsShout, msgCsShout.theMsgHeader.theSize );
}

//------------------------------------------------------------------------------------
// Pet ����
//------------------------------------------------------------------------------------
void CNetworkMgr::Send_CsMakePetName( TCHAR * aPetName , BYTE CancelFlag )
{
	SRpCsMakePetName TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	
	memset( TempMsg.thePetName, 0  , RP_MAX_PET_NAME );
	SAFE_STR_CPY( TempMsg.thePetName, aPetName, RP_MAX_PET_NAME );
	TempMsg.theCancelFlag = CancelFlag;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsMoveItemPCtoPet( DWORD aItemId, BYTE aPetSlot, BYTE aItemPos, BYTE aItemNum )
{
	SRpCsMoveItemPCtoPet TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theItemID = aItemId;
	TempMsg.thePetSlot = aPetSlot;
	TempMsg.theItemPos = aItemPos;
	TempMsg.theItemNum = aItemNum;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsMoveItemPettoPC( DWORD aItemId, BYTE aPetSlot, BYTE aItemPos, BYTE aItemNum )
{
	SRpCsMoveItemPettoPC TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theItemID = aItemId;
	TempMsg.thePetSlot = aPetSlot;
	TempMsg.theItemPos = aItemPos;
	TempMsg.theItemNum = aItemNum;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsUsePetItem( BYTE aPetSlot, DWORD aItemID,  TCHAR * aPetName )
{
	SRpCsUsePetItem	TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.thePetSlot = aPetSlot;
	TempMsg.theItemID = aItemID;

#ifdef UPDATE_PETNAME_PAYITEM
		
	memset( TempMsg.theUpdatePetName, 0 , RP_MAX_PET_NAME );
	
	if( aPetName )
	{
		SAFE_STR_CPY( TempMsg.theUpdatePetName, aPetName, RP_MAX_PET_NAME );
	}

#endif

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsCallPet( BYTE aPetSlot )
{
	///--PRINT_DLG_LOG("Send_CsCallPet");
	SRpCsCallPet TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.thePetSlot = aPetSlot;
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsCancelCallPet()
{
	///--PRINT_DLG_LOG("Send_CsCancelCallPet");
	SRpCsCancelCallPet TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsPetStartMove( DWORD aPetID, DWORD aDestiCell )
{
	SRpCsPetStartMove TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.thePetID = aPetID;
	TempMsg.theDestiCell = aDestiCell;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsPetStop( DWORD aPetID, DWORD aCell )
{
	SRpCsPetStop TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.thePetID = aPetID;
	TempMsg.theCell = aCell;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsPetAttack( DWORD aPetID, DWORD aAttackedID, BYTE aAttackedType )
{
	SRpCsPetAttack TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.thePetID = aPetID;
	TempMsg.theAttackedID = aAttackedID;
	TempMsg.theAttackedType = aAttackedType;
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );
}	

void CNetworkMgr::Send_CsPetSkill( DWORD aPetID, DWORD aTarget, BYTE aTargetType, WORD aSkillCode )
{
	SRpCsPetSkill TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.thePetID = aPetID;
	TempMsg.theTarget = aTarget;
	TempMsg.theTargetType = aTargetType;
	TempMsg.theSkillCode = aSkillCode;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsRideOn()
{
	SRpCsRideOn TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsRideOff()
{
	SRpCsRideOff TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsUseMovePayItem(DWORD aItemID, TCHAR* aPCName)
{
	SRpCsUseMovePayItem TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theItemID = aItemID;
	SAFE_STR_CPY( TempMsg.thePCName, aPCName, RP_MAX_PC_NAME );	
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsAnsMove(TCHAR* aPCName, BYTE aAnswer)
{
	SRpCsAnsMove TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	SAFE_STR_CPY( TempMsg.thePCName, aPCName, RP_MAX_PC_NAME );
	TempMsg.theAnswer = aAnswer;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsDenyWhisper(BYTE aFlag)
{
	SRpCsDenyWhisper TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFlag 	= aFlag;
	
	NetToGame( &TempMsg , sizeof(TempMsg));
}

//------------------------------------------------------------------------------------
//	Colony ����
//------------------------------------------------------------------------------------
void CNetworkMgr::Send_CsReqColonyInfo(WORD aColonyCode)
{
	SRpCsReqColonyInfo TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theColonyCode = aColonyCode;	

	NetToGame( &TempMsg , sizeof(TempMsg));
}

void CNetworkMgr::Send_CsDeclareWar(WORD aColonyCode)
{
	SRpCsDeclareWar TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theColonyCode = aColonyCode;

	NetToGame( &TempMsg , sizeof(TempMsg));
}

void CNetworkMgr::Send_CsCancelWar(WORD aColonyCode)
{
	SRpCsCancelWar TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theColonyCode = aColonyCode;

	NetToGame( &TempMsg , sizeof(TempMsg));
}

//����ȥ�� 
void CNetworkMgr::Send_CsSRpCsMixEldaStone(SRpCsMixEldaStone &TempMsg)
{

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

}

// �������� 
void CNetworkMgr::Send_CsMakeSkillPotion(SBundleItem SKillPotion[MAKESKILLPOTIONNUM] , DWORD CreatePotion )
{
	
	SRpCsMakeSkillPotion TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theCreateSkillPotion = CreatePotion; 
	
	for( int i =0 ; i < MAKESKILLPOTIONNUM ; ++i)
	{
		TempMsg.thePotionElement[i].theItemID = SKillPotion[i].theItemID;
		TempMsg.thePotionElement[i].theBundle = SKillPotion[i].theBundle;
	}

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// �߱� PK ��� ��ȯ
void CNetworkMgr::Send_CsPKMode(BYTE bPKMode)
{
	SRpCsPKMode TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.thePKFlag = bPKMode;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

//  ���� ������ ����
void CNetworkMgr::Send_CsChangeName( DWORD aItemID, char * aNewName )
{
	SRpCsChangeName TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theItemID = aItemID;
	SAFE_STR_CPY( TempMsg.theNewName, aNewName, 50 );	

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

//------------------------------------------------------------------------------------
//  ��ű� ���� �ֹ��� ����
//------------------------------------------------------------------------------------
void CNetworkMgr::Send_CsMakeAddAccessory( DWORD aAddAccUID, BYTE aAddType, DWORD aAddScrollUID )
{
	SRpCsMakeAddAccessory TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theAddAccessoryUID = aAddAccUID;
	TempMsg.theAddType = aAddType;
	TempMsg.theAddScrollUID = aAddScrollUID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}

//------------------------------------------------------------------------------------
//  ���ٽ��� ���� ����
//------------------------------------------------------------------------------------
void CNetworkMgr::Send_CsChangeEldaStone( DWORD aEldaStoneID, DWORD aPayItemID, WORD aSelectedEldaStoneCode )
{
	SRpCsChangeEldaStone TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theEldaStoneID = aEldaStoneID;
	TempMsg.thePayItemID = aPayItemID;
	TempMsg.theSelectedEldaStoneCode = aSelectedEldaStoneCode;

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}

void CNetworkMgr::Send_SRpCsMakeEldastone(int theEldastone , SBundleItem EldastonePowder , SBundleItem Proima )
{
	SRpCsMakeEldastone TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theEldastonePowder = EldastonePowder;
	TempMsg.theProima = Proima;
	TempMsg.theEldastone = theEldastone;
	

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}


void CNetworkMgr::Send_CsHackShield(BYTE Flag , unsigned char *Msg)
{
	SRpCsHackShield TempMsg;
	TempMsg.theFlag = Flag;

	if(Flag == CsGUIDMSG ) 
	{
		TempMsg.theMsgHeader.theSize += 340;
		memcpy(&TempMsg.theMsg , Msg , 340);
		NetToGame( &TempMsg , sizeof( TempMsg ) );
	}
	else 
	{
		TempMsg.theMsgHeader.theSize += 160;
		memcpy(&TempMsg.theMsg , Msg , 160 );
		NetToGame( &TempMsg , sizeof( TempMsg ) - 180  );
	}

}

void CNetworkMgr::Send_CsGameGuard(DWORD Flag , PBYTE Msg , int size)
{
	
	SRpCsGameGuard TempMsg; 
		
	TempMsg.theResult = Flag; 

	if( Flag == n_GG_Normal )
	{
		TempMsg.theMsgHeader.theSize = sizeof(_SRpCsGameGuard) - sizeof(TempMsg.theGameGuardMsg);
		TempMsg.theMsgHeader.theSize += size; 
		memcpy(&TempMsg.theGameGuardMsg  , Msg  , size );
	
	}else if( Flag == 1 )
	{
		TempMsg.theMsgHeader.theSize = sizeof(_SRpCsGameGuard) - sizeof(TempMsg.theGameGuardMsg);
		TempMsg.theMsgHeader.theSize += 512; 
		memcpy(&TempMsg.theGameGuardMsg  , Msg  , sizeof(TempMsg.theGameGuardMsg) );
	}
	else if(Flag == -1)
	{
		TempMsg.theMsgHeader.theSize = sizeof(_SRpCsGameGuard) - sizeof(TempMsg.theGameGuardMsg);
		TempMsg.theMsgHeader.theSize += size; 
		memcpy(&TempMsg.theGameGuardMsg  , Msg  , sizeof(TempMsg.theGameGuardMsg) );
	}
	
	NetToGame( &TempMsg , TempMsg.theMsgHeader.theSize );
}

//��ȭ â
void CNetworkMgr::Send_SRpCsUseStrengthstone(DWORD theStrengthstoneID , DWORD theStrengthItemID , DWORD thePayItem[3])
{
	SRpCsUseStrengthstone TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theStrengthstoneID = theStrengthstoneID;
	TempMsg.theStrengthItemID = theStrengthItemID;

	for( int i = 0 ; i < 3 ; ++i)
	{
		TempMsg.thePayItem[i] =	thePayItem[i];
	}




	NetToGame( &TempMsg , sizeof(TempMsg) );

}

//���� ����  ����
void CNetworkMgr::Send_SRpCsMakeEssence(int	theEssenceCode  , int theCount)
{
//	WORD				theEssence;			// ������ ���� ���� �ڵ�
//	INT					theCount;			// ���� ����
	SRpCsMakeEssence TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theEssence = theEssenceCode;
	TempMsg.theCount = theCount ; 
	NetToGame( &TempMsg , sizeof(TempMsg) );

}

//��ȭ �ֹ��� ����
void CNetworkMgr::Send_SRpCsMakeStrengthstone(int theStrengthstone , SBundleItem	theStrengthstoneMaterial[5] )
{
	SRpCsMakeStrengthstone TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theStrengthstone = theStrengthstone;
	
	memcpy( TempMsg.theStrengthstoneMaterial , theStrengthstoneMaterial , sizeof(SBundleItem)*5 );

	NetToGame( &TempMsg , sizeof(TempMsg));

}

#ifdef TAIWAN30MINEVENT
void CNetworkMgr::Send_SRpCsTaiwan30minEvent()
{
	SRpCsTaiwan30minEvent TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof(TempMsg) );

}

void CNetworkMgr::Send_SRpCsTaiwan30minEventStart()
{
	SRpCsTaiwan30minEventStart TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof(TempMsg));

}
#endif //TAIWAN30MINEVENT


/*

const int MAKESKILLPOTIONNUM = 3;
typedef struct _SRpCsMakeSkillPotion
{
	_SRpCsMakeSkillPotion()
	{
		theMsgHeader.theSize = sizeof( _SRpCsMakeSkillPotion );
		theMsgHeader.theProtocolType = 0;
		theMsgHeader.theProtocol = n_RpCsMakeSkillPotion;
		theMsgHeader.thePCID = 0;
		theCreateSkillPotion = 0;
		ZeroMemory( thePotionElement, sizeof( thePotionElement ) );
	}
	SRpHeader		theMsgHeader;
	DWORD			theCreateSkillPotion;
	SBundleItem		thePotionElement[ MAKESKILLPOTIONNUM ];
}SRpCsMakeSkillPotion, * SRpCsMakeSkillPotionPtr;

typedef struct _SRpScMakeSkillPotion
{
	_SRpScMakeSkillPotion()
	{
		theMsgHeader.theSize = sizeof( _SRpScMakeSkillPotion );
		theMsgHeader.theProtocolType = 0;
		theMsgHeader.theProtocol = n_RpScMakeSkillPotion;
		theMsgHeader.thePCID = 0;
		theResult			 = 0;
		thePCMoney			 = 0;
		ZeroMemory( theDeletePotionElement, sizeof( theDeletePotionElement ) );
	}
	SRpHeader		theMsgHeader;
	URpdItem		theSkillPotion;
	DWORD			thePCMoney;
	BYTE			theResult;
	SBundleItem		theDeletePotionElement[ MAKESKILLPOTIONNUM ];
}SRpScMakeSkillPotion, * SRpScMakeSkillPotionPtr;



  */


