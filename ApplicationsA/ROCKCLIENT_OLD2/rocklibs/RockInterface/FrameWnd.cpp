///////////////////////////////////////////////////////////////////////////////
///
///		File		: FrameWnd.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Rui.h"

#include "FrameWnd.h"

#include "..\\..\\brother.h"

#ifndef _LEFT 

#define _LEFT 0
#define _TOP 1
#define _WIDTH 2
#define _HEIGHT 3
#define _TILE_SIZE 30
#define _TILE_SIZEX2 60

#endif

///!!!!

///-- 0 �̸� ����
///-- 1�̸� �������η� ����
///-- 2�̸� ���ķ� ����
CFrameWnd::CFrameWnd()
{
	
}


//-----------------------------------------------------------------------------
CFrameWnd::CFrameWnd( SDesktop* pDT ) : CWnd( pDT )
{
	signed long i;
	DeltaX = 0;//���콺 �̵��� ��� ��ǥ
	DeltaY = 0;
	
	///-- �⺻������ Ÿ�� Size �� ���� �Ѵ�.
	for( i = 0 ; i < 9; ++i )
	{
		maal_tile[i][_WIDTH] = maal_tile[i][_HEIGHT] = _TILE_SIZE;	
	}
	

	isTitle = true;//Ÿ��Ʋ �� ��� ����
	isCloseBtn = true;
	isVisibleEditFocus = false;
	isResize = false;//������ ���� ��� ����
	me_WndStytle = fwfTex_Style_Normal;
	//isDrag = false;
	isMove = true;
	mb_title_back = false;

	TitleText = SAFE_NEW( CTextBox( pDT ) );	
	TitleBack = SAFE_NEW( CImageBox( pDT ) );	

	SAFE_STR_CPY( theCaptionStr, _RT(""), 64 );
	
	FocusChildWid = WID_None;
	FocusChildWnd = NULL;
	
	
	CurrentEditNum = 0;
	NumEdit = 0;
	memset( EditList, 0, sizeof(WNDID)*10 );
	
	
	FocusPos		= fwfNoneFocus;
	
	theWnd			= NULL;
	IconTID = TID_CTRL_ICON_NONE;
	
	//by simwoosung
	pMoveLinkFrame = NULL;
	pMoveLinkFrame2 = NULL;


	me_do_alpha = fwfWnd_Alpha_None;	
}

//-----------------------------------------------------------------------------
CFrameWnd::~CFrameWnd()
{
	SAFE_DELETE(TitleText);
	SAFE_DELETE(TitleBack);
}

void CFrameWnd::SetMoveLinkFrameWnd(CFrameWnd *pFrameWnd)
{
	pMoveLinkFrame = pFrameWnd;
}

void CFrameWnd::SetMoveLinkFrameWnd2(CFrameWnd *pFrameWnd)
{
	pMoveLinkFrame2 = pFrameWnd;
}

CFrameWnd * CFrameWnd::GetMoveLinkFrameWnd()
{
	return pMoveLinkFrame;
}

CFrameWnd*	CFrameWnd::GetMoveLinkFrameWnd2()
{
	return pMoveLinkFrame2;
}


///---------------------------------------------------------------------------
///-- SetTitleBack
///---------------------------------------------------------------------------
void CFrameWnd::SetTitleBack( const bool ab_titleback )
{
	mb_title_back = ab_titleback;
}
//-----------------------------------------------------------------------------
void CFrameWnd::CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float PosX, float PosY, int Width, int Height, bool isRatio )
{
	this->WType = WType;

	if( Iid == WID_None )
	{	
		Iid = GetWndID();	
	}
	
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

	Apos[0]	= 0; //���������� ���� ����� ���� ��ġ(absolute)
	Apos[1]	= 0;

	if( Pid == WID_None )//�θ� ������
	{
		Rpos[0]	= 0; //�θ������쿡 ���� ��� ��ġ(relative)
		Rpos[1]	= 0;		

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
		
		Apos[0] = PosX + m_fExtentW;
		Apos[1] = PosY + m_fExtentH;
		
#else
		
		Apos[0] = PosX;
		Apos[1] = PosY;
		
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)

	}
	else
	{
		Rpos[0]	= PosX; //�θ������쿡 ���� ��� ��ġ(relative)
		Rpos[1]	= PosY;

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
		
		Apos[0] = Pwnd->Apos[0] + PosX + Pwnd->m_fExtentW;
		Apos[1] = Pwnd->Apos[1] + PosY + Pwnd->m_fExtentH;
		
#else
		
		Apos[0] = Pwnd->Apos[0] + PosX;
		Apos[1] = Pwnd->Apos[1] + PosY;
		
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)

	}
	
	Cx = Apos[0];
	Cy = Apos[1];
	Cw = Csize[2];
	Ch = Csize[3];
}

