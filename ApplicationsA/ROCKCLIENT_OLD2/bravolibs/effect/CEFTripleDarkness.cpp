#include "CEFTripleDarkness.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

const int c_Max_Ball_Num = 3; 


int CEFTripleDarkness::InitDevice(void)
{
	return TRUE;
}

int	CEFTripleDarkness::DeleteDevice(void)
{
	return TRUE;
}


CEFTripleDarkness::CEFTripleDarkness()
{
	m_IsConEffect = TRUE;
}

CEFTripleDarkness::~CEFTripleDarkness()
{
}


void CEFTripleDarkness::Init(void)
{
	//파이어볼을 한개를 캐릭터 머리위에 돌려본후 
	// 이상이 없으면 3개를 돌린다 .. 
	//캐릭터 머리위에 뜬다 .
	
	for(int i =0 ; i < 3 ; ++i)
	{
		DarkData[i].Ball[0].Clear();
		DarkData[i].Ball[1].Clear();
		DarkData[i].Ball[2].Clear();

		DarkData[i].Ball[0].m_AddRange = 10;
		DarkData[i].Ball[1].m_AddRange = 10;	
		DarkData[i].Ball[2].m_AddRange = 10;

		DarkData[i].Ball[0].m_Speed = 50;//UINT(rand()%10)+60;  
		DarkData[i].Ball[1].m_Speed = 60;//UINT(rand()%20)+70;  
		DarkData[i].Ball[2].m_Speed = 70;//UINT(rand()%30)+80;  
		
		DarkData[i].Ball[0].m_Theta = Random(100 , 520) ; // UINT(rand()%520);
		DarkData[i].Ball[1].m_Theta = Random(300 , 720);//UINT(rand()%720);
		DarkData[i].Ball[2].m_Theta = Random(100 , 520);//UINT(rand()%360);
		
		DarkData[i].Ball[0].m_Angle = D3DXToRadian(90);
		DarkData[i].Ball[1].m_Angle = D3DXToRadian(225);
		DarkData[i].Ball[2].m_Angle = D3DXToRadian(315);

	}


	m_fHeight[0] = Random(0.0f,1.0f);
	m_fHeight[1] = Random(0.0f,1.0f);
	m_fHeight[2] = Random(0.0f,1.0f);

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
	
	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24208 ) ) == NULL )
			return;
		
	for(int i =0 ; i < c_Max_Ball_Num ; ++i)
	{
		DarkData[i].m_DarkParticle[0].MakeLink(pData);
		DarkData[i].m_DarkParticle[1].MakeLink(pData);
		DarkData[i].m_DarkParticle[2].MakeLink(pData);
	}

	D3DXVECTOR3 vDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f );
	
	for(int i =0 ; i < c_Max_Ball_Num ; ++i)
	{
	
		//오른손 왼손 센터 
		// 머리 위쪽에 부터 
		D3DXMATRIX matBone;
		D3DXMATRIX matTrns;
		
		m_pSrcChr->GetBoneLinkTM( 2 , matBone );
		
		DarkData[i].Ball[0].m_vStartPos = D3DXVECTOR3(matBone._41  , matBone._42 , matBone._43 );
		DarkData[i].Ball[1].m_vStartPos = D3DXVECTOR3(matBone._41  , matBone._42 , matBone._43 );
		DarkData[i].Ball[2].m_vStartPos = D3DXVECTOR3(matBone._41  , matBone._42 , matBone._43 );


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

		
		*/
		
		m_pDestChr->GetBoneLinkTM( 1 , matBone );
		D3DXVECTOR3 vDestPos( matBone._41 ,
							  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[i]) ,
							  matBone._43 );

		//현재 바라보는 각 ? 
		
		D3DXMatrixRotationY( &DarkData[i].Ball[0].matY , m_pSrcChr->m_radian ) ;
		D3DXMatrixRotationY( &DarkData[i].Ball[1].matY , m_pSrcChr->m_radian ) ;
		D3DXMatrixRotationY( &DarkData[i].Ball[2].matY , m_pSrcChr->m_radian ) ;

		
		//FireData[i].matLocal._41 = FireData[i].m_vStartPos.x ; 
		//FireData[i].matLocal._42 = FireData[i].m_vStartPos.y ; 
		//FireData[i].matLocal._43 = FireData[i].m_vStartPos.z ; 


		DarkData[i].Ball[0].m_RangeTime = PositionToTimer( DarkData[i].Ball[0].m_Speed,  DarkData[i].Ball[0].m_vStartPos , vDestPos );
		DarkData[i].Ball[1].m_RangeTime = PositionToTimer( DarkData[i].Ball[1].m_Speed,  DarkData[i].Ball[1].m_vStartPos , vDestPos );
		DarkData[i].Ball[2].m_RangeTime = PositionToTimer( DarkData[i].Ball[2].m_Speed,  DarkData[i].Ball[2].m_vStartPos , vDestPos );
		
		DarkData[i].Ball[0].m_OldTime = g_nowTime;
		DarkData[i].Ball[1].m_OldTime = g_nowTime;
		DarkData[i].Ball[2].m_OldTime = g_nowTime;
		
		DarkData[i].Ball[0].bStartFlag = true; 
		DarkData[i].Ball[1].bStartFlag = true; 
		DarkData[i].Ball[2].bStartFlag = true; 
					
	}

	//1구에 3볼 동시에 나간다 . 
	

	DarkData[0].bLumpflag = true; 

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFTripleDarkness::Render(float dtime)
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

	for(int i =0 ; i < c_Max_Ball_Num ; ++i)
	{
		if( DarkData[i].bLumpflag )
		{
			for( int j = 0 ; j < 3 ; ++j) 
			{
				//도착하면 그려주지않고 끈다 
				if( DarkData[i].Ball[j].bStartFlag)
				{
					DarkData[i].m_DarkParticle[j].Render();
				}
			}		
		}
	}

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

