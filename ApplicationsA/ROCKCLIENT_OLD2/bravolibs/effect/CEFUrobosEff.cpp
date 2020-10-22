#include "CEFUrobosEff.h"
#include "..\\Effect.h"

extern long		 g_nowTime;

CEFUrobosEff::CEFUrobosEff()
{
#ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
	
	m_bDust = false;
	m_iPoisonBreathCount = 1;
	m_iFrameBalkanLifeTime = 2100;
	
#else
	
	m_bDust = true;
	m_iPoisonBreathCount = 8;
	m_iFrameBalkanLifeTime = 4100;

#endif// #ifdef HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207
}

CEFUrobosEff::~CEFUrobosEff()
{
}

void CEFUrobosEff::Init(void)
{
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);	
		return;
	}	

	ZeroMemory(m_bStep , sizeof(m_bStep));
	
	if(m_bDust)
		g_Particle.SetEffSetCon( 0.0f , 	EF_UROBOROS_DUST , 0 , 4.1f , 
			m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	m_NowTime = g_nowTime;
	m_OldTime = g_nowTime;
	
	switch(m_ID)
	{
		case EF_UROBOROS_POISON:
			InitPoisonBreath();
			break; 

		case EF_UROBOROS_BALKAN:
			InitFrameBalkan();
			break; 

		case EF_UROBOROS_RUN:
			InitCharge();
			break;
	}
}


int CEFUrobosEff::CheckFrame(void)
{
	switch(m_ID)
	{
		case EF_UROBOROS_POISON:
			return CheckPoisonBreath(); 

		case EF_UROBOROS_BALKAN:
			return CheckFrameBalkan();

		case EF_UROBOROS_DRAG_HOWLLING:
			return CheckDragHowlling();

		case EF_UROBOROS_RUN:
			return CheckCharge(); 
	}	
	
	return TRUE;
}


int CEFUrobosEff::Update(float dtime)
{
	switch(m_ID)
	{
		case EF_UROBOROS_POISON:
			return UpdaterPoisonBreath(dtime);
	}	
	
	return TRUE;
}

int CEFUrobosEff::Render(float dtime)
{
	switch(m_ID)
	{
		case EF_UROBOROS_POISON:
			return RenderPoisonBreath(dtime);

		case EF_UROBOROS_BALKAN:
			return RenderFrameBalkan(dtime);
	}	
	
	return TRUE;
}

int CEFUrobosEff::SetAfterEffect()
{
	switch(m_ID)
	{
		case EF_UROBOROS_DRAG_HOWLLING:
			return SetAeffectDragHowlling();
		
		case EF_UROBOROS_RUN:
			return SetAeffectCharge(); 
	}	
	
	return TRUE;	
}

void CEFUrobosEff::InitPoisonBreath()
{
	
	//파이볼 파티클 필요 
	
	
	//파티클 파이볼구조의 파이어 이미지와 파이어 꼬리가 필요하다 .. 
	CParticleData *pData = NULL; 
	
	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(PT_FIREBALL)) == NULL)
		return;
	
	m_FireParticle.m_pReference = pData; 
	
	m_FireParticle.m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_FireParticle.m_bIsSuppressed	= false;
	m_FireParticle.m_bIsCreate		= true;
	m_FireParticle.m_lUnique		= -1;
	m_FireParticle.m_iIndex			= -1;
	m_FireParticle.m_fSize			= 2.5f;			
	
	// 스킬 범위
	m_OldTime    = g_nowTime;
	
	if( m_EffectTargetInfo.vPos.x == 0.0f &&  m_EffectTargetInfo.vPos.z == 0.0f )
	{
		m_EffectDrop.Init( m_pSrcChr->GetPosTM()._41  , m_pSrcChr->GetPosTM()._43		, 150.0f , 
			D3DXToRadian( -25 ) ,  700 , 1200 , m_iPoisonBreathCount, 500 , g_nowTime , 2000 , TRUE );	
	}
	else 
	{
		WORD aSkillCode = 0;
		float fRadius = 0.0f;
		 
		if(m_pExtraInfo)
		{
			aSkillCode = m_pExtraInfo->SkillCode;
			fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
		}
					
		D3DXVECTOR3 vCDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
		D3DXVECTOR3 m_vTempPos;
		D3DXMATRIX matRotY;
	
		D3DXVECTOR3 vStartPos = D3DXVECTOR3(m_EffectTargetInfo.vPos.x , 0.0f  , m_EffectTargetInfo.vPos.z ) ;
		D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection() );
		D3DXVec3TransformCoord( &vCDir , &vCDir , &matRotY );	
		
		//m_vTempPos = vStartPos + ( 50.0f * vCDir );
		
		///-- Init(중심 X좌표, 중심 Z좌표, 높이, Z축회전각도, 최소낙하시간, 최대낙하시간,
		///-- 낙하개수, 효과범위, 현재시간, 러닝타임(간격타임을 구하기위한 용도), 낙하반복여부
		m_EffectDrop.Init( vStartPos.x , vStartPos.z  , 300.0f , 
			D3DXToRadian( -25 ) , 700 , 1200 , m_iPoisonBreathCount, fRadius+5 , g_nowTime , 2000 , TRUE );	
	}

	m_ISDamageProcess = TRUE;
	
	//파티클 매트릭스 초기화 
	D3DXMatrixIdentity( &m_FireParticle.m_LinkMat );

	//구렁이 꾸미기 .. 

	// 목쪽에 들어갈것. . 
	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24378 ) ) == NULL )
	{
		SetDestroy();
		return;
	}

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24755 ) ) == NULL )
	{
		SetDestroy();
		return;
	}
	
	m_AddParticleLink[0].MakeLink( pData );
	m_AddParticleLink[0].m_fSize = 2.5f;
	
	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24756 ) ) == NULL )
	{
		SetDestroy();
		return;
	}
	
	m_AddParticleLink[1].MakeLink( pData );
	m_AddParticleLink[1].m_fSize = 2.5f;

	
}