//-----------------------------------------------------------------------------
void CFrameWnd::Initialize()
{
	int BarX( 0 );
	int BarY( 0 );
	int BarW( 0 );
	int BarH( 0 );

	/*
	if( isTitle )
	{
		BarX = Cx;
		BarY = Cy-20;
		BarW = Cw;
		BarH = 20;
	}
	else

	*/
	{
		BarX = Cx;
		BarY = Cy;
		BarW = Cw;
		BarH = 0;
	}
	

	TitleBack->CreateWnd( n_wtImageBox, WID_None, WID_None, BarX -5,  BarY -3, 170, 30, false );
	TitleBack->SetClientImage( TID_CTRL_WND_TITLE_BACK );
	TitleBack->SetFocusCheck( false );
	
	TitleText->CreateWnd( n_wtTextBox, WID_None, WID_None, BarX + ml_margin_x, BarY + ml_margin_y, BarW, 20, false );	
	TitleText->SetFontR( n_fRsvFontTitle );		
	TitleText->SetText( theCaptionStr );
	
	//	TitleText->SetFontDecoration( n_fdNormal );	
	///TitleText->SetBlank( 2, 20, 1, 1 );

	SetAdjustPosition( 0, 0 );
}

//-----------------------------------------------------------------------------
SEventMessage* CFrameWnd::EventProc( SEventMessage* EMsg )
{
	static int prev_mx = 0;
	static int prev_my = 0;

	DeltaX = EMsg->Mouse.Rposition.x;
	DeltaY = EMsg->Mouse.Rposition.y;

	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;

	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		EMsg->FocusWnd	= Iid;
		OnMouseFocus( Mx, My );
		break;
	case MA_LDOWN:
		OnMouseLButtonDown( Mx, My );
		break;
	case MA_LPRESS:
		OnMouseLButtonPress( Mx, My );
		break;
	case MA_LUP:
		OnMouseLButtonUp( Mx, My );
		EMsg->FocusWnd	= Iid;//pDT->SelectedWnd;
		EMsg->Notify	= NM_FWND_CLICK;
		break;
	case MA_LDRAG:
		OnMouseLDrag( Mx-prev_mx, My-prev_my );
		break;
	case MA_LDROP:
		OnMouseLDrop( Mx, My );
		break;
	case MA_RUP:
		EMsg->FocusWnd = Iid;
		EMsg->Notify = NM_FWND_RCLICK;
		if( theWnd != NULL )
		{
		    theWnd->Proc( EMsg );
		}
		break;	
	}

	prev_mx = Mx;
	prev_my = My;
		
	/// if( FocusPos == fwfTitleText )		{	TitleText->EventProc( EMsg );		}
	if( FocusPos == fwfChildWndFocus )
	{
		FocusChildWnd->EventProc( EMsg );

		if( ( EMsg != NULL ) && ( theWnd != NULL ) )
		{
			theWnd->Proc( EMsg );
		}	

		return( EMsg );
	}

	if( theWnd != NULL )
	{
		theWnd->Proc( EMsg );
	}
	
	return( EMsg );
}

