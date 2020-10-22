///////////////////////////////////////////////////////////////////////////////
///
///		File		: ListBox.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "ListBox.h"


//-----------------------------------------------------------------------------
CListBox::CListBox( SDesktop* DT ) : CTextBox( DT )
{
	LineTextAlign = n_atCenter;

	isScrollBarView = false;
	ScrollBarWidth = 14;
	ScrollBar = SAFE_NEW( CScrollBar( DT ) );	

	ScrbX = 0;
	ScrbY = 0;
	ScrbW = 0;
	ScrbH = 0;

	LBoxX = 0;
	LBoxY = 0;
	LBoxW = 0;
	LBoxH = 0;

	LBoxBlankLeft = 0;
	LBoxBlankRight = 0;
	LBoxBlankTop = 0;
	LBoxBlankBottom = 0;

	LineHeight = 14;

	NumVisibleLine = 3;
	FirstVisibleIdx = 0;
	FocusLine = 0;
	
	VBox = NULL;
	SelectedLine = 0;
	
	InsertOrder = ioLast;
	NumItem = 0;

	Head = NULL;
	Tail = NULL;
}

//-----------------------------------------------------------------------------
CListBox::~CListBox()
{
	ClearItem();
	
	SAFE_DELETE_ARRAY(VBox);	
	SAFE_DELETE(ScrollBar);
}
///---------------------------------------------------------------------------
///-- SetClientColor
///---------------------------------------------------------------------------
void CListBox::SetClientColor( UCHAR Red, UCHAR Green, UCHAR Blue )
{
	ScrollBar->SetClientColor( Red, Green, Blue );
}	
///---------------------------------------------------------------------------
///-- SetClientAlpah
///---------------------------------------------------------------------------
void CListBox::SetClientAlpah( UCHAR Alpha )
{
	ScrollBar->SetClientAlpah( Alpha );
}
///---------------------------------------------------------------------------
///-- EventProc
///---------------------------------------------------------------------------
///-- Scroll bar 부분 Click 시는 체크 제외
SEventMessage* CListBox::EventProc( SEventMessage* EMsg )
{
	if( FocusPos == lbfScrollBarFocus )
	{
		ScrollBar->EventProc( EMsg );	
		ScrollBar->GetValue( &FirstVisibleIdx );
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
		if( FocusPos == lbfClientFocus && SelectedLine != -1 )
		{
			EMsg->Notify = NM_LB_SELECTED;
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

//-----------------------------------------------------------------------------
void CListBox::RenderProc()
{
	UpdateHandler();

	//Render->DrawState();	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	
	
	SetVBoxState();
	DrawVBox();

	if( isScrollBarView )
	{
		ScrollBar->RenderProc();	
	}

}

//-----------------------------------------------------------------------------
IRESULT CListBox::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CTextBox::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emAddItem:
		AddItem( (TCHAR*)fParam );
		break;
	case n_emGetSelectedItem:
		GetSelectedText( (TCHAR*)fParam, sParam );
		break;
	}
	return( Iresult );
}

//-----------------------------------------------------------------------------
WNDID CListBox::FocusCheck( int Mx, int My, bool IsChildCheck )//OR
{
	IsChildCheck;

	if( isFocusCheck )
	{
		//Client BoundingBox 계산
		int Bx = Cx; 
		int By = Cy;
		int Bw = Cw + Cx;
		int Bh = Ch + Cy;

		if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) )
		{
			FocusPos = lbfClientFocus;

			if( isScrollBarView )
			{
				if( ScrollBar->FocusCheck( Mx, My, false ) != WID_None )
				{
					FocusPos = lbfScrollBarFocus;
				}
			}
			
			
			return( Iid );
		}
	}
	
	return( WID_None );
}


