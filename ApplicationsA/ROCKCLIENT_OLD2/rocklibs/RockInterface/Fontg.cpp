///////////////////////////////////////////////////////////////////////////////
///
///		File		: Fontg.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////

#include "..\\..\\StringManager.h"
#include "..\\..\\RockClient.h"

#include "Fontg.h"

///---------------------------------------------------------------------------
///-- CFontg::CFontg
///---------------------------------------------------------------------------
///-- 국가코드에 맞는 Font 지정
///-- Font 의 필수 생성자 이 생성자로 생성되어야만 한다.
///-- CFontman::GetFont 에서 호출된다.

CFontg::CFontg( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth, int al_width  ) 
{	
	TCHAR* Name = NULL;
//	int l_nDevSkip = GetPrivateProfileInt( "Local", "Language", 0, "./setup.ini");
	
	
	// 폰트 설정하는 곳이 여러군데 이므로 모두 찾아서 변경해야 함. 	
	switch( FontType )	
	{
	case n_ftGulimChe:
		{	
			if( g_RockClient.GetLanguage() == eLang_Korea )
			{
				Name = _T( "굴림체" ); 
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif				
			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )
			{ 
//				Name = _T( "굃굍 굊긕긘긞긏" );		// MS 고딕	< 필히 일본 버젼 로컬 폰트 이름으로 사용!!! >   
				Name = _T( "굃굍 긕긘긞긏" );		// MS 고딕	< 필히 일본 버젼 로컬 폰트 이름으로 사용!!! >   
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_Taiwan) 
			{
				Name = _T( "mingliu" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_HongKong) 
			{
				Name = _T( "mingliu" ); 
			}
			else
			{
				Name = _T(FONTSTR_ALL ); 
			}
			
		}
		break;
	case n_ftDotum:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )
			{
				Name = _T( "HY울릉도M" );
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif				

			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )
			{
				Name = _T( "굃굍 뼻뮝" );			// MS 명조 
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_Taiwan)
			{
				Name = _T( "mingliu" ) ; 
			}
			else if(g_RockClient.GetLanguage() == eLang_HongKong)
			{
				Name = _T( "mingliu" ) ; 
			}
			else
			{
				Name = _T( FONTSTR_ALL ); 
			}
			//Name = _T( "HY울릉도M" ); 			
		}
		break;
	default:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )
			{
				Name = _T( "굴림체" );
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif					
			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )
			{				
				Name = _T( "굃굍 긕긘긞긏" );  
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_Taiwan)
			{
				Name = _T( "mingliu" ) ; 
			}
			else if(g_RockClient.GetLanguage() == eLang_HongKong)
			{
				Name = _T( "mingliu" ) ; 
			}
			else
			{
				Name = _T(FONTSTR_ALL ); 
			}
		}
		break;
	}

	theFontType = FontType;
	strcpy( theName, Name );
	theSize		= Size;
	theWeight	= Weight;
	theDeco		= Deco;
	theCharset	= Fontasm->GetTTFontCharset( theName );
	theSmooth = ab_smooth;		
	theFontWidth = al_width;
	theRefCount = 0;

	memset( theFontasm, NULL, sizeof( SFont* ) * MAX_FONTASM );
	
}

//-----------------------------------------------------------------------------
CFontg::~CFontg()
{
	for( int i = 0 ; i < MAX_FONTASM ; ++i )
	{
		if( NULL != theFontasm[ i ] )
		{
			if( theFontasm[i]->pTexture )
			{
				theFontasm[i]->pTexture->Release();
				theFontasm[i]->pTexture = NULL;				
			}
			
			SAFE_DELETE(theFontasm[i]);		
		}
	}
}

