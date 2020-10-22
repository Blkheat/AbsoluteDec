#include "CEFTripleFire.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


int CEFTripleFire::InitDevice(void)
{
	return TRUE;
}

int	CEFTripleFire::DeleteDevice(void)
{
	return TRUE;
}


CEFTripleFire::CEFTripleFire()
{
	m_IsConEffect = TRUE;
}

CEFTripleFire::~CEFTripleFire()
{
}


//스펠이 다되면 고고 
void CEFTripleFire::InitFireBall(void)
{


}


void CEFTripleFire::Init(void)
{
	//파이어볼을 한개를 캐릭터 머리위에 돌려본후 
	// 이상이 없으면 3개를 돌린다 .. 
	//캐릭터 머리위에 뜬다 .
	

	// 초기 값
	for( int i = 0 ; i < 3 ; i++ )
	{
		FireData[i].m_Angle = ( rand()%360 );
		FireData[i].m_OldTime = g_nowTime;
		
	}

	FireData[0].m_AddRange = 20;
	FireData[1].m_AddRange = 20;
	FireData[2].m_AddRange = 20 ; 
	
	m_fHeight[0] = 0.3f;
	m_fHeight[1] = 0.6f;
	m_fHeight[2] = 0.9f;

	FireData[0].m_Speed = 70; 
	FireData[1].m_Speed = 90; 
	FireData[2].m_Speed = 110; 

	m_DamagePercent[0] = 0.3f;
	m_DamagePercent[1] = 0.3f;
	m_DamagePercent[2] = 0.4f;

	m_EffectCnt = 0 ; 

//	m_MaxDamage =m_pExtraInfo->TargetInfo.theEffectValue[0];



	m_OldTime = g_nowTime;

	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	CParticleData	*pData;

	

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( PT_FIREBALL ) ) == NULL )
	{
		SetDestroy(TRUE);
		return;
	}

	for(int i =0 ; i < 3 ; ++i)
	{
		
		m_FireParticle[i].m_pReference	    = pData;
		
		m_FireParticle[i].m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		m_FireParticle[i].m_bIsSuppressed	= false;
		m_FireParticle[i].m_bIsCreate		= true;
		m_FireParticle[i].m_lUnique		= -1;
		m_FireParticle[i].m_iIndex			= -1;
		m_FireParticle[i].m_fSize			= 1.0f;
		D3DXMatrixIdentity( &m_FireParticle[i].m_LinkMat );
	}

	

	for(short i =0 ; i < 3 ; ++i)
	{

		FireData[i].m_vStartPos = D3DXVECTOR3(m_pSrcChr->GetRealLHandTM()._41 , m_pSrcChr->GetRealLHandTM()._42 , m_pSrcChr->GetRealLHandTM()._43 );
		
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 1 , matBone );
		D3DXVECTOR3 vDestPos( matBone._41 ,
							  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
							  matBone._43 );
			
		FireData[i].m_RangeTime  = PositionToTimer( FireData[i].m_Speed,  FireData[i].m_vStartPos , vDestPos );
	}


	FireData[0].bStartFlag = true;



/*	
	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
								 m_pSrcChr->GetRealRHandTM()._42 ,
								 m_pSrcChr->GetRealRHandTM()._43  );
	
	D3DXVECTOR3 vDestPos( m_pDestChr->GetPosTM()._41 ,
						  m_pDestChr->GetPosTM()._42 + m_pDestChr->GetBoundMaxPos().y * 0.5f ,
						  m_pDestChr->GetPosTM()._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );
*/
	

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFTripleFire::Render(float dtime)
{
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	// Z정렬은 하돼 쓰기는 끈다 .. 끄지않으면 파티클이 짤려나온다 .. 
	// 오브젝트는 무조견 켜라 ...
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );

	for(int i =0 ; i < 3 ; ++i)
	{
		m_FireParticle[i].Render();
	}

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFTripleFire::CheckFrame(void)
{
	return TRUE;
}