///---------------------------------------------------------------------------
///-- AddItem
///---------------------------------------------------------------------------
///-- Value 는 Default 0 의 변수
stTextList* CListBox::AddItem( TCHAR* Str , const int al_value_D )
{
	stTextList*	Node;
	Node = SAFE_NEW( stTextList );	

	Node->Len = Rstrlen( Str );
#ifdef UNICODE_ROCK
	Node->Str = SAFE_NEW_ARRAY( TCHAR , (Node->Len+1)*2 ); // +1 == NULL문자
#else
	Node->Str = SAFE_NEW_ARRAY( TCHAR , Node->Len+1 ); // +1 == NULL문자
#endif 

	Node->value = al_value_D;
	Rstrcpy(RWCHAR(Node->Str), RWCHAR(Str) );
	Node->Next = NULL;
	
	if( Head == NULL )
	{
		Head = Node;
		Tail = Node;
	}
	else
	{
		if( (InsertOrder == ioNone) || (InsertOrder == ioLast) )
		{
			InsertNodeLast( Node );
		}
		else if( InsertOrder == ioFirst )
		{
			InsertNodeFirst( Node );
		}
	}

	NumItem++; //아이템 증가
	///-- 보이는 것이 실제 보다 많다면 스크롤 바 숨김
	if( NumVisibleLine >= NumItem )
	{
		isScrollBarView = false;		
		//ScrollBar->SetScope( 0, 0 );//현재..EditBox생성시 ScrollBar윈도우 생성이 안된 상황에서 값을 넣고 있다...
									//초기화시 다시 계산하므로 문제는 없지만...기분이 나쁘다. (컨트롤들 초기화를 다시 생각해보자.)
	}
	else if( NumVisibleLine < NumItem )
	{
		isScrollBarView = true;		
		ScrollBar->SetScope( 0, (NumItem - NumVisibleLine) );
	}
	
	SetAdjustPosition( 0, 0 );
	SetVBox();

	return Node;
}
///---------------------------------------------------------------------------
///-- GetCurSel
///---------------------------------------------------------------------------
///-- 현재 선택된 Text의  index 와 value 를 가져온다.
bool CListBox::GetSelectedItem( int * const rapl_idx, int * const rapl_Value )
{
	if( FocusPos == lbfScrollBarFocus )
	{
		return false;
	}
	if( SelectedLine - FirstVisibleIdx < 0 && SelectedLine - FirstVisibleIdx > NumVisibleLine )
	{
		return false;
	}

	if( VBox == NULL )
	{
		return false;
	}
	
	int apl_Value = GetValue( VBox[SelectedLine - FirstVisibleIdx].Idx );
	int apl_idx = SelectedLine;
	
	*rapl_Value = apl_Value;
	*rapl_idx = apl_idx;

	return true;
}
///---------------------------------------------------------------------------
///-- GetSelectedItem
///---------------------------------------------------------------------------
///-- 현재 선택된 Text 를 가져온다.
bool CListBox::GetSelectedText( TCHAR* Str, int MaxLen )
{
	if( FocusPos == lbfScrollBarFocus )
	{
		return false;
	}
	if( SelectedLine - FirstVisibleIdx < 0 && SelectedLine - FirstVisibleIdx > NumVisibleLine )
	{
		return false;
	}

	TCHAR* Text = GetItem( VBox[SelectedLine - FirstVisibleIdx].Idx );
	if( Text == NULL ){	return false;	}

	int Len =  Rstrlen( Text );
	if( MaxLen <=  Len )
	{
		return false;
	}
	
	Rstrcpy(RWCHAR(Str), RWCHAR(Text) );
	return true;
}

//-----------------------------------------------------------------------------
enListBoxFocusPos CListBox::GetFocusPos()
{
	return( FocusPos );
}
///---------------------------------------------------------------------------
///-- GetValue
///---------------------------------------------------------------------------
const int CListBox::GetValue( int Idx )
{
	if( Idx == -1 ){	return( NULL );	}
	
	int Count( 0 );
	stTextList* Next = NULL;
	stTextList* Node = GetHead();

	while( Node != NULL )
	{
		if( Idx == Count )
		{
			return( Node->value );
		}

		Next = GetNext( Node );
		Node = Next;
		Count++;
	}
	return( NULL );
}

//-----------------------------------------------------------------------------
TCHAR* CListBox::GetItem( int Idx )
{
	if( Idx == -1 ){	return( NULL );	}
	
	int Count( 0 );
	stTextList* Next = NULL;
	stTextList* Node = GetHead();

	while( Node != NULL )
	{
		if( Idx == Count )
		{
			return( Node->Str );
		}

		Next = GetNext( Node );
		Node = Next;
		Count++;
	}

	return( NULL );
}