//-----------------------------------------------------------------------------
void CFrameWnd::NotifyEvent()
{
}
///---------------------------------------------------------------------------
///-- RenderProc
///---------------------------------------------------------------------------
///-- Texutre Style �� ���� �ٸ��� Render
///-- 
void CFrameWnd::RenderProc()
{
	signed long i;
	UpdateHandler();

	///-- fwfTex_Style_Tile9	9 ���� Ÿ�Ϸ� �̷���� Wnd
	if ( me_WndStytle == fwfTex_Style_Tile9 )
	{
		for( i = 0 ; i < 9; ++i )
		{
			Render->Draw( Cimg + i, maal_tile[i][_LEFT], maal_tile[i][_TOP], maal_tile[i][_WIDTH], maal_tile[i][_HEIGHT], Ccolor );
		}
	}
	///-- fwfTex_Style_Tile3	3���� Ÿ�Ϸ� �̷���� Wnd
	else if ( me_WndStytle == fwfTex_Style_Tile3 )
	{
		for( i = 0 ; i < 3; ++i )
		{
			Render->Draw( Cimg + i, maal_tile[i][_LEFT], Cy, maal_tile[i][_WIDTH], Ch, Ccolor );
		}
	}
	///-- ����� �׸���.
	else
	{
		Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	}

	RenderWnd();

	///-- Title Back �� Render
	if( mb_title_back )
	{
		TitleBack->RenderProc();
		Render->Draw( IconTID, Cx, Cy, 26, 26, Ccolor );
	}
	
	///-- Title TEXT �� Render
	if( isTitle )
	{
		TitleText->RenderProc();
	}
}
///---------------------------------------------------------------------------
///-- RenderWnd
///---------------------------------------------------------------------------
///-- ���� Wnd �� Alpha �� ������ �Ǿ��ٸ� Child �� ���� alpha ������ ����
///-- Alpha ������ �ѹ��� ����ȴ�.
void CFrameWnd::RenderWnd()
{
	stChainNode* Node = NULL;

	///-- alpha ����
	if( me_do_alpha != fwfWnd_Alpha_None )
	{
		Node = Chain.GetHead();
		
		while( Node != NULL )
		{
			if( Node->Wnd )
			{
				if( me_do_alpha == fwfWnd_Alpha_DO_Normal )
				{
					Node->Wnd->SetClientAlpah( 255 );
					//by simwoosung
					//TID�� TID_CLIENT�ΰ͵� �־ ^^;;
					//Node->Wnd->SetClientColor( 255, 255, 255 );
				}
				else if( me_do_alpha == fwfWnd_Alpha_DO_Alpha )
				{
					Node->Wnd->SetClientAlpah( 200 );
					//Node->Wnd->SetClientColor( 200, 200, 200 );
				}
			}								
			Node = Chain.GetNext( Node );
		}
		///-- ����Ϸ�. 
		me_do_alpha = fwfWnd_Alpha_None;
	}
	
	///-- Render
	Node = Chain.GetHead();

	while( Node != NULL )
	{
		if( Node->Wnd && Node->Wnd->IsVisible() )
		{
			Node->Wnd->RenderProc();
		}								
		Node = Chain.GetNext( Node );
	}

}

//-----------------------------------------------------------------------------
IRESULT CFrameWnd::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CWnd::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emSetVisible:
		if( isVisible )
		{
			theWnd->IsVisible	= true;
			isVisible			= true;
			VisibleFrameWnd();
			
			nRui->Chain.Ordering( Container->Get( Iid ) );
		}
		else
		{
			theWnd->IsVisible	= false;
			isVisible			= false;
			CloseFrameWnd();
		}
		break;

	case n_emCloseFrameWnd:
		theWnd->IsVisible	= false;
		isVisible			= false;
		CloseFrameWnd();
		break;
	
	case n_emEditSpaceAction:
		EditSpaceAction( (WNDID)fParam );
		break;	
	
	case n_emEditReturnAction:
		EditReturnAction( (WNDID)fParam );
		break;

	case n_emEditTabAction:
		EditTabAction( (WNDID)fParam );
		break;
		
	case n_emEditImmConvStatus:
		EditImmConvStatus( (WNDID)fParam, (bool)sParam );
		break;

	case n_emSetCaption:
		SetCaption( (TCHAR*)fParam );
		break;
	}
	
	return( Iresult );
}

