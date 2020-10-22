///////////////////////////////////////////////////////////////////////////////
///
///		File		: ComboBox.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "ComboBox.h"
#include "ImageBox.h"

//-----------------------------------------------------------------------------
CComboBox::CComboBox( SDesktop* DT ) : CWnd( DT )
{
	FocusPos = cbfClientFocus;

	SAFE_STR_CPY( Text, _RT(""), MAX_TEXT_LENGTH );
	TextBox = SAFE_NEW( CTextBox( DT ) );	
	Button = SAFE_NEW( CButton( DT ) );	

	isActiveListBox = false;
	ListBox = SAFE_NEW( CListBox( DT ) );	
	img_outline= SAFE_NEW( CImageBox9( DT ) );	

	ComboHeight = 16;
	
	ml_sel_idx = 0;
	ml_sel_value = 0;
}

//-----------------------------------------------------------------------------
CComboBox::~CComboBox()
{
	SAFE_DELETE(ListBox);
	SAFE_DELETE(Button);
	SAFE_DELETE(TextBox);
	SAFE_DELETE(img_outline);
}

void CComboBox::SetVisible( bool Visible )
{
	TextBox->SetVisible( Visible );
	isVisible = Visible;
};
	
//-----------------------------------------------------------------------------
void CComboBox::CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float PosX, float PosY, int Width, int Height, bool isRatio )
{
	

	this->WType = WType;

	if( Iid == WID_None ){	Iid = GetWndID();	}
	this->Iid = Iid;
	this->Pid = Pid;

	Pwnd = GetParentHandle();

	if( isRatio )
	{
		POINT Ratio;
		SetRatio( &Ratio, PosX, PosY );
		PosX = Ratio.x;
		PosY = Ratio.y;
	}

	Csize[0] = 0;
	Csize[1] = 0;
	Csize[2] = Width;
	Csize[3] = Height;

	Rpos[0]	= PosX; //부모윈도우에 대한 상대 위치(relative)
	Rpos[1]	= PosY;
	Apos[0]	= 0; //계층구조에 따라 계산한 절대 위치(absolute)
	Apos[1]	= 0;

	if( Pid == WID_None )//부모가 없으면
	{
		Apos[0] = PosX;
		Apos[1] = PosY;
	}
	else
	{
		Apos[0] = Pwnd->Apos[0] + PosX;
		Apos[1] = Pwnd->Apos[1] + PosY;
	}
	
	Cx = Apos[0];
	Cy = Apos[1];
	Cw = Csize[2];
	Ch = Csize[3];
	
	///-- TextBox
	TextBox->CreateWnd( n_wtTextBox, WID_None, WID_None, Cx, Cy, Cw-ComboHeight, ComboHeight, false );	
	TextBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	TextBox->SetAlignText( n_atLeft, n_atCenter );
	TextBox->SetText( _RT( "" ) );
	TextBox->SetTextColor( 255, 255, 255 );
	TextBox->SetTextAlpha( 255 );
	TextBox->SetBlank( 10, 0, 2, 0 );
	TextBox->SetMultiLine( false );
	TextBox->SetIntegralText( false );
	TextBox->SetFocusCheck( false );
	TextBox->SetClientImage( TID_None );		 

	///-- DropDown Button
	Button->CreateWnd( n_wtButton, WID_None, WID_None, (Cx+Cw)-ComboHeight, Cy, 15, 15, false );
	Button->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	Button->SetBtnImage( TID_CTRL_SCR1_DBT, TID_CTRL_SCR1_DBT_OVR, TID_CTRL_SCR1_DBT_CLK, TID_None );  
		
	///-- Drop Down List
	ListBox->CreateWnd( n_wtListBox, WID_None, WID_None, Cx, Cy+ComboHeight, Cw, Ch-ComboHeight, false );
	ListBox->InitScrollBar();
	ListBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	ListBox->SetScrollBarWidth( 15 );
	ListBox->SetTextColor( 0, 0, 0 );
	ListBox->SetClientImage( TID_None );	
	ListBox->SetLBoxBlank( 2, 2, 2, 2 );
	ListBox->SetInsertOrder( ioLast );
	ListBox->SetLineHeight( 14 );		

	///-- Drop Down Image
	SRect size;
	ListBox->GetClientSize( &size );
	img_outline->CreateWnd( n_wtImageBox9, WID_None, WID_None, size.x, size.y, size.w, size.h, false );       	
	img_outline->SetFocusCheck( false );
}

