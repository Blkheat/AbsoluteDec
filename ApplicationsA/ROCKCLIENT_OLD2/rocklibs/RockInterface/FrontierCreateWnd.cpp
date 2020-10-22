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

#include "FrontierCreateWnd.h"
#include "MessageWndProc.h"
#include "stringmanager.h"
#include "ListTime.h"
#include <time.h>

CFrontierCreateWnd g_FrontierCreateWnd;

#ifdef DECO_RENEWAL_GUILD_MJH
CFrontierCreateScrollWnd g_FrontierCreateScrollWnd;
#endif //DECO_RENEWAL_GUILD_MJH

TCHAR PName[256] = {0,};
TCHAR PCMemeber1[256] = {0,};
TCHAR PCMemeber2[256] = {0,};
TCHAR PCMemeber3[256] = {0,};
int	nBufLen( 256 );

CFrontierCreateWnd::CFrontierCreateWnd()
{
	m_pFrameWnd = NULL;
	m_pPNameEdit = NULL;

	m_pPCMemeberEdit[0] = NULL;
	m_pPCMemeberEdit[1] = NULL;
	m_pPCMemeberEdit[2] = NULL;

	m_pCloseBtn = NULL;
	m_pConfirmBtn = NULL;
	m_pCancleBtn = NULL;
	
	Init();
}

CFrontierCreateWnd::~CFrontierCreateWnd()
{

}

void CFrontierCreateWnd::Init()
{

}

void CFrontierCreateWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CTextBox*		tbox = NULL;
	CImageBox*		img = NULL;
	CImageBox9*		img9 = NULL;

	SRect FCWnd;
	FCWnd.w	= 288;
	FCWnd.h	= 260;
	FCWnd.x	= center_x - ( FCWnd.w / 2 );
	FCWnd.y	= 354;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	FCWnd.w	= 388;

#endif
	   
	TCHAR tempBuf[256] = {0,};
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, 
								WID_None, FCWnd.x, FCWnd.y, FCWnd.w, FCWnd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );	
	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_NEW_FRONTIER_CREATE ) );						
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetMove( true );
	m_pFrameWnd->SetZorder( n_zoTop );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	SRect sTempRect;

	sTempRect.x	= 269;
	sTempRect.y	= 6;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 369;

#endif

	///-- 닫기 버튼 
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, 
        WID_None, m_pFrameWnd->GetID(), sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pCloseBtn->SetBtnImage(TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, 
        TID_CTRL_BTN_X_CLK, TID_None );

	sTempRect.x	= 1;
	sTempRect.y	= 45;
	sTempRect.w	= 286;
	sTempRect.h	= 177;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 386;

#endif

	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
							m_pFrameWnd->GetID(), sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	sTempRect.x	= 1;
	sTempRect.y	= 44;
	sTempRect.w	= 286;
	sTempRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 386;

#endif

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
							m_pFrameWnd->GetID(), sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 1;
	sTempRect.y	= 222;
	sTempRect.w	= 286;
	sTempRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 386;

#endif

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
							m_pFrameWnd->GetID(), sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	///-- 프론티어 이름 ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
							m_pFrameWnd->GetID(), 12, 62, 80, 24, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetTextColor(0, 0, 0);	
	tbox->SetText( G_STRING_CHAR( IDS_FRONTIER_NAME  ) );												

	///-- 창단 멤버 1 ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
							m_pFrameWnd->GetID(), 12, 97, 80, 24, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetTextColor(0, 0, 0);
	Rsprintf(RWCHAR(tempBuf), _RT("%s 1"), G_STRING_CHAR( IDS_BEGIN_MEMBER  ) );						
	tbox->SetText( tempBuf );	
#ifdef DECO_RENEWAL_GUILD_MJH
	tbox->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH
	
	///-- 창단 멤버 2 ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
							m_pFrameWnd->GetID(), 12, 132, 80, 24, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetTextColor(0, 0, 0);
	Rsprintf(RWCHAR(tempBuf), _RT("%s 2"), G_STRING_CHAR( IDS_BEGIN_MEMBER  ) );						
	tbox->SetText( tempBuf );
#ifdef DECO_RENEWAL_GUILD_MJH
	tbox->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	///-- 창단 멤버 3 ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
							m_pFrameWnd->GetID(), 12, 167, 80, 24, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetTextColor(0, 0, 0);
	Rsprintf(RWCHAR(tempBuf), _RT("%s 3"), G_STRING_CHAR( IDS_BEGIN_MEMBER  ) );						
	tbox->SetText( tempBuf );
