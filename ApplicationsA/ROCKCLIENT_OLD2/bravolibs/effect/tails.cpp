#include "RockPCH.h"
#include "VFileSystem.h"

#include	"..\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	"..\pc.h"
#include	"..\\Effect.h"
#include	"..\brother.h"
#include	<OBJ\OBJMath.h>
#include	"Tails.h"
#include	"..\\..\\CRenderManager.h"


inline float Eval1(float v[4],float u)
{
	return ((((v[3]*u) + v[2])*u + v[1])*u + v[0]);
}

void	CTails::DisPlay()
{
	
	int calc_cnt = m_count;	// 애니메이션되는 점수의 숫자

	float x_abcd[2][MAX_TRACE_COUNT][4];
	float y_abcd[2][MAX_TRACE_COUNT][4];
	float z_abcd[2][MAX_TRACE_COUNT][4];

	int step = m_Step;//각각의 사이는 3개씩.

	CalcNaturalCubic( x_abcd[0], calc_cnt-1, &m_fStart[0][0] );
	CalcNaturalCubic( x_abcd[1], calc_cnt-1, &m_fEnd[0][0]	 );
	CalcNaturalCubic( y_abcd[0], calc_cnt-1, &m_fStart[0][1] );
	CalcNaturalCubic( y_abcd[1], calc_cnt-1, &m_fEnd[0][1]	 );
	CalcNaturalCubic( z_abcd[0], calc_cnt-1, &m_fStart[0][2] );
	CalcNaturalCubic( z_abcd[1], calc_cnt-1, &m_fEnd[0][2]	 );
	m_TotalTraceCnt = (calc_cnt-1)*step+1;
	
	if( m_TotalTraceCnt >= MAX_TRACE_COUNT )
	{
		m_TotalTraceCnt = 0;
		return;	//버퍼 넘친다.. 
	}

	DWORD temp_cnt = m_TotalTraceCnt-1;
	m_fAllStart[temp_cnt][0]	= Eval1(x_abcd[0][0],0);
	m_fAllEnd[temp_cnt][0]		= Eval1(x_abcd[1][0],0);
	m_fAllStart[temp_cnt][1]	= Eval1(y_abcd[0][0],0);
	m_fAllEnd[temp_cnt][1]		= Eval1(y_abcd[1][0],0);
	m_fAllStart[temp_cnt][2]	= Eval1(z_abcd[0][0],0);
	m_fAllEnd[temp_cnt][2]		= Eval1(z_abcd[1][0],0);
	temp_cnt--;

	for(int j = 0; j < calc_cnt-1; j++)
	{
		for(int i = 1; i <= step; i++)	//각각의 사이는 스탭값으로.
		{
			float u = (i) / (float)step;
			m_fAllStart[temp_cnt][0]	= Eval1(x_abcd[0][j],u);
			m_fAllEnd[temp_cnt][0]		= Eval1(x_abcd[1][j],u);
			m_fAllStart[temp_cnt][1]	= Eval1(y_abcd[0][j],u);
			m_fAllEnd[temp_cnt][1]		= Eval1(y_abcd[1][j],u);
			m_fAllStart[temp_cnt][2]	= Eval1(z_abcd[0][j],u);
			m_fAllEnd[temp_cnt][2]		= Eval1(z_abcd[1][j],u);
			temp_cnt--;
		}
	}

	for( int i = 0; i < m_TotalTraceCnt+1; i++)
	{
#ifdef BATCH_RENDER
		float f_u = (i/(float)m_TotalTraceCnt);
		m_TraceVBuf[i*2].v.x = m_fAllStart[i][0];
		m_TraceVBuf[i*2].v.y = m_fAllStart[i][1];
		m_TraceVBuf[i*2].v.z = m_fAllStart[i][2];
		m_TraceVBuf[i*2].tu = f_u;
		m_TraceVBuf[i*2].tv = 0;
		m_TraceVBuf[i*2].color=0xffffffff;
		
		m_TraceVBuf[i*2+1].v.x = m_fAllEnd[i][0];
		m_TraceVBuf[i*2+1].v.y = m_fAllEnd[i][1];
		m_TraceVBuf[i*2+1].v.z = m_fAllEnd[i][2];
		m_TraceVBuf[i*2+1].tu = f_u;
		m_TraceVBuf[i*2+1].tv = 1;
		m_TraceVBuf[i*2+1].color=0xffffffff;
#else	//	BATCH_RENDER
		float f_u = (i/(float)m_TotalTraceCnt);
		m_TraceVBuf[i*2].x = m_fAllStart[i][0];
		m_TraceVBuf[i*2].y = m_fAllStart[i][1];
		m_TraceVBuf[i*2].z = m_fAllStart[i][2];
		m_TraceVBuf[i*2].u = f_u;
		m_TraceVBuf[i*2].v = 0;
		m_TraceVBuf[i*2].color=0xffffffff;

		m_TraceVBuf[i*2+1].x = m_fAllEnd[i][0];
		m_TraceVBuf[i*2+1].y = m_fAllEnd[i][1];
		m_TraceVBuf[i*2+1].z = m_fAllEnd[i][2];
		m_TraceVBuf[i*2+1].u = f_u;
		m_TraceVBuf[i*2+1].v = 1;
		m_TraceVBuf[i*2+1].color=0xffffffff;
#endif	//	BATCH_RENDER
	}

		

	if( m_TotalTraceCnt )
		g_RenderManager.DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 
										(m_TotalTraceCnt)*2-2, m_TraceVBuf, sizeof(TAILS_VERTEX));
}

