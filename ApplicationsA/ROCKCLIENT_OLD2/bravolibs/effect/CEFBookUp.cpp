#include "CEFBookUp.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

CEFBookUp::CEFBookUp()
{
	m_Level		 = 0;
	m_WeaponType = 0;
}

CEFBookUp::~CEFBookUp()
{
}

void CEFBookUp::Init(void)
{
	// 인첸트 레벨 체크
	int ParNum = -1;
	// 0 ~ 4 기본 박스 이펙트 , 5 ~ 부가토토 
	switch( m_Level )
	{
	case 0:	
		ParNum = 22031;
		break;
	case 1:	
		ParNum = 22032;
		break;
	case 2:	
		ParNum = 22033;
		break;
	case 3:	
		ParNum = 22034;
		break;
	case 4:	
		ParNum = 22035;
		break;
	case 5:
		ParNum = 24555;
		break;
	case 6: 
		ParNum = 24556;
		break; 

		
	default:	
		SetDestroy();
		return;
	}

//	CParticleData	*pData = NULL;
	CBillboardData  *pBillData = NULL ;

//	if( m_Level == 2 || m_Level == 3 || m_Level == 4 )
	{
		if( NULL == ( pBillData = g_Billboard.m_BillboardData.GetKeyDataPointer( ParNum ) ) )
		{
			SetDestroy();
			return;
		}
		
		ENCHANT_POINT TempPoint;
		
		TempPoint.Range = 0.5f;
		
		m_EnChantList.push_back( TempPoint );

		m_Billboard.MakeLink( pBillData );
	}
/*	else
	{
		if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( ParNum ) ) == NULL )
			return;

		ENCHANT_POINT TempPoint;
		
		TempPoint.Range = 0.5f;
		
		m_EnChantList.push_back( TempPoint );
		
		m_ParticleLink.MakeLink( pData );
	}
*/
}

int CEFBookUp::Update(float dtime)
{
	return TRUE;
}

int	CEFBookUp::Render(float dtime)
{
	return TRUE;
}

int	CEFBookUp::SortRender(float dtime)
{
	
	if( m_State == ES_SELF_DESTROY )
	{
		return TRUE;
	}
	
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();
		return TRUE;
	}

	if( g_Particle.m_EffectManager.ISOverLength( m_pSrcChr ) )
	{
		return TRUE;
	}
	
	if( !m_pSrcChr->DrawWeaponCheckStatus() )
		return TRUE;

	if( !m_pSrcChr->m_IsAllLTexture )
		return TRUE;
	
	D3DXVec3Lerp( &m_EnChantList[ 0 ].vPos , &m_pSrcChr->m_pUserInfo->vSwordAxis[ 1 ] , &m_pSrcChr->m_pUserInfo->vSwordAxis[ 0 ] , m_EnChantList[ 0 ].Range );

	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );	
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_INVSRCALPHA );
	
	g_RenderManager.SetLighting( TRUE );
	
//	if( m_Level == 2 || m_Level == 3 || m_Level == 4 )	
	{
		matLocal = m_pSrcChr->GetLHandTM();

		m_Billboard.m_vLoc.x = 0.0f;
		m_Billboard.m_vLoc.y = 0.0f;
		m_Billboard.m_vLoc.z = 0.0f;
		
		g_Billboard.DisplayManager( m_Billboard , dtime );

		g_RenderManager.SetZBias( 1 );
		
		m_Billboard.m_matWorld = matLocal;

#ifdef BATCH_RENDER
		m_Billboard.DrawBatchBillboard();
#ifdef DIRECT_VERSION_9_MJH
		LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
		LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
		if(prevBillboardTexture)
		{
			g_RenderManager.SetTexture( 0, prevBillboardTexture );
			g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
			g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
			g_RenderManager.EndBatchPrimitive();
			g_RenderManager.SetBeginBatchRendering(FALSE);
	}
#else	//	BATCH_RENDER
		m_Billboard.DrawBillboard();
#endif	//	BATCH_RENDER
		
	
		g_RenderManager.SetZBias( 0 );
	}
/*	else
	{
		m_ParticleLink.SetLinkPos( m_EnChantList[ 0 ].vPos );
		
		m_ParticleLink.UpdateLink( dtime );
		
		m_ParticleLink.Render();
	}
*/
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );

	return TRUE;
}