#ifdef DECO_RENEWAL_GUILD_MJH
	tbox->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	///-- : ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 
							84, 62, 23, 24, false );	
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetTextColor(0, 0, 0);
	tbox->SetText( _RT(":"));

	///-- : ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 
							84, 97, 23, 24, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetTextColor(0, 0, 0);
	tbox->SetText( _RT(":"));
#ifdef DECO_RENEWAL_GUILD_MJH
	tbox->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	///-- : ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 
							84, 132, 23, 24, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetTextColor(0, 0, 0);
	tbox->SetText( _RT(":"));
#ifdef DECO_RENEWAL_GUILD_MJH
	tbox->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	///-- : ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 
							84, 167, 23, 24, false );	
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetTextColor(0, 0, 0);
	tbox->SetText( _RT(":"));
#ifdef DECO_RENEWAL_GUILD_MJH
	tbox->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	int nXPos = 104;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nXPos += 40; 

#endif

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pFrameWnd->GetID(), nXPos, 62, 172, 24, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pFrameWnd->GetID(), nXPos, 97, 172, 24, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
#ifdef DECO_RENEWAL_GUILD_MJH
	img9->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pFrameWnd->GetID(), nXPos, 132, 172, 24, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
#ifdef DECO_RENEWAL_GUILD_MJH
	img9->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pFrameWnd->GetID(), nXPos, 167, 172, 24, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
#ifdef DECO_RENEWAL_GUILD_MJH
	img9->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH


	nXPos = 114;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nXPos += 40; 

#endif

	///-- 각 입력박스
		
	m_pPNameEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, 
									m_pFrameWnd->GetID(), nXPos, 62, 162, 24, false );
	m_pPNameEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPNameEdit->SetTextSize( 20 );
	m_pPNameEdit->SetClientImage( TID_None );
	m_pPNameEdit->SetClientColor( 0, 0, 0 );
	m_pPNameEdit->SetAlignText( n_atLeft, n_atCenter );
	m_pPNameEdit->SetBlank( 2, 0, 0, 0 );
	m_pPNameEdit->SetEditStyle( esText );
	m_pPNameEdit->SetTextColor( 255, 255, 255 );
	m_pPNameEdit->SetText( _RT("") );
	m_pPNameEdit->SetReturnActionClear( false );
	m_pPNameEdit->InitEditBox();

	m_pPCMemeberEdit[0] = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, 
									m_pFrameWnd->GetID(), nXPos, 97, 162, 24, false );
	m_pPCMemeberEdit[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPCMemeberEdit[0]->SetTextSize( 17 );
	m_pPCMemeberEdit[0]->SetClientImage( TID_None );
	m_pPCMemeberEdit[0]->SetClientColor( 0, 0, 0 );
	m_pPCMemeberEdit[0]->SetAlignText( n_atLeft, n_atCenter );
	m_pPCMemeberEdit[0]->SetBlank( 2, 0, 0, 0 );
	m_pPCMemeberEdit[0]->SetEditStyle( esText );
	m_pPCMemeberEdit[0]->SetTextColor( 255, 255, 255 );
	m_pPCMemeberEdit[0]->SetText( _RT("") );
	m_pPCMemeberEdit[0]->SetReturnActionClear( false );
	m_pPCMemeberEdit[0]->InitEditBox();
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pPCMemeberEdit[0]->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	m_pPCMemeberEdit[1] = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, 
									m_pFrameWnd->GetID(), nXPos, 132, 162, 24, false );
	m_pPCMemeberEdit[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPCMemeberEdit[1]->SetTextSize( 17 );
	m_pPCMemeberEdit[1]->SetClientImage( TID_None );
	m_pPCMemeberEdit[1]->SetClientColor( 0, 0, 0 );
	m_pPCMemeberEdit[1]->SetAlignText( n_atLeft, n_atCenter );
	m_pPCMemeberEdit[1]->SetBlank( 2, 0, 0, 0 );
	m_pPCMemeberEdit[1]->SetEditStyle( esText );
	m_pPCMemeberEdit[1]->SetTextColor( 255, 255, 255 );
	m_pPCMemeberEdit[1]->SetText( _RT("") );
	m_pPCMemeberEdit[1]->SetReturnActionClear( false );
	m_pPCMemeberEdit[1]->InitEditBox();
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pPCMemeberEdit[1]->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	m_pPCMemeberEdit[2] = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, 
									m_pFrameWnd->GetID(), nXPos, 167, 162, 24, false );
	m_pPCMemeberEdit[2]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPCMemeberEdit[2]->SetTextSize( 17 );
	m_pPCMemeberEdit[2]->SetClientImage( TID_None );
	m_pPCMemeberEdit[2]->SetClientColor( 0, 0, 0 );
	m_pPCMemeberEdit[2]->SetAlignText( n_atLeft, n_atCenter );
	m_pPCMemeberEdit[2]->SetBlank( 2, 0, 0, 0 );
	m_pPCMemeberEdit[2]->SetEditStyle( esText );
	m_pPCMemeberEdit[2]->SetTextColor( 255, 255, 255 );
	m_pPCMemeberEdit[2]->SetText( _RT("") );
	m_pPCMemeberEdit[2]->SetReturnActionClear( false );
	m_pPCMemeberEdit[2]->InitEditBox();
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pPCMemeberEdit[2]->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	sTempRect.x	= 78;
	sTempRect.y	= 230;
	sTempRect.w	= 58;
	sTempRect.h	= 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 128;

