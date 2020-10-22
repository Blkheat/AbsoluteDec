#include "RockPCH.h"
#include "VFileSystem.h"

#include "CEFRockPress.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include <SOUND\Sound.H>
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include "..\\RockClient.h"
#include "..\\quadlist.h"
#include <Map\\Field.h>
#include "..\\Map.H"
#include "..\\PC.H"
#include "..\\Effect.H"
#include "..\\Camera.H"
#include "..\\RockClient.h"

#include "..\\..\\CRenderManager.h"

extern CBravoMap g_Map;
extern long		 g_nowTime;


#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CEFRockPress::m_pRockTexture		= NULL;
#else
LPDIRECT3DTEXTURE8	CEFRockPress::m_pRockTexture		= NULL;
#endif // DIRECT_VERSION_9_MJH
Model			   *CEFRockPress::m_pRockModelList[ 3 ] = { NULL , };

int CEFRockPress::InitDevice(void)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	try
	{
		if( FAILED ( g_RenderManager.CreateTextureFromFile( "Effect\\Static\\Rock1.jpg" , &m_pRockTexture ) ) )
			throw(0);

		m_pRockModelList[ 0 ] = SAFE_NEW( Model );		
		m_pRockModelList[ 1 ] = SAFE_NEW( Model );
		m_pRockModelList[ 2 ] = SAFE_NEW( Model );
		
		if( NULL == m_pRockModelList[ 0 ] || NULL == m_pRockModelList[ 1 ] || NULL == m_pRockModelList[ 2 ])
			throw(1);

		if( !m_pRockModelList[ 0 ]->Read( "Effect\\Static\\Rock.msh" , TRUE ) )
			throw(1);

		if( !m_pRockModelList[ 1 ]->Read( "Effect\\Static\\Rock1.msh" , TRUE ) )
			throw(1);

		if( !m_pRockModelList[ 2 ]->Read( "Effect\\Static\\Rock2.msh" , TRUE ) )
			throw(1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	try
	{
		VFileHandle* pFH = g_VFEffectStatic.OpenFile( "Rock1.jpg" );	
		if( pFH )
		{
			if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pRockTexture ) ) )
			{
				g_VFEffectStatic.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
				throw(0);
			}			
			
			g_VFEffectStatic.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}					

		m_pRockModelList[ 0 ] = SAFE_NEW( Model );
		m_pRockModelList[ 1 ] = SAFE_NEW( Model );
		m_pRockModelList[ 2 ] = SAFE_NEW( Model );
		
		if( NULL == m_pRockModelList[ 0 ] || NULL == m_pRockModelList[ 1 ] || NULL == m_pRockModelList[ 2 ])
			throw(1);

		if( !m_pRockModelList[ 0 ]->ReadVF( &g_VFEffectStatic, "Rock.msh" , TRUE ) )
			throw(1);

		if( !m_pRockModelList[ 1 ]->ReadVF( &g_VFEffectStatic, "Rock1.msh" , TRUE ) )
			throw(1);

		if( !m_pRockModelList[ 2 ]->ReadVF( &g_VFEffectStatic, "Rock2.msh" , TRUE ) )
			throw(1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	

	catch(int Type)
	{
		switch( Type )
		{
			case 0:
///				MessageBox( NULL, "Error Texture Load", "Effect RockPress", MB_OK );
				break;
			case 1:
///				MessageBox( NULL , "Error Load Model" , "Effect RockPress" , MB_OK );
				break;
		}

		CEFRockPress::DeleteDevice();
		
		return FALSE;
	}
	
	return TRUE;
}

int	CEFRockPress::DeleteDevice(void)
{
	SAFE_RELEASE( m_pRockTexture );
	
	SAFE_DELETE( m_pRockModelList[ 0 ] );
	SAFE_DELETE( m_pRockModelList[ 1 ] );
	SAFE_DELETE( m_pRockModelList[ 2 ] );
	
	return TRUE;
}


CEFRockPress::CEFRockPress()
{
	m_IsConEffect = TRUE;
}

CEFRockPress::~CEFRockPress()
{
}

