///////////////////////////////////////////////////////////////////////////////
///
///		File		: EditBox.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "..\\RockPCH.h"
#include "Rui.h"
#include "EditBox.h"
#include "CEditSlashProc.h"

#include "..\\..\\StringManager.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\CRenderManager.h"

#include "..\\..\\CIMEddk.h"
#include "CImm.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

//-----------------------------------------------------------------------------
// Static
//-----------------------------------------------------------------------------
CEditBox* CEditBox::AgentEditBox = NULL;


//-----------------------------------------------------------------------------
CEditBox::CEditBox( SDesktop* DT ) : CTextBox( DT )
{

	
#ifdef UNICODE_ROCK
	EditText	= SAFE_NEW_ARRAY( TCHAR , (TextLimitLen+1)*2 );
	PwText		= SAFE_NEW_ARRAY( TCHAR , (TextLimitLen+1)*2 );
	
	m_AllocEditTextLen = (TextLimitLen+1)*2;
	m_AllocPwTextLen = (TextLimitLen+1)*2;
#else
	EditText	= SAFE_NEW_ARRAY( TCHAR , TextLimitLen+1 );
	PwText		= SAFE_NEW_ARRAY( TCHAR , TextLimitLen+1 );
	
	m_AllocEditTextLen = TextLimitLen+1;
	m_AllocPwTextLen = TextLimitLen+1;
#endif
	
	SAFE_STR_CPY( EditText, _RT(""), m_AllocEditTextLen );
	SAFE_STR_CPY( PwText, _RT(""), m_AllocPwTextLen );

	EditStyle			= esText;
	isAutoScroll		= true;
	isReturnActionClear = false;
	isAlphaNumeric		= true; //����� ����....
	isNumberComma		= false;  

	Caret.Use = false;
	Caret.X = 0;
	Caret.Y = 0;
	Caret.W = StrInfo.Width;
	Caret.H = StrInfo.Height;
	Caret.SelStart = -1;
	Caret.SelEnd = -1;
	Caret.isBlock = false;
	Caret.BlockX = 0;
	Caret.BlockW = 0;
	Caret.FchVisibleIdx = 0;
	Caret.isDirectionMove = false;
	Caret.Img = TID_CLIENT;
	Caret.Color = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	FrameWnd = NULL;
	
	//by simwoosung
	BCheckSlashCommand = FALSE;
	hEdit              = NULL;
	
	Con = 0;
}

//-----------------------------------------------------------------------------
CEditBox::~CEditBox()
{
	
	DeleteObject( hFont );
	//ImmReleaseContext( hEdit, hImc );
	
	// SetWindowLong( hEdit, GWL_WNDPROC, (LONG)OldEditProc );
	
	if( Text != NULL ) 
	{
		delete[] Text;	
		
		Text = NULL;
	}

	if( EditText != NULL ) 
	{
		delete[] EditText;	
		
		EditText = NULL;
	}

	if( PwText != NULL   ) 
	{	
		delete[] PwText;	
		
		PwText = NULL; 	
	}

}

//-----------------------------------------------------------------------------
SEventMessage* CEditBox::EventProc( SEventMessage* EMsg )
{
	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;

	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
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
		break;
	}

	return( EMsg );
}

//-----------------------------------------------------------------------------
void CEditBox::RenderProc()
{
	UpdateHandler();

	//Render->DrawState();	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );

	//Render->SetScissorTest( Tx, Ty, Tw, Th );

	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
	
	DrawText();
	DrawCaret();//ĳ�� ������ ó�� �ʿ�


	//Render->SetScissorTest();
}