#endif	
	
	m_pConfirmBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
										m_pFrameWnd->GetID(),  sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );      
	m_pConfirmBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pConfirmBtn->SetFontR( n_fRsvFontWndButton );
	m_pConfirmBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );	

	sTempRect.x	= 152;
	sTempRect.y	= 230;
	sTempRect.w	= 58;
	sTempRect.h	= 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 202;

#endif	
	
	m_pCancleBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
										m_pFrameWnd->GetID(), sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pCancleBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pCancleBtn->SetFontR( n_fRsvFontWndButton );
	m_pCancleBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );


}

void CFrontierCreateWnd::Update()
{


}

void CFrontierCreateWnd::Proc( SEventMessage* EMsg )
{
	///-- 앤터키가 들어왔을때의 처리
	if( NM_EDT_RETURN_ACTION == EMsg->Notify )
	{
		if( (WNDID)EMsg->FocusWnd == m_pPNameEdit->GetID() )
		{
			nRui->SendEvent( m_pPCMemeberEdit[0]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( m_pPCMemeberEdit[0]->GetID() );
		}
#ifdef DECO_RENEWAL_GUILD_MJH
#else 
		if( (WNDID)EMsg->FocusWnd == m_pPCMemeberEdit[0]->GetID() )
		{
			nRui->SendEvent( m_pPCMemeberEdit[1]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( m_pPCMemeberEdit[1]->GetID() );
		}

		if( (WNDID)EMsg->FocusWnd == m_pPCMemeberEdit[1]->GetID() )
		{
			nRui->SendEvent( m_pPCMemeberEdit[2]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( m_pPCMemeberEdit[2]->GetID() );
		}

		if( (WNDID)EMsg->FocusWnd == m_pPCMemeberEdit[2]->GetID() )
		{
			if( !ConfirmInputContent() )
			{
				nRui->SetEnterFlag( n_nfEditEnter );
			}
			//확인 버튼 누르는 적용효과		
		}
#endif // DECO_RENEWAL_GUILD_MJH
	}

	///-- 윈도우 닫기 버튼
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}

	///-- 확인 버튼
	if( EMsg->FocusWnd == m_pConfirmBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ConfirmInputContent();
		}
	}

	///-- 취소 버튼
	if( EMsg->FocusWnd == m_pCancleBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}
}

BOOL CFrontierCreateWnd::ConfirmInputContent()
{
	TCHAR strTemp[256] = {0,};

	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	
	nRui->SendEvent( m_pPNameEdit->GetID(), n_emGetText, (EPARAM)PName, nBufLen, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[0]->GetID(), n_emGetText, (EPARAM)PCMemeber1, nBufLen, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[1]->GetID(), n_emGetText, (EPARAM)PCMemeber2, nBufLen, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[2]->GetID(), n_emGetText, (EPARAM)PCMemeber3, nBufLen, 0, 0 );

	if( Rstrlen( RWCHAR(PName) ) <= 0  )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_INPUT_FRONTIER_NAME  ), 1200, NULL );			
		nRui->SendEvent( m_pPNameEdit->GetID(), n_emSetFocus, 0, 0, 0, 0 );
		return FALSE;
	}

	///-- 프론티어 네임에 ' '<--스페이스 안들어가게 막기
	char *str = (char *)PName;	
	while( *str != '\0' )
	{
		if( *str == ' '  )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSG_REMOVE_SPACE_INFNAME  ), 1200, NULL );				
			nRui->SendEvent( m_pPNameEdit->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			return FALSE;	
		}

		str++;
	}

	if(g_RockClient.m_SlangFilter.IsCheckSymbol(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ),RWCHAR(PName) ))
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1200 , NULL );
		return FALSE;
	}

	if( !g_RockClient.m_SlangFilter.IsValidString( PName ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
		return FALSE;
	}		
	
	if( g_RockClient.m_SlangFilter.ISFindFilterName( PName ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
		return FALSE;
	}

	//프론티어 이름에 필터링 적용	
	if( g_RockClient.m_SlangFilter.Filter( ( char * ) PName ) )
	{
		memset( PName , 0 , sizeof(TCHAR) * 256 );
		Rsprintf( RWCHAR(PName), _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString() ));
	}
#ifdef DECO_RENEWAL_GUILD_MJH
#else 
	if( Rstrlen( RWCHAR(PCMemeber1) ) <= 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_INPUT_BEGINMEMBER_1  ), 1200, NULL );			
		nRui->SendEvent( m_pPCMemeberEdit[0]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
		return FALSE;
	}

	///-- 창단 멤버에 자신의 이름이 있는지 여부 확인
	if(Rstrcmp(PcParam->PCName, PCMemeber1) == 0)
	{
		SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CNAT_OWN_NAME ) );			 	 
		return true;
	}

	///-- 창단 멤버 1 - 가시범위 내에 존재하는지 여부 확인
	PCTABLE *pPcTable = g_Pc.FindPcTable(PCMemeber1);
	if(!pPcTable)
	{
		Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_SYSPR_PT_OUTOFVIEW ), RWCHAR(PCMemeber1));		
		MessageBoxWnd.Open( strTemp, 1200, NULL );			
		return FALSE;
	}

	if( Rstrlen( RWCHAR(PCMemeber2) ) <= 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_INPUT_BEGINMEMBER_2  ), 1200, NULL );			
		nRui->SendEvent( m_pPCMemeberEdit[1]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
		return FALSE;
	}
	else
	{
		///-- 창단 멤버 1하고 이름이 겹치는지 확인
		if(	!Rstrcmp( PCMemeber1, PCMemeber2 ) )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_INPUT_BEGINMEMBER_2_AGAIN  ), 1200, NULL );		
			nRui->SendEvent( m_pPCMemeberEdit[1]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			return FALSE;
		}
	}

	///-- 창단 멤버에 자신의 이름이 있는지 여부 확인
	if(Rstrcmp(PcParam->PCName, PCMemeber2) == 0)
	{
		SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CNAT_OWN_NAME ) );			 	 
		return true;
	}

	///-- 창단 멤버 2 - 가시범위 내에 존재하는지 여부 확인	
	pPcTable = g_Pc.FindPcTable(PCMemeber2);
	if(!pPcTable)
	{
		Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_SYSPR_PT_OUTOFVIEW ), RWCHAR(PCMemeber2));		
		MessageBoxWnd.Open( strTemp, 1200, NULL );			
		return FALSE;
	}

	if( Rstrlen( RWCHAR(PCMemeber3) ) <= 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_INPUT_BEGINMEMBER_3 ), 1200, NULL );				
		nRui->SendEvent( m_pPCMemeberEdit[2]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
		return FALSE;
	}
	else
	{
		///-- 창단 멤버 1하고 이름이 겹치는지 확인
		if(	!Rstrcmp( PCMemeber3, PCMemeber1 ) || !Rstrcmp( PCMemeber3, PCMemeber2 ) )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_INPUT_BEGINMEMBER_3_AGAIN  ), 1200, NULL );		
			nRui->SendEvent( m_pPCMemeberEdit[2]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			return FALSE;
		}
	}

	///-- 창단 멤버에 자신의 이름이 있는지 여부 확인
	if(Rstrcmp(PcParam->PCName, PCMemeber3) == 0)
	{
		SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CNAT_OWN_NAME ) );			 	 
		return true;
	}
	
	///-- 창단 멤버 3 - 가시범위 내에 존재하는지 여부 확인	
	pPcTable = g_Pc.FindPcTable(PCMemeber3);
	if(!pPcTable)
	{
		Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_SYSPR_PT_OUTOFVIEW ), RWCHAR(PCMemeber3));		
		MessageBoxWnd.Open( strTemp, 1200, NULL );			
		return FALSE;
	}
