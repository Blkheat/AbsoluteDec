 #include "RockPCH.h"

#include	"..\brother.h"

#include	"Model.H"
#include	"OBJECT_VERTEX.H"
#include	"Define.H"
#include	<stdio.H>
#include	<assert.H>
#include	"..\\..\\CRenderManager.h"

extern	long				g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern	LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern	LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH


//-----------------------------------------------------------------------------
// Desc: 모델 생성자 & 파괴자
//-----------------------------------------------------------------------------
void Model::Clear()
{			
	m_pVB			= NULL;		// 버텍스 버퍼
	m_pIB			= NULL;		// 인덱스 버퍼
	m_pNodeID		= NULL;		// 본링크
	m_pVertex		= NULL;		// 버텍스
#ifdef BATCH_RENDER
	m_pBatchVertex	= NULL;
	m_pBatchDefaultVertex = NULL;
#endif	//	BATCH_RENDER
	m_pAnimation	= NULL;		// 애니메이션
	m_pFullAni		= NULL;		// 풀애니메이션 

	m_pIndexList	= NULL;
	m_ppVertexList  = NULL;		// 버텍스 리스트 by wxywxy

	m_bInit			= false;
	m_ani_type		= 0;
	m_Index			= 0;
	m_race			= 0;
	m_ISOneTimeLock = FALSE;
	
	m_lPrevTime = g_nowTime;
}

void Model::Destory()
{	
	// 버텍스 메모리 해제 
	SAFE_DELETE_ARRAY(m_pVertex);
#ifdef BATCH_RENDER
	SAFE_DELETE_ARRAY(m_pBatchVertex);
	SAFE_DELETE_ARRAY(m_pBatchDefaultVertex);
#endif	//	BATCH_RENDER
	SAFE_DELETE_ARRAY(m_pNodeID);

	bool IsVBIB = false;
	if(m_pVB || m_pIB)
	{
		IsVBIB = true;
	}

	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
	
	// 애니메이션 메모리 해제
	SAFE_DELETE_ARRAY(m_pAnimation);
	SAFE_DELETE_ARRAY(m_pFullAni);
	
	// 인덱스 에니 삭제
	if( m_pIndexList != NULL )
	{
		SAFE_DELETE_ARRAY(m_pIndexList);		
	}

	// 버텍스 에니 삭제
	if( m_ppVertexList != NULL )
	{
		for( UINT i = 0 ; i < m_header.m_ani_frame ; ++i )
		{
			SAFE_DELETE_ARRAY(m_ppVertexList[ i ]);			
		}
		SAFE_DELETE_ARRAY(m_ppVertexList);		
	}	

	Clear();
}