//-----------------------------------------------------------------------------
void CEditBox::DrawText()
{
	
	if( TextLength == 0 ){	return;	}

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	int SelStart( 0 );
	int SelEnd( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;

	NumStr = theFontg->Confirm( EditText, IdxBuf );
	
	GetCaretIdx( &SelStart, &SelEnd );
	if( SelStart != SelEnd )
	{
		Caret.isBlock = true;
	}
	else
	{
		Caret.isBlock = false;
	}
	
	int Count( 0 );
	int i( 0 );
		
	for( i = 0; i < NumStr; ++i  )
	{
		if( Caret.FchVisibleIdx <= Count )
        {
			RFont = theFontg->GetFontasm( IdxBuf[i] );
			
			if( RFont != NULL )
			{
				RFont->vt[0].diffuse	= Tcolor;
				RFont->vt[1].diffuse	= Tcolor;
				RFont->vt[2].diffuse	= Tcolor;
				RFont->vt[3].diffuse	= Tcolor;

				RFont->vt[0].x = (float)Ax - 0.5f;
				RFont->vt[0].y = (float)Ay - 0.5f;
				RFont->vt[1].x = RFont->vt[0].x + RFont->w;
				RFont->vt[1].y = RFont->vt[0].y;
				RFont->vt[2].x = RFont->vt[0].x + RFont->w;
				RFont->vt[2].y = RFont->vt[0].y + RFont->h;
				RFont->vt[3].x = RFont->vt[0].x;
				RFont->vt[3].y = RFont->vt[0].y + RFont->h;

				Ax = Ax + RFont->w;
			
				//if( SelStart < 0 )
				if( SelStart < 0 )
				{
					Caret.BlockX = Tx;
					SetCaretSelBlock( Tx, Ay, 1, RFont->h, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				}
				if( i == SelStart )
				{
					Caret.BlockX = Ax;
					
				}
				if( i == SelEnd )
				{
					if( Ax < ( Tx + Tw ) )
					{
						Caret.BlockW = Ax;
					}
					else
					{
						Caret.BlockW = ( Tx + Tw );
					}
				
					///-- IdxBuf �� 2 Byte 1Byte �� ��� Ŀ���� ��ĭ
					///-- IME
#ifdef EDITBOX_COMMA
					if( isAlphaNumeric && isNumberComma  )
					{
						SetCaretSelBlock( Ax , Ay, 1 , RFont->h, D3DCOLOR_ARGB( 255, 255, 255, 255 ));
					}
					else
#endif
					if( isAlphaNumeric ||  unsigned char( IdxBuf[i] ) < 0x0100 )
					{
						SetCaretSelBlock( Ax, Ay, 1, RFont->h, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					}
					else
					{
						SetCaretSelBlock( (Ax - RFont->w), Ay, RFont->w, RFont->h, D3DCOLOR_ARGB( 128, 205, 200, 167 ) );
					}
				}

				if( ( IdxBuf[i] != 32 ) && ( Ax < ( Tx + Tw ) ) )
				{
					//" " ������ ���� �ʴ´�.
					//"\n"�� �״�� ��´�( Line ����̱� ������ ������ ó������ �ʴ´�. )
					// d3d8������ �����׽�Ʈ�� ���� ������...��輱�� �Ѿ��..������� �ʴ´�.
					Render->DrawFont( RFont );
				}
			}
		}

		Count++;
	}

	//selũ�Ⱑ ���ٸ� ĳ�� ��� �ƴϸ� �� ���
	if( SelStart != SelEnd )
	{
		SetCaretSelBlock( Caret.BlockX, Ay, (Caret.BlockW - Caret.BlockX), RFont->h, D3DCOLOR_ARGB( 128, 205, 200, 167 ) );
	}
}

//-----------------------------------------------------------------------------
void CEditBox::DrawCaret()
{
	static bool Switching( true );

	if( Caret.Use == false )
	{	
		return;	
	}

	//if( TextLength == 0 )
	if( TextLength == 0 )
	{	
		SetCaretSelBlock( Tx, Ay, StrInfo.Width, StrInfo.Height, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	
	STexture CaretTex;
	CaretTex.vt[0].diffuse	= Caret.Color;
	CaretTex.vt[1].diffuse	= Caret.Color;
	CaretTex.vt[2].diffuse	= Caret.Color;
	CaretTex.vt[3].diffuse	= Caret.Color;

	CaretTex.vt[0].x = (float)Caret.X - 0.5f;
	CaretTex.vt[0].y = (float)Caret.Y - 0.5f;
	CaretTex.vt[1].x = CaretTex.vt[0].x + Caret.W;
	CaretTex.vt[1].y = CaretTex.vt[0].y;
	CaretTex.vt[2].x = CaretTex.vt[0].x + Caret.W;
	CaretTex.vt[2].y = CaretTex.vt[0].y + Caret.H;
	CaretTex.vt[3].x = CaretTex.vt[0].x;
	CaretTex.vt[3].y = CaretTex.vt[0].y + Caret.H;

	if( Caret.isBlock )
	{
		Render->Draw( Caret.Img, Caret.X, Caret.Y, Caret.W, Caret.H, Caret.Color );
	}
	else
	{
		//ĳ���� ����尡 �ƴ϶��....����....
		if( pDT->Tflag.One_2 )
		{	
			Switching = !Switching;	
		}

		if( Switching )
		{
			Render->Draw( Caret.Img, Caret.X, Caret.Y, Caret.W, Caret.H, Caret.Color );
		}
	}
}

//-----------------------------------------------------------------------------
void CEditBox::GetCaretIdx( int* SelStart, int* SelEnd )
{
#if !defined UNICODE_ROCK
	int Count( 0 );
	int Len( 0 );
	
	Len = strlen( Text );

	for( int i = 0; i < Len; ++i )
	{
		theFontg->DetectionDBCS( &i, Text );
		
		if( Caret.SelStart == i )
		{
			*SelStart = Count;	
		}
		if( Caret.SelEnd == i )
		{
			*SelEnd = Count;	
		}

		Count++;
	}

	if( Caret.SelStart == -1 ){	*SelStart = -1;	}
	if( Caret.SelEnd == -1 ){	*SelEnd = -1;	}
#else

	*SelStart = Caret.SelStart;
	*SelEnd = Caret.SelEnd;
#endif
}

//-----------------------------------------------------------------------------
void CEditBox::GetRealCaretIdx( int* SelStart, int* SelEnd )
{
#if !defined UNICODE_ROCK
	int Count( 0 );
	int Len( 0 );
	
	Len = strlen( Text );

	for( int i = 0; i < Len;  )
	{
		theFontg->DetectionDBCS( &i, Text );
		i++;

		Count++;
		
		if( Caret.SelStart == i )
		{
			*SelStart = Count;	
		}
		if( Caret.SelEnd == i )
		{
			*SelEnd = Count;	
		}	
	}

	if( Caret.SelStart == -1 ){	*SelStart = -1;	}
	if( Caret.SelEnd == -1 ){	*SelEnd = -1;	}
#else

	*SelStart = Caret.SelStart;
	*SelEnd = Caret.SelEnd;
#endif
}

//-----------------------------------------------------------------------------
void CEditBox::SetEditFocus()
{
	g_RockClient.HS_PauseService();

	for( int i = 0; i < FrameWnd->NumEdit; ++i )
	{
		if( FrameWnd->EditList[i] != Iid )
		{
			SendEvent( FrameWnd->EditList[i], n_emEditDisable, 0, 0, 0, 0 );
		}
	}

	if( pDT->FocusEditWnd != WID_None )
	{
		nRui->SendEvent( pDT->FocusEditWnd, n_emEditDisable, 0, 0, 0, 0 );
	}

	FrameWnd->CurrentEditNum = TabNum;
	pDT->hCurWnd = hEdit;
	pDT->FocusEditWnd = Iid;

#ifdef SHUTDOWN_TEST
	if(GetLastErorrCode()) RMessageBox(NULL, _RT("EditBox 01") , _RT("Rocksoft") , MB_OK ) ;
#endif	

	SetFocus(hEdit);

	Caret.Use = true;
	BOOL IsNativeMode = FALSE;

	if( (Con & IME_CMODE_NATIVE) && EditStyle != esOnlyAlphaNumeric && EditStyle != esOnlyNumeric )
	{
		IsNativeMode = TRUE;
	}

#ifdef ONLY_ENGLISH_SET

	IsNativeMode = FALSE;

#endif

	if( IsNativeMode ) // �ѱ۸���...
	{
		if( hImc = ImmGetContext( hEdit )  )
		{
			//IME �ѱ۷� ��ȯ			
		    ImmSetConversionStatus( hImc, Con, Sen);	

			///-- Japan IME
			if( g_RockClient.GetLanguage() == eLang_Japan ) 
			{				
				///-- �������ڸ�� | �������ڸ�� ���� ��Ÿ������ ���� | ROMAN input mode Sentence ��� - IME�� �ڵ����� ��ȯ�� �����Ѵ�
				ImmSetConversionStatus( hImc, Con | IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE & ~IME_CMODE_KATAKANA | IME_CMODE_ROMAN, IME_SMODE_AUTOMATIC );							
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				ImmSetConversionStatus( hImc, Con | IME_CMODE_NATIVE /*| IME_CMODE_FULLSHAPE*/ | IME_CMODE_ROMAN, IME_SMODE_AUTOMATIC );
			}
			else
			{
				ImmSetConversionStatus( hImc, Con | IME_CMODE_NATIVE, IME_SMODE_NONE );							
			}
		    
			///-- IME�� ����.
			ImmSetOpenStatus( hImc, true );
		    ImmReleaseContext(hEdit , hImc );
		}
	}
	else
	{
		if( hImc = ImmGetContext( hEdit )  )
		{
		    //���� ��ȯ
			ImmSetOpenStatus( hImc, false );

			if( g_RockClient.GetLanguage() == eLang_Japan ) 
			{				
				///-- �Ϻ���� ������� ��ȯ�� ���ϳ�... �Ѥ�a;
				ImmSetConversionStatus( hImc, Con | IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE & ~IME_CMODE_KATAKANA | IME_CMODE_ROMAN, IME_SMODE_AUTOMATIC );							
			}
			else
			{
				ImmSetConversionStatus( hImc, IME_CMODE_ALPHANUMERIC , IME_SMODE_NONE );			
			}

			ImmReleaseContext(hEdit , hImc );
		}
	}


#ifdef SHUTDOWN_TEST
	ClientFileLogW(L"SetEditFocus() 09");
#endif

}

//-----------------------------------------------------------------------------
void CEditBox::EditDisable()
{
	/*hImc = ImmGetContext( hEdit );	
	ImmGetConversionStatus( hImc, &Con, &Sen);
	ImmReleaseContext(hEdit , hImc );*/
	
	if( pDT->hCurWnd != pDT->hWnd )
	{
		g_RockClient.HS_ResumeService();
		pDT->hCurWnd = pDT->hWnd;
		SetFocus( pDT->hWnd ); //edit focus return;		
	}

	Caret.Use = false;
	
	if( pDT->FocusEditWnd == Iid )
	{
		pDT->FocusEditWnd = WID_None;
	}
}

//-----------------------------------------------------------------------------
IRESULT CEditBox::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CTextBox::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emSetFocus:
		SetEditFocus();
		break;

	case n_emClearText:
		ClearText();
		break;

	case n_emEditDisable:
		EditDisable();
		break;
	}
	
	return( Iresult );
}

//-----------------------------------------------------------------------------
void CEditBox::SetAdjustPosition( int PosX, int PosY )//OR
{
	PosX; PosY;

	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );
}

//-----------------------------------------------------------------------------
void CEditBox::InitEditBox()
{
	if( Pid != WID_None )
	{
		FrameWnd = (CFrameWnd*)Pwnd;
		for( int i = 0; i < 10; ++i )
		{
			if( FrameWnd->EditList[i] == WID_None )
			{
				FrameWnd->EditList[i] = Iid;
				FrameWnd->NumEdit++;
				TabNum = i;
				break;
			}
		}
		
		CreateWindowEditBox();
	}
}

//-----------------------------------------------------------------------------
void CEditBox::SetEditStyle( enEditStyle EditStyle )
{

#ifdef ONLY_ENGLISH_SET
	if( EditStyle == esText)
	{
		EditStyle = esOnlyAlphaNumeric;
	}
#endif

	this->EditStyle = EditStyle;
}



void CEditBox::ChangeEditStyle( enEditStyle aEditStyle )
{
	if( hEdit == NULL )
		return;

#ifdef UNICODE_ROCK

	
	
#ifdef ONLY_ENGLISH_SET
	if( aEditStyle == esText)
	{
		aEditStyle = esOnlyAlphaNumeric;
	}
#endif
	
	if( aEditStyle != esPassword && EditStyle == esPassword )
	{
		SendMessageW( hEdit, EM_SETPASSWORDCHAR, 0, 0 );						// PassWord Style Remove 
	}
	
	if( aEditStyle != esOnlyNumeric && EditStyle == esOnlyNumeric )
	{
		//		SetWindowLong( hEdit, GWL_STYLE, 0 );			// Only Digits Style Remove 
		SetWindowLongW( hEdit, ES_NUMBER, 0 );				
	}
	
	switch( aEditStyle )
	{
	case esPassword:			
		{
			SendMessageW( hEdit, EM_SETPASSWORDCHAR, 1, 0 );					// Set PassWord Style 
		}
		break;	
	case esText:
		{
		}
		break;	
	case esOnlyNumeric:	
		{
			//			SetWindowLong( hEdit, GWL_STYLE, 1 );							// Only Digits Style Remove 
			SetWindowLongW( hEdit, ES_NUMBER, 1 );				
		}
		break;
	default:
		break;
	}	

#else 

	
	
#ifdef ONLY_ENGLISH_SET
	if( aEditStyle == esText)
	{
		aEditStyle = esOnlyAlphaNumeric;
	}
#endif
	
	if( aEditStyle != esPassword && EditStyle == esPassword )
	{
		SendMessage( hEdit, EM_SETPASSWORDCHAR, 0, 0 );						// PassWord Style Remove 
	}
	
	if( aEditStyle != esOnlyNumeric && EditStyle == esOnlyNumeric )
	{
		//		SetWindowLong( hEdit, GWL_STYLE, 0 );			// Only Digits Style Remove 
		SetWindowLong( hEdit, ES_NUMBER, 0 );				
	}
	
	switch( aEditStyle )
	{
	case esPassword:			
		{
			SendMessage( hEdit, EM_SETPASSWORDCHAR, 1, 0 );					// Set PassWord Style 
		}
		break;	
	case esText:
		{
		}
		break;	
	case esOnlyNumeric:	
		{
			//			SetWindowLong( hEdit, GWL_STYLE, 1 );							// Only Digits Style Remove 
			SetWindowLong( hEdit, ES_NUMBER, 1 );				
		}
		break;
	default:
		break;
	}	


#endif 
	 

	this->EditStyle = aEditStyle;
}


//-----------------------------------------------------------------------------
void CEditBox::SetAutoScroll( bool bAutoScroll )
{
	isAutoScroll = bAutoScroll;
}

//-----------------------------------------------------------------------------
void CEditBox::SetReturnActionClear( bool bReturnActionClear )
{
	isReturnActionClear = bReturnActionClear;
}

//-----------------------------------------------------------------------------
void CEditBox::SetText( void* pStr )
{
	char* Str = (char *)pStr;
	
	TextLength = Rstrlen( Str );
	

	if( TextLimitLen < TextLength )
	{
		return;
	}

	SAFE_STR_CPY( Text, Str, m_AllocTextLen );
	SAFE_STR_CPY( EditText, Str, m_AllocEditTextLen );

	if( NULL != hEdit )
#ifdef UNICODE_ROCK
		SetWindowTextW( hEdit, RWCHAR(Text) );
#else
		SetWindowText( hEdit, Text );
#endif
	

	int len = Rstrlen( Text );

	if( NULL != hEdit )
#ifdef UNICODE_ROCK
		SendMessageW( hEdit , EM_SETSEL , (WPARAM)&len , (LPARAM)&len );
#else 
		SendMessage( hEdit , EM_SETSEL , (WPARAM)&len , (LPARAM)&len );
#endif
	

	theFontg->GetStringInfo( Text, &StrInfo );
	//SendMessage( hEdit, EM_LIMITTEXT, (WPARAM)TextLimitLen, 0 );//����Ʈ ���� 32k
	
	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );

	Ax = GetAlignPosX();
	Ay = GetAlignPosY();

	GetWindowEditCaretInfo();
}

//-----------------------------------------------------------------------------
void CEditBox::ClearText()
{
	if( hEdit != NULL )
#ifdef UNICODE_ROCK
	SetWindowTextW( hEdit , _RT("") );
#else
	SetWindowText( hEdit , _RT("") );
#endif

	
}

//-----------------------------------------------------------------------------
void CEditBox::SetTextSize( int LimitLen )
{
	SAFE_DELETE_ARRAY( Text );
	SAFE_DELETE_ARRAY( EditText );
	SAFE_DELETE_ARRAY( PwText );
	
	Text		= NULL;
	EditText	= NULL;
	PwText		= NULL;

#ifdef UNICODE_ROCK
	
	if( (GetID() != WID_Login_UserNameEdit) && 
		(GetID() != WID_Login_PasswordEdit) )
	{
		int iSwapTemp = (LimitLen>>1)-1;
		LimitLen = iSwapTemp;
	}

	Text		= SAFE_NEW_ARRAY( TCHAR , (LimitLen+1)*2 );	
	EditText	= SAFE_NEW_ARRAY( TCHAR , (LimitLen+1)*2);
	PwText		= SAFE_NEW_ARRAY( TCHAR , (LimitLen+1)*2 );
	
	m_AllocTextLen = (LimitLen+1)*2;
	m_AllocEditTextLen = (LimitLen+1)*2;
	m_AllocPwTextLen = (LimitLen+1)*2;

#else
	Text		= SAFE_NEW_ARRAY( TCHAR , LimitLen+1 );	
	EditText	= SAFE_NEW_ARRAY( TCHAR , LimitLen+1 );
	PwText		= SAFE_NEW_ARRAY( TCHAR , LimitLen+1 );
	
	m_AllocTextLen = LimitLen+1;
	m_AllocEditTextLen = LimitLen+1;
	m_AllocPwTextLen = LimitLen+1;

#endif
	ZeroMemory(Text , m_AllocTextLen );
	ZeroMemory(EditText , m_AllocTextLen );
	ZeroMemory(PwText , m_AllocTextLen );

	TextLimitLen = LimitLen;
	
	if( NULL != hEdit )
	{
		SendMessage( hEdit, EM_LIMITTEXT, (WPARAM)LimitLen, 0 );//����Ʈ ���� 32k

	}
}


///---------------------------------------------------------------------------
///-- CreateWindowEditBox
///---------------------------------------------------------------------------
void CEditBox::CreateWindowEditBox()
{
	//edit style ����
    DWORD Estyle = WS_CHILD|WS_BORDER|WS_VISIBLE;
    
	if( EditStyle == esOnlyNumeric ){	Estyle |= ES_NUMBER;	}
	if( EditStyle == esPassword ){	Estyle |= ES_PASSWORD;	}
	if( isAutoScroll ){	Estyle |= ES_AUTOHSCROLL;	}

    //edit ��Ʈ�� ������ ����
	//int Ex = Csize[0] + BlankLeft;//Ex, Ey�� �ܼ��� ��ġ���̰� ������...�߿����� �ʴ�...
	//int Ey = Csize[1] + BlankTop;
	int Ew = Csize[2] - ( BlankRight + BlankLeft );//Ew ����...Left�� Right�� ���� ���ؼ� ��� �ؾ� �ȴ�. 
	int Eh = Csize[3] - ( BlankBottom + BlankTop );//Ey�� ��������....
#ifdef UNICODE_ROCK
	hEdit = CreateWindowExW( WS_EX_APPWINDOW, _RT("edit"), NULL, Estyle, 0, 9000, Ew, Eh,
		pDT->hWnd, (HMENU)Iid, pDT->hInst, NULL );
	SendMessageW( hEdit, EM_LIMITTEXT, (WPARAM)TextLimitLen, 0 );//����Ʈ ���� 32k

#else
	hEdit = CreateWindowEx( WS_EX_APPWINDOW, _T("edit"), NULL, Estyle, 0, 9000, Ew, Eh,
		pDT->hWnd, (HMENU)Iid, pDT->hInst, NULL );
	SendMessage( hEdit, EM_LIMITTEXT, (WPARAM)TextLimitLen, 0 );//����Ʈ ���� 32k
#endif

#ifdef APP_IME_BLOCK_CHS
	nRui->LanguageChange(hEdit , 0 , (LPARAM)GetKeyboardLayout(0));  

#endif 

    //SendMessage( hEdit, EM_SETPASSWORDCHAR, (WPARAM)_T("��"), 0 );//	
	
	TCHAR*			FontName	= theFontg->GetName();
	int				FontSize	= theFontg->GetSize();
	NFontWeights	FontWeight	= theFontg->GetWeight();
	NFontCharset	FontCharset	= theFontg->GetCharset();
	
    //��Ʈ ������ ���� - (�ܱ��� �����Ҷ�...ĳ���ͼ� ���� ���� �ʿ��Ұ� ������....�ϴ��� �Ѿ��)
    hFont = CreateFont( FontSize, 0, 0, 0, FontWeight, 0, 0, 0, FontCharset, 3, 2, 1, FIXED_PITCH|FF_MODERN, FontName );
    SendMessage( hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM( FALSE, 0 ) );
	
	///---------------------------------------------------------------------------
    ///-- Edit Style �� ���� IMM Set 
	hImc = ImmGetContext( hEdit );
	ImmGetConversionStatus( hImc, &Con, &Sen);				
	if( EditStyle == esOnlyAlphaNumeric || EditStyle == esOnlyNumeric || EditStyle == esPassword )
	{
		ImmSetOpenStatus( hImc, FALSE );
        ImmSetConversionStatus( hImc, Con | IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
	}
	else
	{		
		ImmSetConversionStatus( hImc, Con | IME_CMODE_NATIVE, IME_SMODE_AUTOMATIC );
	}
	ImmReleaseContext(hEdit , hImc );
	
	///---------------------------------------------------------------------------
	///-- Sub classing
	//Edit ���ν����� ��� ����... Ŭ������ �ּҸ� ����


#ifdef UNICODE_ROCK
	SetWindowLongW( hEdit, GWL_USERDATA, (LONG)this );
    OldEditProc = (WNDPROC)SetWindowLongW( hEdit, GWL_WNDPROC, (LONG)AgentEditSubProc );
#else
	SetWindowLong( hEdit, GWL_USERDATA, (LONG)this );
    OldEditProc = (WNDPROC)SetWindowLong( hEdit, GWL_WNDPROC, (LONG)AgentEditSubProc );
#endif

	
}
void CEditBox::GetWindowEditTextW()
{
	//Edit �Էµ� ���ڿ��� ĳ�� ���� ������ 
	WCHAR TestUniCode[256]={ 0, };
	GetWindowTextW( hEdit, reinterpret_cast<WCHAR*>(EditText) , TextLimitLen+1 );
    GetWindowEditCaretInfo();
	//Edit �Է� ��Ÿ�Ͽ� ���� ���ڿ� ����
	//���⼭ �����ڵ�� ��ȯ�Ѵ� . 
	
	WCHAR Input[256] = L"";
	int size = 	Rstrlen(RWCHAR(EditText))  ;
	memcpy( Text, EditText, sizeof(RTCHAR) * (size +1)  );
	TextLength = size;


	//UNICODE_CHAGE
#ifdef EDITBOX_COMMA
	//���ڸ� �������� 0�� 3���̻��� �ÿ��� �޸��� ǥ���Ѵ�. 
    if( EditStyle == esOnlyNumeric && isNumberComma )
	{
		SAFE_STR_CPY(EditText , Text , TextLimitLen);
		
		theFontg->GetStringInfo( Text, &StrInfo );
		return ; 
		
	}
#endif	
	
    if( EditStyle == esPassword )
    {
		WCHAR wchPW[32] = L"";
        if( TextLength == 0 )
        {
            SAFE_STR_CPY( PwText, _RT(""), m_AllocPwTextLen );
        }
        else 
        {
			int i = 0;
            for( i = 0; i < TextLength; ++i )
            {
                wchPW[i] = '*';
			}
			
			wchPW[i] = '\0';
        }
		
		//int size = 	wcslen(wchPW) + 1 ;
        SAFE_STR_CPY( EditText, wchPW , m_AllocEditTextLen );
		//SAFE_STR_CPY(Text , wchPW , m_AllocEditTextLen );
		
		theFontg->GetStringInfo( Text, &StrInfo );
		return;
    }
		
	
	//Caret.isDirectionMove = false;
	
    //
	theFontg->GetStringInfo( Text, &StrInfo );


}
void CEditBox::GetWindowEditText()
{
#ifdef UNICODE_ROCK
	GetWindowEditTextW();
#else 
	GetWindowEditTextA();
#endif 
}


//-----------------------------------------------------------------------------
void CEditBox::GetWindowEditTextA()
{
	//Edit �Էµ� ���ڿ��� ĳ�� ���� ������ 
    GetWindowText( hEdit, EditText, TextLimitLen+1 );
		
	GetWindowEditCaretInfo();
	//Edit �Է� ��Ÿ�Ͽ� ���� ���ڿ� ����
	SAFE_STR_CPY( Text, EditText, m_AllocTextLen );
	TextLength = Rstrlen( Text );

#ifdef EDITBOX_COMMA
	//���ڸ� �������� 0�� 3���̻��� �ÿ��� �޸��� ǥ���Ѵ�. 
    if( EditStyle == esOnlyNumeric && isNumberComma )
	{
		SAFE_STR_CPY(EditText , Text , TextLimitLen);

		theFontg->GetStringInfo( Text, &StrInfo );
		return ; 
		
	}
#endif
	//Caret.isDirectionMove = false;
	    


    if( EditStyle == esPassword )
    {
        if( TextLength == 0 )
        {
            SAFE_STR_CPY( PwText, _RT(""), m_AllocPwTextLen );
        }
        else 
        {
			int i = 0;
            for( i = 0; i < TextLength; ++i )
            {
				PwText[i] = '*';				
            }
			
			PwText[i] = '\0';
        }

        SAFE_STR_CPY( EditText, PwText, m_AllocEditTextLen );
    }

    //
	theFontg->GetStringInfo( Text, &StrInfo );
}
void CEditBox::GetDeleteComma(TCHAR* Str)
{
	int len = strlen(Str);
	
	if(len == 0 )
		return;

	char CurNumber[32]="";
	char NumberComma[32]="";
	
	SAFE_STR_CPY( CurNumber, Str, 32 );

	int i, j ; 
	
	for( i = 0 , j = 0  ; i < 32 ; i++ )
	{
		if( CurNumber[i] == ',')
			continue;
		
		NumberComma[j++] = CurNumber[i] ;
		
		if(CurNumber[i] == NULL) break; 
	}

	ZeroMemory(Str , sizeof(Str));

	SAFE_STR_CPY( Str , NumberComma , 32 );

}

//-----------------------------------------------------------------------------
void CEditBox::GetWindowEditCaretInfo()
{
	POINT Pos;
	GetCaretPos( &Pos );
	Caret.X = Pos.x;
	Caret.Y = Pos.y;
	if( hEdit == NULL )
		return;
	//�ѱ� �������϶��� SelEnd�״��...
#ifdef UNICODE_ROCK
	SendMessageW( hEdit, EM_GETSEL, (WPARAM)&Caret.SelStart, (LPARAM)&Caret.SelEnd );
	Caret.FchVisibleIdx = SendMessageW( hEdit, EM_GETFIRSTVISIBLELINE, 0, 0 );
#else
	SendMessage( hEdit, EM_GETSEL, (WPARAM)&Caret.SelStart, (LPARAM)&Caret.SelEnd );
	Caret.FchVisibleIdx = SendMessage( hEdit, EM_GETFIRSTVISIBLELINE, 0, 0 );
#endif
	

	if( ( isAlphaNumeric == false ) && ( EditStyle != esPassword ) )
	{
		// �ѱ۸���϶��� Idx�� 2�� ����
		// ������ �Ҷ��� �ѱ۵� ����� ���������� �ѱ��ڴ� Idx�� 1�� ó��
		// �׷��� ����� ���߱� ���ؼ� 2�� �����ش�.
		Caret.FchVisibleIdx /= 2;
	}
#ifdef EDITBOX_COMMA
	if( EditStyle == esOnlyNumeric &&  isNumberComma  )
	{
		//�ٽ� ����Ѵ�.  

		char CurNumber[256]="";
		char NumberComma[256]="";
				
		SAFE_STR_CPY( CurNumber, EditText, 256 );
		int i, j ; 
		
		for( i = 0 , j = 0  ; i < 256 ; i++ )
		{
			if( CurNumber[i] == ',')
					continue;
			
			NumberComma[j++] = CurNumber[i] ;
			
			if(CurNumber[i] == NULL) break; 
		}
		

		int Count = 0 ; 
		int Len = strlen(NumberComma);
		int FirstPoint =Len%3  ;
		if( FirstPoint == 0 )
		{
			FirstPoint = 3;
		}

		ZeroMemory(&CurNumber , sizeof(NumberComma));

		for(i =0 , j=0 ; i < 256 ; i++ , j++)
		{
			//3�ڸ����̻� ������ . 
			if( NumberComma[i] != NULL && Count == FirstPoint )
			{
				FirstPoint = 3; 
				Count = 0; 
				CurNumber[j++] = ',';
			}

			CurNumber[j] = NumberComma[i];
			
			if( Count > 3 )	Count = 0 ; 

			Count++;
			
			if(NumberComma[i] == NULL) break; 
		}	
		ZeroMemory(Text , sizeof(Text));
		SAFE_STR_CPY(Text , CurNumber , TextLimitLen);
		SAFE_STR_CPY(EditText , CurNumber , TextLimitLen);
	
		int Commalen = Rstrlen( Text );

		//���� ĳ����ġ�� ��� ����Ű�� �ִ°� .. 
	
		Count = 0 ; 
		int MaxCount = Caret.SelStart;
		for( i = 0 ; i <= MaxCount ; i++)
		{
			if( CurNumber[i] == ',')
			{
				++Count;
				++MaxCount;
			}
		}

		//�޸����� ���� ������ ���̴� . 
		Caret.SelStart = Caret.SelStart + Count;
		Caret.SelEnd = Caret.SelEnd + Count;
		//�׾ȿ� �޸��� �ִ��� �˻� �Ѵ�.
		


//		SendMessage( hEdit, EM_SETSEL, (WPARAM)&Caret.SelStart, (LPARAM)&Caret.SelEnd ); 
		
		//Caret.SelStart
	}
#endif



	Caret.SelStart -= 1;
	Caret.SelEnd -= 1;
}

//-----------------------------------------------------------------------------
void CEditBox::SetCaretSelBlock( int BlockX, int BlockY, int BlockW, int BlockH, D3DCOLOR Ccolor )
{
	Caret.X = BlockX;
	Caret.Y = BlockY;
	Caret.W = BlockW;
	Caret.H = BlockH;
	Caret.Color = Ccolor;
}

//-----------------------------------------------------------------------------
void CEditBox::GetImmConversionStatus()
{
	hImc = ImmGetContext( hEdit ); 
			
	//Edit �Է� ���  �ѱ� or ���� ???
    ImmGetConversionStatus( hImc, &Con, &Sen);

    if( Con & IME_CMODE_NATIVE )
    {
		//EditBox�� ��Ÿ���� PASSWORD�϶�...�� �Է¸��� �׻� �ѱ��ΰ�?
        isAlphaNumeric = false;//"HANGUL" 
    }
    else
    {
        isAlphaNumeric = true;//"ENGLISH"
    }
	ImmReleaseContext( hEdit , hImc );
}


LRESULT CALLBACK CEditBox::EditSubProc_IME( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return EditSubProc_JP( hEdit, iMsg, wParam, lParam );
}

///---------------------------------------------------------------------------
///-- EditSubProc_JP
///---------------------------------------------------------------------------
///---------------------------------------------------------------------------
///-- EditSubProc_JP
///---------------------------------------------------------------------------
LRESULT CALLBACK CEditBox::EditSubProc_JP( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	//static EVENT_MESSAGE em;
    static int SelStart( 0 );
	static int SelEnd( 0 );
    static int FchVindex( 0 );

	WORD wrParam  = 0;
 
	CANDIDATELIST *lpA_CandList = NULL;  
	int ll_candli_buf_size ;
	int ll_length;	
	
	HIMC	lA_hIMC;	
	int ll_candi_start_idx ;
	int i;
	
#ifdef UNICODE_ROCK
	BYTE *lpc_tempbuffer = NULL;
#else
	char *lpc_tempbuffer = NULL;
#endif

    int a = 0;

    switch( iMsg )
    {

	//fine NEWIME
	case WM_INPUTLANGCHANGE:
		nRui->LanguageChange(hEdit , wParam , lParam);

		break;
		///-- Japan IME
		///-- �ش� Edit �ڽ��� Ű���� ��Ŀ���� ������
	case WM_KILLFOCUS:
		{
			///-- Get Context
			lA_hIMC = ImmGetContext( hEdit ); 

			///-- ImmNotifyIME - input context ���º�ȭ�� IME���� �����ϴ� �Լ� 
			///-- composition string �� ����� no composition string���� ���¸� �����Ѵ�.
			ImmNotifyIME( lA_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );			
			///-- candidate list�� �ݴ´�.
			ImmNotifyIME( lA_hIMC, NI_CLOSECANDIDATE, 0, 0 ); 
			
			///-- release
			ImmReleaseContext( hEdit, lA_hIMC );
			///-- candidate ����� �ݴ´�.
			nRui->Set_Cdi_cmpStr( "" );
			nRui->Set_Cdi_visible( false );
		}
		break;
	case WM_IME_SETCONTEXT:
		{
		    lParam = 0;

		}
		break;
	case WM_IME_STARTCOMPOSITION:
		{
			//return 0L;
		}
		break;
	///== IME ends composition		
	case WM_IME_ENDCOMPOSITION:		
		{
			nRui->Set_Cdi_cmpStr( "" );
			nRui->Set_Cdi_visible( false );
		//NEWIME
			nRui->SetReadingStr(_RT(""));
			nRui->SetStringReset();
			RLGS("WM_IME_ENDCOMPOSITION" );
		}
		break;
		///---------------------------------------------------------------------------
		///-- WM_IME_COMPOSITION
		///-- IME�� keystroke����� composition status�� ��ȯ
	case WM_IME_COMPOSITION :
		{				
			///-- wParam ���� �������� ǥ��� DBCS character�� composition string���� ��ȯ
			///-- lParam composition string �Ǵ� ��ȯ�� character�� � ��ȯ���� ����ϴ� ��
		
			///-- Get Context
			lA_hIMC = ImmGetContext( hEdit );
			
			///-- ���հ�� ��ȯ
			if ( lParam & GCS_RESULTSTR )
			{
				///-- �������� ���ڿ��� �ʱ�ȭ 
				nRui->Set_Cdi_cmpStr( "" );			
			}
			///-- ������ ��ȯ
			else if ( lParam & GCS_COMPSTR )
			{
				//ret_val = true;
				///-- ImmGetCompositionString - composition string�� ���� ������ �����´�.
#ifdef UNICODE_ROCK
				ll_length = ImmGetCompositionStringW( lA_hIMC, GCS_COMPSTR, NULL, 0 );
#else
				ll_length = ImmGetCompositionString( lA_hIMC, GCS_COMPSTR, NULL, 0 );
#endif
				nRui->Set_Cdi_Pos( Caret.X , Caret.Y );
				
				if( ll_length > 0 )
				{	
#ifdef UNICODE_ROCK
					wchar_t* temp = SAFE_NEW_ARRAY( wchar_t , ll_length +1 );
					memset(temp , NULL , sizeof(wchar_t)* (ll_length +1)  );
					ImmGetCompositionStringW( lA_hIMC, GCS_COMPSTR, temp, ll_length );
					nRui->Set_Cdi_cmpStr( RCASTWTOC(temp) );
					SAFE_DELETE_ARRAY(temp);
#else
					
					lpc_tempbuffer = SAFE_NEW_ARRAY( TCHAR , ll_length + 1 );					
					ImmGetCompositionString( lA_hIMC, GCS_COMPSTR, lpc_tempbuffer, ll_length );
					lpc_tempbuffer[ll_length]=0;
					nRui->Set_Cdi_cmpStr( lpc_tempbuffer );
#endif
				}
				//ret->insertstring( mpt_getstring, ll_length, false );
				
			}
			///-- ��Ÿ
			else if( !lParam && !wParam )
			{			
				///-- �������� ���ڿ��� �ʱ�ȭ
				nRui->Set_Cdi_cmpStr( "" );
			}
			///-- release
			ImmReleaseContext( hEdit, lA_hIMC );
			SAFE_DELETE_ARRAY( lpc_tempbuffer );
			RLGS("WM_IME_COMPOSITION" );
		}
		break;
	case WM_IME_CONTROL:
		{
			int a = 0;
		}
		break;
		///---------------------------------------------------------------------------
		///-- WM_IME_NOTIFY		
	case WM_IME_NOTIFY:		/// IME window�� ��ȭ 
		switch( wParam )
		{
		//fine NEWIME
		case IMN_PRIVATE:
			nRui->GetPrivateReadingString(hEdit);
				// Trap some messages to hide reading window
			switch( nRui->m_dwId[0] )
			{
			case IMEID_CHT_VER42:
			case IMEID_CHT_VER43:
			case IMEID_CHT_VER44:
			case IMEID_CHS_VER41:
			case IMEID_CHS_VER42:
				if((lParam==1)||(lParam==2)) return 0;
				break;
				
			case IMEID_CHT_VER50:
			case IMEID_CHT_VER51:
			case IMEID_CHT_VER52:
			case IMEID_CHT_VER60:
			case IMEID_CHS_VER53:
				if((lParam==16)||(lParam==17)||(lParam==26)||(lParam==27)||(lParam==28)) return 0;
				break;
			}
			break;
		case IMN_OPENCANDIDATE:				///-- IME�� candidate window�� ����		
		case IMN_CHANGECANDIDATE :			///-- IME�� candidate window�� ������ �ٲܶ�	
			///-- Get Context
			//NEWIME
#ifdef APP_IME_BLOCK_CHS
			if( nRui->GetCheckIME_Unicode() == true )
			{
				return 0 ;
			}
#endif 
			nRui->SetStringReset();
			lA_hIMC = ImmGetContext( hEdit ); 
				
#ifdef UNICODE_ROCK
			ll_candli_buf_size = ImmGetCandidateListW( lA_hIMC, 0, NULL, 0 );			
			lpc_tempbuffer = SAFE_NEW_ARRAY ( BYTE , ll_candli_buf_size );	
			memset(lpc_tempbuffer , NULL , ll_candli_buf_size);
#else
			///-- Get length
			ll_candli_buf_size = ImmGetCandidateList( lA_hIMC, 0, NULL, 0 );			
			///-- Prepare Buffer
			lpc_tempbuffer = SAFE_NEW_ARRAY( char , ll_candli_buf_size );	
#endif

			if( lpc_tempbuffer == NULL ) 
			{
				ImmReleaseContext( hEdit, lA_hIMC );		
				return( CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam ) );
			}

			lpA_CandList = ( CANDIDATELIST *) lpc_tempbuffer;
			
			///-- Set lpA_CandList
			nRui->Set_Cdi_visible( true );
			///-- candidate list�� ������ ī��
#ifdef UNICODE_ROCK
			ll_candli_buf_size = ImmGetCandidateListW( lA_hIMC, 0, lpA_CandList, ll_candli_buf_size );
#else
			ll_candli_buf_size = ImmGetCandidateList( lA_hIMC, 0, lpA_CandList, ll_candli_buf_size );
#endif
			if( g_RockClient.GetLanguage() == eLang_China )
			{
				DWORD nCandidatePageSize = lpA_CandList->dwPageSize;			
				g_nCandidate_Page_Size = nCandidatePageSize;			
			}

			///-- 0~8 ->  ll_candi_start_idx = 0
			///-- 9~17 -> ll_candi_start_idx = 9
			ll_candi_start_idx =  ( lpA_CandList->dwSelection / g_nCandidate_Page_Size ) * g_nCandidate_Page_Size;	///-- ���� ������ Page �� ù idx
			///-- Candidate List ���� Value Set
			///-- lpA_CandList->dwCount - candidate string�� �� ����
			///-- ll_candi_start_idx - ���� �������� ���� �ε���
			///-- lpA_CandList->dwSelection - ���� ���õ� candidate string�� �ε���
			nRui->Set_Cdi_Values( lpA_CandList->dwCount, ll_candi_start_idx, lpA_CandList->dwSelection );
			///-- ���� ĳ����ǥ�� candidate ��ǥ�� �����Ѵ�.
			///-- ml_candi_x, ml_candi_y
			nRui->Set_Cdi_Pos( Caret.X, Caret.Y );			
			
			///-- Candidate List Set - ���������� 9���� candidate string�� ����
			for( i = 0; i < g_nCandidate_Page_Size; ++i )
			{	
				if( i + ll_candi_start_idx < lpA_CandList->dwCount ) 
				{
#ifdef UNICODE_ROCK
					WCHAR *wText =  (wchar_t*)(lpc_tempbuffer + lpA_CandList->dwOffset[i+ll_candi_start_idx ]);
					nRui->Set_Cdi_Str( i , RCHAR(wText));					
#else
					nRui->Set_Cdi_Str( i ,  (CHAR*)lpA_CandList + lpA_CandList->dwOffset[i + ll_candi_start_idx ] );					
#endif
				}
				else
				{
					nRui->Set_Cdi_Str( i ,"" );
				}
			}

			///-- Candidate �ϼ�
			///-- Candidata List�� ������ �������� �ʺ� - ml_width_max
			///-- Candidata List�� ������ �������� Y��ǥ - ml_list_y
			///-- Candidate List�� ������ �������� Y��ǥ - ml_page_y �� ���Ѵ�.
			nRui->Complete_Cdi_List();

			///-- Release
			ImmReleaseContext( hEdit, lA_hIMC );
			SAFE_DELETE_ARRAY( lpc_tempbuffer );
			return 1; ///-- Skip Default IME			
		///-- close the candidate window
		case IMN_CLOSECANDIDATE: 
			nRui->Set_Cdi_visible( false );
			RLGS("IMN_CLOSECANDIDATE" );
			break;			
		}	
		break;
    case WM_CHAR:
		//return 1;
        //�Է� ���ڼ� ���� �Ѱ�����....��~ �Ҹ� ó��
        if( TextLength >= TextLimitLen )
        {
			if( ( unsigned char( wParam ) >= 32 && unsigned char( wParam ) < 128 ) ||
			( unsigned char( wParam ) >= 0xA0 && unsigned char( wParam ) < 0xE0) )		
			{	return 0;	}
        }

#ifdef APP_IME_BLOCK_CHS
		if( nRui->GetCheckIME_Unicode() == true )
		{
			return 0;
		}
#endif 

		wrParam = LOWORD( wParam );
		
#ifdef ONLY_ENGLISH_SET		
		
		if( (((unsigned char)wrParam) & 0x80) )
		{
			return 0;
		}

#endif			
		switch( wrParam )
        {        
		case VK_ESCAPE:
                return( 0 );
        case VK_TAB:
				SendEvent( Pid, n_emEditTabAction, Iid, 0, 0, 0 );
                return( 0 );
        case VK_RETURN:
                //Client�� �޼��� ����
                //winid, NMSG_EDT_ENTER, text, len
                //em.focus_win = Iid;
                //em.notify    = NMSG_EDT_ENTER;
                //em.f_param   = (FPARAM)text;
                //em.s_param   = len;

                //memset(&caret, 0, sizeof(caret));
				//p_gui->cbFunc(&em);		    

				GetWindowEditText();

				if( isReturnActionClear )
				{
					ClearText();
				}

				EditDisable();
				SendEvent( Pid, n_emEditReturnAction, Iid, 0, 0, 0 );
				
				return( 0 );//�̺�Ʈ ó���ϰ�  return�����ν� Edit�� ���� �Է��ϰ� ����Ű�� ������ ����� ����                   
        }

        
		if(nRui->m_IsSurpportHCursor)
		{
			g_RenderManager.ShowCursor( FALSE );

#ifdef UNICODE_ROCK
			CallWindowProcW( OldEditProc, hEdit, iMsg, wParam, lParam );
#else 
			CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam );
#endif 

			g_RenderManager.ShowCursor( TRUE );
		}
		else
		{
			
#ifdef UNICODE_ROCK
			CallWindowProcW( OldEditProc, hEdit, iMsg, wParam, lParam );
#else 
			CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam );
#endif 
		}

        //GetWindowEditCaretInfo();
		//Caret.isDirectionMove = false;
        return( 0 );

    case WM_KEYDOWN:
        //------------------------------------------------------------
        //ID�� PW �Է��� �������θ�....
        //------------------------------------------------------------
        if( EditStyle == esOnlyAlphaNumeric || EditStyle == esOnlyNumeric )
        {
			lA_hIMC = ImmGetContext( hEdit ); 
			///-- ImmSetOpenStatus IME�� �ݴ´�.
			ImmSetOpenStatus( lA_hIMC, FALSE );
			///-- ���� Conversion mode�� ���ĺ�, ���ڻ��·� ��ȯ Sentence Mode�� None���� ��ȯ
            ImmSetConversionStatus( lA_hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
			ImmReleaseContext( hEdit , lA_hIMC );
        }

#ifdef APP_IME_BLOCK_CHS
		if( nRui->GetCheckIME_Unicode() == true )
		{
			
			return 0 ;
		}
#endif 

        //����Ʈ �޼����� �̿��� �޼��� ������ ���������δ� ��������
        //���ڿ��� ����� ���� ���Ѵ�....(���κҸ�)
        //PostMessage(hedit, WM_USER+1, NULL, NULL);

        //�� �ٸ� �Ϲ�Ű�� ������ �ʴ°�?
        //CallWindowProc(OldEditProc, hedit, iMsg, wParam, lParam);
        //���ڿ� ���...��ƾ
	
        //------------------------------------------------------------
        //Ű ���¿� ���� �⺻ ó���� ĳ�� ���� ����
        //------------------------------------------------------------
        if( ( wParam == VK_LEFT )   || ( wParam == VK_RIGHT ) || 
            ( wParam == VK_HOME )   || ( wParam == VK_END )   ||
            ( wParam == VK_DELETE ) || ( wParam == VK_BACK )     )
        {
			
#ifdef UNICODE_ROCK
			CallWindowProcW( OldEditProc, hEdit, iMsg, wParam, lParam );
#else 
			CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam );
#endif 
       		GetWindowEditCaretInfo();
			//Caret.isDirectionMove = true;
            return( 0 );
        }
        break;

	case WM_CUT:
    case WM_COPY:
        if( EditStyle == esPassword )
        {
            //SendMessage(hedit, EM_REPLACESEL, 0, (LPARAM)"");
            SendMessage( hEdit, EM_GETSEL, (WPARAM)&SelStart, (LPARAM)&SelEnd );
            if( SelStart != SelEnd )
            {
                SendMessage( hEdit, EM_SETSEL, (WPARAM)-1, 0 );//������ -1�̸� ���� ���, �ݴ��.... ��μ���				
				//SendMessage( hEdit, EM_REPLACESEL, (WPARAM)0, (LPARAM)PwText );//���ÿ����� �ٸ� ���ڿ��� ��ü
            }
        }
        break;
	case WM_PASTE:
		{

#ifdef APP_IME_BLOCK_CHS
			if(Iid == WID_CharCreate_Name_EditBox )			
			{
				ClearText();
				return  0 ; 
			}
#endif 

#ifdef GLOBAL_VERSION			
			return( 0 );
#endif
		}
		break;
    case ( WM_USER + 44 ):
		{
			GetWindowEditText();
		}
	case ( WM_USER + 45 ):
		{

        ///-- ���� IME�� Conversion mode�� Sentence Mode �ҷ��´�.
		GetImmConversionStatus();
		///-- �ش� �̺�Ʈ�� �������� �ʴ´�.
		SendEvent( Pid, n_emEditImmConvStatus, (EPARAM)Iid, (EPARAM)isAlphaNumeric, 0, 0 );
		break;
		}
	}
