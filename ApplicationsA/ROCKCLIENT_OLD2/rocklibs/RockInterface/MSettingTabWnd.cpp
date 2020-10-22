#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"
#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "UiUtil.h"
#include "MCommunityWnd.h"
#include "MSettingTabWnd.h"
#include "stringmanager.h"

CMSettingTabWnd::CMSettingTabWnd()
{
	m_pMSettingTabWnd = NULL;

	m_pMSOpenSettingTxt = NULL;
	m_pMSLocalTxt = NULL;
	m_pMSLocalCheckBox = NULL;
	m_pMSLevelTxt = NULL;
	m_pMSLevelCheckBox = NULL;
	m_pMSJobTxt = NULL;
	m_pMSJobCheckBox = NULL;
	m_pMSStateSettingTxt = NULL;
	m_pMSOnlineTxt = NULL;
	m_pMSOnlineRadioBtn = NULL;
	m_pMSRestTxt = NULL;
	m_pMSRestRadioBtn = NULL;
	m_pMSMeatTxt = NULL;
	m_pMSMeatRadioBtn = NULL;
	m_pMSOfflineTxt = NULL;
	m_pMSOfflineRadioBtn = NULL;
	m_pGuildAWordTxt = NULL;
	m_pGuildAWordInputBtn = NULL;
	m_pGuildAWordSaveBtn = NULL;

	m_pMSOptionTxt = NULL;
	m_pMSWhisperBlockTxt = NULL;
	m_pMSWhisperBlockBox = NULL;

	m_pSettingCompleteBtn = NULL;
	
	m_pPartyCallTxt = NULL;
	m_pPartyCallBtn = NULL;
	m_pExChangeTxt = NULL;
	m_pExChangeBtn = NULL;


	m_IsSelLocal = TRUE;
	m_IsSelLevel = TRUE;
	m_IsSelJob = TRUE;
	m_IsWhisperBlock = FALSE;
	m_IsPartyCall = FALSE;
	m_IsExChange = FALSE;
	
	m_MessenState = n_MsngrStateOnline;
	
	memset(m_strTemp , NULL , MAX_SIZE_256 );
	memset(m_strServerName ,NULL ,  MAX_SIZE_128 );

	m_nChannelNum = 0;
	
	m_pServerNameTxt = NULL;
	m_pChannelNumTxt = NULL;
}

CMSettingTabWnd::~CMSettingTabWnd()
{


}

void CMSettingTabWnd::SetServerName( TCHAR *strName )
{
	Rstrcpy( RWCHAR(m_strServerName), RWCHAR(strName) );
	Rsprintf( RWCHAR(m_strTemp), _RT("%s : %s"), G_STRING_CHAR( IDS_SERVER ), RWCHAR(m_strServerName) );			
	m_pServerNameTxt->SetText(m_strTemp);
}

void CMSettingTabWnd::SetChannelNum( int nNum )
{
	m_nChannelNum = nNum;
	Rsprintf( RWCHAR(m_strTemp), _RT("%s : %s %d"), G_STRING_CHAR( IDS_WND_CHANAL ), G_STRING_CHAR( IDS_WND_CHANAL ), m_nChannelNum );			
	m_pChannelNumTxt->SetText(m_strTemp);
}




void CMSettingTabWnd::Init()
{
	int option;
	
	option = GetPrivateProfileInt( "Client", "PartyCall", 0, "./setup.ini" );
	m_IsPartyCall = option;
	
	option = GetPrivateProfileInt( "Client", "ExChange", 0, "./setup.ini" );
	m_IsExChange = option;

	option = GetPrivateProfileInt( "Client", "Whisperblock", 0, "./setup.ini" );
	m_IsWhisperBlock = option;


}

