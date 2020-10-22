#include "CEFBigNpc0.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


CEFBigNpc0::CEFBigNpc0()
{
	m_IsConEffect = TRUE;
	m_HashCode = 0;
}


CEFBigNpc0::~CEFBigNpc0()
{

}

void CEFBigNpc0::Init(void)
{

	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	m_Code = g_Map.m_NpcTable[ m_pSrcChr->m_nTableIndex ].code;
	
	int SoundNum = 0;

	switch( m_Code )
	{
		case 3800:
			SoundNum = 286;
			m_ImpactFrame = 82;
			g_Particle.SetEffSetCon( 0.0f , EF_BIG_NPC_ATTACK0 , ESLT_CHARPOS , 10.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			break;
		case 3801:
			SoundNum = 287;
			m_ImpactFrame = 85;
			g_Particle.SetEffSetCon( 0.0f , 707 , ESLT_CHARPOS , 10.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			break;
	}

	D3DXVECTOR3 vDistance;

	vDistance = D3DXVECTOR3( m_pSrcChr->GetPosTM()._41  , 0.0f , m_pSrcChr->GetPosTM()._43 ) - 
				D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41 , 0.0f , g_Pc.GetPlayer()->GetPosTM()._43 );
		
	if( D3DXVec3Length( &vDistance ) <= 800 )
	{
		g_Sound.Play( SoundNum , NULL );
	}

	
	if( m_pDestChr == g_Pc.GetPlayer() )
	{
		if( m_pExtraInfo->TargetInfo.theDieFlag == TRUE )
		{
			switch( m_Code )
			{
			case 3800:
				g_Particle.RockCamera( 2000 );
				break;
				
			case 3801:
				g_Particle.RockCamera( 2000 );
				break;
			}
		}
	}

	//...........................................................................................................
	if( m_Code == 3801 )
	{
		m_vStartPos.x = m_pSrcChr->m_matRealHair._41;
		m_vStartPos.y = m_pSrcChr->m_matRealHair._42;
		m_vStartPos.z = m_pSrcChr->m_matRealHair._43;

		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		m_vDestPos = 	D3DXVECTOR3 ( matBone._41 ,
									  matBone._42 ,
									  matBone._43 );

		m_RangeTime0 = PositionToTimer( 1  , m_vStartPos  , m_vDestPos );
		m_RangeTime1 = PositionToTimer( 10 , m_vStartPos , m_vDestPos );
		
		m_BillLine.InitDevice( g_lpDevice );


		m_vPos[ 0 ] = m_vPos[ 1 ]  = m_vStartPos;

		m_pTexture = NULL;
		
		//m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "laser_up.tga" );
		CStringToCode	HashString( "laser_up.tga" );
		m_HashCode = HashString.GetHashCode();
		
	}
	else
	{
		m_Type = 0;
	}
}


int	CEFBigNpc0::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE, TRUE);
		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		SetAfterEffect();
		SetDestroy();
	}	
	
	if( m_pSrcChr->m_ani_frame == m_ImpactFrame )
	{
		if( m_Code == 3801 )
		{
			m_Type = 1;

			m_OldTimer = g_nowTime;

			m_pSrcChr->m_IsApplyVA = TRUE;
			m_pSrcChr->m_IsNextApplyVA = TRUE;
			m_pSrcChr->BoneRender();
			
			m_vStartPos.x = m_pSrcChr->m_matRealHair._41;
			m_vStartPos.y = m_pSrcChr->m_matRealHair._42;
			m_vStartPos.z = m_pSrcChr->m_matRealHair._43;
		}
		else
		{
			SetAfterEffect();
			SetDestroy();
		}
	}

	return TRUE;
}

int	CEFBigNpc0::Render(float dtime)
{
	if( m_Type == 1 )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD , &g_Particle.m_matIdentity );
	
		g_RenderManager.SetCullMode( D3DCULL_NONE );
	
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable(TRUE );
	
		g_RenderManager.SetAlphaBlendEnable( TRUE );
		g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
		g_RenderManager.SetDestBlend( D3DBLEND_ONE );

		g_RenderManager.SetTexture( 0 , g_Particle.m_EffectTextureManager.GetTexture(m_HashCode) );
		m_BillLine.Render( g_lpDevice );

		g_RenderManager.SetAlphaBlendEnable( TRUE );
		g_RenderManager.SetCullMode( D3DCULL_CCW );
	}
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	return TRUE;
}

int CEFBigNpc0::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr) )
	{
		SetDestroy(TRUE, TRUE);
		return TRUE;
	}

	if( m_Type == 1 )
	{
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		m_vDestPos = 	D3DXVECTOR3 ( matBone._41 ,
									  matBone._42 ,
									  matBone._43 );
		
		D3DXVECTOR3 vDir;
		
		vDir = m_vDestPos - m_vStartPos;

		float       Range0     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime0 ) );
		float       Range1     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime1 ) );

		if( Range0 > 1.0f ) 
			Range0 = 1.0f;

		if( Range1 > 1.0f )
		{
			Range1 = 1.0f;

			m_Type = 0;

			SetAfterEffect();
			SetDestroy();
		}

		m_vPos[ 0 ] = m_vStartPos + ( vDir * Range0 );  
		m_vPos[ 1 ] = m_vStartPos + ( vDir * Range1 );  

		m_BillLine.Update( m_vPos[ 0 ] , m_vPos[ 1 ] , 2.5f , g_Camera.m_vDir , g_lpDevice );

	}

	return TRUE;
}

int CEFBigNpc0::SetAfterEffect(void)
{
	g_Particle.SetEffSetCon( 0.0f , EF_BIG_NPC_ATTACK0ED , ESLT_POS , 8.1f , -1 , -1 , 
		D3DXVECTOR3( m_pDestChr->GetPosTM()._41 , m_pDestChr->GetPosTM()._42 + 10.0f, m_pDestChr->GetPosTM()._43  ) );

	g_Particle.m_EffectManager.StandTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
