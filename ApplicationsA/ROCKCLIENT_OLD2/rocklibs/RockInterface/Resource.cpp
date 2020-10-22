///////////////////////////////////////////////////////////////////////////////
///
///		File		: Resource.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "RockPCH.h"
#include "Resource.h"
#include "..\\..\\VFileSystem.h" 

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "stringmanager.h"

CResource* Resource;

extern long				g_nowTime;

//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
CResource::CResource( LPDIRECT3DDEVICE9 D3DDevice )
#else
CResource::CResource( LPDIRECT3DDEVICE8 D3DDevice )
#endif // DIRECT_VERSION_9_MJH
{
	theD3DDevice = D3DDevice;
	m_lPreTime = g_nowTime;
	m_wDeleteCnt = 0;
	
	memset( Img, 0, sizeof( SImage ) * MAX_IMG_AMOUNT );
	
	
	for( int i = 0 ; i < MAX_TEX_AMOUNT ; i++ )
	{
		Tex[ i ] = NULL;
	}

	memset(Notice, NULL , 10000 );

	LoadTextureInfoFile();
	//LoadNoticeFile();

#ifdef HELPTIP_DATA_RENEWAL
		LoadHelpStringInfoFile();	
#else 
	#ifdef UNICODE_ROCK
		LoadHelpStringInfoFileW();
	#else
		LoadHelpStringInfoFile();
	#endif
#endif 

	
	LoadHelpItemInfoFile();
	LoadCBT();
	LoadSlangFile();
}

//-----------------------------------------------------------------------------
CResource::~CResource()
{
	Release();
}