//-----------------------------------------------------------------------------
void CListBox::DeleteItem( int Idx )
{
	int Count( 0 );
	stTextList* Prev = NULL;
	stTextList* Next = NULL;
	stTextList* Node = GetHead();


	if( Node == NULL )
	{
		NumItem = 0;
		ScrollBar->SetScope( 0, 0 );

		return;
	}

	while( Node != NULL )
	{
		if( Idx == Count )
		{
			Next = GetNext( Node );
			
			SAFE_DELETE_ARRAY(Node->Str);
			SAFE_DELETE(Node);

			if( Prev )
			{
				Prev->Next = Next;
			}
			else
			{
				Head = Next;
			}

			break;
		}
		
		Next = GetNext( Node );
		Prev = Node;
		Node = Next;
		Count++;
	}

	NumItem--;//아이템 감소
	if( NumVisibleLine >= NumItem )
	{
		ScrollBar->SetScope( 0, 0 );
	}
	else if( NumVisibleLine < NumItem )
	{
		ScrollBar->SetScope( 0, (NumItem - NumVisibleLine) );
	}
}

//-----------------------------------------------------------------------------
void CListBox::ClearItem()
{
	stTextList* Temp = NULL;
	stTextList* Node = GetHead();
	
	while( Node != NULL )
	{
		Temp = GetNext( Node );
		
		SAFE_DELETE_ARRAY( Node->Str );
		SAFE_DELETE(Node);	
	
		Node = Temp;
		NumItem--;
	}
	
	SelectedLine = -1;
}

//-----------------------------------------------------------------------------
void CListBox::SetScrollBar()
{
	isScrollBarView = true;
	//InitScrollBar();
}

//-----------------------------------------------------------------------------
void CListBox::SetScrollBarWidth( int Width )
{
	ScrollBarWidth = Width;
}

//-----------------------------------------------------------------------------
void CListBox::SetInsertOrder( enInsertOrder InsertOrder )
{
	this->InsertOrder = InsertOrder;
}

//-----------------------------------------------------------------------------
void CListBox::SetLBoxBlank( int BlankL, int BlankR, int BlankT, int BlankB )
{
	LBoxBlankLeft	= BlankL;
	LBoxBlankRight	= BlankR;
	LBoxBlankTop	= BlankT;
	LBoxBlankBottom	= BlankB;
}

//-----------------------------------------------------------------------------
void CListBox::SetLineHeight( int LineHeight )
{
	this->LineHeight = LineHeight;
	SetVBox();
}

//-----------------------------------------------------------------------------
void CListBox::SetLineTextAlign( NAlignText LineTextAlign )
{
	this->LineTextAlign = LineTextAlign;
}

//-----------------------------------------------------------------------------
stTextList* CListBox::GetHead()
{
	return( Head );
}

//-----------------------------------------------------------------------------
stTextList* CListBox::GetTail()
{
	return( Tail );
}

//-----------------------------------------------------------------------------
stTextList* CListBox::GetNext( stTextList* Node )
{
	return( Node->Next );
}

//-----------------------------------------------------------------------------
void CListBox::InsertNodeFirst( stTextList* Node )
{
	stTextList* Temp = GetHead();

	Node->Next = Temp;
	Head = Node;
}

//-----------------------------------------------------------------------------
void CListBox::InsertNodeLast( stTextList* Node )
{
	stTextList* Temp = GetTail();

	Node->Next = NULL;
	Temp->Next = Node;
	Tail = Node;
}

//-----------------------------------------------------------------------------
int CListBox::GetLineAlignY( TCHAR* Str, int Ly, int Lh )
{
	int FontHeight( 12 );
	int PosY( 0 );
	float PosBoxCenterY( 0.0f );
	float PosFontCenterY( 0.0f );

	if( (Str == NULL) || (Rstrlen( Str ) == 0 ) || ( FontHeight >= Th ) )
	{	
		PosY = Ly;
	}

	switch( LineTextAlign )
    {
    case n_atTop:
            PosY = Ly;
            break;

    case n_atCenter:
            PosBoxCenterY = (float)( Lh / 2 );
            PosFontCenterY = (float)( FontHeight / 2 );

            PosY = (int)( PosBoxCenterY - PosFontCenterY );
			PosY += Ly;
            break;

    case n_atBottom:
			PosY = Lh - FontHeight;
			PosY += Ly;
            break;
    }

    return( PosY );
}

