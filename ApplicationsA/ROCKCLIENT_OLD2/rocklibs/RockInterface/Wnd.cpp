///////////////////////////////////////////////////////////////////////////////
///
///		File		: Wnd.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Wnd.h"
#include "Rui.h"
//#include "RUI_Container.h"


//-----------------------------------------------------------------------------
CWnd::CWnd( SDesktop* DT )
{
	pDT			= DT;

	WType		= n_wtWnd;//������ Ÿ��
	Iid			= WID_None;//�ڽ� ID
	Pid			= WID_None;//�θ� ID 
	Pwnd		= NULL;

	Csize[0]	= 0; //Ŭ���̾�Ʈ ������
	Csize[1]	= 0;
	Csize[2]	= 0;
	Csize[3]	= 0;

	Cimg		= TID_CLIENT;  //�̹���
	Cstyle		= csNone;
	Cborder		= cbNone;

	Cx = 0;
	Cy = 0;
	Cw = 0;
	Ch = 0;
	
	Rpos[0]		= 0; //�θ������쿡 ���� ��� ��ġ(relative)
	Rpos[1]		= 0;
	Apos[0]		= 0; //���������� ���� ����� ���� ��ġ(absolute)
	Apos[1]		= 0;
	
	isVisible	= true;//���� ����
	isFocusCheck= true;//��Ŀ�� üũ ����
	isRegion	= false;//��� ����( Hit Test�� )
	isInit		= false;
	isFitImg	= false;
	
	Zorder		= n_zoNormal;

	Rcolor[A]	= 255;
	Rcolor[R]	= 255;
	Rcolor[G]	= 255;
	Rcolor[B]	= 255;
	InitMaxAlpha( 255 );

	Ccolor = D3DCOLOR_ARGB( Rcolor[A], Rcolor[R], Rcolor[G], Rcolor[B] );
	
	Hid			= HID_None; // Help ID
	
	PrevNotify	= NM_NONE;
	isEnable = true;	
	CExtraimg = TID_None;
	CBgImage = TID_None;

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
	
	m_fExtentW = 0.0f;
	m_fExtentH = 0.0f;
	m_ucCoordStd = 0;
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
}

//-----------------------------------------------------------------------------
CWnd::~CWnd()
{
}

//-----------------------------------------------------------------------------
void CWnd::CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float PosX, float PosY, int Width, int Height, bool isRatio )
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

	Rpos[0]	= PosX; //�θ������쿡 ���� ��� ��ġ(relative)
	Rpos[1]	= PosY;
	Apos[0]	= 0; //���������� ���� ����� ���� ��ġ(absolute)
	Apos[1]	= 0;

	if( Pid == WID_None )//�θ� ������
	{
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
void CWnd::SetRatio( POINT* ratio, float x, float y )
{
	// ����� ������ �����쿡 ���ؼ���...
	ratio->x = (int)( ( pDT->Width * x ) / 100 );
	ratio->y = (int)( ( pDT->Height * y ) / 100 );
}

//-----------------------------------------------------------------------------
void CWnd::RenderProc()
{
	Render->DrawState();

	if( isFitImg )
	{
		Render->DrawFitImg( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	}
	else
	{
		Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	}
}

//-----------------------------------------------------------------------------
SEventMessage* CWnd::EventProc( SEventMessage* EMsg )
{
	return( EMsg );
}

//-----------------------------------------------------------------------------
IRESULT CWnd::SendEvent( WNDID Wid, NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	CWnd* Wnd = NULL;

	if( (Wid == WID_DeskTop) || (Wid == WID_None) )
	{
		return( nRui->ReceiveEvent( Event, fParam, sParam, xParam, exParam ) );
	}

	Wnd = Container->Get( Wid );
	return( Wnd->ReceiveEvent( Event, fParam, sParam, xParam, exParam ) );
}

//-----------------------------------------------------------------------------
IRESULT CWnd::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	xParam;
	exParam;

	int Iresult( 0 );

	switch( Event )
	{
	case n_emSetVisible:
		SetVisible( (bool)fParam );
		break;
	case n_emSetClientImage:
		SetClientImage( (TEXID)fParam );
		break;
	case n_emSetClientColor:
		SetClientColor( (UCHAR)fParam, (UCHAR)sParam, (UCHAR)xParam );
		break;
	case n_emSetGroupWnd:
		SetGroupWnd( (int)fParam, (WNDID*)sParam );
		break;
	case n_emSetSelectedWnd:
		SetSelectedWnd();
		break;
	}

	return( Iresult );
}

//-----------------------------------------------------------------------------
WNDID CWnd::FocusCheck( int Mx, int My, bool IsChildCheck )
{
	IsChildCheck;

	if( isFocusCheck )
	{
		int Bx = Cx; //BoundingBox
		int By = Cy;
		int Bw = Cx + Cw;
		int Bh = Cy + Ch;

		if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) )
		{
			if( isRegion )
			{
				if( Cimg == TID_None ){	return( Iid );	}

				int Lx = Mx - Bx;//Apos[0];
				int Ly = My - By;//Apos[1];
				return RegionFocusCheck( Lx, Ly );
			}
			else
			{
				return( Iid );
			}
		}
	}
	
	return( WID_None );
}