void CEFTripleFire::TripleFire(float dtime)
{

	D3DXVECTOR3 vDirX( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vDirY( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vPos;
	D3DXMATRIX  matRotY;
	D3DXMATRIX	matRotX;
	D3DXMATRIX	matRotZ;
	D3DXMATRIX	matMulti;
	int i =0 ; 

	//전체 프레임 제어 
	{
	//	m_OldTime = g_nowTime;
		
		//볼 3구에 대한처리 .. 
		for(i = 0 ; i < 3 ; ++i)
		{
			FireData[i].m_Angle += FireData[i].m_AddRange;
			
			if( FireData[i].m_Angle >= 360 )
				FireData[i].m_Angle -= 360;
			
			if( FireData[i].m_Angle <= -360 )
				FireData[i].m_Angle += 360;

			D3DXMatrixIdentity( &FireData[i].m_matWorld );
			D3DXMatrixIdentity( &matRotX   );
			D3DXMatrixIdentity( &matRotY   );

		
			//|
			if(i==0 && FireData[i].bStartFlag)
			{
					
			//	D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
				D3DXMatrixRotationX( &matRotX , D3DXToRadian( -FireData[i].m_Angle ) );
				
				D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
			//	D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotY );
				
				D3DXMATRIX matBone;
				m_pDestChr->GetBoneLinkTM( 1 , matBone );
				D3DXVECTOR3 vDestPos( matBone._41 ,
									  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
								      matBone._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;
				
				float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , FireData[i].m_OldTime) / ( float ) FireData[i].m_RangeTime ) );
				
				if( FireData[1].bStartFlag == false && (Range > 0.5f) )
				{
					FireData[1].bStartFlag = true;
					FireData[1].m_OldTime = g_nowTime;
				}
				
				if( Range > 1.0f ) 
					Range = 1.0f;
				
				D3DXVECTOR3 vPosTemp      = FireData[i].m_vStartPos + ( vDistance * Range );  
				
				//회전 
			//	FireData[i].m_matWorld._41 +=	vPosTemp.x + vDirX.x ;
			//	FireData[i].m_matWorld._42 =	vPosTemp.y + vDirX.y   ;
			//	FireData[i].m_matWorld._43 +=	vPosTemp.z + vDirX.z ;
				
				FireData[i].m_matWorld._41 +=	vPosTemp.x ;
				FireData[i].m_matWorld._42 =	vPosTemp.y ;
				FireData[i].m_matWorld._43 +=	vPosTemp.z ;
				
				vPos.x = 	FireData[i].m_matWorld._41;
				vPos.y = 	FireData[i].m_matWorld._42;
				vPos.z =	FireData[i].m_matWorld._43;
				
				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );

			}

			// 오른쪽대각선 
			else if(i==1 && FireData[1].bStartFlag )
			{

				D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
				D3DXMatrixRotationX( &matRotX , D3DXToRadian( FireData[i].m_Angle ) );
				
				D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
				D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(5.0f , 0.0f, 0.0f ) , &matRotY );
			
				
				D3DXMATRIX matBone;
				m_pDestChr->GetBoneLinkTM( 1 , matBone );
				D3DXVECTOR3 vDestPos( matBone._41 ,
									  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
									  matBone._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[1].m_vStartPos;
				
				float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , FireData[1].m_OldTime ) / ( float ) FireData[1].m_RangeTime ) );
				
				if( FireData[2].bStartFlag == false && (Range > 0.5f) )
				{
					FireData[2].bStartFlag = true;
					FireData[2].m_OldTime = g_nowTime;
				}
				

				if( Range > 1.0f ) 
					Range = 1.0f;
				
				D3DXVECTOR3 vPosTemp      = FireData[1].m_vStartPos + ( vDistance * Range );  
				
			// 회전 
