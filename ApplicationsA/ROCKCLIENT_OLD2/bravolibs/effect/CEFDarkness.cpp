#include "CEFDarkness.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


int CEFDarkness::InitDevice(void)
{
	return TRUE;
}

int	CEFDarkness::DeleteDevice(void)
{
	return TRUE;
}


CEFDarkness::CEFDarkness()
{
	m_IsConEffect = TRUE;
}

CEFDarkness::~CEFDarkness()
{
}




void CEFDarkness::Init(void)
{
	//파이어볼을 한개를 캐릭터 머리위에 돌려본후 
	// 이상이 없으면 3개를 돌린다 .. 
	//캐릭터 머리위에 뜬다 .
	FireData[0].Clear();
	FireData[1].Clear();
	FireData[2].Clear();
	
	FireData[0].m_AddRange = 10;
	FireData[1].m_AddRange = 10;
	FireData[2].m_AddRange = 10 ; 
	
	m_fHeight[0] = 0.2f;
	m_fHeight[1] = 0.5f;
	m_fHeight[2] = 0.8f;

	FireData[0].m_Speed =  UINT(rand()%20)+60; 
	FireData[1].m_Speed = UINT(rand()%40)+70;  
	FireData[2].m_Speed = UINT(rand()%60)+80;  


	m_DamagePercent[0] = 0.3f;
	m_DamagePercent[1] = 0.3f;
	m_DamagePercent[2] = 0.4f;

	FireData[0].m_Theta = UINT(rand()%720);
	FireData[1].m_Theta = UINT(rand()%720);
	FireData[2].m_Theta = UINT(rand()%720);

	//초기각 
	FireData[0].m_Angle = D3DXToRadian(90);
	FireData[1].m_Angle = D3DXToRadian(225);
	FireData[2].m_Angle = D3DXToRadian(315);

	


	m_EffectCnt = 0 ; 
//	m_MaxDamage =m_pExtraInfo->TargetInfo.theEffectValue[0];
	m_OldTime = g_nowTime;
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	CParticleData	*pData;
	
	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24208 ) ) == NULL )
	{
		SetDestroy(TRUE);
		return;
	}
	
	for(int i =0 ; i < 3 ; ++i)
	{
		m_FireParticle[i].MakeLink(pData);
	}

	D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f );
	
	for(int i =0 ; i < 3 ; ++i)
	{
		//동시에 시작한다
		FireData[i].bStartFlag = true; 

		//오른손 왼손 센터 
		// 머리 위쪽에 부터 
		D3DXMATRIX matBone;
		D3DXMATRIX matTrns;
			
		m_pSrcChr->GetBoneLinkTM( 2 , matBone );
		FireData[i].m_vStartPos = D3DXVECTOR3(matBone._41  , matBone._42 , matBone._43 );
		//FireData[i].OldRange = FireData[i].m_vStartPos ; 


/*

		if( i == FIRST )
		{
		}

		//왼쪽 
		else if( i == SECOND )
		{
			m_pSrcChr->GetBoneLinkTM( 4 , matBone );
		//	D3DXVec3TransformCoord( &vDir , &D3DXVECTOR3(10.0f , -10.0f, -30.0f ) , &matBone );
			FireData[i].m_vStartPos = D3DXVECTOR3(matBone._41  , matBone._42 , matBone._43 );

		}
		//오른쪽 
		else if( i == THIRD )
		{
			m_pSrcChr->GetBoneLinkTM( 5 , matBone );
		//	D3DXVec3TransformCoord( &vDir , &D3DXVECTOR3(-10.0f , -10.0f, -30.0f ) , &matBone );
			FireData[i].m_vStartPos = D3DXVECTOR3(matBone._41  , matBone._42 , matBone._43 );

		}
*/
		
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
						      matBone._43 );

		//현재 바라보는 각 ? 
		D3DXMatrixRotationY( &FireData[i].matY , m_pSrcChr->m_radian );

		//FireData[i].matLocal._41 = FireData[i].m_vStartPos.x ; 
		//FireData[i].matLocal._42 = FireData[i].m_vStartPos.y ; 
		//FireData[i].matLocal._43 = FireData[i].m_vStartPos.z ; 
		FireData[i].m_RangeTime  = PositionToTimer( FireData[i].m_Speed,  FireData[i].m_vStartPos , vDestPos );
		FireData[i].m_OldTime = g_nowTime;
			
	}


	FireData[0].bStartFlag = true;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFDarkness::Render(float dtime)
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
		//도착하면 그려주지않고 끈다 
		if( FireData[i].bStartFlag)
		{
			m_FireParticle[i].Render();
		}
	}

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFDarkness::CheckFrame(void)
{
	return TRUE;
}