///---------------------------------------------------------------------------
///-- GetWndID
///---------------------------------------------------------------------------
///-- WID_Node ���� ������ Wnd �� ��� �ڵ����� WID �� �����Ѵ�.
///-- ���� Define WID �� �浹 ���� Assert
WNDID CWnd::GetWndID()
{
	static WNDID id( MAX_WND_AMOUNT );
	--id;
	assert( id > WID_AUTO_LIMIT );	
	return( id );
}

//-----------------------------------------------------------------------------
WNDID CWnd::RegionFocusCheck( int Lx, int Ly )
{
	STexture* Tex = Resource->GetTexture( Cimg );
	if( Tex == NULL )
	{
		return( WID_None );	
	}
	
	if( Tex->pTexture == NULL )
	{
		return ( WID_None );
	}
	int Tx = Lx + Tex->x;
	int Ty = Ly + Tex->y;
	
	D3DLOCKED_RECT Rect;
	memset( &Rect, 0, sizeof(D3DLOCKED_RECT) );

	
	if( FAILED( Tex->pTexture->LockRect( 0, &Rect, 0, 0 ) ) )
	{
		return( WID_None );
	}
	
//	char str[40];
	

	WNDID Wid;
	
	//�ؽ����� ǥ���� �����͸� �޾ƿͼ�....Tx, Ty ��ġ�� Į����...Į��Ű���� ��....
	BYTE *pBuf = (BYTE*)Rect.pBits;
	int P = ( Tx * 4 ) + ( Ty * Rect.Pitch );

	//by simwoosung - �Ϻκ��� �ؽ��İ� TGA���� DDS���Ϸ� �ٲ�� �ٶ���
	//�ش� ��ƾ�� �׷��� �������� ������ �ʿ�..;;
	//������ �ð��� ���;;;;;

//	sprintf( str, "%d %d : %d %d %d %d\n", Tx, Ty, pBuf[P], pBuf[P+1], pBuf[P+2], pBuf[P+3] );
//	OutputDebugString( str );

	//BGRA???
	//if( (255 == pBuf[P]) && (255 == pBuf[P+1]) && (255 == pBuf[P+2]) &&
	  //(0 == pBuf[P+3])  )	
	//if( (255 == pBuf[P]) && (0 == pBuf[P+1]) && (255 == pBuf[P+2])  )
	/*if( (0 == pBuf[P+3])  )
	{
		Wid = WID_None;//Į��Ű�̸� �����	
	}
	else
	{*/
		Wid = Iid;
	//}

	Tex->pTexture->UnlockRect( 0 );
	return( Wid );
}

//-----------------------------------------------------------------------------
NWindowType CWnd::GetWindowType()
{
	return( WType );
}

//-----------------------------------------------------------------------------
void CWnd::SetID( WNDID Iid )
{
	this->Iid = Iid;
}

//-----------------------------------------------------------------------------
WNDID CWnd::GetID()
{
	return( Iid );
}

//-----------------------------------------------------------------------------
WNDID CWnd::GetParentID()
{
	return( Pid );
}

//-----------------------------------------------------------------------------
CWnd* CWnd::GetParentHandle()
{
	return( Container->Get( Pid ) );
}

