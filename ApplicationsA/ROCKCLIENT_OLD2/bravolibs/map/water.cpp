#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	<base\\d3dapp.h>
#include	<mmsystem.h>
#include	"water.h"
#include	"Field.h"
#include	"..\\Map.h"
#include	"SuperviseEnv.h"

#include "..\\..\\CRenderManager.h"

#define		WSIZE		16.0f
#define		WBHEIGHT	20.0f

int	n	= 0;
int	s	= 1;


CPlaneWater::CPlaneWater()
{
	D3DXMatrixIdentity(&m_matPos);
	D3DXMatrixIdentity(&m_matTexPos);
	m_lPreCount	= 0;
	m_bIsVisible	= false;
	m_pIB	= NULL;
	m_pVB	= NULL;
}

CPlaneWater::~CPlaneWater()
{
//	FinalCleanup();
}

void	CPlaneWater::FinalCleanup()
{
	_RELEASE(m_pIB);
	_RELEASE(m_pVB);
	D3DXMatrixIdentity(&m_matPos);
	D3DXMatrixIdentity(&m_matTexPos);
	m_lPreCount		= 0;
	m_dwPos			= 0;
	m_nDir			= 0;


	m_texIdx		= 0;
	m_listIdx		= 0;
	m_pDevice		= NULL;
	m_bIsVisible	= false;
}

#ifdef DIRECT_VERSION_9_MJH
bool	CPlaneWater::InitDevice(LPDIRECT3DDEVICE9 pdev)
#else
bool	CPlaneWater::InitDevice(LPDIRECT3DDEVICE8 pdev)
#endif // DIRECT_VERSION_9_MJH
{
	if ( !pdev )
		return	false;

	m_pDevice	= pdev;
	return	true;
}

bool	CPlaneWater::Create(const D3DXVECTOR3& v0, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3)
{
	if ( !m_pDevice )
		return	false;

	m_vPos[0]	= D3DXVECTOR3(v0.x, v0.y, v0.z);
	m_vPos[1]	= D3DXVECTOR3(v1.x, v1.y, v1.z);
	m_vPos[2]	= D3DXVECTOR3(v2.x, v2.y, v2.z);
	m_vPos[3]	= D3DXVECTOR3(v3.x, v3.y, v3.z);

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

	m_pVtxList[0].x	=	v0.x;
	m_pVtxList[0].y	=	v0.y;
	m_pVtxList[0].z	=	v0.z;
	m_pVtxList[0].tu	= 0.0f;
	m_pVtxList[0].tv	= 1.0f;

	m_pVtxList[1].x	=	v1.x;
	m_pVtxList[1].y	=	v1.y;
	m_pVtxList[1].z	=	v1.z;
	m_pVtxList[1].tu	= 0.0f;
	m_pVtxList[1].tv	= 0.0f;


	m_pVtxList[2].x	=	v2.x;
	m_pVtxList[2].y	=	v2.y;
	m_pVtxList[2].z	=	v2.z;
	m_pVtxList[2].tu	= 1.0f;
	m_pVtxList[2].tv	= 0.0f;


	m_pVtxList[3].x	=	v3.x;
	m_pVtxList[3].y	=	v3.y;
	m_pVtxList[3].z	=	v3.z;
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

	return	true;
}

bool		CPlaneWater::Update(const D3DXVECTOR3& v0, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3)
{
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

	m_pVtxList[0].x	=	v0.x;
	m_pVtxList[0].y	=	v0.y;
	m_pVtxList[0].z	=	v0.z;
	m_pVtxList[0].tu	= 0.0f;
	m_pVtxList[0].tv	= 1.0f;

	m_pVtxList[1].x	=	v1.x;
	m_pVtxList[1].y	=	v1.y;
	m_pVtxList[1].z	=	v1.z;
	m_pVtxList[1].tu	= 0.0f;
	m_pVtxList[1].tv	= 0.0f;


	m_pVtxList[2].x	=	v2.x;
	m_pVtxList[2].y	=	v2.y;
	m_pVtxList[2].z	=	v2.z;
	m_pVtxList[2].tu	= 1.0f;
	m_pVtxList[2].tv	= 0.0f;


	m_pVtxList[3].x	=	v3.x;
	m_pVtxList[3].y	=	v3.y;
	m_pVtxList[3].z	=	v3.z;
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
	return	true;
}