//-----------------------------------------------------------------------------
// Desc: 모델파일 읽기 & 버텍스, 인덱스 버퍼 메모리 할당
//-----------------------------------------------------------------------------
bool Model::Read( char* file_path, bool create_buffer )
{	
	if( m_bInit == true )
		return false;

	//-----------------------------------------------------------------------------
	// 파일 포인터 열기
	//-----------------------------------------------------------------------------
	HRESULT	hr;	
	FILE*	file = NULL;
	if( NULL == (file = fopen( file_path, "rb" )) )	
	{
		CloseWnd_Error( "!%s 파일을 찾을 수 없습니다.", file_path );			
		return false;
	}	

	//-----------------------------------------------------------------------------
	// 파일읽기
	//-----------------------------------------------------------------------------
	// 헤더	
	fread( &m_header,	sizeof(MESH_HEADER),	1,	file );	

	//-----------------------------------------------------------------------------
	// 버텍스
	//-----------------------------------------------------------------------------		
	m_pVertex	= SAFE_NEW_ARRAY( VERTEX_TABLE, m_header.m_num_vertex );	
	m_pNodeID	= SAFE_NEW_ARRAY( MESH_NODE_ID , m_header.m_num_vertex );						
	fread( m_pVertex,	sizeof(VERTEX_TABLE), m_header.m_num_vertex, file );
	fread( m_pNodeID,	sizeof(MESH_NODE_ID), m_header.m_num_vertex, file );		

	//-----------------------------------------------------------------------------
	// 인덱스(페이스)버퍼
	//-----------------------------------------------------------------------------		
	hr = g_RenderManager.CreateIndexBuffer( (m_header.m_num_face*3)*sizeof(WORD), D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB );	
	assert( hr == 0 && "!인덱스 버퍼 메모리 할당 실패" );	

	WORD* pIndices;		
	//hr = m_pIB->Lock( 0, m_header.m_num_face*sizeof(WORD), (BYTE**)&pIndices, 0 );
	//by simwoosung
#ifdef DIRECT_VERSION_9_MJH
	hr = m_pIB->Lock( 0, (m_header.m_num_face*3)*sizeof(WORD), (void**)&pIndices, 0 );
#else
	hr = m_pIB->Lock( 0, (m_header.m_num_face*3)*sizeof(WORD), (BYTE**)&pIndices, 0 );
#endif // DIRECT_VERSION_9_MJH
	assert( hr == 0 && "!인덱스 버퍼 락 실패" );		
	fread( pIndices, sizeof(FACE_TABLE), m_header.m_num_face, file );
	hr = m_pIB->Unlock();

	//------------------------------------------------------------------------------------------------
	// 애니메이션 타입 0:애니없음  1:메쉬  2:본(바이패드)  3:스케일애니메이션 4: 버텍스에니 by wxywxy
	//------------------------------------------------------------------------------------------------
	fread( &m_ani_type,	sizeof(m_ani_type),	1, file );		

	switch( m_ani_type )
	{
		case 1:
			{
				m_pAnimation = SAFE_NEW_ARRAY( MESH_ANI , m_header.m_ani_frame );				
				fread( m_pAnimation, sizeof(MESH_ANI), m_header.m_ani_frame, file );
			}
			
			break;
		case 3:
			{
				m_pFullAni = SAFE_NEW_ARRAY( MESH_FULL_ANI , m_header.m_ani_frame );				
				fread( m_pFullAni,   sizeof(MESH_FULL_ANI), m_header.m_ani_frame, file );
			}
			
			break;

		case 4:
			// 버텍스 에니 추가 By wxywxy
			{
				m_ppVertexList = SAFE_NEW_ARRAY( CMesh_AniVertex* , m_header.m_ani_frame );		
				
				m_ppVertexList[ 0 ] = SAFE_NEW_ARRAY( CMesh_AniVertex , m_header.m_num_vertex );				
				UINT i = 0 ;
				for(  i = 0 ; i < m_header.m_num_vertex ; ++i )
				{
					m_ppVertexList[ 0 ][ i ].m_vertex = m_pVertex[i].m_vertex;
					m_ppVertexList[ 0 ][ i ].m_normal = m_pVertex[i].m_normal;
				}
				
				for( i = 1 ; i < m_header.m_ani_frame ; ++i )
				{
					m_ppVertexList[ i ] = SAFE_NEW_ARRAY( CMesh_AniVertex , m_header.m_num_vertex );					
					
					for( int j = 0 ; j < m_header.m_num_vertex ; ++j )
					{
						fread( &m_ppVertexList[ i ][ j ] , sizeof( CMesh_AniVertex ), 1, file ); 
					}
				}
			}
			break;

	}

	//-----------------------------------------------------------------------------
	// 파일 포인터 닫기
	//-----------------------------------------------------------------------------
	fclose( file );	

	//-----------------------------------------------------------------------------
	// 버텍스 버퍼
	//-----------------------------------------------------------------------------
	if( create_buffer == true )
	{
		hr = g_RenderManager.CreateVertexBuffer( m_header.m_num_vertex*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY, 
											 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pVB );
		assert( hr == 0 && "!버텍스 버퍼 메모리 할당 실패" );

		OBJECTVERTEX*	pOBJVERTEX;	 
#ifdef DIRECT_VERSION_9_MJH
		hr = m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 );
#else
		hr = m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );
#endif // DIRECT_VERSION_9_MJH	
		assert( hr == 0 && "!버텍스 버퍼 락 실패" );			
		memcpy( pOBJVERTEX, m_pVertex, sizeof(OBJECTVERTEX)*m_header.m_num_vertex );
		hr = m_pVB->Unlock();
		assert( hr == 0 && "!버텍스 버퍼 언락 실패" );

		//-----------------------------------------------------------------------------
		// 버텍스버퍼 필요없다면 삭제
		//-----------------------------------------------------------------------------
		if( m_ani_type != 2 && m_ani_type != 0 )
		{
			SAFE_DELETE_ARRAY( m_pVertex );
			SAFE_DELETE_ARRAY( m_pNodeID );
		}
	}
	return m_bInit = true;
}