//-----------------------------------------------------------------------------
void CWnd::SetHID( HLPID Hid )
{
	this->Hid = Hid;
}

//-----------------------------------------------------------------------------
HLPID CWnd::GetHID()
{
	return( Hid );
}


//-----------------------------------------------------------------------------
void CWnd::GetClientSize( SRect* Csize )
{
	/*
	Csize->x = this->Csize[0];
	Csize->y = this->Csize[1];
	Csize->w = this->Csize[2];
	Csize->h = this->Csize[3];
	//*/

	Csize->x = Cx;
	Csize->y = Cy;
	Csize->w = this->Csize[2];
	Csize->h = this->Csize[3];
}

///---------------------------------------------------------------------------
///-- SetClientSize
///---------------------------------------------------------------------------
///-- ���� Wnd ��  Size �� ���� �����Ѵ�.
///-- ����� fwfTex_Style_Tile9 �� ����Ѵ�.
void CWnd::SetClientSize( int Width, int Height )
{
	Csize[2] = Width;
	Csize[3] = Height;

	Cw = Csize[2];
	Ch = Csize[3];	
	
	SetAdjustPosition( Cx, Cy );
}

//-----------------------------------------------------------------------------
void CWnd::ResetSize( SRect* size )
{
	Cx = size->x;
	Cy = size->y;
	Cw = size->w;
	Ch = size->h;

	Apos[0]		= Cx;
	Apos[1]		= Cy;
	Csize[2]	= Cw;
	Csize[3]	= Ch;
}

//-----------------------------------------------------------------------------
TEXID CWnd::GetClientImage()
{
	return( Cimg );
}

//-----------------------------------------------------------------------------
void CWnd::SetClientImage( TEXID Cimg )
{
	this->Cimg = Cimg;
}

//-----------------------------------------------------------------------------
enClientStyle CWnd::GetClientStyle()
{
	return( Cstyle );
}

//-----------------------------------------------------------------------------
enClientBorder CWnd::GetClientBorder()
{
	return( Cborder );
}

//-----------------------------------------------------------------------------
void CWnd::GetClientStyle( enClientStyle Cstyle )
{
	this->Cstyle = Cstyle;
}

//-----------------------------------------------------------------------------
void CWnd::GetClientBorder( enClientBorder Cborder )
{
	this->Cborder = Cborder;
}

//-----------------------------------------------------------------------------
void CWnd::SetWndPositionR( int PosX, int PosY )
{
	if(Pwnd != NULL)
	{
        SRect rect;
		Pwnd->GetClientSize(&rect);	
		
		Apos[0] = rect.x + Rpos[0];
		Apos[1] = rect.y + Rpos[1];
	}
	else
	{
		Apos[0] += PosX;
		Apos[1] += PosY;
	}
		
	Cx = Apos[0];
	Cy = Apos[1];
	Cw = Csize[2];
	Ch = Csize[3];
	
	SetAdjustPosition( PosX, PosY );
}

void CWnd::SetWndRepositionR( int PosX, int PosY )
{
	Rpos[0] = PosX;
	Rpos[1] = PosY;

	Pwnd = GetParentHandle();

	if( Pid == WID_None )//�θ� ������
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
	
	SetAdjustPosition( PosX, PosY );
}

//-----------------------------------------------------------------------------
void CWnd::SetWndPositionA( int PosX, int PosY )
{
	Apos[0] = PosX;
	Apos[1] = PosY;

	Cx = Apos[0];
	Cy = Apos[1];
	Cw = Csize[2];
	Ch = Csize[3];
	
	SetAdjustPosition( Cx, Cy );
}

//-----------------------------------------------------------------------------
void CWnd::Reposition( int x, int y )
{
	if( Pwnd != NULL )
	{
		Cx = x + Rpos[0];
		Cy = y + Rpos[1];
		SetAdjustPosition( x, y );//*/
	}
}

//-----------------------------------------------------------------------------
void CWnd::SetAdjustPosition( int PosX, int PosY )
{
	PosX, PosY;
}

//-----------------------------------------------------------------------------
void CWnd::GetWndPosition( IPOS2 Pos )
{
	Pos[0] = Rpos[0];
	Pos[1] = Rpos[1];
}

