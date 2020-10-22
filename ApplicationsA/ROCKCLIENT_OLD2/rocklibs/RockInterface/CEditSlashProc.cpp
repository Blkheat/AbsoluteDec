// EditSlashProc.cpp: implementation of the CEditSlashProc class.
//
//////////////////////////////////////////////////////////////////////

#include "RockPCH.h"
#include  <ctype.h>
#include "..\\..\\RockClient.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"

#include "UiUtil.h"
#include "FrameWnd.h"
#include "Fontasm.h"
#include "EditBox.h"
#include "CAction.h"
#include "ChatBoxWndProc.h"

#include "ExchangeWndProc.h"
#include "PartyInforWnd.h"

#include "CEditSlashProc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEditSlashProc g_CEditSlashProc; 

CEditSlashProc::CEditSlashProc()
{
    m_BCheckSlashCommand = FALSE;
    m_pEditBox = NULL;
	SAFE_STR_CPY(m_TstrBuf,_RT(""), 256 );
	SAFE_STR_CPY(m_TWhisDest, _RT(""), 256 );
	SAFE_STR_CPY(m_LeavePcName,_RT(""), 256 );
	SAFE_STR_CPY(m_TInviteDest, _RT(""), 256 );
	SAFE_STR_CPY(m_TPartyName, _RT(""), 256  );
	SAFE_STR_CPY(m_TGiveLeader, _RT(""), 256 );
	SAFE_STR_CPY(m_TChangeHowRoot, _RT(""), 256 );

	m_nChatCommandType  =  nNormalChatCommand;
	m_nExcuteCommandType =  nNoneCommand; 
	m_Tcolor = D3DCOLOR_XRGB( 255, 255, 255 );
}

CEditSlashProc::~CEditSlashProc()
{

}

int CEditSlashProc::GetSizeText(HWND hEdit, int iTextLimitLen)
{ 
	TCHAR strBuf[256] = {0,};
#ifdef UNICODE_ROCK
	GetWindowTextW( hEdit, RWCHAR(strBuf), iTextLimitLen + 1 );
#else
	GetWindowText( hEdit, strBuf, iTextLimitLen + 1 );
#endif
	
	int iSize = Rstrlen( strBuf );
    return iSize;
}

void CEditSlashProc::CheckSlashCommand(HWND hEdit, int iTextLimitLen)
{
    m_nExcuteCommandType =  nNoneCommand;
	m_BCheckSlashCommand =  FALSE;
	ZeroMemory(m_TstrBuf , MAX_SIZE_256);

#ifdef UNICODE_ROCK
	GetWindowTextW( hEdit, RWCHAR(m_TstrBuf), iTextLimitLen + 1 );
#else
	GetWindowText( hEdit, m_TstrBuf, iTextLimitLen + 1 );
#endif
	

	int iIdx = 0;
	int iLen = 0;
	int i = 0;
	RTCHAR TCh[3] = {0,};
	char Ch[3] = "";

#ifdef UNICODE_ROCK
    TCh[0] = m_TstrBuf[i];
	if(  Rstrcmp(TCh, _RT("/")) == 0 )
       m_BCheckSlashCommand = TRUE;
	else
       m_BCheckSlashCommand = FALSE;
#else
    DetectionDBCS( &i, m_TstrBuf, Ch, FALSE );
	if(EqualString(Ch, "/"))
       m_BCheckSlashCommand = TRUE;
	else
       m_BCheckSlashCommand = FALSE;
#endif

	i++;
    
    if(m_BCheckSlashCommand)
	   CheckCommandType(i);	
}

void CEditSlashProc::CheckCommandType(int index)
{
    	
	TCHAR strTemp[256] = {0,};

	BOOL bFind = FindParameter(strTemp, index, TRUE, " ");
    
	if(bFind)
	{
	  if(CheckChatCommandType(strTemp, index))
	    return;	    
	}
	
	CheckExcuteCommandType(strTemp, index);	
}

