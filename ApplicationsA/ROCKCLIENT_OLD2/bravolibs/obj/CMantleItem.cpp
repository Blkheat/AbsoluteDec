#include "CMantleItem.h"
#include "Object_Vertex.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH
#include "..\\RockClient.h"
#include "..\\..\\CRenderManager.h"

extern D3DXVECTOR3			g_AniVertex[3000];
extern D3DXVECTOR3			g_AniNormal[3000];
#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern CRockClient			g_RockClient;

CMantleItem::CMantleItem()
{
	for( int i = 0 ; i < MAX_MANTLE_TYPE_TEXTURE ; i++ )
	{
		m_pTexture[ i ] = NULL;
	}
}

CMantleItem::~CMantleItem()
{
	for( int i = 0 ; i < MAX_MANTLE_TYPE_TEXTURE ; i++ )
	{
		if( m_pTexture[ i ] != NULL )
		{
			m_pTexture[ i ]->Release();

			m_pTexture[ i ] = NULL;
		}
	}
}

void CMantleItem::Render(int race,int AniIndex,D3DXMATRIX matParent)
{
	Model& Mod = m_ModelList[ race ][ AniIndex ];

	if( !Mod.m_bInit ) return;

	g_RenderManager.SetTexture( 0 ,  m_pTexture[ m_MantleType ] ); 

	g_RenderManager.SetTransform( D3DTS_WORLD, &matParent );

	g_RenderManager.SetIndices( Mod.m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0, Mod.m_pVB, sizeof(OBJECTVERTEX) );

	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, Mod.m_header.m_num_vertex, 0 , Mod.m_header.m_num_face );
}

void CMantleItem::RenderShadow(int race,int AniIndex)
{
	Model& Mod = m_ModelList[ race ][ AniIndex ];

	if( !Mod.m_bInit ) return;

	g_RenderManager.SetIndices( Mod.m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0, Mod.m_pVB, sizeof(OBJECTVERTEX) );

	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, Mod.m_header.m_num_vertex, 0 , Mod.m_header.m_num_face );
}


// 종족 , 에니메이션 인덱스 , 망토 타입 , 현제 에니 값, 중간값		
void CMantleItem::Update(int race,int AniIndex,int Type,int ani_frame,float middle_time)
{
	Model& Mod = m_ModelList[ race ][ AniIndex ];

	if( !Mod.m_bInit ) return;

	int cur_frame = ani_frame;
	
	if( ( int ) Mod.m_header.m_ani_frame <= ani_frame )		
		cur_frame = 0;		

	int next_frame = cur_frame + 1;
	
	if( next_frame >= (int) ( int ) Mod.m_header.m_ani_frame )		
		next_frame = 0;		
	unsigned int i = 0;
	for( i = 0; i < Mod.m_header.m_num_vertex; ++i )
	{
		D3DXVec3Lerp( &g_AniVertex[ i ] , 
			&Mod.m_ppVertexList[ cur_frame ][ i ].m_vertex , 
			&Mod.m_ppVertexList[ next_frame                 ][ i ].m_vertex , middle_time );

		D3DXVec3Lerp( &g_AniNormal[ i ] , 
			&Mod.m_ppVertexList[ cur_frame ][ i ].m_normal , 
			&Mod.m_ppVertexList[ next_frame                 ][ i ].m_normal , middle_time );
	}			

	// 버퍼 락 걸기

	if( Mod.m_pVB != NULL )
	{
		OBJECTVERTEX*	pOBJVERTEX;	
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED( Mod.m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
		if( FAILED( Mod.m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		{
			return;		
		}				

		for( i = 0; i < Mod.m_header.m_num_vertex; ++i )
		{
			pOBJVERTEX[i].Vertex = g_AniVertex[i]; 		
			pOBJVERTEX[i].Normal = g_AniNormal[i]; 		
		}
		
		Mod.m_pVB->Unlock();	
	}

	SetType( Type );
}

// 종족 , 에니메이션 인덱스 , 풀 네임
int  CMantleItem::LoadMSH(int race,int AniIndex,int Name)
{
	if( Name == -1 ) return FALSE;

	char TempStr[ 1024 ] = "";

	wsprintf( TempStr , "%s\\Chr\\VAni\\%d.msh" , g_RockClient.GetRootDir() , Name );
	
	m_ModelList[ race ][ AniIndex ].Read( TempStr , TRUE );

	return TRUE;
}


int  CMantleItem::CreateTexture(void)
{
	for( int i = 0 ; i < MAX_MANTLE_TYPE_TEXTURE ; i++ )
	{
		char TempStr[ 1024 ] = "";

		wsprintf( TempStr , "%s\\Chr\\VAni\\%d.tex" , g_RockClient.GetRootDir() , i );
		
		if ( FAILED ( g_RenderManager.CreateTextureFromFile( TempStr , &m_pTexture[ i ] ) ) )
		{
//			MessageBox( NULL, "Error Texture Load ( Mantle )", "오류", NULL );
			
			return	false;
		}
	}

	return TRUE;
}

