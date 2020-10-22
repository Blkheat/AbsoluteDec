//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CharDelWndProc.cpp
//----------------------------------------------------------------------------------------------------------------------
#include "CharDelWndProc.h"
#include "Rui.h"
#include "StringManager.h"
#include "MessageWndProc.h"

#include "LoginWndProc.h"

#include "EditBox.h"


CharDelWndProc		g_CharDelWndProc;


CharDelWndProc::CharDelWndProc()
{

	memset( Name ,0, sizeof(TCHAR) * 256 );
#ifdef DECO_RENEWAL_MJH
	memset( UserName ,0, sizeof(TCHAR) * 256 );
	memset( PassWord ,0, sizeof(TCHAR) * 256 );
#endif //DECO_RENEWAL_MJH
	
	BufLen = 256;	

}

void CharDelWndProc::Init()
{


}

void CharDelWndProc::Update()
{
		

}

void CharDelWndProc::Composition()
{
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CTextBox*		tbox = NULL;
	CImageBox*		img;
	CImageBox9*		img9;
	
	char temp[256] = {0,};
#ifdef DECO_RENEWAL_MJH
	int Height = 112;
#else
	int Height = 12;
#endif // DECO_RENEWAL_MJH

	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 394, 315, 235, 138 + Height , false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	///fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetCaption( G_STRING_CHAR(IDS_WND_BT_DEL_CH)); 
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetMove( true );	
	m_pFrameWnd->Initialize();	
	m_pFrameWnd->SetZorder( n_zoTop );


	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,m_pFrameWnd->GetID(), 1, 45, 233, 56 + Height, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,m_pFrameWnd->GetID(),  1, 44, 233, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,m_pFrameWnd->GetID(), 1, 101 + Height , 233, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrameWnd->GetID(), 57, 50, 120, 23, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
#ifdef DECO_RENEWAL_MJH
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrameWnd->GetID(), 57, 70, 120, 23, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrameWnd->GetID(), 57, 90, 120, 23, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
#endif // DECO_RENEWAL_MJH

#ifdef DECO_RENEWAL_MJH
	m_EffectText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,m_pFrameWnd->GetID(), 10, 55 + Height , 200, 46, false );
#else 
	m_EffectText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,m_pFrameWnd->GetID(), 10, 65 + Height , 200, 46, false );
#endif // DECO_RENEWAL_MJH
	m_EffectText->SetFontR( n_fRsvFontWndStatic );
	m_EffectText->SetTextColor(255,0,0 );	
	m_EffectText->SetAlignText( n_atLeft, n_atCenter );	
	m_EffectText->SetText( G_STRING_CHAR(IDS_DELETE_CHARACTER_NAME)); 
	m_EffectText->SetMultiLine( true );
//-----------------------------------------------------------------------------------------------------------------
// 케릭터 네임
//-----------------------------------------------------------------------------------------------------------------
	m_NameEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrameWnd->GetID(), 57, 55 , 120, 17, false );
	m_NameEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NameEdit->SetTextSize( RP_MAX_PC_NAME - sizeof(RTCHAR) );	
	m_NameEdit->SetClientImage( TID_None );
	m_NameEdit->SetClientColor( 0, 0, 0 );
	m_NameEdit->SetAlignText( n_atLeft, n_atCenter );
	m_NameEdit->SetBlank( 2, 0, 0, 0 );
	m_NameEdit->SetEditStyle( esText );
	m_NameEdit->SetText(_RT("") );
	m_NameEdit->SetReturnActionClear( false );
	m_NameEdit->InitEditBox();	
	m_NameEdit->SetHID(HID_PET_NAME_CREATE_ED ); //dongs 추가
	


#ifdef DECO_RENEWAL_MJH
//-----------------------------------------------------------------------------------------------------------------
// 로그인 아이디
//-----------------------------------------------------------------------------------------------------------------
	m_UserNameEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrameWnd->GetID(), 57, 75 , 120, 17, false );
	m_UserNameEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_UserNameEdit->SetTextSize( RP_MAX_PC_NAME - sizeof(RTCHAR) );	
	m_UserNameEdit->SetClientImage( TID_None );
	m_UserNameEdit->SetClientColor( 0, 0, 0 );
	m_UserNameEdit->SetAlignText( n_atLeft, n_atCenter );
	m_UserNameEdit->SetBlank( 2, 0, 0, 0 );
	m_UserNameEdit->SetEditStyle( esText );
	m_UserNameEdit->SetText(_RT("") );
	m_UserNameEdit->SetReturnActionClear( false );
	m_UserNameEdit->InitEditBox();
	//m_UserNameEdit->SetTextColor( 0, 0, 0 );
