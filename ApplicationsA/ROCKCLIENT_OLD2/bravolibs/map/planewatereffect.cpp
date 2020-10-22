#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	"base\\d3dapp.h"
#include	"Water.h"
#include	"PlaneWaterEffect.h"
#include	"Field.h"
#include	"..\\Map.h"
#include	<mmsystem.h>

#include "..\\..\\CRenderManager.h"

CPlaneWaterEffect::CPlaneWaterEffect()
{
	m_pIB		= NULL;
	m_pVB		= NULL;
	m_pVtxList	= NULL;
	m_pIdxList	= NULL;
	D3DXMatrixIdentity(&m_matPos);
	m_bIsVisible	= false;
}

CPlaneWaterEffect::~CPlaneWaterEffect()
{
	FinalCleanup();
}

void	CPlaneWaterEffect::FinalCleanup()
{
	_RELEASE(m_pIB);
	_RELEASE(m_pVB);
}

#ifdef DIRECT_VERSION_9_MJH
bool	CPlaneWaterEffect::InitDevice(LPDIRECT3DDEVICE9 pdev)
#else
bool	CPlaneWaterEffect::InitDevice(LPDIRECT3DDEVICE8 pdev)
#endif // DIRECT_VERSION_9_MJH
{
	if ( !pdev )
		return	false;
	m_pDevice	= pdev;
	return	true;
}

bool	CPlaneWaterEffect::Create(BYTE dir, float x, float y, float z)
{
	if ( !m_pDevice )
		return	false;

	m_fAngle	= (float)dir * 45.0f;
	SetupRotTM(m_fAngle);

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(6*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB) ) )
	{
//		MessageBox(NULL, "d3d index buffer create failed", "water", MB_OK);
		return	false;
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(4*sizeof(VTXWATER), D3DUSAGE_WRITEONLY, D3DFVF_WATER, D3DPOOL_MANAGED, &m_pVB) ) )
	{
//		MessageBox(NULL, "d3d vertex buffer create failed", "water", MB_OK);
		return	false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0, 0, (void**)&m_pIdxList, 0 ) ) )
#else
	if ( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&m_pIdxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
//		MessageBox(NULL, "d3d index buffer lock failed", "water", MB_OK);
		return	false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0, 0, (void**)&m_pVtxList, 0 ) ) )
#else
	if ( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&m_pVtxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
//		MessageBox(NULL, "d3d vertex buffer lock failed", "water", MB_OK);
		return	false;
	}

#define		wv_width	12.0f
	m_pVtxList[0].x	= -wv_width;
	m_pVtxList[0].y	= 0.0f;
	m_pVtxList[0].z	= -wv_width;
	m_pVtxList[0].tu	= 0.0f;
	m_pVtxList[0].tv	= 1.0f;

	m_pVtxList[1].x	= -wv_width;
	m_pVtxList[1].y	= 0.0f;
	m_pVtxList[1].z	= wv_width;
	m_pVtxList[1].tu	= 0.0f;
	m_pVtxList[1].tv	= 0.0f;


	m_pVtxList[2].x	= wv_width;
	m_pVtxList[2].y	= 0.0f;
	m_pVtxList[2].z	= wv_width;
	m_pVtxList[2].tu	= 1.0f;
	m_pVtxList[2].tv	= 0.0f;


	m_pVtxList[3].x	= wv_width;
	m_pVtxList[3].y	= 0.0f;
	m_pVtxList[3].z	= -wv_width;
	m_pVtxList[3].tu	= 1.0f;
	m_pVtxList[3].tv	= 1.0f;

	m_pIdxList[0]	= 0;
	m_pIdxList[1]	= 1;
	m_pIdxList[2]	= 2;
	m_pIdxList[3]	= 2;
	m_pIdxList[4]	= 3;
	m_pIdxList[5]	= 0;
	
	m_pIB->Unlock();
	m_pVB->Unlock();

	m_matTemp	= m_matPos;
	m_matTemp._41	= x + 8.0f;
	m_matTemp._42	= y + (rand()%10 * 0.1f);
	m_matTemp._43	= z + 8.0f;
	SetupPosTM(x, y, z);

	m_lPreCount		= 0;
	m_nFps			= rand()%50 + 50;
	m_fPosStep		= ((rand()%10)+1) * 0.01f + 0.1f;

	m_Material.Diffuse.r	= 1.0f;
	m_Material.Diffuse.g	= 1.0f;
	m_Material.Diffuse.b	= 1.0f;
	m_Material.Diffuse.a	= 1.0f;
	m_Material.Ambient.r	= 0.9f;
	m_Material.Ambient.g	= 0.9f;
	m_Material.Ambient.b	= 0.9f;
	m_Material.Ambient.a	= 0.9f;
	m_Material.Specular.r	= 0.9f;
	m_Material.Specular.g	= 0.9f;
	m_Material.Specular.b	= 0.9f;
	m_Material.Specular.a	= 0.9f;
	m_Material.Power	= 1.0f;


	return	true;
}

void	CPlaneWaterEffect::Display() 
{
	g_RenderManager.SetMaterial(&m_Material);
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(VTXWATER));
	g_RenderManager.SetFVF(D3DFVF_WATER);
	g_RenderManager.SetIndices(m_pIB, 0);

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0x08 );
	g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCCOLOR );

	g_RenderManager.SetTexture(0, g_Map.m_pTextureFlow);

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );

	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
	
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetAlphaFunc( D3DCMP_ALWAYS );
	g_RenderManager.SetAlphaBlendEnable(  FALSE );
}

void	CPlaneWaterEffect::SetupPosTM(float x, float y, float z)
{
	m_matPos._41	= x + 8.0f;
	m_matPos._42	= y;
	m_matPos._43	= z + 8.0f;
}

void	CPlaneWaterEffect::SetupRotTM(float angle)
{
	D3DXMatrixRotationY(&m_matPos, angle*0.01745);
}

void	CPlaneWaterEffect::PlayAni()
{
	const short		dir = 	(short)m_fAngle/45.0f;

	LONG	lCurCount	= timeGetTime();
	float	maxf		= m_nFps*60;

	if ( lCurCount - m_lPreCount < maxf/2 )
	{
		m_Material.Diffuse.a	= (lCurCount-m_lPreCount)/(maxf/2);
		if ( m_Material.Diffuse.a > 1.0f)
			m_Material.Diffuse.a	= 1.0f;

	}
	else if ( lCurCount - m_lPreCount < maxf )
	{
		m_Material.Diffuse.a	= 1.0f - (lCurCount-m_lPreCount)/maxf;
		if ( m_Material.Diffuse.a < 0.0f )
			m_Material.Diffuse.a	= 0.0f;
	}
	else
	{
		m_lPreCount	= timeGetTime();
		m_matPos	= m_matTemp;
	}

	switch ( dir )
	{
	case	0:
		m_matPos._43	+= m_fPosStep;
		break;
	case	1:
		m_matPos._41	+= m_fPosStep;
		m_matPos._43	+= m_fPosStep;
		break;
	case	2:
		m_matPos._41	+= m_fPosStep;
		break;
	case	3:
		m_matPos._41	+= m_fPosStep;
		m_matPos._43	-= m_fPosStep;
		break;
	case	4:
		m_matPos._43	-= m_fPosStep;
		break;
	case	5:
		m_matPos._41	-= m_fPosStep;
		m_matPos._43	-= m_fPosStep;
		break;
	case	6:
		m_matPos._41	-= m_fPosStep;
		break;
	case	7:
		m_matPos._41	-= m_fPosStep;
		m_matPos._43	+= m_fPosStep;
		break;
	}
}