#include "CEFWrath_Bomb.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;


CEFWrath_Bomb::CEFWrath_Bomb()
{
	m_IsConEffect = TRUE;
	m_ISCreate = false ;

}

CEFWrath_Bomb::~CEFWrath_Bomb()
{

}

void CEFWrath_Bomb::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return;
	}

	int m_TileCount = 0;

	// 1:N 데미지

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	
	
//	m_OldTimer    = g_nowTime;
//	m_DelayTimer = g_nowTime;

	//처음 위치점 잡고
   	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetPosCenterBoneTM()._41 ,
								 0.0f ,
								 m_pSrcChr->GetPosCenterBoneTM()._43  );
	
	//범위 얻어온다 한타일 16pixel 이고 반지름은 8이다 . 맞나 물어봐야것네.
	//m_TileCount = ( int )( g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode ) / 8.0f );
	
	m_TileCount = (int)((7.0f*8.0f)/8.0f);
	
	//끝거리 ? 어따서 ? 
    m_vEndPos = m_vStartPos;

	//최대이동거리  이동 * 타일칸 
   	m_fDistace = 8.0f * m_TileCount;
	
	//도착점 까지 걸린 시간 이동한다. 
	m_RangeTime = 70 * m_TileCount;
	
	m_vDir = m_pSrcChr->m_vDir;
	
	
	//시작점과 끝점을 셋팅한다 . 
   	m_Range.SetMaxTime( m_RangeTime);
	m_Range.Insert( 0 , 8.0f );
	m_Range.Insert( m_RangeTime , m_fDistace );
	
	//m_ISCreate = TRUE;
		
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	
}


int CEFWrath_Bomb::Render(float dtime)
{
	return TRUE;
}

int CEFWrath_Bomb::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{

//		SetDestroy();
	}

	//	m_ISCreate = TRUE;
	//캐릭터 동작이 50frm
	if( GetSrcAniCount() == 30 )
	{
	
		m_ISCreate = TRUE;
		m_OldTimer = g_nowTime;
		m_DelayTimer = g_nowTime;

	}

	
	return TRUE;
}
int CEFWrath_Bomb::SetAfterEffect(void)
{

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;	
	
	if( m_pExtraInfo != NULL )
	{
		SSkillBaseInfo* skill_Info;
		
		skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_pExtraInfo->SkillCode ); 
		
		if( skill_Info == NULL )
			return TRUE;
		
		int Count = m_pExtraInfo->GetTargetCount();		
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
			
				g_Particle.SetEffSetCon( 0.0f , EF_WRATH_BOMB_ED , 0 , 3.0f , 
					pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				
				Info.AfterEffect = EF_WRATH_BOMB_ED;
				Info.DestType = 0;
				Info.lifeTime = 2.0f;
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
			}
			
			Info.Clear();
		}
	}


	return TRUE;
}


int CEFWrath_Bomb::Update(float dtime)
{


	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	if( m_ISCreate )
	{	
		float Size = 0.0f;
			
		g_Particle.SetEffSetCon( 0.0f , EF_WRATH_BOMB , ESLT_SRC_RAD , 2.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , m_pSrcChr->GetDirection() );
		g_Particle.SetEffSetCon( 0.0f , EF_SWORD_FLASH2 , ESLT_SRC_RAD , 2.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , m_pSrcChr->GetDirection() );
		
		SetAfterEffect();
		SetDestroy();
		
	}	
	
	return TRUE;
}


void CEFWrath_Bomb::PositionEffect(float Size)
{
		
	m_vPos = m_vStartPos + m_vDir * Size;
	m_vPos.y = g_Map.PHF_GetHeight( m_vPos.x , m_vPos.z );
	g_Particle.SetEffSetCon( 0.0f , m_ID , ESLT_POS , 4.1f , -1 , -1 , m_vPos , m_pSrcChr->GetDirection() );
	
}