//-----------------------------------------------------------------------------------------------------------------
// 패스워드
//-----------------------------------------------------------------------------------------------------------------
	m_passWordEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrameWnd->GetID(), 57, 95 , 120, 17, false );
	m_passWordEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_passWordEdit->SetTextSize( RP_MAX_PC_NAME - sizeof(RTCHAR) );	
	m_passWordEdit->SetClientImage( TID_None );
	m_passWordEdit->SetClientColor( 0, 0, 0 );
	m_passWordEdit->SetAlignText( n_atLeft, n_atCenter );
	m_passWordEdit->SetBlank( 2, 0, 0, 0 );
	m_passWordEdit->SetEditStyle( esPassword );
	m_passWordEdit->SetText(_RT("") );
	m_passWordEdit->SetReturnActionClear( false );
	m_passWordEdit->InitEditBox();
	//m_passWordEdit->SetTextColor( 0, 0, 0 );
#endif // DECO_RENEWAL_MJH
	
	m_OkBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(),  56, 108 + Height, 58, 25, false );      
	m_OkBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_OkBtn->SetFontR( n_fRsvFontWndButton );
	m_OkBtn->SetText( G_STRING_CHAR(IDS_WND_COM_OK));	
	m_OkBtn->SetHID( HID_PET_CREAT_BTN );
	
	m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 122, 108 + Height , 58, 25, false ); 
	m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_CancelBtn->SetFontR( n_fRsvFontWndButton );
	m_CancelBtn->SetText( G_STRING_CHAR(IDS_WND_COM_CANCEL));	
	m_CancelBtn->SetHID( HID_PET_CANCEL_BTN );
	

	return;
}

void CharDelWndProc::Proc( SEventMessage* EMsg )
{
//	RLGS("CharDelWndProc");
	
	switch( EMsg->Notify )
	{
	case NM_EDT_RETURN_ACTION:
		if( (WNDID)EMsg->FocusWnd == m_NameEdit->GetID() )
		{
		
		//	nRui->SendEvent(  m_NameEdit->GetID(), n_emGetText, (EPARAM)PetName, BufLen, 0, 0 );
			m_NameEdit->GetText(Name,BufLen);

#ifdef DECO_RENEWAL_MJH
			m_UserNameEdit->GetText( UserName , BufLen );
			m_passWordEdit->GetText( PassWord , BufLen );
#endif // DECO_RENEWAL_MJH
			nRui->SetEnterFlag( n_nfEditEnter );
			
			if(CreateNameAndVerification())
			{ 
			//	_tcscat(PetName,_RT(" "));
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_WND_ASK_DEL_CH ), MB_TYPE_CANCEL , &CharSelectWnd );
				
				Close();
			}
			else
			{								
					nRui->SendEvent( m_NameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
					nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
					nRui->SendEvent( m_NameEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );
#ifdef DECO_RENEWAL_MJH
					nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
					nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
					nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );

					nRui->SendEvent( m_passWordEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
					nRui->SendEvent( m_passWordEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
					nRui->SendEvent( m_passWordEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );
#endif // DECO_RENEWAL_MJH
			}

		
		}
		break;

	case NM_BTN_CLICK:
		if( (WNDID)EMsg->FocusWnd == m_OkBtn->GetID() )
		{
		//	nRui->SendEvent( m_NameEdit->GetID(), n_emGetText, (EPARAM)PetName, BufLen, 0, 0 );
			m_NameEdit->GetText(Name,BufLen);
#ifdef DECO_RENEWAL_MJH
			m_UserNameEdit->GetText( UserName , BufLen );
			m_passWordEdit->GetText( PassWord , BufLen );
#endif // DECO_RENEWAL_MJH
			
			if(CreateNameAndVerification())
			{

				MessageBoxWnd.Open( G_STRING_CHAR( IDS_WND_ASK_DEL_CH ), MB_TYPE_CANCEL , &CharSelectWnd );
				//_tcscat(PetName,_RT(" "));
				g_RockClient.Send_CsMakePetName(Name,false);	
		
				Close();
			}	
			else
			{
					nRui->SendEvent( m_NameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
					nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
					nRui->SendEvent( m_NameEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );
#ifdef DECO_RENEWAL_MJH
					nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
					nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
					nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );

					nRui->SendEvent( m_passWordEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
					nRui->SendEvent( m_passWordEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
					nRui->SendEvent( m_passWordEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );
#endif // DECO_RENEWAL_MJH
			}

		}
		
		if( (WNDID)EMsg->FocusWnd == m_CancelBtn->GetID() )
		{
			Rstrcat(RWCHAR(Name),_RT(" "));
#ifdef DECO_RENEWAL_MJH	
			//m_UserNameEdit->SetText(_RT("") );
			//m_passWordEdit->SetText(_RT("") );
			Rstrcat(RWCHAR(UserName),_RT(" "));
			Rstrcat(RWCHAR(PassWord),_RT(" "));
#endif // DECO_RENEWAL_MJH
			Close();
		}


		break;
	}


}

bool CharDelWndProc::CreateNameAndVerification()
{

	//공백검사 
	if( IsCheckInSpace( Name ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_REINPUT_ID ), 1000, NULL );
		return false;
	}


	if( Rstrlen( RWCHAR(Name) ) <= 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INPUT_NAME ) , 1500 , NULL );

		return false;
	}
 
	//특수문자 한자 검사 
