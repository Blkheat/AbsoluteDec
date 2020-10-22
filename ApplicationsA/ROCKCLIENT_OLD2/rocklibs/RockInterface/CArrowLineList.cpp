#include "..\\..\\RockClient.h"
#include "Rui.h"
#include "CArrowLineList.h"
#include "..\\..\\CRenderManager.h"

#define ARROWADDLENGTH 2
#define ARROWLINEWIDTH 2

CArrowLineList::CArrowLineList( SDesktop* DT )
{
	m_nPointSize = 0;
	m_nPolyCount = 0;
	m_nPointIndex = -1;
}

CArrowLineList::~CArrowLineList()
{
	m_nPointSize = 0;
	m_nPolyCount = 0;
	m_nPointIndex = -1;
}

void  CArrowLineList::Initialize()
{
	m_nPointSize = 0;
	m_nPolyCount = 0;
	m_nPointIndex = -1;
}

void  CArrowLineList::AddPoint(POINT nPoint)
{
	m_nPointIndex++;
	m_nPointSize++;
	m_PointList[m_nPointIndex] = nPoint;
	ConVertPointTo3DVetex();
}

void  CArrowLineList::ConVertPointTo3DVetex()
{
	D3DXMATRIX matProj;
		
	D3DXMatrixOrthoLH( &matProj , 1024.0f , 768.0f , 0.0f , 10000.0f );
	D3DXMATRIX mtrOffset; 
	D3DXMatrixTranslation( &mtrOffset, -0.5, -0.5, 0.0 ); 
	matProj = mtrOffset * matProj;		

	m_VertexList[m_nPointIndex].x = 
		(((2.0f*m_PointList[m_nPointIndex].x)/g_RockClient.GetDisplayWidth())-1) / matProj._11;
	m_VertexList[m_nPointIndex].y = 
		-(((2.0f*m_PointList[m_nPointIndex].y)/g_RockClient.GetDisplayHeight())-1) / matProj._22;
	m_VertexList[m_nPointIndex].z = 0.0f;
}

void  CArrowLineList::MakePolygonList()
{
	if( m_nPointSize < 2 )
	{
		return;
	}	
	
	int i = 0;
	m_nPolyIndex = 0;
	m_nPolyCount = 0;

	D3DXVECTOR3 vStart;
	D3DXVECTOR3 vEnd;
	D3DXVECTOR3 vRight;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vAdd1;
	D3DXVECTOR3 vAdd2;

	for( i = 1 ; i < m_nPointSize ; i++ )
	{
		vStart = m_VertexList[i-1];
		vEnd = m_VertexList[i];

		vDir = ( vEnd - vStart );
		D3DXVec3Normalize( &vDir , &vDir );
		D3DXVec3Cross( &vRight , &vDir, &D3DXVECTOR3(0.0f, 0.0f, 1.0f) );
		D3DXVec3Normalize( &vRight , &vRight );

		vAdd1 = ARROWADDLENGTH * vDir;
		vAdd2 = ARROWLINEWIDTH * vRight;
		
		m_LinePolygon[m_nPolyIndex++].vPos = vStart - vAdd1 - vAdd2;
		m_LinePolygon[m_nPolyIndex++].vPos = vEnd + vAdd1 - vAdd2;
		m_LinePolygon[m_nPolyIndex++].vPos = vStart - vAdd1 + vAdd2;

		m_nPolyCount++;		
		
		m_LinePolygon[m_nPolyIndex++].vPos = vStart - vAdd1 + vAdd2;
		m_LinePolygon[m_nPolyIndex++].vPos = vEnd + vAdd1 - vAdd2;
		m_LinePolygon[m_nPolyIndex++].vPos = vEnd + vAdd1 + vAdd2;

		m_nPolyCount++;		
	}

	if(m_nPolyCount)
	{
		m_ArrowPolygon[0].vPos = vEnd + vAdd1 - (2.0f * vAdd2);
		m_ArrowPolygon[1].vPos = vEnd + (10.0f * vAdd1);
		m_ArrowPolygon[2].vPos = vEnd + vAdd1 + (2.0f * vAdd2);
	}
}

void CArrowLineList::RenderProc()
{
	D3DXVECTOR3 vEye    = g_Camera.m_vEye;
	D3DXVECTOR3 vLookAt = g_Camera.m_vLookat;
	D3DXVECTOR3 vUp     = g_Camera.m_vUp;
		
	D3DXMATRIX  matWorld, matView;	
		
	D3DXVECTOR3 vTempEye( 0.0f , 0.0f , -1.0f );
	D3DXVECTOR3 vTempLookAt( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vTempUp( 0.0f , 1.0f , 0.0f );
	D3DXMATRIX matProj;

	D3DXMatrixIdentity( &matWorld );

	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );	
		
	D3DXMatrixLookAtLH( &matView , &vTempEye, &vTempLookAt, &vTempUp );
	g_RenderManager.SetTransform(D3DTS_VIEW , &matView);
		
	D3DXMatrixOrthoLH( &matProj , 1024.0f , 768.0f , 0.0f , 10000.0f );
	D3DXMATRIX mtrOffset; 
	D3DXMatrixTranslation( &mtrOffset, -0.5, -0.5, 0.0 ); 
	matProj = mtrOffset * matProj;		
	g_RenderManager.SetTransform(D3DTS_PROJECTION, &matProj );
	///-- 이곳에서 화살표 라인을 그린다.

	DWORD Col0Op, Col0ARG1, Col0ARG2;

	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP,   &Col0Op );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1, &Col0ARG1 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2, &Col0ARG2 );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetFVF(D3DFVF_ARROWVERTEX);
	g_RenderManager.SetTexture( 0, NULL );

	if( m_nPolyCount )
	{
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLELIST , m_nPolyCount, 
										 m_LinePolygon, sizeof( ARROWVERTEX ) );
	}

	if( m_nPolyCount )
	{
		g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLELIST , 1, 
										 m_ArrowPolygon, sizeof( ARROWVERTEX ) );
	}

	g_RenderManager.SetTextureColorOP  ( 0 , Col0Op );
	g_RenderManager.SetTextureColorArg1( 0 , Col0ARG1 );
	g_RenderManager.SetTextureColorArg2( 0 , Col0ARG2 );
			
	g_Camera.SetCamera( vEye , vLookAt , vUp );
	g_RenderManager.SetTransform(D3DTS_PROJECTION, &g_Camera.m_matProj );

	g_RenderManager.SetFVF(RUI_VERTEX_FORMAT);
}