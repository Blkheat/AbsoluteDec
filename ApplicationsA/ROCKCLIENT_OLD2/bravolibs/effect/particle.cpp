#include "RockPCH.h"

#include	<windows.h>
#include	"BASE\\D3DBASE.H"
//#include	"EffectBase.h"
#include	"Particle.h"
#include	"EffectSystem.h"

#include	"..\\quadlist.h"
#include	<MAP\\field.h>
#include	"..\\map.H"

extern		long	g_nowTime;

CParticle::CParticle()
{
	memset(this, 0, sizeof(CParticle));
		
	// m_pNext = NULL; m_pPrev = NULL;
	m_pParent = NULL;
	m_vDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPrevUpdateTime = g_nowTime;
	m_nDelUpdateTime = 40;

	m_fDelCumtime = 0.0f;
	m_IsUpdate = FALSE;
	m_IsInitUpdate = FALSE;
}

CParticle::~CParticle()
{
	m_pParent = NULL;
}

void CParticle::operator=(const CParticle &Src)
{
	memcpy( this , ( CParticle * ) &Src , sizeof( CParticle ) );
}


void CParticle::Delete()
{
/*
	if (m_pPrev == NULL)
	{
		if (m_pNext == NULL)
		{
			m_pParent->m_pParticleHead = NULL;			
			m_pParent->m_pParticleTail = NULL;
		}
		else
		{
			m_pParent->m_pParticleHead = m_pNext;
			m_pNext->m_pPrev = NULL;
		}
	}
	else
	{ 
		if (m_pNext == NULL)
		{
			m_pParent->m_pParticleTail = m_pPrev;
			m_pPrev->m_pNext = NULL;

		}
		else
		{
			m_pPrev->m_pNext = m_pNext;
			m_pNext->m_pPrev = m_pPrev;
		}
	}
*/
}


BOOL	CParticle::Update(float fTimeDelta)
{
	if( m_fLifetime == 0.0f )
	{	
		m_fLifetime = m_fAge + fTimeDelta;
	}
	
	//...............................................................................................................
	//��ƼŬ�� ���� ������ �Ѿ� ������
	//...............................................................................................................
	if ( m_fAge + fTimeDelta > m_fLifetime )
	{
		//...........................................................................................................
		//��ƼŬ�� ���̸� -1.0f�� ���� ���ο� ���� ������ �غ� �ϰ� �����.
		//...........................................................................................................
		m_fAge	=	-1.0f;
//		Delete();
		return	FALSE;
	}
	//...............................................................................................................
	// ����ִ� ��ƼŬ ó��
	//...............................................................................................................
	else
	{
		//...........................................................................................................
		//��ƼŬ�� ���� �ñⰡ �Ǳ� ������
		//���̸� �԰� �ϰ�
		//�켱 ������ ��ġ�� �� ��ġ�� ���� �Ŀ�
		//������ ��ġ���ٰ� ������ �ӵ��� �ð� ��ȭ ���� �����ְ�
		//�ӵ����ٴ� �߷°��� �����ְ� 
		//�߷°��� ���࿡ ���� �߷� ��ȭ ���� �����־� ��ȭ�� �ش�. 
		//...........................................................................................................
		m_fAge			+=	fTimeDelta;

		if ( m_pParent == NULL )
		{
			return FALSE;
		}

		m_IsUpdate = TRUE;

		if( m_IsInitUpdate )
		{
			if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_nPrevUpdateTime ) , > , m_nDelUpdateTime ) )
			{
				m_nPrevUpdateTime = g_nowTime;
				fTimeDelta += m_fDelCumtime;
				m_fDelCumtime = 0.0f;
			}
			else
			{
				m_fDelCumtime += fTimeDelta;
				m_IsUpdate = FALSE;
				return TRUE;
			}
		}
		else
		{
			m_IsInitUpdate = TRUE;
			m_nPrevUpdateTime = g_nowTime;
			m_IsUpdate = TRUE;
		}
		
		m_vPrevLocation	 =	m_vLocation;
		m_vLocation		+=	m_vVelocity	* fTimeDelta;
		m_vVelocity		+=	m_vGravity  * fTimeDelta;
		m_vVelocity	    +=  m_vDirection * fTimeDelta;		

		//...........................................................................................................
		// �ٴڿ� �ٿ�� �Ǵ� ��ƼŬ�϶�
		//...........................................................................................................
		if ( m_pParent->IsColliding() )
		{
			float height;
			
			height = g_Map.Get__MapHeight(((int)m_vLocation.x)/8,((int)m_vLocation.z)/8);
			
			if( m_vLocation.y < height )
			{
				if( m_vVelocity.y < -0.3f || m_vVelocity.y > 0.3f)
				{
					if( m_pParent->m_vMoveLocation.y < height )
					{
						float penerty;
						if( height - m_pParent->m_vMoveLocation.y > 10.0f )
							penerty = 10.0f;
						else
							penerty = height - m_pParent->m_vMoveLocation.y;
						m_vLocation.y = m_vPrevLocation.y; 
						m_vVelocity.y = -m_vVelocity.y * EFFECT_REFLECT;

						m_vVelocity.x = (-m_vVelocity.x * penerty + m_vVelocity.x * ( 10.0f - penerty )) * EFFECT_REFLECT * 0.1f;
						m_vVelocity.z = (-m_vVelocity.z * penerty + m_vVelocity.z * ( 10.0f - penerty )) * EFFECT_REFLECT * 0.1f;
					}
					else
					{
						m_vLocation.y = m_vPrevLocation.y; 
						m_vVelocity.y = -m_vVelocity.y * EFFECT_REFLECT;
					}
				}
				else
				{
					m_vLocation.y = height;
					m_vVelocity *= 0.1f; 
				}
			}
		}
		

		//...........................................................................................................
		// �������� ���̴� ��ƼŬ�϶�
		//...........................................................................................................
		if ( m_pParent->IsAttractive() )
		{
			D3DXVECTOR3 AttractDir = m_pParent->m_vAttractLocation - m_vLocation; 
			D3DXVECTOR3	NomalDir;
			D3DXVec3Normalize(&NomalDir, &AttractDir);
//			m_vVelocity += NomalDir * m_pParent->m_pReference->m_fAttractPower * fTimeDelta;
			m_vLocation += NomalDir * m_pParent->m_pReference->m_fAttractPower * fTimeDelta * 0.5f;
			m_vVelocity += NomalDir * m_pParent->m_pReference->m_fAttractPower * fTimeDelta;
		}
		
		//...........................................................................................................
		// ���� & ������ & �߷°�
		//...........................................................................................................
		m_Color.r	+= m_ColorDelta.r * fTimeDelta;
		m_Color.g	+= m_ColorDelta.g * fTimeDelta;
		m_Color.b	+= m_ColorDelta.b * fTimeDelta;

		m_fAlpha	+= m_fAlphaDelta * fTimeDelta;

		m_fSize		+= m_fSizeDelta * fTimeDelta;
	
		if( m_fSize < 0.0f )
			return FALSE;

		m_vGravity	+= m_vGravityDelta * fTimeDelta;
	}

	return	TRUE;
}