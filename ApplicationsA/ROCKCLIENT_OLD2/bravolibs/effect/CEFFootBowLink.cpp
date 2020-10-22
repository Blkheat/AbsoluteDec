#include "CEFFootBowLink.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\..\\CRenderManager.h"

CEFFootBowLink::CEFFootBowLink()
{
	m_vAddPos  = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_fAddRad  = 0.0f;
	m_LinkBone = 0;
	
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixRotationX( &m_matWorld , D3DXToRadian( 270 ) );
}

CEFFootBowLink::~CEFFootBowLink()
{
}

int	CEFFootBowLink::Render(float dtime)
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matWorld );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	CEFArrow::Render( 0 );

	return TRUE;
}

int CEFFootBowLink::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	D3DXMATRIX matBone;

	m_pSrcChr->GetBoneLinkTM( m_LinkBone , matBone );
	
	m_matWorld._41 = matBone._41;
	m_matWorld._42 = matBone._42;
	m_matWorld._43 = matBone._43;

/*
	D3DXVECTOR3 vPos = D3DXVECTOR3( m_matWorld._41 , m_matWorld._42 , m_matWorld._43 );

    m_matWorld._42 = m_matWorld._43 = m_matWorld._41 = 0.0f;

	D3DXMATRIX matRot;

	D3DXMatrixRotationY( &m_matWorld , ( m_fAddRad + m_pSrcChr->m_radian ) );

	m_matWorld._41 = vPos.x + m_vAddPos.x;
	m_matWorld._42 = vPos.y + m_vAddPos.y;
	m_matWorld._43 = vPos.z + m_vAddPos.z;
*/
	return TRUE;
}

void CEFFootBowLink::SetExtraInfo(float AddRad,D3DXVECTOR3 vAddPos,int LinkBone)
{
	m_fAddRad  = AddRad - m_pSrcChr->m_radian;
	
	m_LinkBone = LinkBone;
	m_vAddPos  = vAddPos;
}