int		CEFUrobosEff::CheckPoisonBreath()
{
	m_pSrcChr->DisplayMotionBlow();
	return TRUE ; 
}

int		CEFUrobosEff::UpdaterPoisonBreath(float dtime)
{

	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		///-- 공격 주체가 죽는다해도 2.1초후까지는 데미지처리를 해준다.		
		/*	
		SetDestroy();
		return TRUE;*/
	}

	//떨어지는 무언가를 시간업뎃 ... 
	m_EffectDrop.Update( ( DWORD )g_nowTime );

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	//데미지 처리 . 
	if( m_ISDamageProcess )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 1000 ) )
		{
			if( m_pExtraInfo != NULL )
			{
				// 1:N 데미지
				int Count = m_pExtraInfo->GetTargetCount();		
				
				for( int i = 0 ; i < Count ; ++i )
				{
					//맞은넘이 있으면 뎀쥐 처리와 불이 입어서 몸에서 불이 타오른다 .. 
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						Info.AfterEffect = EF_FIREBALL_ATTACKED0;
						Info.DestType = 0;
						Info.lifeTime = 2.1f;

						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info);

						g_Particle.SetEffSetCon( 0.0f , EF_FIREBALL_ATTACKED0 , 0 , 2.1f , 
												 pTempDest->GetUnique() , pTempDest->GetIndex(), 
												 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );					
												
						if( m_pSrcChr == g_Pc.GetPlayer() )
							g_Particle.RockCamera( (rand()% 200 ) + 300);

					}
					Info.Clear();
				}
			}
			m_ISDamageProcess = FALSE;
		}
	}

	///-- 해제하는 루틴이 없네... 무한반복이라 
	///-- 이렇게 하지 않으면 해제되지 않음	
	///-- 일단 복구시킴 By simwoosung
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 5100 ) )
	{
		m_EffectDrop.SetIsLoop(FALSE);
		return TRUE;
	}

	return TRUE;
}

