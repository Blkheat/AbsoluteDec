#include "CArrowStuck.h"
#include "..\\Effect.h"
#include "..\\..\\CRenderManager.h"

CArrowStuck::CArrowStuck()
{
	m_fAddRad  = 0.0f;
	m_vAddPos  = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_LinkBone = 0;
}

CArrowStuck::~CArrowStuck()
{

}

int	CArrowStuck::Render(float dtime)
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matWorld );
	
    g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	CEFArrow::Render( 0 );

	return TRUE;
}

int CArrowStuck::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldTimer ) , >= , m_RangeTime ) )
	{
		SetDestroy();
	
		return TRUE;
	}

	m_pSrcChr->GetBoneLinkTM( m_LinkBone , m_matWorld );
	
	D3DXVECTOR3 vPos = D3DXVECTOR3( m_matWorld._41 , m_matWorld._42 , m_matWorld._43 );

	m_matWorld._42 = m_matWorld._43 = m_matWorld._41 = 0.0f;

	D3DXMATRIX matRot;

	D3DXMatrixRotationY( &m_matWorld , ( m_fAddRad + m_pSrcChr->m_radian ) );

	m_matWorld._41 = vPos.x + m_vAddPos.x;
	m_matWorld._42 = vPos.y + m_vAddPos.y;
	m_matWorld._43 = vPos.z + m_vAddPos.z;

	return TRUE;
}

void CArrowStuck::Init(void)
{
	m_RangeTime = 3000;
	m_OldTimer  = timeGetTime();
}
	
void CArrowStuck::SetExtraInfo(float AddRad,D3DXVECTOR3 vAddPos,int LinkBone)
{
	m_fAddRad  = AddRad - m_pSrcChr->m_radian;
	
	m_LinkBone = LinkBone;
	m_vAddPos  = vAddPos;
}
