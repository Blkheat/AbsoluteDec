#include "..\\..\\RockClient.h"
#include "..\\..\\brother.h"
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
#include "MFriendTabWnd.h"
#include "stringmanager.h"


int ascendNameCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		int Rt =  Rstrcmp((*a1)->strFName, (*a2)->strFName);

		if( Rt < 0 )
			return -1;

		if( Rt > 0 )
			return 1;
		
		return 0;
	}
	
	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	int Rt =  Rstrcmp((*a1)->strFName, (*a2)->strFName);

	if( Rt < 0 )
		return -1;

	if( Rt > 0 )
		return 1;

	return 0;
}

int descendNameCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		int Rt =  Rstrcmp((*a1)->strFName, (*a2)->strFName);

		if( Rt > 0 )
			return -1;

		if( Rt < 0 )
			return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	int Rt =  Rstrcmp((*a1)->strFName, (*a2)->strFName);

	if( Rt > 0 )
		return -1;

	if( Rt < 0 )
		return 1;

	return 0;
}

int ascendAreaCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	static TCHAR AtempStr1[512] = {0,};
	static TCHAR AtempStr2[512] = {0,};

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		::GetAreaStrName(AtempStr1, (*a1)->uiMapID);
		::GetAreaStrName(AtempStr2, (*a2)->uiMapID);

		int Rt =  Rstrcmp(AtempStr1, AtempStr2);

		if( Rt < 0 )
			return -1;

		if( Rt > 0 )
			return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenArea )
		return 1;

	if( !(*a2)->bOpenArea )
		return -1;
	
	::GetAreaStrName(AtempStr1, (*a1)->uiMapID);
	::GetAreaStrName(AtempStr2, (*a2)->uiMapID);

	int Rt =  Rstrcmp(AtempStr1, AtempStr2);

	if( Rt < 0 )
		return -1;

	if( Rt > 0 )
		return 1;

	return 0;
}

int descendAreaCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	static TCHAR AtempStr1[512] = {0,};
	static TCHAR AtempStr2[512] = {0,};

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		::GetAreaStrName(AtempStr1, (*a1)->uiMapID);
		::GetAreaStrName(AtempStr2, (*a2)->uiMapID);

		int Rt =  Rstrcmp(AtempStr1, AtempStr2);

		if( Rt > 0 )
			return -1;

		if( Rt < 0 )
			return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenArea )
		return 1;

	if( !(*a2)->bOpenArea )
		return -1;

	::GetAreaStrName(AtempStr1, (*a1)->uiMapID);
	::GetAreaStrName(AtempStr2, (*a2)->uiMapID);

	int Rt =  Rstrcmp(AtempStr1, AtempStr2);

	if( Rt > 0 )
		return -1;

	if( Rt < 0 )
		return 1;

	return 0;
}

int ascendChanelCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		if( (*a1)->nChanelNum < (*a2)->nChanelNum )
			return -1;

		if( (*a1)->nChanelNum > (*a2)->nChanelNum )
			 return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenArea )
		return 1;

	if( !(*a2)->bOpenArea )
		return -1;

	if( (*a1)->nChanelNum < (*a2)->nChanelNum )
	   return -1;

    if( (*a1)->nChanelNum > (*a2)->nChanelNum )
	   return 1;
 
    return 0;
}

int descendChanelCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		if( (*a1)->nChanelNum > (*a2)->nChanelNum )
			return -1;

		if( (*a1)->nChanelNum < (*a2)->nChanelNum )
			 return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenArea )
		return 1;

	if( !(*a2)->bOpenArea )
		return -1;

	if( (*a1)->nChanelNum > (*a2)->nChanelNum )
	   return -1;

    if( (*a1)->nChanelNum < (*a2)->nChanelNum )
	   return 1;
 
    return 0;
}

int ascendLevelCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		if( (*a1)->iLevel < (*a2)->iLevel )
			return -1;

		if( (*a1)->iLevel > (*a2)->iLevel )
		    return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenLevel )
		return 1;

	if( !(*a2)->bOpenLevel )
		return -1;

	if( (*a1)->iLevel < (*a2)->iLevel )
	   return -1;

    if( (*a1)->iLevel > (*a2)->iLevel )
	   return 1;
 
    return 0;
}

int descendLevelCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		if( (*a1)->iLevel > (*a2)->iLevel )
			return -1;

		if( (*a1)->iLevel < (*a2)->iLevel )
		    return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenLevel )
		return 1;

	if( !(*a2)->bOpenLevel )
		return -1;

	if( (*a1)->iLevel > (*a2)->iLevel )
	   return -1;

    if( (*a1)->iLevel < (*a2)->iLevel )
	   return 1;
 
    return 0;
}

int ascendJobCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		if( (*a1)->uiJob < (*a2)->uiJob )
		   return -1;

		if( (*a1)->uiJob > (*a2)->uiJob )
		   return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenJob )
		return 1;

	if( !(*a1)->bOpenJob )
		return -1;

	if( (*a1)->uiJob < (*a2)->uiJob )
	   return -1;

    if( (*a1)->uiJob > (*a2)->uiJob )
	   return 1;
 
    return 0;
}

int descendJobCmp( const void *arg1, const void *arg2 )
{
	SFriendData	** a1 = (SFriendData	**)arg1;
	SFriendData	** a2 = (SFriendData	**)arg2;

	if( n_MsngrStateOffline == (*a1)->uFriendState && 
		n_MsngrStateOffline == (*a2)->uFriendState )
	{
		if( (*a1)->uiJob > (*a2)->uiJob )
		   return -1;

		if( (*a1)->uiJob < (*a2)->uiJob )
		   return 1;
		
		return 0;
	}

	if( (*a1)->uFriendState == n_MsngrStateOffline )
		return 1;

	if( (*a2)->uFriendState == n_MsngrStateOffline )
		return -1;

	if( !(*a1)->bOpenJob )
		return 1;

	if( !(*a1)->bOpenJob )
		return -1;

	if( (*a1)->uiJob > (*a2)->uiJob )
	   return -1;

    if( (*a1)->uiJob < (*a2)->uiJob )
	   return 1;
 
    return 0;
}

CMFriendTabWnd::CMFriendTabWnd()
{
	m_pMFieldTabWnd = NULL;
	
	m_pNameFieldBtn = NULL;
	m_pAreaFieldBtn = NULL;
	m_pChanelFieldBtn = NULL;
	m_pLevelFieldBtn = NULL;
	m_pJobFieldBtn = NULL;

	m_FocusOriginTxt = NULL;
	m_FocusToolTipTxt = NULL;
	
	memset( m_pFriendDataList, NULL, sizeof( SFriendData* ) * (MAX_SFRIEND_DATA_NUM + 1) );	
	
	m_nNumOfFriend = 0;
	m_nStartListNum = 0;

	m_nSelectedBtn = -1;

	m_uSortPolicy = SORT_ASCNAME;
	
	m_bAscNameSort = true;
	m_bAscAreaSort = false;
	m_bAscChanelSort = false;
	m_bAscLevelSort = false;
	m_bAscJobSort = false;
}

CMFriendTabWnd::~CMFriendTabWnd()
{
	Init();
}

void CMFriendTabWnd::Init()
{
	for(int i = 0 ; i < (MAX_SFRIEND_DATA_NUM + 1) ; i++)
	{
		SAFE_DELETE(m_pFriendDataList[i]);
	}

	m_nNumOfFriend = 0;
	m_nStartListNum = 0;
	m_nSelectedBtn = -1;
}

