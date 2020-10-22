
#include "..\\RockPCH.h"
#include "ChatEditBox.h"
#include "Rui.h"
#include "..\\..\\CRenderManager.h"
#include "..\\..\\RockClient.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CChatEditBox::CChatEditBox( SDesktop* DT )  : CEditBox(DT)
{	

}

CChatEditBox::~CChatEditBox()
{

}

void CChatEditBox::SetLimitLen(int nLen)
{
	n_LimitLen = nLen;
}

SEventMessage* CChatEditBox::EventProc( SEventMessage* EMsg )
{
	CEditBox::EventProc(EMsg);
	
	return EMsg;
}

void CChatEditBox::RenderProc()
{
	UpdateHandler();

	//Render->DrawState();	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );

	//Render->SetScissorTest( Tx, Ty, Tw, Th );

	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
	
	DrawText();
	DrawCaret();//캐럿 깜박임 처리 필요

}

void CChatEditBox::GetWindowEditCaretInfo()
{
	POINT Pos;
	GetCaretPos( &Pos );
	Caret.X = Pos.x;
	Caret.Y = Pos.y;	
	if(hEdit == NULL)
		return;

    int nLines = (int) SendMessage( hEdit, EM_GETLINECOUNT, 0, 0 );

	//한글 조합중일때는 SelEnd그대로...

#ifdef UNICODE_ROCK
	SendMessageW( hEdit, EM_GETSEL, (WPARAM)&Caret.SelStart, (LPARAM)&Caret.SelEnd );
	SAFE_STR_CPY( Text, EditText, m_AllocTextLen );
	/*
	int size = wcslen(RWCHAR(EditText));
	memcpy( Text, EditText , sizeof(WCHAR) * (size +1) );
	//memcpy( EditText, &Input, sizeof(WCHAR) * (size +1) );			
	*/
#else
	SendMessage( hEdit, EM_GETSEL, (WPARAM)&Caret.SelStart, (LPARAM)&Caret.SelEnd );
	SAFE_STR_CPY( Text, EditText, m_AllocTextLen );
#endif
		
    Caret.FchVisibleIdx = GetVisibleIdx();		

	//static TCHAR str[40];
	//sprintf( str, "%s - %d\n", EditText, Caret.FchVisibleIdx );
	//OutputDebugString( str );

	Caret.SelStart -= 1;
	Caret.SelEnd -= 1;
}

//-----------------------------------------------------------------------------
void CChatEditBox::DrawText()
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
				if( SelStart < 0 || SelStart < Caret.FchVisibleIdx )
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
				
					///-- IdxBuf 는 2 Byte 1Byte 의 경우 커서는 반칸
					///-- IME
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
					//" " 렌더링 하지 않는다.
					//"\n"는 그대로 찍는다( Line 출력이기 때문에 개행은 처리되지 않는다. )
					// d3d8에서는 시저테스트가 없기 때문에...경계선을 넘어가면..출력하지 않는다.
					Render->DrawFont( RFont );
				}
			}
		}

		Count++;
	}

	//sel크기가 같다면 캐럿 출력 아니면 블럭 출력
	if( (SelStart != SelEnd) && (RFont != NULL) )
	{
		SetCaretSelBlock( Caret.BlockX, Ay, (Caret.BlockW - Caret.BlockX), RFont->h, D3DCOLOR_ARGB( 128, 205, 200, 167 ) );
	}
}

int  CChatEditBox::GetVisibleIdx()
{
	if( TextLength == 0 )
	{
		return Caret.FchVisibleIdx;	
	}	
	
	static FONTID IdxVBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	int SelStart( 0 );
	int SelEnd( 0 );

	GetRealCaretIdx( &SelStart, &SelEnd );
	
	memset( IdxVBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;

	NumStr = theFontg->Confirm( EditText, IdxVBuf );
	
	int Count( 0 );
	int i( 0 );

	///-- 현재 보일수 있는 글자수 계산
	int nAx = GetAlignPosX();
	
	for( i = 0; i < NumStr; ++i  )
	{
		if( Caret.FchVisibleIdx <= i )
        {
			RFont = theFontg->GetFontasm( IdxVBuf[i] );
			
			if( RFont != NULL )
			{
				nAx = nAx + RFont->w;
				if( nAx < ( Tx + Tw )  )
				{
					Count++;
				}
			}
		}
	}

	if( (Caret.FchVisibleIdx + Count) < SelEnd )
	{
		return SelEnd - Count;
	}

	if( Caret.FchVisibleIdx > SelEnd )
	{
		return SelEnd;
	}
	
	return Caret.FchVisibleIdx;
}

//-----------------------------------------------------------------------------
void CChatEditBox::DrawCaret()
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
		//캐럿이 블럭모드가 아니라면....점멸....
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

IRESULT CChatEditBox::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, 
								  EPARAM xParam, EPARAM exParam )
{

	int Iresult( 0 );
	Iresult = CEditBox::ReceiveEvent( Event, fParam, sParam, xParam, exParam );	
	
	return( Iresult );
}