///---------------------------------------------------------------------------
///-- SetWndTileSizeLR
///---------------------------------------------------------------------------
void CComboBox::SetListboxTileSizeLR( const int al_left, const int al_right )
{
	img_outline->SetWndTileSizeLR( al_left, al_right );
	img_outline->SetAdjustPosition( 0, 0 );	
}
///---------------------------------------------------------------------------
///-- SetWndTileSizeTB
///---------------------------------------------------------------------------
void CComboBox::SetListboxTileSizeTB( const int al_left, const int al_right )
{
	img_outline->SetWndTileSizeTB( al_left, al_right );
	img_outline->SetAdjustPosition( 0, 0 );	
}
///---------------------------------------------------------------------------
///-- SetListboxImage9
///---------------------------------------------------------------------------
void CComboBox::SetListboxImage9( TEXID Cimg  )
{
	img_outline->SetClientImage( Cimg );
}
///---------------------------------------------------------------------------
///-- SetClientColor
///---------------------------------------------------------------------------
void CComboBox::SetClientColor( UCHAR Red, UCHAR Green, UCHAR Blue )
{
	ListBox->SetClientColor( Red, Green, Blue );
	img_outline->SetClientColor( Red, Green, Blue );
	TextBox->SetClientColor( Red, Green, Blue );
	Button->SetClientColor( Red, Green, Blue );
}	
///---------------------------------------------------------------------------
///-- SetClientAlpah
///---------------------------------------------------------------------------
void CComboBox::SetClientAlpah( UCHAR Alpha )
{
	ListBox->SetClientAlpah( Alpha );
	img_outline->SetClientAlpah( Alpha );
	TextBox->SetClientAlpah( Alpha );
	Button->SetClientAlpah( Alpha );
}
///---------------------------------------------------------------------------
///-- EventProc
///---------------------------------------------------------------------------
SEventMessage* CComboBox::EventProc( SEventMessage* EMsg )//OR
{
	
	if( FocusPos == cbfButtonFocus )
	{
		Button->EventProc( EMsg );	
	}
	if( isActiveListBox )
	{
		if( FocusPos == cbfListBoxFocus ){	ListBox->EventProc( EMsg );	}
	}
	

	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;

	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		OnMouseFocus( Mx, My );
		break;
	case MA_LDOWN:
		OnMouseLButtonDown( Mx, My );
		if( FocusPos == cbfListBoxFocus )
		{
			EMsg->Notify = NM_CB_TEXT_EXCHANGE;
			EMsg->FocusWnd = Iid;
		}
		break;
	case MA_LPRESS:
		OnMouseLButtonPress( Mx, My );
		break;
	case MA_LUP:
		OnMouseLButtonUp( Mx, My );
		break;
	case MA_LDRAG:
		OnMouseLDrag( Mx, My );
		break;
	case MA_LDROP:
		OnMouseLDrop( Mx, My );
		break;
	}

	return( EMsg );
}

///---------------------------------------------------------------------------
///-- AddItem
///---------------------------------------------------------------------------
stTextList* CComboBox::AddItem( void* Str, const int al_value_D )
{
	char *pStr = (char *)Str;

	return ListBox->AddItem( pStr, al_value_D );
}

///---------------------------------------------------------------------------
///-- ClearItem
///---------------------------------------------------------------------------
void CComboBox::ClearItem()
{
	int item_num = ListBox->GetNumItem();

	
	for( int i = item_num - 1; i >= 0; --i )
	{
		ListBox->DeleteItem( i );
	}
	ListBox->ClearItem();

	ml_sel_value = 0;
	ml_sel_idx = 0;
	TextBox->SetText( _RT("") );
	return;
}

//-----------------------------------------------------------------------------
void CComboBox::RenderProc()//OR
{
	UpdateHandler();

	TextBox->RenderProc();
	Button->RenderProc();
	if( isActiveListBox )
	{		
		img_outline->RenderProc();
		ListBox->RenderProc();
	}
}

//-----------------------------------------------------------------------------
IRESULT CComboBox::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )//OR
{
	int Iresult( 0 );
	Iresult = CWnd::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emAddItem:
		ListBox->AddItem( (TCHAR*)fParam );
		break;
	case n_emGetSelectedItem:
		ListBox->GetSelectedText( (TCHAR*)fParam, sParam );
		break;
	}

	return( Iresult );
}