void CEFRockPress::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	switch( m_ID )
	{
		case EF_ROCKPRESS0:
		case EF_ROCKPRESS1:
			
			m_RockInfo.Type = 0;
			
			break;

		case EF_ROCKPRESS2:
		case EF_ROCKPRESS3:
			
			m_RockInfo.Type = 1;
			
			break;

		case EF_ROCKPRESS4:
			
			m_RockInfo.Type = 2;
			
			break;
	}


	m_RockInfo.StartHeight = 50.0f;

	//
	 
	m_RockInfo.OldTimer    = g_nowTime;

	m_RockInfo.WaitTimer   = 1000;
	m_RockInfo.fDir		   = -150.8f;

	m_vSkyEffectPos.x      = m_pDestChr->GetPosTM()._41;
	m_vSkyEffectPos.y      = m_pDestChr->GetPosTM()._42 + m_RockInfo.StartHeight + 10.0f;
	m_vSkyEffectPos.z      = m_pDestChr->GetPosTM()._43;


	// 구름 이펙트 시작
	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_ROCKPRESS_CLOUD ) ) == NULL )
		return;

	m_CloudParticle.MakeLink( pData );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
}


int CEFRockPress::Render(float dtime)
{
	if( m_RockInfo.State == ROCKPRESS_SELF_DESTROY )
		return TRUE;

	D3DXMATRIX	matWorld;
	
	D3DXMatrixIdentity( &matWorld );
	
	DWORD dFVF = g_RenderManager.GetFVF();
	
	if( m_RockInfo.State == ROCKPRESS_START_WAIT )
	{
		m_CloudParticle.SetLinkPos( m_RockInfo.vPos );
		m_CloudParticle.UpdateLink( dtime );

		g_RenderManager.SetZWriteEnable( FALSE );		
		m_CloudParticle.Render();
		g_RenderManager.SetZWriteEnable( TRUE );
	}

	if( m_RockInfo.State != ROCKPRESS_FALL )
	{
#ifdef DIRECT_VERSION_9_MJH
		D3DMATERIAL9	mtrl;	
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
#else
		D3DMATERIAL8	mtrl;	
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH
		
		mtrl.Power = 1.0f;
		
		mtrl.Diffuse.r = mtrl.Ambient.r = m_RockInfo.fAlpha;
		mtrl.Diffuse.g = mtrl.Ambient.g = m_RockInfo.fAlpha;
		mtrl.Diffuse.b = mtrl.Ambient.b = m_RockInfo.fAlpha;
		mtrl.Diffuse.a = mtrl.Ambient.a = m_RockInfo.fAlpha;

		g_RenderManager.SetMaterial( &mtrl );

		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( FALSE );
		g_RenderManager.SetZEnable( FALSE );
		
		g_RenderManager.SetAlphaBlendEnable(  TRUE );
		g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
		g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
		
		g_RenderManager.LightEnable( 0 , TRUE );
		g_RenderManager.LightEnable( 1 , TRUE );
		
		g_RenderManager.SetLighting( TRUE );
	}
	else
	{
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( FALSE );
		g_RenderManager.SetZEnable( FALSE );
		
		g_RenderManager.SetAlphaBlendEnable(  FALSE );
		
		g_RenderManager.LightEnable(0,FALSE);
		g_RenderManager.LightEnable(1,FALSE);

		g_RenderManager.SetLighting( FALSE );
	}

	g_RenderManager.SetTexture( 0, m_pRockTexture );			

	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	matWorld._11 = 1.0f;
	matWorld._22 = 1.0f;
	matWorld._33 = 1.0f;

	matWorld._41 = m_RockInfo.vPos.x;
	matWorld._42 = m_RockInfo.vPos.y;
	matWorld._43 = m_RockInfo.vPos.z;

	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );	
	
	g_RenderManager.SetIndices( m_pRockModelList[ m_RockInfo.Type ]->m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , m_pRockModelList[ m_RockInfo.Type ]->m_pVB , sizeof(OBJECTVERTEX) );

	DrawIndexedPrimitive( 
		m_pRockModelList[ m_RockInfo.Type ]->m_header.m_num_vertex , 
		m_pRockModelList[ m_RockInfo.Type ]->m_header.m_num_face     );
	
	g_RenderManager.SetTexture( 0, NULL );			

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	g_RenderManager.LightEnable(0,FALSE);
	g_RenderManager.LightEnable(1,FALSE);
	
	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetFVF( dFVF );

	return TRUE;
}