void CMFriendTabWnd::Composition()
{
	CWnd *wnd = NULL;
	
	m_pMFieldTabWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_MCommunityBgUI, 
		30, 33, 333, 209, false );
	m_pMFieldTabWnd->SetClientImage( TID_None );
	m_pMFieldTabWnd->SetCaption( _RT("MFriendTab Window") );
	m_pMFieldTabWnd->SetWndProc( this );
	m_pMFieldTabWnd->SetTitle( false );
	m_pMFieldTabWnd->SetVisible( false );
	m_pMFieldTabWnd->Initialize();

	m_BackImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pMFieldTabWnd->GetID(), 0, 0, 333, 29, false );   
	m_BackImg->SetClientImage( TID_MBACK_FRIEND ); //-- ^^ TID_MSubBtnBgUI);
	
	
	m_pNameFieldBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pMFieldTabWnd->GetID(),  0, 0, 105, 29, false );    
	m_pNameFieldBtn->SetClientImage(TID_None);
	m_pNameFieldBtn->SetBtnImage( TID_None, TID_MFNameFieldBtnFocus, TID_MFNameFieldBtnFocus, TID_None );
	m_pNameFieldBtn->SetText( G_STRING_CHAR( IDS_CHARSTATE_NAME ) );			
	m_pNameFieldBtn->SetTextColor(255, 255, 200);
	m_pNameFieldBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pNameFieldBtn->SetBlank( 0, 0, 0, 0 );
	//m_pNameFieldBtn->SetClickTxt(false);
	
	m_pAreaFieldBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pMFieldTabWnd->GetID(), 106, 0, 48, 29, false );    
	m_pAreaFieldBtn->SetClientImage(TID_None);
	m_pAreaFieldBtn->SetBtnImage( TID_None, TID_MFAreaFieldBtnFocus, TID_MFAreaFieldBtnFocus, TID_None );
	m_pAreaFieldBtn->SetTextColor(255, 255, 200);
	m_pAreaFieldBtn->SetText( G_STRING_CHAR( IDS_WND_AREA ) );			
	m_pAreaFieldBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pAreaFieldBtn->SetBlank( 0, 0, 0, 0 );
	//m_pAreaFieldBtn->SetClickTxt(false);

	m_pChanelFieldBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pMFieldTabWnd->GetID(), 155, 0, 62, 29, false );  
	m_pChanelFieldBtn->SetClientImage(TID_None);
	m_pChanelFieldBtn->SetBtnImage( TID_None, TID_MFChanelFieldBtnFocus, TID_MFChanelFieldBtnFocus, TID_None );
	m_pChanelFieldBtn->SetTextColor(255, 255, 200);
	m_pChanelFieldBtn->SetText( G_STRING_CHAR( IDS_WND_CHANAL ) );			
	m_pChanelFieldBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pChanelFieldBtn->SetBlank( 0, 0, 0, 0 );
	//m_pChanelFieldBtn->SetClickTxt(false);

	m_pLevelFieldBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pMFieldTabWnd->GetID(), 218, 0, 62, 29, false );  

	m_pLevelFieldBtn->SetClientImage(TID_None);
	m_pLevelFieldBtn->SetBtnImage( TID_None, TID_MFLevelFieldBtnFocus, TID_MFLevelFieldBtnFocus, TID_None );
	m_pLevelFieldBtn->SetTextColor(255, 255, 200);
	m_pLevelFieldBtn->SetText(G_STRING_CHAR( IDS_CHARSTATE_LEVEL ) );
	m_pLevelFieldBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pLevelFieldBtn->SetBlank( 0, 0, 0, 0 );
	//m_pLevelFieldBtn->SetClickTxt(false);

	m_pJobFieldBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pMFieldTabWnd->GetID(), 281, 0, 52, 29, false ); 
	m_pJobFieldBtn->SetClientImage(TID_None);
	m_pJobFieldBtn->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );
	m_pJobFieldBtn->SetTextColor(255, 255, 200);
	m_pJobFieldBtn->SetText(G_STRING_CHAR( IDS_CHARSTATE_CLASS ) ) ;
	m_pJobFieldBtn->SetAlignText( n_atCenter, n_atCenter);
	m_pJobFieldBtn->SetBlank( 0, 0, 0, 0 );
	//m_pJobFieldBtn->SetClickTxt(false);	
	
	int iAddPosY = 30;	 	
	
	for(int i = 0 ; i < 6 ; i++)
	{
 		FriendField[i].pFNameTxt = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
		1, (iAddPosY * (i + 1)) + 1, 105, 14, false );
		FriendField[i].pFNameTxt->SetClientImage(TID_None);
		FriendField[i].pFNameTxt->SetAlignText( n_atLeft, n_atCenter );
		FriendField[i].pFNameTxt->SetBlank( 5, 0, 0, 0 );		
		FriendField[i].pFNameTxt->SetText(_RT(""));
		FriendField[i].pFNameTxt->SetTextColor(255, 255, 200);

		FriendField[i].pFAreaTxt = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
		1 + 40, (iAddPosY * (i + 1)) + 16, 103, 14, false );
		FriendField[i].pFAreaTxt->SetClientImage(TID_None);
		FriendField[i].pFAreaTxt->SetAlignText( n_atLeft, n_atCenter );
		FriendField[i].pFAreaTxt->SetBlank( 5, 0, 0, 0 );	
		FriendField[i].pFAreaTxt->SetText(_RT(""));

		FriendField[i].pFChanelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
		155, (iAddPosY * (i + 1)) + 1, 62, 29, false );
		FriendField[i].pFChanelTxt->SetClientImage(TID_None);
		FriendField[i].pFChanelTxt->SetAlignText( n_atCenter, n_atCenter );
		FriendField[i].pFChanelTxt->SetBlank( 0, 0, 0, 0 );	
		FriendField[i].pFChanelTxt->SetText(_RT(""));

		FriendField[i].pFLevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
		218, (iAddPosY * (i + 1)) + 1, 62, 29, false );
		FriendField[i].pFLevelTxt->SetClientImage(TID_None);
		FriendField[i].pFLevelTxt->SetAlignText( n_atCenter, n_atCenter);
		FriendField[i].pFLevelTxt->SetBlank( 0, 0, 0, 0 );
		FriendField[i].pFLevelTxt->SetText(_RT(""));
		
		FriendField[i].pFJobTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
		281, (iAddPosY * (i + 1)) + 1, 52, 29, false );
		FriendField[i].pFJobTxt->SetClientImage(TID_None);
		FriendField[i].pFJobTxt->SetAlignText( n_atCenter, n_atCenter);
		FriendField[i].pFJobTxt->SetBlank( 0, 0, 0, 0 );
		FriendField[i].pFJobTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW1));			
		FriendField[i].pFJobTxt->SetVisible(false);

		FriendField[i].pFJobImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pMFieldTabWnd->GetID(), 
		290, (iAddPosY * (i + 1)) + 3, 22, 24, false );
		FriendField[i].pFJobImg->SetClientImage(TID_None);		

		FriendField[i].pFStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
		218, (iAddPosY * (i + 1)) + 1, 114, 29, false );
		FriendField[i].pFStateTxt->SetClientImage(TID_CLIENT);
		FriendField[i].pFStateTxt->SetClientAlpah(220);
		FriendField[i].pFStateTxt->SetClientColor(55, 55, 74);
		FriendField[i].pFStateTxt->SetAlignText( n_atCenter, n_atCenter);
		FriendField[i].pFStateTxt->SetTextColor(255, 255, 255);
		FriendField[i].pFStateTxt->SetVisible(false);
	
		if( i < 5 )
		{
			wnd = (CWnd*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pMFieldTabWnd->GetID(), 
			1, (iAddPosY * (i + 1)) + 30, 333, 1, false );
			wnd->SetClientImage(TID_CLIENT);
			wnd->SetClientColor(151, 159, 186);
		}
	}

	m_FocusOriginTxt = (CFrameTextBox*)nRui->CreateWnd( n_wtFrameTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
	0, 0, 0, 0, false );	
	m_FocusOriginTxt->SetAlignText( n_atLeft, n_atCenter);
	m_FocusOriginTxt->SetBlank( 5, 0, 0, 0 );
	m_FocusOriginTxt->SetText(_RT(""));			
	m_FocusOriginTxt->SetClientImage(TID_CLIENT);
	m_FocusOriginTxt->SetClientColor(55, 55, 74);
	m_FocusOriginTxt->SetFrameColor( D3DCOLOR_ARGB( 255, 151, 159, 186 ) );
	m_FocusOriginTxt->SetVisible(false);

	for(short i = 0 ; i < 6 ; i++ )
	{
		FriendSelBtn[i] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pMFieldTabWnd->GetID(), 
			1, (iAddPosY * (i + 1)) + 1 , 333, 29, false );
		FriendSelBtn[i]->SetClientImage(TID_None);
		FriendSelBtn[i]->SetClientAlpah(128);
		FriendSelBtn[i]->SetBtnImage(TID_None, TID_MFriendSelFocus, TID_MFriendSelFocus, TID_None);
		///FriendSelBtn[i]->SetBtnAlpha(128);
	}

	m_FocusToolTipTxt = (CFrameTextBox*)nRui->CreateWnd( n_wtFrameTextBox, WID_None, m_pMFieldTabWnd->GetID(), 
	0, 0, 0, 0, false );
	m_FocusToolTipTxt->SetClientImage(TID_None);
	m_FocusToolTipTxt->SetAlignText( n_atCenter, n_atCenter);
	m_FocusToolTipTxt->SetBlank( 0, 0, 0, 0 );
	m_FocusToolTipTxt->SetText(_RT(""));			
	m_FocusToolTipTxt->SetClientImage(TID_CLIENT);
	m_FocusToolTipTxt->SetClientColor(55, 55, 74);
	m_FocusToolTipTxt->SetFrameColor( D3DCOLOR_ARGB( 255, 192, 174, 90 ) );
	m_FocusToolTipTxt->SetVisible(false);

	SetFriendField();
}

