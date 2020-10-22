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
	
	memcpy( theNetPackBuf, aMsgPtr, aSize ); /// 원래 메시지 사이즈로 복사. 
	theNetPack2Login.Encrypt( theNetPackBuf, newSize ); /// padding size 로 암호화 


	/// 암호화하지 않은 사이즈 정보 하나를 버퍼에 먼저 넣고 	
	theLoginSocketPtr->PushSendQ( &newSize, sizeof( newSize ) );
	theLoginSocketPtr->PushSendQ( theNetPackBuf, newSize );	/// padding size 째로 보낸다. 
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


	memcpy( theNetPackBuf, aMsgPtr, aSize ); /// 원래 메시지 사이즈로 복사. 
	theNetPack2Game.Encrypt( theNetPackBuf, newSize ); /// padding size 로 암호화 


	/// 암호화하지 않은 사이즈 정보 하나를 버퍼에 먼저 넣고 	
	theGameSocketPtr->PushSendQ( &newSize, sizeof( newSize ) );
	theGameSocketPtr->PushSendQ( theNetPackBuf, newSize );	/// padding size 째로 보낸다. 
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
///-- 결정된 Channel 로 접속을 시도한다.
///-- 현재 사용하지 않는 함수
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
		
		Send_RpCsGameLogin();	/// GameServer에 접속하자마자 C2S_GameLogin 전송 
		
		return TRUE;
	}
	
	return FALSE;
}
///---------------------------------------------------------------------------
///-- ConnectToGameServer
///---------------------------------------------------------------------------
///-- 결정된 Channel 로 접속을 시도한다.
BOOL	CNetworkMgr::ConnectToGameServer( int aListIndex )
{
	
	SUPERASSERT( aListIndex >= 0 || aListIndex < theServerSetNum );
	
	if( CreateAndConnect(	theGameSocketPtr, theServerSetData[aListIndex].theIP, 
		theServerSetData[aListIndex].thePort ) == TRUE )
	{
		theNetPack2Game.InitializeIdx();

		theConnectState = nInGameServer;
	
		Send_RpCsGameLogin();	/// GameServer에 접속하자마자 C2S_GameLogin 전송 
		
		return TRUE;
	}
	
	return FALSE;
}

/// 이걸 메인 Loop에서 매 frame 마다 호출합니다.
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
			PRINT_DLG_LOG( "게임 SOCKET_ERROR 발생" );		
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
	
	if( theConnectState == nInGameServer )		/// 정상 
	{
		PRINT_DLG_LOG( "!로그인서버와 접속 종료(정상)" );
		RLGS( "!로그인서버와 접속 종료(정상)" );	
		UCHAR uMsg[128]="";


	}
	else	/// 에러!
	{
		PRINT_DLG_LOG( "!로그인서버와 접속 종료!!!!!!!!!" );
		RLGS( "!로그인서버와 접속 종료!!!!!!!!!" );		
		CleanUpSocket( theGameSocketPtr );	/// Game Socket도 죽인다. 
		theConnectState = nNoConnect;
		
		Proc_OnSocketClose();		
	}
}