BOOL CEditSlashProc::FindParameter(TCHAR * strTemp, int & index, BOOL BFlag , char *strCmp)
{
   	int iIdx = 0;
	int iLen = Rstrlen( RWCHAR(m_TstrBuf));	
		
	RTCHAR TCh[3] =	{0,};
	char Ch[3] = "";	

	BOOL BfoundFirstCmp = FALSE;
	BOOL BfoundSecondCmp = FALSE;
	BOOL BfoundSecondSpace =FALSE;

	int i = 0;
	for( i = index; i < iLen ; i++)
	{
#ifdef UNICODE_ROCK
		iIdx = (int)m_TstrBuf[i];
		if(BFlag)
		{
			if(islower(iIdx))
				iIdx -= 32;
		}	  
		TCh[0] = iIdx;
		if( !Rstrcmp( TCh, strCmp) && !BfoundFirstCmp )
			BfoundFirstCmp = TRUE;
		
		if(BfoundFirstCmp)
		{
			if( Rstrcmp( TCh, strCmp) == 0 )
			{
				BfoundSecondSpace = TRUE;
				break;
			}
			else
			{
				Rstrcat(RWCHAR(strTemp), RWCHAR(TCh));		  
			}
		}
		
#else
		DetectionDBCS( &i, m_TstrBuf, Ch, BFlag);
		if( !EqualString(Ch, " ") && !BfoundFirstCmp )
			BfoundFirstCmp = TRUE;
		if(BfoundFirstCmp)
		{
			if(EqualString(Ch, strCmp))
			{
				BfoundSecondCmp = TRUE;
				break;
			}
			else
			{
				Rstrcat(RWCHAR(strTemp), RWCHAR(Ch));		  
			}
		}
#endif      
	}

	index = i;
  
    return BfoundSecondCmp;
}

int CEditSlashProc::CheckChatCommandType(TCHAR *strCmd, int index)
{
    if( (Rstrcmp(strCmd, _RT("S") ) == 0) )// || (_tcscmp(strCmd, _T("ㄴ") ) == 0) )
	{
		CheckNormalChat();
	    return 1;
	}

	if( (Rstrcmp(strCmd, _RT("W") ) == 0) )// || (_tcscmp(strCmd, _T("ㅈ") ) == 0) )
	{
		CheckWisperChat(index);
	    return 1;
	}

	//외치기는 차후에 작업
	/*if( _tcscmp(strCmd, _T("Y") ) == 0  )
	{
		CheckYellChat();
	    return 1;
	}*/

	if( (Rstrcmp(strCmd, _RT("P") ) == 0) )// || (_tcscmp(strCmd, _T("ㅔ") ) == 0) )
	{
		CheckPartyChat();
	    return 1;
	}

	return 0;
}

void CEditSlashProc::CheckExcuteCommandType(TCHAR *strCmd, int index)
{    
	m_nExcuteCommandType = nNoSupportCommand;	

	return;
}

void CEditSlashProc::CheckNormalChat()
{
   m_nChatCommandType = nNormalChatCommand;
   m_pEditBox->ClearText();
   m_Tcolor = D3DCOLOR_XRGB( 255, 255, 255 );
   m_pEditBox->SetTextColor(255, 255, 255);

   ChatBoxWnd.StateImg->SetClientImage( TID_None );
   ChatBoxWnd.ChatState = n_NormarChat;   
}

void CEditSlashProc::CheckWisperChat(int index)
{
    SAFE_STR_CPY(m_TWhisDest, _RT(""), 256 );

  	BOOL bFind = FindParameter(m_TWhisDest, index, FALSE, " ");

	if(bFind)
      CheckWhisperChatSecond();

}

void CEditSlashProc::CheckWhisperChatSecond()
{
    m_nChatCommandType = nWhisperChatCommand;
    m_pEditBox->ClearText();
    m_Tcolor = D3DCOLOR_XRGB( 255, 122, 255 );
    m_pEditBox->SetTextColor(255, 122, 255 );
	
	ChatBoxWnd.StateImg->SetClientImage( TID_Chat_ChatType_Whisper );
	ChatBoxWnd.ChatState = n_WhisperChat;	
}

void CEditSlashProc::CheckYellChat()
{
   m_nChatCommandType = nYellChatCommand;
   m_pEditBox->ClearText();
   m_Tcolor = D3DCOLOR_XRGB( 255, 255, 122 );
   m_pEditBox->SetTextColor(255, 255, 122 );  

}