//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
void CFontg::Out( TCHAR* Str, int x, int y, D3DCOLOR Color, LPDIRECT3DDEVICE9 D3DDevice )
#else
void CFontg::Out( TCHAR* Str, int x, int y, D3DCOLOR Color, LPDIRECT3DDEVICE8 D3DDevice )
#endif // DIRECT_VERSION_9_MJH
{
	if( ( Str == NULL ) || ( Rstrlen( Str ) == 0 ) )
	{
		return;
	}
	if( D3DDevice == NULL )
	{
		return;
	}
	
	int PosX = x;
	int PosY = y;
	
	static FONTID IdxBuf[256];
	int NumStr( 0 );
	
	memset( IdxBuf, 0, sizeof(FONTID) * 256 );
	SFont* tmpFont = NULL;

	NumStr = Confirm( Str, IdxBuf );

	//SetScissorTest( x, y, w, h );
	
	//if( SUCCEEDED( g_RenderManager.BeginScene() ) )
    //{
		for( int i = 0; i < NumStr; ++i  )
		{
			tmpFont = GetFontasm( IdxBuf[i] );

			if( tmpFont != NULL )
			{
				tmpFont->vt[0].diffuse	= tmpFont->vt[1].diffuse = tmpFont->vt[2].diffuse	= tmpFont->vt[3].diffuse = Color;
				
				tmpFont->vt[0].x = (float)PosX - 0.5f;
				tmpFont->vt[0].y = (float)PosY - 0.5f;
				tmpFont->vt[1].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[1].y = tmpFont->vt[0].y;
				tmpFont->vt[2].x = tmpFont->vt[0].x + tmpFont->w;
				tmpFont->vt[2].y = tmpFont->vt[0].y + tmpFont->h;
				tmpFont->vt[3].x = tmpFont->vt[0].x;
				tmpFont->vt[3].y = tmpFont->vt[0].y + tmpFont->h;

				PosX = PosX + tmpFont->w;

				if( ( IdxBuf[i] != 32 ) && ( IdxBuf[i] != 10 ) ) //  " "와 "\n"은 렌더링 하지 않는다.
				{
					g_RenderManager.SetTexture( 0, tmpFont->pTexture );
					g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, tmpFont->vt, sizeof( SVertex ) );
				}
			}
		}

	//	g_RenderManager.EndScene();
	//}
}

//-----------------------------------------------------------------------------
bool CFontg::Existence( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth , int al_width )
{
	TCHAR* Name = NULL;
//	int l_nDevSkip = GetPrivateProfileInt( "Local", "Language", 0, "./setup.ini");

	switch( FontType )	
	{
	case n_ftGulimChe:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )
			{
				Name = _T( "굴림체"); 
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif	
			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )
			{
				Name = _T( "굃굍 긕긘긞긏" ); 
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_Taiwan)
			{
				Name = _T( "mingliu" ) ; 
			}
			else if(g_RockClient.GetLanguage() == eLang_HongKong)
			{
				Name = _T( "mingliu" ) ; 
			}
			else
			{
				Name = _T( FONTSTR_ALL ); 
			}
		}
		break;
	case n_ftDotum:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )
			{
				Name = _T( "HY울릉도M" ); 
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif

			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )
			{
				Name = _T( "굃굍 뼻뮝" ); 
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_Taiwan)
			{
				Name = _T( "mingliu" ) ; 
			}
			else if(g_RockClient.GetLanguage() == eLang_HongKong)
			{
				Name = _T( "mingliu" ) ; 
			}
			else
			{
				Name = _T( FONTSTR_ALL ); 
			}
		}
		break;
	default:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )
			{
				Name = _T( "굴림체" ); 
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif	
			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )
			{				
				Name = _T( "굃굍 긕긘긞긏" ); 
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_Taiwan)
			{
				Name = _T( "mingliu" ) ; 
			}
			else if(g_RockClient.GetLanguage() == eLang_HongKong)
			{
				Name = _T( "mingliu" ) ; 
			}
			else
			{
				Name = _T( FONTSTR_ALL ); 
			}
		}
		break;
	}

	if( ( _tcsicmp( theName, Name ) == 0 ) && ( theSize == Size ) && ( theWeight == Weight ) &&
		( theDeco == Deco ) && theSmooth == ab_smooth && theFontWidth == al_width )
	{
		return( true );
	}
	
	return( false );
}

//-----------------------------------------------------------------------------
int CFontg::GetRefCount()
{
	return( theRefCount );
}

//-----------------------------------------------------------------------------
void CFontg::AddRef()
{
	theRefCount++;
}

//-----------------------------------------------------------------------------
void CFontg::Release()
{
	theRefCount--;

	if( theRefCount == 0 )
	{
		for( int i = 0; i < MAX_FONTASM; ++i )
		{
			if( theFontasm[i] != NULL )
			{
				theFontasm[i]->pTexture->Release();
				delete( theFontasm[i] );
				theFontasm[i] = NULL;
			}
		}
	}
}

//-----------------------------------------------------------------------------
int CFontg::Confirm( void* pStr, FONTID* Buf )
{
	char *Str = (char*)(pStr);
	int NumStr( 0 );
	int Result( 0 );
	
	Result = Fontasm->Confirm( &theFontasm[0], Str, Buf );	
	
	if( Result >= 0 )
	{
		return( Result );	// 0:문자열이 없다. 1이상: 문자열의 갯수(이미 폰트텍스쳐는 만들어져 있다.)
							// -1: 폰트 텍스쳐를 생성해야 된다. 계속 진행.
	}

	Fontasm->Setfontg_asm( theFontType, theSize, theWeight, theDeco, theSmooth, theFontWidth ); // fontasm의 fontg설정과 같은지 확인


#ifdef UNICODE_ROCK
	NumStr = Fontasm->CreateUNICODE( &theFontasm[0], Str, Buf );

#else
	NumStr = Fontasm->CreateDBCS( &theFontasm[0], Str, Buf );

#endif
	
	return( NumStr );
}