#ifdef UNICODE_ROCK
	return( CallWindowProcW( OldEditProc, hEdit, iMsg, wParam, lParam ) );
#else 
	return( CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam ) );
#endif 
    
}

///---------------------------------------------------------------------------
///-- EditSubProc
///---------------------------------------------------------------------------
LRESULT CALLBACK CEditBox::EditSubProc( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	//static EVENT_MESSAGE em;
    static int SelStart( 0 );
	static int SelEnd( 0 );
    static int FchVindex( 0 );

	WORD wrParam  = 0;
	   
    switch( iMsg )
    {
	case WM_IME_STARTCOMPOSITION:
		{
			
		}
		break;
	case WM_CHAR:
			//return 1;
			//�Է� ���ڼ� ���� �Ѱ�����....��~ �Ҹ� ó��
			if( TextLength >= TextLimitLen )
			{
				// VK_NUMPAD0 ~ VK_NUMPAD9 (0x60 - 0x69)
				// VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) 
				// VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) 
				if( ( LOWORD( wParam ) >= 32 ) && ( LOWORD( wParam ) <= 122 ) )
				{
					return 0;
				}
			}

		wrParam = LOWORD( wParam );
		
#ifdef ONLY_ENGLISH_SET		
		
		///-- By simwoosung
		///-- ��Ƽ����Ʈ �ڵ��϶� ����
		if( (((unsigned char)wrParam) & 0x80) )
		{
			return 0;
		}

#endif
			
		switch( wrParam )
        {
		case VK_LMENU:
		case VK_RMENU:
				return( 0 );
        case VK_ESCAPE:
                return( 0 );
        case VK_TAB:
				SendEvent( Pid, n_emEditTabAction, Iid, 0, 0, 0 );
                return( 0 );
        case VK_RETURN:
                //Client�� �޼��� ����
                //winid, NMSG_EDT_ENTER, text, len
                //em.focus_win = Iid;
                //em.notify    = NMSG_EDT_ENTER;
                //em.f_param   = (FPARAM)text;
                //em.s_param   = len;

                //memset(&caret, 0, sizeof(caret));
				//p_gui->cbFunc(&em);
				
				GetWindowEditText();

				if( isReturnActionClear )
				{
					ClearText();
				}

				EditDisable();
				SendEvent( Pid, n_emEditReturnAction, Iid, 0, 0, 0 );				
				
				return( 0 );//�̺�Ʈ ó���ϰ�  return�����ν� Edit�� ���� �Է��ϰ� ����Ű�� ������ ����� ����                   
        }

  		if(nRui->m_IsSurpportHCursor)
		{
			g_RenderManager.ShowCursor( FALSE );
			CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam );
			g_RenderManager.ShowCursor( TRUE );
		}
		else
		{
			CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam );
		}

        //GetWindowEditCaretInfo();
		//Caret.isDirectionMove = false;
        return( 0 );

		
    case WM_KEYDOWN:
        //------------------------------------------------------------
        //ID�� PW �Է��� �������θ�....
        //------------------------------------------------------------
        if( EditStyle == esOnlyAlphaNumeric || EditStyle == esOnlyNumeric )
        {
			HIMC	lA_hIMC;	
			lA_hIMC = ImmGetContext( hEdit ); 
			ImmSetOpenStatus( lA_hIMC, FALSE );
            ImmSetConversionStatus( lA_hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
			ImmReleaseContext( hEdit , lA_hIMC );
        }

        //����Ʈ �޼����� �̿��� �޼��� ������ ���������δ� ��������
        //���ڿ��� ����� ���� ���Ѵ�....(���κҸ�)
        //PostMessage(hedit, WM_USER+1, NULL, NULL);

        //�� �ٸ� �Ϲ�Ű�� ������ �ʴ°�?
        //CallWindowProc(OldEditProc, hedit, iMsg, wParam, lParam);
        //���ڿ� ���...��ƾ
        //------------------------------------------------------------
        //Ű ���¿� ���� �⺻ ó���� ĳ�� ���� ����
        //------------------------------------------------------------
        if( ( wParam == VK_LEFT )   || ( wParam == VK_RIGHT ) || 
            ( wParam == VK_HOME )   || ( wParam == VK_END )   ||
            ( wParam == VK_DELETE ) || ( wParam == VK_BACK )     )
        {
            CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam );
			GetWindowEditCaretInfo();

			//Caret.isDirectionMove = true;
            return( 0 );
        }
        break;
	/*case WM_KEYUP:
		{
			return( 0 );
		}
		break;
	*/
	case WM_IME_NOTIFY:	
		{
		
			switch(wParam)
			{
			case IMN_SETCONVERSIONMODE:
		//	case IMN_SETOPENSTATUS:
				if( EditStyle == esOnlyAlphaNumeric )
				{
					HIMC	lA_hIMC;	
					lA_hIMC = ImmGetContext( hEdit ); 
					if(lA_hIMC)
					{
						ImmSetOpenStatus( lA_hIMC, FALSE );
						ImmSetConversionStatus( lA_hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
						ImmReleaseContext( hEdit , lA_hIMC );
					}
					
				}
				break;
			}
			
			
		}
		break; 
	case WM_CUT:
    case WM_COPY:
        if( EditStyle == esPassword )
        {
            //SendMessage(hedit, EM_REPLACESEL, 0, (LPARAM)"");
            SendMessage( hEdit, EM_GETSEL, (WPARAM)&SelStart, (LPARAM)&SelEnd );
            if( SelStart != SelEnd )
            {
                SendMessage( hEdit, EM_SETSEL, (WPARAM)-1, 0 );//������ -1�̸� ���� ���, �ݴ��.... ��μ���			
				//SendMessage( hEdit, EM_REPLACESEL, (WPARAM)0, (LPARAM)PwText );//���ÿ����� �ٸ� ���ڿ��� ��ü
            }
        }
        break;
	case WM_PASTE:
		{
#ifdef GLOBAL_VERSION			
			return( 0 );
#endif
		}
		break;
    case ( WM_USER + 44 ):
		GetWindowEditText();
        break;

	case ( WM_USER + 45 ):
        GetImmConversionStatus();
		SendEvent( Pid, n_emEditImmConvStatus, (EPARAM)Iid, (EPARAM)isAlphaNumeric, 0, 0 );
		break;	
	}

    return( CallWindowProc( OldEditProc, hEdit, iMsg, wParam, lParam ) );
}