//-----------------------------------------------------------------------------
// Desc: 모델파일 읽기 & 버텍스, 인덱스 버퍼 메모리 할당
//-----------------------------------------------------------------------------
bool	Model::ReadVF( CVFileSystem* pVF, char* file_path, bool create_buffer  )	/// sooree pack
{	
	if( m_bInit == true )
		return false;

	//-----------------------------------------------------------------------------
	// 파일 포인터 열기
	//-----------------------------------------------------------------------------
	HRESULT	hr;
	FACE_TABLE *pIndexList = NULL;
	VFileHandle* pFH = pVF->OpenFile( file_path );
	
	if( pFH == NULL ) 
	{
		CloseWnd_Error( "!%s 파일을 찾을 수 없습니다.", file_path );			
		
		return false;
	}

	//-----------------------------------------------------------------------------
	// 파일읽기
	//-----------------------------------------------------------------------------
	// 헤더	
	pFH->Read( &m_header,	sizeof(MESH_HEADER)	);	

	//-----------------------------------------------------------------------------
	// 버텍스
	//-----------------------------------------------------------------------------		
	m_pVertex	= SAFE_NEW_ARRAY( VERTEX_TABLE, m_header.m_num_vertex );
#ifdef BATCH_RENDER
	m_pBatchVertex			= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_header.m_num_vertex );
	m_pBatchDefaultVertex	= SAFE_NEW_ARRAY( VERTEX_XYZ_DIFFUSE_TEX1, m_header.m_num_vertex );
#endif	//	BATCH_RENDER

#ifdef	BATCH_RENDER
	if( NULL == m_pVertex || NULL == m_pBatchVertex || m_pBatchDefaultVertex == NULL )
#else	//	BATCH_RENDER
	if( NULL == m_pVertex )
#endif	//	BATCH_RENDER
	{
		CloseWnd_Error( "New Vertex Memory ( Model::ReadVF ) %s" , file_path );

		goto _MODEL_LOAD_ERROR;
	}
	
	m_pNodeID	= SAFE_NEW_ARRAY( MESH_NODE_ID , m_header.m_num_vertex );					

	if( NULL == m_pNodeID )
	{
		CloseWnd_Error( "New Node Memory ( Model::ReadVF ) %s" , file_path );

		goto _MODEL_LOAD_ERROR;
	}

	pFH->Read( m_pVertex,	sizeof(VERTEX_TABLE) * m_header.m_num_vertex );
	pFH->Read( m_pNodeID,	sizeof(MESH_NODE_ID) * m_header.m_num_vertex );

	//-----------------------------------------------------------------------------
	// 인덱스(페이스)버퍼
	//-----------------------------------------------------------------------------	
	
	hr = g_RenderManager.CreateIndexBuffer( (m_header.m_num_face)*sizeof(FACE_TABLE), D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB );	

	if( FAILED( hr ) )
	{
		CloseWnd_Error( "Create Index Buffer ( Model::ReadVF ) %s" , file_path );

		goto _MODEL_LOAD_ERROR;
	}

	WORD* pIndices;	
	//hr = m_pIB->Lock( 0, m_header.m_num_face*sizeof(WORD), (BYTE**)&pIndices, 0 );
	//수정 by simwoosung
#ifdef DIRECT_VERSION_9_MJH
	hr = m_pIB->Lock( 0, (m_header.m_num_face)*sizeof(FACE_TABLE), (void**)&pIndices, 0 );
#else
	hr = m_pIB->Lock( 0, (m_header.m_num_face)*sizeof(FACE_TABLE), (BYTE**)&pIndices, 0 );
#endif // DIRECT_VERSION_9_MJH
	
	if( FAILED( hr ) )
	{
		CloseWnd_Error( "Lock Index Buffer ( Model::ReadVF ) %s" , file_path );

		goto _MODEL_LOAD_ERROR;
	}

