// EFFameItemSet.cpp: implementation of the CEFFameItemSet class.
//
//////////////////////////////////////////////////////////////////////

#include "CEFFameItemSet.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEFFameItemSet::CEFFameItemSet()
{
	m_Level     = 0;
	m_EldaType	= 1; 

	m_Theta		= 0.0f; 
	m_fSize		= 1.4f;
	m_fSpeed	= 1.0f;	
	m_vGravity = D3DXVECTOR3( 0.0f , 0.0f, 0.0f);
	m_ColorEnd = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
	m_ColorStart = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f );
	m_OldTime = timeGetTime();
	m_OldUpTime = m_OldDownTime = m_OldTime; 
	
	
	m_bSubtract = true;
	m_bKeepUp = false; 

	m_bDownTime = false;
	m_bUpTime =	false;
	
}

CEFFameItemSet::~CEFFameItemSet()
{

}


void CEFFameItemSet::Init(void)
{
	int ParNum = 0 ; 
/*
자주색 -> 마법방어 1
연두색 -> 물리방어 2
녹색  - > 체력    3
보라 - > 지능 4
남색  - > 지구력 5 
은색 - > 민첩	6 
노랑 -> 교감 7 
*/
	m_OldTime = timeGetTime();

	
	CStringToCode	HashString1("fire_8.tga");
	CStringToCode	HashString2("fire_9.tga");
	CStringToCode	HashString3("fire_10.tga");
	CStringToCode	HashString4("fire_11.tga");
	CStringToCode	HashString5("fire_12.tga");
	CStringToCode	HashString6("fire_13.tga");
	CStringToCode	HashString7("fire_14.tga"); 

	long HashCode  =0  ; 



	// 몸에서 얉게 나오는 이펙트 레벨 3까지 .. 
	// 각엘다스톤 특정 색깔에 맞게 이곳에서 셋팅해준다 . 

	switch(m_EldaType)
	{
		case 1:
			HashCode = HashString1.GetHashCode();

			break;
		case 2:
			HashCode = HashString2.GetHashCode();
			break;

		case 3:

			HashCode = HashString3.GetHashCode();
			break;

		case 4: 
			HashCode = HashString4.GetHashCode();

			break;

		case 5: 
			HashCode = HashString5.GetHashCode();

			break;

		case 6:
			HashCode = HashString6.GetHashCode();

			break;
		case 7:
			HashCode = HashString7.GetHashCode();

			break;


		default:
			SetDestroy();
			return;

	}
	
	

	
	switch(m_Level)
	{		
	case 5:
	case 6:

		m_fSize	=  1.4f;
		m_fSpeed = 0.7f;
		m_Theta = 40.f;
		
		m_ColorStart = D3DXCOLOR( 0.2f , 0.2f , 0.2f , 0.0f );
		m_ColorEnd = D3DXCOLOR( 0.6f , 0.6f , 0.8f , 0.0f );
		m_vGravity = D3DXVECTOR3( 0.0f , 0.4f , 0.0f);
		break;

	case 7:
	case 8:
		
		m_fSize	=  1.6f;
		m_fSpeed = 1.0f;
		m_Theta = 45.0f;
		m_ColorStart = D3DXCOLOR( 0.3f , 0.3f , 0.3f , 0.0f );
		m_ColorEnd = D3DXCOLOR( 0.8f , 0.8f , 0.8f , 0.0f );
		m_vGravity = D3DXVECTOR3( 0.0f , 0.5f , 0.0f);
		
		break;
	case 9:
	case 10:
		
		m_fSize	=  1.7f;
		m_fSpeed = 1.0f;
		m_Theta = 45.0f;
		m_ColorStart =  D3DXCOLOR( 0.5f , 0.5f , 0.5f , 0.0f );
		m_ColorEnd =	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.0f );
		m_vGravity =	D3DXVECTOR3( 0.0f , 0.6f , 0.0f);
			
		break;

	default:
		SetDestroy();
		return; 
			
	}
	
	//m_Theta = 43.0f  ;
	//m_ColorStart = D3DXCOLOR( 0.3f , 0.3f , 0.3f , 0.0f );
	//m_ColorEnd = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.0f );
	
	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 	24175 ) ) == NULL )
		return;

	pData->m_ColorStart = m_ColorStart; 
	pData->m_ColorEnd = m_ColorEnd;
	pData->m_fTheta = m_Theta ; 
	pData->m_HashCode = HashCode;
	pData->m_fSpeed = m_fSpeed ; 
	pData->m_vGravityStart = m_vGravity;
	pData->m_fSizeStart = m_fSize ;
	pData->m_fSizeEnd = m_fSize ; 


	for( int i = 0 ; i < 5 ; ++i)
		m_ParticleAnother[i].MakeLink(pData);



	for(int  i = 0 ; i < 5 ; ++i )
	{
		ENCHANT_POINT TempPoint;
		
		if( i == 0 )
			TempPoint.Range = 0.0f;
		else
		{
			//위에서 팔목까지.. 
			if( i <= 2 )
			{
				TempPoint.Range = 1.0f * ( ( float ) i / ( float ) 2 );
			}
			else 
			{
				TempPoint.Range = 1.0f * ( (( float ) i-2) / ( float ) 4 );
			}

		}

		m_ParticleLink[ i ].MakeLink( pData );
	
		m_RArm.push_back( TempPoint );
		m_LArm.push_back( TempPoint );
		m_Rleg.push_back( TempPoint );
		m_Lleg.push_back( TempPoint );
	}

	/*

	m_ParticleLink[2].m_pReference->m_fSizeStart = 1.2f;
	m_ParticleLink[2].m_pReference->m_fSizeEnd  = 1.2f;
	
	m_ParticleLink[3].m_pReference->m_fSizeStart = 1.1f;
	m_ParticleLink[3].m_pReference->m_fSizeEnd  = 1.1f;

	m_ParticleLink[4].m_pReference->m_fSizeStart = 1.0f;
	m_ParticleLink[4].m_pReference->m_fSizeEnd  = 1.0f;
*/


}