//-----------------------------------------------------------------------------
void CWnd::GetScenePos( IPOS2 Pos )
{
	Pos[0] = Apos[0];
	Pos[1] = Apos[1];
}

//-----------------------------------------------------------------------------
bool CWnd::IsVisible()
{
	return( isVisible );
}

//-----------------------------------------------------------------------------
bool CWnd::IsFocusCheck()
{
	return( isFocusCheck );
}

//-----------------------------------------------------------------------------
bool CWnd::IsRegion()
{
	return( isRegion );
}

//-----------------------------------------------------------------------------
void CWnd::SetVisible( bool Visible )
{	
	isVisible = Visible;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void CWnd::SetFocusCheck( bool FocusCheck )
{
	isFocusCheck = FocusCheck;
}

//-----------------------------------------------------------------------------
void CWnd::SetRegion( bool Region )
{
	isRegion = Region;
}

void CWnd::SetFitImg( bool FitImg )
{
	isFitImg = FitImg;
}

void CWnd::SetEnable( bool Enable )
{
	isEnable = Enable;
}
void CWnd::SetClientBgImage( TEXID Cimg )
{
	CBgImage = Cimg;
}

void CWnd::SetClientExtraImage( TEXID Cimg )
{
	CExtraimg = Cimg;
}

//-----------------------------------------------------------------------------
NDepthOrder CWnd::GetZorder()
{
	return( Zorder );
}

bool CWnd::GetEnable()
{
	return isEnable;
}


//-----------------------------------------------------------------------------
void CWnd::SetZorder( NDepthOrder Zorder )
{
	this->Zorder = Zorder;
}

//-----------------------------------------------------------------------------
void CWnd::InitMaxAlpha( UCHAR Alpha )
{
	MaxAlpha = Alpha;
	if( MaxAlpha < Rcolor[A] )
	{
		Rcolor[A] = MaxAlpha;
	}
	Ccolor = D3DCOLOR_ARGB( Rcolor[A], Rcolor[R], Rcolor[G], Rcolor[B] );
}

//-----------------------------------------------------------------------------
void CWnd::SetClientAlpah( UCHAR Alpha )
{
	Rcolor[A] = Alpha;
	if( MaxAlpha < Rcolor[A] )
	{
		Rcolor[A] = MaxAlpha;
	}
	Ccolor = D3DCOLOR_ARGB( Rcolor[A], Rcolor[R], Rcolor[G], Rcolor[B] );
}

//-----------------------------------------------------------------------------
UCHAR CWnd::GetClientAlpah()
{
	return( Rcolor[0] );
}

//-----------------------------------------------------------------------------
void CWnd::SetClientColor( UCHAR Red, UCHAR Green, UCHAR Blue )
{
	Rcolor[R] = Red;
	Rcolor[G] = Green;
	Rcolor[B] = Blue;
	Ccolor = D3DCOLOR_ARGB( Rcolor[A], Rcolor[R], Rcolor[G], Rcolor[B] );
}

//-----------------------------------------------------------------------------
void CWnd::SetGroupWnd( int Count, WNDID* WndList )
{
	Count;
	WndList;
}

//-----------------------------------------------------------------------------
void CWnd::SetSelectedWnd()
{
}

//-----------------------------------------------------------------------------
void CWnd::UpdateHandler()
{
	if( !isInit )
	{
		isInit = true;
	}
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseFocus( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseLButtonDown( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseLButtonPress( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseLButtonUp( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseRButtonDown( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseRButtonPress( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseRButtonUp( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseLDrag( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseLDrop( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseRDrag( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseRDrop( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseLDblClick( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CWnd::OnMouseRDblClick( int Mx, int My )
{
}

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
// ���� ������ ������ 1024 * 768���� Ȯ��� ������. ������������ ȣ��Ǿ� �Ǵ� �Լ�
void CWnd::SetExtentWH(float fW, float fH)
{
	if(WType != n_wtFrameWnd)
	{
		assert(false);
		return;
	}
	
	m_fExtentW = fW * nRui->m_fResolutionRateW;// �̵� ��
	m_fExtentH = fH * nRui->m_fResolutionRateH;// �̵� ����
}

#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)

//-----------------------------------------------------------------------------