#ifdef BATCH_RENDER
	pIndexList = SAFE_NEW_ARRAY( FACE_TABLE , m_header.m_num_face );
	m_pIndexList = SAFE_NEW_ARRAY( WORD, m_header.m_num_face*3 );
	if( m_pIndexList )
	{
		pFH->Read( pIndexList, sizeof(FACE_TABLE) * m_header.m_num_face );
		
		memcpy(pIndices, pIndexList, sizeof(FACE_TABLE) * m_header.m_num_face);

		memcpy(m_pIndexList, pIndexList, sizeof(WORD) * m_header.m_num_face*3);	
		
		hr = m_pIB->Unlock();	
	}
	SAFE_DELETE_ARRAY(pIndexList);
#else	//	BATCH_RENDER
	pIndexList = SAFE_NEW_ARRAY( FACE_TABLE , m_header.m_num_face );
	if( pIndexList )
	{
		pFH->Read( pIndexList, sizeof(FACE_TABLE) * m_header.m_num_face );
		
		memcpy(pIndices, pIndexList, sizeof(FACE_TABLE) * m_header.m_num_face);
		
		hr = m_pIB->Unlock();	
	}
	
	SAFE_DELETE_ARRAY(pIndexList);
#endif	//	BATCH_RENDER
	


	if( FAILED( hr ) )
	{
		CloseWnd_Error( "UnLock Index Buffer ( Model::ReadVF ) %s" , file_path );

		goto _MODEL_LOAD_ERROR;
	}

	//------------------------------------------------------------------------------------------------
	// 애니메이션 타입 0:애니없음  1:메쉬  2:본(바이패드)  3:스케일애니메이션 4: 버텍스에니 by wxywxy
	//------------------------------------------------------------------------------------------------
	pFH->Read( &m_ani_type,	sizeof(m_ani_type) );		

	switch( m_ani_type )
	{
		case 1:
			{
				m_pAnimation = SAFE_NEW_ARRAY( MESH_ANI , m_header.m_ani_frame );				
				pFH->Read( m_pAnimation, sizeof(MESH_ANI) * m_header.m_ani_frame );
			}
			
			break;
		case 3:
			{
				m_pFullAni = SAFE_NEW_ARRAY( MESH_FULL_ANI , m_header.m_ani_frame );				
				pFH->Read( m_pFullAni,   sizeof(MESH_FULL_ANI) * m_header.m_ani_frame );
			}
			
			break;

		case 4:
			// 버텍스 에니 추가 By wxywxy
			{
				m_ppVertexList = SAFE_NEW_ARRAY( CMesh_AniVertex* , m_header.m_ani_frame );				
				
				m_ppVertexList[ 0 ] = SAFE_NEW_ARRAY( CMesh_AniVertex , m_header.m_num_vertex );				
				UINT i = 0 ;
				for(  i = 0 ; i < m_header.m_num_vertex ; ++i )
				{
					m_ppVertexList[ 0 ][ i ].m_vertex = m_pVertex[i].m_vertex;
					m_ppVertexList[ 0 ][ i ].m_normal = m_pVertex[i].m_normal;
				}
				
				for( i = 1 ; i < m_header.m_ani_frame ; ++i )
				{
					m_ppVertexList[ i ] = SAFE_NEW_ARRAY( CMesh_AniVertex , m_header.m_num_vertex );				
					
					for( int j = 0 ; j < m_header.m_num_vertex ; ++j )
					{
						pFH->Read( &m_ppVertexList[ i ][ j ], sizeof( CMesh_AniVertex ) ); 
					}
				}
			}
			break;

	}

	//-----------------------------------------------------------------------------
	// 파일 포인터 닫기
	//-----------------------------------------------------------------------------
	pVF->CloseFile( pFH );

	//-----------------------------------------------------------------------------
	// 버텍스 버퍼
	//-----------------------------------------------------------------------------
	if( create_buffer == true )
	{
		hr = g_RenderManager.CreateVertexBuffer( m_header.m_num_vertex * sizeof( OBJECTVERTEX ) , D3DUSAGE_WRITEONLY, 
											 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pVB );
		
		//assert( hr == 0 && "!버텍스 버퍼 메모리 할당 실패" );
		if( FAILED( hr ) )
		{
			CloseWnd_Error( "Create Vertex Buffer ( Model::ReadVF ) %s" , file_path );

			goto _MODEL_LOAD_ERROR;
		}

		OBJECTVERTEX*	pOBJVERTEX;	 	
#ifdef DIRECT_VERSION_9_MJH
		hr = m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 );
