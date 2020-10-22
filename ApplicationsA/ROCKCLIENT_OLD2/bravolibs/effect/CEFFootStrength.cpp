#include "CEFFootStrength.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFFootStrength::CEFFootStrength()
{
}

CEFFootStrength::~CEFFootStrength()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFFootStrength::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
	
	if( m_pSrcChr->m_curt_event.type == NULL )
	{
		if( m_Tail.GetSize() )
		{
			m_Tail.Clear();		
		}

		return TRUE;
	}

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;
	
	// 머리
	m_pSrcChr->GetBoneLinkTM( 1 , matTemp );
	vHead.x = matTemp._41;
	vHead.y = matTemp._42 + 2.0f;
	vHead.z = matTemp._43;

	// 낭심 
	m_pSrcChr->GetBoneLinkTM( 3 , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_Tail.AddPoint( vCenter , vHead , g_lpDevice );

	return TRUE;
}

int	CEFFootStrength::Render(float dtime)
{
	if( m_pSrcChr->m_curt_event.type == NULL )
	{
		return TRUE;
	}	
	
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	m_Tail.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

void CEFFootStrength::Init(void)
{
	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_Tail.InitDevice( g_lpDevice );
	
	CStringToCode	HashString( "jansang.TGA" );
	m_Tail.m_HashCode = HashString.GetHashCode();

	//m_Tail.m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "jansang.TGA" );

}


HRESULT	CEFFootStrength::InvalidateDeviceObject()
{
	m_Tail.InvalidateDeviceObject();

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEFFootStrength::RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CEFFootStrength::RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{

	m_Tail.InitDevice( g_lpDevice );

	CStringToCode	HashString( "jansang.TGA" );
	m_Tail.m_HashCode = HashString.GetHashCode();

	//m_Tail.m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "jansang.TGA" );

	return S_OK;
}

