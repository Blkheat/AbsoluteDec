#include "Rui.h"
#include "EditBox.h"
#include "LoginWndProc.h"
#include "MessageWndProc.h"
#include "stringmanager.h"

CLoginWnd	LoginWnd;


TCHAR UserName[256] = {0,};
TCHAR Password[256] = {0,};
#ifdef DECO_RENEWAL_MJH

TCHAR m_userName[256] = {0,} ;
TCHAR m_passWord[256] = {0,};

#endif // DECO_RENEWAL_MJH
int	BufLen( 256 );


//-----------------------------------------------------------------------------
void CLoginWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CLoginWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CFrameWnd*		fw;
	CEditBox*		eb;
	CButton*		btn;
	CTextBox*		tbox;
	CImageBox*		img;
	CImageBox9*		img9;

	SRect sTempRect;
	
	sTempRect.x	= 394;
	sTempRect.y	= 315;
	sTempRect.w	= 235;
	sTempRect.h	= 138;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 335;

#endif	

	sTempRect.x	= center_x - ( sTempRect.w / 2 );
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_LoginWnd, WID_None, 
									  sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );	
	
	fw->SetCaption( G_STRING_CHAR( IDS_WND_LOGIN ) ); 
	fw->SetWndProc( this );	
	fw->SetTitle( true );
	fw->SetMove( false );	
	fw->Initialize();
	
	sTempRect.x	= 1;
	sTempRect.y	= 45;
	sTempRect.w	= 233;
	sTempRect.h	= 56;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 333;

#endif	
	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_LoginWnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	sTempRect.x	= 1;
	sTempRect.y	= 44;
	sTempRect.w	= 233;
	sTempRect.h	= 1;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 333;

#endif	

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_LoginWnd,  
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 1;
	sTempRect.y	= 101;
	sTempRect.w	= 233;
	sTempRect.h	= 1;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 333;

#endif	

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_LoginWnd, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 14;
	sTempRect.y	= 50;
	sTempRect.w	= 212;
	sTempRect.h	= 23;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 312;

#endif	
	
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_LoginWnd, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
	

	sTempRect.x	= 14;
	sTempRect.y	= 74;
	sTempRect.w	= 212;
	sTempRect.h	= 23;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 312;

#endif
	
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_LoginWnd, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	///-- 아이디 ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_LoginWnd, 28, 55, 23, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetText( G_STRING_CHAR( IDS_ID ) );
	
	///-- 패스워드 ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_LoginWnd, 28, 77, 23, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetText( G_STRING_CHAR( IDS_PASS ) );

	///--  ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_LoginWnd, 73, 55, 23, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetText( _RT(":") );
	
	///-- ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_LoginWnd, 73, 77, 23, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetText( _RT(":"));
	
	sTempRect.x	= 100;
	sTempRect.y	= 55;
	sTempRect.w	= 120;
	sTempRect.h	= 17;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 220;

#endif	

	eb = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_Login_UserNameEdit, WID_LoginWnd, 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	eb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	
#ifdef INDONESIA 
	eb->SetTextSize( RP_MAX_ACCOUNT_ID - 1 );
#else
#ifdef GLOBAL_LOGIN_VERSION
	eb->SetTextSize( RP_MAX_ACCOUNT_ID - 1 );	
#else
	eb->SetTextSize( RP_MAX_ACCOUNT_ID - 1 );	
#endif
#endif
	
	eb->SetClientImage( TID_None );
	eb->SetClientColor( 0, 0, 0 );
	eb->SetAlignText( n_atLeft, n_atCenter );
	eb->SetBlank( 2, 0, 0, 0 );
	//피그윙만 한글과 특수문자를 입력 받을수있다.  
	
	if( g_RockClient.m_GameTypeInfo.theFrom == n_Kr_Pigwing || 
		g_RockClient.m_GameTypeInfo.theFrom == n_Kr_MyClub )
	{
		eb->SetEditStyle( esText );
	}
	else 
	{
		eb->SetEditStyle( esOnlyAlphaNumeric );
	}

	eb->SetText( _RT("") );
	eb->SetReturnActionClear( false );
	eb->InitEditBox();	
	eb->SetHID( HID_ID );
	
	sTempRect.x	= 100;
	sTempRect.y	= 77;
	sTempRect.w	= 120;
	sTempRect.h	= 17;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.w	= 220;