#else
		hr = m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );
#endif // DIRECT_VERSION_9_MJH			
		
		if( FAILED( hr ) )
		{
			CloseWnd_Error( "Lock Vertex Buffer ( Model::ReadVF ) %s" , file_path );
		
			goto _MODEL_LOAD_ERROR;
		}
		
		memcpy( pOBJVERTEX, m_pVertex, sizeof(OBJECTVERTEX) * m_header.m_num_vertex );
#ifdef	BATCH_RENDER
		for(int i = 0; i < m_header.m_num_vertex; i++)
		{
//			m_pBatchVertex[i].nv	= m_pVertex[i].m_normal;
			m_pBatchDefaultVertex[i].v	= m_pBatchVertex[i].v	= m_pVertex[i].m_vertex;
			m_pBatchDefaultVertex[i].tu	= m_pBatchVertex[i].tu	= m_pVertex[i].m_u;
			m_pBatchDefaultVertex[i].tv	= m_pBatchVertex[i].tv	= m_pVertex[i].m_v;
		}
//		memcpy( m_pBatchDefaultVertex, m_pBatchVertex, sizeof(BATCHVERTEX) * m_header.m_num_vertex );
#endif	//	BATCH_RENDER
		
		hr = m_pVB->Unlock();
		
		if( FAILED( hr ) )
		{
			CloseWnd_Error( "Error UnLock Vertex Buffer ( Model::ReadVF ) %s" , file_path );
		
			goto _MODEL_LOAD_ERROR;
		}

		//-----------------------------------------------------------------------------
		// 버텍스버퍼 필요없다면 삭제
		//-----------------------------------------------------------------------------
		if( m_ani_type != 2 && m_ani_type != 0 )
		{
			SAFE_DELETE_ARRAY( m_pVertex );
			SAFE_DELETE_ARRAY( m_pNodeID );			
		}
	}

	return m_bInit = true;

_MODEL_LOAD_ERROR:

	SAFE_DELETE_ARRAY( m_pVertex );	
	SAFE_DELETE_ARRAY( m_pNodeID );

	return false;
}