void CMFriendTabWnd::Update()
{
	static CLimitLenTextBox *pTextBox = NULL;
	static SFriendData *pFriendData = NULL;
	static CImageBox* pImageBox = NULL;

	m_FocusOriginTxt->SetVisible(false);
	m_FocusToolTipTxt->SetVisible(false);

	if(pTextBox)
	{
		pTextBox->SetVisible(true);		
	}
	
	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	static TCHAR strTemp[512] = {0,};

	SRect temRect;
	SStringInfo *pStringInfo = NULL;
	
	for(int i = 0 ; i < 6 ; i++)
	{
		if( FriendField[i].pFNameTxt->FocusCheck(ptCursor.x, ptCursor.y, false) )
		{
			if(!FriendField[i].pFNameTxt->IsEqualString())
			{
				if(FriendField[i].pFriendData == NULL)
				{
					continue;
				}
				
				FriendField[i].pFNameTxt->GetClientSize(&temRect);
				pStringInfo = FriendField[i].pFNameTxt->GetStringInfo();
				temRect.w = pStringInfo->Width + 10;
				m_FocusOriginTxt->ResetSize(&temRect);			
				
				FriendField[i].pFNameTxt->GetText(strTemp, 512);
				m_FocusOriginTxt->SetText(strTemp);
				m_FocusOriginTxt->SetVisible(true);
				
				pTextBox = FriendField[i].pFNameTxt;				
				pTextBox->SetVisible(false);

				break;
			}
		}

		if( FriendField[i].pFAreaTxt->FocusCheck(ptCursor.x, ptCursor.y, false) )
		{
			if(!FriendField[i].pFAreaTxt->IsEqualString())
			{			
				if(FriendField[i].pFriendData == NULL)
				{
					continue;
				}
				
				FriendField[i].pFAreaTxt->GetClientSize(&temRect);
				pStringInfo = FriendField[i].pFAreaTxt->GetStringInfo();
				temRect.w = pStringInfo->Width + 10;
				m_FocusOriginTxt->ResetSize(&temRect);			
				
				FriendField[i].pFAreaTxt->GetText(strTemp, 512);
				m_FocusOriginTxt->SetText(strTemp);
				m_FocusOriginTxt->SetVisible(true);

				pTextBox = FriendField[i].pFAreaTxt;				
				pTextBox->SetVisible(false);

				break;
			}
		}
	}

	SStringInfo		StrInfo;
	
	for(short i = 0 ; i < 6 ; i++ )
	{
		if( (FriendField[i].pFJobImg->IsVisible()) && !(FriendField[i].pFStateTxt->IsVisible()) )
		{
			if( FriendField[i].pFJobImg->FocusCheck(ptCursor.x, ptCursor.y, false) )
			{
				if(FriendField[i].pFriendData == NULL)
				{
					continue;
				}		
				
				::GetClassText((int)(FriendField[i].pFriendData->uiJob), strTemp);
				nRui->GetHelpTip()->theFontg->GetStringInfo( strTemp, &StrInfo );		
				
				FriendField[i].pFJobImg->GetClientSize(&temRect);
				temRect.x += temRect.w + 5;
				temRect.y += 7;
				temRect.w = StrInfo.Width + 5;
				temRect.h = 14;
				m_FocusToolTipTxt->ResetSize(&temRect);				

				m_FocusToolTipTxt->SetText(strTemp);
				m_FocusToolTipTxt->SetVisible(true);

				pImageBox = FriendField[i].pFJobImg;
	
				break;
			}		
		}
	}
}