//-----------------------------------------------------------------------------
void CListBox::SetVBoxState()
{
	for( int i = 0; i < NumVisibleLine; ++i )
	{
		if( NumItem <= FirstVisibleIdx + i )
		{
			VBox[i].Idx = -1;
		}
		else
		{
			VBox[i].Idx = FirstVisibleIdx + i;
		}
		
		VBox[i].Color = D3DCOLOR_ARGB( 0, 255, 255, 255 );
	}

	if( FocusLine != -1 )
	{
		VBox[FocusLine].Color = D3DCOLOR_ARGB( 100, 100, 100, 130 );
	}
	if( SelectedLine != -1 )
	{
		int real_line = SelectedLine - FirstVisibleIdx  ;
		if( real_line >= 0 && real_line < NumVisibleLine - 1)
		{
			VBox[ real_line ].Color = D3DCOLOR_ARGB( 100, 150, 150, 255 );
		}
		
	}
	
	///-- 보이는 것이 실제 보다 많다면 스크롤 바 숨김
	if( NumVisibleLine >= NumItem )
	{
		isScrollBarView = false;			
	}
	else if( NumVisibleLine < NumItem )
	{
		isScrollBarView = true;				
	}
}

//-----------------------------------------------------------------------------
void CListBox::SetVBox()
{
	NumVisibleLine = (int)( LBoxH / LineHeight );

	if( NumVisibleLine == 0 )
	{
		return;
	}

	if( VBox != NULL )
	{
		SAFE_DELETE_ARRAY( VBox );		
	}
	
	VBox = SAFE_NEW_ARRAY( stVisibleTextBox , NumVisibleLine );	

	memset( VBox, 0, sizeof(stVisibleTextBox) * NumVisibleLine );

	for( int i = 0; i < NumVisibleLine; ++i )
	{
		int AccumY = LineHeight * i;
		
		if( NumItem <= FirstVisibleIdx + i )
		{
			VBox[i].Idx = -1;
		}
		else
		{
			VBox[i].Idx = FirstVisibleIdx + i;
		}
		VBox[i].X = LBoxX; 
		VBox[i].Y = LBoxY + AccumY;
		VBox[i].W = LBoxW ;
		VBox[i].H = LineHeight;
		VBox[i].Color = D3DCOLOR_ARGB( 0, 255, 255, 255 );
	}
}

//-----------------------------------------------------------------------------
void CListBox::DrawVBox()
{
	TCHAR* Str = NULL;
	int LAy( 0 );// "     "   Y

	for( int i = 0; i < NumVisibleLine; ++i )
	{
		Str = GetItem( VBox[i].Idx );
		LAy = GetLineAlignY( Str, VBox[i].Y, VBox[i].H );

		Render->DrawAlignText( theFontg, Str, VBox[i].X, LAy, VBox[i].X, VBox[i].Y, VBox[i].W, VBox[i].H, Tcolor );
		Render->Draw( TID_CLIENT, VBox[i].X, VBox[i].Y, VBox[i].W, VBox[i].H, VBox[i].Color );
	}

	//VBox초기화....
	FocusLine = -1;
}

//-----------------------------------------------------------------------------
int CListBox::CheckFocusVBoxLine( int Mx, int My )
{
	int VBoxIdx( -1 );
	
	for( int i = 0; i < NumVisibleLine; ++i )
	{
		int Bx = VBox[i].X;
		int By = VBox[i].Y;
		int Bw = VBox[i].W + VBox[i].X;
		int Bh = VBox[i].H + VBox[i].Y;

		if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) && NumItem > i )
		{
			VBoxIdx = i;
			return( VBoxIdx );
		}
	}
	
	return( VBoxIdx );
}

