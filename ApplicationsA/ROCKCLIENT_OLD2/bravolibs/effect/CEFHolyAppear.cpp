#include "CEFHolyAppear.h"
#include "..\\Effect.h"

extern long		 g_nowTime;

CEffectModel  CEFHolyAppear::m_BaseModel;
D3DXVECTOR3  *CEFHolyAppear::m_pVertexList;
UINT	  	  CEFHolyAppear::m_VertexCount;

CEFHolyAppear::CEFHolyAppear()
{
}

CEFHolyAppear::~CEFHolyAppear()
{
	DeleteAllMemory();
}

int CEFHolyAppear::InitDevice(void)
{
	return FALSE;

	if( !CEFHolyAppear::m_BaseModel.InitDevice( "appearholy.msh" , NULL ) )
		return FALSE;

	CEFHolyAppear::m_pVertexList = NULL;
	CEFHolyAppear::m_VertexCount = 0;
	
	CEFHolyAppear::m_VertexCount = CEFHolyAppear::m_BaseModel.m_pModel->m_header.m_num_vertex;

	CEFHolyAppear::m_pVertexList = SAFE_NEW_ARRAY( D3DXVECTOR3 , CEFHolyAppear::m_VertexCount );	
	
	if( NULL == CEFHolyAppear::m_pVertexList )
	{
		CEFHolyAppear::m_BaseModel.DeleteDevice();
	
		return FALSE;
	}
		
	
	for( int i = 0 ; i < CEFHolyAppear::m_VertexCount ; ++i )
	{
		CEFHolyAppear::m_pVertexList[ i ] = CEFHolyAppear::m_BaseModel.m_pModel->m_ppVertexList[ 0 ][ i ].m_vertex;
	}

	CEFHolyAppear::m_BaseModel.DeleteDevice();

	return TRUE;
}

int CEFHolyAppear::DeleteDevice(void)
{
	
	SAFE_DELETE_ARRAY( CEFHolyAppear::m_pVertexList );

	return TRUE;
}


void CEFHolyAppear::DeleteAllMemory(void)
{
	SAFE_DELETE_ARRAY( m_pProcessList );
}

void CEFHolyAppear::Init(void)
{
	m_State = HOLYAPPEAR_START;

	m_pProcessList = NULL;

	m_pProcessList = SAFE_NEW_ARRAY( D3DXVECTOR3 , CEFHolyAppear::m_VertexCount );

	if( NULL == m_pProcessList )
	{
		SetDestroy();

		return;
	}
	
	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 9111666 ) ) == NULL )
//	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_FIREBALL ) ) == NULL )
	{
		SetDestroy();

		return;
	}

	m_ParticleLink.MakeLink( pData );

	m_vStart.x = m_pSrcChr->GetPosTM()._41;
	m_vStart.y = m_pSrcChr->GetPosTM()._42;
	m_vStart.z = m_pSrcChr->GetPosTM()._43;

	m_State = HOLYAPPEAR_RUN;
	
	m_pSrcChr->m_CharAlpha.Set( CHAR_HIDE , 0.0f );

	m_OldTime = g_nowTime;
}

int CEFHolyAppear::Render(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	if( m_State == HOLYAPPEAR_START )
		return TRUE;

	D3DXMATRIX matLocal;

	D3DXMatrixIdentity( &matLocal );

	matLocal._41 = m_vStart.x;
	matLocal._42 = m_vStart.y;
	matLocal._43 = m_vStart.z;

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );

	D3DXVECTOR3 vPos;
	
	for( UINT i = 0; i < CEFHolyAppear::m_VertexCount ; i++ )
	{
		m_ParticleLink.SetLinkPos( m_pProcessList[ i ] );
		m_ParticleLink.UpdateLink( dtime );
		m_ParticleLink.Render();
	}
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFHolyAppear::Update(float dtime)
{
	if( m_State == HOLYAPPEAR_START )
		return TRUE;

	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
	
		return TRUE;
	}

	if(m_State == HOLYAPPEAR_RUN )
	{
		float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTime ) / ( float ) 3000 ) );
	
		if( Range > 1.0f ) 
		{
			Range = 1.0f;
		
			m_State = HOLYAPPEAR_WAIT;

			m_OldTime = g_nowTime;
		}

		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vDest;
		D3DXVECTOR3 vDistance;
		D3DXMATRIX  matRot;
		D3DXMATRIX  matScale;
		D3DXMATRIX  matLocal;

		D3DXMatrixRotationY( &matRot   , D3DXToRadian(  180.0f * ( 1.0f - Range ) ) );
		
		D3DXMatrixScaling( &matScale   , ( 1.0f - Range ) * 50.0f + 1.0f ,
										 ( 1.0f - Range ) * 5.0f + 1.0f , 
										 ( 1.0f - Range ) * 50.0f + 1.0f );

		D3DXMatrixMultiply( &matLocal  , &matScale , &matRot );

		for( UINT i = 0; i < CEFHolyAppear::m_VertexCount ; i++ )
		{
			if( Range == 1.0f )
			{
				m_pProcessList[ i ] = m_vStart + CEFHolyAppear::m_pVertexList[ i ]; 

				m_pProcessList[ i ].y = m_vStart.y + CEFHolyAppear::m_pVertexList[ i ].y + 20.0f;				
			}
			else
			{
				D3DXVec3TransformCoord( &vDest , &CEFHolyAppear::m_pVertexList[ i ] , &matLocal );

//				vDest.x +=  m_vStart.x;
//				vDest.y +=  m_vStart.y;// - ( vDest.y * ( 1.0f - Range ) );
//				vDest.z +=  m_vStart.z;

//				vDistance = vDest - ( CEFHolyAppear::m_pVertexList[ i ] + m_vStart );
//				vDistance = vDest - CEFHolyAppear::m_pVertexList[ i ];
				vDistance = vDest - CEFHolyAppear::m_pVertexList[ i ];
				
				m_pProcessList[ i ] = m_vStart + ( vDistance * Range );
				
				m_pProcessList[ i ].y += ( 20.0f + ( 100.0f * ( 1.0f - Range ) ) );
			}
		}
	
	}
	else
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 5000 ) )
		{
			g_Particle.SetEffSetCon( 0.0f , 725 , 0 , 3.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			m_pSrcChr->m_CharAlpha.Clear();
		
			SetDestroy();
			
			return TRUE;
		}
	}

	return TRUE;
}
