// PacketTransLogDlg.cpp: implementation of the CPacketTransLogDlg class.
//////////////////////////////////////////////////////////////////////

#include "RockPCH.h"

#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#include "RockClient.h"
#include "PacketTransLogDlg.h"
#include "stringmanager.h"

static CPacketTransLogDlg* g_CPacketTransLogDlg = NULL;
WNDPROC OldEditProc2;

LRESULT CALLBACK	ListLogProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	if( hWnd == g_CPacketTransLogDlg->GetLogHandle() )
	{
		switch (iMessage) 
		{
		  case WM_KEYDOWN:
			  break;
		}
	
	}	
	return CallWindowProc(OldEditProc2,hWnd,iMessage,wParam,lParam);
}


LRESULT CALLBACK	PacketTransLogProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return( g_CPacketTransLogDlg->DlgMsgProc( hWnd, iMessage, wParam, lParam ) );
}

CPacketTransLogDlg::CPacketTransLogDlg()
{
	g_CPacketTransLogDlg = this;
	m_IsPrintLog = true;
	
	m_hParentInstance = NULL;			
	m_hParentWnd	  = NULL;					
	m_hMainDlg		  = NULL;
	
	m_hEditLog		  = NULL;
	m_hHiddenWnd	  = NULL;
	m_hActiveBtn	  = NULL;

	m_pStrEdit		  =	NULL;
	m_nMaxStrLength = 100000;

	m_IsShow = true;

	m_nPrintType = NDebugPrint;
}

CPacketTransLogDlg::~CPacketTransLogDlg()
{
	DestroyDlg();
}

void  CPacketTransLogDlg::CreateDlg( HINSTANCE hParentInst, HWND hParentWnd )
{
	if( m_hMainDlg )
	{
		ShowWindow( m_hMainDlg, SW_SHOW);
		return;
	}
	
	m_hParentWnd = hParentWnd;
	m_hParentInstance = hParentInst;
	
	m_hMainDlgBgBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	RECT rect;
	GetWindowRect( hParentWnd, &rect );
	m_MainDlgRect.left = rect.right;
	m_MainDlgRect.right = rect.right + DLG_PACTLOG_WIDTH;
	m_MainDlgRect.top = rect.top;	m_MainDlgRect.bottom = rect.bottom;
	
	WNDCLASS wndClass = {	0, PacketTransLogProc, 0, 0, hParentInst,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor( NULL, IDC_ARROW ),
		m_hMainDlgBgBrush,
		NULL, DLG_PACTLOG_NAME };
	RegisterClass(&wndClass);
	
	m_hMainDlg = CreateWindow( DLG_PACTLOG_NAME, DLG_PACTLOG_NAME, 
		WS_OVERLAPPED | WS_CAPTION,
		rect.right, rect.top, DLG_PACTLOG_WIDTH, 600,
		NULL, (HMENU)NULL, hParentInst, NULL);
	
	ShowWindow( m_hMainDlg, SW_SHOW);	
}

void CPacketTransLogDlg::DestroyDlg()
{
	if(m_hMainDlg)
	{
		
		if( m_hMainDlg )
		{
			DestroyWindow( m_hMainDlg );
			m_hMainDlg = NULL;
		}
		
		if( m_hMainDlgBgBrush )
		{
			DeleteObject( m_hMainDlgBgBrush );
			m_hMainDlgBgBrush = NULL;
		}
		
		if( m_hFont )
		{
			DeleteObject( m_hFont );
			m_hFont = NULL;
		}
	}
}

void CPacketTransLogDlg::ActiveDlg()
{
	SetActiveWindow( m_hMainDlg );
}

LRESULT CPacketTransLogDlg::DlgMsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static HBRUSH hWhite;
	
	switch(iMessage) 
	{
	case WM_CREATE:	
		
		m_hHiddenWnd = CreateWindow( "edit", NULL, WS_CHILD | WS_BORDER,
			10, 10, 400, 20, hWnd, (HMENU)0, m_hParentInstance, NULL);		
		
		m_hEditLog	= CreateWindow( "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL |
			        ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE | ES_READONLY,
			10, 10, 570, 460, hWnd, (HMENU)IDC_PACTLOG_LIST_LOG, m_hParentInstance, NULL );

		m_hDebLog = CreateWindow( "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL |
			        ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE | ES_READONLY,
			10, 10, 570, 460, hWnd, (HMENU)IDC_PACTLOG_DLIST_LOG, m_hParentInstance, NULL );
		ShowWindow( m_hEditLog, SW_HIDE);
		
		m_hActiveBtn = CreateWindow( "button", _T("Not Print Log"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						20, 490, 120, 35, hWnd, (HMENU)IDC_PACTLOG_ACT_BTN, m_hParentInstance, NULL);

		m_hPacketBtn = CreateWindow( "button", _T("Packet Log"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						220, 490, 120, 35, hWnd, (HMENU)IDC_PACTLOG_PACT_BTN, m_hParentInstance, NULL);
		m_hLogSaveBtn = CreateWindow( "button", _T("Save Log"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						360, 490, 120, 35, hWnd, (HMENU)IDC_PACTLOG_SAVE_BTN, m_hParentInstance, NULL);

		switch( g_RockClient.GetLanguage() ) 
		{
		default:
		case eLang_Korea: 
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "굴림" );
			break;
		case eLang_Japan:
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "굃굍 긕긘긞긏" );		 
			break;
		case eLang_Indonesia:
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FONTSTR_ALL );		
			break;
		case eLang_English:
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FONTSTR_ALL );		
			break;	
		case eLang_China:
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "SIMSUN");		
			break;
		case eLang_Taiwan:
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "SimHei" );		
			break;
		case eLang_HongKong:
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "SimHei" );		
			break;
		case eLang_Singapore:
			m_hFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FONTSTR_ALL );		
			break;	
	
		}

		
		SendMessage( m_hEditLog, WM_SETFONT, (WPARAM)m_hFont, (LPARAM)MAKELPARAM(TRUE, 0) );
		SendMessage( m_hEditLog, EM_LIMITTEXT, (WPARAM)m_nMaxStrLength, 0);

		SendMessage( m_hDebLog, WM_SETFONT, (WPARAM)m_hFont, (LPARAM)MAKELPARAM(TRUE, 0) );
		SendMessage( m_hDebLog, EM_LIMITTEXT, (WPARAM)m_nMaxStrLength, 0);
	
		return 0;
	case WM_SETFOCUS:	
		
		if(!g_RockClient.m_IsCursor)
		{
		   ShowCursor( TRUE );
		   g_RockClient.m_IsCursor = TRUE;
		   SetFocus( m_hEditLog );
		}	
		return 0;
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_NCHITTEST:
		{
			LRESULT ret = DefWindowProc( hWnd, iMessage, wParam, lParam );
			if( ret == HTCLIENT ) ret = HTCAPTION;		
			return ret;
		}
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_PACTLOG_ACT_BTN:
				{
					m_IsPrintLog = !m_IsPrintLog;
					if(m_IsPrintLog)
					{
						SetWindowText( m_hActiveBtn, _T( "Not Print Log" ) );
					}
					else
					{
						SetWindowText( m_hActiveBtn, _T( "Print Log" ) );
					}				
				}
				break;
			case IDC_PACTLOG_PACT_BTN:
				{
					if( m_nPrintType == NPacketPrint )
					{
						m_nPrintType = NDebugPrint;
						SetWindowText( m_hPacketBtn, _T( "Packet Log" ) );

						ShowWindow( m_hEditLog, SW_HIDE);
						ShowWindow( m_hDebLog, SW_SHOW);
					}
					else if( m_nPrintType == NDebugPrint )
					{
						m_nPrintType = NPacketPrint;
						SetWindowText( m_hPacketBtn, _T( "Debug Log" ) );

						ShowWindow( m_hDebLog, SW_HIDE);
						ShowWindow( m_hEditLog, SW_SHOW);
					}
				}
				break;
			case IDC_PACTLOG_SAVE_BTN:
				{
					SaveLogFile();
				}
				break;
		}		
		return 0;
	case WM_CLOSE:
	case WM_DESTROY:		
		return 0;
	}	
	
	return( DefWindowProc( hWnd, iMessage, wParam, lParam ) );
}

void CPacketTransLogDlg::Open()
{
	if( m_hMainDlg )
	{
		m_IsShow = true;
		ShowWindow( m_hMainDlg, SW_SHOW);
		SetFocus(g_RockClient.GetApp());
	}
}

void CPacketTransLogDlg::Close()
{
	if( m_hMainDlg )
	{
		m_IsShow = false;
		ShowWindow( m_hMainDlg, SW_HIDE);		
	}
}

void  CPacketTransLogDlg::InsertListString( char *buf, ... )
{
	char tmp_buf[1024];
	va_list args;
	
	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );
	
	strcat(tmp_buf, "\r\n");

	int ilength = GetWindowTextLength(m_hEditLog);

	SendMessage(m_hEditLog, EM_SETSEL, ilength, ilength);
	SendMessage(m_hEditLog, EM_REPLACESEL, 0, (LPARAM)tmp_buf);	
}

void  CPacketTransLogDlg::PrintPacketTransInfo(void * pPacket,BOOL IsRecv)
{
	if(m_hMainDlg)
	{	
		if( !m_IsPrintLog )
			return;	
		
		SRpHeader* header = ( SRpHeader* ) pPacket;
		char tmp_buf[1024];
		
		time_t tval;
		time( &tval );
		struct tm *currTM = localtime( &tval );
		sprintf( tmp_buf, "[%d/%02d/%02d %02d:%02d:%02d]", currTM->tm_year+1900, 
			currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec);
		
		PrintHighStoCPacketInfo(pPacket, tmp_buf, "[Server->Client]");
		PrintStoCPacketInfo(pPacket, tmp_buf, "[Server->Client]");
		PrintCtoSPacketInfo(pPacket, tmp_buf, "[Client->Server]");
		
		int ilength = GetWindowTextLength(m_hEditLog);
		
		int iLimitLength = m_nMaxStrLength * 0.8;
		if( iLimitLength < ilength )
		{
		   SendMessage(m_hEditLog, EM_SETSEL, 0, iLimitLength);
		   SendMessage(m_hEditLog, EM_REPLACESEL, 0, (LPARAM)"");	
		}
	}
}