//-----------------------------------------------------------------------------
WNDID CFrameWnd::FocusCheck( int Mx, int My, bool IsChildCheck )
{
	IsChildCheck;
	WNDID FocusWid		= WID_None;
	WNDID TempFocusWid	= WID_None;
	
	
	if( isFocusCheck )
	{
		int Tx		= Wsize[0];
		int Ty		= Wsize[1];
		int Twidth	= Wsize[2];
		int Theight	= Wsize[3];

		//---------------------------------------------------------------------
		// Wsize�� ������ �������� �� �����̶�....Wsize Focus Check�ϱ����� �˻�

		FocusPos = fwfNoneFocus; //���۽� �ʱ�ȭ
		TCHAR	str[40] = {0,};
		static int count=0;		

		//---------------------------------------------------------------------
		if( ( (Tx < Mx) && (Twidth > Mx) ) && ( (Ty < My) && (Theight > My) ) )
		{
			FocusPos = fwfClientFocus;

			if( isRegion )
			{
				if( Cimg == TID_None ){	return( Iid );	}

				int Lx = Mx - Tx;//Apos[0];
				int Ly = My - Ty;//Apos[1];
				FocusWid = RegionFocusCheck( Lx, Ly );

				if( FocusWid == WID_None )
				{
					return( WID_None );
				}

				FocusWid = Iid;
			}
			else
			{
				FocusWid = Iid;
			}
			

			//-----------------------------------------------------------------
			/*
			if( isTitle )
			{
				if( TitleText->FocusCheck( Mx, My, false ) != WID_None )
				{
					FocusPos = fwfTitleText;
				}
				if( isCloseBtn )
				{
					if( CloseBtn->FocusCheck( Mx, My, false ) != WID_None )
					{
						FocusPos = fwfCloseButton;
					}
				}
			}
			*/
			
			//-----------------------------------------------------------------
			
			TempFocusWid= ChildFocusCheck( Mx, My );
			if( TempFocusWid != WID_None )
			{
				FocusChildWid = TempFocusWid; //FocusChildWid������ �ʿ������ ������...��....
				FocusWid = TempFocusWid;
				FocusPos = fwfChildWndFocus;
				
				FocusChildWnd = Container->Get( FocusChildWid );
			    
				WORD WinType = -1;
				if(FocusChildWnd)
				{
				   WinType = FocusChildWnd->GetWindowType();
				   /*
				   if(WinType == n_wtComboBox)
				   {
                      Chain.Ordering( FocusChildWnd );
				   }*/
				   
				   //by simwoosung
				   if(WinType == n_wtFrameWnd)
				   {
					  FocusWid = Iid;					 
				   }
				}

				/*if( FocusChildWnd )&& ( FocusChildWnd->GetWindowType() == n_wtComboBox ) )
				{
					Chain.Ordering( FocusChildWnd );
				}*/
			}
			
			return( FocusWid );
		}
	}

	if( FocusPos != fwfNoneFocus )
	{
		FocusWid = Iid;
		return( FocusWid );
	}
		
	return( WID_None );
}

//-----------------------------------------------------------------------------
void CFrameWnd::ResetSize( SRect* size )
{
	Cx = size->x;
	Cy = size->y;
	Cw = size->w;
	Ch = size->h;

	Apos[0]		= Cx;
	Apos[1]		= Cy;
	Csize[2]	= Cw;
	Csize[3]	= Ch;

	//ä�����â�� ���ؼ���..ó��(�ӽ÷�...)
	//Ÿ��Ʋ��, �����ӿ� ���ؼ��� ������� �ʾҴ�.
	Wsize[0]	= Cx;
	Wsize[1]	= Cy;
	Wsize[2]	= Cx + Cw;
	Wsize[3]	= Cy + Ch;
}

//-----------------------------------------------------------------------------
/*
WNDID CFrameWnd::RegionFocusCheck( int Lx, int Ly )
{
	Lx;
	Ly;
	return( WID_None );
}
//*/

///---------------------------------------------------------------------------
///-- ChildFocusCheck
///---------------------------------------------------------------------------
///-- FrameWnd ���� Child �� Focus �� ã�´�
///-- IsFocusCheck false �� ��� ����
WNDID CFrameWnd::ChildFocusCheck( int Mx, int My )
{
	WNDID TempWid = WID_None;
	WNDID FocusWid = WID_None;
	stChainNode* Node = NULL;

	Node = Chain.GetHead();

	while( Node != NULL )
	{
		if( Node->Wnd && Node->Wnd->IsVisible() )
		{
			if(Node->Wnd->IsFocusCheck())
			{
				TempWid = Node->Wnd->FocusCheck( Mx, My, false );
				
				if( TempWid != WID_None )
				{	
					FocusWid = TempWid;
				}
			}
		}
		
		Node = Chain.GetNext( Node );
	}
	
	return( FocusWid );
}

//-----------------------------------------------------------------------------
void CFrameWnd::VisibleFrameWnd()
{
	if( ( NumEdit > 0 ) && ( isVisibleEditFocus == true ) )
	{
		SendEvent( EditList[CurrentEditNum], n_emSetFocus, 0, 0, 0, 0 );
	}
}

//-----------------------------------------------------------------------------
void CFrameWnd::CloseFrameWnd()
{
	//isVisible = false;
	
	//Edit Control�� ��Ŀ���� ������ Desktop�� ��Ŀ�� ��ȯ�Ѵ�.
	if( NumEdit > 0 )
	{	
		SendEvent( EditList[CurrentEditNum], n_emEditDisable, 0, 0, 0, 0 );		
	}

	WNDID TempWid = WID_None;
	WNDID FocusWid = WID_None;
	stChainNode* Node = NULL;

	Node = Chain.GetHead();

	while( Node != NULL )
	{
		if( Node->Wnd->GetWindowType() == n_wtFrameWnd )
		{
			CFrameWnd * pFrameWnd = (CFrameWnd *)(Node->Wnd);
			pFrameWnd->CloseFrameWnd();
		}	
		
		Node = Chain.GetNext( Node );
	}	

}

