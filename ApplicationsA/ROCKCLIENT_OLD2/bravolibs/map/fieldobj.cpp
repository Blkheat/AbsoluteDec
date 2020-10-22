#include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
//#include	"..\Network.H"
#include	"..\\quadlist.h"
#include	"..\\RockClient.h"
#include	"..\\Camera.h"
#include	"..\\Pc.h"
#include	"Field.h"
#include	<OBJ\\ObjMath.h>
#include	<OBJ\Object_Vertex.h>

#include "profile\\profile.h"

#define		OBJ_SORT_TIME		2000.0f		// 소팅시간(5초 5000ms) by soohyun

extern	long	g_nowTime;	// 현재시간
extern	bool	g_bBlockDebug;



void		CField::CleanupObj()
{
	for ( DWORD i = 0; i < MAX_OBJ; i ++ )
	{
		if ( m_Object[i].m_bIsLoaded == true )
		{
			m_Object[i].Destroy();
		}
	}
	//m_TempObj.Destroy();
	//m_dwObjCount	= 0;
	m_nCurObj		= -1;
	m_ObjCount		= 0;
}

void		CField::CleanObjParticleAliveInfo()
{
	for ( short i = 0; i < m_ObjCount; i ++ )
	{
		for ( short j = 0; j < m_Object[i].m_child_count; j++ )
		{
			m_Object[i].m_pAniPos[j].m_bIsParticleAlive = false;
			m_Object[i].m_pAniPos[j].m_bIsBillboardAlive = false;
		}
	}
}