int CEFDarkness::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	int i =0 ;
		
	//볼 3구에 대한처리 .. 
	for(i = 0 ; i < 3 ; ++i)
	{
		
		DarkBall(i , FireData[i].m_Theta , dtime );
	}
	//스펠이 끝나면..

	/*
	for(i =0 ; i < 3 ; ++i)
	{
		
	}
	*/

	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFDarkness::SetAfterEffect(void)
{
/*
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
*/

	
	// 데미지 효과
	
	//................................................................................	
	// 어택 사운드
	//................................................................................	
/*
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	D3DXVECTOR3 vec( m_pDestChr->GetPosTM()._41 ,
						  m_pDestChr->GetPosTM()._42 + (m_pDestChr->GetBoundMaxPos().y  * m_fHeight[i]) ,
						  m_pDestChr->GetPosTM()._43 );

*/


//	g_Particle.SetEffSetCon( 0.0f ,EF_DARKNESSED01, 0 , 2.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex() , );


	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_DARKNESSED01;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 1.1f;
	}

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );


	return TRUE;
}

void CEFDarkness::DarkBall(int Num , int Theta , float dtime)
{
	if( !FireData[Num].bStartFlag)
		return;
	

	
	D3DXVECTOR3 vDirX( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vDirY( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vDirZ( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vDir(0.0f, 0.0f, 0.0f );

	D3DXVECTOR3 vPos;
	D3DXMATRIX  matRotY;
	D3DXMATRIX	matRotX;
	D3DXMATRIX	matRotZ;
	D3DXMATRIX	matMulti;


	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - FireData[Num].m_vStartPos;
	
	float       Range     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , FireData[Num].m_OldTime ) ) / ( float ) FireData[Num].m_RangeTime ) );
	float		RangeRadius = D3DXToRadian( 360 ) * Range ; 
	
	D3DXMATRIX matRot;
	D3DXMATRIX matMul;

	if(Range > 1.0f )
		Range = 1.0f;
	