void CMSettingTabWnd::Composition()
{
	m_pMSettingTabWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MSettingTabWnd, WID_MCommunityBgUI, 
		30, 33, 333, 241, false );
	m_pMSettingTabWnd->SetClientImage( TID_None );
	m_pMSettingTabWnd->SetCaption( _RT("MSettingTab Window") );
	m_pMSettingTabWnd->SetWndProc( this );
	m_pMSettingTabWnd->SetTitle( false );
	m_pMSettingTabWnd->SetVisible( false );
	m_pMSettingTabWnd->Initialize();

	m_pMSOpenSettingTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSOpenSettingTxt, WID_MSettingTabWnd, 
		0, 0, 333, 17, false );
	m_pMSOpenSettingTxt->SetAlignText(n_atCenter, n_atCenter);
	m_pMSOpenSettingTxt->SetClientImage( TID_MBACK_BLOCK );//--^^TID_MSettingFieldTxtImg);
	m_pMSOpenSettingTxt->SetBlank( 0, 0, 0, 0 );
	m_pMSOpenSettingTxt->SetTextColor(255, 255, 200);
	m_pMSOpenSettingTxt->SetText(G_STRING_CHAR( IDS_WND_PUBLIC ));			

	m_pMSLocalTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSLocalTxt, WID_MSettingTabWnd, 
		6, 23, 33, 11, false );    
	m_pMSLocalTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSLocalTxt->SetClientImage(TID_None);
	m_pMSLocalTxt->SetText( G_STRING_CHAR( IDS_WND_AREA2 ) );					

	m_pMSLocalCheckBox = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_MSLocalCheckBox, WID_MSettingTabWnd, 
		46, 24, 10, 10, false );  
	m_pMSLocalCheckBox->SetClientImage( TID_CTRL_RAD_DIS );
	m_pMSLocalCheckBox->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pMSLocalCheckBox->SetDirectCheck(false);
	
	m_pMSLevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSLevelTxt, WID_MSettingTabWnd, 
		84 , 23, 33, 11, false );  
	m_pMSLevelTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSLevelTxt->SetClientImage(TID_None);
	m_pMSLevelTxt->SetText( G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ) );					

	m_pMSLevelCheckBox = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_MSLevelCheckBox, WID_MSettingTabWnd, 
		124, 24, 10, 10, false ); 
	m_pMSLevelCheckBox->SetClientImage( TID_CTRL_RAD_DIS );
	m_pMSLevelCheckBox->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pMSLevelCheckBox->SetDirectCheck(false);

	m_pMSJobTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSJobTxt, WID_MSettingTabWnd, 
		162 , 23, 33, 11, false ); 
	m_pMSJobTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSJobTxt->SetClientImage(TID_None);
	m_pMSJobTxt->SetText( G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ) );					

	m_pMSJobCheckBox = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_MSJobCheckBox, WID_MSettingTabWnd, 
		 202, 24, 10, 10, false );  
	m_pMSJobCheckBox->SetClientImage( TID_CTRL_RAD_DIS );
	m_pMSJobCheckBox->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pMSJobCheckBox->SetDirectCheck(false);

	m_pMSStateSettingTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSStateSettingTxt, WID_MSettingTabWnd, 
		1, 42, 333, 17, false );
	m_pMSStateSettingTxt->SetAlignText(n_atCenter, n_atCenter);
	m_pMSStateSettingTxt->SetClientImage( TID_MBACK_BLOCK );
	m_pMSStateSettingTxt->SetBlank( 0, 0, 0, 0 );
	m_pMSStateSettingTxt->SetTextColor(255, 255, 200);
	m_pMSStateSettingTxt->SetText( G_STRING_CHAR( IDS_MSGER_STATE_VIEW ) );		

	m_pMSOnlineTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSOnlineTxt, WID_MSettingTabWnd, 
		6, 71, 33, 11, false );      
	m_pMSOnlineTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSOnlineTxt->SetClientImage(TID_None);
	m_pMSOnlineTxt->SetText(G_STRING_CHAR( IDS_MSGER_STATE_ONLINE ));				

	m_pMSOnlineRadioBtn = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_MSOnlineRadioBtn, WID_MSettingTabWnd,
		46, 72, 10, 10, false );   
	m_pMSOnlineRadioBtn->SetClientImage( TID_CTRL_RAD_DIS );
	m_pMSOnlineRadioBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK1, TID_None);
	m_pMSOnlineRadioBtn->SetDirectCheck(false);

	m_pMSRestTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSRestTxt, WID_MSettingTabWnd, 
		 84 , 71, 34, 11, false );   
	m_pMSRestTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSRestTxt->SetClientImage(TID_None);
	m_pMSRestTxt->SetText(G_STRING_CHAR( IDS_MSGER_STATE_REST ));					

	m_pMSRestRadioBtn = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_MSRestRadioBtn, WID_MSettingTabWnd,
		124, 72, 10, 10, false );   
	m_pMSRestRadioBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pMSRestRadioBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK1, TID_None);
	m_pMSRestRadioBtn->SetDirectCheck(false);

	m_pMSMeatTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSMeatTxt, WID_MSettingTabWnd, 
		162, 71, 34, 11, false );  
	m_pMSMeatTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSMeatTxt->SetClientImage(TID_None);
	m_pMSMeatTxt->SetText(G_STRING_CHAR( IDS_MSGER_STATE_EATTING ));					


	m_pMSMeatRadioBtn = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_MSMeatRadioBtn, WID_MSettingTabWnd,
		202, 72, 10, 10, false );
	m_pMSMeatRadioBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pMSMeatRadioBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK1, TID_None);
	m_pMSMeatRadioBtn->SetDirectCheck(false);

	m_pMSOfflineTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MSOfflineTxt, WID_MSettingTabWnd, 
		234 -10, 71, 33, 11, false );    
	m_pMSOfflineTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSOfflineTxt->SetClientImage(TID_None);
	m_pMSOfflineTxt->SetText(G_STRING_CHAR( IDS_MSGER_STATE_OFFLINE ));				
	
	m_pMSOfflineRadioBtn = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_MSOfflineRadioBtn, WID_MSettingTabWnd,
		280, 72, 10, 10, false );   
	m_pMSOfflineRadioBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pMSOfflineRadioBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK1, TID_None);
	m_pMSOfflineRadioBtn->SetDirectCheck(false);
		

	//설   정
	m_pMSOptionTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MSettingTabWnd, 
		1, 90 , 333, 17, false );
	m_pMSOptionTxt->SetAlignText(n_atCenter, n_atCenter);
	m_pMSOptionTxt->SetClientImage( TID_MBACK_BLOCK );
	m_pMSOptionTxt->SetBlank( 0, 0, 0, 0 );
	m_pMSOptionTxt->SetTextColor(255, 255, 200);
	m_pMSOptionTxt->SetText( G_STRING_CHAR(IDS_WND_SETTING2));  

	//전체귓속말차단 
	m_pMSWhisperBlockTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MSettingTabWnd, 
		6, 120 , 200, 11, false );      
	m_pMSWhisperBlockTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pMSWhisperBlockTxt->SetClientImage(TID_None);
	m_pMSWhisperBlockTxt->SetText(G_STRING_CHAR(IDS_WND_ALL_INTERCEPTION_WHISPER));	

	//차단 버튼 ....
	m_pMSWhisperBlockBox = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_None, WID_MSettingTabWnd, 
		6+(m_pMSWhisperBlockTxt->GetStringInfo()->Width) , 121, 10, 10, false );  
	m_pMSWhisperBlockBox->SetClientImage( TID_CTRL_RAD_DIS );
	m_pMSWhisperBlockBox->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pMSWhisperBlockBox->SetDirectCheck(false);
	
