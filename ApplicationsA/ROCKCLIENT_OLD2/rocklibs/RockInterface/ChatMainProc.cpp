#include "Rui.h"

#include "..\\RockPCH.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\GTimer.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\bravolibs\\obj\\player.h"


#include "UiUtil.h"

#include "ChatMainProc.h"
#include "stringmanager.h"

extern bool g_IsHelpView;

CChatMainProc g_ChatMainProc;

CChatMainProc::CChatMainProc()
{
	m_OutState = COutState_None;
	
	Init();
}

CChatMainProc::~CChatMainProc()
{

}

void   CChatMainProc::Init()
{
	m_nInsertIndex = 0;

	memset( m_ChatTxtData, 0, sizeof(SChatTxtData) * (MAX_CHATDATA_NUM + 1) );
	memset( m_pViewChatTxtData, 0, sizeof(SChatTxtData*) * (MAX_CHATDATA_NUM + 1) );

	memset(m_strTempBuf1 , NULL , 512);
	memset(m_strTempBuf2 , NULL , 512);
	
	
	memset( &m_StrInfo, 0, sizeof(SStringInfo));

	

}

UINT   CChatMainProc::GetOutState()
{
	return m_OutState;
}

void CChatMainProc::LogOff()
{
	SetOutState(COutState_None);
	
	Init();	
}

void CChatMainProc::ReRPosition(int Rx, int Ry)
{
	m_ChatActPrintWnd.m_nLeftDownPos.x += Rx;
	m_ChatActPrintWnd.m_nLeftDownPos.y += Ry;	
	UINT nSizeState = m_ChatActPrintWnd.m_uSizeState;
	m_ChatActPrintWnd.m_uSizeState = ChatWndSizeNone;
	m_ChatActPrintWnd.SetSizeState(nSizeState);
	m_ChatActPrintWnd.RePosition();

	m_ChatNonActPrintWnd.m_nLeftDownPos.x += Rx;
	m_ChatNonActPrintWnd.m_nLeftDownPos.y += Ry;
	m_ChatNonActPrintWnd.ResetSizeFrame();
}

void   CChatMainProc::SetOutState(UINT uState)
{
	if(m_OutState == uState)
	{
		return;
	}
	
	m_OutState = uState;

	switch(m_OutState)
	{
		case COutState_None:
			{
				m_ChatActPrintWnd.SetVisible(false);
				m_ChatNonActPrintWnd.SetVisible(false);	
			}
			break;	
		case COutState_Act:
			{
				m_ChatActPrintWnd.SetVisible(true);
				m_ChatNonActPrintWnd.SetVisible(false);			
			}
			break;
		case COutState_NonAct:
			{
				m_ChatActPrintWnd.SetVisible(false);
				m_ChatNonActPrintWnd.SetVisible(true);
			}
			break;
	}	
}

void   CChatMainProc::Composition()
{
	m_ChatNonActPrintWnd.Composition();
	m_ChatActPrintWnd.Composition();	

	SetOutState(COutState_None);
}

void   CChatMainProc::Update()
{
	if(m_ChatNonActPrintWnd.IsVisible)
	{	m_ChatNonActPrintWnd.Update();	}
	
	if(m_ChatActPrintWnd.IsVisible)
	{	m_ChatActPrintWnd.Update();  }
	
	UpdateChatTxtData();
}

void   CChatMainProc::UpdateChatTxtData()
{
	for(int i = 0 ; i < m_nInsertIndex ; i++)
	{
		switch(m_ChatTxtData[i].ViewState)
		{
			case NTxtView:
				{
					m_ChatTxtData[i].CumTime += g_GTimer.GetFrameElapsedSecTime();
					if(m_ChatTxtData[i].CumTime >= SChatTxtData::ViewTime)
					{
						m_ChatTxtData[i].CumTime = 0.0f;
						m_ChatTxtData[i].ViewState = NTxtFadeout;
					}
				}
				break;
			case NTxtFadeout:
				{
					m_ChatTxtData[i].CumTime += g_GTimer.GetFrameElapsedSecTime();
					float fRatio = m_ChatTxtData[i].CumTime/SChatTxtData::FadingTime;
					if(m_ChatTxtData[i].CumTime >= SChatTxtData::FadingTime)
					{
						fRatio = 1.0f;
						m_ChatTxtData[i].CumTime = 0.0f;
						m_ChatTxtData[i].ViewState = NTxtNone;
					}					
					UINT uAlpha = (int)(255.0f - fRatio * 255.0f);
					m_ChatTxtData[i].Rcolor[A] = uAlpha;		
				}
				break;
		}

	}
}