void CMFriendTabWnd::Proc( SEventMessage* EMsg )
{
	 for(int i = 0 ; i < 6 ; i++)
	 {
		if(FriendSelBtn[i]->GetID() == EMsg->FocusWnd)
		{			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				FieldBntSelected( i );				
			}	

			if( EMsg->Notify == NM_BTN_RCLICK )
			{
				SFriendData * pFriendData = FriendField[i].pFriendData;
				if( pFriendData && (pFriendData->uFriendState != n_MsngrStateOffline) )
				{
					FieldBntSelected( i );
					
					POINT		ptCursor;
					GetCursorPos( &ptCursor );
					::ScreenToClient( g_RockClient.GetApp(), &ptCursor );

					ptCursor.x -= 5;
					ptCursor.y -= 5;				
					g_MCommunityWnd.m_MPopupMenuWnd.GetPopUpWnd()->SetAbsolutePosition(ptCursor.x, ptCursor.y);
					g_MCommunityWnd.m_MPopupMenuWnd.SetParam(pFriendData->strFName);

					//여기에 팝업메뉴 비활성화 기능부여
					if(g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
					{
						g_MCommunityWnd.m_MPopupMenuWnd.SetActiveMenu(true, false, true, true);
					}
					else
					{
						g_MCommunityWnd.m_MPopupMenuWnd.SetActiveMenu(true, true, false, true);
					}				
					
					nRui->SendEvent( WID_MPopupWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
				}		
			}
		}
	 }

	if(m_pNameFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscNameSort = !m_bAscNameSort;
				
			if(m_bAscNameSort)
			{
				m_uSortPolicy = SORT_ASCNAME;						
			}
			else
			{
				m_uSortPolicy = SORT_DESCNAME;		
			}

			SortFriendData();
			SetFriendField();
		}	
	}

	if(m_pAreaFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscAreaSort = !m_bAscAreaSort;
			if(m_bAscAreaSort)
			{
				m_uSortPolicy = SORT_ASCAREA;	
			}
			else
			{
				m_uSortPolicy = SORT_DESCAREA;
			}
			
			SortFriendData();
			SetFriendField();	
		}
	}

	if(m_pChanelFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscChanelSort = !m_bAscChanelSort;
			if(m_bAscChanelSort)
			{
				m_uSortPolicy = SORT_ASCCHANEL;
			}
			else
			{
				m_uSortPolicy = SORT_DESCCHANEL;
			}
			
			SortFriendData();
			SetFriendField();		
		}
	}

	if(m_pLevelFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscLevelSort = !m_bAscLevelSort;
			if(m_bAscLevelSort)
			{
				m_uSortPolicy = SORT_ASCLEVEL;
			}
			else
			{
				m_uSortPolicy = SORT_DESCLEVEL;	
			}
			
			SortFriendData();
			SetFriendField();	
		}
	}

	if(m_pJobFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscJobSort = !m_bAscJobSort;
			if(m_bAscJobSort)
			{
				m_uSortPolicy = SORT_ASCJOB;
			}
			else
			{
				m_uSortPolicy = SORT_DESCJOB;
			}
			
			SortFriendData();
			SetFriendField();	
		}
	}
}
int CMFriendTabWnd::GetClassNation(int iClass)
{

	//방랑자 -1
	// RAIN 0 
	// MILLENA 1 
			
	switch(iClass)
	{
		case Wanderer:			return -1;		break;
		case Stranger:			return 1;		break;
		case Swordman:			return 1;		break;
		case Archer:			return 1;			break;
		case Magician:			return 0;		break;
		case Shaman:			return 0;			break;
		case Knight:			return 1;			break;
		case Mercenary:			return 1;		break;
		case Slayer:			return 1;			break;
		case Sniper:			return 1;			break;
		case Prominas:			return 0;		break;
		case Priest:			return 0;			break;
		case Holy_Avenger:		return 0;		break;
		case Psyche:			return 0;			break;	
	}
	
	return -1;
}