#endif // DECO_RENEWAL_GUILD_MJH	
#ifdef FRONTIER
	
	#ifdef DECO_RENEWAL_GUILD_MJH
		g_RockClient.Send_CsReqMakeFrontier( PName );
	#else 
		g_RockClient.Send_CsReqMakeFrontier(PName, PCMemeber1, PCMemeber2, PCMemeber3);	
	#endif // DECO_RENEWAL_GUILD_MJH
	
#endif
	
	Close();
	
	return TRUE;
}

void CFrontierCreateWnd::Open()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SendEvent( m_pPNameEdit->GetID(), n_emSetFocus, 0, 0, 0, 0 );
}

void CFrontierCreateWnd::Close()
{
	memset(PName, 0 , sizeof(TCHAR) * 256 );
	memset(PCMemeber1, 0 , sizeof(TCHAR) * 256 );
	memset(PCMemeber2, 0 , sizeof(TCHAR) * 256 );
	memset(PCMemeber3, 0 , sizeof(TCHAR) * 256 );

/*	nRui->SendEvent( m_pPCMemeberEdit[0]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[0]->GetID(), n_emClearText, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[1]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[1]->GetID(), n_emClearText, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[2]->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPCMemeberEdit[2]->GetID(), n_emClearText, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPNameEdit->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPNameEdit->GetID(), n_emClearText, 0, 0, 0, 0 );	
*/	
	
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DECO_RENEWAL_GUILD_MJH
static bool	g_IsAscend_Sort = false; // or Descend_Sort

int SortMemberScrollGradeList( const void *arg1, const void *arg2 );

int SortMemberScrollGradeList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;

	
	if( g_IsAscend_Sort )
	{
		if( a1->Grade == a2->Grade )
		{
			return 0;
		}
		else if( a1->Grade < a2->Grade )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->Grade == a2->Grade )
		{
			return 0;
		}
		else if( a1->Grade > a2->Grade )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

CFrontierCreateScrollWnd::CFrontierCreateScrollWnd()
{	
	m_pFrameWnd = NULL;
	m_pCloseBtn = NULL;
	m_pCreateTime = NULL; 

	for (int i =0 ; i < 10 ; i++)
	{
		m_pNamebox[i] = NULL;
	}
	Init();
}

CFrontierCreateScrollWnd::~CFrontierCreateScrollWnd()
{

}

void CFrontierCreateScrollWnd::Init()
{
	m_CurTime = timeGetTime();
	m_NowTime = timeGetTime();
	m_Cumulative = 0;
	m_MilliSec = 0 ; 
	m_FrontierLimitTime = 0;
}

void CFrontierCreateScrollWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect FCWnd;
	FCWnd.w	= 288;
	FCWnd.h	= 260;
	FCWnd.x	= center_x - ( FCWnd.w / 2 );
	FCWnd.y	= 354;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	FCWnd.w	= 388;

#endif
//-----------------------------------------------------------------------------------------------------------------
// 길드 창설 명단표
//-----------------------------------------------------------------------------------------------------------------	
	
	TCHAR tempBuf[256] = {0,};
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_FrontierCreateScroll, 
								WID_None, FCWnd.x, FCWnd.y, FCWnd.w, FCWnd.h+50, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );	
	m_pFrameWnd->SetCaption( "길드 창설 명단표" );						
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetMove( true );
	//m_pFrameWnd->SetZorder( n_zoTop );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

