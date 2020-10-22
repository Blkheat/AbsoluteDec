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
// Desc: �� ������ & �ı���
//-----------------------------------------------------------------------------
void Model::Clear()
{			
	m_pVB			= NULL;		// ���ؽ� ����
	m_pIB			= NULL;		// �ε��� ����
	m_pNodeID		= NULL;		// ����ũ
	m_pVertex		= NULL;		// ���ؽ�
#ifdef BATCH_RENDER
	m_pBatchVertex	= NULL;
	m_pBatchDefaultVertex = NULL;
#endif	//	BATCH_RENDER
	m_pAnimation	= NULL;		// �ִϸ��̼�
	m_pFullAni		= NULL;		// Ǯ�ִϸ��̼� 

	m_pIndexList	= NULL;
	m_ppVertexList  = NULL;		// ���ؽ� ����Ʈ by wxywxy

	m_bInit			= false;
	m_ani_type		= 0;
	m_Index			= 0;
	m_race			= 0;
	m_ISOneTimeLock = FALSE;
	
	m_lPrevTime = g_nowTime;
}

void Model::Destory()
{	
	// ���ؽ� �޸� ���� 
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
	
	// �ִϸ��̼� �޸� ����
	SAFE_DELETE_ARRAY(m_pAnimation);
	SAFE_DELETE_ARRAY(m_pFullAni);
	
	// �ε��� ���� ����
	if( m_pIndexList != NULL )
	{
		SAFE_DELETE_ARRAY(m_pIndexList);		
	}

	// ���ؽ� ���� ����
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
// Desc: ������ �б� & ���ؽ�, �ε��� ���� �޸� �Ҵ�
//-----------------------------------------------------------------------------
bool Model::Read( char* file_path, bool create_buffer )
{	
	if( m_bInit == true )
		return false;

	//-----------------------------------------------------------------------------
	// ���� ������ ����
	//-----------------------------------------------------------------------------
	HRESULT	hr;	
	FILE*	file = NULL;
	if( NULL == (file = fopen( file_path, "rb" )) )	
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", file_path );			
		return false;
	}	

	//-----------------------------------------------------------------------------
	// �����б�
	//-----------------------------------------------------------------------------
	// ���	
	fread( &m_header,	sizeof(MESH_HEADER),	1,	file );	

	//-----------------------------------------------------------------------------
	// ���ؽ�
	//-----------------------------------------------------------------------------		
	m_pVertex	= SAFE_NEW_ARRAY( VERTEX_TABLE, m_header.m_num_vertex );	
	m_pNodeID	= SAFE_NEW_ARRAY( MESH_NODE_ID , m_header.m_num_vertex );						
	fread( m_pVertex,	sizeof(VERTEX_TABLE), m_header.m_num_vertex, file );
	fread( m_pNodeID,	sizeof(MESH_NODE_ID), m_header.m_num_vertex, file );		

	//-----------------------------------------------------------------------------
	// �ε���(���̽�)����
	//-----------------------------------------------------------------------------		
	hr = g_RenderManager.CreateIndexBuffer( (m_header.m_num_face*3)*sizeof(WORD), D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB );	
	assert( hr == 0 && "!�ε��� ���� �޸� �Ҵ� ����" );	

	WORD* pIndices;		
	//hr = m_pIB->Lock( 0, m_header.m_num_face*sizeof(WORD), (BYTE**)&pIndices, 0 );
	//by simwoosung
#ifdef DIRECT_VERSION_9_MJH
	hr = m_pIB->Lock( 0, (m_header.m_num_face*3)*sizeof(WORD), (void**)&pIndices, 0 );
#else
	hr = m_pIB->Lock( 0, (m_header.m_num_face*3)*sizeof(WORD), (BYTE**)&pIndices, 0 );
#endif // DIRECT_VERSION_9_MJH
	assert( hr == 0 && "!�ε��� ���� �� ����" );		
	fread( pIndices, sizeof(FACE_TABLE), m_header.m_num_face, file );
	hr = m_pIB->Unlock();

	//------------------------------------------------------------------------------------------------
	// �ִϸ��̼� Ÿ�� 0:�ִϾ���  1:�޽�  2:��(�����е�)  3:�����Ͼִϸ��̼� 4: ���ؽ����� by wxywxy
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
			// ���ؽ� ���� �߰� By wxywxy
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
	// ���� ������ �ݱ�
	//-----------------------------------------------------------------------------
	fclose( file );	

	//-----------------------------------------------------------------------------
	// ���ؽ� ����
	//-----------------------------------------------------------------------------
	if( create_buffer == true )
	{
		hr = g_RenderManager.CreateVertexBuffer( m_header.m_num_vertex*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY, 
											 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pVB );
		assert( hr == 0 && "!���ؽ� ���� �޸� �Ҵ� ����" );

		OBJECTVERTEX*	pOBJVERTEX;	 
#ifdef DIRECT_VERSION_9_MJH
		hr = m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 );
#else
		hr = m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );
#endif // DIRECT_VERSION_9_MJH	
		assert( hr == 0 && "!���ؽ� ���� �� ����" );			
		memcpy( pOBJVERTEX, m_pVertex, sizeof(OBJECTVERTEX)*m_header.m_num_vertex );
		hr = m_pVB->Unlock();
		assert( hr == 0 && "!���ؽ� ���� ��� ����" );

		//-----------------------------------------------------------------------------
		// ���ؽ����� �ʿ���ٸ� ����
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
// Desc: ������ �б� & ���ؽ�, �ε��� ���� �޸� �Ҵ�
//-----------------------------------------------------------------------------
bool	Model::ReadVF( CVFileSystem* pVF, char* file_path, bool create_buffer  )	/// sooree pack
{	
	if( m_bInit == true )
		return false;

	//-----------------------------------------------------------------------------
	// ���� ������ ����
	//-----------------------------------------------------------------------------
	HRESULT	hr;
	FACE_TABLE *pIndexList = NULL;
	VFileHandle* pFH = pVF->OpenFile( file_path );
	
	if( pFH == NULL ) 
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", file_path );			
		
		return false;
	}

	//-----------------------------------------------------------------------------
	// �����б�
	//-----------------------------------------------------------------------------
	// ���	
	pFH->Read( &m_header,	sizeof(MESH_HEADER)	);	

	//-----------------------------------------------------------------------------
	// ���ؽ�
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
	// �ε���(���̽�)����
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
	//���� by simwoosung
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
	// �ִϸ��̼� Ÿ�� 0:�ִϾ���  1:�޽�  2:��(�����е�)  3:�����Ͼִϸ��̼� 4: ���ؽ����� by wxywxy
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
			// ���ؽ� ���� �߰� By wxywxy
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
	// ���� ������ �ݱ�
	//-----------------------------------------------------------------------------
	pVF->CloseFile( pFH );

	//-----------------------------------------------------------------------------
	// ���ؽ� ����
	//-----------------------------------------------------------------------------
	if( create_buffer == true )
	{
		hr = g_RenderManager.CreateVertexBuffer( m_header.m_num_vertex * sizeof( OBJECTVERTEX ) , D3DUSAGE_WRITEONLY, 
											 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pVB );
		
		//assert( hr == 0 && "!���ؽ� ���� �޸� �Ҵ� ����" );
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
		// ���ؽ����� �ʿ���ٸ� ����
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
// Desc: ������ �б� & ���ؽ�, �ε��� ���� �޸� �Ҵ� ( ���� ���� �� ��� )
//-----------------------------------------------------------------------------
BOOL Model::ReadAndHeight( char* file_path , float &height, bool create_buffer )
{	
	if( m_bInit == true )
		return false;


	float fy = -9999.0f;

	//-----------------------------------------------------------------------------
	// ���� ������ ����
	//-----------------------------------------------------------------------------
	HRESULT	hr;	
	FILE*	file = NULL;
	if( NULL == (file = fopen( file_path, "rb" )) )	
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", file_path );			
		return false;
	}

	//-----------------------------------------------------------------------------
	// �����б�
	//-----------------------------------------------------------------------------
	// ���	
	fread( &m_header,	sizeof(MESH_HEADER),	1,	file );	

	//-----------------------------------------------------------------------------
	// ���ؽ�
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
	// �ε���(���̽�)����
	//-----------------------------------------------------------------------------		
	hr = g_RenderManager.CreateIndexBuffer( (m_header.m_num_face*3)*sizeof(WORD), D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB );	
	assert( hr == 0 && "!�ε��� ���� �޸� �Ҵ� ����" );	

	WORD* pIndices;
#ifdef DIRECT_VERSION_9_MJH
	hr = m_pIB->Lock( 0, m_header.m_num_face*sizeof(WORD), (void**)&pIndices, 0 );	
#else
	hr = m_pIB->Lock( 0, m_header.m_num_face*sizeof(WORD), (BYTE**)&pIndices, 0 );	
#endif // DIRECT_VERSION_9_MJH
	assert( hr == 0 && "!�ε��� ���� �� ����" );		
	fread( pIndices, sizeof(FACE_TABLE), m_header.m_num_face, file );
	hr = m_pIB->Unlock();

	//------------------------------------------------------------------------------------------------
	// �ִϸ��̼� Ÿ�� 0:�ִϾ���  1:�޽�  2:��(�����е�)  3:�����Ͼִϸ��̼� 4: ���ؽ����� by wxywxy
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
			// ���ؽ� ���� �߰� By wxywxy
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
	// ���� ������ �ݱ�
	//-----------------------------------------------------------------------------
	fclose( file );	

	//-----------------------------------------------------------------------------
	// ���ؽ� ����
	//-----------------------------------------------------------------------------
	if( create_buffer == true )
	{
		hr = g_RenderManager.CreateVertexBuffer( m_header.m_num_vertex*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY, 
											 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pVB );
		assert( hr == 0 && "!���ؽ� ���� �޸� �Ҵ� ����" );

		OBJECTVERTEX*	pOBJVERTEX;	 
#ifdef DIRECT_VERSION_9_MJH
		hr = m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 );
#else
		hr = m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );
#endif // DIRECT_VERSION_9_MJH	
		assert( hr == 0 && "!���ؽ� ���� �� ����" );			
		memcpy( pOBJVERTEX, m_pVertex, sizeof(OBJECTVERTEX)*m_header.m_num_vertex );
		hr = m_pVB->Unlock();
		assert( hr == 0 && "!���ؽ� ���� ��� ����" );

		//-----------------------------------------------------------------------------
		// ���ؽ����� �ʿ���ٸ� ����
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
// Desc: �ִϸ��̼ǰ� ���þ��� ���ؽ����� ����
//-----------------------------------------------------------------------------
void Model::DeleteVertex()
{

}