TEXID CMFriendTabWnd::GetClassIcon(int iClass)
{
	switch(iClass)
	{
		case Wanderer:			return TID_MWandererIcon;		break;
		case Stranger:			return TID_MStrangerIcon;		break;
		case Swordman:			return TID_MSwordmanIcon;		break;
		case Archer:			return TID_MArcherIcon;			break;
		case Magician:			return TID_MMagicianIcon;		break;
		case Shaman:			return TID_MShamanIcon;			break;
		case Knight:			return TID_MKnightIcon;			break;
		case Mercenary:			return TID_MMercenaryIcon;		break;
		case Slayer:			return TID_MSlayerIcon;			break;
		case Sniper:			return TID_MSniperIcon;			break;
		case Prominas:			return TID_MProminasIcon;		break;
		case Priest:			return TID_MPriestIcon;			break;
		case Holy_Avenger:		return TID_MHoly_AvengerIcon;	break;
		case Psyche:			return TID_MPsycheIcon;			break;	
	}
	
	return TID_None;
}

void CMFriendTabWnd::DeleteFriendData(TCHAR * strName, bool bReNewFlag)
{
	for(int i = 0; i < m_nNumOfFriend ; i++)
	{
		SFriendData *pFriendData = m_pFriendDataList[i];
		
		if( pFriendData && !Rstrcmp( strName, pFriendData->strFName) )
		{
			SAFE_DELETE(pFriendData);
			m_pFriendDataList[i] = NULL;
			
			for(int j = i ; j < m_nNumOfFriend ; j++)
			{
				m_pFriendDataList[j] = m_pFriendDataList[j + 1];
			}
			
			m_nNumOfFriend--;
			
			if(bReNewFlag)
			{
				SetFriendField();
			}

			break;
		}	
	}	
}