HRESULT		CPlaneWater::Display() 
{
	PlayAni();

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(VTXWATER));
	g_RenderManager.SetFVF(D3DFVF_WATER);
	g_RenderManager.SetIndices(m_pIB, 0);

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCCOLOR );

	g_RenderManager.SetTexture(0, g_Map.m_pTextureWater);
	g_RenderManager.SetTransform( D3DTS_TEXTURE0, &m_matTexPos );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT2 );

	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

	return	S_OK;
}

void		CPlaneWater::SetupPosTM(float x, float y, float z)
{
	m_matPos._41	= x;
	m_matPos._42	= y;
	m_matPos._43	= z;
}


void		CPlaneWater::PlayAni()
{
#define		water_step		0.05f;
#define		water_step2		0.025f;
	LONG	lCurCount	= timeGetTime();

	if ( lCurCount > m_lPreCount + 40 )
	{
		m_lPreCount	= lCurCount;

		switch ( m_nDir )
		{
			case	DIRW_NONE:
				m_matTexPos._31	+= (float)(0.005);
				m_matTexPos._32	+= (float)(0.005);
				if ( m_matTexPos._31 == 1.0f )
					m_matTexPos._31	= 0.0f;
				if ( m_matTexPos._32 == 1.0f )
					m_matTexPos._32	= 0.0f;
				break;
			case	DIRW_NORTH:
				m_matTexPos._32	+=	water_step;
				if ( m_matTexPos._32 >= 1.0f )
					m_matTexPos._32	= 0.0f;
				break;
			case	DIRW_NORTHEAST:
				m_matTexPos._32	+=	water_step2;
				if ( m_matTexPos._32 >= 1.0f )
					m_matTexPos._32	= 0.0f;
				m_matTexPos._31	-=	water_step2;
				if ( m_matTexPos._31 <= 0.0f )
					m_matTexPos._31	= 1.0f;
				break;
			case	DIRW_EAST:
				m_matTexPos._31	-=	water_step;
				if ( m_matTexPos._31 <= 0.0f )
					m_matTexPos._31	= 1.0f;
				break;
			case	DIRW_SOUTHEAST:
				m_matTexPos._31	-=	water_step2;
				if ( m_matTexPos._31 <= 0.0f )
					m_matTexPos._31	= 1.0f;
				m_matTexPos._32	-=	water_step2;
				if ( m_matTexPos._32 <= 0.0f )
					m_matTexPos._32	= 1.0f;
				break;
			case	DIRW_SOUTH:
				m_matTexPos._32	-=	water_step;
				if ( m_matTexPos._32 <= 0.0f )
					m_matTexPos._32	= 1.0f;
				break;
			case	DIRW_SOUTHWEST:
				m_matTexPos._31	+=	water_step2;
				if ( m_matTexPos._31 >= 1.0f )
					m_matTexPos._31	= 0.0f;
				m_matTexPos._32	-=	water_step2;
				if ( m_matTexPos._32 <= 0.0f )
					m_matTexPos._32	= 1.0f;
				break;
			case	DIRW_WEST:
				m_matTexPos._31	+=	water_step;
				if ( m_matTexPos._31 >= 1.0f )
					m_matTexPos._31	= 0.0f;
				break;
			case	DIRW_NORTHWEST:
				m_matTexPos._31	+=	water_step2;
				if ( m_matTexPos._31 >= 1.0f )
					m_matTexPos._31	= 0.0f;
				m_matTexPos._32	+=	water_step2;
				if ( m_matTexPos._32 >= 1.0f )
					m_matTexPos._32	= 0.0f;
				break;
		}
	}
}