int CEFFameItemSet::Update(float dtime)
{
	return TRUE;
}

float tx = 0.0f;
float tz  = 0.0f;


int	CEFFameItemSet::SortRender(float dtime)
{

	
	int i  = 0 ; 
	


	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
	
	if( g_Particle.m_EffectManager.ISOverLength( m_pSrcChr ) )
	{
		return TRUE;
	}

	if( m_pSrcChr->m_curt_event.type == SM_STATE_DIE)
	{
		SetDestroy();
		return TRUE;
	}


	//m_OldEffectTime1 불이 타오른 시간은 5초이다 .. 5초동안 대기상태 
	// 5초후에 불은 서서히 줄어들며 꺼진다 .. 
	// 완전히 0.0f 가 되면  불은 3초간 정도 안보 였다가 3초후에 다시  
	// 5초이내로 타오른다 .. 
	DWORD test = timeGetTime();
/*

	//이 루틴은 상항 실행된다 .. 0.5초마다 ... 실행됨 

//	if( m_fSpeed == 0.0f ) return TRUE;

	for (  i = 0 ; i < 5 ; ++i )
	{
		m_ParticleLink[ i ].m_pReference->m_fSpeed = m_fSpeed ; 
		m_ParticleLink[ i ].m_pReference->m_fTheta = m_Theta ; 

	}
*/


//	if( !m_pSrcChr->DrawWeaponCheckStatus() )
//		return TRUE;

		
	D3DXMATRIX   matTemp;
	D3DXVECTOR3  v0;
	D3DXVECTOR3  v1;
	D3DXVECTOR3	 v2;


	D3DXMATRIX matPos;
	D3DXVECTOR3 StartVec;
	D3DXVECTOR3	EndVec; 

	
	D3DXMATRIX matRarm[3];

	// 오른쪽 어깨 
	m_pSrcChr->GetBoneLinkTM( 13 , matTemp );
	matRarm[0] = matTemp;
	v0 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	// 오른꿈치  
	m_pSrcChr->GetBoneLinkTM( 15 , matTemp );
	matRarm[1] = matTemp;
	v1 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	// 오른 손 
	m_pSrcChr->GetBoneLinkTM(  5 , matTemp ) ; 
	matRarm[2] = matTemp;
	v2 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );

	D3DXVECTOR3 vDir;
	/*
	for( i =0  ; i < 3 ; ++i )
	{
		m_RArm[i].vPos = D3DXVECTOR3(matRarm[i]._41 , matRarm[i]._42, matRarm[i]._43);
	}
	
	*/

	vDir = m_RArm[0].vPos - m_RArm[1].vPos ; 
	//방향만 
	D3DXVec3Normalize(&vDir, & vDir);
	vDir *= 1.0f ; 
//	m_ParticleAnother[0].m_pReference->m_vGravityStart = vDir;
	

	vDir = m_RArm[1].vPos - m_RArm[2].vPos ; 
	//방향만 
	D3DXVec3Normalize(&vDir, & vDir);
	vDir *= 1.0f ; 