#endif	

	eb = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_Login_PasswordEdit, WID_LoginWnd, 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	eb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	eb->SetTextSize( RP_MAX_PASSWD - 1 );
	eb->SetClientImage( TID_None );
	eb->SetClientColor( 0, 0, 0 );
	eb->SetAlignText( n_atLeft, n_atCenter );
	eb->SetBlank( 2, 0, 0, 0 );
	eb->SetText( _RT("") );
	eb->SetEditStyle( esPassword );
	eb->SetReturnActionClear( false );
	eb->InitEditBox();
	eb->SetHID(HID_PASS );
	
	sTempRect.x	= 56;
	sTempRect.y	= 108;
	sTempRect.w	= 58;
	sTempRect.h	= 25;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.x	= 106;

#endif	
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Login_OkBtn, WID_LoginWnd,  
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );      
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	btn->SetFontR( n_fRsvFontWndButton );
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );	
	btn->SetHID(HID_LOGIN );

	sTempRect.x	= 122;
	sTempRect.y	= 108;
	sTempRect.w	= 58;
	sTempRect.h	= 25;	

#ifdef GLOBAL_LOGIN_VERSION
	
	sTempRect.x	= 172;

#endif	
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Login_CancelBtn, WID_LoginWnd, 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	btn->SetFontR( n_fRsvFontWndButton );
	btn->SetText( G_STRING_CHAR( IDS_WND_GOOUT ) );	
	btn->SetHID( HID_EXIT );

	return;
}