void CEditSlashProc::CheckPartyChat()
{
   m_nChatCommandType = nPartyChatCommand;
   m_pEditBox->ClearText();
   m_Tcolor = D3DCOLOR_XRGB( 122, 255, 255 );
   m_pEditBox->SetTextColor(122, 255, 255);   

   ChatBoxWnd.StateImg->SetClientImage( TID_Chat_ChatType_Party );
   ChatBoxWnd.ChatState = n_PartyChat;  
}

void CEditSlashProc::CheckInviteParty(int index)
{
   SAFE_STR_CPY( m_TInviteDest, _RT(""), 256 );
    
   FindParameter(m_TInviteDest, index, FALSE, " ");   
   CheckInvitePartySecond();
}

void CEditSlashProc::CheckInvitePartySecond()
{
   m_nExcuteCommandType =  nInvitePatryCommand;
}

void CEditSlashProc::CheckLeaveParty(int index)
{
   SAFE_STR_CPY( m_LeavePcName, _RT(""), 256 );
    
   FindParameter(m_LeavePcName, index, FALSE, " ");   
   CheckLeavePartySecond();
}

void CEditSlashProc::CheckLeavePartySecond()
{
   m_nExcuteCommandType =  nLeavePartyCommand;
}

void CEditSlashProc::CheckChangePName(int index)
{
  	SAFE_STR_CPY(m_TPartyName, _RT(""), 256);

	FindParameter(m_TPartyName, index, FALSE, "\\");    
    CheckChangePNameSecond();
}

void CEditSlashProc::CheckChangePNameSecond()
{
    m_nExcuteCommandType =  nChangePnameCommand;
}

void CEditSlashProc::CheckGiveLearder(int index)
{
   	SAFE_STR_CPY(m_TGiveLeader, _RT(""), 256);

	FindParameter(m_TGiveLeader, index, FALSE, " ");
	CheckGiveLeaderSecond();
}

void CEditSlashProc::CheckGiveLeaderSecond()
{
   m_nExcuteCommandType =  nGiveLearderCommand;
}

void CEditSlashProc::CheckChangeHowRoot(int index)
{
    SAFE_STR_CPY(m_TChangeHowRoot, _RT(""), 256);
	FindParameter(m_TChangeHowRoot, index, FALSE, " ");
    CheckChangeHowRootSecond();
}

void CEditSlashProc::CheckChangeHowRootSecond()
{ 
   m_nExcuteCommandType = nChangeHowRootCommand;
}

void CEditSlashProc::CheckExchangeEnable( int aIindex )
{ 
   m_nExcuteCommandType = nExchangeEnableCommand;
}

void CEditSlashProc::CheckExchangeNotEnable( int aIindex )
{ 
   m_nExcuteCommandType = nExchangeNotEnableCommand;
}		


int CEditSlashProc::DetectionDBCS( int* Count, char* Str, char *Db, BOOL bFlag )
{
   	static char Ch;
	int *Idx;

    //------------------------------------------------------------
    //첫바이트가 0x80이상이면 DBCS....
    //------------------------------------------------------------
    if( IsDBCSLeadByteEx( CP_ACP, Str[*Count] ) )
    {	//DBCS
	    Db[0] = Str[*Count];  
	    Db[1] = Str[(*Count)+1];
		Db[2] = 0;

	    Idx = (int*)Db;
	    (*Count)++;
    }
    else
    {   //ASCII
	    Ch    = Str[(*Count)];
        if(bFlag)
		{
			if(islower(Ch))
		    Ch -= 32;
		}

	    Db[0] = Ch;
	    Db[1] = 0;

	    Idx = (int*)Db;
    }
    
    return( *Idx );
}

BOOL CEditSlashProc::EqualString(char *str1, char *str2)
{
   if(strcmp(str1, str2) == 0)
	   return TRUE;

   return FALSE; 
}