SFriendData * CMFriendTabWnd::FindFriendData(TCHAR * strName)
{
	for(int i = 0; i < m_nNumOfFriend ; i++)
	{
		SFriendData *pFriendData = m_pFriendDataList[i];
		
		if( pFriendData && !Rstrcmp( strName, pFriendData->strFName) )
		{
			return pFriendData;
		}	
	}	
  
	return NULL;
}

void  CMFriendTabWnd::InsertFriendData(SFriendData * pFriendData, bool bReNewFlag)
{
	if( m_nNumOfFriend < MAX_SFRIEND_DATA_NUM )
	{
		m_pFriendDataList[m_nNumOfFriend] = pFriendData;
		m_nNumOfFriend++;
		
		if(bReNewFlag)
		{
			SetFriendField();
		}		
	}
	else
	{
		SAFE_DELETE(pFriendData);		
	}
}

void  CMFriendTabWnd::SortFriendData()
{
	switch(m_uSortPolicy)
	{
		case SORT_ASCNAME:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), ascendNameCmp );
			}
			break;
		case SORT_DESCNAME:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), descendNameCmp );
			}
			break;
		case SORT_ASCAREA:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), ascendAreaCmp );
			}
			break;
		case SORT_DESCAREA:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), descendAreaCmp );
			}
			break;
		case SORT_ASCCHANEL:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), ascendChanelCmp );
			}
			break;
		case SORT_DESCCHANEL:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), descendChanelCmp );
			}
			break;
		case SORT_ASCLEVEL:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), ascendLevelCmp );
			}
			break;
		case SORT_DESCLEVEL:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), descendLevelCmp );
			}
			break;
		case SORT_ASCJOB:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), ascendJobCmp );
			}
			break;
		case SORT_DESCJOB:
			{
				qsort( (void *)m_pFriendDataList, (size_t)m_nNumOfFriend, 
					sizeof(SFriendData	*), descendJobCmp );
			}
			break;
	}
}