//	m_ParticleAnother[1].m_pReference->m_vGravityStart = vDir;



	//오른쪽 팔 .. 
	for(  i = 0 ; i < m_RArm.size() ; ++i )
	{
		if( i == 0 )
		{
			StartVec = v0;
			EndVec = v1; 
		}
		else if( i == 3 )
		{
			StartVec = v1 ; 
			EndVec = v2 ; 
		}
		D3DXVec3Lerp( &m_RArm[ i ].vPos , &StartVec , &EndVec , m_RArm[ i ].Range );

	}

// 배열로 .. 

//	for(int i =0 ;  i < 3 ; ++i )
///	{
//		D3DXMatrixIdentity( matRarm[i]);
//	}
/*
	D3DXMATRIX matRHand;
	D3DXMATRIX matREblow;
	D3DXMATRIX matRShoulder;
*/

	
	//  왼쪽 어깨 
	m_pSrcChr->GetBoneLinkTM( 12 , matTemp );
	v0 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	// 왼 꿈치  
	m_pSrcChr->GetBoneLinkTM( 14 , matTemp );
	v1 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	matRarm[1] = matTemp;
	// 왼  손 
	m_pSrcChr->GetBoneLinkTM(  4 , matTemp ) ; 
	v2 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	matRarm[2] = matTemp;
	/*
	void GetTMtoLerp(std :: vector<ENCHANT_POINT *> Point )
	{
		

	}
*/
	//왼쪽 팔만 ..한다 .. 
	for(  i = 0 ; i < m_LArm.size() ; ++i )
	{
		if( i == 0 )
		{
			StartVec = v0;
			EndVec = v1; 

		}
		else if( i == 3)
		{
			StartVec = v1 ; 
			EndVec = v2 ; 
		}
		D3DXVec3Lerp( &m_LArm[ i ].vPos , &StartVec , &EndVec , m_LArm[ i ].Range );
	}

	//  왼쪽 골반  
	m_pSrcChr->GetBoneLinkTM( 18 , matTemp );
	v0 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	// 왼 무릎  
	m_pSrcChr->GetBoneLinkTM( 16 , matTemp );
	v1 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	// 왼  발 
	m_pSrcChr->GetBoneLinkTM(  6 , matTemp ) ; 
	v2 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );


	//왼쪽 팔만 ..한다 .. 
	for(  i = 0 ; i < m_Lleg.size() ; ++i )
	{
		if( i == 0 )
		{
			StartVec = v0;
			EndVec = v1; 
		}
		else if( i == 3)
		{
			StartVec = v1 ; 
			EndVec = v2 ; 
		}
		D3DXVec3Lerp( &m_Lleg[ i ].vPos , &StartVec , &EndVec , m_Lleg[ i ].Range );
	}

	//  오른 골반  
	m_pSrcChr->GetBoneLinkTM( 19 , matTemp );
	v0 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	//  오른 무릎 
	m_pSrcChr->GetBoneLinkTM( 17 , matTemp );
	v1 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	// 오른 발 
	m_pSrcChr->GetBoneLinkTM(  7 , matTemp ) ; 
	v2 = D3DXVECTOR3(matTemp._41 , matTemp._42 , matTemp._43 );
	
	//왼쪽 팔만 ..한다 .. 
	for(  i = 0 ; i < m_Rleg.size() ; ++i )
	{
		if( i == 0 )
		{
			StartVec = v0;
			EndVec = v1; 

		}
		else if( i == 3)
		{
			StartVec = v1 ; 
			EndVec = v2 ; 
		}
		D3DXVec3Lerp( &m_Rleg[ i ].vPos , &StartVec , &EndVec , m_Rleg[ i ].Range );
	}



	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetLighting( TRUE );
	
	
	//현재 위치값을 얻고 시간이 지났을때 에 .. 똑같은 위치면  그라비티 방향을 .. 위로 향하게 하며 .. 
	// 캐릭터가 이동중일시에는 그라비티 방향을 캐릭터 중심축으로 부터 뒤축 방향으로 향하게 한다 .. 


//	now_Yrad
//	m_pSrcChr->now_Yrad
/*
	D3DXMATRIX   matRotY;
									
	D3DXVECTOR3 vDir = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_pSrcChr->GetBoneLinkTM( 2  ,  matRotY );

	D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection());
	D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );
	
	vDir = vDir * 1.0f ; 

	for( i = 0 ; i < 5 ; ++i )
	{
		m_ParticleLink[i].m_pReference->m_vGravityStart = D3DXVECTOR3(vDir.x  , 0.0f , vDir.z ) ;
	}
*/