/*
	
	D3DXMatrixRotationZ( &matRotZ , RangeRadius + FireData[Num].m_Angle );
	D3DXMatrixMultiply( &matMul ,  &matRotZ, &FireData[Num].matY );
	D3DXVec3TransformCoord( &vDir , &D3DXVECTOR3(8.0f-MaxRange, 0.0f, 0.0f ) , &matMul );
		
	vPos      = FireData[Num].m_vStartPos + ( vDistance * Range );  
	
	FireData[i].matLocal._41 =	vPos.x + vDir.x ;
	FireData[i].matLocal._42 =	vPos.y + vDir.y ;
	FireData[i].matLocal._43 =	vPos.z + vDir.z ;
	
	vPos.x  = FireData[i].matLocal._41;
	vPos.y  = FireData[i].matLocal._42;
	vPos.z  = FireData[i].matLocal._43;
*/	

	//보간시킨다..
	if(FireData[Num].OldRange != Range )
	{
					
		//예전과 현재를 차이를 구한다 .
		float RangeTemp =  Range - FireData[Num].OldRange;
		float OldRadius  = 0 ;
		float MaxRange= 0 ; 
		
		//RangeTemp 는 소수점으로 나오고 ex)0.1f나왔을경우 0.1/0.02 로 나누어서 5개를 더구한다. 
		// 
		D3DXVECTOR3 vPos2 = D3DXVECTOR3(0.0f, 0.0f , 0.0f );
		D3DXVECTOR3 vCurPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f );
			
		//보간을 한다 
		for(float fi = 0.0f ; fi < RangeTemp; fi+=0.02f)
		{
			MaxRange= (FireData[Num].OldRange + fi)*4.0f;
			OldRadius = D3DXToRadian( Theta ) * (FireData[Num].OldRange + fi);
			D3DXMatrixRotationZ( &matRotZ , OldRadius + FireData[Num].m_Angle );
			D3DXMatrixMultiply( &matMul ,  &matRotZ, &FireData[Num].matY );
			D3DXVec3TransformCoord( &vDir , &D3DXVECTOR3(8.0f-MaxRange, 0.0f, 0.0f ) , &matMul );
			
			vPos2      = FireData[Num].m_vStartPos + ( vDistance *  (FireData[Num].OldRange + fi)  );  
						

			vCurPos.x = vPos2.x  + vDir.x ;
			vCurPos.y = vPos2.y  + vDir.y ;
			vCurPos.z = vPos2.z  + vDir.z ;
			
			g_Particle.DisplayUpdate( 24207 , vCurPos );
				
		}
		
		m_FireParticle[Num].SetLinkPos( vCurPos );
		m_FireParticle[Num].UpdateLink( dtime );

		FireData[Num].OldRange  = Range;
	}
	
	if( Range >= 1.0f ) 
	{
		
		FireData[Num].bStartFlag = false;

		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

		D3DXVECTOR3 vec( m_pDestChr->GetPosTM()._41 ,
			m_pDestChr->GetPosTM()._42 + (m_pDestChr->GetBoundMaxPos().y  * m_fHeight[Num]) ,
			m_pDestChr->GetPosTM()._43 );

		g_Particle.SetEffSetCon( 0.0f , EF_DARKNESSED01 , ESLT_POS , 2.1f , -1 , -1 , vec , 0.0f, -1 , 0  );

		if(Num == 2)
		{
			SetDestroy();
			SetAfterEffect();
		}
				
	}
	
}




/*
	
			//	D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
			//	D3DXMatrixRotationX( &matRotX , D3DXToRadian( -FireData[i].m_Angle ) );
				
			//	D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
			//	D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotY );
							
				D3DXVECTOR3 vDestPos( m_pDestChr->GetPosTM()._41 ,
					m_pDestChr->GetPosTM()._42 + m_pDestChr->GetBoundMaxPos().y * m_fHeight[i] ,
					m_pDestChr->GetPosTM()._43 );
				

				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;		
	
			
				D3DXMATRIX matRot;
				D3DXMatrixRotationX( &matRot , D3DXToRadian( 90.0f ) - RangeRadius );
				D3DXMatrixMultiply( &FireData[i].matLocal , &matRot , &FireData[i].matY);

				if( FireData[2].bStartFlag == false  )
				{
					FireData[2].bStartFlag = true;
					FireData[2].m_OldTime = g_nowTime;
				}
				
				if( Range > 1.0f ) 
				{
					Range = 1.0f;
				}
				D3DXVECTOR3 vPos      = FireData[i].m_vStartPos + ( vDistance * Range );  
			
//				//회전 
//				FireData[i].matLocal._41 +=	vPos.x + vDirX.x ;
//				FireData[i].matLocal._42 =	vPos.y + vDirX.y   ;
//				FireData[i].matLocal._43 +=	vPos.z + vDirX.z ;
//				

		//		vPos.x = 	FireData[i].matLocal._41;
		//		vPos.y = 	FireData[i].matLocal._42;
		//		vPos.z =	FireData[i].matLocal._43;
				
				
				float Radius = D3DXToRadian( 90 ) * Range ; 
				vPos.y = vPos.y + cosf( Radius  ) * 10.0f;
				vPos.x 	= vPos.x + sinf( RangeRadius ) * 30.0f;
				
				FireData[i].matLocal._41 = vPos.x;
				FireData[i].matLocal._42 = vPos.y;
				FireData[i].matLocal._43 = vPos.z;
		
				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );





  */
  //////////////////////////////////////////////////

