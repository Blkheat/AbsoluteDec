#include "CEFHolyBomb.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;

CEFHolyBomb::CEFHolyBomb()
{
	m_IsConEffect = TRUE;
}

CEFHolyBomb::~CEFHolyBomb()
{
}

void CEFHolyBomb::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}
	
	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24216 ) ) == NULL )
		return;
	m_BallParticle[0].MakeLink(pData);

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( 24228 ) ) == NULL )
		return;
	m_BallParticle[1].MakeLink(pData);

/*
	  g_Particle.m_EffectManager.m_TargetInfo.TargetType = n_Cell;
			  g_Particle.m_EffectManager.m_TargetInfo.vPos = 
			  CharSelectWnd.GetPos( CharSelectWnd.PosInfo[ 8 ].CelPos[ 0 ] , 
			  CharSelectWnd.PosInfo[ 8 ].CelPos[ 1 ] );
			  g_Particle.m_EffectManager.Create( EF_ICE_RAIN , NULL , NULL );		
		
*/
	
	m_OldTimer    = g_nowTime;

	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
								 m_pSrcChr->GetRealRHandTM()._42 ,
								 m_pSrcChr->GetRealRHandTM()._43  );
	
	float fDelHeight = m_vStartPos.y - g_Map.PHF_GetHeight( m_vStartPos.x , m_vStartPos.z );

	D3DXVECTOR3 m_vDestPos( m_EffectTargetInfo.vPos.x ,
						  g_Map.PHF_GetHeight( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z )+fDelHeight ,
						  m_EffectTargetInfo.vPos.z );

	m_vDistance = m_vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , m_vDestPos );

	if( m_EffectTargetInfo.vPos.x == 0.0f &&  m_EffectTargetInfo.vPos.z == 0.0f )
	{
		SetDestroy(TRUE);
	}
	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}


int CEFHolyBomb::Render(float dtime)
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

	m_BallParticle[0].Render();
	m_BallParticle[1].Render();

	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );


	return TRUE;
}

int CEFHolyBomb::CheckFrame(void)
{
	return TRUE;
}

int CEFHolyBomb::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
  
	D3DXVECTOR3 vDistance = m_vDestPos - m_vStartPos;
	
	float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTimer ) / ( float ) m_RangeTime ) );
	
	if( Range > 1.0f ) 
	{
		Range = 1.0f;
		SetAfterEffect();
		SetDestroy();

	}

	D3DXVECTOR3 vPos      = m_vStartPos + ( m_vDistance * Range );  
	m_BallParticle[0].SetLinkPos( vPos );
	m_BallParticle[0].UpdateLink( dtime );
	m_BallParticle[1].SetLinkPos( vPos );
	m_BallParticle[1].UpdateLink( dtime );

//	g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );
	

	//............................................................................................................
	// 끝
	//............................................................................................................
	
	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFHolyBomb::SetAfterEffect(void)
{

	// 데미지 효과
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );


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
				g_Particle.SetEffSetCon( 0.0f , EF_HOLY_BOMB_ED , 0 , 3.0f , 
					pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				
				Info.AfterEffect = EF_HOLY_BOMB_ED;
				Info.DestType = 0;
				Info.lifeTime = 2.0f;
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
			}
			
			Info.Clear();
		}
	}

	
	return TRUE;
}