///---------------------------------------------------------------------------
///-- AgentEditSubProc
///---------------------------------------------------------------------------
///-- Edit Box �� ���� Ŭ����
LRESULT CALLBACK CEditBox::AgentEditSubProc( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	AgentEditBox = (CEditBox*)GetWindowLong( hEdit, GWL_USERDATA );	
	
	///g_nCandidate_Page_Size = 10;
	
	///return( AgentEditBox->EditSubProc_IME( hEdit, iMsg, wParam, lParam ) );
	
	///-- Japan IME
	if( g_RockClient.GetLanguage() == eLang_Japan ) 
	{
		return( AgentEditBox->EditSubProc_JP( hEdit, iMsg, wParam, lParam ) );
	}
	else if( g_RockClient.GetLanguage() == eLang_China )
	{
		return( AgentEditBox->EditSubProc_IME( hEdit, iMsg, wParam, lParam ) );
	}
	else if(g_RockClient.GetLanguage() == eLang_Taiwan || g_RockClient.GetLanguage() == eLang_HongKong )
	{
		return( AgentEditBox->EditSubProc_IME( hEdit, iMsg, wParam, lParam ) );
	}
	
	///-- Korea IME
    return( AgentEditBox->EditSubProc( hEdit, iMsg, wParam, lParam ) );
}

//-----------------------------------------------------------------------------
void CEditBox::UpdateHandler()//OR
{
	if( !isInit )
	{
		SetAdjustPosition( 0, 0 );
		isInit = true;
	}
}

//-----------------------------------------------------------------------------
void CEditBox::OnMouseFocus( int Mx, int My )
{
	Mx; My;
}

//-----------------------------------------------------------------------------
void CEditBox::OnMouseLButtonDown( int Mx, int My )
{
	Mx; My;
}

//-----------------------------------------------------------------------------
void CEditBox::OnMouseLButtonPress( int Mx, int My )
{
	Mx; My;
}

//-----------------------------------------------------------------------------
void CEditBox::OnMouseLButtonUp( int Mx, int My )
{
	Mx; My;
	
	SetEditFocus();
}

//-----------------------------------------------------------------------------