/*
else if(i==1 && FireData[i].bStartFlag)
			{
							
			//	D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
			//	D3DXMatrixRotationX( &matRotX , D3DXToRadian( -FireData[i].m_Angle ) );
			
			//	D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
			//	D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotY );
	
				D3DXVECTOR3 vDestPos( m_pDestChr->GetPosTM()._41 ,
					m_pDestChr->GetPosTM()._42 + m_pDestChr->GetBoundMaxPos().y * m_fHeight[i] ,  
					m_pDestChr->GetPosTM()._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;
				
				D3DXMATRIX matRot;
				D3DXMATRIX matMul;
			
			//보간시킨다..
				if(FireData[i].OldRange != Range )
				{

					//Prev Range and Next Range if Update
					// 0.0f ~ 1.0f 0.???? appear 0.02로 계산한다 . 
					// 
					//예전과 현재를 차이를 구한다 .
					float RangeTemp =  Range - FireData[i].OldRange;
					float OldRadius  = 0 ;
					
					//RangeTemp 는 소수점으로 나오고 ex)0.1f나왔을경우 0.1/0.02 로 나누어서 5개를 더구한다. 
					// 
					D3DXVECTOR3 vPos2 = D3DXVECTOR3(0.0f, 0.0f , 0.0f );
					D3DXVECTOR3 vCurPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f );

					//보간을 한다 
					for(float fi = 0.0f ; fi < RangeTemp; fi+=0.02f)
					{
					
						OldRadius = D3DXToRadian( 360 ) * (FireData[i].OldRange + fi);
						D3DXMatrixRotationZ( &matRotZ , OldRadius + FireData[i].m_Angle );
						D3DXMatrixMultiply( &matMul ,  &matRotZ, &FireData[i].matY );
						D3DXVec3TransformCoord( &vDir , &D3DXVECTOR3(5.0f, 5.0f, 0.0f ) , &matMul );
						
						vPos2      = FireData[i].m_vStartPos + ( vDistance *  (FireData[i].OldRange + fi)  );  
	
						vCurPos.x = vPos2.x  + vDir.x ;
						vCurPos.y = vPos2.y  + vDir.y ;
						vCurPos.z = vPos2.z  + vDir.z ;
										
						g_Particle.DisplayUpdate( 24210 , vCurPos );
						//g_Particle.DisplayUpdate( 24208 , vCurPos );
						//g_Particle.DisplayUpdate( 24207 , vCurPos );
					
					}
					FireData[i].OldRange  = Range;
				}
				
			//	float RangeDegree =  D3DXToDegree(RangeRadius);

			
				if( FireData[2].bStartFlag == false )
				{
					FireData[2].bStartFlag = true;
					FireData[2].m_OldTime = g_nowTime;
					
				}
							
				if( Range > 1.0f ) 
				{
				//	SetDestroy();
					SetAfterEffect();
					FireData[i].bStartFlag = false;
					Range = 1.0f;
				}
				D3DXVECTOR3 vPos      = FireData[i].m_vStartPos + ( vDistance * Range );  
				
			
			//	vPos.y = vPos.y + sinf( RangeRadius  ) * 20.0f;
			/*	
				FireData[i].matLocal._41 =	vPos.x + vDir.x ;
				FireData[i].matLocal._42 =	vPos.y + vDir.y ;
				FireData[i].matLocal._43 =	vPos.z + vDir.z ;

				vPos.x  = FireData[i].matLocal._41;
				vPos.y  = FireData[i].matLocal._42;
				vPos.z  = FireData[i].matLocal._43;

				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
			//	m_FireParticle[i].Render();


					

			*/

			

