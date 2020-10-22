#include	"RockPCH.h"

#include	"brother.h"
#include	"RockClient.h"
#include	"RockInterface\\Rui.h"
#include	"RockInterface\\SysMsgWndProc.h"
#include	"RockInterface\\SystemPrintWnd.h"

LPDWORD			g_pTestDest; 
void putpixel( int x, int y, DWORD Color, int size )   
{ 
//	if( x >= 32 && y >= 32 )
//		int a = 0;
//	BOOL fl = Flag;

	*(g_pTestDest+(y*size*2)+x)	= Color;
}

void line_Bresenham_X_plus(int x,int y,int dx,int dy,DWORD color, int size)
{
   int twoDx=dx+dx,twoDy=abs(dy+dy),twoDyDx=twoDy-twoDx;
   int p=twoDy-dx;
   int xEnd=x+dx; 

   putpixel(x,y,color, size);
   if(dy<0)
   {
     while(x<xEnd)
     {
        x++;
        if(p<0) p+=twoDy;
        else
        {  y--; p+=twoDyDx; }
        putpixel(x,y,color, size);
     }
   }
   else
   {
     while(x<xEnd)
     {
        x++;
        if(p<0) p+=twoDy;
        else
        {  y++; p+=twoDyDx; }
        putpixel(x,y,color, size);
     }
   }
}
void line_Bresenham_X_minus(int x,int y,int dx,int dy,DWORD color, int size)
{
   int twoDx=abs(dx+dx),twoDy=abs(dy+dy),twoDyDx=twoDy-twoDx;
   int p=twoDy+dx;
   int xEnd=x+dx;

   putpixel(x,y,color, size);
   if(dy<0)
   {
     while(x>xEnd)
     {
        x--;
        if(p<0) p+=twoDy;
        else
        {  y--; p+=twoDyDx; }
        putpixel(x,y,color,size);
     }
   }
   else
   {
     while(x>xEnd)
     {
        x--;
        if(p<0) p+=twoDy;
        else
        {  y++; p+=twoDyDx; }
        putpixel(x,y,color, size);
     }
   }
}

void line_Bresenham_Y_plus(int x,int y,int dx,int dy,DWORD color, int size)
{
   int twoDx=abs(dx+dx),twoDy=dy+dy,twoDxDy=twoDx-twoDy;
   int p=twoDx-dy;
   int yEnd=y+dy;

   putpixel(x,y,color,size);
   if(dx<0)
   {
     while(y<yEnd)
     {
        y++;
        if(p<0) p+=twoDx;
        else
        {  x--; p+=twoDxDy; }
        putpixel(x,y,color,size);
     }
   }
   else
   {
     while(y<yEnd)
     {
        y++;
        if(p<0) p+=twoDx;
        else
        {  x++; p+=twoDxDy; }
        putpixel(x,y,color,size);
     }
   }
}

void line_Bresenham_Y_minus(int x,int y,int dx,int dy,DWORD color,int size)
{
   int twoDx=abs(dx+dx),twoDy=abs(dy+dy),twoDxDy=twoDx-twoDy;
   int p=twoDx+dy;
   int yEnd=y+dy;

   putpixel(x,y,color,size);
   if(dx<0)
   {
     while(y>yEnd)
     {
        y--;
        if(p<0) p+=twoDx;
        else
        {  x--; p+=twoDxDy; }
        putpixel(x,y,color,size);
     }
   }
   else
   {
     while(y>yEnd)
     {
        y--;
        if(p<0) p+=twoDx;
        else
        {  x++; p+=twoDxDy; }
        putpixel(x,y,color,size);
     }
   }
}