#ifndef NEW_PVP_DECO
	m_pMSOptionTxt->SetVisible(false);
	m_pMSWhisperBlockTxt->SetVisible(false);
	m_pMSWhisperBlockBox->SetVisible(false);
#endif
	//파티(리그) 거부 
	m_pPartyCallTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MSettingTabWnd, 
		6, 140 , 100, 11, false );      
	m_pPartyCallTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pPartyCallTxt->SetClientImage(TID_None);
	m_pPartyCallTxt->SetText(G_STRING_CHAR(IDS_WND_PARTY_REFUSED));	

	//차단 버튼 ....
	m_pPartyCallBtn = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_None, WID_MSettingTabWnd, 
		10+(m_pPartyCallTxt->GetStringInfo()->Width) , 141, 10, 10, false );  
	m_pPartyCallBtn->SetClientImage( TID_CTRL_RAD_DIS );
	m_pPartyCallBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pPartyCallBtn->SetDirectCheck(false);
	
	// 거래 거부 
	m_pExChangeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MSettingTabWnd, 
		6, 160 , 100, 11, false );      
	m_pExChangeTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pExChangeTxt->SetClientImage(TID_None);
	m_pExChangeTxt->SetText(G_STRING_CHAR(IDS_WND_EXCHANGE_REFUSED));	

	//차단 버튼 ....
	m_pExChangeBtn = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_None, WID_MSettingTabWnd, 
		10+(m_pExChangeTxt->GetStringInfo()->Width) , 161, 10, 10, false );  
	m_pExChangeBtn->SetClientImage( TID_CTRL_RAD_DIS );
	m_pExChangeBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pExChangeBtn->SetDirectCheck(false);
		
	m_pSettingCompleteBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MSSetCompleteBtn, WID_MSettingTabWnd, 135, 214, 58, 25, false );
	m_pSettingCompleteBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pSettingCompleteBtn->SetFontR( n_fRsvFontWndButton );
	m_pSettingCompleteBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );			
	
	nRui->SetGroupWnd( 4, WID_MSOnlineRadioBtn, WID_MSOnlineRadioBtn, WID_MSRestRadioBtn, WID_MSMeatRadioBtn, WID_MSOfflineRadioBtn );

	nRui->SendEvent( WID_MSLocalCheckBox, (NEventMessage)(n_emSetChecked + (int)!m_IsSelLocal), 
					0, 0, 0, 0 );
	nRui->SendEvent( WID_MSLevelCheckBox, (NEventMessage)(n_emSetChecked + (int)!m_IsSelLevel), 
					0, 0, 0, 0 );
	nRui->SendEvent( WID_MSJobCheckBox, (NEventMessage)(n_emSetChecked + (int)!m_IsSelJob), 
					0, 0, 0, 0 );
	nRui->SendEvent( (WID_MSOnlineRadioBtn + (m_MessenState * 2)), n_emSetChecked, 0, 0, 0, 0 );	

	m_pServerNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MSettingTabWnd, 
		105, 105, 200, 15, false );
	m_pServerNameTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pServerNameTxt->SetBlank( 5, 0, 0, 0 );
	m_pServerNameTxt->SetClientImage(TID_None);
	m_pServerNameTxt->SetText(_RT(""));
	m_pServerNameTxt->SetFocusCheck( false );
	
	m_pChannelNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MSettingTabWnd, 
		105, 132, 200, 15, false );
	m_pChannelNumTxt->SetAlignText(n_atLeft, n_atCenter);
	m_pChannelNumTxt->SetBlank( 5, 0, 0, 0 );
	m_pChannelNumTxt->SetClientImage(TID_None);
	m_pChannelNumTxt->SetText(_RT(""));
	m_pChannelNumTxt->SetFocusCheck( false );

	Init();
}