void CFrameWnd::SetVisible( bool Visible )
{
	CWnd::SetVisible( Visible );

	if(!Visible)
	{
		CloseFrameWnd();
	}
}

void CFrameWnd::EditSpaceAction( WNDID Eid )
{
	SEventMessage EMsg;
	memset( &EMsg, 0, sizeof( SEventMessage ) );

	EMsg.FocusWnd	= Eid;
	EMsg.Notify		= NM_EDT_SPACE_ACTION;
	//EMsg.fParam = (EPARAM)Eid;

	if( theWnd != NULL )
	{
		theWnd->Proc( &EMsg );
	}
}

//-----------------------------------------------------------------------------
void CFrameWnd::EditReturnAction( WNDID Eid )//EditId
{
	SEventMessage EMsg;
	memset( &EMsg, 0, sizeof( SEventMessage ) );

	EMsg.FocusWnd	= Eid;
	EMsg.Notify		= NM_EDT_RETURN_ACTION;
	//EMsg.fParam = (EPARAM)Eid;

	if( theWnd != NULL )
	{
		theWnd->Proc( &EMsg );
	}
}

//-----------------------------------------------------------------------------
void CFrameWnd::EditTabAction( WNDID Eid )//EditId
{
	SEventMessage EMsg;
	memset( &EMsg, 0, sizeof( SEventMessage ) );

	EMsg.FocusWnd	= Eid;
	EMsg.Notify		= NM_EDT_TAB_ACTION;
	EMsg.fParam		= (EPARAM)Eid;

	CurrentEditNum++;
	if( CurrentEditNum >= NumEdit )
	{
		CurrentEditNum = 0;
	}
	
	SendEvent( EditList[CurrentEditNum], n_emSetFocus, 0, 0, 0, 0 );
		

	if( theWnd != NULL )
	{
		theWnd->Proc( &EMsg );
	}
}

//-----------------------------------------------------------------------------
void CFrameWnd::EditImmConvStatus( WNDID Eid, bool isAlphaNumeric )
{
	SEventMessage EMsg;
	memset( &EMsg, 0, sizeof( SEventMessage ) );

	EMsg.FocusWnd	= Eid;
	EMsg.Notify		= NM_EDT_IMMCONVSTATUS;
	EMsg.fParam		= (EPARAM)isAlphaNumeric;
	
	if( theWnd != NULL )
	{
		theWnd->Proc( &EMsg );
	}
}



///---------------------------------------------------------------------------
///-- SetAdjustPosition
///---------------------------------------------------------------------------
void CFrameWnd::SetAdjustPosition( int PosX, int PosY )//OR
{
	signed long int ll_stretch_width;
	signed long int ll_stretch_height;

	if(pMoveLinkFrame)
	{
		pMoveLinkFrame->SetWndPositionR( PosX, PosY );
	}	
	
	if(pMoveLinkFrame2)
	{
		pMoveLinkFrame2->SetWndPositionR( PosX, PosY );
	}	


	if(Pwnd != NULL)
	{
		SRect rect;
		Pwnd->GetClientSize(&rect);
		
		Apos[0] = rect.x + Rpos[0];
		Apos[1] = rect.y + Rpos[1];
		Cx = Apos[0];
		Cy = Apos[1];
	}	
	
	Wsize[0] = Cx;
	Wsize[1] = Cy;
	Wsize[2] = Cx + Cw;
	Wsize[3] = Cy + Ch;
	
	///-- Tile 9 �� Tile Window ���� ���� ����
	if( me_WndStytle == fwfTex_Style_Tile9 )
	{
		ll_stretch_width = Cw - ml_Wnd_width[0] - ml_Wnd_width[1];
		ll_stretch_height= Ch - ml_Wnd_height[0] - ml_Wnd_height[1]; 


		maal_tile[0][_LEFT] = maal_tile[3][_LEFT] = maal_tile[6][_LEFT] = Cx;
		maal_tile[1][_LEFT] = maal_tile[4][_LEFT] = maal_tile[7][_LEFT] = Cx + ml_Wnd_width[0];
		maal_tile[2][_LEFT] = maal_tile[5][_LEFT] = maal_tile[8][_LEFT] = maal_tile[1][_LEFT] + ll_stretch_width;

		maal_tile[0][_TOP] = maal_tile[1][_TOP] = maal_tile[2][_TOP] = Cy;
		maal_tile[3][_TOP] = maal_tile[4][_TOP] = maal_tile[5][_TOP] = Cy + ml_Wnd_height[0];
		maal_tile[6][_TOP] = maal_tile[7][_TOP] = maal_tile[8][_TOP] = maal_tile[3][_TOP] + ll_stretch_height;

		maal_tile[0][_WIDTH] = maal_tile[3][_WIDTH] = maal_tile[6][_WIDTH] = ml_Wnd_width[0];
		maal_tile[1][_WIDTH] = maal_tile[4][_WIDTH] = maal_tile[7][_WIDTH] = ll_stretch_width ;
		maal_tile[2][_WIDTH] = maal_tile[5][_WIDTH] = maal_tile[8][_WIDTH] = ml_Wnd_width[1];

		maal_tile[0][_HEIGHT] = maal_tile[1][_HEIGHT] = maal_tile[2][_HEIGHT] = ml_Wnd_height[0];
		maal_tile[3][_HEIGHT] = maal_tile[4][_HEIGHT] = maal_tile[5][_HEIGHT] = ll_stretch_height ;
		maal_tile[6][_HEIGHT] = maal_tile[7][_HEIGHT] = maal_tile[8][_HEIGHT] = ml_Wnd_height[1];
	}
	
	///-- Tile 3 �� Tile Window ���� ���� ����
	if( me_WndStytle == fwfTex_Style_Tile3 )
	{
		ll_stretch_width = Cw - ml_Wnd_width[0] - ml_Wnd_width[1];

		maal_tile[0][_LEFT] = Cx;
		maal_tile[1][_LEFT] = Cx + ml_Wnd_width[0];
		maal_tile[2][_LEFT] = maal_tile[1][_LEFT] + ll_stretch_width;

		maal_tile[0][_WIDTH]  = ml_Wnd_width[0];
		maal_tile[1][_WIDTH]  = ll_stretch_width;
		maal_tile[2][_WIDTH]  = ml_Wnd_width[1];
	}

	stChainNode* Node = NULL;
	Node = Chain.GetHead();

	while( Node != NULL )
	{
		if( Node->Wnd )
		{
			Node->Wnd->SetWndPositionR( PosX, PosY );
		}
		Node = Chain.GetNext( Node );
	}
}

