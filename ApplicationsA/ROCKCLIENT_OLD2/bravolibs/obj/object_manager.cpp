#include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	<BASE\\D3DBASE.H>
#include	"..\\quadlist.h"
#include	<Map\Field.h>
#include	"..\\RockClient.h"
#include	"..\\Map.H"
#include	"..\\Pc.H"
#include	"Object_Manager.H"
#include	<assert.H>
#include	<stdio.h>
#include	"profile\profile.h"
#include	"profile\custom_time.h"

extern	int		g_Shadow;
extern	long	g_nowTime;
extern	FILE*	g_LogFile;
 
extern BOOL		g_ISObjTexLoad;
//-----------------------------------------------------------------------------
// Desc: Object_Manager클래스 초기화, 파괴자
//-----------------------------------------------------------------------------

Object_Manager::Object_Manager()
{
	memset( m_Mod, 0, sizeof(Model) * MAX_OBJ_MODCOUNT );
	memset( m_Tex, 0, sizeof(Texture) * MAX_OBJ_TEXCOUNT );
	memset( m_Ani, 0, sizeof(Bone) * MAX_OBJ_ANICOUNT );

	m_nCurtSet = 0;

	// 캐릭터 선택화면에서 사용	
	m_TempRot  = 0.0f;
	m_TempTime = g_nowTime;
	D3DXMatrixIdentity( &m_matTempRot );
}

Object_Manager::~Object_Manager()
{
	Clear();
}

void Object_Manager::InitLockState(void)
{
	for( int i = 0 ; i < MAX_OBJ_MODCOUNT ; ++i )
	{
		m_Mod[ i ].m_ISOneTimeLock = FALSE;
	}
}

void Object_Manager::Clear()
{
	ClearObj();	/// 이거 안하면 텍스쳐 메모리 샐 수 있습니다. by sooree

	m_nCurtSet = 0;

	// 캐릭터 선택화면에서 사용	
	m_TempRot  = 0.0f;
	m_TempTime = g_nowTime;
	D3DXMatrixIdentity( &m_matTempRot );	
}

void Object_Manager::Destroy()
{
	Clear();
}

void Object_Manager::ClearObj()
{
	int i = 0;
	for( i = 0; i < MAX_OBJ_MODCOUNT; ++i )
	{
		m_Mod[i].Destory();
	}
	for( i = 0; i < MAX_OBJ_TEXCOUNT; ++i )
	{
		m_Tex[i].Destory();
	}
	for( i = 0; i < MAX_OBJ_ANICOUNT; ++i )
	{
		m_Ani[i].Destory();
	}
	
	memset( m_Mod, 0, sizeof(Model) * MAX_OBJ_MODCOUNT );
	memset( m_Tex, 0, sizeof(Texture) * MAX_OBJ_TEXCOUNT );
	memset( m_Ani, 0, sizeof(Bone) * MAX_OBJ_ANICOUNT );
}

//-----------------------------------------------------------------------------
// Desc: LOD RANGE
//-----------------------------------------------------------------------------
void Object_Manager::SetLodRange( int range )
{
	switch( range )
	{
	case 0: m_LodRange[0] = 550;	m_LodRange[1] = 700;	m_LodRange[2] = 750;	break;
	case 1: m_LodRange[0] = 650;	m_LodRange[1] = 1000;	m_LodRange[2] = 1100;	break;
	case 2:	m_LodRange[0] = 850;	m_LodRange[1] = 1300;	m_LodRange[2] = 1500;	break;
	case 3: m_LodRange[0] = 1000;	m_LodRange[1] = 1700;	m_LodRange[2] = 1800;	break;
	default: m_LodRange[0] = 850;	m_LodRange[1] = 1300;	m_LodRange[2] = 1500;	break;
	}

	return;
}

void Object_Manager::UpdateLodRange()
{
	return;
}


void Object_Manager::InvalidateDeviceObjects()
{
	// 선택화면 모델
	for( int i = 0; i < SELECTOBJ_MODCOUNT; ++i )	
		m_TempMod[i].Destory();	
	m_TempTex.Destory();
	
	// 마우스 포인터 모델
	for(int i = 0; i < MOUSEPOINT_MODCOUNT; ++i )	
		m_PointMod[i].Destory();
	m_PointTex.Destory();
}