//-----------------------------------------------------------------------------
void CListBox::InitScrollBar()
{
	ScrollBar->CreateWnd( n_wtScrollBar, Iid, WID_None, ScrbX, ScrbY, ScrbW, ScrbH, false );	
	ScrollBar->InitSlide( stVertSld, 0, 1, 0, 13 );//Type, min, max, value, 썸의 폭

	if( NumVisibleLine >= NumItem )
	{
		ScrollBar->InitSlide( stVertSld, 0, 0, 0, 13 );//Type, min, max, value, 썸의 폭
	}
	else if( NumVisibleLine < NumItem )
	{
		ScrollBar->InitSlide( stVertSld, 0, (NumItem - NumVisibleLine), 0, 13 );//Type, min, max, value, 썸의 폭
	}
		ScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

}

void CListBox::SetPosition(int PosX, int PosY)
{
	SetAdjustPosition( PosX, PosY );	
}

//-----------------------------------------------------------------------------
void CListBox::SetAdjustPosition( int PosX, int PosY )
{
	if( isScrollBarView )
	{
		ScrbX = ( Cx + Cw ) - ScrollBarWidth;//ListBox의 ScrollBar는 폭에 제한을 둔다.
		ScrbY = Cy;
		ScrbW = ScrollBarWidth;
		ScrbH = Ch;
	}
	
	LBoxX = Cx + LBoxBlankLeft;
	LBoxY = Cy + LBoxBlankTop;
	LBoxW = Cw - ( LBoxBlankRight + LBoxBlankLeft + ScrbW );
	LBoxH = Ch - ( LBoxBlankBottom + LBoxBlankTop );

	for( int i = 0; i < NumVisibleLine; ++i )
	{
		int AccumY = LineHeight * i;

		if( NumItem <= FirstVisibleIdx + i )
		{
			VBox[i].Idx = -1;
		}
		else
		{
			VBox[i].Idx = FirstVisibleIdx + i;
		}
		VBox[i].X = LBoxX; 
		VBox[i].Y = LBoxY + AccumY;
		VBox[i].W = LBoxW ;
		VBox[i].H = LineHeight;
		VBox[i].Color = D3DCOLOR_ARGB( 0, 255, 255, 255 );
	}

	ScrollBar->SetWndPositionR( PosX, PosY );
}

//-----------------------------------------------------------------------------
void CListBox::UpdateHandler()
{
	if( !isInit )
	{
		SetAdjustPosition( 0, 0 );
		SetVBox();
		InitScrollBar();		
		isInit = true;
	}
	
}

//-----------------------------------------------------------------------------
void CListBox::OnMouseFocus( int Mx, int My )
{
	int ll_temp_sel;
	if( FocusPos == lbfClientFocus )
	{
		ll_temp_sel = CheckFocusVBoxLine( Mx, My );
		if( ll_temp_sel >= 0)
		{
			FocusLine = ll_temp_sel;
		}
	}
	
	
}

//-----------------------------------------------------------------------------
void CListBox::OnMouseLButtonDown( int Mx, int My )
{
	int ll_temp_sel;
	if( FocusPos == lbfScrollBarFocus )
	{
		ScrollBar->GetValue( &FirstVisibleIdx );
		ll_temp_sel = -1;
	}
	if( FocusPos == lbfClientFocus )
	{
		ll_temp_sel  = CheckFocusVBoxLine( Mx, My );
		
		if( ll_temp_sel >= 0)
		{
			SelectedLine = ll_temp_sel + FirstVisibleIdx;
		}
		
	}

}

/*
//-----------------------------------------------------------------------------
void CListBox::OnMouseLDragStart( int Mx, int My )
{
	if( FocusPos == lbfScrollBarFocus )
	{
		SelectedLine = -1;
		ScrollBar->GetValue( &FirstVisibleIdx );
	}
}
//*/

//-----------------------------------------------------------------------------
void CListBox::OnMouseLDrag( int Mx, int My )
{
	if( FocusPos == lbfScrollBarFocus )
	{
		ScrollBar->GetValue( &FirstVisibleIdx );
	}
}

//-----------------------------------------------------------------------------
void CListBox::OnMouseLDrop( int Mx, int My )
{
	if( FocusPos == lbfScrollBarFocus )
	{
		ScrollBar->GetValue( &FirstVisibleIdx );
	}
}

///---------------------------------------------------------------------------
///-- SetCurSelIdx
///---------------------------------------------------------------------------
bool CListBox::SetCurSelIdx( const int al_idx )
{	
	if( NumItem > al_idx )
	{
		SelectedLine = al_idx;
		return true;
	}
	return false;
}