//-----------------------------------------------------------------------------
void CLoginWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->Notify )
	{
	case NM_EDT_RETURN_ACTION:
		if( (WNDID)EMsg->FocusWnd == WID_Login_UserNameEdit )
		{
			nRui->SendEvent( WID_Login_PasswordEdit, n_emSetFocus, 0, 0, 0, 0 );
			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( WID_Login_UserNameEdit );
		}

		if( (WNDID)EMsg->FocusWnd == WID_Login_PasswordEdit )
		{
			nRui->SendEvent( WID_Login_UserNameEdit, n_emGetText, (EPARAM)UserName, BufLen, 0, 0 );
			nRui->SendEvent( WID_Login_PasswordEdit, n_emGetText, (EPARAM)Password, BufLen, 0, 0 );

#ifdef UNICODE_ROCK
			TCHAR StrUesrTmp[256]= {0,};
			TCHAR StrPwTmp[256]= {0,};
			DXUtil_ConvertWideStringToAnsi(StrUesrTmp , RWCHAR(UserName) , -1 )	;
			DXUtil_ConvertWideStringToAnsi(StrPwTmp , RWCHAR(Password) , -1 )	;
			strcpy(UserName , StrUesrTmp);
			strcpy(Password , StrPwTmp);
#endif

			
			//g_RockClient.m_SlangFilter.ConvertToUpper( UserName );
			//.................................................................................................
			//  문자열 없을때 체크
			//.................................................................................................
	
			if( g_RockClient.GetLanguage() == eLang_Korea && 
				( (n_Kr_Pigwing == g_RockClient.m_GameTypeInfo.theFrom) || 
				(n_Kr_MyClub == g_RockClient.m_GameTypeInfo.theFrom) ))
			{
			
				if(IsCheckSymbol(RCHAR(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE )), UserName))
				{					
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1500 , NULL );
					return ;
				}
			}

			if( strlen( UserName ) <= 0  )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INPUT_ID ), 1000, NULL );
				return;
			}

			if( strlen(Password) <= 0 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INPUT_PASS ), 1000, NULL );
				return;
			}

			if( IsCheckInSpace( UserName ) )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_REINPUT_ID ), 1000, NULL );
				return;
			}
			
			if( IsCheckInSpace( Password ) )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_REINPUT_PASS ), 1000, NULL );
				return;
			}
			//.................................................................................................

			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( WID_Login_PasswordEdit );

			// Login Packet Send
			if( g_RockClient.LoginProc( UserName, Password, g_RockClient.m_GameTypeInfo.theFrom ) )
			{
				nRui->SendEvent( WID_LoginWnd, n_emCloseFrameWnd, 0, 0, 0, 0 );
				nRui->EnableBGround( false );
			}

		}
		break;
		
	case NM_BTN_CLICK:
		if( (WNDID)EMsg->FocusWnd == WID_Login_OkBtn )
		{
			nRui->SendEvent( WID_Login_UserNameEdit, n_emGetText, (EPARAM)UserName, BufLen, 0, 0 );
			nRui->SendEvent( WID_Login_PasswordEdit, n_emGetText, (EPARAM)Password, BufLen, 0, 0 );

			//g_RockClient.m_SlangFilter.ConvertToUpper( UserName );
			//.................................................................................................
			//  문자열 없을때 체크
			//.................................................................................................

#ifdef UNICODE_ROCK
			TCHAR StrUesrTmp[256]= _T("");
			TCHAR StrPwTmp[256]= _T("");
			DXUtil_ConvertWideStringToAnsi(StrUesrTmp , RWCHAR(UserName) , -1 )	;
			DXUtil_ConvertWideStringToAnsi(StrPwTmp , RWCHAR(Password) , -1 )	;
			strcpy(UserName , StrUesrTmp);
			strcpy(Password , StrPwTmp);
#endif

#ifdef ROCKCLIENT_DEV	
			TestDecoIDCheck(UserName);
#endif
					
			if( g_RockClient.GetLanguage() == eLang_Korea && 
				( (n_Kr_Pigwing == g_RockClient.m_GameTypeInfo.theFrom) ||
					(n_Kr_MyClub == g_RockClient.m_GameTypeInfo.theFrom))	)
			{
				if(IsCheckSymbol(RCHAR(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE )), UserName))
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1500 , NULL );
					return ;
				}
			}


			if( strlen( UserName ) <= 0  )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INPUT_ID ), 1000, NULL );
				return;
			}

			if( strlen( Password) <= 0 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INPUT_PASS ), 1000, NULL );
				return;
			}
			//.................................................................................................

			if( IsCheckInSpace( UserName ) )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_REINPUT_ID ), 1000, NULL );
				return;
			}
			
			if( IsCheckInSpace( Password ) )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_REINPUT_PASS ), 1000, NULL );
				return;
			}

			nRui->SendEvent( WID_Login_UserNameEdit, n_emEditDisable, 0, 0, 0, 0 );
			nRui->SendEvent( WID_Login_PasswordEdit, n_emEditDisable, 0, 0, 0, 0 );
			nRui->SetEnterFlag( n_nfEditEnter );
			
			// Login Packet Send
			if( g_RockClient.LoginProc( UserName, Password, g_RockClient.m_GameTypeInfo.theFrom ) )
			{
#ifdef DECO_RENEWAL_MJH
				strcpy(m_userName , UserName );
				strcpy(m_passWord , Password );
#endif // DECO_RENEWAL_MJH
				
				nRui->SendEvent( WID_LoginWnd, n_emCloseFrameWnd, 0, 0, 0, 0 );
				nRui->EnableBGround( false );
			}
		
		}
		
		if( (WNDID)EMsg->FocusWnd == WID_Login_CancelBtn )
		{
			nRui->SendEvent( WID_Login_PasswordEdit, n_emSetFocus, 0, 0, 0, 0 );
			nRui->SendEvent( WID_Login_PasswordEdit, n_emClearText, 0, 0, 0, 0 );
			nRui->SendEvent( WID_Login_UserNameEdit, n_emSetFocus, 0, 0, 0, 0 );
			nRui->SendEvent( WID_Login_UserNameEdit, n_emClearText, 0, 0, 0, 0 );
			SendMessage( g_RockClient.GetApp(), WM_CLOSE, NULL, 1 );
		}
		

		break;
	}
	 
}
//-----------------------------------------------------------------------------
bool CLoginWnd::TestDecoIDCheck(char *Str)
{	
	
#ifdef _DEBUG	

	int Num =  (int)atoi(&Str[1]);
	
	if(Str[0] == 't' && ( Num != 0) )
	{
		if( Num != 0 )
		{
			char chTemp[30] = "";
		
				
			if( Num / 10 )
			{
				sprintf(chTemp , "test00%d", Num );		
			}
			else 
			{
				sprintf(chTemp , "test000%d", Num );		
			}
			strcpy(Str , chTemp );
			strcpy(Password,"epzhtkfkd1");
		}
	}

	if(Str[0] == 'w' && ( Num != 0)  )
	{
		if( Num != 0 )
		{
			char chTemp[30] = "";
			sprintf(chTemp , "WORLDDECO0%d", Num );
			strcpy(Str , chTemp);
			strcpy(Password,"1111");
		}
	}

#endif

	return true;

}

bool CLoginWnd::IsCheckSymbol(char *SrcStr , char *DstStr)
{
	
	int SrcLen = strlen(SrcStr);
	int DstLen = strlen(DstStr);
	
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
//................................................................................................................
// 스페이스 있나 확인
//................................................................................................................
int CLoginWnd::IsCheckInSpace(char *Str)
{
	int Len = strlen( Str );

	for( int i = 0 ; i < Len ; ++i )
	{
		if( Str[ i ] == ' ' )
			return TRUE;
	}

	return FALSE;
}
//................................................................................................................
// 케릭터 아이디 패스워드 정보
//................................................................................................................
#ifdef DECO_RENEWAL_MJH

TCHAR* CLoginWnd::GetUserName()
{
	return UserName;
}
TCHAR* CLoginWnd::GetPassWord()
{
	return Password;
}
#endif // DECO_RENEWAL_MJH