void	CNetworkMgr::OnGameSocketClose()
{
	PRINT_DLG_LOG( "!게임서버와 접속 종료!!!!!!!!!" );
	RLGS( "!게임서버와 접속 종료!!!!!!!!!" );	
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
	//	aSocketPtrRef->SetSockTCPNoDelay(); 		///  client에선 default로 쓴다. 
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
	
	WORD			packetSize = 0;	/// 암호화되지 않은 패킷 사이즈 정보는 2 Byte 크기 

	for(;;)
	{
		msg = aSocketPtr->theMsgQRecv.GetBuffer();
		size = aSocketPtr->theMsgQRecv.GetSize();
		//size = 0;
		
#ifdef USE_ROCKCRYPTO
		if( size == 0) 
		{
			break;				/// 버퍼에 2 Byte도 안들어있넹 
		}
		
		if( size < sizeof( WORD ) )
		{
			break;				/// 버퍼에 2 Byte도 안들어있넹 
		}
		
 

		memcpy( &packetSize, msg, sizeof( WORD ) );	/// 암호화되지 않은 size 정보를 읽고...
		
		if( packetSize > MAX_PACKET_SIZE * 2 )	/// 사이즈 정보 오류 체크 
		{
			///--SEND_DEBUG_LOG("MsgProcLS Invalid Packet Header : -> size info is too big(%d)", packetSize );
			
			RLG1( "Invalid Packet Header : -> size info is too big(%d)", packetSize );
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}

		if( size < int( packetSize + sizeof( WORD ) ) ) 
		{
			break;	/// 실제 메시지 크기 + sizeof( size 정보 ) 
		}

		/// 충분히 큰 크기가 들어왔으므로 복호화 작업 
		/// 먼저 sizeof( size 정보 ) pop 
		aSocketPtr->PopRecvQ( sizeof( packetSize ) );

		/// 실제 메시지 포인터 얻어와서 
		msg = aSocketPtr->theMsgQRecv.GetBuffer();

		/// 복호화 
		theNetPack2Login.Decrypt( msg, packetSize );

		/// 헤더 캐스팅 : 새로운 CsHeader 
		header = static_cast< SRpHeaderPtr > ( msg );

		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// 사이즈 정보 오류 체크 
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
		/// 헤더 확인 
		if ( size < sizeof( SRpHeader ) ) break;
		header = ( SRpHeader* ) msg;
		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// 사이즈 정보 오류 체크 
		{
			RLG2( "Invalid Packet Header : %d -> size info is too big(%d)", header->theProtocol , header->theSize );
			
			///-- SEND_DEBUG_LOG( "MsgProcLS Invalid Packet Header : %d -> size info is too big(%d)" , header->theProtocol , header->theSize );
			
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}
		
		/// 모두 도착했는가?
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
			// Other Game Protocols : 게임 프로토콜은 상속받은 클래스에서 처리 
			//-----------------------------------------------------------------
		default :
///			Proc_RpScProtocols( msg );											
			break;
		}
		
		/// sooree net remodel 
		aSocketPtr->PopRecvQ( packetSize );
		
	} // for( size )
	
	// Proc_RpScPrepareLogin 시에 소켓을 끊기 위해서...
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
	
	WORD			packetSize = 0;	/// 암호화되지 않은 패킷 사이즈 정보는 2 Byte 크기 

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
			break;				/// 버퍼에 2 Byte도 안들어있넹 
		}
		
		memcpy( &packetSize, msg, sizeof( WORD ) );	/// 암호화되지 않은 size 정보를 읽고...
		
		if( packetSize > MAX_PACKET_SIZE * 2 )	/// 사이즈 정보 오류 체크 
		{
			RLG1( "Invalid Packet Header : -> size info is too big(%d)", packetSize );
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}

		if( size < int( packetSize + sizeof( WORD ) ) ) 
		{
			break;	/// 실제 메시지 크기 + sizeof( size 정보 ) 
		}

		/// 충분히 큰 크기가 들어왔으므로 복호화 작업 
		/// 먼저 sizeof( size 정보 ) pop 
		aSocketPtr->PopRecvQ( sizeof( packetSize ) );

		/// 실제 메시지 포인터 얻어와서 
		msg = aSocketPtr->theMsgQRecv.GetBuffer();

		/// 복호화 
		theNetPack2Game.Decrypt( msg, packetSize );

		/// 헤더 캐스팅 : 새로운 CsHeader 
		header = static_cast< SRpHeaderPtr > ( msg );

		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// 사이즈 정보 오류 체크 
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
		/// 헤더 확인 
		if ( size < sizeof( SRpHeader ) ) break;
		header = ( SRpHeader* ) msg;
		if( header->theSize > MAX_PACKET_SIZE * 2 )	/// 사이즈 정보 오류 체크 
		{
			RLG2( "Invalid Packet Header : %d -> size info is too big(%d)", header->theProtocol , header->theSize );
			aSocketPtr->PopRecvQ( sizeof( SRpHeader ) );
			continue;
		}
		
		/// 모두 도착했는가?
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
			// Other Game Protocols : 게임 프로토콜은 상속받은 클래스에서 처리 
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
		//PRINT_DLG_LOG( "처리 패킷수 - %d 처리시간  - %f초", nProcessPCount, 
		//	fDelTime );
	}