void CPacketTransLogDlg::PrintHighStoCPacketInfo(void * pPacket,
												 char * pstrTimeInfo, char * pstrDirInfo)
{
	
#ifdef _DEBUG
	
	SRpHeader* header = ( SRpHeader* ) pPacket;
	int i = 0;
	int count = 0;

	SRpScErrorFramework * pErrorPacket = NULL;
	
	switch(header->theProtocol)
	{
		//CNetworkMgr::MsgProc() 수신패킷
	case n_RpScKeepAliveCheck:		       
		break;
	case n_RpScErrorFramework:
		
		pErrorPacket = (SRpScErrorFramework *)pPacket;
		
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScErrorFramework");
		InsertListString("theErrorCode: %d  theErrorStr: %s", 
		((SRpScErrorFramework *)pPacket)->theErrorCode,
		((SRpScErrorFramework *)pPacket)->theErrorStr);
		break;
		
	case n_RpScServerSetList:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScServerSetList");	  
		InsertListString("theServerSetNum: %d", ((SRpScServerSetList *)pPacket)->theServerSetNum);
        count = ((SRpScServerSetList *)pPacket)->theServerSetNum;
	    /*for(i = 0 ; i < count ; i++)
		{
		   InsertListString("%d: theName: %s  theIP: %s  thePort: %d  theConnectUser: %d", i+1,
		   ((SRpScServerSetList *)pPacket)->theServerSetList[i].theName,
		   ((SRpScServerSetList *)pPacket)->theServerSetList[i].theIP,
		   ((SRpScServerSetList *)pPacket)->theServerSetList[i].thePort,
		   ((SRpScServerSetList *)pPacket)->theServerSetList[i].theStatus);		
		}*/
		break;
		
		/*
	case n_RpScWorldList: 
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScWorldList");
		InsertListString("theWorldNum: %d", 
		((SRpScWorldList *)pPacket)->theWorldNum);
		break;


	case n_RpScChannelList:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScChannelList");
		InsertListString("theChannelNum: %d ", 
		((SRpScChannelList *)pPacket)->theChannelNum );
		break;
		*/
		
	case n_RpScCharacterList:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScCharacterList");
		InsertListString("theCharacterNum: %d", ((SRpScCharacterList *)pPacket)->theCharacterNum);
		count = ((SRpScCharacterList *)pPacket)->theCharacterNum;
		for(i = 0 ; i < count ; i++)
		{
           InsertListString("%d: theData.thePCName: %s", i+1,
           ((SRpScCharacterList *)pPacket)->theCharacterList[i].theData.thePCName);
		}
		break;	      
	case n_RpScPrepareLogin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPrepareLogin");
		InsertListString("theResultCode: %d", ((SRpScPrepareLogin *)pPacket)->theResultCode);
		break;
	case n_RpScPCLogin:		
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPCLogin");
		InsertListString("thePCInfo.thePCName: %s  thePCInfo.theAccountID: %s",
		((SRpScPCLogin *)pPacket)->thePCInfo.thePCName,
		((SRpScPCLogin *)pPacket)->thePCInfo.theAccountID);
		break;
		
	}
	
#endif

}