void CMSettingTabWnd::Update()
{   
	
}

void CMSettingTabWnd::Proc( SEventMessage* EMsg )
{

	if(EMsg->FocusWnd == m_pMSWhisperBlockTxt->GetID() || EMsg->FocusWnd == m_pMSWhisperBlockBox->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			UINT CheckMark = m_pMSWhisperBlockBox->GetCheckMark();
			CheckMark = !CheckMark;		
			nRui->SendEvent( m_pMSWhisperBlockBox->GetID(), (NEventMessage)(n_emSetChecked + CheckMark),
				0, 0, 0, 0 );		
		}
	}

	if(EMsg->FocusWnd == m_pPartyCallTxt->GetID() || EMsg->FocusWnd == m_pPartyCallBtn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			UINT CheckMark = m_pPartyCallBtn->GetCheckMark();
			CheckMark = !CheckMark;		
			nRui->SendEvent( m_pPartyCallBtn->GetID(), (NEventMessage)(n_emSetChecked + CheckMark),
				0, 0, 0, 0 );		
		}
	}
	if(EMsg->FocusWnd == m_pExChangeTxt->GetID() || EMsg->FocusWnd == m_pExChangeBtn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			UINT CheckMark = m_pExChangeBtn->GetCheckMark();
			CheckMark = !CheckMark;		
			nRui->SendEvent( m_pExChangeBtn->GetID(), (NEventMessage)(n_emSetChecked + CheckMark),
				0, 0, 0, 0 );		
		}
	}

	switch( EMsg->FocusWnd )
	{		
		case WID_MSLocalTxt:
		case WID_MSLocalCheckBox:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				UINT CheckMark = m_pMSLocalCheckBox->GetCheckMark();
				CheckMark = !CheckMark;		
				nRui->SendEvent( WID_MSLocalCheckBox, (NEventMessage)(n_emSetChecked + CheckMark),
				0, 0, 0, 0 );		
			}
			break;
		case WID_MSLevelTxt:
		case WID_MSLevelCheckBox:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				UINT CheckMark = m_pMSLevelCheckBox->GetCheckMark();
				CheckMark = !CheckMark;		
				nRui->SendEvent( WID_MSLevelCheckBox, (NEventMessage)(n_emSetChecked + CheckMark),
				0, 0, 0, 0 );		
			}
			break;
		case WID_MSJobTxt:
		case WID_MSJobCheckBox:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				UINT CheckMark = m_pMSJobCheckBox->GetCheckMark();
				CheckMark = !CheckMark;		
				nRui->SendEvent( WID_MSJobCheckBox, (NEventMessage)(n_emSetChecked + CheckMark),
				0, 0, 0, 0 );			
			}
			break;
		case WID_MSOnlineTxt:
		case WID_MSOnlineRadioBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( WID_MSOnlineRadioBtn , n_emSetChecked, 0, 0, 0, 0 );
			}
			break;
		case WID_MSRestTxt:
		case WID_MSRestRadioBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( WID_MSRestRadioBtn , n_emSetChecked, 0, 0, 0, 0 );
			}
			break;
		case WID_MSMeatTxt:
		case WID_MSMeatRadioBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( WID_MSMeatRadioBtn , n_emSetChecked, 0, 0, 0, 0 );
			}
			break;
		case WID_MSOfflineTxt:
		case WID_MSOfflineRadioBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( WID_MSOfflineRadioBtn , n_emSetChecked, 0, 0, 0, 0 );
			}
			break;
		case WID_MSSetCompleteBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				//서버로 설정정보 보내기 - 일단 셋팅정보로
				BOOL IsSelLocal = !((BOOL)(m_pMSLocalCheckBox->GetCheckMark()));
				BOOL IsSelLevel = !((BOOL)(m_pMSLevelCheckBox->GetCheckMark()));
				BOOL IsSelJob = !((BOOL)(m_pMSJobCheckBox->GetCheckMark()));
				
				BOOL IsWhisperBlock = !((BOOL)(m_pMSWhisperBlockBox->GetCheckMark()));
				BOOL IsPartyCall = !((BOOL)(m_pPartyCallBtn->GetCheckMark()));
				BOOL IsExChange = !((BOOL)(m_pExChangeBtn->GetCheckMark()));


				if( m_IsPartyCall != IsPartyCall )
				{
					m_IsPartyCall = IsPartyCall;
					char buf[12] = "";
					wsprintf( buf, "%d", m_IsPartyCall );
					WritePrivateProfileString( "Client", "PartyCall", buf , "./setup.ini"  );

				}

				if( m_IsExChange != IsExChange )
				{
					m_IsExChange = IsExChange;

					char buf[12] = "";
					wsprintf( m_strTemp , ".\\%s", "setup.ini" );
					wsprintf( buf, "%d", m_IsPartyCall );
					WritePrivateProfileString( "Client", "ExChange", buf , "./setup.ini" );
    
				}
				UINT uMessenState = n_MsngrStateOnline;	
				if( !((BOOL)(m_pMSOnlineRadioBtn->GetCheckMark())) )
				{
					uMessenState = n_MsngrStateOnline;				
				}
				if( !((BOOL)(m_pMSRestRadioBtn->GetCheckMark())) )
				{
					uMessenState = n_MsngrStateRest;					
				}
				if( !((BOOL)(m_pMSMeatRadioBtn->GetCheckMark())) )
				{
					uMessenState = n_MsngrStateEat;				
				}
				if( !((BOOL)(m_pMSOfflineRadioBtn->GetCheckMark())) )
				{
					uMessenState = n_MsngrStateOffline;				
				}
				
				if( m_IsWhisperBlock  != IsWhisperBlock)
				{
					m_IsWhisperBlock = IsWhisperBlock;
					//g_RockClient.Send_CsDenyWhisper((BYTE)IsWhisperBlock);

					char buf[12] = {0,};
					wsprintf( buf, "%d", m_IsWhisperBlock );
					WritePrivateProfileString( "Client", "WhisperBlock", buf , "./setup.ini" );
				}

				if( (m_IsSelLocal != IsSelLocal) || (m_IsSelLevel != IsSelLevel) ||
					(m_IsSelJob != IsSelJob) || (m_MessenState != uMessenState) )
				{
					SRpCsMsngrChangeOpt msgCsMsngrChangeOpt;
					msgCsMsngrChangeOpt.theOptMapFlag = (BYTE)IsSelLocal;
					msgCsMsngrChangeOpt.theOptLevelFlag  = (BYTE)IsSelLevel;
					msgCsMsngrChangeOpt.theOptJobFlag  = (BYTE)IsSelJob;
					
					msgCsMsngrChangeOpt.theOptState = (BYTE)uMessenState;
					g_RockClient.Send_CsMsngrChangeOpt(&msgCsMsngrChangeOpt);
				}			
			}
	}

}