int		CEFUrobosEff::RenderPoisonBreath(float dtime)
{

	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();
	
	if( m_EffectDrop.GetDataSize() )
	{
		DROP_EFFECT_DATA *pData = NULL;
		D3DXMATRIX		  matLocal;
		
		D3DXMatrixIdentity( &matLocal );
		
		m_EffectDrop.StartPosition();	
		
		D3DXMatrixIdentity( &matLocal );
		
		g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
		
		g_RenderManager.SetCullMode( D3DCULL_NONE );
		
		g_RenderManager.SetZWriteEnable( FALSE );
		g_RenderManager.SetZEnable( D3DZB_TRUE );
		
		g_RenderManager.SetAlphaBlendEnable( TRUE );
		g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
		g_RenderManager.SetDestBlend( D3DBLEND_ONE );
				

		D3DXMATRIX mat;
		D3DXVECTOR3 vPos , vPos1 , vPos2; 

		m_pSrcChr->GetDirectBoneLinkTM( 22 , mat);
		
		vPos.x = mat._41;
		vPos.y = mat._42;
		vPos.z = mat._43;
				
		D3DXVECTOR3 vCDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
		D3DXVECTOR3 vCurPos;

		D3DXMATRIX matRotY;
	
		D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection() );
		D3DXVec3TransformCoord( &vCDir , &vCDir , &matRotY );	
		
		vCurPos = vPos + ( 5.0f * vCDir );
		
		m_AddParticleLink[0].SetLinkPos( vCurPos );
		m_AddParticleLink[0].UpdateLink( dtime );
		m_AddParticleLink[0].Render();		
		m_AddParticleLink[1].SetLinkPos( vCurPos );
		m_AddParticleLink[1].UpdateLink( dtime );
		m_AddParticleLink[1].Render();		

		while( ( pData = m_EffectDrop.GetDropData() ) != NULL )
		{			
			if( pData->State == DROPST_DROP	|| 	pData->State == DROPST_END )
			{					
				//바닥에 떨어 졌을때 .. 
				if( pData->State == DROPST_END )
				{
					g_Particle.m_fAddSize = 1.2f; 
					g_Particle.SetEffSetCon( 0.0f , /*EF_FOOT_QUAKE0*/EF_UROBOROS_POISON_ED , 100 , 2.6f , -1 , -1 , pData->vPos );
				}				
				
				g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
				
				m_FireParticle.SetLinkPos( pData->vPos );
				m_FireParticle.UpdateLink( dtime );
				m_FireParticle.Render();
				
				//파이어볼 꼬리를 달고 간다 .. 
				//꼬리 싸이즈를 키운다... 
				// 크기를 늘린다 .. ㅇㅋㅂ?  
				g_Particle.DisplayUpdate( 24875 , pData->vPos );
			}
			
		}
		
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		g_RenderManager.SetAlphaBlendEnable( FALSE );
		
	}
	else
	{
		
		g_RenderManager.SetCullMode( dCullMode );
		g_RenderManager.SetZWriteEnable( dZWriteEnable );
		g_RenderManager.SetZEnable( dZEnable );
		g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
		g_RenderManager.SetSrcBlend( dSrcBlend );
		g_RenderManager.SetDestBlend( dDestBlend );		
		
		SetDestroy();
		return TRUE;
	}
	
	g_RenderManager.SetCullMode( dCullMode );
	g_RenderManager.SetZWriteEnable( dZWriteEnable );
	g_RenderManager.SetZEnable( dZEnable );
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );




	return TRUE ; 
}

//////////////////////////////////////////////////////////////////////////
// 프레임 발칸 
//////////////////////////////////////////////////////////////////////////

int		CEFUrobosEff::InitFrameBalkan()
{
	//파티클 파이볼구조의 파이어 이미지와 파이어 꼬리가 필요하다 .. 
	CParticleData *pData = NULL; 

	//이클래스 안에서는 한모션이 끝난후에도 돌아야 한다 그러므로 
	//용에 입붙은 이펙트를 꺼야 하고 불기둥은 총 5초정도 돌린다. 
	//모션이 다를때는 B동작만 작동하게한다 .. 

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24825 ) ) == NULL )
	{
		SetDestroy(TRUE);
		return FALSE;
	}

	m_AddParticleLink[0].MakeLink( pData );
	m_AddParticleLink[0].m_fSize = 3.0f;
	
	m_ISDamageProcess = TRUE; 

	//................................................................................	
	// 스킬 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );



	return TRUE; 
}
int		CEFUrobosEff::CheckFrameBalkan()
{

	m_pSrcChr->DisplayMotionBlow();

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index)
	{	
		g_Particle.DelEffSetCon( EF_UROBOROS_BALKAN_MOUTH , ESLT_CHARPOS , 
			m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	}
	
	//자동으로 끄게 처리 .. 
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , m_iFrameBalkanLifeTime ) )
	{
		SetDestroy();
		
		g_Particle.DelEffSetCon( EF_UROBOROS_BALKAN_MOUTH , ESLT_CHARPOS , 
			m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		
		
		return TRUE;
	}


	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
		return TRUE;


	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;


	if( m_ISDamageProcess )
	{
		///-- 이펙트 시전후.. 1초가 지나면 데미지 처리
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 1500 ) )
		{
			if( m_pExtraInfo != NULL )
			{
				// 1:N 데미지
				int Count = m_pExtraInfo->GetTargetCount();			
				
				for( int i = 0 ; i < Count ; ++i )
				{
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						Info.AfterEffect = EF_UROBOROS_BALKAN_ED;
						Info.DestType = 0;
						Info.lifeTime = 5.1f;
						
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );					
						
						g_Particle.SetEffSetCon( 0.0f , EF_UROBOROS_BALKAN_ED , 0 , 4.1f , 
							pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
					}
					
					Info.Clear();
				}
			}
			
			m_ISDamageProcess = FALSE;
		}
	}

	
	

	return TRUE;
}