void	CTails::AddPoint( D3DXVECTOR3& v, float width )
{
	vTop.z = width/2;
	vBottom.z = (width/2)*-1.0f;
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	mat._41 = v.x;
	mat._42 = v.y;
	mat._43 = v.z;
	Float3MatrixMultiply(	m_fStart[ m_count ][0],
							m_fStart[ m_count ][1],
							m_fStart[ m_count ][2],
							vTop, mat );
	Float3MatrixMultiply(	m_fEnd[m_count][0],
							m_fEnd[m_count][1],
							m_fEnd[m_count][2],
							vBottom, mat );
	m_count++;
}

void	CTails::AddPoint2( D3DXVECTOR3& v, float width )
{
//	vTop.z = width/2;
//	vBottom.z = (width/2)*-1.0f;
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	mat._41 = v.x;
	mat._42	= g_Pc.GetMapHeight( v.x/8, v.z/8 ) + 0.5f;
	mat._43 = v.z;
	Float3MatrixMultiply(	m_fStart[ m_count ][0],
							m_fStart[ m_count ][1],
							m_fStart[ m_count ][2],
							vTop, mat );
	Float3MatrixMultiply(	m_fEnd[m_count][0],
							m_fEnd[m_count][1],
							m_fEnd[m_count][2],
							vBottom, mat );
	m_count++;
}


CTailsManager::CTailsManager()
{
	for( int i = 0; i < MAX_TAILS; i++ )
	{
		m_Tails[ i ].Clear();
	}

	tex = false;

	texnum = 0;
}

CTailsManager::~CTailsManager()
{
}

void	CTailsManager::LoadTex( char* dir )
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
//	char path[256] = {0,};
	char temp[256] = {0,};
	strcpy( temp, dir);
	strcat( temp, "\\effect\\tex\\tail-02.tga");
	char temp1[512] = {0,};
	sprintf( temp1, "!텍스쳐 읽기 실패:%s", temp );

	if( FAILED(g_RenderManager.CreateTextureFromFileEx( temp, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
														D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
														0, NULL, NULL, &m_TexTail[0] )))
	{
///		MessageBox( NULL, temp1, "오류", NULL );
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFEffectTex.OpenFile( "tail-02.tga" );	
	if( pFH )
	{
		HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_TexTail[0] );
		
		g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}		
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

int		CTailsManager::FindNextTail()
{
	for( int i = 0; i < MAX_TAILS; i++ )
	{
		if( m_Tails[ i ].m_count == -1 )
			return i;
	}

	return -1;
}

void	CTailsManager::Render()
{
	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
	g_RenderManager.SetFVF( D3DFVF_TAILS_VERTEX );

	g_RenderManager.SetTexture( 0, m_TexTail[texnum] );

	for( int i = 0; i < MAX_TAILS; i++ )
	{
		if( m_Tails[ i ].m_count > 0 )
			m_Tails[ i ].DisPlay();
	}
}