#endif
	
	// Proc_RpScPrepareLogin 시에 소켓을 끊기 위해서...
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
	//PigWing 만 이프로토콜을 쓴다.  
	memset(theAccountName ,0,sizeof(theAccountName));
	memset(theChangeAccountName, 0, sizeof(theChangeAccountName));

	SAFE_STR_CPY( theAccountName, aMsg->theAccountID, 65 );	/// 기억해야 합니다.
	SAFE_STR_CPY( theChangeAccountName , aMsg->theChangedAccountID, 65 );
	
	if( n_Kr_Pigwing == g_RockClient.m_GameTypeInfo.theFrom  || 
		n_Kr_MyClub == g_RockClient.m_GameTypeInfo.theFrom )
	{
		memset(g_RockClient.m_UserSysID , 0 , sizeof(g_RockClient.m_UserSysID));
		SAFE_STR_CPY( g_RockClient.m_UserSysID, theChangeAccountName, 64 );	/// 기억해야 합니다.
	}
}

void	CNetworkMgr::Proc_RpScServerSetList( SRpScServerSetList* aMsg )
{
	char userName[128] = "";
	
	memset( theServerSetData , 0 , sizeof( SServerSetData ) * 64 );
	memset(g_RockClient.m_SysID , 0 , sizeof(g_RockClient.m_SysID));	
	
	thePCID = aMsg->theMsgHeader.thePCID;	/// 여기서 꼭 기억해야 한다. 
	SAFE_STR_CPY( theAccountName, aMsg->theAccountID, 65 );	/// 기억해야 합니다.
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
			///-- 드림라인은 check값으로 암호화된 유저아이디를 보내준다.
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
///-- 서버군을 선택한다.
/*
void	CNetworkMgr::Proc_RpScWorldList( SRpScWorldList* aMsg )
{
	memset( theWorldData , 0 , sizeof( SWorldData ) * 64 );

	thePCID = aMsg->theMsgHeader.thePCID;	/// 여기서 꼭 기억해야 한다. 
	theWorldNum = aMsg->theWorldNum;
	memcpy( theWorldData, aMsg->theWorldList, sizeof( SWorldData ) * theWorldNum );	
}*/
///---------------------------------------------------------------------------
///-- Proc_RpScChannelList
///---------------------------------------------------------------------------
///-- 채널을 선택한다.
/*
void	CNetworkMgr::Proc_RpScChannelList( SRpScChannelList * aMsg )
{
	memset( theChannelData , 0 , sizeof( SChannelData ) * 64 );

	thePCID = aMsg->theMsgHeader.thePCID;	/// 여기서 꼭 기억해야 한다. 
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
	/// 원래 이걸 받으면 클라이언트는 로딩 등 게임 월드 진입을 위한 최종 준비를 마무리해야 한다.
	/// 준비 완료 후, C2S_PCLogin 전송 
	return true;
}

void	CNetworkMgr::Proc_RpScPCLogin( SRpScPCLogin* aMsg )
{
	/// 로그인 완료!
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
	  
	/// 한국 버전 중에 공식 사이트나 게임엔젤 사이트에서 온 것은 대문자화하지 않고 보낸다.
	_strupr( aAccountName );
	strcpy( theAccountName, aAccountName );	/// 기억해야 합니다.

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
	
// 피그윙은 로그인 서버에서 SysID 얻어와서  채널로그인 할시 theAccountName 으로 SysID 로내준다.
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
	msg_CsAttack.theComboType = aComboType;				// 콤보 타입 - 일반공격 : 0  ex> c_ComboXKey1, ....
	msg_CsAttack.theComboCount = aComboCount;			// 콤보 횟수 - 일반공격 : 0
	msg_CsAttack.theComboStatus	= aComboStatus;			// 콤보 상태 - 완성, 실패 등...
	msg_CsAttack.theComboSeq = aComboSeq;
	
	NetToGame( &msg_CsAttack, sizeof( msg_CsAttack ) );
	
	// 콤보가 아닌 평타를 연속해서 보내는것은 버그 플레이로 단정!!! 
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

// 스킬 사용 요청
void CNetworkMgr::Send_RpCsSkill( DWORD aSkillID, DWORD aTarget, BYTE aTargetType, WORD aSkillCode )
{
	// 개인 상점시 스킬 막기
	if( aSkillCode == c_SkillCode_UserSellStore )
	{
		if( g_Pc.GetPlayer()->m_CharAlpha.GetState() == CHAR_HIDE )
		{
			SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_USERSTORE_SELL_NOT ) );

			return;
		}

	}	
		
	///--PRINT_DLG_LOG("스킬 보냄 %d", aSkillCode);
	
	SRpCsSkill msg_CsSkill;
	msg_CsSkill.theMsgHeader.thePCID = aSkillID;
	
	msg_CsSkill.theTarget = aTarget;			// target은 셀값이 될수도 있고.. 캐릭터가 될수도 있다.
	msg_CsSkill.theTargetType = aTargetType;	// n_PC, n_NPC, n_Cell
	msg_CsSkill.theSkillCode = aSkillCode;

	// 스킬 사용후 바로 일반 공격을 연속해서 보내는것은 버그 플레이로 단정!!! 
	g_RockClient.m_PickAttack_DelayTime = g_nowTime;

	NetToGame( &msg_CsSkill, sizeof( msg_CsSkill ) );
	return;
} 

void CNetworkMgr::Send_RpCsSpelling( BYTE aChantCount )
{
	SRpCsSpelling msg_CsSpelling;
	msg_CsSpelling.theMsgHeader.thePCID = thePCID;
	
	msg_CsSpelling.theSpellingCount = aChantCount;			// 성공 영창 수
	
	NetToGame( &msg_CsSpelling, sizeof( msg_CsSpelling ) );
	
	
	return;
}

// 아처 준비 동작 : target -> Cell or PC or NPC | n_PC, n_NPC, n_Cell
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
	//이거 스태틱으로 잡아야함 - 센드메시지동작이 끝나기전에 SRpCsSay 지역변수가
	//사라질수 있음 채팅처럼 긴 데이터를 처리해야할 경우
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
	//이거 스태틱으로 잡아야함
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

//GM이란 글자가 있으면...
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
	
	msgCsPoint.thePower		= aPower;		// 파워
	msgCsPoint.theVital		= aVital;		// 체력	
	msgCsPoint.theSympathy	= aSympathy;	// 교감
	msgCsPoint.theInt		= aInt;			// 지능	
	msgCsPoint.theStamina	= aStamina;		// 스태미너 	
	msgCsPoint.theDex		= aDex;			// 민첩 		
	
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
#ifdef DECO_RENEWAL_MJH //직업선택
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

  // 캐릭터 삭제 유예기간내에 삭제 취소 요청 메시지
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


// .., 명상 등..  지속스킬 종료가 예약되지 않은 스킬. 클라이언트가 종료를 요청함.
void CNetworkMgr::Send_RpCsKeepUpSkillEnd( WORD aSkillCode )
{
	SRpCsKeepUpSkillEnd msgCsKeepUpSkillEnd;
	msgCsKeepUpSkillEnd.theMsgHeader.thePCID = thePCID;
	
	msgCsKeepUpSkillEnd.theSkillCode = aSkillCode;		 // 스킬코드 USHRT_MAX : 적용된 모든 스킬 해제 theEffectType 무시
	
	NetToGame( &msgCsKeepUpSkillEnd, sizeof( msgCsKeepUpSkillEnd ) );
	
}

// 퀘스트 포기 프로토콜
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

//by simwoosung - 게임종료, 로그오프, 귀환 하기전에 딜레이동작을 서버에 알려줌
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
// 교환 시스템 관련 프로토콜 
//============================================================================================================
//============================================================================================================
// 교환 기능 켜기, 꺼기 
void CNetworkMgr::Send_RpCsExchange( BYTE aIsOpen ) 
{
	SRpCsExchange msgCsExchange;
	msgCsExchange.theMsgHeader.thePCID = thePCID;
	msgCsExchange.theExchange = aIsOpen;
	
	NetToGame( &msgCsExchange, sizeof( msgCsExchange ) );
	
	return;
}

// A가 교환 요청시 메시지 
void CNetworkMgr::Send_RpCsAskExchange( DWORD aPCID ) 
{
	SRpCsAskExchange msgCsAskExchange;
	msgCsAskExchange.theMsgHeader.thePCID = thePCID;
	msgCsAskExchange.thePCID = aPCID;		// 교환을 하고자 하는 상대ID
	
	NetToGame( &msgCsAskExchange, sizeof( msgCsAskExchange ) );
	
	return;
}


// B의 응답 메시지
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
// A,B의 올리기 할 경우 아이템 리스트
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

	//서버에 보내기전에 다시한번 검사한다..
	for(i = 0 ; i < list_num ; ++i)
	{
		if(msgCsSuggestExItem.theItemList[i].theItemUID)		
		{
			TempdItemUID = msgCsSuggestExItem.theItemList[i].theItemUID;
			Num = 0;
		
			for( j = 0;  j <list_num ; ++j)
			{
				//2개이상이면 핵유져..
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

// 교환승락 버튼 클릭시 전송
void CNetworkMgr::Send_RpCsAgreeExchange() 
{
	SRpCsAgreeExchange msgCsAgreeExchange;
	msgCsAgreeExchange.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msgCsAgreeExchange, sizeof( msgCsAgreeExchange ) );
	
	return;
}

// 교환취소 버튼 클릭시 전송
void CNetworkMgr::Send_RpCsCancelExchange() 
{
	SRpCsCancelExchange msgCsCancelExchange;
	msgCsCancelExchange.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msgCsCancelExchange, sizeof( msgCsCancelExchange ) );
	
	return;
}

//============================================================================================================
//============================================================================================================
// 보관 시스템 관련 프로토콜 
//============================================================================================================
//============================================================================================================
// 보관창을 닫을때 서버에 전송 (DB에 저장한다.)
void CNetworkMgr::Send_RpCsCloseBank() 
{
	SRpCsCloseBank msgCsCloseBank;
	msgCsCloseBank.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &msgCsCloseBank, sizeof( msgCsCloseBank ) );
	
	return;
}

// 아이템을 보관한다. 
void CNetworkMgr::Send_RpCsDeposit( DWORD aItemID, BYTE aBundle, BYTE aBankSlot ) 
{
	//	DWORD			theItemID;
	//	BYTE			theBundle;			// 소모품일 경우
	//	BYTE			theBankSlot;		// 아이템이 은행에 위치할 슬롯 번호(0 ~ 124)
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

// 아이템을 찾는다.
void CNetworkMgr::Send_RpCsDraw( DWORD aBankItemID, BYTE aBundle ) 
{
	//	BYTE			theBankItemID;		// 찾을 아이템의 ID
	//	BYTE			theBundle;			// 소모품의 경우 찾을 번들수 
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

// Eld를 은행에 맡기다.
void CNetworkMgr::Send_RpCsDepositEld( DWORD aEld ) 
{
	//	DWORD			theEld;
	
	SRpCsDepositEld msgCsDepositEld;
	msgCsDepositEld.theMsgHeader.thePCID = thePCID;
	msgCsDepositEld.theEld = aEld;
	
	NetToGame( &msgCsDepositEld, sizeof( msgCsDepositEld ) );
	
	return;
}

// 앨드를 찾는다. 
void CNetworkMgr::Send_RpCsDrawEld( DWORD aEld ) 
{
	//	DWORD			theEld;
	
	SRpCsDrawEld msgCsDrawEld;
	msgCsDrawEld.theMsgHeader.thePCID = thePCID;
	msgCsDrawEld.theEld = aEld;
	
	NetToGame( &msgCsDrawEld, sizeof( msgCsDrawEld ) );
	
	return;
}

// 보관창에서의 아이템 이동 
void CNetworkMgr::Send_RpCsWHChangeSlot( DWORD aBankItemID, BYTE aSlot ) 
{
	//	BYTE			theBankItemID;		// 옮기고자 하는 아이템의 현재 슬롯번호 
	//	BYTE			theSlot;			// 옮길 위치 
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
// 길드창고 시스템 관련 프로토콜 
//============================================================================================================
//============================================================================================================
// 아이템을 보관한다. 
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
	//	BYTE			theBundle;			// 소모품일 경우
	//	BYTE			theBankSlot;		// 아이템이 은행에 위치할 슬롯 번호(0 ~ 124)
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

// 길드창고에서 아이템을 찾는다.
void CNetworkMgr::Send_RpCsDrawFrontier( DWORD aBankItemID, BYTE aBundle ) 
{
	//	BYTE			theBankItemID;		// 찾을 아이템의 ID
	//	BYTE			theBundle;			// 소모품의 경우 찾을 번들수 
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

// 길드창고에서의 아이템 이동 
void CNetworkMgr::Send_RpCsChangeFrontierSlot( DWORD aBankItemID, BYTE aSlot ) 
{
	//	BYTE			theBankItemID;		// 옮기고자 하는 아이템의 현재 슬롯번호 
	//	BYTE			theSlot;			// 옮길 위치 
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
///-- 아이템 개조 시 아이템과 엘다 ID 전송
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
///-- Game 진행시 한 Column 완성시 정보 전송
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
///-- 아이템 제조
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
// ... 관련 프로토콜 
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
// 어떤 것을 쓸지는 서버에서 결정...
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
// 사교 동작 관련
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
// PVP 시스템 관련 
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


// 경기 타입 선택
void CNetworkMgr::Send_CsSelectFightGameType( BYTE aGameType )
{
	SRpCsSelectFightGameType	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFightGameType = aGameType;			// n_ReguralGame (공식전), n_IrreguralGame (비공식전) 

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}
/*
// 등급 선택
void CNetworkMgr::Send_CsSelectFightGrade( BYTE aGradeType )
{	
	SRpCsSelectFightGrade	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFightGrade = aGradeType;				// 0, 1... 차례로...

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
	
}
*/

// 방만들기 ( 대전방 생성 )
void CNetworkMgr::Send_CsMakeFightRoom( _SFightRoomInfo aRoomInfo )
{
	SRpCsMakeFightRoom	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theInfo = aRoomInfo;			

	NetToGame( &TempMsg , sizeof( TempMsg ) );
	
	return;
}

// 게임 참가하기
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

// 게임방에서 나가기 ( 나가기 및 강퇴 )
void CNetworkMgr::Send_CsOutFightRoom( DWORD aKickID )
{
	SRpCsOutFightRoom	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.thePCID = aKickID;				

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// 대전정보 변경( 방장만 가능)
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

// 팀 선택 
void CNetworkMgr::Send_CsSelectTeam( BYTE aSelectTeam )
{
	SRpCsSelectTeam	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theTeam = aSelectTeam;				

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// 대전 준비 버튼 클릭!! or 취소!! 
void CNetworkMgr::Send_CsFightReady( BYTE aFlag )
{
	SRpCsFightReady	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFlag = aFlag;				// n_Success : 준비   n_Fail : 취소 	

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// 게임 시작 
void CNetworkMgr::Send_CsFightStart()
{
	SRpCsFightStart	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// 대전맵 로딩 완료
void CNetworkMgr::Send_CsFightReadyWarp()
{
	SRpCsFightReadyWarp	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// 대전대기실 나가기 ( 게임으로 돌아가기 )
void CNetworkMgr::Send_CsOutWaitingRoom()
{
	SRpCsOutWaitingRoom	TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

//..........................................................................................................
// 개인 상점 관련
//..........................................................................................................

// 개인 상점 요청
void CNetworkMgr::Send_CsOpenPCShopWindow()
{
	SRpCsOpenPCShopWindow TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// 개인 상점 열기
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
	// 패킷 가변 사이즈 계산
	//......................................................................................................
	WORD DataSize = sizeof( SRpCsOpenPCShop );

	DataSize = DataSize - ( sizeof( _ShopInfo ) * ( 50 - Count ) );

	TempMsg.theMsgHeader.theSize = DataSize;

	NetToGame( &TempMsg , DataSize );
}

// 개인 상점 닫기
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

// 개인 상점 보기
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



// 개인 상점 사기
void CNetworkMgr::Send_CsBuyInPCShop(DWORD ShopPCID,DWORD ItemUID,BYTE Count)
{
	SRpCsBuyInPCShop TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.thePCID              = ShopPCID;
	TempMsg.theItemUID           = ItemUID;
	TempMsg.theBundle            = Count;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}


// 구매한 유료아이템 리스트 열기 
void CNetworkMgr::Send_CsOpenPayShop()
{
	SRpCsOpenPayShop TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

	return;
}

// 구매한 유료아이템 인벤으로 옮기기 
void CNetworkMgr::Send_CsGetPayItem( DWORD aPayItemID )
{
	SRpCsGetPayItem TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.thePayItemID = aPayItemID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}


// 창단 신청 
#ifdef DECO_RENEWAL_GUILD_MJH
void CNetworkMgr::Send_CsReqMakeFrontier( TCHAR* aFrontierName )
{
//	char		theFrontierName[RP_MAX_FRONTIER_NAME];	// 생성할 프론티어명

	SRpCsReqMakeFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	SAFE_STR_CPY( TempMsg.theFrontierName, aFrontierName, RP_MAX_FRONTIER_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );	
}
// 공지사항 작성
void CNetworkMgr::Send_CsChangeFrontierNotice( TCHAR* aFrontierNotice , DWORD aFrontierID)
{
//  char       theNewNotice  // 길드 공지사항   

	SRpCsChangeFrontierNotice TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theFrontierID = aFrontierID;

	SAFE_STR_CPY( TempMsg.theNewNotice, aFrontierNotice, 50 );

	NetToGame( &TempMsg , sizeof( TempMsg ) );	

}
void CNetworkMgr::Send_CsChangeFrontierMemberIntro( TCHAR* aMemberIntro )
{
//  char       theNewNotice  // 길드 자기소개   

	SRpCsChangeFrontierMemberIntro TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;

	SAFE_STR_CPY( TempMsg.theNewIntroduce, aMemberIntro, 30  );

	NetToGame( &TempMsg , sizeof( TempMsg ) );	

}
//길드장 위임
void CNetworkMgr::Send_CsChangeFrontierMaster( TCHAR* aNewMaster )
{
//  DWORD       aFrontierID      // 프론티어아이디
//  char        aNewMaster       // 새로운 마스터명

	SRpCsChangeFrontierMaster TempMsg;
	
	TempMsg.theMsgHeader.thePCID = thePCID;
	SAFE_STR_CPY( TempMsg.theNewFrontierMasterName, aNewMaster, RP_MAX_PC_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsMsngrProposeJoinFrontier( DWORD aReceiverID , DWORD aEld , _SExItem aItem )
{
//	DWORD		theReceiverID;		// 제안받는 유저 ID

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
//	char		theFrontierName[RP_MAX_FRONTIER_NAME];	// 생성할 프론티어명
//	char		theMemberName[3][RP_MAX_PC_NAME];		// 창단맴버 3명

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
//	DWORD		theReceiverID;		// 제안받는 유저 ID

	SRpCsMsngrProposeJoinFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theReceiverID = aReceiverID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
#endif // DECO_RENEWAL_GUILD_MJH

// 가입제안에 대한 답변 전송 
void CNetworkMgr::Send_CsMsngrAnswerJoinFrontier( BYTE aAnswer )
{
//	DWORD		theReceiverID;		// 제안받는 유저 ID

	SRpCsMsngrAnswerJoinFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theAnswer = aAnswer;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// 탈퇴 
// 본인이 직접 탈퇴가능, Major 이상급 강퇴 가능 
void CNetworkMgr::Send_CsMsngrLeaveFrontier( TCHAR* aPCName )
{
//	char		thePCName[RP_MAX_PC_NAME];	// 탈퇴대상자명 (로그아웃중인 유저를 탈퇴시킬수도 있기 때문)

	SRpCsMsngrLeaveFrontier TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	SAFE_STR_CPY( TempMsg.thePCName, aPCName, RP_MAX_PC_NAME );

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}


// 전체보기창에서 맴버의 호칭과 등급 변경 요청 
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

// 엠블램은 Appear시 버전정보를 클라이언트에 알려주고
// 클라이언트의 해당 프론티어의 엠블램의 버전이 낮을 경우 서버에 새 정보를 요청하고 받아서 갱신한다. 
// 엠블램 갱신 

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

// 엠블램 요청 (클라이언트에 없는 엠블렘의 경우)
void CNetworkMgr::Send_CsMsngrEmblem( DWORD aFrontierID )
{
//	DWORD		theFrontierID;

	SRpCsMsngrEmblem TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.theFrontierID = aFrontierID; 

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// 프론티어 엘드 납부
void CNetworkMgr::Send_CsPayDues( DWORD aDues )
{
//	DWORD		theDues;		// 프론티어 금액 내에서 분배할 금액 

	SRpCsPayDues TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.theDues = aDues; 

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// 프론티어 엘드 분배 
void CNetworkMgr::Send_CsMsngrDivideDues( DWORD aDues )
{
//	DWORD		theDues;		// 프론티어 금액 내에서 분배할 금액 

	SRpCsMsngrDivideDues TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	TempMsg.theDues = aDues; 

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

void CNetworkMgr::Send_CsMsngrFrontierSay( TCHAR* aStr )
{
//	char		theStr[128]; 			/// 대화 내용 

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

// 신수생성처리 요청 
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

// 신수 소환 요청 
void CNetworkMgr::Send_CsMsngrCallFrontierPet()
{
	SRpCsMsngrCallFrontierPet TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// 프론티어 연합 신청 (마스터권한)
void CNetworkMgr::Send_CsMsngrReqCombine(DWORD aProposedPCID)
{
	SRpCsMsngrReqCombine TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theProposedPCID = aProposedPCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

//연합프론티어 신청에 대한 답변
void CNetworkMgr::Send_CsMsngrAnsCombine(DWORD aProposerPCID, BYTE aAnswer)
{
	SRpCsMsngrAnsCombine TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theProposerPCID = aProposerPCID;
	TempMsg.theAnswer = aAnswer;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// 연합 프론티어 해제 요청
void CNetworkMgr::Send_CsMsngrSeperateF()
{
	SRpCsMsngrSeperateF TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
#ifdef DECO_RENEWAL_CORPORATION_MJH
// 군단 가입 신청
void CNetworkMgr::Send_CsMsngrCorpJoin( BYTE aCorps )
{
	SRpCsJoinCorps TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theCorps = aCorps;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
// 군단 탈퇴 신청
void CNetworkMgr::Send_CsMsngrCorpLeave()
{
	SRpCsLeaveCorps TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}
#endif // DECO_RENEWAL_CORPORATION_MJH
// 신수 소환 해제 요청 
void CNetworkMgr::Send_CsMsngrCancelCallFrontierPet()
{
	SRpCsMsngrCancelCallFrontierPet TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	
	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

// 메가폰 유료아이템용 
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
// Pet 관련
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
//	Colony 관련
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

//엘다혼합 
void CNetworkMgr::Send_CsSRpCsMixEldaStone(SRpCsMixEldaStone &TempMsg)
{

	TempMsg.theMsgHeader.thePCID = thePCID;

	NetToGame( &TempMsg , sizeof( TempMsg ) );

}

// 포션제조 
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

// 중국 PK 모드 전환
void CNetworkMgr::Send_CsPKMode(BYTE bPKMode)
{
	SRpCsPKMode TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.thePKFlag = bPKMode;

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

//  유료 아이템 관련
void CNetworkMgr::Send_CsChangeName( DWORD aItemID, char * aNewName )
{
	SRpCsChangeName TempMsg;
	TempMsg.theMsgHeader.thePCID = thePCID;

	TempMsg.theItemID = aItemID;
	SAFE_STR_CPY( TempMsg.theNewName, aNewName, 50 );	

	NetToGame( &TempMsg , sizeof( TempMsg ) );
}

//------------------------------------------------------------------------------------
//  장신구 가공 주문서 관련
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
//  엘다스톤 변경 관련
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

//강화 창
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

//진주 정기  제조
void CNetworkMgr::Send_SRpCsMakeEssence(int	theEssenceCode  , int theCount)
{
//	WORD				theEssence;			// 생성될 진주 정기 코드
//	INT					theCount;			// 생성 갯수
	SRpCsMakeEssence TempMsg;

	TempMsg.theMsgHeader.thePCID = thePCID;
	TempMsg.theEssence = theEssenceCode;
	TempMsg.theCount = theCount ; 
	NetToGame( &TempMsg , sizeof(TempMsg) );

}

//강화 주문석 제조
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