//
int		CEFUrobosEff::RenderFrameBalkan(float dtime)
{

	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );
	
	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	

	D3DXMATRIX mat;
	D3DXVECTOR3 vPos , vPos1 , vPos2; 
	
	m_pSrcChr->GetDirectBoneLinkTM( 22 , mat);
	
	vPos.x = mat._41;
	vPos.y = mat._42;
	vPos.z = mat._43;
	
	D3DXVECTOR3 vCDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 vCurPos;
	
	D3DXMATRIX matRotY;
	
	D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection() );
	D3DXVec3TransformCoord( &vCDir , &vCDir , &matRotY );	
	
	vCurPos = vPos + ( 8.0f * vCDir );
	
	m_AddParticleLink[0].SetLinkPos( vCurPos );
	m_AddParticleLink[0].UpdateLink( dtime );
	m_AddParticleLink[0].Render();	

	//1초마다 터져라
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowTime ) , > , 1000 ) )
	{
		m_NowTime = g_nowTime;

		float fRadius = 8.0f; 
		float fR = Random( -fRadius , fRadius );
		fR = fR > 0.0f ? -(fRadius / 2) : fRadius / 2;
		
		D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
		D3DXMATRIX   matRot;
		D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
		///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
		D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
		vPos *= 10;
		vPos += D3DXVECTOR3(  m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z);
		vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
		
		g_Particle.SetEffSetCon( 0.0f , EF_FRAME_STORM , ESLT_POS , 2.1f , -1 , -1 , vPos);

	}
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}

//
int		CEFUrobosEff::CheckDragHowlling()
{
	m_pSrcChr->DisplayMotionBlow();

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		SetDestroy();

	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > m_pSrcChr->m_end_frame )
		SetDestroy();

	if(m_pSrcChr->m_ani_frame == 40)
	{
		m_ISDamageProcess = TRUE;

		D3DXVECTOR3 vPos , vStartPos , vEndPos;
		D3DXMATRIX mat;
		
		m_pSrcChr->GetDirectBoneLinkTM( 22 , mat);
		
		vStartPos.x = mat._41;
		vStartPos.y = mat._42;
		vStartPos.z = mat._43;
		
		D3DXVECTOR3 vCDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
		D3DXVECTOR3 vCurPos;
		D3DXMATRIX matRotY;
		
		D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection() );
		D3DXVec3TransformCoord( &vCDir , &vCDir , &matRotY );	
		
		vPos = vStartPos + ( 10.0f * vCDir );
				
		g_Particle.SetEffSetCon( 0.0f , EF_UROBOROS_DRAG_HOWLLING , ESLT_POS , 
					5.1f , -1 , -1 , vPos , m_pSrcChr->GetDirection());
		
		SetAfterEffect();
	
	}
	
	return TRUE;
}

//
int		CEFUrobosEff::SetAeffectDragHowlling()
{
	if( m_pExtraInfo == NULL )
		return  TRUE; 

	int Count = m_pExtraInfo->GetTargetCount();
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			
			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );
			
			g_Particle.SetEffSetCon( 0.0f , EF_UROBOROS_DRAG_HOWLLING_ED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ) );
			
			Info.AfterEffect = 	EF_RUGAL_NAIL_OFDECAYED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//우로 보스 돌진 
//////////////////////////////////////////////////////////////////////////
int		CEFUrobosEff::InitCharge()
{

	D3DXVECTOR3  vPos , vSatrtPos;

	vSatrtPos.x = m_pSrcChr->GetPosTM()._41; 
	vSatrtPos.z = m_pSrcChr->GetPosTM()._43; 
	vSatrtPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
	vSatrtPos.y += 20; 

	D3DXMATRIX matRotY;
	D3DXVECTOR3 vCDir = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
	D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection() );
	D3DXVec3TransformCoord( &vCDir , &vCDir , &matRotY );	
	
	vPos = vSatrtPos + ( 10.0f * vCDir );

	g_Particle.m_fAddSize = 1.0f ; 
	g_Particle.SetEffSetCon( 0.0f , 1151 , ESLT_POS , 2.1f , -1 , -1 , vPos);

	return TRUE;
}


int		CEFUrobosEff::CheckCharge()
{
	m_pSrcChr->DisplayMotionBlow();

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index)
		SetDestroy();

	if(m_pSrcChr->m_ani_frame == 50)
		SetAeffectCharge();

	return TRUE;
}

//
int		CEFUrobosEff::SetAeffectCharge()
{
	if( m_pExtraInfo == NULL )
		return  TRUE; 

	int Count = m_pExtraInfo->GetTargetCount();
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			
			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );
			
			g_Particle.SetEffSetCon( 0.0f , EF_UROBOROS_RUN_ED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ) );
			
			Info.AfterEffect = 	EF_UROBOROS_RUN_ED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}

	return TRUE;
}