int  CEditSlashProc::RenderTitle(int Ax, int Ay, CFontg * theFontg, int & CaretH)
{
    static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );

	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;

	NumStr = theFontg->Confirm( m_Title, IdxBuf );
	
	for(int i = 0; i < NumStr; ++i  )
	{
	   RFont = theFontg->GetFontasm( IdxBuf[i] );

	   if( RFont != NULL )
	   {
			RFont->vt[0].diffuse	= m_Tcolor;
			RFont->vt[1].diffuse	= m_Tcolor;
			RFont->vt[2].diffuse	= m_Tcolor;
			RFont->vt[3].diffuse	= m_Tcolor;

			RFont->vt[0].x = (float)Ax - 0.5f;
			RFont->vt[0].y = (float)Ay - 0.5f;
			RFont->vt[1].x = RFont->vt[0].x + RFont->w;
			RFont->vt[1].y = RFont->vt[0].y;
			RFont->vt[2].x = RFont->vt[0].x + RFont->w;
			RFont->vt[2].y = RFont->vt[0].y + RFont->h;
			RFont->vt[3].x = RFont->vt[0].x;
			RFont->vt[3].y = RFont->vt[0].y + RFont->h;

			Ax = Ax + RFont->w;

			if( ( IdxBuf[i] != 32 ) )
			{
				//" " 렌더링 하지 않는다.
				//"\n"는 그대로 찍는다( Line 출력이기 때문에 개행은 처리되지 않는다. )
				// d3d8에서는 시저테스트가 없기 때문에...경계선을 넘어가면..출력하지 않는다.
				Render->DrawFont( RFont );
			}	
	   }
	}
	
	CaretH = RFont->h;
	
	return Ax;
}

int CEditSlashProc::ProcessExcuteCommand(HWND HEdit)
{
	int  iRe = 0;

	if( m_BCheckSlashCommand )
	{
		char TempStr[ 1024 ] = "";
	
		m_pEditBox->GetText( TempStr , 1024 );
		m_pEditBox->ClearText();

		iRe = 1;

		switch( m_nExcuteCommandType )
		{
		case nNoneCommand:				iRe = 0; 
			break;
		case nInvitePatryCommand:		ExcuteInvitePatry();   
			break;
		case nLeavePartyCommand:		ExcuteLeaveParty();
			break;
		case nChangePnameCommand:		ExcuteChangePname();
			break;
		case nGiveLearderCommand:		ExcuteGiveLearder();
			break;
		case nChangeHowRootCommand:		ExcuteChangeHowRoot();
			break;
		case nExchangeEnableCommand:	ExcuteExchangeEnable();
			break;
		case nExchangeNotEnableCommand: ExcuteExchangeNotEnable();
			break;
		default:                   			                        

			LPACTION_DATA pActionData = NULL;

			//................................................................................................
			// 플레이어 행동 처리부분 By wxywxy
			//................................................................................................
			if( ( pActionData = g_RockClient.m_Action.CheckKeyWord( ( char * ) TempStr ) ) != NULL )
			{
				switch( g_Pc.GetPlayer()->m_curt_event.type )
				{
					case NULL:
					case SM_SIMSIM:
		
						g_RockClient.Send_CsExpression( g_Pc.GetPlayer()->GetUnique() , ( BYTE ) pActionData->Index );
						
						break;
				}
			}
			//................................................................................................
			break;
		}
		
		m_BCheckSlashCommand = FALSE;
		m_nExcuteCommandType = nNoneCommand;	  
	}
	
	return iRe;
}

void CEditSlashProc::SetInviteDestName(TCHAR *destName)
{
	SAFE_STR_CPY( m_TInviteDest, destName, 256 );
}

void CEditSlashProc::ExcuteInvitePatry()
{
  
}

void CEditSlashProc::ExcuteLeaveParty()
{

}

void CEditSlashProc::ExcuteChangePname()
{

}

void CEditSlashProc::ExcuteGiveLearder()
{

}

void CEditSlashProc::ExcuteChangeHowRoot()
{
    
}

void CEditSlashProc::ExcuteExchangeEnable()
{
	g_RockClient.Send_RpCsExchange( (BYTE)n_OPEN );

	g_ExchangeWnd.m_bIsExchangeEnable = TRUE;

	return;
}

void CEditSlashProc::ExcuteExchangeNotEnable()
{
	g_RockClient.Send_RpCsExchange( (BYTE)n_CLOSE );
	
	g_ExchangeWnd.m_bIsExchangeEnable = FALSE;

	return;
}


