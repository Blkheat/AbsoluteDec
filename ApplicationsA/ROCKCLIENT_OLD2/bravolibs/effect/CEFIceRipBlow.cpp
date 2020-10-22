//CEFIceRipBlow
#include "CEFIceRipBlow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFIceRipBlow::CEFIceRipBlow()
{
	m_IsConEffect = TRUE;
}

CEFIceRipBlow::~CEFIceRipBlow()
{
}

void CEFIceRipBlow::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}


	D3DXMATRIX matStart;

	// �Ӹ� ��ġ ���ϱ�
	m_pSrcChr->GetBoneLinkTM( 1 , matStart );

	m_vStartPos.x = matStart._41;
	m_vStartPos.y = matStart._42;
	m_vStartPos.z = matStart._43;
	
	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );
	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( 70 , m_vStartPos , vDestPos );

	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_OldTimer    = g_nowTime;
}


int CEFIceRipBlow::Render(float dtime)
{
	return TRUE;
}

int CEFIceRipBlow::Update(float dtime)
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
	
	float       Range     = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTimer ) / ( float ) m_RangeTime ) );

	if( Range > 1.0f ) 
		Range = 1.0f;

	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );  
	
	g_Particle.DisplayUpdate( 21756 , vPos );

	//............................................................................................................
	// ���̾ ĳ���Ϳ� �浹 ó�� ( ����Ʈ .... T.T )
	//............................................................................................................
	if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( m_RangeTime , m_OldTimer ) ) )
	{
		SetDestroy();
	
		SetAfterEffect();
	}
	
	return TRUE;
}

//................................................................................................................
// ����Ʈ ó���� �ڷο� ����Ʈ �¾�
//................................................................................................................
int CEFIceRipBlow::SetAfterEffect(void)
{
	//................................................................................	
	// ���� ����
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_ICE_RIP_BLOWED , 0 , 5.1f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_ICE_RIP_BLOWED;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 5.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	return TRUE;
}
