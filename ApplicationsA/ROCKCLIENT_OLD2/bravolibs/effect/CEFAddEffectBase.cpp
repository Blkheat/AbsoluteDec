#include "CEFAddEffectBase.h"
#include "..\\..\\Effect.h"
#include "..\\..\\CRenderManager.h"

using namespace	std;

extern		long	g_nowTime;

list   <ADD_EFFECT_DATA>	CEFAddEffectBase::m_AddEffectSetList;

int	CEFAddEffectBase::InitDevice(void)
{
	ADD_EFFECT_DATA TempData;
	// 이동불가
	TempData.SetData( D3DCOLOR_XRGB( 255, 100, 0 ) , EF_MOVE_OBSTRUCTION );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 마비
	TempData.SetData( D3DCOLOR_XRGB( 0, 150, 255 ) , EF_PARALYSYS );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 기절
	TempData.SetData( D3DCOLOR_XRGB( 0, 150, 255 ) , EF_FAINT );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 빙결
	TempData.SetData( D3DCOLOR_XRGB( 200, 255, 200 ) , EF_FREEZING );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 석화
	TempData.SetData( D3DCOLOR_XRGB( 180, 180, 180 ) , EF_STONY );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 중독
	TempData.SetData( D3DCOLOR_XRGB( 0, 180, 100 ) , EF_POISONING );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 슬립
	TempData.SetData( D3DCOLOR_XRGB( 255 , 255 , 255 ) , EF_SLEEP );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 스킬불가	
	TempData.SetData( D3DCOLOR_XRGB( 255, 50 , 50 ) , EF_SKILL_DISABLE );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 콤보불가
	TempData.SetData( D3DCOLOR_XRGB( 50 , 50 , 255 ) , EF_COMBO_OBSTRUCTION );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 도발
	TempData.SetData( D3DCOLOR_XRGB( 255 , 255 , 255 ) , EF_PROVOCATION );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 저주
	TempData.SetData( D3DCOLOR_XRGB( 60, 60, 80 ) , EF_ADD_CURSE );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );
	// 상승
	TempData.SetData( D3DCOLOR_XRGB( 255, 255, 0 ) , EF_ADD_UP );
	CEFAddEffectBase::m_AddEffectSetList.push_back( TempData );

	return TRUE;
}

int	CEFAddEffectBase::DeleteDevice(void)
{
	return TRUE;
}

CEFAddEffectBase::CEFAddEffectBase()
{
	m_NowEffect = -1;
}

CEFAddEffectBase::~CEFAddEffectBase()
{
	DeleteEffect( m_NowEffect );

	m_NowEffect = -1;
}


ADD_EFFECT_DATA	*CEFAddEffectBase::GetSetData(int Num)
{
	std::list<ADD_EFFECT_DATA>::iterator i = CEFAddEffectBase::m_AddEffectSetList.begin();
	
	for( ; i != CEFAddEffectBase::m_AddEffectSetList.end(); ++i )
	{
		if( i->EffectSetNum == Num )
		{
			return (ADD_EFFECT_DATA *)&(*i);
		}
	}

	return NULL;
}

int	CEFAddEffectBase::InsertAddSkill(int EffectCode)
{
	std::list <int>::iterator	i = m_AddEffectList.begin();
		
	// 같은거 검사
	for( ; i != m_AddEffectList.end() ; ++i )
	{
		if( (*i) == EffectCode )
			return FALSE;
	}

	m_AddEffectList.push_back( EffectCode );
	SetEffect( EffectCode );

	return TRUE;
}

int CEFAddEffectBase::DeleteAddSkill(int EffectCode)
{
	std::list <int>::iterator	i = m_AddEffectList.begin();

	for( ; i != m_AddEffectList.end() ;  )
	{
		if( (*i) == EffectCode )
		{
			if( EffectCode == m_NowEffect )	
			{
				DeleteEffect( m_NowEffect );

				m_NowEffect = -1;
			}

			m_AddEffectList.erase( i++ );
	
			break;
		}
		else
		{
			i++;
		}
	}

	if( m_AddEffectList.size() <= 0 )
	{
		SetDestroy();
	}
	else
	{
		int Count = 0;

		i = m_AddEffectList.begin();
	
		for( ; i != m_AddEffectList.end() ; ++i )
		{
			Count++;

			if( Count == m_AddEffectList.size() )
			{
				SetEffect( *i );
			
				break;
			}
		}
	}

	return TRUE;
}

int CEFAddEffectBase::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return FALSE;
	}

	return TRUE;
}

int CEFAddEffectBase::SetEffect(int Num)
{
	DeleteEffect( m_NowEffect );

	m_NowEffect = Num;

	ADD_EFFECT_DATA	*pData = GetSetData( Num );
	
	if( pData != NULL )
	{
		g_Particle.SetEffSetCon( 0.0f , Num , ESLT_CHARPOS , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

		m_pSrcChr->m_EffectColor	 = pData->Color;		// 이펙트 걸렸을때 색상값
		m_pSrcChr->m_ISEffectColor	 = TRUE;				// 이펙트 칼라값 적용
	}

	return TRUE;
}

int CEFAddEffectBase::DeleteEffect(int Num)
{
	if( Num != -1 )
	{
		g_Particle.DelEffSetCon( Num , ESLT_CHARPOS , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			
		// 색상
		m_pSrcChr->m_EffectColor	 = D3DCOLOR_XRGB( 0 , 0 , 0 );				// 이펙트 걸렸을때 색상값
		m_pSrcChr->m_ISEffectColor	 = FALSE;									// 이펙트 칼라값 적용
	}

	return TRUE;
}