//
// x1,y1 시작점
// x2,y2 끝나는점
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void line_Bresenham( int x1,int y1,int x2,int y2,DWORD color, int size)
{
   int dx=x2-x1,dy=y2-y1;
   
   if(abs(dy)>abs(dx))
   {
      if(dy<0) line_Bresenham_Y_minus(x1,y1,dx,dy,color, size);
      else     line_Bresenham_Y_plus (x1,y1,dx,dy,color, size);
   }
   else
   {
      if(dx<0) line_Bresenham_X_minus(x1,y1,dx,dy,color, size);
      else     line_Bresenham_X_plus (x1,y1,dx,dy,color, size);
   }
}
//============================================================


// 화일 개수를 셉시다.
int	GetFileCount( char* file_pos, char* file_ext )
{
	// 모델파일이 있는 경로 만들기	
	char file_path[ 256 ] = {0,};
	strcpy( file_path, file_pos);
	strcat( file_path, file_ext);	
	
	// 파일 찾기	
	WIN32_FIND_DATA	fild_data;
	HANDLE hFindFile = FindFirstFile( file_path, &fild_data );
	if( hFindFile == INVALID_HANDLE_VALUE )	return 0;

	int count	= 0;
	int Find	= -1;

	while( Find != 0 )
	{			
		Find = FindNextFile( hFindFile, &fild_data );
		count++;
	}					

	FindClose( hFindFile );

	return count;
}


//큐빅커브함수.
void CalcNaturalCubic(float get_abcd[][4],int n, float *vptr)
{
	float gamma[1000];
	float delta[1000];
	float D[1000];
	int i;
	typedef struct{
		float r;
		float no_use[2];
	}_X;
	_X *x=(_X *)vptr;

	gamma[0] = 1.0f/2.0f;
	for(i=1; i<n; i++)
	{
		gamma[i] = 1/(4-gamma[i-1]);
	}
	gamma[n] = 1/(2-gamma[n-1]);
	
	delta[0] = 3*(x[1].r-x[0].r)*gamma[0];
	for(i=1; i<n; i++)
	{
		delta[i] = (3*(x[i+1].r-x[i-1].r)-delta[i-1])*gamma[i];
	}
	delta[n] = (3*(x[n].r-x[n-1].r)-delta[n-1])*gamma[n];
	
	D[n] = delta[n];
	for(i=n-1; i>=0; i--)
	{
		D[i] = delta[i] - gamma[i]*D[i+1];
	}

	for(i=0; i<n; i++)
	{
		get_abcd[i][0] = (float)x[i].r;
		get_abcd[i][1] = (float)D[i];
		get_abcd[i][2] = (float)3*(x[i+1].r - x[i].r) - 2*D[i] - D[i+1];
		get_abcd[i][3] = (2.0f*(x[i].r - x[i+1].r) + D[i] + D[i+1]);
	}
}

//................................................................................................................
// 파일 이름으로부터 _이후에 인덱스 값을 읽는다 ( XXXX_123.XXX = 123 )	by wxywxy
//................................................................................................................
int GetIndexKeyInFileName(char *pName)
{
	char Str   [ 1024 ] = "";
	char StrNum[ 1024 ] = "";
	char *pFindChar;

	pFindChar = strrchr( pName , '_' );

	if( pFindChar == NULL ) 
	{
		memcpy( Str , pName , strlen( pName ) );
	}
	else
	{
		memcpy( Str , ( pFindChar + 1 ) , strlen( pName ) - ( ( pFindChar - pName ) ) );
	}


	pFindChar = strrchr( Str , '.' );

	memcpy( StrNum , Str , ( pFindChar - Str ) );							// 확장자 빼기

	int Index = -1;

	Index = atoi( StrNum );

	return Index;
}