//-----------------------------------------------------------------------------
void CFrameWnd::Reposition( int x, int y )
{
	if( Pwnd != NULL )
	{
		Cx = x + Rpos[0];
		Cy = y + Rpos[1];
		SetAbsolutePosition( Cx, Cy );
		return;
	}	
	
	//������ġ������...�����ġ������..��ȯ�ؼ�..ó��
	int Rx = x - Cx;
	int Ry = y - Cy;	

	SetWndPositionR( Rx, Ry );

	if( isTitle && TitleText )
		TitleText->SetWndPositionR( Rx, Ry );
	
	TitleBack->SetWndPositionR( Rx, Ry );

	/*
	Apos[0] = x;
	Apos[1] = y;

	Cx = Apos[0];
	Cy = Apos[1];
	Cw = Csize[2];
	Ch = Csize[3];

	Wsize[0] = Cx;
	Wsize[1] = Cy;
	Wsize[2] = Cx + Cw;
	Wsize[3] = Cy + Ch;
//*/


	/*
	stChainNode* Node = NULL;
	Node = Chain.GetHead();

	while( Node != NULL )
	{
		Node->Wnd->SetWndPositionR( Cx, Cy );//�θ��� ���ŵ� ��ġ�� �Ѱ��ش�.
		Node = Chain.GetNext( Node );
	}
	//*/
}

//-----------------------------------------------------------------------------
void CFrameWnd::RegisterChild( WNDID Cid )
{
	Chain.Add( Container->Get( Cid ) );
}

//-----------------------------------------------------------------------------
void CFrameWnd::SetWndProc( void (*WndProc)( SEventMessage* EMsg ) )
{
	this->WndProc = WndProc;	
}

//-----------------------------------------------------------------------------
void CFrameWnd::SetWndProc( CWndProc* Wnd )
{
	theWnd = Wnd;
}

//-----------------------------------------------------------------------------
void CFrameWnd::SetCaption( void* pCaption )
{
	//���ڿ� üũ 
	char* Caption = (char*)pCaption;
	SAFE_STR_CPY( theCaptionStr, Caption, 64 );
	if( TitleText )
	TitleText->SetText( theCaptionStr );
}

///---------------------------------------------------------------------------
///-- SetTitle
///---------------------------------------------------------------------------
///-- Title ���� ���� ��� Back Image �� ����
void CFrameWnd::SetTitle( bool bTitle , const int al_margin_y, const int al_margin_x )
{
	isTitle = bTitle;
	
	if( isTitle )
	{
		mb_title_back = true;
	}
	else
	{
		mb_title_back = false;
	}
	
	ml_margin_x = al_margin_x;
	ml_margin_y = al_margin_y;
	
	Wsize[0] = Cx;
	Wsize[1] = Cy;
	Wsize[2] = Cx + Cw;
	Wsize[3] = Cy + Ch;
	
}