int CEFRockPress::CheckFrame(void)
{
	return TRUE;
}

int CEFRockPress::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	if( m_RockInfo.State == ROCKPRESS_LANDING_WAIT )
	{
		m_RockInfo.fAlpha = ( float ) 1.0f - ( SAFE_TIME_SUB( g_nowTime , m_RockInfo.OldTimer ) / ( float ) m_RockInfo.WaitTimer );

		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_RockInfo.OldTimer ) , >= , m_RockInfo.WaitTimer ) )
		{
			SetDestroy();
			
			m_State = ROCKPRESS_SELF_DESTROY;

			m_RockInfo.OldTimer = g_nowTime;
		}


		return TRUE;
	}

	//............................................................................................................
	// 시간 처리
	//............................................................................................................
	switch( m_RockInfo.State )
	{
		case ROCKPRESS_START_WAIT:
			
			m_RockInfo.fAlpha = ( float ) SAFE_TIME_SUB( g_nowTime , m_RockInfo.OldTimer ) / ( float ) m_RockInfo.WaitTimer;

			m_RockInfo.vPos.x = m_pDestChr->GetPosTM()._41;
			m_RockInfo.vPos.y = m_pDestChr->GetPosTM()._42 + m_RockInfo.StartHeight;
			m_RockInfo.vPos.z = m_pDestChr->GetPosTM()._43;

			if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_RockInfo.OldTimer ) , >= , m_RockInfo.WaitTimer ) )
			{
				m_RockInfo.OldTimer = g_nowTime;
				
				m_RockInfo.State   = ROCKPRESS_FALL;
			}
		
			break;
			
		case ROCKPRESS_FALL:
			{
			
				m_RockInfo.vPos.y += m_RockInfo.fDir * dtime;
				
				m_RockInfo.vPos.x = m_pDestChr->GetPosTM()._41;
				m_RockInfo.vPos.z = m_pDestChr->GetPosTM()._43;
				
				float fLandHeight = g_Map.Get__MapHeightF( m_RockInfo.vPos.x , m_RockInfo.vPos.z );
				
				if( m_RockInfo.vPos.y <= fLandHeight )
				{
					m_RockInfo.vPos.y = fLandHeight;
					
					int EffectID;
					
					switch( m_ID )
					{
					case EF_ROCKPRESS0:
					case EF_ROCKPRESS1:
						
						EffectID = EF_ROCKPRESS0;
						
						break;
						
					case EF_ROCKPRESS2:
					case EF_ROCKPRESS3:
						
						EffectID = EF_ROCKPRESS2;
						
						break;
						
					case EF_ROCKPRESS4:
						
						EffectID = EF_ROCKPRESS4;
						
						break;
					}
					
					g_Particle.SetEffSetCon( 0.0f , EffectID , 100 , 0.3f , -1 , -1 , m_RockInfo.vPos );

					D3DXMATRIX matBone;
					m_pDestChr->GetBoneLinkTM( 2 , matBone );
					D3DXVECTOR3 vDestPos( matBone._41 ,
										  matBone._42 ,
										  matBone._43 );
					
					g_Billboard.SetManager( 0.0f , BB_ROCKPRESS_DAMAGE , vDestPos );
					

					if( m_pSrcChr == g_Pc.GetPlayer() )
						g_Particle.RockCamera( ( rand() % 200 ) + 300 );
					
					// 데미지 처리
					SetAfterEffect();

					m_RockInfo.State = ROCKPRESS_LANDING_WAIT;

					m_RockInfo.OldTimer = g_nowTime;

					m_RockInfo.fAlpha   = 1.0f;
				}

			}

			break;
	}		

	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFRockPress::SetAfterEffect(void)
{
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	return TRUE;
}