//-----------------------------------------------------------------------------
void CResource::Release()
{
	int i( 0 );

	for( i = 0; i < MAX_TEX_AMOUNT; ++i )
	{
		if( Tex[i] != NULL )
		{
			Tex[i]->pTexture = NULL;

			SAFE_DELETE(Tex[i]);
		}
	}

	for( i = 0; i < MAX_IMG_AMOUNT; ++i )
	{
		if( Img[i].pTexture != NULL )
		{
			Img[i].pTexture->Release();
			Img[i].pTexture = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
STexture* CResource::GetTexture( TEXID Tid )
{
	return( Tex[Tid] );
}

//-----------------------------------------------------------------------------
SImage* CResource::GetImageInfo( int ImgId )
{
	return( &Img[ImgId] );
}

//-----------------------------------------------------------------------------
TCHAR* CResource::GetNotice()
{
	return( Notice );
}

//-----------------------------------------------------------------------------
SHelpTip* CResource::GetHelpTipInfo( HLPID Hid )
{
	return( &HelpTip[Hid] );
}


//-----------------------------------------------------------------------------
SCursor* CResource::GetCursorInfo( NCursorType cursor_type )
{
	return( &Cursor[cursor_type] );
}

//-----------------------------------------------------------------------------
SDlgBorder* CResource::GetBorderInfo()
{
	return( Border );
}

//-----------------------------------------------------------------------------
void CResource::LoadNoticeFile()
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	FILE* fp;

	fp = _tfopen( _T("Interface\\Notice.txt"), _T("rt") );
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!���� ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	TCHAR Line[256] = _T("");
	
	while( !feof( fp ) )
	{
		_fgetts( Line, 256, fp );
		_tcscat( Notice, Line );
	}

	fclose( fp );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFInterface.OpenFile( "Notice.txt" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!���� ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	char tmp_str[256] = "";
	char tmp_line[256] = "";
	
	char	string [512];			// ���� �ٴ��� ���� ����
	bool	bRead  = true;			// ���� �� �о����� üũ		


	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return; 
	}		
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		strcpy( Notice , string);

		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			

	g_VFInterface.CloseFile( pFH );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}

void CResource::DeleteNotUsedTex()
{
	if( SAFE_TIME_COMPARE( timeGetTime() , >= , SAFE_TIME_ADD( m_lPreTime , c_UIDeleteTex_Update_Time ) ) )
	{
		m_lPreTime = timeGetTime();

		if(Img[m_wDeleteCnt].pTexture)
		{
			long lCheckTime = SAFE_TIME_SUB( timeGetTime() , Img[m_wDeleteCnt].m_lPreTime );
			if( SAFE_TIME_COMPARE( lCheckTime , >= , 60000 ) )
			{
				SAFE_RELEASE(Img[m_wDeleteCnt].pTexture);
				///--PRINT_DLG_LOG( "���� UI �ؽ��� ���� %s", Img[m_wDeleteCnt].FileName );
			}			
		}
		
		m_wDeleteCnt++;
		if( m_wDeleteCnt >= MAX_IMG_AMOUNT )
		{
			m_wDeleteCnt = 0;
		}
	}
}

///---------------------------------------------------------------------------
///-- DestroyDynamic
///---------------------------------------------------------------------------
///-- 2005.04.01 / Lyul
///-- �ش� TID �� ����ϴ� IMG �� �����

///-- 2005.05.09 / Lyul
///-- ���� Tid Image �� Dynamic Loading �� ���� Image �� �����Ѵ�.
void CResource::DestroyDynamic( int a_nTID )
{	
	if( Tex[a_nTID] && Img[Tex[a_nTID]->ImgId].pTexture )
	{
		///-- Log
		RLG3( _T( "## Dynamic Release : IMG(%d) TID(%d) %s" ), Tex[a_nTID]->ImgId,  a_nTID, _T(Img[Tex[a_nTID]->ImgId].FileName));

		///-- Image ����
		Img[Tex[a_nTID]->ImgId].pTexture->Release();
		Img[Tex[a_nTID]->ImgId].pTexture = NULL;
	}
}
///----------------------------------------------------------------------------
///-- Dynamic Loading Texture
///----------------------------------------------------------------------------
///-- 2005.04.01 / Lyul  
///-- ���� Texture Resource�� Loading �Ѵ�.

///-- 2005.05.08 / Lyul
///-- ���� Tid �� ������ Image �� Dynamic Loading ����
void CResource::LoadTextureDynamic( int a_nTID )
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE* fp;

	fp = _tfopen( _T("Interface\\tid.txt"), _T("rt") );//VSS�ø���...�ɼ� ����(�б�����)�϶�....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!TID ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	TCHAR Line[256] = _T("");
	TCHAR Str[120] = _T("");
	TCHAR DefineStr[120] = _T(""); //Separator
	TCHAR TypeStr[40] = _T("");
	int ImgId( 0 );
	int Tid( 0 );
	int FS_W( 0 );//fullsize_width
	int FS_H( 0 );//fullsize_height
	int X( 0 );
	int Y( 0 );
	int Width( 0 );
	int Height( 0 );
	
	int tmpx = 0;
	while( !feof( fp ) )
	{
		_fgetts( Line, 256, fp );
		_stscanf( Line, _T("%s %s"), DefineStr, TypeStr );
		
		if( _tcsicmp( DefineStr, _T("{") ) == 0 )
		{
			
			_fgetts( Line, 256, fp );
			_stscanf( Line, _T("%d %s %d %d"), &ImgId, Str, &FS_W, &FS_H );

			///-- Critical Error Check : Array Index 
			assert( ImgId < MAX_IMG_AMOUNT);			
			LoadImage( &Img[ImgId], Str, FS_W, FS_H );
			
			while( _tcsicmp( DefineStr, _T("}") ) != 0 )
			{
				_fgetts( Line, 256, fp );
				_stscanf( Line, _T("%s %d %d %d %d %d"), DefineStr, &Tid, &X, &Y, &Width, &Height );
				/*
				char tstr[80] = "";
				sprintf( tstr, "%s ---- tid: %d\n", DefineStr, Tid );
				OutputDebugString( tstr );
//*/
				SetTextureImageInfo( &Img[ImgId], ImgId, Tid, X, Y, Width, Height );
			}
		}
	}

	fclose( fp );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFInterface.OpenFile( "tid.txt" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!TID ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	char string[512] = "";			// ���� �ٴ��� ���� ����
	TCHAR Str[120] = _T("");
	TCHAR DefineStr[120] = _T(""); //Separator
	TCHAR TypeStr[40] = _T("");
	int ImgId( 0 );
	int Tid( 0 );
	int FS_W( 0 );//fullsize_width
	int FS_H( 0 );//fullsize_height
	int X( 0 );
	int Y( 0 );
	int Width( 0 );
	int Height( 0 );
	int tmpx = 0;	
	bool	bRead  = true;			// ���� �� �о����� üũ		


	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return; 
	}		
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		_stscanf( string, _T("%s %s"), DefineStr, TypeStr );

		///---------------------------------------------------------------
		///-- Block Access
		if( _tcsicmp( DefineStr, _T("{") ) == 0 )
		{
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}

			_stscanf( string, _T("%d %s %d %d"), &ImgId, Str, &FS_W, &FS_H );
			
			///-- Critical Error Check : Array Index 
			assert( ImgId < MAX_IMG_AMOUNT);						
			
			///---------------------------------------------------------------
			///-- Image Loading			
			if( Tex[a_nTID]->ImgId == ImgId )
			{
				///-- Log
				RLG2( _T( "#### Dynamic Loading : IMG(%d) %s" ), Tex[a_nTID]->ImgId, Str );
				
				LoadImage( &Img[ImgId], Str, FS_W, FS_H );					
				///---------------------------------------------------------------
				///-- TID Loading
				do
				{
					if( pFH->Gets( string, sizeof( string ) ) == 0 )
					{
						bRead = false;
						break;
					}
					_stscanf( string, _T("%s %d %d %d %d %d"), DefineStr, &Tid, &X, &Y, &Width, &Height );
					
					if( _tcsicmp( DefineStr, _T("}") ) != 0 )
					{
						Tex[Tid]->pTexture = Img[ImgId].pTexture;
						Tex[Tid]->vt[0].tu = Tex[Tid]->vt[0].x / Img[ImgId].Width;
						Tex[Tid]->vt[0].tv = Tex[Tid]->vt[0].y / Img[ImgId].Height;
						
						Tex[Tid]->vt[1].tu = Tex[Tid]->vt[1].x / Img[ImgId].Width;
						Tex[Tid]->vt[1].tv = Tex[Tid]->vt[1].y / Img[ImgId].Height;
						
						Tex[Tid]->vt[2].tu = Tex[Tid]->vt[2].x / Img[ImgId].Width;
						Tex[Tid]->vt[2].tv = Tex[Tid]->vt[2].y / Img[ImgId].Height;
						
						Tex[Tid]->vt[3].tu = Tex[Tid]->vt[3].x / Img[ImgId].Width;
						Tex[Tid]->vt[3].tv = Tex[Tid]->vt[3].y / Img[ImgId].Height;
						 ///-- RLG1(_T( "## Set Dynamic Texture Tid: %d"), Tid );
					}
					
				}while( _tcsicmp( DefineStr, _T("}") ) != 0 );

				g_VFInterface.CloseFile( pFH );
				return;
			}
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			

	g_VFInterface.CloseFile( pFH );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


}
///----------------------------------------------------------------------------
///-- LoadTextureInfoFile
///----------------------------------------------------------------------------
///-- ������ �� ��� Texture �� Loading �Ѵ�.

///-- 2005.04.01 / Lyul / DynamicLoading Flag �� True �̸� Loading Skip
///-- 2005.05.09 / Lyul / Multi TID �� ������ Image �� Dynamic Loading ����
///-- !!!! ���� Ÿ�̹��� Source Code �� �����ؾ���
void CResource::LoadTextureInfoFile()
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE* fp;

	fp = _tfopen( _T("Interface\\tid.txt"), _T("rt") );//VSS�ø���...�ɼ� ����(�б�����)�϶�....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!TID ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	TCHAR Line[256] = _T("");
	TCHAR Str[120] = _T("");
	TCHAR DefineStr[120] = _T(""); //Separator
	TCHAR TypeStr[40] = _T("");
	int ImgId( 0 );
	int Tid( 0 );
	int FS_W( 0 );//fullsize_width
	int FS_H( 0 );//fullsize_height
	int X( 0 );
	int Y( 0 );
	int Width( 0 );
	int Height( 0 );
	
	int tmpx = 0;
	while( !feof( fp ) )
	{
		_fgetts( Line, 256, fp );
		_stscanf( Line, _T("%s %s"), DefineStr, TypeStr );
		
		if( _tcsicmp( DefineStr, _T("{") ) == 0 )
		{
			
			_fgetts( Line, 256, fp );
			_stscanf( Line, _T("%d %s %d %d"), &ImgId, Str, &FS_W, &FS_H );

			///-- Critical Error Check : Array Index 
			assert( ImgId < MAX_IMG_AMOUNT);			
			LoadImage( &Img[ImgId], Str, FS_W, FS_H );
			
			while( _tcsicmp( DefineStr, _T("}") ) != 0 )
			{
				_fgetts( Line, 256, fp );
				_stscanf( Line, _T("%s %d %d %d %d %d"), DefineStr, &Tid, &X, &Y, &Width, &Height );
				/*
				char tstr[80] = "";
				sprintf( tstr, "%s ---- tid: %d\n", DefineStr, Tid );
				OutputDebugString( tstr );
//*/
				SetTextureImageInfo( &Img[ImgId], ImgId, Tid, X, Y, Width, Height );
			}
		}
	}

	fclose( fp );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFInterface.OpenFile( "tid.txt" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!TID ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	char string[512] = "";			// ���� �ٴ��� ���� ����
	TCHAR Str[120] = _T("");
	TCHAR DefineStr[120] = _T(""); //Separator
	TCHAR TypeStr[40] = _T("");
	int ImgId( 0 );
	int Tid( 0 );
	int FS_W( 0 );//fullsize_width
	int FS_H( 0 );//fullsize_height
	int li_dyload;
	int X( 0 );
	int Y( 0 );
	int Width( 0 );
	int Height( 0 );
	int tmpx = 0;	
	bool	bRead  = true;			// ���� �� �о����� üũ		


	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return; 
	}		
	
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		_stscanf( string, _T("%s %s"), DefineStr, TypeStr );

		///---------------------------------------------------------------
		///-- Block Access
		if( _tcsicmp( DefineStr, _T("{") ) == 0 )
		{
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}

			_stscanf( string, _T("%d %s %d %d %d"), &ImgId, Str, &FS_W, &FS_H, &li_dyload );

			///-- Critical Error Check : Array Index 
			assert( ImgId < MAX_IMG_AMOUNT);			


			if( ImgId == 165)
			{
				int a = 0 ; 
			}
			if (Img[ImgId].Width != NULL)
			{
				char StrMsg[ 256 ] = "";
				
				wsprintf( StrMsg , "CResource::SetTextureImageInfo -> ImgId : %d" , ImgId );
				
				MessageBox( NULL , StrMsg , "!�ߺ��� ImgId �߰�" , MB_OK );
			}
			

			///---------------------------------------------------------------
			///-- Image Loading			
			if( !li_dyload )
			{				
				LoadImage( &Img[ImgId], Str, FS_W, FS_H );	
			}	
			///-- Dynamic Loading Texture �� ����			
			else
			{
				///-- RLG1(_T( "## Loading of Dynamic Texture Skiped : %s"), _T(Str) );
				Img[ImgId].Width = FS_W;
				Img[ImgId].Height= FS_H;				
			}
			
			///---------------------------------------------------------------
			///-- TID Loading
			do
			{
				if( pFH->Gets( string, sizeof( string ) ) == 0 )
				{
					bRead = false;
					break;
				}

				_stscanf( string, _T("%s %d %d %d %d %d"), DefineStr, &Tid, &X, &Y, &Width, &Height );
	
				if( _tcsicmp( DefineStr, _T("}") ) != 0 )
				{					
					SetTextureImageInfo( &Img[ImgId], ImgId, Tid, X, Y, Width, Height );
				}
			}while( _tcsicmp( DefineStr, _T("}") ) != 0 );
			

		}

		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			

	g_VFInterface.CloseFile( pFH );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


}