/*

  
		
			else if(i==2 && FireData[i].bStartFlag)
			{
							
			//	D3DXMatrixRotationY( &matRotY , D3DXToRadian( FireData[i].m_Angle ) );
			//	D3DXMatrixRotationX( &matRotX , D3DXToRadian( -FireData[i].m_Angle ) );
			
			//	D3DXVec3TransformCoord( &vDirX , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotX );
			//	D3DXVec3TransformCoord( &vDirY , &D3DXVECTOR3(0.0f , 5.0f, 0.0f ) , &matRotY );
	
				D3DXVECTOR3 vDestPos( m_pDestChr->GetPosTM()._41 ,
					m_pDestChr->GetPosTM()._42 + m_pDestChr->GetBoundMaxPos().y * m_fHeight[i] ,  
					m_pDestChr->GetPosTM()._43 );
				
				D3DXVECTOR3 vDistance = vDestPos - FireData[i].m_vStartPos;	
			
				D3DXMATRIX matRot;
				D3DXMATRIX matMul;
			

			//보간시킨다..
				if(FireData[i].OldRange != Range )
				{

					//Prev Range and Next Range if Update
					// 0.0f ~ 1.0f 0.???? appear 0.02로 계산한다 . 
					// 
					//예전과 현재를 차이를 구한다 .
					float RangeTemp =  Range - FireData[i].OldRange;
					float OldRadius  = 0 ;
					
					//RangeTemp 는 소수점으로 나오고 ex)0.1f나왔을경우 0.1/0.02 로 나누어서 5개를 더구한다. 
					// 
					D3DXVECTOR3 vPos2 = D3DXVECTOR3(0.0f, 0.0f , 0.0f );
					D3DXVECTOR3 vCurPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f );

					//보간을 한다 
					for(float fi = 0.0f ; fi < RangeTemp; fi+=0.02f)
					{
					
						OldRadius = D3DXToRadian( 360 ) * (FireData[i].OldRange + fi);
						D3DXMatrixRotationZ( &matRotZ , OldRadius + FireData[i].m_Angle );
						D3DXMatrixMultiply( &matMul ,  &matRotZ, &FireData[i].matY );
						D3DXVec3TransformCoord( &vDir , &D3DXVECTOR3(5.0f, 5.0f, 0.0f ) , &matMul );
						
						vPos2      = FireData[i].m_vStartPos + ( vDistance *  (FireData[i].OldRange + fi)  );  
	
						vCurPos.x = vPos2.x  + vDir.x ;
						vCurPos.y = vPos2.y  + vDir.y ;
						vCurPos.z = vPos2.z  + vDir.z ;
										
						g_Particle.DisplayUpdate( 24210 , vCurPos );
						//g_Particle.DisplayUpdate( 24208 , vCurPos );
						//g_Particle.DisplayUpdate( 24207 , vCurPos );
					
					}
					FireData[i].OldRange  = Range;
				}
				
			//	float RangeDegree =  D3DXToDegree(RangeRadius);
				
				if( Range > 1.0f ) 
				{
					SetDestroy();
					SetAfterEffect();
					FireData[i].bStartFlag = false;
					Range = 1.0f;
				}
			//	D3DXVECTOR3 vPos      = FireData[i].m_vStartPos + ( vDistance * Range );  
				
			
			//	vPos.y = vPos.y + sinf( RangeRadius  ) * 20.0f;
			/*	
				FireData[i].matLocal._41 =	vPos.x + vDir.x ;
				FireData[i].matLocal._42 =	vPos.y + vDir.y ;
				FireData[i].matLocal._43 =	vPos.z + vDir.z ;

				vPos.x  = FireData[i].matLocal._41;
				vPos.y  = FireData[i].matLocal._42;
				vPos.z  = FireData[i].matLocal._43;

				m_FireParticle[i].SetLinkPos( vPos );
				m_FireParticle[i].UpdateLink( dtime );
			//	m_FireParticle[i].Render();


			



  */