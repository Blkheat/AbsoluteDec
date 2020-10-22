#include "RockPCH.h"
#include "VFileSystem.h"

#include	<NETWORK\\Protocol.h>
#include	"BASE\\D3DBASE.H"
#include	"EffectVertex.h"
#include	"EffectInsect.h"
#include	"EffectSystem.h"

#include	"..\\quadlist.h"
#include	"..\\RockClient.H"
#include	"..\\Camera.h"
#include	"..\\PC.h"
#include	"..\\MAP.h"
#include	"..\\..\\CRenderManager.h"

extern	long	g_nowTime;
extern  BOOL g_ISLandEffect;

CInsect::~CInsect()
{
}

CBird::~CBird()
{
}

CInsect_Manager::~CInsect_Manager()
{
}



/***************************************************************
0 : 반디
1 : 나비
2 : 새 
3 : 앉은 새 
4 : 박쥐 
***************************************************************/



void	CInsect_Manager::Create_Insect(char* dir, int type)
{
	if( !g_ISLandEffect )
		return;

	//우선 시간 체크 
	m_fPrvTime = (float)timeGetTime();

	Insect_Setting(dir, type);

	//캐릭터 현재 위치에서 파티클들이 나옴 
	m_vInsect_Str_Pos = D3DXVECTOR3( 220 * 8 + 4, 0, 150 * 8 + 4 );
	int MapSize = g_Pc.GetMapSize();


	switch( type )
	{
	case 0:
		{
			for( DWORD	i = 0; i < MAX_INSECT; i++ )
			{
				Relife_Insect( i );
				Change_Insect( i );
			}
			break;
		}
	case 1:
		{
			for( DWORD	i = 0; i < MAX_INSECT; i++ )
			{
				//랜덤으로 방향 생성
				float RandomYaw		 = RANDOM_NUM * 6.28318f;
				float RandomPitch	 = RANDOM_NUM * 6.28318f ;
				m_Insect[i].m_vInsect_Vel.x	= sinf(RandomPitch) * cosf(RandomYaw) * 0.5f;
				m_Insect[i].m_vInsect_Vel.y	= 0.0f;
				m_Insect[i].m_vInsect_Vel.z	= sinf(RandomPitch) * sinf(RandomYaw) * 0.5f;
				m_Insect[i].m_fInsect_Dir		= Dir_Insect(m_Insect[i].m_vInsect_Vel);
				m_Insect[i].m_fInsect_Life	= (RANDOM_NUM + 2);

				int x = (int)timeGetTime()%4;
				int y , z;

				switch( x ){
				case 0:	{y = 1;	z = 1;	break;}
				case 1:	{y = 1;	z = -1;	break;}
				case 2:	{y = -1;z = 1;	break;}
				case 3:	{y = -1;z = -1;	break;}
				}

				m_Insect[i].m_vInsect_Loc.x = ((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) * y +0 * 8 + 4;
				m_Insect[i].m_vInsect_Loc.z = ((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) * z + 0 * 8 + 4;
				int xloc = (int)(m_Insect[i].m_vInsect_Loc.x/8);
				int zloc = (int)(m_Insect[i].m_vInsect_Loc.z/8);
				if( MapSize > xloc || 0 < xloc || MapSize > zloc || zloc < 0 )
				{
					m_Insect[i].m_vInsect_Loc.y =  g_Map.Get__MapHeight( xloc, zloc );
				}
				else
				{
					Create_Insect(dir, type);
					return;
				}
			}
			break;
		}
	case 2:
		{
			//3~5 까지 랜덤의 숫자로 생성 
			//새 숫자를 새로 만들때 랜덤으로 조정
			m_iBird_Num = MAX_BIRD - 2 + (int)(RANDOM_NUM * 2);

			//랜덤으로 방향 생성에 필요한 인자들
			float RandomYaw		 = RANDOM_NUM * 6.28318f;
			float RandomPitch	 = RANDOM_NUM * 6.28318f ;

			float velx = LimitVel(sinf(RandomPitch) * cosf(RandomYaw));
			float velz = LimitVel(sinf(RandomPitch) * sinf(RandomYaw));

			//사면에서랜덤으로 생성 방향은 화면 안쪽으로
			int x = (int)timeGetTime()%4;
			switch( x ){
			case 0:	{
				m_vBird_Pos.x = ((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) + 0 * 8 + 4;
				m_vBird_Pos.z = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + 0 * 8 + 4;
				m_vBird_Vel.x	= -MinusToPlus(velx);
				m_vBird_Vel.z	= velz;
				break;}
			case 1:	{
				m_vBird_Pos.x = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + 0 * 8 + 4;
				m_vBird_Pos.z = ((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) + 0 * 8 + 4;
				m_vBird_Vel.x	= velx;
				m_vBird_Vel.z	= -MinusToPlus(velz);
				break;}
			case 2:	{
				m_vBird_Pos.x = (((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) * -1 ) + 0 * 8 + 4;
				m_vBird_Pos.z = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + 0 * 8 + 4;
				m_vBird_Vel.x	= MinusToPlus(velx);
				m_vBird_Vel.z	= velz;
				break;}
			case 3:	{
				m_vBird_Pos.x = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + 0 * 8 + 4;
				m_vBird_Pos.z = (((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) * -1 ) + 0 * 8 + 4;
				m_vBird_Vel.x	= velx;
				m_vBird_Vel.z	= MinusToPlus(velz);
				break;}
			}
		//	if (!g_Camera.PointInFrustum(m_vBird_Pos.x,m_vBird_Pos.y,m_vBird_Pos.z)) {Change_Bird();return;}

			int xloc = (int)(m_vBird_Pos.x/8);
			int zloc = (int)(m_vBird_Pos.z/8);
			if( MapSize > xloc || 0 < xloc || MapSize > zloc || zloc < 0 )
			{
				m_vBird_Pos.y =  g_Map.Get__MapHeight( xloc, zloc );
			}
			else
			{
				Create_Insect(dir, type);
				return;
			}
			
			m_fBird_Fast	= MinusToPlus(sinf(RandomPitch));
			m_fBird_Life	= 0.0f;

			//휘어짐의 정도 인자 
			x = (int)timeGetTime()%2;
			switch( x ){
			case 0:	{m_vBird_Facter.x	= m_fBird_Fast/100;		break;}
			case 1:	{m_vBird_Facter.x	= -m_fBird_Fast/100;	break;}
			}

			m_vBird_Facter.z = ((m_vBird_Facter.x * m_vBird_Vel.x) * -1.0f) / m_vBird_Vel.z;

			//중점으로 부터 거리 조정 계산 그리고 그 거리 저장 
			for( DWORD	i = 0; i < m_iBird_Num; i++ )
			{
				m_Bird[i].m_fBird_Opp[0] = RANDOM_NUM * 2;
				m_Bird[i].m_fBird_Opp[1] = RANDOM_NUM * 2;
				m_Bird[i].m_vBird_Loc.x = m_vBird_Pos.x + m_Bird[i].m_fBird_Opp[0];
				m_Bird[i].m_vBird_Loc.z = m_vBird_Pos.z + m_Bird[i].m_fBird_Opp[1];

				//날개짓 함
				m_Bird[i].m_bStop_Bird	= false;
			}
			break;
		}
	case 3:
		{

		}
		break;
	}
}



HRESULT	CInsect_Manager::Insect_Setting(char* directory, int insecttype)
{
	///////////////////////////////////////////////////////////반딧불만들기 
	if( FAILED(g_RenderManager.CreateIndexBuffer( 6*sizeof(WORD),
												 D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
												 D3DPOOL_MANAGED, &m_pGlimmerIB) ) )
	{
		return	E_FAIL;
	}

	WORD	*pGlimmerIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pGlimmerIB->Lock(0, 6*sizeof(WORD), (void**)&pGlimmerIndices, 0) ) )
#else
	if ( FAILED ( m_pGlimmerIB->Lock(0, 6*sizeof(WORD), (BYTE**)&pGlimmerIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	pGlimmerIndices[0] = 0;
	pGlimmerIndices[1] = 1;
	pGlimmerIndices[2] = 3;
	pGlimmerIndices[3] = 3;
	pGlimmerIndices[4] = 1;
	pGlimmerIndices[5] = 2;

	m_pGlimmerIB->Unlock();


	//스페이스 버텍스 버퍼를 쓸때 
	if( FAILED(g_RenderManager.CreateVertexBuffer( 4 * sizeof(INSECTVERTEX),
												  D3DUSAGE_WRITEONLY, D3DFVF_INSECTVERTEX,
												  D3DPOOL_MANAGED, &m_pGlimmerVB ) ) )
	{
		return E_FAIL;
	}

	INSECTVERTEX* pGlimmerVertices;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pGlimmerVB->Lock( 0, 0, (void**)&pGlimmerVertices, 0 ) ) )
#else
	if( FAILED( m_pGlimmerVB->Lock( 0, 0, (BYTE**)&pGlimmerVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;

	pGlimmerVertices[0].v	=	D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	pGlimmerVertices[1].v	=	D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	pGlimmerVertices[2].v	=	D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	pGlimmerVertices[3].v	=	D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	pGlimmerVertices[0].tu	=	0.0f;
	pGlimmerVertices[0].tv	=	1.0f;
	pGlimmerVertices[1].tu	=	0.0f;
	pGlimmerVertices[1].tv	=	0.0f;
	pGlimmerVertices[2].tu	=	1.0f;
	pGlimmerVertices[2].tv	=	0.0f;
	pGlimmerVertices[3].tu	=	1.0f;
	pGlimmerVertices[3].tv	=	1.0f;

	m_pGlimmerVB->Unlock();


	//////////////////////////////////////////////////////////////////여기서부터는 나비 만들기 

	if( FAILED(g_RenderManager.CreateIndexBuffer( 6*sizeof(WORD),
												 D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
												 D3DPOOL_MANAGED, &m_pButterFlyIB[0]) ) )
	{
		return	E_FAIL;
	}

	WORD	*pButterFlyIdx;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pButterFlyIB[0]->Lock(0, 6*sizeof(WORD), (void**)&pButterFlyIdx, 0) ) )
#else
	if ( FAILED ( m_pButterFlyIB[0]->Lock(0, 6*sizeof(WORD), (BYTE**)&pButterFlyIdx, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	pButterFlyIdx[0] = 1;
	pButterFlyIdx[1] = 2;
	pButterFlyIdx[2] = 3;
	pButterFlyIdx[3] = 3;
	pButterFlyIdx[4] = 0;
	pButterFlyIdx[5] = 1;

	m_pButterFlyIB[0]->Unlock();


	if( FAILED(g_RenderManager.CreateVertexBuffer( 4 * sizeof(BIRDVERTEX),
												  D3DUSAGE_WRITEONLY, D3DFVF_BIRDVERTEX,
												  D3DPOOL_MANAGED, &m_pButterFlyVB[0] ) ) )
	{
		return E_FAIL;
	}

	BIRDVERTEX* pButterFlyVtx;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pButterFlyVB[0]->Lock( 0, 0, (void**)&pButterFlyVtx, 0 ) ) )
#else
	if( FAILED( m_pButterFlyVB[0]->Lock( 0, 0, (BYTE**)&pButterFlyVtx, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;

	pButterFlyVtx[0].v	=	D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pButterFlyVtx[1].v	=	D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pButterFlyVtx[2].v	=	D3DXVECTOR3(1.0f, 0.0f, -1.0f);
	pButterFlyVtx[3].v	=	D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	
	pButterFlyVtx[0].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pButterFlyVtx[1].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pButterFlyVtx[2].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pButterFlyVtx[3].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pButterFlyVtx[0].tu	=	0.5f;
	pButterFlyVtx[0].tv	=	1.0f;
	pButterFlyVtx[1].tu	=	0.5f;
	pButterFlyVtx[1].tv	=	0.0f;
	pButterFlyVtx[2].tu	=	1.0f;
	pButterFlyVtx[2].tv	=	0.0f;
	pButterFlyVtx[3].tu	=	1.0f;
	pButterFlyVtx[3].tv	=	1.0f;

	m_pButterFlyVB[0]->Unlock();

	/////////////////////////////////////////////////////////////나머지 반쪽 날개 
	if( FAILED(g_RenderManager.CreateIndexBuffer( 6*sizeof(WORD),
												 D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
												 D3DPOOL_MANAGED, &m_pButterFlyIB[1]) ) )
	{
		return	E_FAIL;
	}

	WORD	*pButterFlyIdxa;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pButterFlyIB[1]->Lock(0, 6*sizeof(WORD), (void**)&pButterFlyIdxa, 0) ) )
#else
	if ( FAILED ( m_pButterFlyIB[1]->Lock(0, 6*sizeof(WORD), (BYTE**)&pButterFlyIdxa, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	E_FAIL;
	pButterFlyIdxa[0] = 1;
	pButterFlyIdxa[1] = 2;
	pButterFlyIdxa[2] = 3;
	pButterFlyIdxa[3] = 3;
	pButterFlyIdxa[4] = 0;
	pButterFlyIdxa[5] = 1;

	m_pButterFlyIB[1]->Unlock();


	if( FAILED(g_RenderManager.CreateVertexBuffer( 4 * sizeof(BIRDVERTEX),
												  D3DUSAGE_WRITEONLY, D3DFVF_BIRDVERTEX,
												  D3DPOOL_MANAGED, &m_pButterFlyVB[1] ) ) )
	{
		return E_FAIL;
	}

	BIRDVERTEX* pButterFlyVtxa;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pButterFlyVB[1]->Lock( 0, 0, (void**)&pButterFlyVtxa, 0 ) ) )
#else
	if( FAILED( m_pButterFlyVB[1]->Lock( 0, 0, (BYTE**)&pButterFlyVtxa, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return E_FAIL;

	pButterFlyVtxa[0].v	=	D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	pButterFlyVtxa[1].v	=	D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	pButterFlyVtxa[2].v	=	D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pButterFlyVtxa[3].v	=	D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	
	pButterFlyVtxa[0].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pButterFlyVtxa[1].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pButterFlyVtxa[2].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pButterFlyVtxa[3].nv =	D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pButterFlyVtxa[0].tu	=	0.0f;
	pButterFlyVtxa[0].tv	=	1.0f;
	pButterFlyVtxa[1].tu	=	0.0f;
	pButterFlyVtxa[1].tv	=	0.0f;
	pButterFlyVtxa[2].tu	=	0.5f;
	pButterFlyVtxa[2].tv	=	0.0f;
	pButterFlyVtxa[3].tu	=	0.5f;
	pButterFlyVtxa[3].tv	=	1.0f;

	m_pButterFlyVB[1]->Unlock();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	char	m_cTexPath[128];
	char	m_cTexPathTemp[128];
	sprintf(m_cTexPath, "%s\\Effect\\tex\\", directory);

	switch( insecttype )
	{
	case 0:
		{
			break;
		}
	case 1:
		{
			sprintf(m_cTexPathTemp, "%sbutterfly.tga", m_cTexPath);

			m_pInsectTex[1] = NULL;
			if( FAILED(g_RenderManager.CreateTextureFromFileEx( m_cTexPathTemp, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
																D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																0xff000000, NULL, NULL, &m_pInsectTex[1] )))
					return E_FAIL;
			break;
		}
	case 2:
		{
			sprintf(m_cTexPathTemp, "%sBird.tga", m_cTexPath);

			m_pInsectTex[2] = NULL;
			if( FAILED(g_RenderManager.CreateTextureFromFileEx(  m_cTexPathTemp, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
																D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																0xff000000, NULL, NULL, &m_pInsectTex[2] )))
					return E_FAIL;
			break;
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	switch( insecttype )
	{
	case 0:
		{
			break;
		}
	case 1:
		{
			m_pInsectTex[1] = NULL;
			VFileHandle* pFH = g_VFEffectTex.OpenFile( "butterfly.tga" );	
			if( pFH )
			{
				HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																			D3DX_DEFAULT, D3DX_DEFAULT, 
																			1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																			0, NULL, NULL, &m_pInsectTex[1] );
				
				g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
			}		
			break;
		}
	case 2:
		{
			m_pInsectTex[2] = NULL;
			VFileHandle* pFH = g_VFEffectTex.OpenFile( "Bird.tga" );	
			if( pFH )
			{
				HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																			D3DX_DEFAULT, D3DX_DEFAULT, 
																			1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																			0, NULL, NULL, &m_pInsectTex[2] );
				
				g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
			}		
			break;
		}
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	return S_OK;
}



void	CInsect_Manager::Change_Insect( DWORD num )
{
	//랜덤으로 방향 생성
	float RandomYaw		 = RANDOM_NUM * 6.28318f;
	float RandomPitch	 = RANDOM_NUM * 6.28318f ;
	m_Insect[num].m_vInsect_Vel.x	= sinf(RandomPitch) * cosf(RandomYaw) * 0.5f;
	m_Insect[num].m_vInsect_Vel.y	= 0.0f;
	m_Insect[num].m_vInsect_Vel.z	= sinf(RandomPitch) * sinf(RandomYaw) * 0.5f;
	m_Insect[num].m_fInsect_Dir		= Dir_Insect(m_Insect[num].m_vInsect_Vel);
	m_Insect[num].m_fInsect_Life	= (RANDOM_NUM + 2);
}

void	CInsect_Manager::Relife_Insect( DWORD num )
{
	/*
	int x = (int)timeGetTime()%4;
	int y , z;

	switch( x ){
	case 0:	{y = 1;	z = 1;	break;}
	case 1:	{y = 1;	z = -1;	break;}
	case 2:	{y = -1;z = 1;	break;}
	case 3:	{y = -1;z = -1;	break;}
	}

	m_Insect[num].m_vInsect_Loc.x = ((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) * y + g_Pc.GetDestX() * 8 + 4;
	m_Insect[num].m_vInsect_Loc.z = ((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) * z + g_Pc.GetDestY() * 8 + 4;
	*/
	int MapSize = g_Pc.GetMapSize();

	//랜덤으로 방향 생성에 필요한 인자들 
	float RandomYaw		 = RANDOM_NUM * 6.28318f;
	float RandomPitch	 = RANDOM_NUM * 6.28318f ;

	//사면에서랜덤으로 생성 방향은 화면 안쪽으로 
	float velx = LimitVel(sinf(RandomPitch) * cosf(RandomYaw));
	float velz = LimitVel(sinf(RandomPitch) * sinf(RandomYaw));

	int x = (int)timeGetTime()%4;
	switch( x ){
	case 0:	{
		m_Insect[num].m_vInsect_Loc.x = ((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) + g_Pc.GetDestX() * 8 + 4;
		m_Insect[num].m_vInsect_Loc.z = (MAX_MOVINGINSECT - 45) * RANDOM_NUM + g_Pc.GetDestY() * 8 + 4;
		m_Insect[num].m_vInsect_Vel.x	= -MinusToPlus(velx);
		m_Insect[num].m_vInsect_Vel.z	= velz;
		break;}
	case 1:	{
		m_Insect[num].m_vInsect_Loc.x = (MAX_MOVINGINSECT - 45) * RANDOM_NUM + g_Pc.GetDestX() * 8 + 4;
		m_Insect[num].m_vInsect_Loc.z = ((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) + g_Pc.GetDestY() * 8 + 4;
		m_Insect[num].m_vInsect_Vel.x	= velx;
		m_Insect[num].m_vInsect_Vel.z	= -MinusToPlus(velz);
		break;}
	case 2:	{
		m_Insect[num].m_vInsect_Loc.x = (((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) * -1 ) + g_Pc.GetDestX() * 8 + 4;
		m_Insect[num].m_vInsect_Loc.z = (MAX_MOVINGINSECT - 45) * RANDOM_NUM + g_Pc.GetDestY() * 8 + 4;
		m_Insect[num].m_vInsect_Vel.x	= MinusToPlus(velx);
		m_Insect[num].m_vInsect_Vel.z	= velz;
		break;}
	case 3:	{
		m_Insect[num].m_vInsect_Loc.x = (MAX_MOVINGINSECT - 45) * RANDOM_NUM + g_Pc.GetDestX() * 8 + 4;
		m_Insect[num].m_vInsect_Loc.z = (((MAX_MOVINGINSECT - 15) + RANDOM_NUM * 5 ) * -1 ) + g_Pc.GetDestY() * 8 + 4;
		m_Insect[num].m_vInsect_Vel.x	= velx;
		m_Insect[num].m_vInsect_Vel.z	= MinusToPlus(velz);
		break;}
	}
	int xloc = (int)(m_Insect[num].m_vInsect_Loc.x/8);
	int zloc = (int)(m_Insect[num].m_vInsect_Loc.z/8);
	if( MapSize > xloc || 0 < xloc || MapSize > zloc || zloc < 0 )
	{
		if( xloc > 0 && zloc > 0 )
		{
			m_Insect[num].m_vInsect_Loc.y =  g_Map.Get__MapHeight( xloc, zloc );
		}
	}
	else
	{
		Relife_Insect(num);
		return;
	}
}


void	CInsect_Manager::Change_Bird()
{
	int MapSize = g_Pc.GetMapSize();

	int random = (int)timeGetTime()%10;
	if( random )
	{
		//새 숫자를 새로 만들때 랜덤으로 조정
		m_iBird_Num = MAX_BIRD - 2 + (int)(RANDOM_NUM * 2);

		//랜덤으로 방향 생성에 필요한 인자들 
		float RandomYaw		 = RANDOM_NUM * 6.28318f;
		float RandomPitch	 = RANDOM_NUM * 6.28318f ;

		float velx = LimitVel(sinf(RandomPitch) * cosf(RandomYaw));
		float velz = LimitVel(sinf(RandomPitch) * sinf(RandomYaw));

		//사면에서랜덤으로 생성 방향은 화면 안쪽으로 
		int x = (int)timeGetTime()%4;
		switch( x ){
		case 0:	{
			m_vBird_Pos.x = ((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) + g_Pc.GetDestX() * 8 + 4;
			m_vBird_Pos.z = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + g_Pc.GetDestY() * 8 + 4;
			m_vBird_Vel.x	= -MinusToPlus(velx);
			m_vBird_Vel.z	= velz;
			break;}
		case 1:	{
			m_vBird_Pos.x = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + g_Pc.GetDestX() * 8 + 4;
			m_vBird_Pos.z = ((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) + g_Pc.GetDestY() * 8 + 4;
			m_vBird_Vel.x	= velx;
			m_vBird_Vel.z	= -MinusToPlus(velz);
			break;}
		case 2:	{
			m_vBird_Pos.x = (((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) * -1 ) + g_Pc.GetDestX() * 8 + 4;
			m_vBird_Pos.z = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + g_Pc.GetDestY() * 8 + 4;
			m_vBird_Vel.x	= MinusToPlus(velx);
			m_vBird_Vel.z	= velz;
			break;}
		case 3:	{
			m_vBird_Pos.x = (MAX_MOVINGBIRD - 45) * RANDOM_NUM + g_Pc.GetDestX() * 8 + 4;
			m_vBird_Pos.z = (((MAX_MOVINGBIRD - 15) + RANDOM_NUM * 5 ) * -1 ) + g_Pc.GetDestY() * 8 + 4;
			m_vBird_Vel.x	= velx;
			m_vBird_Vel.z	= MinusToPlus(velz);
			break;}
		}
	//	if (!g_Camera.PointInFrustum(m_vBird_Pos.x,m_vBird_Pos.y,m_vBird_Pos.z)) {Change_Bird();return;}
		int xloc = (int)(m_vBird_Pos.x/8);
		int zloc = (int)(m_vBird_Pos.z/8);
		if( MapSize > xloc || 0 < xloc || MapSize > zloc || zloc < 0 )
		{
			if( xloc > 0 && zloc > 0 )
			{
				m_vBird_Pos.y =  g_Map.Get__MapHeight( xloc, zloc );
			}
		}
		else
		{
			Change_Bird();
			return;
		}

		m_fBird_Fast	= MinusToPlus(sinf(RandomPitch));
		m_fBird_Life	= 0.0f;

		//휘어짐의 정도 인자 
		x = (int)timeGetTime()%2;
		switch( x ){
		case 0:	{m_vBird_Facter.x	= m_fBird_Fast/100;		break;}
		case 1:	{m_vBird_Facter.x	= -m_fBird_Fast/100;	break;}
		}

		m_vBird_Facter.z = ((m_vBird_Facter.x * m_vBird_Vel.x) * -1.0f) / m_vBird_Vel.z;
		
		//중점으로 부터 거리 조정 계산 그리고 그 거리 저장
		for( DWORD	i = 0; i < m_iBird_Num; i++ )
		{
			m_Bird[i].m_fBird_Opp[0] = RANDOM_NUM * 2;
			m_Bird[i].m_fBird_Opp[1] = RANDOM_NUM * 2;
			m_Bird[i].m_vBird_Loc.x = m_vBird_Pos.x + m_Bird[i].m_fBird_Opp[0];
			m_Bird[i].m_vBird_Loc.z = m_vBird_Pos.z + m_Bird[i].m_fBird_Opp[1];

			//날개짓 함
			m_Bird[i].m_bStop_Bird	= false;
		}
	}
}




void	CInsect_Manager::Update_Insect(int insectnum, float time)
{
	if( g_Pc.GetMainPC().nIndex == -1 )
	{
		return;		
	}

	switch( insectnum )
	{
	case 0:
		{
			D3DXMATRIX matworldtemp;
			D3DXMATRIX matworldtempA;
			
			D3DXMatrixRotationY(&matworldtemp, -g_Camera.m_fCameraYaw );
			D3DXMatrixRotationX(&matworldtempA, g_Camera.m_fNewPitch);
			D3DXMatrixMultiply(&matworldtemp, &matworldtempA, &matworldtemp);

			int MapSize = g_Pc.GetMapSize();
			for( DWORD	i = 0; i < MAX_INSECT; i++ )
			{
				float x = m_Insect[i].m_vInsect_Loc.x/8;
				float z = m_Insect[i].m_vInsect_Loc.z/8;
				
				if( MapSize > x || 0 < x || MapSize > z || MapSize < 0 )
				{
					m_Insect[i].m_fInsect_Life -= time;
					
					if( m_Insect[i].m_fInsect_Life < 0 )
					{
						Change_Insect(i);
					}
					//60의 범위 에서돌아 다닌다 아닌경우에는 새로운 방향 
					else if( g_Pc.GetDestX()*8 + MAX_MOVINGINSECT + 4.0f < m_Insect[i].m_vInsect_Loc.x ||
								g_Pc.GetDestX()*8 - MAX_MOVINGINSECT + 4.0f > m_Insect[i].m_vInsect_Loc.x ||
								g_Pc.GetDestY()*8 + MAX_MOVINGINSECT + 4.0f < m_Insect[i].m_vInsect_Loc.z ||
								g_Pc.GetDestY()*8 - MAX_MOVINGINSECT + 4.0f > m_Insect[i].m_vInsect_Loc.z )
					{
						Relife_Insect(i);
					}
					else
					{
						m_Insect[i].m_vInsect_Loc		+=	m_Insect[i].m_vInsect_Vel;
//						m_Insect[i].m_vInsect_Loc.y	=	sinf((float)timeGetTime()/100) + 10 + g_Map.Get__MapHeight( x, z );
					}
					
					D3DXMATRIX matWorld;
					D3DXMatrixTranslation(&matWorld, m_Insect[i].m_vInsect_Loc.x, 
														m_Insect[i].m_vInsect_Loc.y,
														m_Insect[i].m_vInsect_Loc.z );
					m_Insect[i].m_matInsect	=	matWorld;
					D3DXMatrixMultiply(&m_Insect[i].m_matInsect, &matworldtemp, &m_Insect[i].m_matInsect);
				}
				else
				{
					Relife_Insect(i);
				}
			}
			break;
		}
	case 1:
		{
			float temploc;
			int MapSize = g_Pc.GetMapSize();
			for( DWORD	i = 0; i < MAX_INSECT; i++ )
			{
				int x = (int)(m_Insect[i].m_vInsect_Loc.x/8);
				int z = (int)(m_Insect[i].m_vInsect_Loc.z/8);
				if( MapSize > x || 0 < x || MapSize > z || MapSize < 0 )
				{
					m_Insect[i].m_fInsect_Life -= time;

					if( m_Insect[i].m_fInsect_Life < 0 )
					{
						Change_Insect(i);
					}
					//60의 범위 에서돌아 다닌다 아닌경우에는 새로운 방향
					else if( g_Pc.GetDestX()*8 + MAX_MOVINGINSECT + 4.0f < m_Insect[i].m_vInsect_Loc.x ||
								g_Pc.GetDestX()*8 - MAX_MOVINGINSECT + 4.0f > m_Insect[i].m_vInsect_Loc.x ||
								g_Pc.GetDestY()*8 + MAX_MOVINGINSECT + 4.0f < m_Insect[i].m_vInsect_Loc.z ||
								g_Pc.GetDestY()*8 - MAX_MOVINGINSECT + 4.0f > m_Insect[i].m_vInsect_Loc.z )
					{
						Relife_Insect(i);
					}
					else
					{
						m_Insect[i].m_vInsect_Loc		+=	m_Insect[i].m_vInsect_Vel;
						temploc	=	(float)sinf((float)(g_nowTime/100)) + 10 + m_Insect[i].m_vInsect_Loc.y;
					}

					D3DXMATRIX matWorld;
					D3DXMatrixTranslation(&matWorld, m_Insect[i].m_vInsect_Loc.x,
														temploc,
														m_Insect[i].m_vInsect_Loc.z );
					m_Insect[i].m_matInsect	=	matWorld;
				}
				else
				{
					Relife_Insect(i);
				}
			}
			break;
		}
	case 2:
		{
			int MapSize = g_Pc.GetMapSize();
			m_fBird_Life += time;
			//60의 범위 에서돌아 다닌다 아닌경우에는 새로운 방향 
			if( g_Pc.GetDestX()*8 + MAX_MOVINGBIRD + 4.0f < m_vBird_Pos.x ||
				g_Pc.GetDestX()*8 - MAX_MOVINGBIRD + 4.0f > m_vBird_Pos.x ||
				g_Pc.GetDestY()*8 + MAX_MOVINGBIRD + 4.0f < m_vBird_Pos.z ||
				g_Pc.GetDestY()*8 - MAX_MOVINGBIRD + 4.0f > m_vBird_Pos.z )
			{
				Change_Bird();
			}
			else
			{
				m_vBird_Pos			+=	m_vBird_Vel * 50.0f * time * 0.667f;
//				m_vBird_Pos			+=	m_vBird_Vel;
				m_vBird_Vel			+=	m_vBird_Facter;
				//방향과 속도 
				m_fBird_Dir		= Dir_Insect(m_vBird_Vel);

				for( DWORD	i = 0; i < m_iBird_Num; i++ )
				{
					m_Bird[i].m_vBird_Loc.x = m_vBird_Pos.x + m_Bird[i].m_fBird_Opp[0];
					m_Bird[i].m_vBird_Loc.z = m_vBird_Pos.z + m_Bird[i].m_fBird_Opp[1];
					float x = m_Bird[i].m_vBird_Loc.x/8;
					float z = m_Bird[i].m_vBird_Loc.z/8;
					
					if( MapSize > x || 0 < x || MapSize > z || z < 0 )
					{
						float	temp, temptime, count;
						
						temptime = (float)timeGetTime()/1000 + i;
						temp = cosf(temptime);
						count = (float)((((int)(temptime) * 100 )% 628)/100);
						
						if ( count < 3.14f ){	m_Bird[i].m_bStop_Bird = true;}
						else{					m_Bird[i].m_bStop_Bird = false;}

						m_Bird[i].m_vBird_Loc.y	= temp * 10 + 60 + m_vBird_Pos.y + m_fBird_Life;
					}
					else
					{
						Change_Bird();
					}
				}
			}

			for( DWORD	i = 0; i < m_iBird_Num; i++ )
			{
				D3DXMATRIX matWorld;
				D3DXMatrixTranslation(&matWorld, m_Bird[i].m_vBird_Loc.x, 
													m_Bird[i].m_vBird_Loc.y,
													m_Bird[i].m_vBird_Loc.z );
				m_Bird[i].m_mat_Bird	=	matWorld;
			}
			break;
		}
	}
}




void	CInsect_Manager::Ani_Insect()
{

	float temptime =	(float)timeGetTime()/100;
	//0번 날개와 1번 날개는 대칭 되야함 
	D3DXMatrixRotationZ(&m_matIst_Ani[0], sinf(temptime));
	D3DXMatrixRotationZ(&m_matIst_Ani[1], -sinf(temptime));
}



void	CInsect_Manager::Ani_Bird()
{
	float temptime =	(float)timeGetTime()/80;
	//0번 날개와 1번 날개는 대칭 되야함 
	D3DXMatrixRotationZ(&m_matIst_Ani[0], sinf(temptime));
	D3DXMatrixRotationZ(&m_matIst_Ani[1], -sinf(temptime));
}




float	CInsect_Manager::Dir_Insect(D3DXVECTOR3& v)
{
	if( v.z == 0 ){ v.z = 0.00001f; }

	float	Dir = 0.0f;
	double	Theta = v.x/v.z;

	if( v.x > 0 && v.z > 0)
	{
		Dir = (float)atan(Theta) - 3.14159f;
	}
	else if( v.x > 0 && v.z < 0)
	{
		Dir = (float)atan(Theta);
	}
	else if( v.x < 0 && v.z > 0)
	{
		Dir = (float)atan(Theta) + 3.14159f;
	}
	else if( v.x < 0 && v.z < 0)
	{
		Dir = (float)atan(Theta);
	}



	return	Dir;
}

float	CInsect_Manager::MinusToPlus(float	Minus)
{
	if( Minus > 0 )
	{
		return 	Minus;
	}
	else
		return	-Minus;
}

float	CInsect_Manager::LimitVel(float vel)
{
	if( vel > 0 ){
		if( vel > 0.8f ){		vel = 0.8f; }
		else if( vel < 0.5f ){	vel = 0.5f;	}
	}
	else{	if( vel < -0.8f ){		vel = -0.8f; }
			else if( vel > -0.5f ){	vel = -0.5f; }
	}

	return vel;
}

HRESULT	CInsect_Manager::Render_Insect(int isttype, float time)
{
	if( isttype == 1 )
	{
		Ani_Insect();
	}
	else
	{
		Ani_Bird();
	}
	Update_Insect(isttype, time);

	switch( isttype )
	{
	case 0:
		{
			g_RenderManager.SetStreamSource( 0, m_pGlimmerVB, sizeof(INSECTVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_INSECTVERTEX );
			g_RenderManager.SetIndices(m_pGlimmerIB, 0);
			g_RenderManager.SetTexture( 0, m_pInsectTex[1] );

			for( DWORD	i = 0; i < MAX_INSECT; i++ )
			{
				g_RenderManager.SetTransform(D3DTS_WORLD, &m_Insect[i].m_matInsect );
				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
			}
			break;
		}
	case 1:
		{
#ifdef DIRECT_VERSION_9_MJH
			D3DMATERIAL9 mtrl;
#else
			D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH
			//색을 넣기 위한 매터리얼 
			D3DUtil_InitMaterial( mtrl, 1.0f,1.0f,0.0f,1.0f);
			mtrl.Emissive.r = 1.0f;
			mtrl.Emissive.g = 1.0f;
			mtrl.Emissive.b = 1.0f;
			mtrl.Emissive.a = 1.0f;
			mtrl.Specular.r = 1.0f;
			mtrl.Specular.g = 1.0f;
			mtrl.Specular.b = 1.0f;
			mtrl.Specular.a = 1.0f;
			g_RenderManager.SetMaterial( &mtrl );
			
			g_RenderManager.SetStreamSource( 0, m_pButterFlyVB[0], sizeof(BIRDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BIRDVERTEX );
			g_RenderManager.SetIndices(m_pButterFlyIB[0], 0);
			g_RenderManager.SetTexture( 0, m_pInsectTex[1] );

			for( DWORD	i = 0; i < MAX_INSECT; i++ )
			{
				D3DXMATRIX temp;
				
				D3DXMatrixRotationY(&temp, m_Insect[i].m_fInsect_Dir);
				D3DXMatrixMultiply(&temp, &m_matIst_Ani[0], &temp);
				D3DXMatrixMultiply(&temp, &temp,			&m_Insect[i].m_matInsect);

				g_RenderManager.SetTransform(D3DTS_WORLD, &temp );
				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2 );
			}
			g_RenderManager.SetStreamSource( 0, m_pButterFlyVB[1], sizeof(BIRDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BIRDVERTEX );
			g_RenderManager.SetIndices(m_pButterFlyIB[1], 0);
			g_RenderManager.SetTexture( 0, m_pInsectTex[1] );

			for(int i = 0; i < MAX_INSECT; i++ )
			{
				D3DXMATRIX tempa;
				
				D3DXMatrixRotationY(&tempa, m_Insect[i].m_fInsect_Dir);
				D3DXMatrixMultiply(&tempa, &m_matIst_Ani[1], &tempa);
				D3DXMatrixMultiply(&tempa, &tempa,			&m_Insect[i].m_matInsect);

				g_RenderManager.SetTransform(D3DTS_WORLD, &tempa );
				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2 );
			}

			break;
		}
		case 2:
		{
#ifdef DIRECT_VERSION_9_MJH
			D3DMATERIAL9 mtrl;
#else
			D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH
			//색을 넣기 위한 매터리얼 
			D3DUtil_InitMaterial( mtrl, 0.8f,0.5f,0.3f,1.0f);
			mtrl.Emissive.r = 1.0f;
			mtrl.Emissive.g = 1.0f;
			mtrl.Emissive.b = 1.0f;
			mtrl.Emissive.a = 1.0f;
			mtrl.Specular.r = 1.0f;
			mtrl.Specular.g = 1.0f;
			mtrl.Specular.b = 1.0f;
			mtrl.Specular.a = 1.0f;
			g_RenderManager.SetMaterial( &mtrl );

			g_RenderManager.SetStreamSource( 0, m_pButterFlyVB[0], sizeof(BIRDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BIRDVERTEX );
			g_RenderManager.SetIndices(m_pButterFlyIB[0], 0);
			g_RenderManager.SetTexture( 0, m_pInsectTex[2] );

			for( DWORD	i = 0; i < m_iBird_Num; i++ )
			{
				D3DXMATRIX temp;
				D3DXMatrixRotationY(&temp, m_fBird_Dir);
				
				if( m_Bird[i].m_bStop_Bird == true)
				{
					D3DXMatrixMultiply(&temp, &temp,			&m_Bird[i].m_mat_Bird);
				}
				else
				{
					D3DXMatrixMultiply(&temp, &m_matIst_Ani[0], &temp);
					D3DXMatrixMultiply(&temp, &temp,			&m_Bird[i].m_mat_Bird);
				}

				g_RenderManager.SetTransform(D3DTS_WORLD, &temp );
				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2 );
			}
	
			g_RenderManager.SetStreamSource( 0, m_pButterFlyVB[1], sizeof(BIRDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BIRDVERTEX );
			g_RenderManager.SetIndices(m_pButterFlyIB[1], 0);
			g_RenderManager.SetTexture( 0, m_pInsectTex[2] );

			for(int i = 0; i < m_iBird_Num; i++ )
			{
				D3DXMATRIX tempa;
				D3DXMatrixRotationY(&tempa, m_fBird_Dir);

				if( m_Bird[i].m_bStop_Bird == true)
				{
					D3DXMatrixMultiply(&tempa, &tempa,			&m_Bird[i].m_mat_Bird);
				}
				else
				{
					D3DXMatrixMultiply(&tempa, &m_matIst_Ani[1], &tempa);
					D3DXMatrixMultiply(&tempa, &tempa,			&m_Bird[i].m_mat_Bird);
				}
				g_RenderManager.SetTransform(D3DTS_WORLD, &tempa );
				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2 );
			}

			break;
		}
	}

	return	S_OK;
}