///---------------------------------------------------------------------------
///-- SetTileWnd
///---------------------------------------------------------------------------
void CFrameWnd::SetWndTexStyle( enFrameWndTextureStyle ae_wndtex_style )
{
	me_WndStytle = ae_wndtex_style;		
}
///---------------------------------------------------------------------------
///-- SetWndTileSizeLR
///---------------------------------------------------------------------------
void CFrameWnd::SetWndTileSizeLR( const int al_left, const int al_right )
{
	ml_Wnd_width[0] = al_left;
	ml_Wnd_width[1] = al_right;
}
///---------------------------------------------------------------------------
///-- SetWndTileSizeTB
///---------------------------------------------------------------------------
void CFrameWnd::SetWndTileSizeTB( const int al_top, const int al_bottom )
{
	ml_Wnd_height[0] = al_top;
	ml_Wnd_height[1] = al_bottom;
}

///---------------------------------------------------------------------------
///-- SetIconTID
///---------------------------------------------------------------------------
void CFrameWnd::SetIconTID( const TEXID ae_tid )
{
	IconTID = ae_tid;
}
//-----------------------------------------------------------------------------
void CFrameWnd::SetResize( bool bResize )
{
	isResize = bResize;
}

//-----------------------------------------------------------------------------
void CFrameWnd::SetMove( bool bMove )
{
	isMove = bMove;
}

//-----------------------------------------------------------------------------
void CFrameWnd::UpdateHandler()
{
}

//-----------------------------------------------------------------------------
void CFrameWnd::OnMouseFocus( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CFrameWnd::OnMouseLButtonDown( int Mx, int My )
{
	// ������ �ڽ� ��Ʈ�ѵ鿡�� �̺�Ʈ���� ������ ��Ŀ���ֱ� by wxywxy
	CFrameWnd* fwnd	= (CFrameWnd*)nRui->Chain.GetNormalWndFocusHandle();

	if( fwnd == NULL )
	{
		nRui->SendEvent( GetID() , n_emSetVisible , TRUE , 0, 0, 0 );
	}
	else
	{
		if( GetID() == fwnd->GetID() )
		{
		}
		else
		{
			nRui->SendEvent( GetID() , n_emSetVisible , TRUE , 0, 0, 0 );
		}
	}
}

//-----------------------------------------------------------------------------
void CFrameWnd::OnMouseLButtonPress( int Mx, int My )
{

}

//-----------------------------------------------------------------------------
void CFrameWnd::OnMouseLButtonUp( int Mx, int My )
{
	if( FocusPos == fwfCloseButton )
	{
		CloseFrameWnd();
	}
}
/*
//-----------------------------------------------------------------------------
void CFrameWnd::OnMouseLDragStart( int Mx, int My )
{
	if( FocusPos == fwfClientFocus )
	{
		if( isMove )
		{
			SetWndPositionR( DeltaX, DeltaY );
			isDrag = true;
		}
	}
}
//*/

///-----------------------------------------------------------------------------
///-- �� â�� ������. Mainmenu Bar ������ ���� ���� �ȵȴ�.
///-----------------------------------------------------------------------------
///-- 2005.03.12 / Lyul
///-- Mainmenu �� TOPMOST �̹Ƿ� �� â�� �� ������ ���� ���� �ȵȴ�.
///-- �� �ػ󵵿����� �Ѱ谪�� CMainMenuWnd::Composition() �� ������ �� - 10
///--
void CFrameWnd::OnMouseLDrag( int Mx, int My )
{
	if( isMove )
	{		
		//Ŭ���̾�Ʈ �������� �����������찡 ��������(nDelValue) �̻��� ����� ���ϰ� �����ش�.
		int nDelValue = 25;

		if( (Apos[0] + Cw) + Mx < nDelValue )				//���ʸ���		
		{		
			Mx = nDelValue - (Apos[0] + Cw);
		}		
		
		if( (Apos[1] + Ch) + My < nDelValue )				//���ʸ���
		{		
			My = nDelValue - (Apos[1] + Ch);
		}	

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
		
		if( Apos[0] + Mx > (pDT->Width - nDelValue) )			//�����ʸ���
			Mx = (pDT->Width - nDelValue) - Apos[0];							
		
		if( Apos[1] + My > pDT->Height - (800 - 544) )						//�Ʒ��ʸ��� 
			My = pDT->Height - (800 - 544) - Apos[1];			
		
#else
		
		if( nRui->GetResolution() == n_800X600 )
		{
			if( Apos[0] + Mx > (800 - nDelValue) )			//�����ʸ���
				Mx = (800 - nDelValue) - Apos[0];							
			
			if( Apos[1] + My > 544 )						//�Ʒ��ʸ��� 
				My = 544 - Apos[1];			
		}
		else if( nRui->GetResolution() == n_1024X768 )
		{
			if( Apos[0] + Mx > (1024 - nDelValue) )			//�����ʸ���
				Mx = (1024 - nDelValue) - Apos[0];								
			
			if( Apos[1] + My > 712 )						//�Ʒ��ʸ���
				My = 712 - Apos[1] ;
		}
		
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)

		SetWndPositionR( Mx, My );
		if( isTitle && TitleText )
		TitleText->SetWndPositionR( Mx, My );

		TitleBack->SetWndPositionR( Mx, My );
	}

	//by simwoosung
	if( (Pid != WID_None) && !isMove )
	{
		if(Pwnd)
		{
			if( n_wtFrameWnd == Pwnd->GetWindowType() )
			{
				if( ((CFrameWnd *)Pwnd)->GetIsMove() )
				{
					((CFrameWnd *)Pwnd)->InDirectOnMouseLDrag( Mx, My );
				}
			}
		}
	}

}