//-----------------------------------------------------------------------------------------------------------------
// 길드 네임 리스트
//-----------------------------------------------------------------------------------------------------------------	

	for( int j = 0 ; j < CREATE_MEMBER_MAX_COUNT ; ++j )
	{
		int TxtPosY = 23*j;

		m_pNamebox[j]  =(CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
								 WID_FrontierCreateScroll, 10, 30+TxtPosY, 150, 20, false );
		m_pNamebox[j]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal ); 
		m_pNamebox[j]->SetClientImage( TID_None );
		m_pNamebox[j]->SetClientColor( 180, 175, 160 );
		m_pNamebox[j]->SetAlignText( n_atLeft, n_atCenter );
		m_pNamebox[j]->SetText("");
		m_pNamebox[j]->SetTextColor( 0, 0, 0 );
	}
//-----------------------------------------------------------------------------------------------------------------
// 확인버튼
//-----------------------------------------------------------------------------------------------------------------
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, 
        WID_None, WID_FrontierCreateScroll, 190, 270, 58, 25, false );
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );
	m_pCloseBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );

//-----------------------------------------------------------------------------------------------------------------
// 길드 창단 제한시간
//-----------------------------------------------------------------------------------------------------------------

	m_pCreateTime = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, 
								 WID_FrontierCreateScroll, 10, 270, 150, 25, false );
	m_pCreateTime->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal ); 
	m_pCreateTime->SetClientImage( TID_None );
	m_pCreateTime->SetClientColor( 180, 175, 160 );
	m_pCreateTime->SetAlignText( n_atLeft, n_atCenter );
	m_pCreateTime->SetText("길드 창설 제한시간" );
	m_pCreateTime->SetTextColor( 0, 0, 0 );
		
}