//-----------------------------------------------------------------------------
SFont* CFontg::GetFontasm( int Idx )
{
	return( theFontasm[Idx] );
}

//by simwoosung
void CFontg::GetRIStringInfo(TCHAR* Str, SStringInfo* StrInfo)
{
	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int Width( 0 );
	int Height( 0 );
	int NumStr( 0 );

	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* tmpFont = NULL;


	int Len = Rstrlen( Str );
	StrInfo->nCLen = Len;
	
	if( Len == 0 )
	{
		NumStr	= Confirm( _RT("A"), IdxBuf );
		tmpFont	= GetFontasm( IdxBuf[0] );

		if( tmpFont != NULL && StrInfo != NULL )
		{
			StrInfo->Width = 1;
			StrInfo->Height = tmpFont->Rih;
			StrInfo->NumStr = 0;
		}
	}
	else
	{
		NumStr	= Confirm( Str, IdxBuf );
		
		for( int i = 0; i < NumStr; ++i )
		{
			tmpFont	= GetFontasm( IdxBuf[i] );
			if( tmpFont != NULL )
			{
				Width = Width + tmpFont->Riw;
				Height = tmpFont->Rih;
			}
		}

		if( StrInfo != NULL )
		{
			StrInfo->Width	= Width;
			StrInfo->Height = Height;
			StrInfo->NumStr = NumStr;
		}
	}
}

//-----------------------------------------------------------------------------
void CFontg::GetStringInfo( void* Str, SStringInfo* StrInfo )
{
//	char *Str = RCHAR(pStr);
	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int Width( 0 );
	int Height( 0 );
	int NumStr( 0 );

	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* tmpFont = NULL;


	int Len = Rstrlen( RWCHAR(Str) );
	StrInfo->nCLen = Len;

	if( Len == 0 )
	{
		NumStr	= Confirm( _RT("A"), IdxBuf );
		tmpFont	= GetFontasm( IdxBuf[0] );

		if( tmpFont != NULL && StrInfo != NULL )
		{
			StrInfo->Width = 1;
			StrInfo->Height = tmpFont->h;
			StrInfo->NumStr = 0;
		}
	}
	else
	{
		NumStr	= Confirm( Str, IdxBuf );
		
		for( int i = 0; i < NumStr; ++i )
		{
			tmpFont	= GetFontasm( IdxBuf[i] );
			if( tmpFont != NULL )
			{
				Width = Width + tmpFont->w;
				Height = tmpFont->h;
			}
		}

		if( StrInfo != NULL )
		{
			StrInfo->Width	= Width;
			StrInfo->Height = Height;
			StrInfo->NumStr = NumStr;
		}
	}
}

//-----------------------------------------------------------------------------
void CFontg::GetStringInfo( void* pStr, SStringInfo* StrInfo, int MaxWidth )
{
	char *Str = (char*)pStr;

	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int Width( 0 );
	int Height( 0 );
	int NumStr( 0 );
	int	LineCount( 1 );

	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* tmpFont = NULL;


	int Len = Rstrlen( RWCHAR(Str) );
	if( Len == 0 )
	{
		return;
	}
	
	NumStr	= Confirm( Str, IdxBuf );
	int nPosX = 0;
		
	for( int i = 0; i < NumStr; ++i )
	{
		tmpFont	= GetFontasm( IdxBuf[i] );
		if( tmpFont != NULL )
		{
			Width = Width + tmpFont->w;

			if( LineCount == 1 )
			{
				Height = tmpFont->h;
			}

			if( ( Width > MaxWidth ) || ( IdxBuf[i] == 10 ) || (IdxBuf[i] == 92)  || 
				( (IdxBuf[i] == 32) ) )
			{
				if( IdxBuf[i] == 32 )	///스페이스
				{
					//다음 라인이 위드값을 초과하면....
					int j = i;
					nPosX = Width;
					while( IdxBuf[++j] != 32 && j < NumStr )
					{
						SFont* RtFont = NULL;
						RtFont =  GetFontasm( IdxBuf[j] );
						if(RtFont == NULL)
						{
							goto tEnd;
						}
						nPosX += ( RtFont )->w;	
						int niIdx = (int)IdxBuf[j];
						if( 47 > niIdx || niIdx > 122) //아스키코드가 아닐 경우에만
						{
							if(nPosX > MaxWidth)
							{
								Width = 0;
								goto tTerm;
							}
						}
						if(nPosX > MaxWidth)
						{
							Width = 0;
							goto tTerm;
						}
					}
					
					if( !( (nPosX > MaxWidth) ) )
					{
						goto tEnd;
					}
				}

				if( (nPosX > MaxWidth) )
				{
					int niIdx = (int)IdxBuf[i];
					if( 47 <= niIdx && niIdx <= 122) //아스키코드일 경우에만
					{
						goto tEnd;
					}
				}

tTerm:
				LineCount++;
				Width = 0;
				Height = Height + tmpFont->h;
tEnd:
				if( IdxBuf[i] != 10 )
				{
					Width = Width + tmpFont->w;
				}
			}
		}
	}

	if( StrInfo )
	{
		StrInfo->Width	= max(Width,nPosX);
		StrInfo->Height = Height;
		StrInfo->NumStr = NumStr;
		StrInfo->Line	= LineCount;
	}
}