void CMSettingTabWnd::SetSelectLocal(BOOL IsSelLocal, bool bReNewFlag)
{
	m_IsSelLocal = IsSelLocal;
	if(bReNewFlag)
	{
		ResetSettingData();
	}
}

void CMSettingTabWnd::SetSelectLevel(BOOL IsSelLevel, bool bReNewFlag)
{
	m_IsSelLevel = IsSelLevel;
	if(bReNewFlag)
	{
		ResetSettingData();
	}
}

void CMSettingTabWnd::SetSelectJob(BOOL IsSelJob, bool bReNewFlag)
{
	m_IsSelJob = IsSelJob;
	
	if(bReNewFlag)
	{
		ResetSettingData();
	}
}

void CMSettingTabWnd::SetMessenState(UINT uMessenState, bool bReNewFlag)
{
	m_MessenState = uMessenState;
	if(bReNewFlag)
	{
		ResetSettingData();
	}
}

void CMSettingTabWnd::ResetSettingData()
{
	nRui->SendEvent( m_pPartyCallBtn->GetID(), (NEventMessage)(n_emSetChecked + (int)!m_IsPartyCall ),
				0, 0, 0, 0 );

	nRui->SendEvent( m_pExChangeBtn->GetID(), (NEventMessage)(n_emSetChecked + (int)!m_IsExChange ),
				0, 0, 0, 0 );
	nRui->SendEvent( m_pMSWhisperBlockBox->GetID(), (NEventMessage)(n_emSetChecked + (int)!m_IsWhisperBlock ),
				0, 0, 0, 0 );

	nRui->SendEvent( WID_MSLocalCheckBox, (NEventMessage)(n_emSetChecked + (int)!m_IsSelLocal), 
					0, 0, 0, 0 );
	nRui->SendEvent( WID_MSLevelCheckBox, (NEventMessage)(n_emSetChecked + (int)!m_IsSelLevel), 
					0, 0, 0, 0 );
	nRui->SendEvent( WID_MSJobCheckBox, (NEventMessage)(n_emSetChecked + (int)!m_IsSelJob), 
					0, 0, 0, 0 );
	nRui->SendEvent( (WID_MSOnlineRadioBtn + (m_MessenState * 2)), n_emSetChecked, 0, 0, 0, 0 );
}