void  CChatMainProc::InsertChatContent(UINT uChatState, void * pStrName, 
									   void *pStrContent, bool bFlag)
{
	RTCHAR *StrName = RWCHAR(pStrName);
	RTCHAR *StrContent = RWCHAR(pStrContent);
	
	m_TempChatData.ChatState = uChatState;
	m_TempChatData.ViewState = NTxtView;
	
	memset(m_strTempName, NULL, sizeof(TCHAR) * 512 );	
	SAFE_STR_CPY(m_strTempName, StrName, 512 );

	switch(uChatState)
	{
		case n_NormarChat:
			{
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 255;
				m_TempChatData.Rcolor[G] = 255;
				m_TempChatData.Rcolor[B] = 255;
			}
			break;
		case n_PartyChat:
			{
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 122;
				m_TempChatData.Rcolor[G] = 255;
				m_TempChatData.Rcolor[B] = 255;				
			}
			break;
		case n_GuildChat:
			{
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 122;
				m_TempChatData.Rcolor[G] = 255;
				m_TempChatData.Rcolor[B] = 122;
			}
			break;
		case n_WholeServerChat:
			{
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 255;
				m_TempChatData.Rcolor[G] = 168;
				m_TempChatData.Rcolor[B] = 243;
				
				Rsprintf(RWCHAR(m_strTempBuf1), _RT("%s [%s]"),G_STRING_CHAR( IDS_WND_EMOTICON_SERVER_CHAT ), RWCHAR(m_strTempName));			
						
		
			}
			break;
		case n_WholeChannelChat:
			{
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 156;
				m_TempChatData.Rcolor[G] = 214;
				m_TempChatData.Rcolor[B] = 255;
				Rsprintf(RWCHAR(m_strTempBuf1), _RT("%s [%s]"),G_STRING_CHAR( IDS_WND_EMOTICON_CHANNEL_CHAT  ),RWCHAR(m_strTempName));		
				
			}
			break;
		case n_WholeAreaChat:
			{

				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 252;
				m_TempChatData.Rcolor[G] = 126;
				m_TempChatData.Rcolor[B] = 100;

				Rsprintf(RWCHAR(m_strTempBuf1), _RT("%s [%s]"),G_STRING_CHAR( IDS_WND_EMOTICON_AREA_CHAT  ),RWCHAR(m_strTempName));		
			}
			break;
		case n_WhisperChat:
			{
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 255;
				m_TempChatData.Rcolor[G] = 175;
				m_TempChatData.Rcolor[B] = 255;				
				
				if( bFlag )
				{
					Rsprintf(RWCHAR(m_strTempBuf1), _RT("To[%s]"), RWCHAR(m_strTempName));			
				}
				else
				{
					Rsprintf(RWCHAR(m_strTempBuf1), _RT("From[%s]"), RWCHAR(m_strTempName));			
				}			
			}
			break;
		case n_AnnounChat:
			{
				m_ChatActPrintWnd.SetInsertAnnounce(true);
				
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 255;
				m_TempChatData.Rcolor[G] = 255;
				m_TempChatData.Rcolor[B] = 122;
				Rsprintf(RWCHAR(m_strTempName), G_STRING_CHAR( IDS_NOTICE_T ) );							
			}
			break;
		case n_HelpChat:
			{
				m_TempChatData.Rcolor[A] = 255;
				m_TempChatData.Rcolor[R] = 255;
				m_TempChatData.Rcolor[G] = 211;
				m_TempChatData.Rcolor[B] = 115;
				Rsprintf(RWCHAR(m_strTempName), G_STRING_CHAR( IDS_WND_HELP ) );							
			}
			break;
	}

	if(uChatState != n_WhisperChat &&
		uChatState != n_WholeServerChat &&
		uChatState != n_WholeChannelChat &&
		uChatState != n_WholeAreaChat)
	{
		Rsprintf(RWCHAR(m_strTempBuf1), _RT("[%s]"), RWCHAR(m_strTempName));						
	}

	m_ChatNonActPrintWnd.GetChatOutList()->GetStringInfo( m_strTempBuf1, &m_StrInfo );	
	
#if defined (ONLY_ENGLISH_SET) 	
	Rsprintf(RWCHAR(m_strTempBuf2), _RT("%s:%s"), RWCHAR(m_strTempBuf1), RWCHAR(StrContent));			
#else
	Rsprintf(RWCHAR(m_strTempBuf2), _RT("%s : %s"), RWCHAR(m_strTempBuf1), RWCHAR(StrContent));		
#endif

	memset(m_strTempBuf1, NULL, sizeof(TCHAR) * 512 );

	RTCHAR * pTempBuf = RWCHAR(m_strTempBuf2);
	RTCHAR * pTempOut = RWCHAR(m_strTempBuf1);

	int nCount = 0;
	
	int nLineSize = 60;

	while(pTempBuf)
	{
		if(nCount == 0)
		{

//#ifdef INDONESIA
//			nLineSize = 60;			
//#else
			nLineSize = 60;

		#ifdef APPLY_BOLD_FONT
			nLineSize = 52;
		#endif

		#ifdef APPLY_ENGLISH_ORIGIN_SIZE	
			nLineSize = 48;
		#endif

//#endif


#ifdef UNICODE_ROCK
		pTempBuf = ::GetStringReturnInStringW('\n', false, nLineSize, pTempBuf, pTempOut);		
#else
		pTempBuf = ::GetStringReturnInString('\n', false, nLineSize, pTempBuf, pTempOut);
#endif	
			
		m_TempChatData.TSize.tWidth = m_StrInfo.Width;
		m_TempChatData.TSize.tHeight = m_StrInfo.Height;
		SAFE_STR_CPY(m_TempChatData.Dest, m_strTempName, 256);

		}
		else
		{

//#ifdef INDONESIA
//			nLineSize = 54;			
//#else
			nLineSize = 54;

		#ifdef APPLY_BOLD_FONT
			nLineSize = 52;
		#endif

		#ifdef APPLY_ENGLISH_ORIGIN_SIZE	
			nLineSize = 48;
		#endif

//#endif

#ifdef UNICODE_ROCK
			pTempBuf = ::GetStringReturnInStringW('\n', false, nLineSize, pTempBuf, pTempOut);
#else
			pTempBuf = ::GetStringReturnInString('\n', false, nLineSize, pTempBuf, pTempOut);
#endif
				
			m_TempChatData.TSize.tWidth = -1;
			m_TempChatData.TSize.tHeight = -1;
			memset(m_TempChatData.Dest , NULL ,  256  );
		}	
		
		SAFE_STR_CPY(m_TempChatData.Text, pTempOut, 256);
		ZeroMemory(pTempOut , MAX_SIZE_512);

//		SAFE_STR_CPY(pTempOut, _RT(""), 512 );
	
	
		InsertChatData();			

		nCount++;
	}
	
	//m_ChatActPrintWnd.ResetScroll();
	//m_ChatNonActPrintWnd.ResetScroll();
}

