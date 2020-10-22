// EFFameItem.cpp: implementation of the CEFFameItem class.
//
//////////////////////////////////////////////////////////////////////

#include "CEFFameItem.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern long		 g_nowTime;


CEFFameItem::CEFFameItem()
{

}

CEFFameItem::~CEFFameItem()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}



int CEFFameItem::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	D3DXMATRIX   matTemp;
	D3DXVECTOR3  v0;
	D3DXVECTOR3  v1;
	
	// ¿À¸¥²ÞÄ¡  
	m_pSrcChr->GetBoneLinkTM( 14 , matTemp );
	v0.x = matTemp._41;
	v0.y = matTemp._42 ;
	v0.z = matTemp._43;

	// ¿À¸¥ÂÊ ¾î±ú 
	m_pSrcChr->GetBoneLinkTM( 12 , matTemp );
	v1.x = matTemp._41;
	v1.y = matTemp._42;
	v1.z = matTemp._43;
	m_Tail.AddPoint( v0 , v1 , g_lpDevice );



	// ¿À¸¥ÂÊ ¾î±ú 
	m_pSrcChr->GetBoneLinkTM( 14 , matTemp );
	v1.x = matTemp._41;
	v1.y = matTemp._42;
	v1.z = matTemp._43;
	// ¿À¸¥¼Õ 
	m_pSrcChr->GetBoneLinkTM( 5 , matTemp );
	v0.x = matTemp._41;
	v0.y = matTemp._42 ;
	v0.z = matTemp._43;
	m_Tail2.AddPoint( v0 , v1 , g_lpDevice );


	// ¿Þ ²ÞÄ¡  
	m_pSrcChr->GetBoneLinkTM( 15 , matTemp );
	v0.x = matTemp._41;
	v0.y = matTemp._42 ;
	v0.z = matTemp._43;

	// ¿Þ ¾î±ú 
	m_pSrcChr->GetBoneLinkTM( 13 , matTemp );
	v1.x = matTemp._41;
	v1.y = matTemp._42;
	v1.z = matTemp._43;
	m_Tail3.AddPoint( v0 , v1 , g_lpDevice );



	// ¿Þ ²ÞÄ¡ 
	m_pSrcChr->GetBoneLinkTM( 15 , matTemp );
	v1.x = matTemp._41;
	v1.y = matTemp._42;
	v1.z = matTemp._43;
	// ¿Þ¼Õ 
	m_pSrcChr->GetBoneLinkTM( 4 , matTemp );
	v0.x = matTemp._41;
	v0.y = matTemp._42 ;
	v0.z = matTemp._43;
	m_Tail4.AddPoint( v0 , v1 , g_lpDevice );
	
	
	


	
	//void AddPoint (D3DXVECTOR3 &v0,D3DXVECTOR3 &v1,LPDIRECT3DDEVICE8 pDevice)
	return TRUE;
}

int	CEFFameItem::Render(float dtime)
{
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
	m_Tail3.Render( g_lpDevice );
	m_Tail4.Render( g_lpDevice );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

void CEFFameItem::Init(void)
{
	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	//................................................................................	
	// ¹ßµ¿ »ç¿îµå
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_Tail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "jansang.TGA" );
	m_Tail.m_HashCode = HashString.GetHashCode();

	m_Tail2.InitDevice( g_lpDevice );
	m_Tail2.m_HashCode = HashString.GetHashCode();
//	
	m_Tail3.InitDevice( g_lpDevice );
	m_Tail3.m_HashCode = HashString.GetHashCode();
//
//
	m_Tail4.InitDevice( g_lpDevice );
	m_Tail4.m_HashCode = HashString.GetHashCode();
//
//


	//m_Tail.m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "jansang.TGA" );

}


HRESULT	CEFFameItem::InvalidateDeviceObject()
{
	m_Tail.InvalidateDeviceObject();
	m_Tail2.InvalidateDeviceObject();
	m_Tail3.InvalidateDeviceObject();
	m_Tail4.InvalidateDeviceObject();

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEFFameItem::RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CEFFameItem::RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{

	m_Tail.InitDevice( g_lpDevice );
	CStringToCode	HashString( "jansang.TGA" );
	m_Tail.m_HashCode = HashString.GetHashCode();

	m_Tail2.InitDevice( g_lpDevice );
	m_Tail2.m_HashCode = HashString.GetHashCode();


	
	m_Tail3.InitDevice( g_lpDevice );
	m_Tail3.m_HashCode = HashString.GetHashCode();


	m_Tail4.InitDevice( g_lpDevice );
	m_Tail4.m_HashCode = HashString.GetHashCode();



	//m_Tail.m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( "jansang.TGA" );

	return S_OK;
}