void  CMFriendTabWnd::SetFriendField()
{
	FieldBntSelected(-1);
	
	TCHAR tempStr[512] = {0,};

	for( int i = 0; i < 6; ++i )
	{
		FriendField[i].pFriendData = m_pFriendDataList[m_nStartListNum + i];

		FriendField[i].pFNameTxt->SetTextColor(255, 255, 200);
		FriendField[i].pFAreaTxt->SetTextColor(255, 255, 255);
		FriendField[i].pFChanelTxt->SetTextColor(255, 255, 255);
		FriendField[i].pFLevelTxt->SetTextColor(255, 255, 255);
		FriendField[i].pFJobTxt->SetTextColor(255, 255, 255);		
		
		FriendField[i].pFStateTxt->SetVisible(false);
		FriendField[i].pFJobTxt->SetVisible(false);
		FriendField[i].pFJobImg->SetVisible(true);

		if(FriendField[i].pFriendData)
		{
			Rstrcpy(RWCHAR(tempStr), RWCHAR(FriendField[i].pFriendData->strFName));			
			FriendField[i].pFNameTxt->SetText(tempStr);
			FriendField[i].pFNameTxt->SetLimitLength(RP_MAX_PC_NAME-1);
			
#ifdef NEW_PVP_DECO
	
			if( (BYTE)nRui->thePcParam.Citizen != FriendField[i].pFriendData->bCitizen )
			{
				FriendField[i].pFriendData->uFriendState = n_MsngrStateOffline;
			}
#endif

			if( FriendField[i].pFriendData->uFriendState == n_MsngrStateOffline )
			{
				FriendField[i].pFNameTxt->SetTextColor(255, 130, 130);
				FriendField[i].pFAreaTxt->SetTextColor(255, 130, 130);
				FriendField[i].pFChanelTxt->SetTextColor(255, 130, 130);
				FriendField[i].pFLevelTxt->SetTextColor(255, 130, 130);
				FriendField[i].pFJobTxt->SetTextColor(255, 130, 130);
				
				FriendField[i].pFAreaTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW3));	  
				FriendField[i].pFAreaTxt->SetLimitLength(14);
				FriendField[i].pFChanelTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW1));	  
				FriendField[i].pFLevelTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW1));

				FriendField[i].pFJobImg->SetVisible(false);
				FriendField[i].pFJobTxt->SetVisible(true);				
				
				continue;	
			}
			
			
			if( FriendField[i].pFriendData->bOpenArea )			//지역 공개여부
			{
				GetAreaStrName(tempStr, FriendField[i].pFriendData->uiMapID);			
				FriendField[i].pFAreaTxt->SetText(tempStr);
				FriendField[i].pFAreaTxt->SetLimitLength(10);

				Rsprintf(RWCHAR(tempStr), _RT("%d"), FriendField[i].pFriendData->nChanelNum + 1);
				FriendField[i].pFChanelTxt->SetText(tempStr);
			}
			else
			{
				FriendField[i].pFAreaTxt->SetText( G_STRING_CHAR(IDS_WND_DO_NOT_KNOW3));	  
				FriendField[i].pFAreaTxt->SetLimitLength(10);
				FriendField[i].pFChanelTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW1));	  
			}
			
			if( FriendField[i].pFriendData->bOpenLevel )	   //레벨 공개여부
			{
				Rsprintf(RWCHAR(tempStr), _RT("%d"), FriendField[i].pFriendData->iLevel );
				FriendField[i].pFLevelTxt->SetText(tempStr);
			}
			else
			{
				FriendField[i].pFLevelTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW1));
			}
			
			if( FriendField[i].pFriendData->bOpenJob )			//직업 공개여부
			{
				TEXID tid = GetClassIcon(FriendField[i].pFriendData->uiJob);
				FriendField[i].pFJobImg->SetClientImage(tid);
			}
			else
			{
				FriendField[i].pFJobImg->SetVisible(false);
				FriendField[i].pFJobTxt->SetVisible(true);
			}

			if( FriendField[i].pFriendData->uFriendState == n_MsngrStateRest )
			{
				FriendField[i].pFStateTxt->SetVisible(true);
				FriendField[i].pFStateTxt->SetText( G_STRING_CHAR( IDS_MSGER_STATE_REST ) );		
			}
			
			if( FriendField[i].pFriendData->uFriendState == n_MsngrStateEat )
			{
				FriendField[i].pFStateTxt->SetVisible(true);
				FriendField[i].pFStateTxt->SetText( G_STRING_CHAR( IDS_MSGER_STATE_EATTING ) );		
			}
		}
		else
		{
			FriendField[i].pFNameTxt->SetText(_RT(""));
			FriendField[i].pFNameTxt->SetLimitLength(14);
			FriendField[i].pFAreaTxt->SetText(_RT(""));
			FriendField[i].pFAreaTxt->SetLimitLength(14);
			FriendField[i].pFChanelTxt->SetText(_RT(""));
			FriendField[i].pFLevelTxt->SetText(_RT(""));
			FriendField[i].pFJobImg->SetClientImage(TID_None);
		}
	}
}

void  CMFriendTabWnd::SetListScroll( int _linenum )
{
	FieldBntSelected(-1);
	m_nStartListNum = _linenum * 1;

	SetFriendField();
}

void  CMFriendTabWnd::FieldBntSelected( int _num_btn )
{
	m_nSelectedBtn = _num_btn;

	for(int i = 0; i < 6 ; i++)
	{
		FriendSelBtn[i]->SetClientImage( TID_None );
	}

	if(m_nSelectedBtn != -1)
	{
		FriendSelBtn[m_nSelectedBtn]->SetClientImage( TID_MFriendSelFocus );
	}
}

SFriendData * CMFriendTabWnd::GetSelectData()
{
	if(m_nSelectedBtn != -1)
	{
		return FriendField[m_nSelectedBtn].pFriendData;
	}

	return NULL;
}