void Object_Manager::RestoreDeviceObjects()
{
	/*
	// 선택화면 모델
	char	file_path[256] = {0,};
	for( int i = 0; i < SELECTOBJ_MODCOUNT; ++i )
	{
		sprintf( file_path, "%s\\Etc\\SelectMap\\%04d.msh", g_RockClient.GetRootDir(), i ); 
		if( m_TempMod[i].Read( file_path, true ) == false )
			MessageBox( NULL, "!캐릭터 선택화면에 사용할 모델없음", "", MB_OK );
	}
	sprintf( file_path, "%s\\Etc\\SelectMap\\0000.brv_tex", g_RockClient.GetRootDir() );
	if( m_TempTex.Read( file_path, true ) == false )
		MessageBox( NULL, "!캐릭터 선택화면에 사용할 텍스쳐없음", "", MB_OK );
	
	// 모델로딩
	for( i = 0; i < MOUSEPOINT_MODCOUNT; ++i )
	{
		sprintf( file_path, "%s\\Etc\\SelectPoint\\%04d.msh", g_RockClient.GetRootDir(), i ); 
		if( m_PointMod[i].Read( file_path, true ) == false )
			MessageBox( NULL, "!마우스포인터에 사용할 모델없음", "", MB_OK );
	}	
	sprintf( file_path, "%s\\Etc\\SelectPoint\\0000.brv_tex", g_RockClient.GetRootDir() ); 
	if( m_PointTex.Read( file_path, true ) == false )		
		MessageBox( NULL, "!마우스포인터에 사용할 텍스쳐없음", "", MB_OK );
	*/
}


//-----------------------------------------------------------------------------
// Desc: 오브젝트에 관련된 파일 메모리에 할당(모델,텍스쳐,애니메이션)
//-----------------------------------------------------------------------------