//-----------------------------------------------------------------------------
// Desc: 모델파일 읽기 & 버텍스, 인덱스 버퍼 메모리 할당 ( 가장 높은 점 얻기 )
//-----------------------------------------------------------------------------
BOOL Model::ReadAndHeight( char* file_path , float &height, bool create_buffer )
{	
	if( m_bInit == true )
		return false;


	float fy = -9999.0f;

	//-----------------------------------------------------------------------------
	// 파일 포인터 열기
	//-----------------------------------------------------------------------------
	HRESULT	hr;	
	FILE*	file = NULL;
	if( NULL == (file = fopen( file_path, "rb" )) )	
	{
		CloseWnd_Error( "!%s 파일을 찾을 수 없습니다.", file_path );			
		return false;
	}

	//-----------------------------------------------------------------------------
	// 파일읽기
	//-----------------------------------------------------------------------------
	// 헤더	
	fread( &m_header,	sizeof(MESH_HEADER),	1,	file );	

	//-----------------------------------------------------------------------------
	// 버텍스
	//-----------------------------------------------------------------------------		
	m_pVertex	= SAFE_NEW_ARRAY( VERTEX_TABLE, m_header.m_num_vertex );	
	m_pNodeID	= SAFE_NEW_ARRAY( MESH_NODE_ID , m_header.m_num_vertex );						
	fread( m_pVertex,	sizeof(VERTEX_TABLE), m_header.m_num_vertex, file );
	fread( m_pNodeID,	sizeof(MESH_NODE_ID), m_header.m_num_vertex, file );		

	for( int i = 0 ; i < m_header.m_num_vertex ; ++i )
	{
		if( fy < m_pVertex[ i ].m_vertex.y )
			fy = m_pVertex[ i ].m_vertex.y;
	}

	height = fy;

	//-----------------------------------------------------------------------------
	// 인덱스(페이스)버퍼
	//-----------------------------------------------------------------------------		
	hr = g_RenderManager.CreateIndexBuffer( (m_header.m_num_face*3)*sizeof(WORD), D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB );	
	assert( hr == 0 && "!인덱스 버퍼 메모리 할당 실패" );	

	WORD* pIndices;
#ifdef DIRECT_VERSION_9_MJH
	hr = m_pIB->Lock( 0, m_header.m_num_face*sizeof(WORD), (void**)&pIndices, 0 );	
#else
	hr = m_pIB->Lock( 0, m_header.m_num_face*sizeof(WORD), (BYTE**)&pIndices, 0 );	
#endif // DIRECT_VERSION_9_MJH
	assert( hr == 0 && "!인덱스 버퍼 락 실패" );		
	fread( pIndices, sizeof(FACE_TABLE), m_header.m_num_face, file );
	hr = m_pIB->Unlock();

	//------------------------------------------------------------------------------------------------
	// 애니메이션 타입 0:애니없음  1:메쉬  2:본(바이패드)  3:스케일애니메이션 4: 버텍스에니 by wxywxy
	//------------------------------------------------------------------------------------------------
	fread( &m_ani_type,	sizeof(m_ani_type),	1, file );		

	switch( m_ani_type )
	{
		case 1:
			{
				m_pAnimation = SAFE_NEW_ARRAY( MESH_ANI , m_header.m_ani_frame );				
				fread( m_pAnimation, sizeof(MESH_ANI), m_header.m_ani_frame, file );
			}
			
			break;
		case 3:
			{
				m_pFullAni = SAFE_NEW_ARRAY( MESH_FULL_ANI , m_header.m_ani_frame );				
				fread( m_pFullAni,   sizeof(MESH_FULL_ANI), m_header.m_ani_frame, file );
			}
			
			break;

		case 4:
			// 버텍스 에니 추가 By wxywxy
			{
				m_ppVertexList = SAFE_NEW_ARRAY( CMesh_AniVertex* , m_header.m_ani_frame );
				
				m_ppVertexList[ 0 ] = SAFE_NEW_ARRAY( CMesh_AniVertex , m_header.m_num_vertex );				
				UINT i = 0 ;
				for(  i = 0 ; i < m_header.m_num_vertex ; ++i )
				{
					m_ppVertexList[ 0 ][ i ].m_vertex = m_pVertex[i].m_vertex;
					m_ppVertexList[ 0 ][ i ].m_normal = m_pVertex[i].m_normal;
				}
				
				for( i = 1 ; i < m_header.m_ani_frame ; ++i )
				{
					m_ppVertexList[ i ] = SAFE_NEW_ARRAY( CMesh_AniVertex , m_header.m_num_vertex );					
					
					for( int j = 0 ; j < m_header.m_num_vertex ; ++j )
					{
						fread( &m_ppVertexList[ i ][ j ] , sizeof( CMesh_AniVertex ), 1, file ); 
					}
				}
			}
			break;

	}

	//-----------------------------------------------------------------------------
	// 파일 포인터 닫기
	//-----------------------------------------------------------------------------
	fclose( file );	

	//-----------------------------------------------------------------------------
	// 버텍스 버퍼
	//-----------------------------------------------------------------------------
	if( create_buffer == true )
	{
		hr = g_RenderManager.CreateVertexBuffer( m_header.m_num_vertex*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY, 
											 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pVB );
		assert( hr == 0 && "!버텍스 버퍼 메모리 할당 실패" );

		OBJECTVERTEX*	pOBJVERTEX;	 
#ifdef DIRECT_VERSION_9_MJH
		hr = m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 );
#else
		hr = m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );
#endif // DIRECT_VERSION_9_MJH	
		assert( hr == 0 && "!버텍스 버퍼 락 실패" );			
		memcpy( pOBJVERTEX, m_pVertex, sizeof(OBJECTVERTEX)*m_header.m_num_vertex );
		hr = m_pVB->Unlock();
		assert( hr == 0 && "!버텍스 버퍼 언락 실패" );

		//-----------------------------------------------------------------------------
		// 버텍스버퍼 필요없다면 삭제
		//-----------------------------------------------------------------------------
		if( m_ani_type != 2 && m_ani_type != 0 )
		{
			SAFE_DELETE_ARRAY( m_pVertex );
			SAFE_DELETE_ARRAY( m_pNodeID );
		}
	}
	return m_bInit = true;
}

//-----------------------------------------------------------------------------
// Desc: 애니메이션과 관련없는 버텍스버퍼 삭제
//-----------------------------------------------------------------------------
void Model::DeleteVertex()
{

}