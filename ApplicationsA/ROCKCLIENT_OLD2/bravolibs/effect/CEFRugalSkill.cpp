#include "CEFRugalSkill.h"
#include "..\\Effect.h"

extern long		 g_nowTime;


//----------------------------------------------------------------------------------------------------------------------
CEFRugalSkill::CEFRugalSkill()
{
	m_Scale = 1.0f;
	m_NowScaleTime = g_nowTime;
}

//----------------------------------------------------------------------------------------------------------------------
CEFRugalSkill::~CEFRugalSkill()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

//----------------------------------------------------------------------------------------------------------------------
int CEFRugalSkill::Update(float dtime)
{		
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();
		return TRUE;
	}

	switch(m_ID)
	{
	case EF_RUGAL_HOWLLING_1: 
		UpdateWolfShout();
		break;

	case EF_RUGAL_CURSE_1: 
		UpdateWolfCurse();
		break;

	case EF_RUGAL_ILLUSIONTREE_1:
		UpdateWolfTree();
		break;

	case EF_RUGAL_ONECUTOFRUIN :
		UpdateWolfDestory();
		break;

	case EF_RUGAL_NAIL_OFDECAY : 
		UpdateWolfNail();
		break;

	case EF_RUGAL_RECALL_SOLTAR: 
		UpdateSoltarRecall();
		break;

	case EF_RUGAL_CLO_CYCLON:
		UpdateSoltarCyclon();
		break;

	case EF_RUGAL_DRIL_TEMPEST:
		UpdateTempestDril();
		break;

	case EF_RUGAL_DRIL_HURRICANE:
		UpdateTempestHurricane();
		break;	
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int	CEFRugalSkill::Render(float dtime)
{

	switch(m_ID)
	{
	case EF_RUGAL_HOWLLING_1: 
	case EF_RUGAL_CURSE_1: 
	case EF_RUGAL_ILLUSIONTREE_1:
	case EF_RUGAL_ONECUTOFRUIN :
	case EF_RUGAL_RECALL_SOLTAR: 
	case EF_RUGAL_CLO_CYCLON:
	case EF_RUGAL_DRIL_TEMPEST:
	case EF_RUGAL_DRIL_HURRICANE:
		SideEffect();
		break;

	case EF_RUGAL_NAIL_OFDECAY : 
		RenderWolfNail();
		break;
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFRugalSkill::SetAfterEffect()
{
	if(m_pExtraInfo  == NULL)
		return TRUE;

	switch(m_ID)
	{
	case EF_RUGAL_HOWLLING_1: 
		SetAfterEffetWolfShout();
		break; 

	case EF_RUGAL_CURSE_1: 
		SetAfterEffetCurse();
		break;

	case EF_RUGAL_ILLUSIONTREE_1:
		SetAfterEffetTree();
		break;

	case EF_RUGAL_ONECUTOFRUIN :
		SetAfterEffetDestory();
		break;

	case EF_RUGAL_NAIL_OFDECAY : 
		SetAfterEffetNail();
		break;

	case EF_RUGAL_CLO_CYCLON:
		SetAfterEffetSoltarCyclon();
		break;

	case EF_RUGAL_DRIL_TEMPEST:
		 SetAfterEffetTempestDril();
		 break;

	case EF_RUGAL_DRIL_HURRICANE:
		SetAfterEffetTempestHurricane();
		break;
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
int CEFRugalSkill::CheckFrame()
{
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
		SetDestroy();
	
	if(m_pSrcChr->m_ani_frame + c_MotionBlendHalfFrame > (int)m_pSrcChr->m_end_frame)
	{
		SetDestroy();
		return TRUE;
	}	


	switch(m_ID)
	{
	case EF_RUGAL_HOWLLING_1: 
		CheckFrmWolfShout();
		break; 

	case EF_RUGAL_CURSE_1: 
		CheckFrmWolfCurse();
		break;

	case EF_RUGAL_ILLUSIONTREE_1:
		CheckFrmWolfTree();
		break;

	case EF_RUGAL_ONECUTOFRUIN :
		CheckFrmWolfDestory();
		break;

	case EF_RUGAL_NAIL_OFDECAY : 
		CheckFrmWolfNail();
		break;

	case EF_RUGAL_CLO_CYCLON:	
		CheckFrmSoltarCyclon();
		break;

	case EF_RUGAL_DRIL_TEMPEST: 
		CheckFrmTempestDril();
		break;

	case EF_RUGAL_DRIL_HURRICANE: 
		CheckFrmTempestHurricane();
		break;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::Init(void)
{
	m_OldTime = g_nowTime;
	m_NowScaleTime = g_nowTime;
	m_Scale = 1.0f;
	m_EffectRun =false;
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
		
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( m_pExtraInfo == NULL || m_pDestChr == NULL )
		return ;
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	switch(m_ID)
	{
		case EF_RUGAL_HOWLLING_1: 
			m_SideEffectColor =  D3DXCOLOR(0.2f , 0.2f, 1.0f, 1.0f);
			g_Particle.SetEffSetCon( 1.3f , EF_RUGAL_HOWLLING_1  , 0 , 5.1f , m_pSrcChr->GetUnique()  , m_pSrcChr->GetUnique()  );
			break; 

		case EF_RUGAL_CURSE_1: 
			m_SideEffectColor =  D3DXCOLOR(1.0f , 0.0f, 1.0f, 1.0f);
			g_Particle.SetEffSetCon( 1.3f , EF_RUGAL_CURSE_1  , 0 , 5.1f , m_pSrcChr->GetUnique()  , m_pSrcChr->GetUnique()  );
			break;

		case EF_RUGAL_ILLUSIONTREE_1:
			m_SideEffectColor = D3DXCOLOR(1.0f , 1.0f, 1.0f, 1.0f);
			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_ILLUSIONTREE_1  , 0 , 5.1f , m_pSrcChr->GetUnique()  , m_pSrcChr->GetUnique()  );
			break;

		case EF_RUGAL_ONECUTOFRUIN :
			m_SideEffectColor = D3DXCOLOR(1.0f , 1.0f, 1.0f, 1.0f);
			break;

		case EF_RUGAL_RECALL_SOLTAR: 
			m_SideEffectColor = D3DXCOLOR(1.0f , 1.0f, 1.0f, 1.0f);
			g_Particle.SetEffSetCon( 1.5f , EF_RUGAL_RECALL_SOLTAR  , 0 , 5.1f , m_pSrcChr->GetUnique()  , m_pSrcChr->GetUnique()  );
			break; 

		case EF_RUGAL_CLO_CYCLON:	
			m_SideEffectColor = D3DXCOLOR(1.0f , 1.0f, 1.0f, 1.0f);
			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_CLO_CYCLON  , 0 , 5.1f , m_pSrcChr->GetUnique()  , m_pSrcChr->GetUnique()  );
			break;

		case EF_RUGAL_DRIL_TEMPEST: 
			m_SideEffectColor = D3DXCOLOR(1.0f , 0.0f, 1.0f, 1.0f);
			break;

		case EF_RUGAL_DRIL_HURRICANE:
			m_SideEffectColor = D3DXCOLOR(1.0f , 0.0f, 1.0f, 1.0f);
			D3DXVECTOR3 vStartPos   = D3DXVECTOR3( m_pSrcChr->GetPosCenterBoneTM()._41 ,0.0f ,m_pSrcChr->GetPosCenterBoneTM()._43  );
			D3DXVECTOR3 vPos = vStartPos + (m_pSrcChr->m_vDir * 40.0f) ;
			vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_DRIL_HURRICANE  , ESLT_POS , 4.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetUnique() 
					, vPos , m_pSrcChr->GetDirection() );
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateWolfShout()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.2f )
		m_Scale =1.0f; 
	
	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/20.f);
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmWolfShout()
{
	if(GetSrcAniCount() == 50 || GetSrcAniCount() == 18 )
		SetAfterEffetWolfShout();
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::SetAfterEffetWolfShout()
{
	//  몬스터를 소환 한다..
	// 서버 준대로 몬스터를 소환 한다.
	// 어캐 .. 쩝.. 
	int Count = m_pExtraInfo->GetTargetCount();

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			//................................................................................	
			// 어택 사운드
			//................................................................................	
			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );


			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_HOWLLING_1_ED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ) );
			
			Info.AfterEffect = 	EF_RUGAL_HOWLLING_1_ED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}

}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmWolfCurse()
{
	if(GetSrcAniCount() == 40 )
		SetAfterEffetCurse();
}

//----------------------------------------------------------------------------------------------------------------------
void	CEFRugalSkill::SideEffect()
{
	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );
	
	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	float Alpha = 0.0f;
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	
	g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );
	g_RenderManager.LightEnable( 0, TRUE );
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetLighting( TRUE );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	D3DXVECTOR3 vec = D3DXVECTOR3(m_pSrcChr->GetPosTM()._41 , 
		m_pSrcChr->GetPosTM()._42,
		m_pSrcChr->GetPosTM()._43);
	
	m_pSrcChr->RenderToAlphaEffect( vec , m_Alpha , m_Scale , m_SideEffectColor , FALSE );
	
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.LightEnable( 0, FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateWolfCurse()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.2f )
		m_Scale =1.0f; 
	
	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/20.f);
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::SetAfterEffetCurse()
{
	int Count = m_pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{

			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );

			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_CURSE_1_ED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ) );
			
			Info.AfterEffect = 	EF_RUGAL_CURSE_1_ED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmWolfTree()
{
	if(GetSrcAniCount() == 40 )
		SetAfterEffetTree();
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::SetAfterEffetTree()
{
	int Count = m_pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{

			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );

			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_ILLUSIONTREE_1_ED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ) );
			
			Info.AfterEffect = 	EF_RUGAL_ILLUSIONTREE_1_ED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateWolfTree()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.2f )
		m_Scale =1.0f; 
	
	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/20.f);
	
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmWolfDestory()
{  
	if(GetSrcAniCount() == 38 )
	{
		D3DXVECTOR3 vStartPos   = D3DXVECTOR3( m_pSrcChr->GetPosCenterBoneTM()._41 ,0.0f ,m_pSrcChr->GetPosCenterBoneTM()._43  );
		
		D3DXVECTOR3 vPos = vStartPos + (m_pSrcChr->m_vDir * 20.0f) ;
		vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_ONECUTOFRUIN  , ESLT_POS , 4.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetUnique() 
			, vPos , m_pSrcChr->GetDirection() );
		
		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_ONECUTOFRUINED  , ESLT_POS , 4.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetUnique() 
			, vPos , m_pSrcChr->GetDirection() );

		SetAfterEffetDestory();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateWolfDestory()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.2f )
		m_Scale =1.0f; 

	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/20.f);
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::SetAfterEffetDestory()
{
	int Count = m_pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );

			Info.AfterEffect = 	EF_RUGAL_ONECUTOFRUINED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::RenderWolfNail()
{
	D3DXMATRIX matLocal;	
	
	D3DXMatrixIdentity( &matLocal );
	
	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	float AlphaList[] = {
		0.2f,
			0.3f,
			0.4f,
			0.5f,
			0.6f,
			0.7f	
	};
	
	float Alpha = 0.0f;
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
	
	g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );
	g_RenderManager.LightEnable( 0, TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetLighting( TRUE );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_ADD );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	long lDelAtime = (long)m_pSrcChr->m_nDelVATime;
	if(lDelAtime < 30)
		lDelAtime = 30;	

	float fRatio =  30.0f / (float)lDelAtime;	
	int nCount = (int)(m_AImageList.size() * fRatio);
	
	if(m_AImageList.size() && nCount < 1)
		nCount = 1;
	
	for( int i = 0 ; i < nCount ; ++i )
	{
		m_AImageList[i].fAlpha = AlphaList[i];
		m_pSrcChr->DisplayAfterImage(m_AImageList[i]);
	}
	
	m_pSrcChr->AniRender();
	m_pSrcChr->Display(0, 0, false, FALSE);
	
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.LightEnable( 0, FALSE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmWolfNail()
{
	if(GetSrcAniCount() == 22 )
		SetAfterEffetNail();
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::SetAfterEffetNail()
{
	int Count = m_pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{

			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );

			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_NAIL_OFDECAYED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ) );
			
			Info.AfterEffect = 	EF_RUGAL_NAIL_OFDECAYED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateWolfNail()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 70 ) )
	{
		
		SAImageData sData;
		sData.n_end_frame = m_pSrcChr->m_end_frame;
		sData.n_blend_frame = m_pSrcChr->m_blend_frame;
		sData.n_start_blend_frame = m_pSrcChr->m_start_blend_frame;
		sData.n_ani_frame = m_pSrcChr->m_ani_frame;
		sData.n_ani_number = m_pSrcChr->m_ani_number;
		sData.n_pre_end_frame = m_pSrcChr->m_pre_end_frame;
		sData.n_pre_ani_number = m_pSrcChr->m_pre_ani_number;
		sData.f_middle_time = m_pSrcChr->m_middle_time;
		
		sData.IsAfterWImage = 1;
		
		sData.sPos.x = m_pSrcChr->GetPosTM()._41;
		sData.sPos.y = m_pSrcChr->GetPosTM()._42;
		sData.sPos.z = m_pSrcChr->GetPosTM()._43;
		
		sData.sDiffuse.r = 0.0f;
		sData.sDiffuse.g = 0.0f;
		sData.sDiffuse.b = 0.15f;
		
		m_AImageList.push_front( sData );
		
		if( m_AImageList.size() > 3 )
			m_AImageList.pop_back();
		
		m_NowScaleTime = g_nowTime;
	}	
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateSoltarRecall()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.3f )
		m_Scale =1.0f; 
	
	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/40.f);
	
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateSoltarCyclon()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.3f )
		m_Scale =1.0f; 
	
	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/40.f);
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmSoltarCyclon()
{
	if( GetSrcAniCount() == 32) 
		SetAfterEffetSoltarCyclon();
}