//-----------------------------------------------------------------------------
void CFrameWnd::OnMouseLDrop( int Mx, int My )
{
}

//-----------------------------------------------------------------------------

void CFrameWnd::SetFocusWndColor( bool aIsFocus )
{
	if( aIsFocus )
	{
		SetClientAlpah( 255 );
		SetClientColor( 255, 255, 255 );
		me_do_alpha = fwfWnd_Alpha_DO_Normal;
	}
	else
	{
		SetClientAlpah( 200 );
		SetClientColor( 200, 200, 200 );
		me_do_alpha = fwfWnd_Alpha_DO_Alpha;
	}

	//By simwoosung
	stChainNode* Node = NULL;
	Node = Chain.GetHead();
	WORD WinType = -1;
		
	while( Node != NULL )
	{
		if( Node->Wnd )
		{
			WinType = Node->Wnd->GetWindowType();
			if(WinType == n_wtFrameWnd)
			{
				((CFrameWnd *)(Node->Wnd))->SetFocusWndColor(aIsFocus);					 
			}	
		}
		
		Node = Chain.GetNext( Node );
	}

	return;
}

///---------------------------------------------------------------------------
///-- SetAbsolutePosition
///---------------------------------------------------------------------------
///-- �ش� Wnd �� ������ǥ ��� ������
void CFrameWnd::SetAbsolutePosition( int PosX, int PosY )
{
  	int Rx = PosX - Cx;
	int Ry = PosY - Cy;

	if(pMoveLinkFrame)
	{
		pMoveLinkFrame->SetWndPositionR( Rx, Ry );
	}	

	if(pMoveLinkFrame)
	{

		pMoveLinkFrame->SetWndPositionR( Rx, Ry) ;
	}
	
	Apos[0] = PosX;
	Apos[1] = PosY;
	
	Cx = Apos[0];
	Cy = Apos[1];
	Cw = Csize[2];
	Ch = Csize[3];

	Wsize[0] = Cx;
	Wsize[1] = Cy;
	Wsize[2] = Cx + Cw;
	Wsize[3] = Cy + Ch;

	//������, Ÿ��Ʋ �ٸ� ������ ��ü ������ ũ��
	/*
	if( isTitle )
	{
		Wsize[0] = Cx;
		Wsize[1] = Cy - 20;
		Wsize[2] = Cx + Cw;
		Wsize[3] = Cy + Ch;

		TitleText->Reposition( PosX, PosY );//Ÿ��Ʋ �� ��ġ
		CloseBtn->Reposition( PosX, PosY );//��ư ��ġ
	}
	*/
	

	stChainNode* Node = NULL;
	Node = Chain.GetHead();

	while( Node != NULL )
	{
		if( Node->Wnd )
		{
			Node->Wnd->Reposition( PosX, PosY );
		}
		Node = Chain.GetNext( Node );
	}
}

void CFrameWnd::InDirectOnMouseLDrag( int Mx, int My )
{
	OnMouseLDrag(Mx, My);
}

void CFrameWnd::SetFrameWndFocusPos(enFrameWndFocusPos	tFocusPos)
{
	FocusPos = tFocusPos;
}



