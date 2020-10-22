#include "CEFBunchStrength.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFBunchStrength::CEFBunchStrength()
{
}

CEFBunchStrength::~CEFBunchStrength()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFBunchStrength::Update(float dtime)
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
			m_Tail2.Clear();
		}

		return TRUE;
	}
	
	D3DXMATRIX   matTemp;
	D3DXVECTOR3  vHead;
	D3DXVECTOR3  vCenter;
	
	int nNode = 0;

	if( m_pSrcChr->GetAttrIndex() == n_Rain_Man )
	{
		nNode = 14;
	}
	else
	{
		nNode = 20;
	}	
	
	m_pSrcChr->GetDirectBoneLinkTM( nNode , matTemp );	
	
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	if( m_pSrcChr->GetAttrIndex() == n_Rain_Man )
	{
		nNode = 15;
	}
	else
	{
		nNode = 21;
	}
	
	m_pSrcChr->GetDirectBoneLinkTM( nNode , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_Tail.AddPoint( vCenter , vHead , g_lpDevice );

	if( m_pSrcChr->GetAttrIndex() == n_Rain_Man )
	{
		nNode = 22;
	}
	else
	{
		nNode = 28;
	}

	m_pSrcChr->GetDirectBoneLinkTM( nNode , matTemp );
	vHead.x = matTemp._41;
	vHead.y = matTemp._42;
	vHead.z = matTemp._43;

	if( m_pSrcChr->GetAttrIndex() == n_Rain_Man )
	{
		nNode = 23;
	}
	else
	{
		nNode = 29;
	}

	m_pSrcChr->GetDirectBoneLinkTM( nNode , matTemp );
	vCenter.x = matTemp._41;
	vCenter.y = matTemp._42;
	vCenter.z = matTemp._43;

	m_Tail2.AddPoint( vCenter , vHead , g_lpDevice );

	return TRUE;
}

int	CEFBunchStrength::Render(float dtime)
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
	m_Tail2.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

void CEFBunchStrength::Init(void)
{
	if( m_pSrcChr->GetAttrIndex() != n_Rain_Man && 
		m_pSrcChr->GetAttrIndex() != n_Rain_Woman )
	{
		SetDestroy();
		return;
	}
		
	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_Tail.InitDevice( g_lpDevice );
	m_Tail2.InitDevice( g_lpDevice );
	
	CStringToCode	HashString( "afterimage3.tga" );
	m_Tail.m_HashCode = HashString.GetHashCode();
	m_Tail2.m_HashCode = HashString.GetHashCode();

	//m_Tail.m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "jansang.TGA" );

}


HRESULT	CEFBunchStrength::InvalidateDeviceObject()
{
	m_Tail.InvalidateDeviceObject();
	m_Tail2.InvalidateDeviceObject();

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEFBunchStrength::RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CEFBunchStrength::RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{

	m_Tail.InitDevice( g_lpDevice );
	m_Tail2.InitDevice( g_lpDevice );

	CStringToCode	HashString( "afterimage3.tga" );
	m_Tail.m_HashCode = HashString.GetHashCode();
	m_Tail2.m_HashCode = HashString.GetHashCode();

	//m_Tail.m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "jansang.TGA" );

	return S_OK;
}