void CPacketTransLogDlg::PrintStoCPacketInfo(void * pPacket,
											 char * pstrTimeInfo, char * pstrDirInfo)
{
	
#ifdef _DEBUG	
	
	SRpHeader* header = ( SRpHeader* ) pPacket;
	
	switch(header->theProtocol)
	{
		//CRockClient::Proc_RpScProtocols() 수신패킷
	case n_RpScAppearPC:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAppearPC");
		InsertListString("theAppearPC.thePCName: %s  theAppearPC.thePCID: %d  theAppearType: %d",
        ((SRpScAppearPC *)pPacket)->theAppearPC.thePCName,
		((SRpScAppearPC *)pPacket)->theAppearPC.thePCID,
		((SRpScAppearPC *)pPacket)->theAppearType);
		break;
	case n_RpScDisappearPC:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDisappearPC");
		InsertListString("theDisappearPCID: %d  theDisappearType: %d",
		((SRpScDisappearPC *)pPacket)->theDisappearPCID,
        ((SRpScDisappearPC *)pPacket)->theDisappearType);
		break;				   
	case n_RpScStartMove:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScStartMove");
		InsertListString("theDestiCell: %d  theMoveSpeed: %d  theResult: %d",
        ((SRpScStartMove *)pPacket)->theDestiCell,
		((SRpScStartMove *)pPacket)->theMoveSpeed,
		((SRpScStartMove *)pPacket)->theResult);
		break;		 
	case n_RpScSeeStartMove:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeStartMove");
		InsertListString("thePCID: %d  theStartCell: %d  theDestiCell: %d  theMoveSpeed: %d",
        ((SRpScSeeStartMove *)pPacket)->thePCID,
		((SRpScSeeStartMove *)pPacket)->theStartCell,
		((SRpScSeeStartMove *)pPacket)->theDestiCell,
		((SRpScSeeStartMove *)pPacket)->theMoveSpeed);*/
		break;
	case n_RpScStop:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScStop");
		InsertListString("thePCID: %d  theCell: %d",
		((SRpScStop *)pPacket)->thePCID,
		((SRpScStop *)pPacket)->theCell);*/
		break;
	case n_RpScAttack:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAttack");
		InsertListString("theAttackedID: %d  theAttackedHP: %d  theMotion: %d  theResult  %d",
        ((SRpScAttack *)pPacket)->theAttackedID,
        ((SRpScAttack *)pPacket)->theAttackedHP,
        ((SRpScAttack *)pPacket)->theMotion,
        ((SRpScAttack *)pPacket)->theResult);
		InsertListString("theAttackedType: %d  theCritical: %d  theComboCount: %d  theComboStatus  %d",
		((SRpScAttack *)pPacket)->theAttackedType,
        ((SRpScAttack *)pPacket)->theCritical,
        ((SRpScAttack *)pPacket)->theComboCount,
        ((SRpScAttack *)pPacket)->theComboStatus);
		InsertListString("theComboGage: %d  theDieFlag: %d",
		((SRpScAttack *)pPacket)->theGage,
        ((SRpScAttack *)pPacket)->theDieFlag);
		break;
	case n_RpScAttacked:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAttacked");
		InsertListString("theAttackID: %d  theAttackedCell: %d  theAttackedHP: %d  theMotion: %d theDieFlag: %d",
		((SRpScAttacked *)pPacket)->theAttackID,
		((SRpScAttacked *)pPacket)->theAttackedCell,
		((SRpScAttacked *)pPacket)->theAttackedHP,
		((SRpScAttacked *)pPacket)->theMotion,
		((SRpScAttacked *)pPacket)->theDieFlag);
		break;   
	case n_RpScSeeAttack:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeAttack");
		InsertListString("theAttackID: %d  theAttackedID: %d  theAttackedCell: %d  theAttackedHP: %d",
		((SRpScSeeAttack *)pPacket)->theAttackID,
		((SRpScSeeAttack *)pPacket)->theAttackedID,
		((SRpScSeeAttack *)pPacket)->theAttackedCell,
		((SRpScSeeAttack *)pPacket)->theAttackedHP);
		InsertListString("theMotion: %d  theDieFlag:  %d",
		((SRpScSeeAttack *)pPacket)->theMotion,
		((SRpScSeeAttack *)pPacket)->theDieFlag);
		break;
	case n_RpScAppearNPC:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAppearNPC");
		InsertListString("theNPCID: %d  theNPCType: %d  theCellIndex: %d",
		((SRpScAppearNPC *)pPacket)->theNPCID,
		((SRpScAppearNPC *)pPacket)->theNPCType,
		((SRpScAppearNPC *)pPacket)->theCellIndex);*/
		break;
	case n_RpScDisappearNPC:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDisappearNPC");
		InsertListString("theNPCID: %d", ((SRpScDisappearNPC *)pPacket)->theNPCID);*/
		break;		   
	case n_RpScNPCMove:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScNPCMove");
		InsertListString("theNPCID: %d  theMoveType: %d  theDistance: %d",
		((SRpScNPCMove *)pPacket)->theNPCID,
		((SRpScNPCMove *)pPacket)->theMoveType,
		((SRpScNPCMove *)pPacket)->theDistance);*/
		break;
		
	case n_RpScSay://SRpScSay
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSay");
		//InsertListString("theSenderID: %d  theSender: %s  theMessage: %s",
		//((SRpScSay *)pPacket)->theSenderID, ((SRpScSay *)pPacket)->theSender,
		//((SRpScSay *)pPacket)->theMessage);		
		break;
		
	case n_RpScWhisper://SRpScWhisper
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScWhisper");
		InsertListString("theReceiver: %s  theResult: %d",
		((SRpScWhisper *)pPacket)->theReceiver,
		((SRpScWhisper *)pPacket)->theResult);		
		break;
		
	case n_RpScRecvWhisper://SRpScRecvWhisper
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScRecvWhisper");
		//InsertListString("theSender: %s  theMessage: %s",
		//((SRpScRecvWhisper *)pPacket)->theSender,
		//((SRpScRecvWhisper *)pPacket)->theMessage);		
		break;
		
	case n_RpScAnswerWarp :
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAnswerWarp");
		InsertListString("theWarpCell: %d  theWarpMap: %d  theResult: %d",
		((SRpScAnswerWarp *)pPacket)->theWarpCell,
		((SRpScAnswerWarp *)pPacket)->theWarpMap,
		((SRpScAnswerWarp *)pPacket)->theResult);
		break;								
		
	case n_RpScSpelling:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSpelling");
		InsertListString("theCode: %d  theTarget: %d  theTargetType: %d",
		((SRpScSpelling *)pPacket)->theCode,
		((SRpScSpelling *)pPacket)->theTarget,
		((SRpScSpelling *)pPacket)->theTargetType);
		break;		
		
	case n_RpScSeeSpelling:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeSpelling");
		InsertListString("thePCID: %d  theCode: %d  theSpellingCount: %d  theTarget: %d  theTargetType: %d",
        ((SRpScSeeSpelling *)pPacket)->thePCID,
		((SRpScSeeSpelling *)pPacket)->theCode,
		((SRpScSeeSpelling *)pPacket)->theSpellingCount,
		((SRpScSeeSpelling *)pPacket)->theTarget,
		((SRpScSeeSpelling *)pPacket)->theTargetType);
		break;
		
	case n_RpScSkill :
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSkill");
		InsertListString("theResult: %d  theTarget: %d  theTargetType: %d  theSkillCode: %d",
		((SRpScSkill *)pPacket)->theResult,
		((SRpScSkill *)pPacket)->theTarget,
		((SRpScSkill *)pPacket)->theTargetType,
		((SRpScSkill *)pPacket)->theSkillCode);
		break;								
		
	case n_RpScSeeSkill :
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeSkill");
		InsertListString("theAttackID: %d  theAttackType: %d  theTarget: %d  theTargetType: %d  theSkillCode: %d",
		((SRpScSeeSkill *)pPacket)->theAttackID,
		((SRpScSeeSkill *)pPacket)->theAttackType,
		((SRpScSeeSkill *)pPacket)->theTarget,
		((SRpScSeeSkill *)pPacket)->theTargetType,
		((SRpScSeeSkill *)pPacket)->theSkillCode);
		break;
		
		//////////// Item Protocol ////////////////////////////////////////////
	case n_RpScPickup:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPickup");
		InsertListString("theResult: %d  theItemData.theItem.theItemUID: %d  theItemData.theItem.theCode %d",
		((SRpScPickup *)pPacket)->theResult,
		((SRpScPickup *)pPacket)->theItemData.theItem.theItemUID,
		((SRpScPickup *)pPacket)->theItemData.theItem.theCode);
		InsertListString("theProcess: %d  theProcessNPC: %d",
		((SRpScPickup *)pPacket)->theProcess,
		((SRpScPickup *)pPacket)->theProcessNPC);
		break;
		
	case n_RpScPickupEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPickupEld");
		InsertListString("theEld: %d", ((SRpScPickupEld *)pPacket)->theEld);
		break;
		
	case n_RpScDrop:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDrop");
		InsertListString("theItemID: %d  theBundle: %d  theItemTab %d  theResult %d",
		((SRpScDrop *)pPacket)->theItemID,
		((SRpScDrop *)pPacket)->theBundle,
		((SRpScDrop *)pPacket)->theItemTab,
		((SRpScDrop *)pPacket)->theResult);
		break;
		
	case n_RpScDropEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDropEld");
		InsertListString("theResult: %d  theEld: %d",
		((SRpScDropEld *)pPacket)->theResult,
		((SRpScDropEld *)pPacket)->theEld);
		break;
		
	case n_RpScUse:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScUse");
		InsertListString("theItemID: %d  theBundle: %d  theResult: %d",
		((SRpScUse *)pPacket)->theItemID,
		((SRpScUse *)pPacket)->theBundle,
		((SRpScUse *)pPacket)->theResult);
		break;
		
	case n_RpScSeeUse:	
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeUse");
		InsertListString("thePCID: %d  theItemCode: %d",
		((SRpScSeeUse *)pPacket)->thePCID,
		((SRpScSeeUse *)pPacket)->theItemCode);
		break;
		
	case n_RpScEquip:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScEquip");
		InsertListString("theItemID: %d  thePosition: %d  theResult: %d",
		((SRpScEquip *)pPacket)->theItemID,
		((SRpScEquip *)pPacket)->thePosition,
		((SRpScEquip *)pPacket)->theResult);
		break;
		
	case n_RpScChangeEquip:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScChangeEquip");
		InsertListString("thePreItemID: %d  theNewItemID: %d  thePrePosition: %d  theNewPosition: %d  theResult: %d",
		((SRpScChangeEquip *)pPacket)->thePreItemID,
		((SRpScChangeEquip *)pPacket)->theNewItemID,
		((SRpScChangeEquip *)pPacket)->thePrePosition,
		((SRpScChangeEquip *)pPacket)->theNewPosition,
		((SRpScChangeEquip *)pPacket)->theResult);			
		break;
		
	case n_RpScSeeEquip:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeEquip");
		InsertListString("thePCID: %d  theItem.theCode: %d  thePosition: %d",
		((SRpScSeeEquip *)pPacket)->thePCID,
		((SRpScSeeEquip *)pPacket)->theItem.theCode,
		((SRpScSeeEquip *)pPacket)->thePosition);
		break;
		
	case n_RpScTakeoff:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScTakeoff");
		InsertListString("theItemID: %d  thePosition: %d  theResult: %d",
		((SRpScTakeoff *)pPacket)->theItemID,
		((SRpScTakeoff *)pPacket)->thePosition,
		((SRpScTakeoff *)pPacket)->theResult);
		break;
		
	case n_RpScSeeTakeoff:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeTakeoff");
		InsertListString("thePCID: %d  thePosition: %d",
		((SRpScSeeTakeoff *)pPacket)->thePCID,
		((SRpScSeeTakeoff *)pPacket)->thePosition);
		break;
		
	case n_RpScBuy:	
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScBuy");
		InsertListString("theMoney: %d  theResult: %d  theItemData.theItem.theItemUID: %d",
		((SRpScBuy *)pPacket)->theMoney,
		((SRpScBuy *)pPacket)->theResult,
		((SRpScBuy *)pPacket)->theItemData.theItem.theItemUID);
		break;
		
	case n_RpScSell:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSell");
		InsertListString("theItemID: %d  theMoney: %d  theBundle: %d  theItemTab: %d  theResult: %d",
		((SRpScSell *)pPacket)->theItemID,
		((SRpScSell *)pPacket)->theMoney,
		((SRpScSell *)pPacket)->theBundle,
		((SRpScSell *)pPacket)->theItemTab,
		((SRpScSell *)pPacket)->theResult);
		break;
		
	case n_RpScCombine:	
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScCombine");
		InsertListString("theItemID1: %d  theItemID2: %d  theBundle1: %d  theBundle2: %d  theItemTab: %d  theResult: %d",
		((SRpScCombine *)pPacket)->theItemID1,
		((SRpScCombine *)pPacket)->theItemID2,
		((SRpScCombine *)pPacket)->theBundle1,
		((SRpScCombine *)pPacket)->theBundle2,
		((SRpScCombine *)pPacket)->theItemTab,
		((SRpScCombine *)pPacket)->theResult);
		break;
		
	case n_RpScChangeSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScChangeSlot");
		InsertListString("theItemID: %d  theItemID2: %d  theSlot: %d  theItemTab: %d  theResult: %d",
		((SRpScChangeSlot *)pPacket)->theItemID,
		((SRpScChangeSlot *)pPacket)->theItemID2,
		((SRpScChangeSlot *)pPacket)->theSlot,
		((SRpScChangeSlot *)pPacket)->theItemTab,
		((SRpScChangeSlot *)pPacket)->theResult);
		break;

	///--JAPAN_BUG_MODIFY		
	case n_RpScLinkQuickSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScLinkQuickSlot");
		InsertListString("theQuickSlot1.theType: %d  theQuickSlot1.thePosition: %d",			
		((SRpScLinkQuickSlot *)pPacket)->theQuickSlot1.theType,
		((SRpScLinkQuickSlot *)pPacket)->theQuickSlot1.thePosition);
		InsertListString("theQuickSlot2.theType: %d  theQuickSlot2.thePosition: %d  theResult: %d",			
		((SRpScLinkQuickSlot *)pPacket)->theQuickSlot2.theType,
		((SRpScLinkQuickSlot *)pPacket)->theQuickSlot2.thePosition,	
		((SRpScLinkQuickSlot *)pPacket)->theResult);		
		break;	
	case n_RpScUnlinkQuickSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScUnlinkQuickSlot");
		InsertListString("thePosition: %d  theResult: %d",
		((SRpScUnlinkQuickSlot *)pPacket)->thePosition,
		((SRpScUnlinkQuickSlot *)pPacket)->theResult);
		break;
		
	case n_RpScAppearItem:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAppearItem");
		InsertListString("theItemID: %d  theCell: %d  theNPCID: %d  theItemCode: %d",
		((SRpScAppearItem *)pPacket)->theItemID,
		((SRpScAppearItem *)pPacket)->theCell,
		((SRpScAppearItem *)pPacket)->theNPCID,
		((SRpScAppearItem *)pPacket)->theItemCode);
		break;
		
	case n_RpScDisappearItem:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDisappearItem");
		InsertListString("theItemID: %d", ((SRpScDisappearItem *)pPacket)->theItemID);
		break;
		
	case n_RpScAlive:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAlive");
		InsertListString("theMap: %d  theCell: %d",
		((SRpScAlive *)pPacket)->theMap,
		((SRpScAlive *)pPacket)->theCell);
		break;
		
	case n_RpScSeeAliveOnSpot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeAliveOnSpot");
		InsertListString("thePCID: %d", ((SRpScSeeAliveOnSpot *)pPacket)->thePCID);
		break;
		
	case n_RpScGetExp:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScGetExp");
		InsertListString("theExp: %d  theFrom: %d",
		((SRpScGetExp *)pPacket)->theExp,
		((SRpScGetExp *)pPacket)->theFrom);
		break;
		
	case n_RpScLevelup:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScLevelup");
		InsertListString("theLevel: %d  theMaxHP: %d  theMaxSP: %d  theMaxMP: %d",
		((SRpScLevelup *)pPacket)->theLevel,
		((SRpScLevelup *)pPacket)->theMaxHP,
		((SRpScLevelup *)pPacket)->theMaxSP,
		((SRpScLevelup *)pPacket)->theMaxMP);
		break;
		
	case n_RpScSeeLevelup:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeLevelup");
		InsertListString("thePCID: %d  theLevel: %d  theMaxHP: %d  theMaxSMP: %d",
		((SRpScSeeLevelup *)pPacket)->thePCID,
		((SRpScSeeLevelup *)pPacket)->theLevel,
		((SRpScSeeLevelup *)pPacket)->theMaxHP,
		((SRpScSeeLevelup *)pPacket)->theMaxSMP);
		break;
		
	case n_RpScGetSkillPoint:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScGetSkillPoint");
		InsertListString("theSkillPoint: %d theTotalSkillPoint: %d", 
		((SRpScGetSkillPoint *)pPacket)->theSkillPoint,
		((SRpScGetSkillPoint *)pPacket)->theTotalSkillPoint);
		break;
		
	case n_RpScPoint:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPoint");
		InsertListString("thePoint: %d  theResult: %d",
		((SRpScPoint *)pPacket)->thePoint,
		((SRpScPoint *)pPacket)->theResult);
		break;
		
	case n_RpScScriptStart:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScScriptStart");
		InsertListString("theResult: %d", ((SRpScScriptStart *)pPacket)->theResult);
		break;
		
	case n_RpScDialogScript:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDialogScript");
		InsertListString("theCommand: %d  theCode: %d  theStringIndex: %d",
		((SRpScDialogScript *)pPacket)->theCommand,
		((SRpScDialogScript *)pPacket)->theCode,
		((SRpScDialogScript *)pPacket)->theStringIndex);
		break;
		
	case n_RpScAskScript:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAskScript");
		InsertListString("theStringIndex: %d", ((SRpScAskScript *)pPacket)->theStringIndex);
		break;
		
	case n_RpScItemShopInfo:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScItemShopInfo");
		InsertListString("theVersion: %d", ((SRpScItemShopInfo *)pPacket)->theVersion);
		break;
		
	case n_RpScCreateCharacter:			
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScCreateCharacter");
		InsertListString("theResultCode: %d", ((SRpScCreateCharacter *)pPacket)->theResultCode);
		break;
		
	case n_RpScDeleteCharacter:			
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDeleteCharacter");
		InsertListString("theDelDelayTime: %s  theResultCode: %d",
		((SRpScDeleteCharacter *)pPacket)->theDelDelayTime,
		((SRpScDeleteCharacter *)pPacket)->theResultCode);
		break;
		
	case n_RpScStatus:			
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScStatus");
		InsertListString("theHP: %d  theSP: %d  theMP: %d",
		((SRpScStatus *)pPacket)->theHP,
		((SRpScStatus *)pPacket)->theSP,
		((SRpScStatus *)pPacket)->theMP);
		break;
		
	case n_RpScSeeKeepupSkillEnd:			
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeKeepupSkillEnd");
		InsertListString("theID: %d  theType: %d  theSkillCode: %d  theEndFlag: %d",
		((SRpScSeeKeepupSkillEnd *)pPacket)->theID,
		((SRpScSeeKeepupSkillEnd *)pPacket)->theType,
		((SRpScSeeKeepupSkillEnd *)pPacket)->theSkillCode,
		((SRpScSeeKeepupSkillEnd *)pPacket)->theEndFlag);
		InsertListString("theEffectType: %d  theEffectValue: %d  theEffectMaxValue: %d",		
		((SRpScSeeKeepupSkillEnd *)pPacket)->theEffectType,
		((SRpScSeeKeepupSkillEnd *)pPacket)->theEffectValue,
		((SRpScSeeKeepupSkillEnd *)pPacket)->theEffectMaxValue);		
		break;
		
	case n_RpScSeeKeepupSkillPreEnd:			
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeKeepupSkillPreEnd");
		InsertListString("theID: %d  theType: %d  theEffectType: %d  theSkillCode: %d",
		((SRpScSeeKeepupSkillPreEnd *)pPacket)->theID,
		((SRpScSeeKeepupSkillPreEnd *)pPacket)->theType,
		((SRpScSeeKeepupSkillPreEnd *)pPacket)->theEffectType,
		((SRpScSeeKeepupSkillPreEnd *)pPacket)->theSkillCode);
		break;
		
	case n_RpScChangeMainClass:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScChangeMainClass");
		InsertListString("theMainClass: %d", ((SRpScChangeMainClass *)pPacket)->theMainClass);
		break;
		
	case n_RpScChangeSubClass:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScChangeMainClass");
		InsertListString("theSubClass: %d", ((SRpScChangeSubClass *)pPacket)->theSubClass);
		break;
		
		
	case n_RpScNPCStat:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScNPCStat");
		InsertListString("theNPCID: %d  theHP: %d  theSP: %d",
		((SRpScNPCStat *)pPacket)->theNPCID,
		((SRpScNPCStat *)pPacket)->theHP,
		((SRpScNPCStat *)pPacket)->theSP);
		break;
		
	case n_RpScSeeOtherStatus:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeOtherStatus");
		InsertListString("thePCID: %d  theHP: %d  theSP: %d  theMP: %d",
		((SRpScSeeOtherStatus *)pPacket)->thePCID,
		((SRpScSeeOtherStatus *)pPacket)->theHP,
		((SRpScSeeOtherStatus *)pPacket)->theSP,
		((SRpScSeeOtherStatus *)pPacket)->theMP);
		break;
		
	case n_RpScRecoveryStat:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScRecoveryStat");
		InsertListString("theMAXHP: %d  theMAXMP: %d  theMAXSP: %d",
		((SRpScRecoveryStat *)pPacket)->theMaxHP,
		((SRpScRecoveryStat *)pPacket)->theMaxMP,
		((SRpScRecoveryStat *)pPacket)->theMaxSP);
		break;
		
	case n_RpScGetSkill:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScGetSkill");
		InsertListString("theSkillCode: %d  theResult: %d",
		((SRpScGetSkill *)pPacket)->theSkillCode,
		((SRpScGetSkill *)pPacket)->theResult);
		
		break;
		
	case n_RpScTakeSkill:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScTakeSkill");
		InsertListString("theSkillCode: %d", ((SRpScTakeSkill *)pPacket)->theSkillCode);
		break;
		
	case n_RpScStatus2:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScStatus2");
		break;
		
	case n_RpScQuestWindow:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScQuestWindow");
		InsertListString("theQuestCode: %d  theStatus: %d",
		((SRpScQuestWindow *)pPacket)->theQuestCode,
		((SRpScQuestWindow *)pPacket)->theStatus);
		break;
		
	case n_RpScQuestStatus:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScQuestStatus");
		InsertListString("theQuestCode: %d  theFlag: %d  theDisplay: %d",
		((SRpScQuestStatus *)pPacket)->theQuestCode,
		((SRpScQuestStatus *)pPacket)->theFlag,
		((SRpScQuestStatus *)pPacket)->theDisplay);
		break;
		
	case n_RpScGMCmd:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScGMCmd");
		InsertListString("theCommand: %d  theNParam[0]: %d  theNParam[1]: %d  theNParam[2]: %d  theNParam[3]: %d  theSParam: %s",
		((SRpScGMCmd *)pPacket)->theCommand,
		((SRpScGMCmd *)pPacket)->theNParam[0],
		((SRpScGMCmd *)pPacket)->theNParam[1],
		((SRpScGMCmd *)pPacket)->theNParam[2],
		((SRpScGMCmd *)pPacket)->theNParam[3],
		((SRpScGMCmd *)pPacket)->theSParam);
		break;
		
	case n_RpScGMCmdErr:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScGMCmdErr");
		InsertListString("theCommand: %d  theErrorCode: %d",
		((SRpScGMCmdErr *)pPacket)->theCommand,
		((SRpScGMCmdErr *)pPacket)->theErrorCode);
		break;
		
	case n_RpScAnnouncement:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScAnnouncement");
		InsertListString("theAnnouncement: %s", ((SRpScAnnouncement *)pPacket)->theAnnouncement);
		break;
		
	case n_RpScUpdateGage:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScUpdateGage");
		InsertListString("theGage: %d",			
		((SRpScUpdateGage *)pPacket)->theGage);
		break;
		//.....................................................................................................
		// 아처 작업

		//.....................................................................................................
	case n_RpScArcherReady:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScArcherReady");
		InsertListString("theResult: %d", ((SRpScArcherReady *)pPacket)->theResult);
		break;
		
	case n_RpScSeeArcherReady:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSeeArcherReady");
		InsertListString("thePCID: %d  theTarget: %d  theTargetType: %d",
		((SRpScSeeArcherReady *)pPacket)->thePCID,
		((SRpScSeeArcherReady *)pPacket)->theTarget,
		((SRpScSeeArcherReady *)pPacket)->theTargetType);
		break;
		
	case n_RpScQuestCount:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScQuestCount");
		InsertListString("theType: %d  theCode: %d  theCount: %d  theTotalCount: %d",
		((SRpScQuestCount *)pPacket)->theType,
		((SRpScQuestCount *)pPacket)->theCode,
		((SRpScQuestCount *)pPacket)->theCount,
		((SRpScQuestCount *)pPacket)->theTotalCount);			
		break;
		
		//--------------------------------------------------------------------//
		////////////////////////// Party Protocol //////////////////////////////
		//--------------------------------------------------------------------//
	case n_RpScPartyAskJoin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyAskJoin");
		InsertListString("theLeaderName: %s", ((SRpScPartyAskJoin *)pPacket)->theLeaderName);
		break;
		
	case n_RpScPartyAnsJoin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyAnsJoin");
		InsertListString("thePCName: %s  theAnswer: %d",
		((SRpScPartyAnsJoin *)pPacket)->thePCName,
		((SRpScPartyAnsJoin *)pPacket)->theAnswer);
		break;
		
	case n_RpScPartyCreate:
		//InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyCreate");
		break;
		
	case n_RpScPartyJoin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyJoin");
		InsertListString("theMemberID: %d  theMemberName: %s",
		((SRpScPartyJoin *)pPacket)->theMemberID,
		((SRpScPartyJoin *)pPacket)->theMemberName);
		InsertListString("thePartyID: %d  thePartyName: %s",	
		((SRpScPartyJoin *)pPacket)->thePartyID,
		((SRpScPartyJoin *)pPacket)->thePartyName);
		break;
		
	case n_RpScPartyInfo:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyInfo");
		InsertListString("thePartyID: %d  thePartyName: %s  theLeaderID: %d",
		((SRpScPartyInfo *)pPacket)->thePartyID,
		((SRpScPartyInfo *)pPacket)->thePartyName,
		((SRpScPartyInfo *)pPacket)->theLeaderID);
		InsertListString("theMemberIDList[0]: %d  theMemberIDList[1]: %d  theMemberIDList[2]: %d  theMemberIDList[3]: %d  theItemMgr: %d",
		((SRpScPartyInfo *)pPacket)->theMemberIDList[0],
		((SRpScPartyInfo *)pPacket)->theMemberIDList[1],
		((SRpScPartyInfo *)pPacket)->theMemberIDList[2],
		((SRpScPartyInfo *)pPacket)->theMemberIDList[3],
		((SRpScPartyInfo *)pPacket)->theItemMgr);
		break;
		
	case n_RpScPartyMemberInfo:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyMemberInfo");
		InsertListString("thePCID: %d  thePCName: %s",
		((SRpScPartyMemberInfo *)pPacket)->thePCID,
		((SRpScPartyMemberInfo *)pPacket)->thePCName);
		InsertListString("theCitizen: %d  theGender: %d  theFace: %d  theHair: %d",
		((SRpScPartyMemberInfo *)pPacket)->theCitizen,
		((SRpScPartyMemberInfo *)pPacket)->theGender,
		((SRpScPartyMemberInfo *)pPacket)->theFace,
		((SRpScPartyMemberInfo *)pPacket)->theHair);
		InsertListString("theMainClass: %d  theFame: %d  theLevel: %d  theGage: %d",
		((SRpScPartyMemberInfo *)pPacket)->theMainClass,
		((SRpScPartyMemberInfo *)pPacket)->theFame,
		((SRpScPartyMemberInfo *)pPacket)->theLevel,
		((SRpScPartyMemberInfo *)pPacket)->theGage);
		InsertListString("theMaxGage: %d  theMap: %d  theCell: %d",
		((SRpScPartyMemberInfo *)pPacket)->theMaxGage,
		((SRpScPartyMemberInfo *)pPacket)->theMap,
		((SRpScPartyMemberInfo *)pPacket)->theCell);
		break;
		
	case n_RpScPartyChangeName:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyChangeName");
		InsertListString("thePartyID: %d  thePartyName: %s",
		((SRpScPartyChangeName *)pPacket)->thePartyID,
		((SRpScPartyChangeName *)pPacket)->thePartyName);
		break;
		
	case n_RpScPartyChangeLeader:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyChangeLeader");
		InsertListString("theNewLeaderID: %d  theResult: %d",
		((SRpScPartyChangeLeader *)pPacket)->theNewLeaderID,
		((SRpScPartyChangeLeader *)pPacket)->theResult);
		break;
		
	case n_RpScPartyLeave:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyLeave");
		InsertListString("thePCName: %s  theLeaveFlag: %d",
		((SRpScPartyLeave *)pPacket)->thePCName,
		((SRpScPartyLeave *)pPacket)->theLeaveFlag);
		break;
		
	case n_RpScPartyTarget:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyTarget");
		InsertListString("theTargetID: %d", ((SRpScPartyTarget *)pPacket)->theTargetID);
		break;
		
	case n_RpScPartyGiveGage:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyGiveGage");
		InsertListString("theGiverID: %d  theReceiverID: %d  theGage: %d  theResult: %d",
		((SRpScPartyGiveGage *)pPacket)->theGiverID,
		((SRpScPartyGiveGage *)pPacket)->theReceiverID,
		((SRpScPartyGiveGage *)pPacket)->theGage,
		((SRpScPartyGiveGage *)pPacket)->theResult);
		break;
		
	case n_RpScPartyUpdateGage:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyUpdateGage");
		InsertListString("theMemberID: %d  theGage: %d",
		((SRpScPartyUpdateGage *)pPacket)->theMemberID,
		((SRpScPartyUpdateGage *)pPacket)->theGage);
		break;
		
	case n_RpScPartyChat:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyChat");
		//InsertListString("thePCName: %s  theString: %s",
		//((SRpScPartyChat *)pPacket)->thePCName,
		//((SRpScPartyChat *)pPacket)->theString);
		break;
		
	case n_RpScPartyItemMgr:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyItemMgr");
		InsertListString("theItemMgr: %d", ((SRpScPartyItemMgr *)pPacket)->theItemMgr);
		break;
		
	case n_RpScPartyGetItem:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyGetItem");
		InsertListString("thePCName: %s  theItemCode: %d",
		((SRpScPartyGetItem *)pPacket)->thePCName,
		((SRpScPartyGetItem *)pPacket)->theItemCode);
		break;
		
	case n_RpScPartyGetEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyGetEld");
		InsertListString("theEld: %d", ((SRpScPartyGetEld *)pPacket)->theEld);
		break;
		
	case n_RpScPartyExpMgr:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyExpMgr");
		InsertListString("theClassBonus: %d  theMemberNumBonus: %d  theLevelPenalty: %d",
		((SRpScPartyExpMgr *)pPacket)->theClassBonus,
		((SRpScPartyExpMgr *)pPacket)->theMemberNumBonus,
		((SRpScPartyExpMgr *)pPacket)->theLevelPenalty);
		break;
		
	case n_RpScPartyPosition:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPartyPosition");
		InsertListString("theMemberID: %d  theMap: %d  theCell: %d",
		((SRpScPartyPosition *)pPacket)->theMemberID,
		((SRpScPartyPosition *)pPacket)->theMap,
		((SRpScPartyPosition *)pPacket)->theCell);
		break;
		//.....................................................................................................
		// NPC 선점 관련
		//.....................................................................................................
	case n_RpScNPCPriority:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScNPCPriority");
		InsertListString("theNPCID: %d  thePriorityID: %d  thePriorityType: %d",
		((SRpScNPCPriority *)pPacket)->theNPCID,
		((SRpScNPCPriority *)pPacket)->thePriorityID,
		((SRpScNPCPriority *)pPacket)->thePriorityType);
		break;	
		
	//============================================================================================================
	//============================================================================================================
	// 보관 시스템 관련 프로토콜 
	//============================================================================================================
	//============================================================================================================
	case n_RpScOpenBank:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScOpenBank");
		InsertListString("theLoadFlag=%d, theEld=%d, theItemNum=%d",
		((SRpScOpenBank *)pPacket)->theLoadFlag,
		((SRpScOpenBank *)pPacket)->theEld,
		((SRpScOpenBank *)pPacket)->theItemNum);
		break;	
	case n_RpScDeposit:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDeposit");
		InsertListString("theResult=%d, theItemID=%d, theBundle=%d",
		((SRpScDeposit *)pPacket)->theResult,
		((SRpScDeposit *)pPacket)->theItemID,
		((SRpScDeposit *)pPacket)->theBundle);
		break;	
	case n_RpScDraw:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDraw");
		InsertListString("theResult=%d, theBankItemID=%d, theBankBundle=%d",
		((SRpScDraw *)pPacket)->theResult,
		((SRpScDraw *)pPacket)->theBankItemID,
		((SRpScDraw *)pPacket)->theBankBundle);
		break;
	case n_RpScDepositEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDepositEld");
		InsertListString("theResult=%d, theEld=%d, theBankEld=%d",
		((SRpScDepositEld *)pPacket)->theResult,
		((SRpScDepositEld *)pPacket)->theEld,
		((SRpScDepositEld *)pPacket)->theBankEld);
		break;	
	case n_RpScDrawEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDrawEld");
		InsertListString("theResult=%d, theEld=%d, theBankEld=%d",
		((SRpScDrawEld *)pPacket)->theResult,
		((SRpScDrawEld *)pPacket)->theEld,
		((SRpScDrawEld *)pPacket)->theBankEld);
		break;	
	case n_RpScWHChangeSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScWHChangeSlot");
		InsertListString("theResult=%d, theBankItemID1=%d, theSlot1=%d, theBankItemID2=%d, theSlot2=%d",
		((SRpScWHChangeSlot *)pPacket)->theResult,
		((SRpScWHChangeSlot *)pPacket)->theBankItemID1,
		((SRpScWHChangeSlot *)pPacket)->theSlot1,
		((SRpScWHChangeSlot *)pPacket)->theBankItemID2,
		((SRpScWHChangeSlot *)pPacket)->theSlot2);
		break;	

	case n_RpScEldacraft:
		InsertListString( "%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScEldacraft" );
		InsertListString( "theType=%d",
		( ( SRpScEldacraft * )pPacket )->theType);
		break;	

	case n_RpScSetStone:
		InsertListString( "%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScSetStone" );
		InsertListString( "result=%d ",
		( ( SRpScSetStone * )pPacket )->theResult);
		break;
		
	case n_RpScEldacraftGame:
		InsertListString( "%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScEldacraftGame" );
		InsertListString( "theResult=%d SubExp=%d DelstoneID=%d",
		( ( SRpScEldacraftGame * )pPacket )->theResult,
		( ( SRpScEldacraftGame * )pPacket )->theSubExp,
		( ( SRpScEldacraftGame * )pPacket )->theDelStoneID );
		break;		

	case n_RpCsStartMove:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsStartMove");
		InsertListString("Dest=%d Speed=%d",
			( (SRpCsStartMove * )pPacket )->theDestiCell,
			( (SRpCsStartMove * )pPacket )->theMoveSpeed );
		break;
		///-- Repair(수리)
	case n_RpScOpenRepairSkill:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScOpenRepairSkill");
		InsertListString("NPC/Skill=%d (1:NPC, 2:SKILL)",
			( ( SRpScOpenRepairSkillPtr )pPacket )->theRepairType );
		break;
		
		///-- 내구도 변화
	case n_RpScUpdateDurabilityItem:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScUpdateDurabilityItem");
		InsertListString("count = %d first = %d",
			( ( SRpScUpdateDurabilityItem * )pPacket )->theCount ,
			( ( SRpScUpdateDurabilityItem * )pPacket )->theRePairItemArray[0].theEndurance );
		break;
		///-- 수리 결과

	case n_RpScReplyRepair:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScReplyRepair");
		InsertListString("theResult = %d  Eld = %d ",
			( ( SRpScReplyRepair * )pPacket )->theErrorCode,
			( ( SRpScReplyRepair *)pPacket )->theEld );		
		break;

		///-- 웨폰 / 아머 크래프트 게임 시작.
	case n_RpScMakeItem:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMakeItem");
		InsertListString("theResult = %d  MapIndex = %d ",
			( ( SRpScMakeItem * )pPacket )->theResult,
			( ( SRpScMakeItem *)pPacket )->theMapIndex );		
		break;
		
		///-- 웨폰 / 아머 크래프트 게임 끝
	case n_RpScMakeItemResult:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMakeItemResult");
		InsertListString("theResult = %d  theSubExp = %d theItem = %d",
			( ( SRpScMakeItemResult * )pPacket )->theResult,
			( ( SRpScMakeItemResult * )pPacket )->theSubExp,
			( ( SRpScMakeItemResult *)pPacket )->theItem.theItem.theItemUID );		
		break;
	///-----------------------------------------------------------------------
	///-- community Protocols 
	///-----------------------------------------------------------------------
	case n_RpScMsngrError:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrError");
		InsertListString("theErrorCode %d  theStr %s",
			( ( SRpScMsngrError *)pPacket )->theErrorCode,
			( ( SRpScMsngrError *)pPacket )->theStr );
		break;
	case n_RpScMsngrMemberState:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrMemberState");
		InsertListString("theProcType %d thePCName %s theState %d",
			( ( SRpScMsngrMemberState *)pPacket )->theProcType,
			( ( SRpScMsngrMemberState *)pPacket )->thePCName,
			( ( SRpScMsngrMemberState *)pPacket )->theState);
		InsertListString("theChannel %d theMap %d theLevel %d theJob %d",
			( ( SRpScMsngrMemberState *)pPacket )->theChannel,
			( ( SRpScMsngrMemberState *)pPacket )->theMap, 
			( ( SRpScMsngrMemberState *)pPacket )->theLevel,
			( ( SRpScMsngrMemberState *)pPacket )->theJob);
		break;
	case n_RpScMsngrMemberInsertAsk:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrMemberInsertAsk");
		InsertListString("thePCName %s",
			( ( SRpScMsngrMemberInsertAsk *)pPacket )->thePCName);
		break;
	case n_RpScMsngrMemberDelete:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrMemberDelete");
		InsertListString("thePCName %s",
			( ( SRpScMsngrMemberDelete *)pPacket )->thePCName);
		break;
	case n_RpScMsngrMemberBlock:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrMemberBlock");
		InsertListString("theProcType %d  thePCName %s",
			( ( SRpScMsngrMemberBlock *)pPacket )->theProcType,
			( ( SRpScMsngrMemberBlock *)pPacket )->thePCName );
		break;
	case n_RpScMsngrChangeOpt:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChangeOpt");
		InsertListString("theOptState %d  theOptMapFlag %d theOptLevelFlag %d theOptJobFlag %d",
			( ( SRpScMsngrChangeOpt *)pPacket )->theOptState,
			( ( SRpScMsngrChangeOpt *)pPacket )->theOptMapFlag, 
			( ( SRpScMsngrChangeOpt *)pPacket )->theOptLevelFlag,
			( ( SRpScMsngrChangeOpt *)pPacket )->theOptJobFlag );
		break;
	case n_RpScMsngrSay:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrSay");
		InsertListString("theFromPCName %s theStr %s",
			( ( SRpScMsngrSay *)pPacket )->theFromPCName,
			( ( SRpScMsngrSay *)pPacket )->theStr);
		break;
	case n_RpScMsngrChatSystemMsg:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChatSystemMsg");
		InsertListString("theCode %d thePCName %s",
			( ( SRpScMsngrChatSystemMsg *)pPacket )->theCode,
			( ( SRpScMsngrChatSystemMsg *)pPacket )->thePCName);
		break;
	case n_RpScMsngrChatMemberState:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChatMemberState");
		InsertListString("theProcType %d thePCName %s",
			( ( SRpScMsngrChatMemberState *)pPacket )->theProcType,
			( ( SRpScMsngrChatMemberState *)pPacket )->thePCName );
		break;
	case n_RpScMsngrChatCreateRoom:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChatCreateRoom");
		InsertListString("theResult %d",
			( ( SRpScMsngrChatCreateRoom *)pPacket )->theResult );		
		break;
	case n_RpScMsngrChatDestroyRoom:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChatDestroyRoom");
		InsertListString("theOwnerPCName %s",
			( ( SRpScMsngrChatDestroyRoom *)pPacket )->theOwnerPCName );		
		break;
	case n_RpScMsngrChatInviteAsk:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChatDestroyRoom");
		InsertListString("theFromPCName %s theOwnerPCName %s",
			( ( SRpScMsngrChatInviteAsk *)pPacket )->theFromPCName,
			( ( SRpScMsngrChatInviteAsk *)pPacket )->theOwnerPCName);
		break;
	case n_RpScMsngrChatJoinRoom:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChatJoinRoom");
		InsertListString("theOwnerPCName %s",
			( ( SRpScMsngrChatJoinRoom *)pPacket )->theOwnerPCName);
		{
			for(int i = 0 ; i < 6 ; i++)
			{
				if(( ( SRpScMsngrChatJoinRoom *)pPacket )->theJoinedMemberPCName[i])
				{
					InsertListString("theJoinedMemberPCName[%d] %s", i, 
					( ( SRpScMsngrChatJoinRoom *)pPacket )->theJoinedMemberPCName[i]);
				}
			}		
		}		
		break;
	case n_RpScMsngrChatSay:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrChatSay");
		InsertListString("theFromPCName %s theStr %s",
			( ( SRpScMsngrChatSay *)pPacket )->theFromPCName,
			( ( SRpScMsngrChatSay *)pPacket )->theStr);
		break;
	case n_RpScPKAttack:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScPKAttack");
		InsertListString("thePKFlag %d theAttackUserPCID %d thePKPoint %d ",
			( ( SRpScPKAttack *)pPacket )->thePKFlag,
			( ( SRpScPKAttack *)pPacket )->theAttackUserPCID,
			( ( SRpScPKAttack *)pPacket )->thePKPoint );
		break;
	case n_RpScDeletedPKAttackUser:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScDeletedPKAttackUser");
		InsertListString("theAttackUserPCID %d ",			
			( ( SRpScDeletedPKAttackUser *)pPacket )->theAttackUserPCID );
		break;
	case n_RpScUpdatePKPoint:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScUpdatePKPoint");
		InsertListString("thePCID %d thePKPoint %d ",
			( ( SRpScUpdatePKPoint *)pPacket )->thePCID,
			( ( SRpScUpdatePKPoint *)pPacket )->thePKPoint );
		break;
	case n_RpScChangeFrontierPCName:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScChangeFrontierPCName");
		InsertListString("theOldPCName %s theNewPCName %s ",
			( ( SRpScChangeFrontierPCName *)pPacket )->theOldPCName,
			( ( SRpScChangeFrontierPCName *)pPacket )->theNewPCName );
		break;
	case n_RpScChangeName:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScChangeName");
		InsertListString("theType %d theID %d theNewName %s",
			( ( SRpScChangeName *)pPacket )->theType,
			( ( SRpScChangeName *)pPacket )->theID,
			( ( SRpScChangeName *)pPacket )->theNewName );
		break;
	case n_RpScMsngrJoinFrontier:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMsngrJoinFrontier");
		InsertListString("theFrontierName %s theMasterName %s",
			( ( SRpScMsngrJoinFrontier *)pPacket )->theFrontierInfo.theFrontierName,
			( ( SRpScMsngrJoinFrontier *)pPacket )->theFrontierInfo.theMasterName );
		break;
	case n_RpScCheckSoldierTime:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScCheckSoldierTime");
		InsertListString("theSlotNumber %d theRemainTime %u",
			( ( SRpScCheckSoldierTime *)pPacket )->theSlotNumber ,
			( ( SRpScCheckSoldierTime *)pPacket )->theRemainTime );
		break;
	case n_RpScNPCAttackedMsg:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScNPCAttackedMsg");
		InsertListString("theResult %d theNPCID %u theMapCode %u theFiledFlag = %d theOccupationNation = %d",
			( ( SRpScNPCAttackedMsg *)pPacket )->theResult , 
			( ( SRpScNPCAttackedMsg *)pPacket )->theNPCID ,
			( ( SRpScNPCAttackedMsg *)pPacket )->theMapCode,
			( ( SRpScNPCAttackedMsg *)pPacket )->theFiledFlag,
			( ( SRpScNPCAttackedMsg *)pPacket )->theOccupationNation );
		break;
	case n_RpScNPCLevelUP:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScNPCLevelUP");
		InsertListString("theNPCID %u theNPCCode %u theHP = %d",
			( ( SRpScNPCLevelUP *)pPacket )->theNPCID ,
			( ( SRpScNPCLevelUP *)pPacket )->theNPCCode,
			( ( SRpScNPCLevelUP *)pPacket )->theHP );
		break;
	case n_RpScEnterPvPField:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScEnterPvPField");
		InsertListString("theInfoNum %d",
			( ( SRpScEnterPvPField *)pPacket )->theInfoNum );
		break;
	case n_RpScUpdatePvPField:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScEnterPvPField");
		InsertListString("theNPCID %u theNPCCode %d theNPCCell %d theOccupationNation %d theMapCode %d",
			( ( SRpScUpdatePvPField *)pPacket )->theNPCID,
			( ( SRpScUpdatePvPField *)pPacket )->theNPCCode, 
			( ( SRpScUpdatePvPField *)pPacket )->theNPCCell,
			( ( SRpScUpdatePvPField *)pPacket )->theOccupationNation, 
			( ( SRpScUpdatePvPField *)pPacket )->theMapCode );
		break;
	case n_RpScMakeAddAccessory:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScMakeAddAccessory");
		InsertListString("theResult %d theAddAccessory.theItemUID %d theAddAccessory.theEldaStone %d",
			( ( SRpScMakeAddAccessory *)pPacket )->theResult,
			( ( SRpScMakeAddAccessory *)pPacket )->theAddAccessory.theWearItem.theItemUID, 
			( ( SRpScMakeAddAccessory *)pPacket )->theAddAccessory.theWearItem.theEldaStone);
		InsertListString("theAddScroll.theItemID %d theAddScroll.theBundle %d",
			( ( SRpScMakeAddAccessory *)pPacket )->theAddScroll.theItemID, 
			( ( SRpScMakeAddAccessory *)pPacket )->theAddScroll.theBundle);
		break;
	case n_RpScOccupationTime:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpScOccupationTime");
		InsertListString("theField %d theRemainTime %u",
			( ( SRpScOccupationTime *)pPacket )->theField,
			( ( SRpScOccupationTime *)pPacket )->theRemainTime);		
		break;
    }

#endif

}