//-----------------------------------------------------------------------------
void CFontg::GetStringInfo( void* pStr, SStringInfo* StrInfo, int MaxWidth, bool n )
{
	char *Str = (char*)pStr;
	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int Width( 0 );
	int Height( 0 );
	int NumStr( 0 );
	int	LineCount( 1 );
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* tmpFont = NULL;


	int Len = Rstrlen( Str );
	if( Len == 0 )
	{
		return;
	}
	
	NumStr	= Confirm( Str, IdxBuf );		
	
	int nTempMaxWidth = 0;
	
	for( int i = 0; i < NumStr; ++i )
	{
		tmpFont	= GetFontasm( IdxBuf[i] );
		if( tmpFont != NULL )
		{
			Width = Width + tmpFont->w;

			if( LineCount == 1 )
			{
				Height = tmpFont->h;
			}

			if( ( Width > MaxWidth ) || ( IdxBuf[i] == 10 )  || ( IdxBuf[i] == 92 ) )
			{
				if(nTempMaxWidth < Width)
				{
					nTempMaxWidth = Width;
				}
				
				LineCount++;
				Width = 0;

				if( ( i + 1 ) != NumStr )
				{
					Height = Height + tmpFont->h;
				}

				if( (IdxBuf[i] != 10) && ( IdxBuf[i] != 92 ))
				{
					Width = Width + tmpFont->w;
				}
			}
		}
	}


	if( StrInfo != NULL )
	{	
		StrInfo->Width	= Width;
	
		if(LineCount != 1)
		{
			if(nTempMaxWidth < Width )
			{
				StrInfo->Width = Width;
			}
			else
			{
				StrInfo->Width = nTempMaxWidth;
			}
			
		}		
		StrInfo->Height = Height;
		StrInfo->NumStr = NumStr;
		StrInfo->Line	= LineCount;
	}
}

//-----------------------------------------------------------------------------
void CFontg::GetStringDistribute( TCHAR* LineStr, TCHAR* DescStr )
{
	int Idx( 0 );
	int Len( 0 );
	int Count( 0 );
	
    
	for( int i = 0; i < 100; ++i )
    {
		if( ( Count == 20 ) || ( DescStr[i] == '\0' ) )
		{
			return;
		}


		Idx = DetectionDBCS( &i, DescStr );
    
		if( Idx < 256 )
		{
			LineStr[i] = DescStr[i];
			Count++;
		}
		else
		{
			LineStr[i-1]= DescStr[i-1];
			LineStr[i]	= DescStr[i];
			Count++;
		}
	}
}

//-----------------------------------------------------------------------------
int CFontg::GetStringCount( TCHAR* Str )
{
	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );

	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* tmpFont = NULL;


	int Len = Rstrlen( Str );
	if( Len == 0 )
	{
		return ( 0 );
	}
	
	NumStr	= Confirm( Str, IdxBuf );
	return( NumStr );
}

//-----------------------------------------------------------------------------
int CFontg::DetectionDBCS( int* Count, char* Str )
{
	return( Fontasm->DetectionDBCS( Count, Str ) );
}

//-----------------------------------------------------------------------------
int	CFontg::GetPostoIndex( TCHAR* Str, int x, int y )
{
	/*
	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int xPos( 0 );
	int NumStr( 0 );

	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	stFont* tmpFont = NULL;

	NumStr	= Confirm( Str, IdxBuf );
		
	for( int i = 0; i < NumStr; ++i )
	{
		tmpFont	= GetFontasm( IdxBuf[i] );
		if( tmpFont != NULL )
		{
			xPos = xPos + tmpFont->w + 2;
			if( x == xPos )
			{
				return( i++ );
			}
		}
	}
//*/
	return( 0 );
}

//-----------------------------------------------------------------------------
TCHAR* CFontg::GetName()
{
	return( theName ); 
}

//-----------------------------------------------------------------------------
int CFontg::GetSize()
{
	return( theSize );
}

//-----------------------------------------------------------------------------
NFontWeights CFontg::GetWeight()
{
	return( theWeight );
}

//-----------------------------------------------------------------------------
NFontDecoration CFontg::GetDecoration()
{
	return( theDeco );
}

//-----------------------------------------------------------------------------
NFontCharset CFontg::GetCharset()
{
	return( theCharset );
}

//-----------------------------------------------------------------------------