bool		CField::UpdateObj()
{
	dwObjVisibleCount	= 0;

	//for ( DWORD i = 0; i < m_dwObjCount; i ++ )
	for ( DWORD i = 0; i < MAX_OBJ; i ++ )
	{
		if ( m_Object[i].m_bIsLoaded )
		{	
			for( int j = 0; j < m_Object[i].m_child_count; ++j )
			{
				if ( m_Object[i].m_dwIndex == 41 )
					int	ongs= 1;

				float		temp;
				
				if ( m_Object[i].m_Bound.m_min_vertex.z > m_Object[i].m_Bound.m_max_vertex.z 
				||	m_Object[i].m_Bound.m_min_vertex.x > m_Object[i].m_Bound.m_max_vertex.x 
				||	m_Object[i].m_Bound.m_min_vertex.y > m_Object[i].m_Bound.m_max_vertex.y )
				{
					temp	= m_Object[i].m_Bound.m_min_vertex.z;
					m_Object[i].m_Bound.m_min_vertex.z	= m_Object[i].m_Bound.m_max_vertex.z;
					m_Object[i].m_Bound.m_max_vertex.z	= temp;

					temp	= m_Object[i].m_Bound.m_min_vertex.x;
					m_Object[i].m_Bound.m_min_vertex.x	= m_Object[i].m_Bound.m_max_vertex.x;
					m_Object[i].m_Bound.m_max_vertex.x	= temp;

					temp	= m_Object[i].m_Bound.m_min_vertex.y;
					m_Object[i].m_Bound.m_min_vertex.y	= m_Object[i].m_Bound.m_max_vertex.y;
					m_Object[i].m_Bound.m_max_vertex.y	= temp;
				}

				D3DXVECTOR3		min	= m_Object[i].m_Bound.m_min_vertex;
				D3DXVECTOR3		max	= m_Object[i].m_Bound.m_max_vertex;
				D3DXMATRIX		mat;
				D3DXMatrixIdentity(&mat);
				mat._41	= m_Object[ i ].m_pAniPos[ j ].GetPosTM()._41;
				mat._42	= m_Object[ i ].m_pAniPos[ j ].GetPosTM()._42;
				mat._43	= m_Object[ i ].m_pAniPos[ j ].GetPosTM()._43;


				D3DXVec3TransformCoord(&min, &min, &mat);
				D3DXVec3TransformCoord(&max, &max, &mat);

				float	sizex	= (max.x - min.x);
				float	sizey	= (max.y - min.y);
				float	sizez	= (max.z - min.z);


				m_Object[i].m_pAniPos[ j ].m_bIsVisible = g_Camera.CubeInFrustum(min.x, min.y, min.z, sizex, sizey, sizez); 

				if( m_Object[i].m_pAniPos[ j ].m_bIsVisible )
				{
					dwObjVisibleCount	++;
				}				
			}
		}
	}

//	for ( DWORD i = 0; i < m_dwObjCount; i ++ )
//		m_Object[i].m_bIsVisible	= true;
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 오브젝트 그리기
//-----------------------------------------------------------------------------
int ObjectSortProcess( const VOID* arg1, const VOID* arg2 )
{	
    OBJ_SORT_TABLE* p1 = (OBJ_SORT_TABLE*)arg1;
    OBJ_SORT_TABLE* p2 = (OBJ_SORT_TABLE*)arg2;
	
	// log
//	#ifdef GAMETYPE_TEST
//	{
//		if( p1 == NULL )
//			MessageBox( NULL, "ObjectSortCB : p1 Pointer NULL", "", MB_OK );
//		if( p2 == NULL )
//			MessageBox( NULL, "ObjectSortCB : p2 Pointer NULL", "", MB_OK );
//	}
//	#endif
    
	
	// p1 오브젝트가 보이는가?
	if( g_Map.m_Object[p1->Index].m_pAniPos[p1->Child].m_bIsVisible == false )
		return -1;

	// 물 오브젝트 소트 위치 뒤로
	if( g_Map.m_Object[p1->Index].m_pAttr[0].m_bNight == TRUE &&
		g_Map.m_Object[p2->Index].m_pAttr[0].m_bNight == TRUE    )
	{
	}
	else if( g_Map.m_Object[p1->Index].m_pAttr[0].m_bNight == TRUE &&
			 g_Map.m_Object[p2->Index].m_pAttr[0].m_bNight == FALSE    )
	{
		return +1;
	}
	else if( g_Map.m_Object[p1->Index].m_pAttr[0].m_bNight == FALSE &&
			 g_Map.m_Object[p2->Index].m_pAttr[0].m_bNight == TRUE    )
	{
		return -1;
	}


	

	// 2개 오브젝트의 위치값을 비교
	float _fX1	= g_Map.m_Object[p1->Index].m_pAniPos[p1->Child].GetPosTM()._41;
	float _fZ1	= g_Map.m_Object[p1->Index].m_pAniPos[p1->Child].GetPosTM()._43;
	float _fX2	= g_Map.m_Object[p2->Index].m_pAniPos[p2->Child].GetPosTM()._41;
	float _fZ2	= g_Map.m_Object[p2->Index].m_pAniPos[p2->Child].GetPosTM()._43;
	float _fD1	= (_fX1*g_Camera.GetAlphaSortRot().x) + (_fZ1*g_Camera.GetAlphaSortRot().z);
	float _fD2	= (_fX2*g_Camera.GetAlphaSortRot().x) + (_fZ2*g_Camera.GetAlphaSortRot().z);
	if( _fD1 < _fD2 )
		return +1;
	return -1;
}

//.....................................................................................................................
// Vec3 , Color ( CVERTEX )
//.....................................................................................................................
typedef struct s_CVERTEX
{
	
	D3DXVECTOR3	pos;
	D3DCOLOR	color;

	s_CVERTEX()
	{
		color = D3DCOLOR_XRGB( 0 , 0 , 255 );
	}

}CVERTEX,*LPCVERTEX;

#define D3DFVF_CVERTEX		(D3DFVF_XYZ|D3DFVF_DIFFUSE)

void SetupMaxMin2(D3DXVECTOR3* max, D3DXVECTOR3* min)
{
	float	temp;

	// setup max
	if ( max->x < min->x )
	{
		temp	= max->x;
		max->x	= min->x;
		min->x	= temp;
	}
	if ( max->y < min->y )
	{
		temp	= max->y;
		max->y	= min->y;
		min->y	= temp;
	}
	if ( max->z < min->z )
	{
		temp	= max->z;
		max->z	= min->z;
		min->z	= temp;
	}
}


HRESULT		CField::DisplayObj()
{	
/*

/**/

	return S_OK;
}

//-----------------------------------------------------------------------------
// Desc: 오브젝트 이펙트
//-----------------------------------------------------------------------------
void	CField::EffectDisplayUpdate()
{
	for( DWORD i = 0; i < m_ObjCount; ++i )
	{
		if( m_Object[i].m_bIsLoaded )		
		{			
			m_Object[i].EffectDisplayUpdate();
			m_Object[i].BillboardDisplayUpdate();
		}
	}

}


//-----------------------------------------------------------------------------
// Desc: 오브젝트 그림자 생성하기
//-----------------------------------------------------------------------------
void	CField::ShadowMakeObj()
{
	
	if( m_bShadowMakeObj == true )
	{		
		for ( DWORD i = 0; i < m_ObjCount; ++i )
		{
			if( m_Object[i].m_bIsLoaded )							
				m_Object[i].ShadowMake();			
		}
		m_bShadowMakeObj    = false;
		m_bShadowDisplayObj = true;
	}
	
}



//-----------------------------------------------------------------------------
// Desc: 오브젝트 그림자 그리기
//-----------------------------------------------------------------------------
void	CField::ShadowDisplayObj()
{	
	
	if( m_bShadowDisplayObj == true )
	{		
		for ( DWORD i = 0; i < m_ObjCount; ++i )
		{
			if ( m_Object[i].m_bIsLoaded )					
				m_Object[i].ShadowDisplay();
		}		
	}
	
}




//-----------------------------------------------------------------------------
// Desc: 알파 오브젝트 테이블만들기
//-----------------------------------------------------------------------------
void	CField::CreateObjSortTable()
{
/*
	// 테이블저장	
	int number = 0;
	for( int i = 0; i < m_ObjCount; ++i )
	{	
		// 알파블렌딩 해야 하는가?
//		if( m_Object[i].GetAlpahEnable() == true )
		{
			for( int j = 0; j < m_Object[i].m_child_count; ++j )
			{
				m_pAlphaTable[number].Index	= i;
				m_pAlphaTable[number].Child	= j;
				//m_pAlphaTable[number].x = m_Object[i].m_pAniPos[j].GetPosTM()._41;
				//m_pAlphaTable[number].z = m_Object[i].m_pAniPos[j].GetPosTM()._43;				
				number++;
			}
		}
	}
	
	// 소트갯수 저장
	m_SortCount = number;
*/
}

//-----------------------------------------------------------------------------
// Desc: 빌보드 오브젝트
//-----------------------------------------------------------------------------
void	CField::CreateBilObj()
{	
	// 현재 맵의 셋트 알아내기		
//	int i = g_Obj_Manager.GetNowSet( g_Pc.m_MainPC.lCurMap );
	int i = 0;

	// 빌보드 오브젝트 로딩
	m_BilObjCount = 0;
	char file_path[256];
	for( int j = 0; j < MAX_OBJ; ++j )
	{
		// 빌보드 인덱스
		int lod = g_Obj_Manager.m_SetManager.SetInfo[i].nlod[j];
		if( lod >= 0 )
		{
			// 빌보드 리스트에서 해당 인덱스와 같은 것이 있는지 검사
			bool bFind = true;
			for( int k = 0; k < m_BilObjCount; ++k )
			{
				if( m_BilObj[k].m_dwIndex == lod )
				{
					bFind = false;
					break;
				}
			}
			// 새로운 인덱스만 추가
			if( bFind == true )
			{
				m_BilObj[m_BilObjCount].SetupIndex( g_Obj_Manager.m_SetManager.SetInfo[i].nlod[j] );
				sprintf( file_path, "%s\\Obj\\%04d.brv_obj", g_RockClient.GetRootDir(), m_BilObj[m_BilObjCount].m_dwIndex );
				m_BilObj[m_BilObjCount].File__Open( file_path );
				m_BilObjCount++;
			}
		}
	}
}

void	CField::DisplayBilObj( int lod, int child, Object* pObj )
{
	for( int i = 0; i < MAX_BILOBJ; ++i )
	{
		if( m_BilObj[i].m_dwIndex == lod )	
		{
			m_BilObj[i].BilDisplay( pObj, child );
			break;		
		}
	}
}


/****************************************************************************************************
	* Temporary Display
****************************************************************************************************/


HRESULT		CField::PickObject(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
{
	return S_OK;
}


/****************************************************************************************************
	* end of Temporary Display
****************************************************************************************************/