void  CPacketTransLogDlg::PrintCtoSPacketInfo(void * pPacket,
											  char * pstrTimeInfo, char * pstrDirInfo)
{
	
#ifdef _DEBUG
	
	SRpHeader* header = ( SRpHeader* ) pPacket;	
	
	switch(header->theProtocol)
	{
		//클라이언트에서 서버로 보내는 패킷
		//-----------------------------------------------------------
		// Framework Related
		//-----------------------------------------------------------
	case n_RpCsAccountLogin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsAccountLogin");
		InsertListString("theAccountID: %s  thePasswd: %s  theVersion: %d",
		((SRpCsAccountLogin *)pPacket)->theAccountID,
		((SRpCsAccountLogin *)pPacket)->thePasswd,
		((SRpCsAccountLogin *)pPacket)->theVersion);
		break;
	case n_RpCsGameLogin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsGameLogin");
		InsertListString("theAccountID: %s", ((SRpCsGameLogin *)pPacket)->theAccountID);
		break;
		/*
	case n_RpCsSelectWorld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsSelectWorld");
		InsertListString("theWorldIndex: %d", ((SRpCsSelectWorld *)pPacket)->theWorldIndex);
		break;
		*/

	case n_RpCsSelectCharacter:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsSelectCharacter");
		InsertListString("thePCName: %s", ((SRpCsSelectCharacter *)pPacket)->thePCName);
		break;
	case n_RpCsPCLogin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPCLogin");
		break;
	case n_RpCsPCLogout:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPCLogout");
		InsertListString("theLogoutType: %d", ((SRpCsPCLogout *)pPacket)->theLogoutType);
		break;
	case n_RpCsCreateCharacter:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsCreateCharacter");
		InsertListString("theCore: %d  thePCName: %s",
		((SRpCsCreateCharacter *)pPacket)->theCore,
		((SRpCsCreateCharacter *)pPacket)->thePCName);
		break;
	case n_RpCsDeleteCharacter:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsDeleteCharacter");
		InsertListString("thePCName: %s", ((SRpCsDeleteCharacter *)pPacket)->thePCName);
		break;
		//-----------------------------------------------------------
		// Other Game Protocols
		//-----------------------------------------------------------
	case n_RpCsStartMove:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsStartMove");
		InsertListString("theDestiCell: %d  theMoveSpeed: %d",
        ((SRpCsStartMove *)pPacket)->theDestiCell,
		((SRpCsStartMove *)pPacket)->theMoveSpeed);*/
		break;
	case n_RpCsMove:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMove");
		InsertListString("theCell: %d", ((SRpCsMove *)pPacket)->theCell);*/
		break;
	case n_RpCsStop:
		/*InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsStop");
		InsertListString("theCell: %d", ((SRpCsStop *)pPacket)->theCell);*/
		break;
	case n_RpCsAttack:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsAttack");
		InsertListString("theAttackedID: %d  theAttackedType: %d  theComboType: %d",
		((SRpCsAttack *)pPacket)->theAttackedID,
		((SRpCsAttack *)pPacket)->theAttackedType,
		((SRpCsAttack *)pPacket)->theComboType);		
		InsertListString("theComboSeq: %d  theComboCount: %d  theComboStatus: %d",
		((SRpCsAttack *)pPacket)->theComboSeq,
		((SRpCsAttack *)pPacket)->theComboCount,
		((SRpCsAttack *)pPacket)->theComboStatus);
		InsertListString("theMotion: %d",
		((SRpCsAttack *)pPacket)->theMotion);
		break;
	case n_RpCsRequestWarp:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsRequestWarp");
		InsertListString("theGateIndex: %d", ((SRpCsRequestWarp *)pPacket)->theGateIndex);
		break;
	case n_RpCsReadyWarp:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsReadyWarp");
		break;
	case n_RpCsSkill:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsSkill");
		InsertListString("theTarget: %d  theTargetType: %d  theSkillCode:  %d",
		((SRpCsSkill *)pPacket)->theTarget,
		((SRpCsSkill *)pPacket)->theTargetType,
		((SRpCsSkill *)pPacket)->theSkillCode);
		break;
	case n_RpCsSpelling:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsSpelling");
		InsertListString("theSpellingCount: %d", ((SRpCsSpelling *)pPacket)->theSpellingCount);
		break;
	case n_RpCsAlive:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsAlive");
		break;
	case n_RpCsHotKey:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsHotKey");
		InsertListString("theKeyType: %d  theSkillCode: %d",
		((SRpCsHotKey *)pPacket)->theKeyType,
		((SRpCsHotKey *)pPacket)->theSkillCode);
		break;
	case n_RpCsReqCharInfo:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsReqCharInfo");
		InsertListString("theID: %d  theType: %d",
		((SRpCsReqCharInfo *)pPacket)->theID,
		((SRpCsReqCharInfo *)pPacket)->theType);
		break;
	case n_RpCsKeepUpSkillEnd:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsKeepUpSkillEnd");
		InsertListString("theSkillCode: %d", ((SRpCsKeepUpSkillEnd *)pPacket)->theSkillCode);
		break;
//	case n_RpCsGetSkill:
//		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsGetSkill");
//		InsertListString("theSkillCode: %d", ((SRpCsGetSkill *)pPacket)->theSkillCode);
//		break;		
		//-----------------------------------------------------------
		// Chatting Protocols
		//-----------------------------------------------------------
	case n_RpCsSay:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsSay");
		InsertListString("theMessage: %s", ((SRpCsSay *)pPacket)->theMessage);
		break;
	case n_RpCsWhisper:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsWhisper");
		InsertListString("theReceiver: %s  theMessage: %s",
		((SRpCsWhisper *)pPacket)->theReceiver,
		((SRpCsWhisper *)pPacket)->theMessage);
		break;				   
		
		//--------------------------------------------------
		// Item Protocols
		//--------------------------------------------------
	case n_RpCsPickup:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPickup");
		InsertListString("theItemID: %d", ((SRpCsPickup *)pPacket)->theItemID);
		break;		
	case n_RpCsDrop:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsDrop");
		InsertListString("theItemID: %d  theBundle: %d  theItemTab: %d",
		((SRpCsDrop *)pPacket)->theItemID,
		((SRpCsDrop *)pPacket)->theBundle,
		((SRpCsDrop *)pPacket)->theItemTab);
		break;
	case n_RpCsDropEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsDropEld");
		InsertListString("theEld: %d", ((SRpCsDropEld *)pPacket)->theEld);
		break;
	case n_RpCsUse:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsUse");
		InsertListString("theItemID: %d", ((SRpCsUse *)pPacket)->theItemID);
		break;
	case n_RpCsEquip:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsEquip");
		InsertListString("theItemID: %d  thePosition: %d",
		((SRpCsEquip *)pPacket)->theItemID,
		((SRpCsEquip *)pPacket)->thePosition);
		break;
	case n_RpCsTakeoff:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsTakeoff");
		InsertListString("theItemID: %d  thePosition: %d",
		((SRpCsTakeoff *)pPacket)->theItemID,
		((SRpCsTakeoff *)pPacket)->thePosition);
		break;
	case n_RpCsBuy:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsBuy");
		InsertListString("theNPCID: %d  theItemCode: %d  theBundle: %d  thePosition: %d",
		((SRpCsBuy *)pPacket)->theNPCID,
		((SRpCsBuy *)pPacket)->theItemCode,
		((SRpCsBuy *)pPacket)->theBundle,
		((SRpCsBuy *)pPacket)->thePosition);
		break;
	case n_RpCsSell:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsSell");
		InsertListString("theItemID: %d  theBundle: %d  theItemTab: %d  theNPCID: %d",
		((SRpCsSell *)pPacket)->theItemID,
		((SRpCsSell *)pPacket)->theBundle,
		((SRpCsSell *)pPacket)->theItemTab,
		((SRpCsSell *)pPacket)->theNPCID);
		break;
	case n_RpCsCombine:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsCombine");
		InsertListString("theItemID1: %d  theItemID2: %d  theItemTab: %d",
		((SRpCsCombine *)pPacket)->theItemID1,
		((SRpCsCombine *)pPacket)->theItemID2,
		((SRpCsCombine *)pPacket)->theItemTab);
		break;
	case n_RpCsChangeSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsChangeSlot");
		InsertListString("theItemID1: %d  theItemID2: %d  theSlot: %d  theItemTab: %d",
		((SRpCsChangeSlot *)pPacket)->theItemID1,
		((SRpCsChangeSlot *)pPacket)->theItemID2,
		((SRpCsChangeSlot *)pPacket)->theSlot,
		((SRpCsChangeSlot *)pPacket)->theItemTab);
		break;
	case n_RpCsLinkQuickSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsLinkQuickSlot");
		InsertListString("theType: %d  thePosition: %d",
		((SRpCsLinkQuickSlot *)pPacket)->theQuickSlot.theType,
		((SRpCsLinkQuickSlot *)pPacket)->theQuickSlot.thePosition);
		break;
	case n_RpCsUnlinkQuickSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsUnlinkQuickSlot");
		InsertListString("thePosition: %d", ((SRpCsUnlinkQuickSlot *)pPacket)->thePosition);
		break;
		//--------------------------------------------------
		// Exp, LevelUp Protocols
		//--------------------------------------------------
		
	case n_RpCsPoint:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPoint");
		break;
		//--------------------------------------------------
		// Npc Chatting, Script, Quest Protoclos
		//--------------------------------------------------
	case n_RpCsScriptStart:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsScriptStart");
		InsertListString("theNPCID: %d", ((SRpCsScriptStart *)pPacket)->theNPCID);
		InsertListString("theScriptCode: %d", ((SRpCsScriptStart *)pPacket)->theScriptCode);
		break;
	case n_RpCsScriptEnd:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsScriptEnd");
		InsertListString("theNPCCode: %d", ((SRpCsScriptEnd *)pPacket)->theScriptCode);
		break;	
	case n_RpCsAnswerScript:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsAnswerScript");
		InsertListString("theAnswer: %d", ((SRpCsAnswerScript *)pPacket)->theAnswer);
		break;		   
		//--------------------------------------------------
		// Quest
		//--------------------------------------------------
	case n_RpCsGiveupQuest:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsGiveupQuest");
		InsertListString("theQuestCode: %d", ((SRpCsGiveupQuest *)pPacket)->theQuestCode);
		break;	
		//--------------------------------------------------
		// Archer
		//--------------------------------------------------
	case n_RpCsArcherReady:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsArcherReady");
		InsertListString("theTarget: %d  theTargetType: %d",
		((SRpCsArcherReady *)pPacket)->theTarget,
		((SRpCsArcherReady *)pPacket)->theTargetType);
		break;	   
		
		//--------------------------------------------------
		// Delay Reason //by simwoosung
		//--------------------------------------------------	
	case n_RpCs10SecDelay:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCs10SecDelay");
		InsertListString("theDelayReason: %d", ((SRpCs10SecDelay *)pPacket)->theDelayReason);
		break; 		   
		
		//--------------------------------------------------
		// Party Protocols                   //by simwoosung
		//--------------------------------------------------
	case n_RpCsPartyAskJoin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyAskJoin");
		InsertListString("thePCName: %s", ((SRpCsPartyAskJoin *)pPacket)->thePCName);
		break; 
	case n_RpCsPartyAnsJoin:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyAnsJoin");
		InsertListString("theLeaderName: %s  theAnswer: %d",
		((SRpCsPartyAnsJoin *)pPacket)->theLeaderName,
		((SRpCsPartyAnsJoin *)pPacket)->theAnswer);
		break; 
	case n_RpCsPartyLeave:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyLeave");
		InsertListString("thePCName: %s", ((SRpCsPartyLeave *)pPacket)->thePCName);
		break; 
	case n_RpCsPartyChangeName:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyChangeName");
		InsertListString("thePartyName: %s", ((SRpCsPartyChangeName *)pPacket)->thePartyName);
		break; 
	case n_RpCsPartyChat:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyChat");
		InsertListString("theString: %s", ((SRpCsPartyChat *)pPacket)->theString);
		break; 
	case n_RpCsPartyChangeLeader:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyChangeLeader");
		InsertListString("theNewLeaderName: %s", ((SRpCsPartyChangeLeader *)pPacket)->theNewLeaderName);
		break; 
	case n_RpCsPartyItemMgr:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyItemMgr");
		InsertListString("theItemMgr: %d", ((SRpCsPartyItemMgr *)pPacket)->theItemMgr);
		break; 
	case n_RpCsPartyGiveGage:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPartyGiveGage");
		InsertListString("thePCID: %d  theGage: %d",
		((SRpCsPartyGiveGage *)pPacket)->thePCID,
		((SRpCsPartyGiveGage *)pPacket)->theGage);
		break; 
	case n_RpCsCloseBank:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsCloseBank");		
		break; 
	case n_RpCsDeposit:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsDeposit");
		InsertListString("theItemID=%d, theBundle=%d, theBankSlot=%d",
		((SRpCsDeposit *)pPacket)->theItemID, 
		((SRpCsDeposit *)pPacket)->theBundle, 
		((SRpCsDeposit *)pPacket)->theBankSlot);
		break; 
	case n_RpCsDraw:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsDraw");
		InsertListString("theBankItemID=%d, theBundle=%d",
		((SRpCsDraw *)pPacket)->theBankItemID, 
		((SRpCsDraw *)pPacket)->theBundle);
		break; 
	case n_RpCsDepositEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsDepositEld");
		InsertListString("theEld=%d",
		((SRpCsDepositEld *)pPacket)->theEld);
		break; 
	case n_RpCsDrawEld:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsDrawEld");
		InsertListString("theEld=%d",
		((SRpCsDrawEld *)pPacket)->theEld);
		break; 
	case n_RpCsWHChangeSlot:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsWHChangeSlot");
		InsertListString("theBankItemID=%d, theSlot=%d",
		((SRpCsWHChangeSlot *)pPacket)->theBankItemID, 
		((SRpCsWHChangeSlot *)pPacket)->theSlot);
		break;
		
		//-----------------------------------------------------------
		// 
		//-----------------------------------------------------------
	case n_RpCsGMCmd:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsGMCmd");
		InsertListString("theNParam[0]: %d  theNParam[1]: %d  theNParam[2]: %d  theNParam[3]: %d  theSParam: %s",
        ((SRpCsGMCmd *)pPacket)->theNParam[0],
		((SRpCsGMCmd *)pPacket)->theNParam[1],
		((SRpCsGMCmd *)pPacket)->theNParam[2],
		((SRpCsGMCmd *)pPacket)->theNParam[3],
		((SRpCsGMCmd *)pPacket)->theSParam);
		break; 	

	case n_RpCsSetStone:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsSetStone");
		InsertListString("ItemID : %d  EldaID : %d ",
			(( SRpCsSetStone *)pPacket)->theItemID,
			(( SRpCsSetStone *)pPacket)->theStoneID);
		break;

	case n_RpCsEldacraftGame:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsEldacraftGame");
		InsertListString("Cells: %d-%d-%d-%d-%d Miss100: %d Miss50: %d Combo : %d  Score : %d  Ending : %d",
			(( SRpCsEldacraftGame *)pPacket)->theCellList[0].theType,			
			(( SRpCsEldacraftGame *)pPacket)->theCellList[1].theType,
			(( SRpCsEldacraftGame *)pPacket)->theCellList[2].theType,
			(( SRpCsEldacraftGame *)pPacket)->theCellList[3].theType,
			(( SRpCsEldacraftGame *)pPacket)->theCellList[4].theType,
			(( SRpCsEldacraftGame *)pPacket)->theMissCount_100,
			(( SRpCsEldacraftGame *)pPacket)->theMissCount_50,
			(( SRpCsEldacraftGame *)pPacket)->theCombo,  
			(( SRpCsEldacraftGame *)pPacket)->theScore,   
			(( SRpCsEldacraftGame *)pPacket)->theEndingFlag   
			);
		break;
	case n_RpCsRequstRepair:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsRequstRepair");
		InsertListString("NPC/Skill=%d (1:NPC, 2:SKILL)  count=%d Skillcode=%d", 		
			(( SRpCsRequstRepair *)pPacket)->theRepairType,
			(( SRpCsRequstRepair *)pPacket)->theCount ,
			(( SRpCsRequstRepair *)pPacket)->theSkillCode
			);
		break;
	case n_RpCsMakeItem:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMakeItem");
		InsertListString("NPC/Skill=%d (1:NPC, 2:SKILL) 0=%d 1=%d 2=%d 3=%d ", 		
			(( SRpCsMakeItem *)pPacket)->theType,
			(( SRpCsMakeItem *)pPacket)->theItem[0].theItemID,
			(( SRpCsMakeItem *)pPacket)->theItem[1].theItemID,
			(( SRpCsMakeItem *)pPacket)->theItem[2].theItemID,
			(( SRpCsMakeItem *)pPacket)->theItem[3].theItemID			
			);
		break;
	///-----------------------------------------------------------------------
	///-- community Protocols 
	///-----------------------------------------------------------------------
	case n_RpCsMsngrMemberInsert:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrMemberInsert");
		InsertListString("thePCName %s", ((SRpCsMsngrMemberInsert *)pPacket)->thePCName );
		break;
	case n_RpCsMsngrMemberInsertAns:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrMemberInsertAns");
		InsertListString("theYesNoFlag %d   thePCName %s", 		
			(( SRpCsMsngrMemberInsertAns *)pPacket)->theYesNoFlag,
			(( SRpCsMsngrMemberInsertAns *)pPacket)->thePCName			
			);
		break;
	case n_RpCsMsngrMemberDelete:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrMemberDelete");
		InsertListString("thePCName %s", ((SRpCsMsngrMemberDelete *)pPacket)->thePCName );
		break;
	case n_RpCsMsngrMemberBlock:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrMemberBlock");
		InsertListString("theProcType %d   thePCName %s", 		
			(( SRpCsMsngrMemberBlock *)pPacket)->theProcType,
			(( SRpCsMsngrMemberBlock *)pPacket)->thePCName			
			);
		break;
	case n_RpCsMsngrChangeOpt:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrChangeOpt");
		InsertListString("theOptState %d theOptMapFlag %d theOptLevelFlag %d theOptJobFlag %d", 		
			(( SRpCsMsngrChangeOpt *)pPacket)->theOptState,
			(( SRpCsMsngrChangeOpt *)pPacket)->theOptMapFlag ,
			(( SRpCsMsngrChangeOpt *)pPacket)->theOptLevelFlag ,	
			(( SRpCsMsngrChangeOpt *)pPacket)->theOptJobFlag 	
			);
		break;
	case n_RpCsMsngrSay:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrSay");
		InsertListString("theToPCName %s  theStr %s", 
			((SRpCsMsngrSay *)pPacket)->theToPCName,
			((SRpCsMsngrSay *)pPacket)->theStr
			);
		break;
	case n_RpCsMsngrChatCreateRoom:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrChatCreateRoom");
		break;
	case n_RpCsMsngrChatExitRoom:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrChatExitRoom");
		break;
	case n_RpCsMsngrChatInviteAsk:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrChatInviteAsk");
		InsertListString("theToPCName %s", ((SRpCsMsngrChatInviteAsk *)pPacket)->theToPCName );
		break;
	case n_RpCsMsngrChatInviteAns:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrChatInviteAns");
		InsertListString("theYesNoFlag %d  theFromPcName %s theOwnerPCName %s", 
			((SRpCsMsngrChatInviteAns *)pPacket)->theYesNoFlag,
			((SRpCsMsngrChatInviteAns *)pPacket)->theFromPCName,
			((SRpCsMsngrChatInviteAns *)pPacket)->theOwnerPCName
			);
		break;
	case n_RpCsMsngrChatSay:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMsngrChatSay");
		InsertListString("theStr %s", ((SRpCsMsngrChatSay *)pPacket)->theStr );
		break;	
		
	case n_RpCsOnRestoreOK:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsOnRestoreOK");		
		break;	
		
	case n_RpCsOnInactivated:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsOnInactivated");		
		break;	
	case n_RpCsPKMode:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsPKMode");
		InsertListString("thePKFlag %d", ((SRpCsPKMode *)pPacket)->thePKFlag );
		break;
	case n_RpCsMakeAddAccessory:
		InsertListString("%s  %s:  %s", pstrTimeInfo, pstrDirInfo, "n_RpCsMakeAddAccessory");
		InsertListString("theAddAccessoryUID %d  theAddType %d theAddScrollUID %d", 
			((SRpCsMakeAddAccessory *)pPacket)->theAddAccessoryUID,
			((SRpCsMakeAddAccessory *)pPacket)->theAddType,
			((SRpCsMakeAddAccessory *)pPacket)->theAddScrollUID
			);
     }

#endif

}