//----------------------------------------------------------------------------------------------------------------------	
void CEFRugalSkill::SetAfterEffetSoltarCyclon()
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( m_pExtraInfo == NULL || m_pSrcChr  == NULL )
		return;

	int Count = m_pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );


			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_CLO_CYCLONED , 0 , 2.6f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ) );
			
			Info.AfterEffect = 	EF_RUGAL_CLO_CYCLONED	;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateTempestDril()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.4f )
		m_Scale =1.0f; 
	
	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/40.f);
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmTempestDril()
{

	if( m_pSrcChr->m_ani_frame == 35 )
	{
		D3DXMATRIX mat;
		m_pSrcChr->GetDirectBoneLinkTM( 15 , mat);	
		
		D3DXVECTOR3 vStartPos   = D3DXVECTOR3( mat._41 ,mat._42 ,mat._43 );
		D3DXVECTOR3 vPos = vStartPos; 
		
		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_DRIL_TEMPEST  , ESLT_POS , 5.1f , 
			m_pSrcChr->GetUnique()  , m_pSrcChr->GetUnique() ,vPos , m_pSrcChr->GetDirection() );
		
		
		SetAfterEffetTempestDril();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::SetAfterEffetTempestDril()
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( m_pExtraInfo == NULL || m_pSrcChr  == NULL )
		return;
	
	int Count = m_pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );
			
			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_DRIL_TEMPESTED , 0 , 4.1f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), 
				::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );					
				
			Info.AfterEffect = 	EF_RUGAL_DRIL_TEMPESTED;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::UpdateTempestHurricane()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_NowScaleTime ) , > , 10 ) )
	{
		m_NowScaleTime = g_nowTime;
		m_Scale += 0.01f;
	}
	
	if(m_Scale > 1.4f )
		m_Scale =1.0f; 
	
	float AlphaA = (m_Scale - 1.0f) * 100.0f;	
	m_Alpha = 1.0f - (AlphaA/40.f);
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::CheckFrmTempestHurricane()
{
	if( m_pSrcChr->m_ani_frame == 66 )
	{
		D3DXMATRIX mat;
		m_pSrcChr->GetDirectBoneLinkTM( 15 , mat);	
		
		D3DXVECTOR3 vStartPos   = D3DXVECTOR3( mat._41 ,mat._42 ,mat._43 );
		D3DXVECTOR3 vPos = vStartPos; //(m_pSrcChr->m_vDir/* * 30.0f*/);
		
		SetAfterEffetTempestHurricane();
	}
}

//----------------------------------------------------------------------------------------------------------------------
void CEFRugalSkill::SetAfterEffetTempestHurricane()
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( m_pExtraInfo == NULL || m_pSrcChr  == NULL )
		return;
	
	int Count = m_pExtraInfo->GetTargetCount();
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			if( m_pExtraInfo != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );
			
			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_DRIL_HURRICANEED , 0 , 4.1f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), 
				::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );					
			
			Info.AfterEffect = 	EF_RUGAL_DRIL_HURRICANEED;
			Info.DestType = 0;
			Info.lifeTime = 2.6f;
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
		}
	}
}