//-----------------------------------------------------------------------------
WNDID CComboBox::FocusCheck( int Mx, int My, bool IsChildCheck )//OR
{
	IsChildCheck;

	if( isFocusCheck )
	{
		//Client BoundingBox 계산
		int Bx = Cx; 
		int By = Cy;
		int Bw = Cx + Cw;
		int Bh( 0 );

		if( isActiveListBox == true )
		{
			Bh = Cy + Ch;
		}
		else
		{
			Bh = Cy + ComboHeight;
		}
		

		if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) )
		{
			FocusPos = cbfClientFocus;

			if( TextBox->FocusCheck( Mx, My, false ) != WID_None )
			{
				FocusPos = cbfTextBoxFocus;
			}
			if( Button->FocusCheck( Mx, My, false ) != WID_None )
			{
				FocusPos = cbfButtonFocus;
			}
			if( isActiveListBox )
			{
				if( ListBox->FocusCheck( Mx, My, false ) != WID_None )
				{
					FocusPos = cbfListBoxFocus;
				}
			}

			return( Iid );
		}
	}
	
	return( WID_None );
}

//-----------------------------------------------------------------------------
void CComboBox::SetAdjustPosition( int PosX, int PosY )//OR
{	
	TextBox->SetWndPositionR( PosX, PosY );
	Button->SetWndPositionR( PosX, PosY );
	ListBox->SetWndPositionR( PosX, PosY );
	img_outline->SetWndPositionR( PosX, PosY );
}

//-----------------------------------------------------------------------------
void CComboBox::UpdateHandler()//OR
{
	if( !isInit )
	{
		isInit = true;
	}
}

//-----------------------------------------------------------------------------
void CComboBox::OnMouseFocus( int Mx, int My )//OR
{
	Mx; My;
}
///---------------------------------------------------------------------------
///-- GetCurSel
///---------------------------------------------------------------------------
int CComboBox::GetCurSelIdx()
{
	return ml_sel_idx;
}
///---------------------------------------------------------------------------
///-- GetCurSelValue
///---------------------------------------------------------------------------
int CComboBox::GetCurSelValue()
{
	return ml_sel_value;
}
///---------------------------------------------------------------------------
///-- SetCurSelIdx
///---------------------------------------------------------------------------
void CComboBox::SetCurSelIdx( const int al_idx )
{
	TCHAR *TEMP;
	assert( al_idx >= 0 );
	if( ListBox->SetCurSelIdx( al_idx ) )
	{	
		TEMP = ListBox->GetItem( al_idx );
		SAFE_STR_CPY( Text, TEMP, MAX_TEXT_LENGTH );
		if( Text )
		{	
			ListBox->GetSelectedItem( &ml_sel_idx, &ml_sel_value );
			TextBox->SetText( Text );
		}
	}
}
///---------------------------------------------------------------------------
///-- SetSelectText
///---------------------------------------------------------------------------
void CComboBox::SetSelectText( void* strTxt )
{
	TextBox->SetText( strTxt );
	
	return;
}

///---------------------------------------------------------------------------
///-- OnMouseLButtonDown
///---------------------------------------------------------------------------
///-- 정규적인 좌표 클릭일 경우만 TEXT 지정
void CComboBox::OnMouseLButtonDown( int Mx, int My )//OR
{
	Mx; My;

	if( FocusPos == cbfListBoxFocus )
	{
		if( ListBox->GetSelectedText( Text, MAX_TEXT_LENGTH ) )
		{
			ListBox->GetSelectedItem( &ml_sel_idx, &ml_sel_value );
			TextBox->SetText( Text );

		}
	}
}

//-----------------------------------------------------------------------------
void CComboBox::OnMouseLButtonPress( int Mx, int My )//OR
{
	Mx; My;
}

//-----------------------------------------------------------------------------
void CComboBox::OnMouseLButtonUp( int Mx, int My )//OR
{
	Mx; My;
	
	if( FocusPos == cbfButtonFocus )
	{
		isActiveListBox = !isActiveListBox;
	}
	if( FocusPos == cbfListBoxFocus )
	{
		if( ListBox->GetFocusPos() == 0 )//0 == lbfClientFocus
		{
			isActiveListBox = false;
		}
	}
}

//-----------------------------------------------------------------------------
void CComboBox::OnMouseLDrag( int Mx, int My )//OR
{
}

//-----------------------------------------------------------------------------
void CComboBox::OnMouseLDrop( int Mx, int My )//OR
{
}

//-----------------------------------------------------------------------------