//-----------------------------------------------------------------------------
void CResource::LoadImage( SImage* Img, TCHAR* FileName, int FS_W, int FS_H )
{
	strcpy( Img->FileName, FileName );
	Img->Width = FS_W;
	Img->Height = FS_H;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TCHAR PathName[256] = _T("Interface\\");
	TCHAR FullName[256] = _T("");
	
	strcpy( FullName, PathName );
	strcat( FullName, FileName );
		
	if( LoadTexture( &Img->pTexture, FullName, true, D3DCOLOR_XRGB(255, 0, 255) ) )
	{
		Img->m_lPreTime = timeGetTime();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	if( LoadTexture( &Img->pTexture, FileName, true, D3DCOLOR_XRGB(255, 0, 255) ) )
	{
		Img->m_lPreTime = timeGetTime();
	}	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}

void CResource::LoadHelpStringInfoFileW()
{
	ZeroMemory(HelpTip , sizeof(HelpTip));
	
	VFileHandle* pFH = g_VFInterface.OpenFile( "HelpTip.txt" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!HelpTip ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	WCHAR	string[512] = {0,};		// ���� �ٴ��� ���� ����
	bool	bRead  = true;			// ���� �� �о����� üũ
	WCHAR	def_str[40] = L"";
	int		len = 0;
	int		id_num = 0;

	pFH->Seek(2 , SEEK_SET);
	
	// �� ������ �б�
	if( pFH->Getws( string, sizeof( string ) ) == 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return; 
	}		
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Getws( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		swscanf( string, L"%s", def_str );
		
		if( wcscmp( def_str, L"{" ) == 0 )
		{
			while( wcscmp( def_str, L"}" ) != 0 )
			{
				if( pFH->Getws( string, sizeof( string ) ) == 0 )
				{
					bRead = false;
					break;
				}
				swscanf( string, L"%s %d", def_str, &id_num );
				
				len = wcslen( string );
	
				wcscpy( &string[len], L"" );
	
				for( int i = 0; i < len; ++i )
				{
					if( string[i] == ':'  )
					{
						WCHAR *Desc = (WCHAR*)HelpTip[id_num].Desc;
						wcscpy( Desc , &string[i+1] );
						break;
					}
				}
			}
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Getws( string, sizeof( string ) ) == 0 )
			bRead = false;			

	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			

   g_VFInterface.CloseFile( pFH );

	

}
//-----------------------------------------------------------------------------
void CResource::LoadHelpStringInfoFile()
{
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE* fp;

	//fp = _tfopen( _T("Interface\\HelpTip.txt"), _T("rt") );//VSS�ø���...�ɼ� ����(�б�����)�϶�....
	fp = _tfopen( _T("local_test\\HelpTip.txt"), _T("rt") );//VSS�ø���...�ɼ� ����(�б�����)�϶�....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!HelpTip ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	TCHAR	line[256] = "";	
	TCHAR	def_str[40] = "";
	int		len = 0;
	int		id_num = 0;

	while( !feof( fp ) )
	{
		_fgetts( line, 256, fp );
		_stscanf( line, _T("%s"), def_str );
		
		if( _tcsicmp( def_str, _T("{") ) == 0 )
		{
			while( _tcsicmp( def_str, _T("}") ) != 0 )
			{
				_fgetts( line, 256, fp );
				_stscanf( line, _T("%s %d"), def_str, &id_num );
				
				len = _tcslen( line );
				_tcscpy( &line[len-1], "" );//����...�ٹٲ� ���� ����
				for( int i = 0; i < len; ++i )
				{
					if( line[i] == ':'  )
					{
						_tcscpy( HelpTip[id_num].Desc, &line[i+1] );
						break;
					}
				}
			}
		}
	}

	fclose( fp );
	}
	else
	{
///////////////////////////////////////////////////////////////////////////////////////////////////	
//#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HELPTIP_DATA_RENEWAL
	VFileHandle* pFH = g_VFInterface.OpenFile( "HelpTip.dat" );
	
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!HelpTip ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}
	int  iCount = 0 ; 
	int  iIndex = 0 ; 
	int  iStrlen = 0 ; 


	///-- Read string count
	pFH->Read( ( void * )&iCount , sizeof( iCount ));
	//mpp8_string = SAFE_NEW_ARRAY( BYTE* , ml_count );	
	


	///-- Read Strings
	for(int i = 0 ; i < iCount; ++i )
	{
		///-- index check
		pFH->Read( ( void * )&iIndex, sizeof( iIndex ));

		///-- Length
		pFH->Read( ( void * )&iStrlen, sizeof( iStrlen ));
		
		///-- String
#ifdef UNICODE_ROCK
		pFH->Read( ( void * )HelpTip[iIndex].Desc, iStrlen+1 );
#else
	
		pFH->Read( ( void * )HelpTip[iIndex].Desc, iStrlen );
#endif	
	}
	
	g_VFInterface.CloseFile( pFH );
	


#else 

	VFileHandle* pFH = g_VFInterface.OpenFile( "HelpTip.txt" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!HelpTip ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	char	string[512] = {0,};		// ���� �ٴ��� ���� ����
	bool	bRead  = true;			// ���� �� �о����� üũ
	TCHAR	def_str[40] = _T( "" );
	int		len = 0;
	int		id_num = 0;


	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return; 
	}		
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		_stscanf( string, _T("%s"), def_str );
		
		if( _tcsicmp( def_str, _T("{") ) == 0 )
		{
			while( _tcsicmp( def_str, _T("}") ) != 0 )
			{
				if( pFH->Gets( string, sizeof( string ) ) == 0 )
				{
					bRead = false;
					break;
				}
				_stscanf( string, _T("%s %d"), def_str, &id_num );
				
				len = _tcslen( string );

				//by simwoosung
				_tcscpy( &string[len], "" );
				
				/*if( g_RockClient.GetLanguage() == eLang_Japan )
				{
					_tcscpy( &string[len], "" ); 		// �Ϻ��� ��� �� ���� , ���� ���� 			
				}
				else
				{
					_tcscpy( &string[len], "" );		//����...�ٹٲ� ���� ����	"," ����...���� �ѱ��� ���� ���� 
				}*/

				for( int i = 0; i < len; ++i )
				{
					if( string[i] == ':'  )
					{
						int str_length = 0;
						int Cnt = 0;
						int Garbage = 0 ; 

						str_length = strlen( string );
					
						if(g_RockClient.GetLanguage() != eLang_Japan )
						{
							while(Cnt <= str_length)
							{
								if(string[Cnt] == ';')
								{
									_tcscpy( &string[Cnt] , "");
									break; 								
								}
								Cnt++;
							}
							Garbage = str_length - Cnt;
						}										
						
						// _tcscpy( HelpTip[id_num].Desc, &string[i+1] );
						_tcsncpy( HelpTip[id_num].Desc, string + i + 1, (str_length- Garbage ) - i + 1 );						

						break;
					}
				}
			}
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			

	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			

   g_VFInterface.CloseFile( pFH );

#endif 

	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
//#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
}

const char* CResource::GetHelpTip(int Index)
{
	return HelpTip[Index].Desc;
}


//-----------------------------------------------------------------------------
void CResource::LoadHelpItemInfoFile()
{
}

//-----------------------------------------------------------------------------
void CResource::LoadCBT()
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE* fp;

	fp = _tfopen( _T("Interface\\CBT.txt"), _T("rt") );//VSS�ø���...�ɼ� ����(�б�����)�϶�....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!CBT ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	TCHAR	line[256] = "";	
	TCHAR	def_str[40] = "";
	int		count = 0;

	while( !feof( fp ) )
	{
		_fgetts( line, 256, fp );
		_stscanf( line, _T("%s"), def_str );
	
		//Ŀ�� ���� �б�
		if( _tcsicmp( def_str, _T("CURSOR_TYPE") ) == 0 )
		{
			_fgetts( line, 256, fp );
			_stscanf( line, _T("%s"), def_str );

			if( _tcsicmp( def_str, _T("{") ) == 0 )
			{
				while( _tcsicmp( def_str, _T("}") ) != 0 )
				{
					_fgetts( line, 256, fp );
					_stscanf( line, _T("%s %d %d %d %d %d %d"), def_str, &Cursor[count].img[0], &Cursor[count].img[1], &Cursor[count].img[2], &Cursor[count].img[3], &Cursor[count].img[4], &Cursor[count].img[5] );
					count++;
				}
			}
		}

		
		count = 0;
		//ǳ�� ���� �б�
		if( _tcsicmp( def_str, _T("BALLOON_TYPE") ) == 0 )
		{
			_fgetts( line, 256, fp );
			_stscanf( line, _T("%s"), def_str );

			if( _tcsicmp( def_str, _T("{") ) == 0 )
			{
				while( _tcsicmp( def_str, _T("}") ) != 0 )
				{
					_fgetts( line, 256, fp );
					_stscanf( line, _T("%s %d %d %d %d %d %d %d %d %d %d %d %d"),
									def_str, &Border[count].width,
									&Border[count].north, &Border[count].north_east, &Border[count].east, &Border[count].south_east,
									&Border[count].south, &Border[count].south_west, &Border[count].west, &Border[count].north_west,
									&Border[count].tail, &Border[count].tail_w, &Border[count].tail_h );

					count++;
				}
			}
		}
	}

	fclose( fp );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFInterface.OpenFile( "CBT.txt" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!CBT ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	char	string[512];			// ���� �ٴ��� ���� ����
	bool	bRead  = true;			// ���� �� �о����� üũ
	TCHAR	def_str[40] = "";
	int		count = 0;


	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return; 
	}		
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		_stscanf( string, _T("%s"), def_str );

		//Ŀ�� ���� �б�
		if( _tcsicmp( def_str, _T("CURSOR_TYPE") ) == 0 )
		{
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
			_stscanf( string, _T("%s"), def_str );

			if( _tcsicmp( def_str, _T("{") ) == 0 )
			{
				while( _tcsicmp( def_str, _T("}") ) != 0 )
				{
					if( pFH->Gets( string, sizeof( string ) ) == 0 )
					{
						bRead = false;
						break;
					}
					_stscanf( string, _T("%s %d %d %d %d %d %d"), def_str, &Cursor[count].img[0], &Cursor[count].img[1], &Cursor[count].img[2], &Cursor[count].img[3], &Cursor[count].img[4], &Cursor[count].img[5] );
					count++;
				}
			}
		}

		count = 0;
		//ǳ�� ���� �б�
		if( _tcsicmp( def_str, _T("BALLOON_TYPE") ) == 0 )
		{
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
			_stscanf( string, _T("%s"), def_str );

			if( _tcsicmp( def_str, _T("{") ) == 0 )
			{
				while( _tcsicmp( def_str, _T("}") ) != 0 )
				{
					if( pFH->Gets( string, sizeof( string ) ) == 0 )
					{
						bRead = false;
						break;
					}
					_stscanf( string, _T("%s %d %d %d %d %d %d %d %d %d %d %d %d"),
									def_str, &Border[count].width,
									&Border[count].north, &Border[count].north_east, &Border[count].east, &Border[count].south_east,
									&Border[count].south, &Border[count].south_west, &Border[count].west, &Border[count].north_west,
									&Border[count].tail, &Border[count].tail_w, &Border[count].tail_h );

					count++;
				}
			}
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			

	g_VFInterface.CloseFile( pFH );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


}

//-----------------------------------------------------------------------------
void CResource::LoadSlangFile()
{
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE* fp;

	fp = _tfopen( _T("Interface\\Slang.txt"), _T("rt") );//VSS�ø���...�ɼ� ����(�б�����)�϶�....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!Slang ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	TCHAR	line[256]		= "";	
	TCHAR	def_str[40]		= "";
	TCHAR	imsi[10]		= "";

	int		count = 0;

	while( !feof( fp ) )
	{
		_fgetts( line, 256, fp );
		_stscanf( line, _T("%s"), def_str );
		
		if( _tcsicmp( def_str, _T("{") ) == 0 )
		{
			while( 1 )
			{
				_fgetts( line, 256, fp );
				_stscanf( line, _T("%s"), def_str );

				if( _tcsicmp( def_str, _T("}") ) != 0 )
				{
					_stscanf( line, _T("%s %s %s"), FilteringList.FWord[count].Slang, imsi, FilteringList.FWord[count].Standard );
				
					count++;
					FilteringList.Num = count;
					if( count >= 4096 )
					{
						MessageBox( NULL, _T("!Slang�ִ� ������ �Ѿ����ϴ�."), _T("�˸�"), MB_OK );
						fclose( fp );
						return;
					}
				}
				else
				{
					break;
				}
				
			}
		}
	}

	fclose( fp );
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFInterface.OpenFile( "Slang.txt" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!Slang ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return;
	}

	char	string [512];			// ���� �ٴ��� ���� ����
	bool	bRead  = true;			// ���� �� �о����� üũ		
	TCHAR	def_str[40]		= "";
	TCHAR	imsi[10]		= "";

	int		count = 0;

	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return; 
	}		
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}

		_stscanf( string, _T("%s"), def_str );

		if( _tcsicmp( def_str, _T("{") ) == 0 )
		{
			while( 1 )
			{
				if( pFH->Gets( string, sizeof( string ) ) == 0 )
				{
					bRead = false;
					break;
				}
				_stscanf( string, _T("%s"), def_str );

				if( _tcsicmp( def_str, _T("}") ) != 0 )
				{
					_stscanf( string, _T("%s %s %s"), FilteringList.FWord[count].Slang, imsi, FilteringList.FWord[count].Standard );
				
					count++;
					FilteringList.Num = count;
					if( count >= 4096 )
					{
						MessageBox( NULL, _T("!Slang�ִ� ������ �Ѿ����ϴ�."), _T("�˸�"), MB_OK );
						g_VFInterface.CloseFile( pFH );
						return;
					}
				}
				else
				{
					break;
				}
				
			}
		}
				
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			

	g_VFInterface.CloseFile( pFH );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
*/

}

//-----------------------------------------------------------------------------
void CResource::SetTextureImageInfo( SImage* Img, int ImgId, int Tid, int X, int Y, int W, int H )
{
	///-- Critical Error Check : Array Index 
	if( Tid >= MAX_TEX_AMOUNT )
	{
		__asm int 3
	}


	assert( Tid < MAX_TEX_AMOUNT );

	// �̹� �ε��� �̹����� ������ Tid �߸� ������ By wxywxy
	if( NULL != Tex[Tid]  )
	{
		char StrMsg[ 256 ] = "";
		
		wsprintf( StrMsg , "CResource::SetTextureImageInfo -> Tid : %d" , Tid );

		MessageBox( NULL , StrMsg , "!�ߺ��� Tid �߰�" , MB_OK );
	}
	

	Tex[Tid] = SAFE_NEW( STexture );	
	Tex[Tid]->ImgId = ImgId;
	Tex[Tid]->pTexture = Img->pTexture;

	Tex[Tid]->x = X;
	Tex[Tid]->y = Y;
	Tex[Tid]->w = W;
	Tex[Tid]->h = H;
	
	Tex[Tid]->vt[0].x = (float)X;
	Tex[Tid]->vt[0].y = (float)Y;
	Tex[Tid]->vt[0].z = 0.0f;
	Tex[Tid]->vt[0].w = 1.0f;
	Tex[Tid]->vt[0].diffuse = 0xff000000;

	Tex[Tid]->vt[1].x = Tex[Tid]->vt[0].x + W;
	Tex[Tid]->vt[1].y = Tex[Tid]->vt[0].y;
	Tex[Tid]->vt[1].z = 0.0f;
	Tex[Tid]->vt[1].w = 1.0f;
	Tex[Tid]->vt[1].diffuse = 0xff000000;

	Tex[Tid]->vt[2].x = Tex[Tid]->vt[0].x + W;
	Tex[Tid]->vt[2].y = Tex[Tid]->vt[0].y + H;
	Tex[Tid]->vt[2].z = 0.0f;
	Tex[Tid]->vt[2].w = 1.0f;
	Tex[Tid]->vt[2].diffuse = 0xff000000;

	Tex[Tid]->vt[3].x = Tex[Tid]->vt[0].x;
	Tex[Tid]->vt[3].y = Tex[Tid]->vt[0].y + H;
	Tex[Tid]->vt[3].z = 0.0f;
	Tex[Tid]->vt[3].w = 1.0f;
	Tex[Tid]->vt[3].diffuse = 0xff000000;


    //��ġ�� ũ�⸦ �̿��ؼ� Image�� UV��ǥ�� ����Ѵ�.
	Tex[Tid]->vt[0].tu = Tex[Tid]->vt[0].x / Img->Width;
	Tex[Tid]->vt[0].tv = Tex[Tid]->vt[0].y / Img->Height;

	Tex[Tid]->vt[1].tu = Tex[Tid]->vt[1].x / Img->Width;
	Tex[Tid]->vt[1].tv = Tex[Tid]->vt[1].y / Img->Height;

	Tex[Tid]->vt[2].tu = Tex[Tid]->vt[2].x / Img->Width;
	Tex[Tid]->vt[2].tv = Tex[Tid]->vt[2].y / Img->Height;

	Tex[Tid]->vt[3].tu = Tex[Tid]->vt[3].x / Img->Width;
	Tex[Tid]->vt[3].tv = Tex[Tid]->vt[3].y / Img->Height;
}

//-----------------------------------------------------------------------------
void CResource::SetTextureFontInfo( SImage* Img, int ImgId, int Tid, int X, int Y, int W, int H )
{
	Img;
	ImgId;
	Tid;
	X;
	Y;
	W;
	H;
}

//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
bool CResource::LoadTexture( LPDIRECT3DTEXTURE9* ppTexture, const TCHAR* FileName, 
#else
bool CResource::LoadTexture( LPDIRECT3DTEXTURE8* ppTexture, const TCHAR* FileName, 
#endif // DIRECT_VERSION_9_MJH
					bool bUseTransparency, D3DCOLOR TransparencyColor )
{
	D3DCOLOR transparencycolor;
	

	if(*ppTexture != NULL)
	{
		int a = 0;
	}

	///-- Memory Error Check : Reallocation
	assert( *ppTexture==NULL );
	
	if( bUseTransparency )
	{
		transparencycolor = TransparencyColor | 0xff000000;
	}
	else
	{
		transparencycolor = 0;
	}
	
    // ������� �ش��ϴ� ������ ���İ��� 0���� ����� �ش�.
	if( theD3DDevice == NULL ){ return( false );	}
		
	D3DXIMAGE_INFO imageinfo;
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if( FAILED( D3DXGetImageInfoFromFile( FileName, &imageinfo ) ) )
    {       
		char strError[255] = {0, };
		sprintf(strError, "!���� %s �ؽ��� �б� ����", FileName);
		MessageBox( NULL, strError, _T("!����"), MB_OK );
		return( false );
    }

    switch( imageinfo.ResourceType )
    {
    case D3DRTYPE_TEXTURE:
        if( FAILED( g_RenderManager.CreateTextureFromFileEx( FileName, 
            imageinfo.Width, imageinfo.Height, imageinfo.MipLevels, 0,
            imageinfo.Format, D3DPOOL_MANAGED, 
			D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
			transparencycolor, NULL, NULL, ppTexture ) ) )
		{
			char strError[255] = {0, };
			sprintf(strError, "!���� %s �ؽ��� �б� ����", FileName);
			MessageBox( NULL, strError, _T("!����"), MB_OK );
			return( false );
		}
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFInterface.OpenFile( FileName );	
	if( pFH )
	{
		if( FAILED( D3DXGetImageInfoFromFileInMemory( pFH->GetData(), pFH->GetSize(), &imageinfo ) ) )
		{       
			char strError[255] = {0, };
			sprintf(strError, "!���� %s �ؽ��� �б� ����", FileName);
			MessageBox( NULL, strError, _T("!����"), MB_OK );
			return( false );
		}		
		
		switch( imageinfo.ResourceType )
		{
		case D3DRTYPE_TEXTURE:
			if( FAILED( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// ������ ������, ������ 
																		imageinfo.Width, imageinfo.Height, 1, 0,
																		// imageinfo.Width, imageinfo.Height, imageinfo.MipLevels, 0,
																		// imageinfo.Width, imageinfo.Height, D3DX_DEFAULT, 0,
																		// D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
																		imageinfo.Format, D3DPOOL_MANAGED, 
																		D3DX_FILTER_LINEAR, D3DX_DEFAULT, 
																		//transparencycolor, NULL, NULL, ppTexture ) ) ) // TGA
																		0, NULL, NULL, ppTexture ) ) )				// DDS
			{
				char strError[255] = {0, };
				sprintf(strError, "!���� %s �ؽ��� �б� ����", FileName);
				MessageBox( NULL, strError, _T("!����"), MB_OK );
				return( false );
			}
		}		
		g_VFInterface.CloseFile( pFH );	
	}
	///-- �ش� File �� rvf���� ã�� ������ ���� �޼���
	else
	{	
		char strError[255] = {0, };
		sprintf(strError, "! Error : File Not Found %s " , FileName);
		MessageBox( NULL, strError, _T("!����"), MB_OK );
		//RLG1( " !!!! Interface.rvf Data Missing !!!! : File not found - %s \n", FileName );
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	return( true );
}

//-----------------------------------------------------------------------------