//................................................................................................................
// 화일 이름으로부터 시작점 위치를 알려준 이후에 인덱스 값을 읽는다 ( XXXX_123.XXX = 123 )	by wxywxy
//................................................................................................................
int  GetIndexKeyInFileName(char *pName , char *pStartStr)
{
	char Str   [ 1024 ] = "";
	char StrNum[ 1024 ] = "";
	char *pFindChar;

	pFindChar = strrchr( pName , *pStartStr );

	if( pFindChar == NULL ) 
	{
		memcpy( Str , pName , strlen( pName ) );
	}
	else
	{
		memcpy( Str , ( pFindChar + 1 ) , strlen( pName ) - ( ( pFindChar - pName ) ) );
	}


	pFindChar = strrchr( Str , '.' );

	memcpy( StrNum , Str , ( pFindChar - Str ) );							// 확장자 빼기

	int Index = -1;

	Index = atoi( StrNum );

	return Index;
}



//................................................................................................................
// 시스템 메세지 찍기 
//................................................................................................................
void SystemPrint( int aColor, NSysMsgType aType, void *pStr, ... )
{
//	return;

	static char		sbuffer[1024] = "";
	va_list		strlist;

#ifdef UNICODE_ROCK

	wchar_t *pwstr = (WCHAR*)(pStr);
	va_start( strlist, pStr );
	vswprintf( RWCHAR(sbuffer), pwstr , strlist );
	va_end( strlist );

#else

	va_start( strlist, pStr );
	vsprintf( sbuffer, (char *)pStr, strlist );	
	va_end( strlist );

#endif
	
	switch( aType )
	{
	case n_SystemMsg:
		{
			g_SystemPrintWnd.InsertSystemMsg((TCHAR *)sbuffer, (UINT)aColor);
			//SysMsgOutputWnd.SysMsgTxt->SetSystemMsg( buffer, sysColor );
		}
		break;
	case n_AdminMsg:
		{
			//SysMsgOutputWnd.AdminMsgTxt->SetSystemMsg( buffer, sysColor );
		}
		break;
	case n_MapNameMsg:
		{
			//SysMsgOutputWnd.MapNameMsgTxt->SetSystemMsg( buffer, sysColor );
		}
		break;	
	default:
		break;
	}

	return;
}


/// sooree pack
void GetFileNameOnly( char* pFullName, char* pOutStr )
{
	strcpy( pOutStr, pFullName); 
	char* pEnd = &( pOutStr[ lstrlen( pOutStr ) ] );
	while( pEnd != pOutStr )		/// 뒤에서부터 뒤져서 
	{
		if( (*pEnd) == '.' ) /// 처음 . 이 나오는 위치 
		{
			*pEnd = '\0';
			break;
		}
		--pEnd;
	}
}

/// sooree pack(경로에서 확장자 가져오는 함수)
// (루프를 이용하는 것보단 C함수 _splitpath()를 이용하는 wrapper 함수를 만드는게 더 좋을듯)
void GetFileExt( char* pFullName, char* pOutStr )
{
	pOutStr[0] = '\0';
	char* pStart = pFullName;
	char* pEnd = &( pFullName[ lstrlen( pFullName ) ] );
	while( pEnd != pStart )			/// 뒤에서부터 뒤져서 
	{
		if( (*pEnd) == '.' ) break;	/// 처음 . 이 나오는 위치 
		--pEnd;
	}
	
	if( pEnd != pStart )
	{
		strcpy( pOutStr, ++pEnd ); /// 확장자 카피 
	}
}

int RandomNum( int iMin, int iMax )
{
	if( iMin == iMax )
		return( iMin );

	return ( ( rand() % ( abs(iMax-iMin)+1 ) ) + iMin );
}

float RandomNum( float fMin, float fMax )
{
	if ( fMin == fMax ) 
		return( fMin );

	float fRandom = (float)rand() / (float)RAND_MAX;

	return( ( fRandom * (float)fabs(fMax-fMin) ) + fMin );
}

void CloseWnd_Error( char* aErrorStr, ... )
{
	char		buffer[ 1024 ];
	va_list		strlist;

	va_start( strlist, aErrorStr );
	_vstprintf( buffer, aErrorStr, strlist );
	va_end( strlist );
	
	MessageBox( NULL, buffer, "Error", MB_OK );

	ClientClose( g_RockClient.GetApp() , 1);

	return;
}