bool Object_Manager::Create( char* file_pos, bool bTempObj )
{	
	//ProfileBegin( "< Object_Manager::Create >" );
	char	file_path[256] = {0,};

	//-----------------------------------------------------------------------------
	// 캐릭터 선택화면에 사용할 오브젝트
	//-----------------------------------------------------------------------------
	if( bTempObj == true )
	{
		// 모델로딩		
/*		for( int i = 0; i < SELECTOBJ_MODCOUNT; ++i )
		{
			sprintf( file_path, "%s\\Etc\\SelectMap\\%04d.msh", file_pos, i ); 
			if( m_TempMod[i].Read( file_path, true ) == false )
			{
				#ifdef GAMETYPE_TEST
					MessageBox( NULL, "!캐릭터 선택화면에 사용할 모델없음", "", MB_OK );
				#endif
			}
		}
					
		// 텍스쳐로딩		
		sprintf( file_path, "%s\\Etc\\SelectMap\\0000.brv_tex", file_pos ); 
		if( m_TempTex.Read( file_path, true ) == false )
		{
			#ifdef GAMETYPE_TEST
				MessageBox( NULL, "!캐릭터 선택화면에 사용할 텍스쳐없음", "", MB_OK );
			#endif
		}

		// 오브젝트로딩		
		sprintf( file_path, "%s\\Etc\\SelectMap\\0000.brv_obj", file_pos ); 
		g_Map.m_TempObj.m_child_count = 1;			
		if( g_Map.m_TempObj.File__Open( file_path ) == false )
		{
			#ifdef GAMETYPE_TEST
				MessageBox( NULL, "!캐릭터 선택화면에 사용할 오브젝트없음", "", MB_OK );	
			#endif
		}

		D3DXMATRIX mat = g_Map.m_TempObj.m_pAniPos[0].GetPosTM();
		mat._41 = 5.0f;
		g_Map.m_TempObj.m_pAniPos[0].SetPosTM(mat);		
		OutputDebugString( "!\n\n캐릭터 선택화면에 사용할 오브젝트 로딩" );

		//-----------------------------------------------------------------------------
		// 마우스 포인터에 사용할 오브젝트
		//-----------------------------------------------------------------------------
		// 모델로딩
		for( i = 0; i < MOUSEPOINT_MODCOUNT; ++i )
		{
			sprintf( file_path, "%s\\Etc\\SelectPoint\\%04d.msh", file_pos, i ); 
			if( m_PointMod[i].Read( file_path, true ) == false )
				MessageBox( NULL, "!마우스포인터에 사용할 모델없음", "", MB_OK );
		}
					
		// 텍스쳐로딩
		sprintf( file_path, "%s\\Etc\\SelectPoint\\0000.brv_tex", file_pos ); 
		if( m_PointTex.Read( file_path, true ) == false )		
			MessageBox( NULL, "!마우스포인터에 사용할 텍스쳐없음", "", MB_OK );

		// 오브젝트로딩		
		sprintf( file_path, "%s\\Etc\\SelectPoint\\0000.brv_obj", file_pos ); 				
		if( g_Map.m_PointObj.File__Open( file_path ) == false )
			MessageBox( NULL, "!마우스포인터에 사용할 오브젝트없음", "", MB_OK );
		g_Map.m_PointObj.m_dwIndex = MOUSEPOINT_INDEX;
		OutputDebugString( "!\n\n마우스 포인터에 사용할 오브젝트 로딩" );			
		return;
		*/
	}

	//-----------------------------------------------------------------------------
	// 셋트 속성 로딩, 현재 맵에 해당하는 셋트 로딩
	//-----------------------------------------------------------------------------
///	SetCreate( file_pos );	/// by sooree : lod set 정보는 처음 뜰 때 한번만 읽습니다. 
	GetNowSet();

#if !defined( LOAD_VF )

	WIN32_FIND_DATA fileFindData;
	HANDLE			hFileFind;
	BOOL			bFind;

	char			buf[256] = {0,};
	char*			c;
	int				temp;

	//---------------------------------------------------------------
	// 모델 파일 메모리에 올리기 by sooree	
	//---------------------------------------------------------------
	bFind = TRUE;
	temp = 0;
	sprintf( file_path, "%s\\Data\\*.msh", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			sprintf( file_path, "%s\\Data\\%s", file_pos, fileFindData.cFileName );
			if( m_Mod[temp].Read( file_path, true ) == true )
			{		
				strcpy( buf, fileFindData.cFileName);
				c = strchr( buf, '.' );	/// 파일명에서 확장자 떼고 숫자 부분만 추려냅니다. 
				*c = NULL;
				m_Mod[temp].m_Index = atoi( buf );				
				temp++;
				assert( temp < MAX_OBJ_MODCOUNT );
			}
			else
			{
				return false;
			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}

	//---------------------------------------------------------------
	// 텍스쳐 파일 메모리에 올리기 by sooree	
	//---------------------------------------------------------------
	bFind = TRUE;
	temp = 0;
//	sprintf( file_path, "%s\\Data\\*.tex", file_pos );
	sprintf( file_path, "%s\\Data\\*.dds", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			sprintf( file_path, "%s\\Data\\%s", file_pos, fileFindData.cFileName );
			//if( m_Tex[temp].Read( file_path, n_Texture ) == true )
			if( m_Tex[temp].Read( file_path, n_Texture , !g_ISObjTexLoad ) == true )
			{		
				strcpy( buf, fileFindData.cFileName);
				c = strchr( buf, '.' );	/// 파일명에서 확장자 떼고 숫자 부분만 추려냅니다. 
				*c = NULL;
				m_Tex[temp].m_Index = atoi( buf ) ;
				m_Tex[temp].m_byAttr = DATA_NON_DELETE;							
				temp++;
				assert( temp < MAX_OBJ_TEXCOUNT );
			}
			else
			{
				return false;
			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}

	//---------------------------------------------------------------
	// 애니메이션 파일 메모리에 올리기 by sooree	
	//---------------------------------------------------------------
	bFind = TRUE;
	temp = 0;
	sprintf( file_path, "%sj\\Data\\*.ani", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			sprintf( file_path, "%s\\Data\\%s", file_pos, fileFindData.cFileName );
			m_Ani[temp].Read( file_path );
			if( m_Ani[temp].Read( file_path ) == true )
			{		
				strcpy( buf, fileFindData.cFileName);
				c = strchr( buf, '.' );	/// 파일명에서 확장자 떼고 숫자 부분만 추려냅니다. 
				*c = NULL;
				m_Ani[temp].m_Index = atoi( buf ) ;				
				temp++;
				assert( temp < MAX_OBJ_ANICOUNT );				
			}
			else
			{
				return false;
			}

		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}

#else
	////////////////////////////////////////////////////////////////////////////////
	///			read from virtual filesystem	/// sooree pack
	////////////////////////////////////////////////////////////////////////////////

	char buf[256] = {0,};
	int	cnt_msh = 0;
	int	cnt_tex = 0;
	int	cnt_ani = 0;

	VFileHandleList* pFHL;
	VFileHandleList_i it;

	g_VFMap.ChangeDir( "/obj/data" );

	pFHL = g_VFMap.GetFileHandleList();

	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, buf );
			
			// 모델 파일
			if( lstrcmp( buf, "msh" ) == 0 )	/// 확장자가 msh 파일인 넘만 읽는다 
			{				
				ProfileBegin( "m_Mod[cnt_msh].ReadVF" );
				///-- Critical Error Check : Array Index
				assert( cnt_msh < MAX_OBJ_MODCOUNT ); 
				if( m_Mod[cnt_msh].ReadVF( &g_VFMap, (*it)->GetFileContext()->strName, true ) )
				{	
					GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
					m_Mod[cnt_msh].m_Index = atoi( buf ) ;
					++cnt_msh;
					assert( cnt_msh < MAX_OBJ_MODCOUNT );
				}
				else
				{
					ProfileEnd( "m_Mod[cnt_msh].ReadVF" );
					return false;
				}
				ProfileEnd( "m_Mod[cnt_msh].ReadVF" );
			}
			// 텍스쳐 파일
			else if( lstrcmp( buf, "dds" ) == 0 )	/// 확장자가 dds 파일인 넘만 읽는다 
			{
				ProfileBegin( "m_Tex[cnt_tex].ReadVF" );
				///-- Critical Error Check : Array Index
				assert( cnt_tex < MAX_OBJ_TEXCOUNT ); 
//				if( m_Tex[cnt_tex].ReadVF( &g_VFMap, (*it)->GetFileContext()->strName, n_Texture, l_nDevSkip ) )
				if( m_Tex[cnt_tex].ReadVF( &g_VFMap, (*it)->GetFileContext()->strName, n_Texture, !g_ISObjTexLoad ) )
				{	
					GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
					m_Tex[cnt_tex].m_Index = atoi( buf );
					m_Tex[cnt_tex].m_byAttr = DATA_NON_DELETE;	
					++cnt_tex;
					assert( cnt_tex < MAX_OBJ_TEXCOUNT );
				}
				else
				{
					ProfileEnd( "m_Tex[cnt_tex].ReadVF" );
					return false;
				}
				ProfileEnd( "m_Tex[cnt_tex].ReadVF" );
			}
			// 애니메이션 파일
			else if( lstrcmp( buf, "ani" ) == 0 )	/// 확장자가 ani 파일인 넘만 읽는다 
			{
				ProfileBegin( "m_Ani[cnt_ani].ReadVF" );
				
				///-- Critical Error Check : Array Index
				assert( cnt_ani < MAX_OBJ_ANICOUNT ); 

				if( m_Ani[cnt_ani].ReadVF( &g_VFMap, (*it)->GetFileContext()->strName ) )
				{	
					GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
					m_Ani[cnt_ani].m_Index = atoi( buf ) ;
					++cnt_ani;
					assert( cnt_ani < MAX_OBJ_ANICOUNT );
				}
				else
				{
					ProfileEnd( "m_Ani[cnt_ani].ReadVF" );
					return false;
				}
				ProfileEnd( "m_Ani[cnt_ani].ReadVF" );
			}
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////
#endif
	
	//-----------------------------------------------------------------------------
	// 빌보드 생성하기 
	//-----------------------------------------------------------------------------
	//	g_Map.CreateBilObj();
	
	//-----------------------------------------------------------------------------
	// 애니메이션과 관련없는 버텍스는 삭제하기
	//-----------------------------------------------------------------------------
	//ProfileBegin( "DeleteVertex" );
	DeleteVertex();
	//ProfileEnd( "DeleteVertex" );
	//ProfileEnd( "< Object_Manager::Create >" );

	return true;
}

/// sooree pack : 주의! 
///					1) 호출전에 반드시 해당디렉토리로 VF.ChangeDir(...) 미리 해주어야 한다. 
///					2) file_ext는 "obj", "msh" 등 처럼 확장자 세글자만 넘긴다 
int Object_Manager::GetFileCountVF( CVFileSystem* pVF, char* file_ext )	/// sooree pack
{
	int count = 0;
	char ext[256];

	VFileHandleList* pFHL;
	VFileHandleList_i it;

	pFHL = pVF->GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, ext );

			if( lstrcmp( ext, file_ext ) == 0 )
			{
				++count;
			}
		}
	}
		
	return count;
}