/*	
				FireData[i].m_matWorld._41	+=	m_pSrcChr->GetBonCenterTM()._41 + vDirX.x + vDirY.x;
				FireData[i].m_matWorld._42  =	(m_pSrcChr->GetBonCenterTM()._42 + m_pSrcChr->GetBoundMaxPos().y)  + vDirX.y + vDirY.y  ;
				FireData[i].m_matWorld._43  +=	m_pSrcChr->GetBonCenterTM()._43 + vDirX.z - vDirY.z;
*/
				FireData[i].m_matWorld._41 +=	vPosTemp.x  ;
				FireData[i].m_matWorld._42 =	vPosTemp.y    ;
				FireData[i].m_matWorld._43 +=	vPosTemp.z ;
				

				vPos.x = 	FireData[i].m_matWorld._41;
				vPos.y = 	FireData[i].m_matWorld._42;
				vPos.z =	FireData[i].m_matWorld._43;

				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );

			}
			//왼쪽 대각선
			else if( i==2 && FireData[i].bStartFlag )
			{
				D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
				D3DXMatrixRotationX( &matRotX , D3DXToRadian( FireData[i].m_Angle ) );
				
				D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
				D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(5.0f , 0.0f, 0.0f ) , &matRotY );
						
						
				D3DXMATRIX matBone;
				m_pDestChr->GetBoneLinkTM( 1 , matBone );
				D3DXVECTOR3 vDestPos( matBone._41 ,
									  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
									  matBone._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;
				
				float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , FireData[i].m_OldTime ) / ( float ) FireData[i].m_RangeTime ) );
				
				if( Range > 1.0f ) 
					Range = 1.0f;
				
				D3DXVECTOR3 vPosTemp      = FireData[i].m_vStartPos + ( vDistance * Range );  
			
				
				//회전 
				//FireData[i].m_matWorld._41 +=	vPosTemp.x + vDirX.x - vDirY.x;
				//FireData[i].m_matWorld._42 =	vPosTemp.y + vDirX.y + vDirY.y  ;
				//FireData[i].m_matWorld._43 +=	vPosTemp.z + vDirX.z - vDirY.z;
				
				
				FireData[i].m_matWorld._41 +=	vPosTemp.x  ;
				FireData[i].m_matWorld._42 =	vPosTemp.y    ;
				FireData[i].m_matWorld._43 +=	vPosTemp.z ;
			

				vPos.x = 	FireData[i].m_matWorld._41;
				vPos.y = 	FireData[i].m_matWorld._42;
				vPos.z =	FireData[i].m_matWorld._43;

				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );

			}
		
		}
	}


}
void CEFTripleFire::TestTripleFire(float dtime)
{

	D3DXVECTOR3 vDirX( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vDirY( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vDirZ( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vPos;
	D3DXMATRIX  matRotY;
	D3DXMATRIX	matRotX;
	D3DXMATRIX	matRotZ;
	D3DXMATRIX	matMulti;
	int i =0 ; 

	//전체 프레임 제어 
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , >= , 50 ) )
	{
		m_OldTime = g_nowTime;
		
		//볼 3구에 대한처리 .. 
		for(i = 0 ; i < 3 ; ++i)
		{
			FireData[i].m_Angle += FireData[i].m_AddRange;
			
			// 360 도 즉 한바퀴까 다돌면 0도로 돌려놓는다 . 
			if( FireData[i].m_Angle >= 360 )FireData[i].m_Angle -= 360;
			// -360 도 즉 한바퀴까 다돌면 0 도로 돌려놓는다 . 
			if( FireData[i].m_Angle <= -360 )FireData[i].m_Angle += 360;

			D3DXMatrixIdentity( &FireData[i].m_matWorld );
			D3DXMatrixIdentity( &matRotX   );
			D3DXMatrixIdentity( &matRotY   );
			D3DXMatrixIdentity( &matRotZ   );
			
			//|
			if(i==0 )
			{
				//D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
				//D3DXMatrixRotationX( &matRotX , D3DXToRadian( -FireData[i].m_Angle ) );
				D3DXMatrixRotationZ( &matRotZ , D3DXToRadian( 90 ) );
				D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(5.0f, 0.0f, 0.0f ) , &matRotZ );
							
				//D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
				//D3DXVec3TransformCoord( &vDirZ , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotZ );
				
			//	D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotY );
				
				D3DXMATRIX matBone;
				m_pDestChr->GetBoneLinkTM( 1 , matBone );
				D3DXVECTOR3 vDestPos( matBone._41 ,
									  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
									  matBone._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;
				
				float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , FireData[i].m_OldTime) / ( float ) FireData[i].m_RangeTime ) );
				
				if( Range > 1.0f ) 
					Range = 1.0f;
		
				FireData[i].m_matWorld._41	+=	m_pSrcChr->GetBonCenterTM()._41 + vDirX.x ;
				FireData[i].m_matWorld._42  =	(m_pSrcChr->GetBonCenterTM()._42 + m_pSrcChr->GetBoundMaxPos().y+10 )  + vDirX.y  ;
				FireData[i].m_matWorld._43  +=	m_pSrcChr->GetBonCenterTM()._43 + vDirX.z ;
				
				vPos.x = 	FireData[i].m_matWorld._41;
				vPos.y = 	FireData[i].m_matWorld._42;
				vPos.z =	FireData[i].m_matWorld._43;
				
				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );

			}
			// 오른쪽대각선 
			else if(i==1  )
			{

				D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
				D3DXMatrixRotationX( &matRotX , D3DXToRadian( FireData[i].m_Angle ) );
				
				D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
				D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(5.0f , 0.0f, 0.0f ) , &matRotY );
			
				
				D3DXMATRIX matBone;
				m_pDestChr->GetBoneLinkTM( 1 , matBone );
				D3DXVECTOR3 vDestPos( matBone._41 ,
									  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
									  matBone._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;
				
				float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , FireData[i].m_OldTime ) / ( float ) FireData[i].m_RangeTime ) );
			

				if( Range > 1.0f ) 
					Range = 1.0f;
				
							
				FireData[i].m_matWorld._41	+=	m_pSrcChr->GetBonCenterTM()._41 + vDirX.x + vDirY.x;
				FireData[i].m_matWorld._42  =	(m_pSrcChr->GetBonCenterTM()._42 + m_pSrcChr->GetBoundMaxPos().y+10)  + vDirX.y + vDirY.y  ;
				FireData[i].m_matWorld._43  +=	m_pSrcChr->GetBonCenterTM()._43 + vDirX.z - vDirY.z;

				vPos.x = 	FireData[i].m_matWorld._41;
				vPos.y = 	FireData[i].m_matWorld._42;
				vPos.z =	FireData[i].m_matWorld._43;

				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );

			}
			//왼쪽 대각선
			else if( i==2  )
			{
				D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
				D3DXMatrixRotationX( &matRotX , D3DXToRadian( FireData[i].m_Angle ) );
				
				D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
				D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(5.0f , 0.0f, 0.0f ) , &matRotY );
						
						
				D3DXMATRIX matBone;
				m_pDestChr->GetBoneLinkTM( 1 , matBone );
				D3DXVECTOR3 vDestPos( matBone._41 ,
									  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
									  matBone._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;
				
				float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , FireData[i].m_OldTime ) / ( float ) FireData[i].m_RangeTime ) );
				
				if( Range > 1.0f ) 
					Range = 1.0f;
				
													
				FireData[i].m_matWorld._41	+=	m_pSrcChr->GetBonCenterTM()._41 + vDirX.x - vDirY.x;
				FireData[i].m_matWorld._42  =	(m_pSrcChr->GetBonCenterTM()._42 + m_pSrcChr->GetBoundMaxPos().y+10)  + vDirX.y + vDirY.y  ;
				FireData[i].m_matWorld._43  +=	m_pSrcChr->GetBonCenterTM()._43 + vDirX.z - vDirY.z;

				vPos.x = 	FireData[i].m_matWorld._41;
				vPos.y = 	FireData[i].m_matWorld._42;
				vPos.z =	FireData[i].m_matWorld._43;

				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );

			}
			
		}
	}





}