LRESULT CALLBACK CChatEditBox::EditSubProc_IME( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return EditSubProc_JP( hEdit, iMsg, wParam, lParam );
}

///---------------------------------------------------------------------------
///-- EditSubProc_JP
///---------------------------------------------------------------------------
LRESULT CALLBACK CChatEditBox::EditSubProc_JP( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam )
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
		
    
    switch( iMsg )
    {
		//fine NEWIME
	case WM_INPUTLANGCHANGE:
		nRui->LanguageChange(hEdit , wParam , lParam);
		///-- Japan IME 
	case WM_KILLFOCUS:
		{
			///-- Get Context
			lA_hIMC = ImmGetContext( hEdit ); 
			
			ImmNotifyIME( lA_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );			
			ImmNotifyIME( lA_hIMC, NI_CLOSECANDIDATE, 0, 0 ); 
			
			///-- release
			ImmReleaseContext( hEdit, lA_hIMC );
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
			nRui->SetReadingStr(_RT(""));
			nRui->SetStringReset();
			RLGS("WM_IME_ENDCOMPOSITION" );
		}
		break;
		///---------------------------------------------------------------------------
		///-- WM_IME_COMPOSITION	
	case WM_IME_COMPOSITION :
		{				
			///-- Get Context
			lA_hIMC = ImmGetContext( hEdit ); 
			
			///-- 조합결과

			if ( lParam & GCS_RESULTSTR )
			{
				nRui->Set_Cdi_cmpStr( "" );			
			}
			///-- 조합중
			else if ( lParam & GCS_COMPSTR )
			{
				//ret_val = true;
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
			///-- 기타
			else if( !lParam && !wParam )
			{			
				nRui->Set_Cdi_cmpStr( "" );
			}
			///-- release
			ImmReleaseContext( hEdit, lA_hIMC );
			SAFE_DELETE_ARRAY( lpc_tempbuffer );			
		}
		break;
		///---------------------------------------------------------------------------
		///-- WM_IME_NOTIFY		
	case WM_IME_NOTIFY:		
		switch( wParam )
		{
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
		case IMN_OPENCANDIDATE:
		case IMN_CHANGECANDIDATE :
			nRui->SetStringReset();
			///-- Get Context
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

			ll_candi_start_idx =  ( lpA_CandList->dwSelection / g_nCandidate_Page_Size ) * g_nCandidate_Page_Size;	///-- 현재 보여줄 Page 의 첫 idx
		
			nRui->Set_Cdi_Values( lpA_CandList->dwCount, ll_candi_start_idx, lpA_CandList->dwSelection );
			nRui->Set_Cdi_Pos( Caret.X, Caret.Y );
			
			///-- Candidate List Set
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
			///-- Candidate 완성
			nRui->Complete_Cdi_List();
			
			///-- Release
			ImmReleaseContext( hEdit, lA_hIMC );
			SAFE_DELETE_ARRAY( lpc_tempbuffer );
			return 1; ///-- Skip default IME
			
			break;			
		case IMN_CLOSECANDIDATE: 
			nRui->Set_Cdi_visible( false );
			RLGS("IMN_CLOSECANDIDATE" );
			break;			
		}	
		break;
    case WM_CHAR:
        //입력 문자수 제한 넘겼을때....띵~ 소리 처리
        if( TextLength >= TextLimitLen )
        {
			if( ( unsigned char( wParam ) >= 32 && unsigned char( wParam ) < 128 ) ||
				( unsigned char( wParam ) >= 0xA0 && unsigned char( wParam ) < 0xE0) )		
			{	return 0;	}
        }

		wrParam = LOWORD( wParam );
		
#ifdef ONLY_ENGLISH_SET		
		
		///-- By simwoosung
		///-- 멀티바이트 코드일때 리턴
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
		case 92:				//역슬래쉬
				return ( 0 );
        case VK_RETURN:
                //Client에 메세지 전송
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

				//EditDisable();
				SendEvent( Pid, n_emEditReturnAction, Iid, 0, 0, 0 );				

				return( 0 );//이벤트 처리하고  return함으로써 Edit에 글자 입력하고 엔터키를 누를때 경고음 방지                   
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

		if( LOWORD( wParam ) == VK_SPACE )
		{
			GetWindowEditText();
	
			SendEvent( Pid, n_emEditSpaceAction, Iid, 0, 0, 0 );
		}

        //GetWindowEditCaretInfo();
		//Caret.isDirectionMove = false;
        return( 0 );

    case WM_KEYDOWN:
        //------------------------------------------------------------
        //ID와 PW 입력은 영문으로만....
        //------------------------------------------------------------
        if( EditStyle == esOnlyAlphaNumeric || EditStyle == esOnlyNumeric )
        {
			lA_hIMC = ImmGetContext( hEdit ); 
			ImmSetOpenStatus( lA_hIMC, FALSE );
            ImmSetConversionStatus( lA_hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);			
			ImmReleaseContext( hEdit , lA_hIMC );
        }

        //포스트 메세지를 이용한 메세지 전달은 구조적으로는 괜찮은데
        //문자열을 제대로 읽지 못한다....(원인불명)
        //PostMessage(hedit, WM_USER+1, NULL, NULL);

        //왜 다른 일반키는 먹히지 않는가?
        //CallWindowProc(OldEditProc, hedit, iMsg, wParam, lParam);
        //문자열 얻기...루틴

        //------------------------------------------------------------
        //키 상태에 따라 기본 처리와 캐럿 정보 갱신
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

	case WM_CUT:
    case WM_COPY:
        if( EditStyle == esPassword )
        {
            //SendMessage(hedit, EM_REPLACESEL, 0, (LPARAM)"");
            SendMessage( hEdit, EM_GETSEL, (WPARAM)&SelStart, (LPARAM)&SelEnd );
            if( SelStart != SelEnd )
            {
                SendMessage( hEdit, EM_SETSEL, (WPARAM)-1, 0 );//시작이 -1이면 선택 취소, 반대면.... 모두선택				
				//SendMessage( hEdit, EM_REPLACESEL, (WPARAM)0, (LPARAM)PwText );//선택영역을 다른 문자열로 대체
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
///-- EditSubProc
///---------------------------------------------------------------------------
LRESULT CALLBACK CChatEditBox::EditSubProc( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	//static EVENT_MESSAGE em;
    static int SelStart( 0 );
	static int SelEnd( 0 );
    static int FchVindex( 0 );
	
	WORD wrParam  = 0;
    
    switch( iMsg )
    {
    case WM_CHAR:
        //입력 문자수 제한 넘겼을때....띵~ 소리 처리
        if( TextLength >= TextLimitLen )
        {
            // VK_NUMPAD0 ~ VK_NUMPAD9 (0x60 - 0x69)
            // VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) 
            // VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) 
            if( ( LOWORD( wParam ) >= 32 ) && ( LOWORD( wParam ) <= 122 ) ){	return 0;	}
        }

		wrParam = LOWORD( wParam );
		
#ifdef ONLY_ENGLISH_SET		
		
		///-- By simwoosung
		///-- 멀티바이트 코드일때 리턴
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
		case 92:				//역슬래쉬
				return ( 0 );
        case VK_RETURN:
                //Client에 메세지 전송
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

				//EditDisable();
				SendEvent( Pid, n_emEditReturnAction, Iid, 0, 0, 0 );			
				
				return( 0 );//이벤트 처리하고  return함으로써 Edit에 글자 입력하고 엔터키를 누를때 경고음 방지                   
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

		if( LOWORD( wParam ) == VK_SPACE )
		{
			GetWindowEditText();
			SendEvent( Pid, n_emEditSpaceAction, Iid, 0, 0, 0 );
		}

        //GetWindowEditCaretInfo();
		//Caret.isDirectionMove = false;
        return( 0 );
	case WM_IME_NOTIFY:	
		{
			switch(wParam)
			{
			case IMN_SETCONVERSIONMODE:
			case IMN_SETOPENSTATUS:
				if( EditStyle == esOnlyAlphaNumeric )
				{
					HIMC	lA_hIMC;	
					lA_hIMC = ImmGetContext( hEdit ); 
					ImmSetOpenStatus( lA_hIMC, FALSE );
					ImmSetConversionStatus( lA_hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
					ImmReleaseContext( hEdit , lA_hIMC );
				}
				break;
			}
			

		}
		break;
    case WM_KEYDOWN:
        //------------------------------------------------------------
        //ID와 PW 입력은 영문으로만....
        //------------------------------------------------------------
        if( EditStyle == esOnlyAlphaNumeric || EditStyle == esOnlyNumeric )
        {
			HIMC	lA_hIMC;	
			lA_hIMC = ImmGetContext( hEdit ); 
			ImmSetOpenStatus( lA_hIMC, FALSE );
            ImmSetConversionStatus( lA_hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
			ImmReleaseContext( hEdit , lA_hIMC );
        }

        //포스트 메세지를 이용한 메세지 전달은 구조적으로는 괜찮은데
        //문자열을 제대로 읽지 못한다....(원인불명)
        //PostMessage(hedit, WM_USER+1, NULL, NULL);

        //왜 다른 일반키는 먹히지 않는가?
        //CallWindowProc(OldEditProc, hedit, iMsg, wParam, lParam);
        //문자열 얻기...루틴

        //------------------------------------------------------------
        //키 상태에 따라 기본 처리와 캐럿 정보 갱신
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

	case WM_CUT:
    case WM_COPY:
        if( EditStyle == esPassword )
        {
            //SendMessage(hedit, EM_REPLACESEL, 0, (LPARAM)"");
            SendMessage( hEdit, EM_GETSEL, (WPARAM)&SelStart, (LPARAM)&SelEnd );
            if( SelStart != SelEnd )
            {
                SendMessage( hEdit, EM_SETSEL, (WPARAM)-1, 0 );//시작이 -1이면 선택 취소, 반대면.... 모두선택			
				//SendMessage( hEdit, EM_REPLACESEL, (WPARAM)0, (LPARAM)PwText );//선택영역을 다른 문자열로 대체
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

void CChatEditBox::CreateWindowEditBox()
{
		//edit style 지정
    DWORD Estyle = WS_CHILD|WS_BORDER|WS_VISIBLE;
    
	if( EditStyle == esOnlyNumeric ){	Estyle |= ES_NUMBER;	}
	if( EditStyle == esPassword ){	Estyle |= ES_PASSWORD;	}
	if( isAutoScroll )
	{	Estyle |= ES_AUTOHSCROLL;	}

    //edit 컨트롤 생성과 설정
	//int Ex = Csize[0] + BlankLeft;//Ex, Ey는 단순히 위치값이가 때문에...중요하지 않다...
	//int Ey = Csize[1] + BlankTop;
	int Ew = Csize[2] - ( BlankRight + BlankLeft );//Ew 폭은...Left와 Right의 값을 더해서 계산 해야 된다. 
	int Eh = Csize[3] - ( BlankBottom + BlankTop );//Ey도 마찬가지....

#ifdef UNICODE_ROCK 
    hEdit = CreateWindowExW( WS_EX_APPWINDOW, L"edit", NULL, Estyle, 0, -9000, Ew, Eh,
		pDT->hWnd, (HMENU)Iid, pDT->hInst, NULL );
	SendMessageW( hEdit, EM_LIMITTEXT, (RP_MAX_CHAT_SIZE/2)-1 , 0 );//디폴트 길이 32k  
#else
    hEdit = CreateWindowEx( WS_EX_APPWINDOW, "edit", NULL, Estyle, 0, -9000, Ew, Eh,
		pDT->hWnd, (HMENU)Iid, pDT->hInst, NULL );
	SendMessage( hEdit, EM_LIMITTEXT, (WPARAM)TextLimitLen, 0 );//디폴트 길이 32k  
#endif
    

	//SendMessage( hEdit, EM_SETPASSWORDCHAR, (WPARAM)_T("●"), 0 );//

	
	TCHAR*			FontName	= theFontg->GetName();
	int				FontSize	= theFontg->GetSize();
	NFontWeights	FontWeight	= theFontg->GetWeight();
	NFontCharset	FontCharset	= theFontg->GetCharset();
	
    //폰트 생성과 설정 - (외국어 지원할때...캐릭터셋 설정 변경 필요할것 같은데....일단은 넘어가자)
    hFont = CreateFont( FontSize, 0, 0, 0, FontWeight, 0, 0, 0, FontCharset, 3, 2, 1, FIXED_PITCH|FF_MODERN, FontName );
	SendMessage( hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM( FALSE, 0 ) );

  
	
	///---------------------------------------------------------------------------
    ///-- Edit Style 에 따른 IMM Set 
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
	//Edit 프로시져를 얻기 위해... 클래스의 주소를 저장
#ifdef UNICODE_ROCK
	SetWindowLong( hEdit, GWL_USERDATA, (LONG)this );
    OldEditProc = (WNDPROC)SetWindowLongW( hEdit, GWL_WNDPROC, (LONG)AgentEditSubProc );
#else
	SetWindowLong( hEdit, GWL_USERDATA, (LONG)this );
    OldEditProc = (WNDPROC)SetWindowLong( hEdit, GWL_WNDPROC, (LONG)AgentEditSubProc );
#endif
    
}