void CFrontierCreateScrollWnd::Update()
{
	m_NowTime = timeGetTime(); //time
	m_Cumulative +=  m_NowTime - m_CurTime;
	m_CurTime = m_NowTime;
	if( GetSec() >= 1 )
	{
		if( m_FrontierLimitTime != NULL)
		{
			m_FrontierLimitTime -= m_MilliSec;
			if((int)m_FrontierLimitTime <= 0  )
			{
					m_FrontierLimitTime = 0 ;
			}
		}
		m_Cumulative -= (m_MilliSec*1000);
	}
	// 시간 갱신 
	FrontierLimitTime();

}

void CFrontierCreateScrollWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}
}

int CFrontierCreateScrollWnd::GetSec()
{

	m_MilliSec = m_Cumulative/1000;

	return m_MilliSec;
}

void CFrontierCreateScrollWnd::FrontierMemberInfoList()
{
	int i = 0;
	static TCHAR tmpStr[128] = {0,};
	static TCHAR tmpStr2[128] = {0,};
	
	memset(tmpStr, 0 ,sizeof(tmpStr));
	memset(tmpStr2, 0 ,sizeof(tmpStr2));

	std::vector < SFMemberInfo > :: iterator ir;
	// 등급순으로 정렬
	qsort( (void *) &g_FrontierMemberListWnd.m_FrontierMemberInfoList[0],(size_t) g_FrontierMemberListWnd.m_FrontierMemberInfoList.size(), 
		sizeof( SFMemberInfo ), SortMemberScrollGradeList );
	//사이즈 받아오기
	int MemberListNum = g_FrontierMemberListWnd.m_FrontierMemberInfoList.size();
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();

	for ( i = 0 ; i < CREATE_MEMBER_MAX_COUNT ; i++)
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%d :%s"), i + 1 , G_STRING_CHAR( IDS_CHARSTATE_GUILD ) );
		m_pNamebox[i]->SetText( RWCHAR(tmpStr) );
	}
	
	for ( i = 0 ; i < MemberListNum ; i++) // 길드 인원 표시
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%d : %s"), i + 1 ,RWCHAR( ir[i].Name )); 
		m_pNamebox[i]->SetText(RWCHAR(tmpStr));
	}
		
}

void CFrontierCreateScrollWnd::FrontierLimitTime()
{
	static TCHAR tmpStr[128] = {0,};
	// 서버에서 남은시간 받아온다음 변환 

	tm Time = g_ListTime.GetTimeTm( m_FrontierLimitTime );

	if (Time.tm_mday != 0  )
	{
		Rsprintf(RWCHAR(tmpStr) , _RT("%s : %d%s/%d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_mday , G_STRING_CHAR(IDS_HELPTIP_DAY), Time.tm_hour ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else if(Time.tm_hour != 0  )
	{
		Rsprintf(RWCHAR(tmpStr) , _RT("%s : %d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_hour ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else if(Time.tm_min	!= 0 )
	{
		Rsprintf(RWCHAR(tmpStr) , _RT("%s : %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else 
	{
		Rsprintf(RWCHAR(tmpStr) , _RT("%s : %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			1 , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}	
	//출력
	m_pCreateTime->SetText(tmpStr);
	
}

void CFrontierCreateScrollWnd::Open()
{
	// 길드원 리스트 
	FrontierLimitTime();
	FrontierMemberInfoList();
	nRui->SendEvent( WID_FrontierCreateScroll, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CFrontierCreateScrollWnd::Close()
{
	nRui->SendEvent( WID_FrontierCreateScroll, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

#endif //DECO_RENEWAL_GUILD_MJH