int CEFTripleFire::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	int i =0 ;
	
	//스펠중에 3구를 회전시킨다.
	TripleFire(dtime);
	
//	TestTripleFire(dtime);

	//스펠이 끝나면..

	//............................................................................................................
	// 파이어볼 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................

	for(i =0 ; i < 3 ; ++i)
	{
		if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( FireData[i].m_RangeTime , FireData[i].m_OldTime ) ) 	&& FireData[i].bStartFlag )
		{
			if(i == 2 )
			{
				SetDestroy();
			}
			SetAfterEffect();
			FireData[i].bStartFlag = false ;
		}
	}
	


	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFTripleFire::SetAfterEffect(void)
{
	int EffectID = 0;
	
	switch( m_ID )
	{
		case EF_FIREBALL0:
			EffectID = EF_FIREBALL_ATTACKED0;
			break;
		case EF_FIREBALL1:
			EffectID = EF_FIREBALL_ATTACKED1;
			break;
		case EF_FIREBALL2:
			EffectID = EF_FIREBALL_ATTACKED2;
			break;
		case EF_FIREBALL3:
			EffectID = EF_FIREBALL_ATTACKED3;
			break;
		case EF_FIREBALL4:
			EffectID = EF_FIREBALL_ATTACKED4;
			break;
	}

	// 데미지 효과
	
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_FIREBALL_ATTACKED2 , 0 , 2.1f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_FIREBALL_ATTACKED2;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 1.1f;
	}

	
//	m_pExtraInfo->TargetInfo.theEffectValue[0] = ((float)m_MaxDamage*m_DamagePercent[m_EffectCnt]);
/*

	// 데미지 숫자 찍기
	D3DXVECTOR3 vHeight = D3DXVECTOR3( 
		m_pDestChr->GetPosTM()._41 ,
		m_pDestChr->GetPosTM()._42 + ( m_pDestChr->GetBoundMaxPos().y * 1.2f ) ,
		m_pDestChr->GetPosTM()._43 ); 
	
	// 데미지 찍을때 플레이어랑 연관있는것만 찍기
	if( ( m_pSrcChr == g_Pc.GetPlayer() || m_pDestChr == g_Pc.GetPlayer() ) && m_pExtraInfo->TargetInfo.theEffectValue[0] > 0 )
		g_Particle.m_DamageNum.Add( vHeight  , m_pExtraInfo->TargetInfo.theEffectValue[0], 0 );

*/

	if(m_EffectCnt == 2)
	{
		if( m_pSrcChr == g_Pc.GetPlayer() )
		{
			g_Particle.RockCamera( ( rand() % 200 ) + 300 );
		}
		
		g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );	
	}	
	
	m_EffectCnt++;

	return TRUE;
}