//	if( !g_RockClient.m_SlangFilter.IsValidString( PetName ) )
//	{
//		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
//		
//		return false;
//	}
//	
//	// 사용할수 없는 단어 검사 
//	if( g_RockClient.m_SlangFilter.ISFindFilterName( PetName ) )
//	{
//		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
//		
//		return false;
//	}
	// ";%<>- 사용 금지 

	if( CharSelectWnd.m_pSelectChar != NULL  )
	{
		
		if( strcmp(CharSelectWnd.m_pSelectChar->theData.thePCName , Name ) != 0 )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_WRONG_NAME ) , 1500 , NULL );
			return false;
		}


	}
	else
	{
		return false;
	}



	if(IsCheckSymbol( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ), Name ))
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1500 , NULL );
		return false;
	}

	// 필터링 
	if( g_RockClient.m_SlangFilter.Filter( Name ) )
	{
		//욕설이 나오면 
		if( g_RockClient.m_SlangFilter.ISSlangFound() )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SLANG_TEXT ) , 1500 , NULL );
			return false;
		}
	}
#ifdef DECO_RENEWAL_MJH	
	#ifdef _DEBUG
	#else
	if( Rstrcmp( RWCHAR(LoginWnd.GetUserName()), RWCHAR(UserName) ) != 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSINPUT_NEWNAME ) , 1500 , NULL );
		return false;
	}
	if( Rstrcmp( RWCHAR(LoginWnd.GetPassWord()), RWCHAR(PassWord) ) != 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_PASS ) , 1500 , NULL );
		return false;
	}
	// 케릭터 삭제시 길드에 가입되있으면 삭제 불가능
	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	if(pChar[ CharSelectWnd.GetCharacterSelectNum() ].theData.theFrontierName[0] != NULL )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_MAKEFRONTIER_OTHERFT ) , 1500 , NULL );
		return false;
	}
	#endif _DEBUG
#endif // DECO_RENEWAL_MJH


	return true;

}

void CharDelWndProc::Close()
{
		
		nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
		nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef DECO_RENEWAL_MJH
		nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
		nRui->SendEvent( m_passWordEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
#endif // DECO_RENEWAL_MJH 
}

void CharDelWndProc::Open()
{
		
		nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_NameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
		nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
#ifdef DECO_RENEWAL_MJH
		nRui->SendEvent( m_UserNameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
		nRui->SendEvent( m_passWordEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
#endif // DECO_RENEWAL_MJH
				
}


int CharDelWndProc::IsCheckInSpace(void *pStr)
{
	RTCHAR *Str = RWCHAR(pStr);

	int Len = Rstrlen( Str );

	for( int i = 0 ; i < Len ; ++i )
	{
		if( Str[ i ] == ' ' )
			return TRUE;
	}

	return FALSE;
}

bool CharDelWndProc::IsCheckSymbol(void *pSrcStr , void *pDstStr)	
{
	RTCHAR *SrcStr = RWCHAR(pSrcStr);
	RTCHAR *DstStr = RWCHAR(pDstStr);
	
	int SrcLen = Rstrlen(SrcStr);
	int DstLen = Rstrlen(DstStr);
	
	int Dinx = 0 ; 
	int Sinx = 0 ; 

	for( Sinx  =0 ; Sinx< SrcLen ; ++Sinx )
	{
		for(Dinx = 0 ; Dinx < DstLen ; ++Dinx)
		{
			if( SrcStr[Sinx] == DstStr[Dinx])
			{
				return true;				
			}
			
		}
		
	}
	return false; 

}