int Object_Manager::GetFileCount( char* file_pos, char* file_ext )
{
	// 모델파일이 있는 경로 만들기	
	char file_path[256] = {0,};
	strcpy( file_path, file_pos);
	strcat( file_path, file_ext);	
	
	// 파일 찾기	
	WIN32_FIND_DATA	fild_data;
	HANDLE hFindFile = FindFirstFile( file_path, &fild_data );
			
	if( INVALID_HANDLE_VALUE == hFindFile ) return 0;	// By wxywxy
	
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


//-----------------------------------------------------------------------------
// Desc: 애니메이션과 관련없는 버텍스버퍼 삭제
//-----------------------------------------------------------------------------
void Object_Manager::DeleteVertex()
{
	/*
	for( int i = 0; i < MAX_MODCOUNT; ++i )
	{
		m_Mod[i].DeleteVertex();
	}
	*/
}


//-----------------------------------------------------------------------------
// Desc: 오브젝트 LOD옵션 리턴
//-----------------------------------------------------------------------------
int Object_Manager::GetBlendAttr( int _nIndex )
{
	for( int i = 0; i < m_nSetCount; ++i )
	{
		for( int j = 0; j < MAX_OBJCOUNT; ++j )
		{
			if( m_SetManager.SetInfo[i].nIndex[j] == _nIndex )
			{
				// 빌보드, 일반적인 옵션
				if(  m_SetManager.SetInfo[i].nlod[j] > 0 ||
					(m_SetManager.SetInfo[i].nlod[j] < 0 && m_SetManager.SetInfo[i].nlod[j] > -10) )
				{
					break;
				}
				// 추가옵션	
				return abs(m_SetManager.SetInfo[i].nlod[j]%10);
			}
		}
	}
	return 0;
}


int Object_Manager::GetISBlendAttr( int _nIndex )
{
	for( int j = 0; j < MAX_OBJCOUNT; ++j )
	{
		if( m_SetManager.SetInfo[m_nCurtSet].nIndex[j] != j ) break;

		if( m_SetManager.SetInfo[m_nCurtSet].nIndex[j] == _nIndex )
		{
			return m_SetManager.SetInfo[m_nCurtSet].ISBlend[j];
		}
	}

	return 1;
}

int Object_Manager::GetLodAttr( int _nIndex )
{
/*	for( int i = 0; i < m_nSetCount; ++i )
	{
		for( int j = 0; j < MAX_OBJCOUNT; ++j )
		{
			if( m_SetManager.SetInfo[i].nIndex[j] == _nIndex )
			{
				// 빌보드, 일반적인 옵션
				if(  m_SetManager.SetInfo[i].nlod[j] > 0 || 
					(m_SetManager.SetInfo[i].nlod[j] < 0 && m_SetManager.SetInfo[i].nlod[j] > -10) )
					return m_SetManager.SetInfo[i].nlod[j];
				// 추가옵션
				else
					return m_SetManager.SetInfo[i].nlod[j]/10;
			}
		}
	}
	return -1;
*/
	/// 이거 호출 전에 반드시 GetNowSet() 이 호출되었다는 것을 전제 
	for( int j = 0; j < MAX_OBJCOUNT; ++j )
	{
		if( m_SetManager.SetInfo[m_nCurtSet].nIndex[j] != j ) break;

		if( m_SetManager.SetInfo[m_nCurtSet].nIndex[j] == _nIndex )
		{
			// 빌보드, 일반적인 옵션
			if(  m_SetManager.SetInfo[m_nCurtSet].nlod[j] > 0 || 
				(m_SetManager.SetInfo[m_nCurtSet].nlod[j] < 0 && m_SetManager.SetInfo[m_nCurtSet].nlod[j] > -10) )
				return m_SetManager.SetInfo[m_nCurtSet].nlod[j];
			// 추가옵션
			else
				return m_SetManager.SetInfo[m_nCurtSet].nlod[j]/10;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------
// Desc: 오브젝트 셋트 파일 읽기
//-----------------------------------------------------------------------------
void Object_Manager::SetCreate( char* file_pos )
{
	char	file_path[256] = {0,};

#if !defined( LOAD_VF )

	// 오브젝트 셋트파일 읽어오기
	m_nSetCount = 0;
	for( int i = 0; i < SET_COUNT; ++i )
	{
		sprintf( file_path, "%s\\%d.set", file_pos, i  );
		if( Set__Info( file_path, i ) )
		{
			//++m_nSetCount;
			m_nSetCount = i + 1;
		}
	}

#else	/// sooree pack

	sprintf( file_path, "%s\\set.rvf", file_pos );
	g_VFMap.OpenFileSystem( file_path );

	// 오브젝트 셋트파일 읽어오기
	m_nSetCount = 0;
	for( int i = 0; i < SET_COUNT; ++i )
	{
		sprintf( file_path, "%d.set", i  );
		
		if( Set__InfoVF( &g_VFMap, file_path, i ) )
		{
			//++m_nSetCount;
			m_nSetCount = i + 1;
		}
	}
#endif

}

bool Object_Manager::Set__Info( char* file_path, int index )
{
	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL ) return false;

	char*	token;				// 토큰
	char	string [512];			// 문서 줄단위 저장 변수
	bool	bRead  = true;			// 문서 다 읽었는지 체크		
	char	splitter[] = " \n\t";	// 구분자 : 캐리지리턴, 탭 
	BYTE	btType		= 0;
	int		ObjNumber	= 0;

	// 줄 단위로 읽기
	if( fgets( string, 512, file ) == NULL )				
	{
		fclose( file );
		return false; 
	}
		
	do 
	{	
		// 주석이 있다면 다음 줄 읽음
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// NPC 속성값이니?
			token = strtok( string, splitter );
			if( strcmp( "//TEX", token ) == 0 )			
				btType++;

			// 더 이상 읽은게 없다면 루프 종료
			if( fgets( string, 512 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// 한줄 읽기
		token = strtok( string, splitter );
		if( token == NULL )			
			break;
		
		// 타입에 따라서 처리
		switch( btType )
		{
		case 0:	// 맵아이디
			{					
				for( int i = 0; i < MAX_MAPIDCOUNT; ++i )
				{
					if( token == NULL ) break;
					else m_SetManager.byMapId[index][i] = (int)atoi(token);

					token = strtok( NULL, splitter ); 
				}
				btType++;
			}
			break;
		case 1:	// 오브젝트속성
			{
				m_SetManager.SetInfo[index].nIndex[ObjNumber] = (unsigned int)atoi(token);										
				token = strtok( NULL, splitter );
				
				if( token == NULL )
					bRead = false;
				else
				{
					m_SetManager.SetInfo[index].nlod[ObjNumber] = (unsigned int)atoi(token);
				}
				
				token = strtok( NULL, splitter );
				
				if( token == NULL )
					bRead = false;
				else
				{
					m_SetManager.SetInfo[index].ISBlend[ObjNumber] = (unsigned int)atoi(token);
					ObjNumber++;
				}
			}
			break;
		case 2:	// 텍스쳐속성 
			{
				int TexNumber = atoi( token );
				token = strtok( NULL, splitter );
				if( token == NULL )
					bRead = false;
				else					
					m_Tex[TexNumber].m_byAttr = atoi( token );					
			}
			break;
		}
		
		// 다음 줄 읽기, 읽은게 없다면 string은 0으로									
		if( fgets( string, 512 , file ) == NULL )		
			bRead = false;			
	} 
	while( bRead != false ); // 더 이상 읽은게 없다면 루프 종료			

	fclose( file );

	return true;
}

/// sooree pack
bool Object_Manager::Set__InfoVF( CVFileSystem* pVF, char* file_path, int index )
{
	VFileHandle* pFH = pVF->OpenFile( file_path );
	if( pFH == NULL ) return false;

	char*	token;				// 토큰
	char	string [512];			// 문서 줄단위 저장 변수
	bool	bRead  = true;			// 문서 다 읽었는지 체크		
	char	splitter[] = " \n\t";	// 구분자 : 캐리지리턴, 탭 
	BYTE	btType		= 0;
	int		ObjNumber	= 0;

	
	memset(  m_SetManager.SetInfo[index].nIndex  , 0  ,  sizeof(  m_SetManager.SetInfo[index].nIndex   ) );
	memset(  m_SetManager.SetInfo[index].nlod      , 0  ,  sizeof(  m_SetManager.SetInfo[index].nlod      ) );
	memset(  m_SetManager.SetInfo[index].ISBlend  , 0  ,  sizeof(  m_SetManager.SetInfo[index].ISBlend ) );

	// 줄 단위로 읽기
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		pVF->CloseFile( pFH );
		return false; 
	}
		
	do 
	{	
		// 주석이 있다면 다음 줄 읽음
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// NPC 속성값이니?
			token = strtok( string, splitter );
			if( strcmp( "//TEX", token ) == 0 )			
				btType++;

			// 더 이상 읽은게 없다면 루프 종료
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}
		
		// 한줄 읽기
		token = strtok( string, splitter );
		if( token == NULL )			
			break;
		
		// 타입에 따라서 처리
		switch( btType )
		{
		case 0:	// 맵아이디
			{					
				for( int i = 0; i < MAX_MAPIDCOUNT; ++i )
				{
					if( token == NULL ) break;
					else m_SetManager.byMapId[index][i] = (int)atoi(token);

					token = strtok( NULL, splitter ); 
				}
				btType++;
			}
			break;
		case 1:	// 오브젝트속성
			{
				m_SetManager.SetInfo[index].nIndex[ObjNumber] = (unsigned int)atoi(token);										
				token = strtok( NULL, splitter );
				if( token == NULL )
					bRead = false;
				else
				{
					m_SetManager.SetInfo[index].nlod[ObjNumber] = (unsigned int)atoi(token);
				}

				token = strtok( NULL, splitter );
				if( token == NULL )
					bRead = false;
				else
				{
					m_SetManager.SetInfo[index].ISBlend[ObjNumber] = (unsigned int)atoi(token);
					ObjNumber++;
				}
			}
			break;
		case 2:	// 텍스쳐속성 
			{
				int TexNumber = atoi( token );
				token = strtok( NULL, splitter );
				if( token == NULL )
					bRead = false;
				else					
					m_Tex[TexNumber].m_byAttr = atoi( token );					
			}
			break;
		}
		
		// 다음 줄 읽기, 읽은게 없다면 string은 0으로									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	} 
	while( bRead != false ); // 더 이상 읽은게 없다면 루프 종료			

	pVF->CloseFile( pFH );

	return true;
}

int	Object_Manager::GetNowSet( int map_id )
{
	if( map_id == -1 )				
		map_id = g_Map.m_NowMapID;		

	for( int i = 0; i < m_nSetCount; ++i )
	{
		for( int j = 0; j < MAX_MAPIDCOUNT; ++j )
		{
			if( m_SetManager.byMapId[i][j] == 255 ) break;
			if( m_SetManager.byMapId[i][j] == map_id )
			{				
				m_nCurtSet = i;
				i = m_nSetCount;
				break;
			}
		}
	}
	return m_nCurtSet;
}

//-----------------------------------------------------------------------------
// Desc: 오브젝트 파일 이름 리턴
//-----------------------------------------------------------------------------
char*	Object_Manager::GetFileName( int count, bool& bFind )
{
///	 호출할 일 없음 : by sooree
/*	// 모델파일이 있는 경로 만들기	
	char file_path[256] = {0,};
	sprintf( file_path, "%s\\obj\\%08d.obj", g_RockClient.GetCurrentMapPath(), count );	/// by sooree

	// 파일 찾기	
	WIN32_FIND_DATA	fild_data;
	HANDLE hFindFile = FindFirstFile( file_path, &fild_data );

	if( hFindFile != INVALID_HANDLE_VALUE )
	{
		bFind = true;
		FindClose( hFindFile );
		return fild_data.cFileName;
	}
	FindClose( hFindFile );
*/
	return NULL;
}


int Object_Manager::GetVertexCount( int index )        
{
	int vtx_cnt = 0;

	vtx_cnt = m_Mod[index].m_header.m_num_vertex; 

	return vtx_cnt;
}


D3DXVECTOR3	Object_Manager::GetVertex( int index, int number ) 
{ 
	D3DXVECTOR3 vtx = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	
	if( m_Mod[index].m_pVertex )
	{
		vtx = m_Mod[index].m_pVertex[number].m_vertex; 
	}
	else
	{
		vtx = m_Mod[index].m_ppVertexList[0][number].m_vertex; 
	}

	return vtx;
}	