void	CPacketTransLogDlg::PrintDebugLog(char *clog, ... )
{
	if(m_hMainDlg)
	{		
		if(!m_IsPrintLog)
			return;
		
		static char tmp_buf[1024];
		
		time_t tval;
		time( &tval );
		struct tm *currTM = localtime( &tval );
		sprintf( tmp_buf, "[%d/%02d/%02d %02d:%02d:%02d]", currTM->tm_year+1900, 
			currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec);	
		
		static char tmp_buf2[1024];
		va_list args;
		
		va_start( args, clog );
		vsprintf( tmp_buf2, clog, args );
		va_end( args );		
		
		strcat(tmp_buf, tmp_buf2);		
		strcat(tmp_buf, "\r\n");

		int ilength = GetWindowTextLength(m_hDebLog);

		SendMessage(m_hDebLog, EM_SETSEL, ilength, ilength);
		SendMessage(m_hDebLog, EM_REPLACESEL, 0, (LPARAM)tmp_buf);	
		
		int iLimitLength = m_nMaxStrLength * 0.8;
		if( iLimitLength < ilength )
		{
		   SendMessage(m_hDebLog, EM_SETSEL, 0, iLimitLength);
		   SendMessage(m_hDebLog, EM_REPLACESEL, 0, (LPARAM)"");	
		}
	}
}

void CPacketTransLogDlg::SaveLogFile()
{
	if(m_hMainDlg)
	{
		return;
		FILE* fp = fopen("DlgLog.txt", "w");
		if(!fp) return;

		static char sTempbuf[100000] = "";
		HWND m_hEdit = m_hEditLog;	
		
		if( m_nPrintType == NDebugPrint )
		{
			m_hEdit = m_hDebLog;
		}

		GetWindowText(m_hEdit, sTempbuf, m_nMaxStrLength );	

		if( strcmp(sTempbuf,"") != 0 )
		{
			fwrite(sTempbuf, sizeof(char)*m_nMaxStrLength, 1, fp);	
		}	
			
		fclose( fp );
	}
}