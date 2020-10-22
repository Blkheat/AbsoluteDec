#include "CEFBubbleCrash.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;

CEFBubbleCrash::CEFBubbleCrash()
{
	m_IsConEffect = TRUE;
}

CEFBubbleCrash::~CEFBubbleCrash()
{
}

void CEFBubbleCrash::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	m_OldTimer    = g_nowTime;
	
	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
								 m_pSrcChr->GetRealRHandTM()._42 ,
								 m_pSrcChr->GetRealRHandTM()._43  );
	
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	switch( m_ID )
	{
		case EF_BUBBLE_CRASH0:
		case EF_BUBBLE_CRASH1:
			
			m_Type = 0;

			break;
		case EF_BUBBLE_CRASH2:
		case EF_BUBBLE_CRASH3:

			m_Type = 1;

			break;
		case EF_BUBBLE_CRASH4:

			m_Type = 2;
			
			break;
	}

	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFBubbleCrash::Render(float dtime)
{

	return TRUE;
}

int CEFBubbleCrash::CheckFrame(void)
{
	return TRUE;
}

int CEFBubbleCrash::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
	
	float       Range     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime ) );

	if( Range > 1.0f ) 
		Range = 1.0f;

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );  
	
	g_Particle.DisplayUpdate( PT_BUBBLE_TAIL , vPos );
	
	//............................................................................................................
	// 끝
	//............................................................................................................
	if( SAFE_TIME_COMPARE( g_nowTime , > ,  SAFE_TIME_ADD( m_RangeTime , m_OldTimer ) ) )
	{
		SetDestroy();	
		SetAfterEffect();
	}

	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFBubbleCrash::SetAfterEffect(void)
{
	int EffectID = 0;
	
	switch( m_ID )
	{
		case EF_BUBBLE_CRASH0:
		case EF_BUBBLE_CRASH1:
			EffectID = EF_BUBBLE_CRASH_ATTACKED0;
			break;

		case EF_BUBBLE_CRASH2:
		case EF_BUBBLE_CRASH3:
			EffectID = EF_BUBBLE_CRASH_ATTACKED2;
			break;

		case EF_BUBBLE_CRASH4:
			EffectID = EF_BUBBLE_CRASH_ATTACKED4;
			break;
	}

	// 데미지 효과
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EffectID , 0 , 2.1f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EffectID;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	
	return TRUE;
}