void  CChatMainProc::InsertChatData()
{
	if( m_nInsertIndex >= MAX_CHATDATA_NUM )
	{
		for(int i = 0 ; i < MAX_CHATDATA_NUM ; i++)
		{
			m_ChatTxtData[i] = m_ChatTxtData[i + 1];			
		}
		m_ChatTxtData[m_nInsertIndex-1] = m_TempChatData;
	}
	else
	{
		m_ChatTxtData[m_nInsertIndex] = m_TempChatData;
		m_nInsertIndex++;
	}
	
	UpdateViewChatTxtData();		
	//필터링을 통해 실제보여지는 채팅창 출력리스트를
	//선별해낸다.
}

void CChatMainProc::UpdateViewChatTxtData()
{
	int nCount = 0;
	SChatTxtData *pTempChatTxtData = NULL;
	
	//배열 초기화  
	memset( m_pViewChatTxtData, 0, sizeof(SChatTxtData*) * (MAX_CHATDATA_NUM + 1) );
	
	for(int i = 0; i < m_nInsertIndex ; i++)
	{
		pTempChatTxtData = &m_ChatTxtData[i];
		switch(m_ChatActPrintWnd.GetFilterState())
		{
			case COutTotalView:
				{
					if(!g_IsHelpView)
					{
						if( pTempChatTxtData->ChatState == n_HelpChat )
						{
							continue;
						}
					}
				}
				break;
			case COutPartyView:
				{
					if( !( (pTempChatTxtData->ChatState == n_PartyChat) || 
						  (pTempChatTxtData->ChatState == n_WhisperChat) ) )
					{
						continue;
					}
				}
				break;
			case COutGuildView:
				{
					if( !( (pTempChatTxtData->ChatState == n_GuildChat) || 
						  (pTempChatTxtData->ChatState == n_WhisperChat) ) )
					{
						continue;
					}
				}
				break;
			case COutAnnounView:
				{
					if( !(pTempChatTxtData->ChatState == n_AnnounChat)  )
					{
						continue;
					}
				}
				break;
			case COutHelpView:
				{
					if( !(pTempChatTxtData->ChatState == n_HelpChat) )
					{
						continue;
					}
				}
				break;		
		}
		m_pViewChatTxtData[nCount] = pTempChatTxtData;
		nCount++;
	}	

}