/*
	for( i = 0 ; i < 5 ; ++i )
	{
		m_ParticleAnother[ i ].SetLinkPos( m_RArm[i].vPos );
		m_ParticleAnother[ i ].UpdateLink( dtime );
		m_ParticleAnother[ i ].Render();
	}
*/
	
	
	/*


	switch(nRui->TestNumFame)
	{		
	case 0:
		

		m_fSize	= 2.0f;
		m_fSpeed = 0.2f;
		m_Theta = 180.0f  ;
		
		m_ColorStart = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f );
		m_ColorEnd = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 0.6f );
		m_vGravity = D3DXVECTOR3( 0.0f , 0.0f , 0.0f);

		break; 
	case 1:
		m_fSize	= 2.0f;
		m_fSpeed = 0.3f;
		m_Theta = 180.f;
		
		m_ColorStart = D3DXCOLOR( 0.1f , 0.1f , 0.1f , 0.0f );
		m_ColorEnd = D3DXCOLOR( 0.8f , 0.8f , 0.8f , 0.0f );
		m_vGravity = D3DXVECTOR3( 0.0f , 0.0f , 0.0f);
		break;
	case 2:
		m_fSize	= 2.0f;
		m_fSpeed = 0.4f;
		m_Theta = 360.f;
		m_ColorStart = D3DXCOLOR( 0.2f , 0.2f , 0.2f , 0.0f );
		m_ColorEnd = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.0f );
		m_vGravity = D3DXVECTOR3( 0.0f , 0.0f , 0.0f);
		break;
	case 3:
		m_fSize	= 1.4f;
		m_fSpeed = 1.0f;
		m_Theta = 40.0f;
		m_ColorStart = D3DXCOLOR( 0.1f , 0.1f , 0.1f , 0.0f );
		m_ColorEnd = D3DXCOLOR( 0.7f , 0.7f , 0.7f , 0.0f );
		m_vGravity = D3DXVECTOR3( 0.0f , 0.3f , 0.0f);
		break;
	case 4:
		m_fSize	= 1.4f;
		m_fSpeed = 1.0f;
		m_Theta = 40.0f;
		m_ColorStart = D3DXCOLOR( 0.3f , 0.3f , 0.3f , 0.0f );
		m_ColorEnd = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.0f );
		m_vGravity = D3DXVECTOR3( 0.0f , 0.6f , 0.0f);
		break;
	}


	for ( i = 0 ; i < 5 ; ++i )
	{
	
		m_ParticleLink[ i ].m_pReference->m_ColorStart = m_ColorStart;
		m_ParticleLink[ i ].m_pReference->m_ColorEnd = m_ColorEnd; 
		m_ParticleLink[ i ].m_pReference->m_fTheta = m_Theta ; 
		m_ParticleLink[ i ].m_pReference->m_fSizeStart = m_fSize;
		m_ParticleLink[ i ].m_pReference->m_fSizeEnd = m_fSize;
		m_ParticleLink[ i ].m_pReference->m_fSpeed = m_fSpeed ; 
		m_ParticleLink[ i ].m_pReference->m_vGravityStart = m_vGravity ; 
	
	}
*/



	for( i = 0 ; i < m_RArm.size() ; ++i )
	{
		m_ParticleLink[ i ].SetLinkPos( m_RArm[ i ].vPos );
		m_ParticleLink[ i ].UpdateLink( dtime );
		m_ParticleLink[ i ].Render();


	}

	for( i = 0 ; i < m_LArm.size() ; ++i )
	{
		m_ParticleLink[ i ].SetLinkPos( m_LArm[ i ].vPos );
		m_ParticleLink[ i ].UpdateLink( dtime );
		m_ParticleLink[ i ].Render();

	}

	for( i = 0 ; i < m_Lleg.size() ; ++i )
	{
		m_ParticleLink[ i ].SetLinkPos( m_Lleg[ i ].vPos );
		m_ParticleLink[ i ].UpdateLink( dtime );
		m_ParticleLink[ i ].Render();
	}
	
	for( i = 0 ; i < m_Rleg.size() ; ++i )
	{
		m_ParticleLink[ i ].SetLinkPos( m_Rleg[ i ].vPos );
		m_ParticleLink[ i ].UpdateLink( dtime );
		m_ParticleLink[ i ].Render();
	}



	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable(  FALSE );


	return TRUE;
}


int	CEFFameItemSet::Render(float dtime)
{
	return TRUE;
}