int CEFTripleDarkness::CheckFrame(void)
{
	return TRUE;
}

int CEFTripleDarkness::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	int i =0 ;
	int j = 0 ; 	
	//볼 3구에 대한처리 .. 
	for( i =0 ; i < c_Max_Ball_Num ; ++i)
	{
		if(!DarkData[i].bLumpflag) 
		{
			return TRUE;
		}
		for( j =0 ; j < 3 ; ++j) 
		{
			if( DarkData[i].Ball[j].bStartFlag )
			{
				DarkBall(i , j , DarkData[i].Ball[j].m_Theta , dtime );
			}
		}		
	}

	//스펠이 끝나면..

	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFTripleDarkness::SetAfterEffect(void)
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
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_DARKNESSED01 , 0 , 2.1f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );


	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_DARKNESSED01;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 1.1f;
	}

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
		

	m_EffectCnt++;

	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );


	return TRUE;
}

void CEFTripleDarkness::DarkBall(int LumpNum , int Num , int Theta , float dtime )
{
	
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
	m_pDestChr->GetBoneLinkTM( 1 , matBone );
	D3DXVECTOR3 vDestPos( matBone._41 ,
						  m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[Num]) ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - DarkData[LumpNum].Ball[Num].m_vStartPos;
	
	float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , DarkData[LumpNum].Ball[Num].m_OldTime) / ( float ) DarkData[LumpNum].Ball[Num].m_RangeTime ) );
	float		RangeRadius = D3DXToRadian( 360 ) * Range ; 
	

	if( Range > 1.0f ) 
	{
		Range  = 1.0f ; 
	}

	D3DXMATRIX matRot;
	D3DXMATRIX matMul;
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
	if(DarkData[LumpNum].Ball[Num].OldRange != Range )
	{
					
		//예전과 현재를 차이를 구한다 .
		float RangeTemp =  Range - DarkData[LumpNum].Ball[Num].OldRange;
		float OldRadius  = 0 ;
		float MaxRange= 0 ; 
		
		//RangeTemp 는 소수점으로 나오고 ex)0.1f나왔을경우 0.1/0.02 로 나누어서 5개를 더구한다. 
		// 
		D3DXVECTOR3 vPos2 = D3DXVECTOR3(0.0f, 0.0f , 0.0f );
		D3DXVECTOR3 vCurPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f );
		
		//보간을 한다 
		for(float fi = 0.0f ; fi < RangeTemp; fi+=0.03f)
		{
			MaxRange= (DarkData[LumpNum].Ball[Num].OldRange + fi)*4.0f;
			OldRadius = D3DXToRadian( Theta ) * (DarkData[LumpNum].Ball[Num].OldRange + fi);
		
			D3DXMatrixRotationZ( &matRotZ , OldRadius + DarkData[LumpNum].Ball[Num].m_Angle );
			D3DXMatrixMultiply( &matMul ,  &matRotZ, &DarkData[LumpNum].Ball[Num].matY );
			D3DXVec3TransformCoord( &vDir , &D3DXVECTOR3(8.0f-MaxRange, 0.0f, 0.0f ) , &matMul );
					
			vPos2      = DarkData[LumpNum].Ball[Num].m_vStartPos + ( vDistance *  (DarkData[LumpNum].Ball[Num].OldRange + fi)  );  
						

			vCurPos.x = vPos2.x  + vDir.x ;
			vCurPos.y = vPos2.y  + vDir.y ;
			vCurPos.z = vPos2.z  + vDir.z ;
			
			g_Particle.DisplayUpdate( 24207 , vCurPos );
				
		}
		
		DarkData[LumpNum].m_DarkParticle[Num].SetLinkPos( vCurPos );
		DarkData[LumpNum].m_DarkParticle[Num].UpdateLink( dtime );

		DarkData[LumpNum].Ball[Num].OldRange  = Range;
	}

	//첫볼이 반을 넘어서면 두번째 럼프를 켜준다
	if(LumpNum < 2 && Num == 0  )
	{
		if(Range > 0.5f && (!DarkData[LumpNum+1].bLumpflag))
		{
			if(LumpNum < 2 )
			{
				DarkData[LumpNum+1].bLumpflag = true;
				DarkData[LumpNum+1].Ball[0].m_OldTime = g_nowTime;
				DarkData[LumpNum+1].Ball[1].m_OldTime = g_nowTime;
				DarkData[LumpNum+1].Ball[2].m_OldTime = g_nowTime;
	
			}
		}
	}

	
	if( Range >= 1.0f ) 
	{
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
		
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 1 , matBone );
		D3DXVECTOR3 vec( matBone._41 ,
						 m_pDestChr->GetPosTM()._42 + ((matBone._42 - m_pDestChr->GetPosTM()._42)*m_fHeight[Num]) ,
						 matBone._43 );		

		 if( ((UINT)rand()%11) > 5) 
		 {
			g_Particle.SetEffSetCon( 0.0f , EF_DARKNESSED01 , ESLT_POS , 2.1f , -1 , -1 , vec , 0.0f, -1 , 0  );
		 }
	
		DarkData[LumpNum].Ball[Num].bStartFlag = false;

		if( LumpNum == 2 && Num == 2)
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