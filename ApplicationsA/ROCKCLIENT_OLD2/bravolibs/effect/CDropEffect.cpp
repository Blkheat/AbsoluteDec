#include "RockPCH.h"
#include <NETWORK\Protocol.h>
#include "..\\quadlist.h"
#include <BASE\\D3DBASE.H>
#include "..\\RockClient.h"
#include "..\\PC.h"
#include "..\\Camera.h"
#include "..\\Map.h"
#include "ObjBase.H"
#include "..\\bravolibs\\obj\\Character_Item.H"
#include "..\\bravolibs\\obj\\Player_Manager.H"
#include "..\\bravolibs\\obj\\Line_Vertex.H"
#include "CDamageEffect.h"
#include "..\\Effect.h"
#include <stdio.h>
#include <assert.H>
#include "CDropEffect.h"

CDropEffect::CDropEffect()
{
	m_ISStart = TRUE;
	m_pPointer = m_List.begin();

	m_fSRadius = 0.0f;
}

CDropEffect::~CDropEffect()
{
	m_List.clear();
}


///-- Init(중심 X좌표, 중심 Z좌표, 높이, Z축회전각도, 최소낙하시간, 최대낙하시간,
///-- 낙하개수, 효과범위, 현재시간, 러닝타임(간격타임을 구하기위한 용도), 낙하반복여부
int	CDropEffect::Init(float CenterX,float CenterZ,float Height,float Angle,
					  DWORD DropMinTime,DWORD DropMaxTime,int Count,float Radius,DWORD nowTime,
					  DWORD RunningTime,BOOL Loop)
{
	D3DXMATRIX   matRot;
	D3DXVECTOR3  vRad;
	float		 fLandHeight;
	
	///--  떨어지는 간격타임 구하기... 
	DWORD		 RangeTime = ( DWORD ) ( ( RunningTime - DropMinTime - 500 ) / ( float ) Count );

	m_vDir = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
	
	m_ISLoop = Loop;
	
	m_Radius = Radius;
	m_Height = Height;
	
	m_CenterX = CenterX;
	m_CenterZ = CenterZ;

	m_vCenter = D3DXVECTOR3( m_CenterX , 0.0f , m_CenterZ );
	
	if( Radius == 0.0f )
	{
	}
	else
	{
		///-- 효과 범위 반경이 존재하면....
		D3DXMATRIX matRot;

		D3DXMatrixRotationZ( &matRot , Angle ); 

		///-- Up 벡터를 Z축으로 Angle만큼 회전된 방향을 구한다.
		D3DXVec3TransformCoord( &m_vDir , &m_vDir , &matRot );
	}

	for( int i = 0 ; i < Count ; ++i )
	{
		DROP_EFFECT_DATA TempData;

		///-- DROP_EFFECT_DATA가 초기화된 시간
		TempData.OldTime = nowTime;
			
		float fR = Random( -Radius , Radius );		
		vRad = D3DXVECTOR3( fR , 0.0f , 0.0f );
		D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
		///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
		D3DXVec3TransformCoord( &vRad , &vRad , &matRot );

		TempData.State        = DROPST_DELAY;
		///-- 랜덤반경 위치에 시작중심값을 더한다.
		TempData.vStartPos    = vRad + D3DXVECTOR3( CenterX , 0.0f , CenterZ );
		///-- 최종위치를 구한다.
		TempData.vEndPos      = TempData.vStartPos;
		///-- 최종위치의 타일높이값을 구한다.
		fLandHeight           = g_Map.PHF_GetHeight( TempData.vStartPos.x , TempData.vStartPos.z );
		TempData.vStartPos.y += fLandHeight;
		TempData.vEndPos.y	  = fLandHeight;
		//TempData.WaitTime	  = ( DWORD )Random( 0 , ( int ) WaitTime );
		
		///-- DROPST_DROP상태로 변환하기 위한 대기시간...
		TempData.WaitTime     = i * RangeTime;
		///-- 최소낙하타임과 최대낙하타임사이의 랜덤타임을 낙하타임으로 잡는다.
		TempData.DropTime     = ( DWORD )Random( ( int ) DropMinTime , ( int ) DropMaxTime );

		///-- 낙하시작위치를 구한다. 반경위치 + (높이)*방향
		TempData.vStartPos	  = TempData.vStartPos + Height * m_vDir;

		TempData.vPos         = TempData.vStartPos;		

		m_List.push_back( TempData );
	}

	m_pPointer = m_List.begin();

	return TRUE;
}

int	CDropEffect::InitBack(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, float fMinHei,float fMaxHei, 
					  DWORD dMinTime,DWORD dMaxTime,int Count,float fSRadius,
					  float fERadius,DWORD nowTime,DWORD RunningTime,BOOL Loop )
{
	D3DXMATRIX   matRot;
	D3DXVECTOR3  vERad, vSRad;	
	
	///--  떨어지는 간격타임 구하기... 
	DWORD		 RangeTime = ( DWORD ) ( ( RunningTime - dMinTime - 500 ) / ( float ) Count );	
	
	m_ISLoop = Loop;
	
	m_fSRadius = fSRadius;
	m_Radius = fERadius;

	m_fMinHei = fMinHei;
	m_fMaxHei = fMaxHei;

	m_vCenter = vStart;
	m_vStart = vStart;
	m_vEnd = vEnd;
	m_vDir = vEnd - vStart; 

	for( int i = 0 ; i < Count ; ++i )
	{
		DROP_EFFECT_DATA TempData;

		///-- DROP_EFFECT_DATA가 초기화된 시간
		TempData.OldTime = nowTime;
			
		float fR = Random( 0.0f , m_Radius );		
		vERad = D3DXVECTOR3( fR , 0.0f , 0.0f );
		fR = fR * ( m_fSRadius / m_Radius );
		vSRad = D3DXVECTOR3( fR , 0.0f , 0.0f );		
		D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );

		///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
		D3DXVec3TransformCoord( &vERad , &vERad , &matRot );
		D3DXVec3TransformCoord( &vSRad , &vSRad , &matRot );

		float fRndHeight = Random( m_fMinHei , m_fMaxHei );
		vSRad.y = vERad.y = fRndHeight;

		TempData.State        = DROPST_DELAY;
		///-- 랜덤반경 위치에 시작중심값을 더한다.
		TempData.vStartPos    = vSRad + m_vStart;
		///-- 최종위치를 구한다.
		TempData.vEndPos      = vERad + m_vEnd;	

		D3DXVECTOR3 vDistance = TempData.vEndPos - TempData.vStartPos;
		vDistance.y = 0.0f;
		D3DXVec3Normalize(&vDistance, &vDistance);
		float fY = D3DXVec3Dot( &vDistance , &D3DXVECTOR3(0.0f, 0.0f, -1.0f) );
		fY = acos((double)fY);
		if(vDistance.x > 0.0f)
		{
			fY = (2.0f * D3DX_PI) - fY;
		}
		TempData.fYadian = fY;
		
		///-- DROPST_DROP상태로 변환하기 위한 대기시간...
		TempData.WaitTime     = i * RangeTime;
		///-- 최소낙하타임과 최대낙하타임사이의 랜덤타임을 낙하타임으로 잡는다.
		TempData.DropTime     = ( DWORD )Random( ( int ) dMinTime , ( int ) dMaxTime );

		TempData.vPos         = TempData.vStartPos;		

		m_List.push_back( TempData );
	}

	m_pPointer = m_List.begin();

	return TRUE;
}

void CDropEffect::Update(DWORD Time)
{
	///--Time은 g_nowTime이다.	
	std::list	<DROP_EFFECT_DATA>::iterator i = m_List.begin();
	float	Range;
	
	for( ; i != m_List.end() ; )
	{
		switch( i->State )
		{
			// 대기 
			case DROPST_DELAY:
				
				///-- 대기시간이 끝나면 드롭상태로 변한다.
				if( ( Time - i->OldTime ) >= i->WaitTime )
				{
					i->State = DROPST_DROP;
					i->vPos = i->vStartPos;
				}
				
				++i;

				break;

			// 떨어질때
			case DROPST_DROP:
				{
					///-- 낙하상태에서 위치를 선형보간으로 계산 낙하가 끝나면
					///-- DROPST_END로 전환 
					Range = ( float ) ( Time - ( i->WaitTime + i->OldTime ) ) / ( float ) i->DropTime;
			
					if( Range >= 1.0f )
					{
						i->vPos = i->vEndPos;
						
						i->State = DROPST_END;
					}
					else
						D3DXVec3Lerp( &i->vPos , &i->vStartPos , &i->vEndPos , Range );
					
					++i;
				}

				break;

			// 끝
			case DROPST_END:
								
				i->State = DROPST_DESTROY;

				++i;

				break;
			
			// 죽을때
			case DROPST_DESTROY:

				///-- 반복형태가 아니면 지운다.
				if( !m_ISLoop )
				{
					m_List.erase( i++ );
				}
				else
				{
					///-- 반복시에는 반경위치와 낙하시작위치를 다시구한다.
					///-- 딜레이 상태로 전환
					float fR = Random( -m_Radius , m_Radius );

					i->State = DROPST_DELAY;
					i->vPos = i->vStartPos;
					i->OldTime = Time;

					D3DXVECTOR3 vRad = D3DXVECTOR3( fR , 0.0f , 0.0f );
					
					D3DXMATRIX matRot;

					D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
					
					D3DXVec3TransformCoord( &vRad , &vRad , &matRot );
					
					i->vStartPos    = vRad + D3DXVECTOR3( m_CenterX , 0.0f , m_CenterZ );
					i->vEndPos      = i->vStartPos;
					
					float fLandHeight = g_Map.PHF_GetHeight( i->vStartPos.x , i->vStartPos.z );
					
					i->vStartPos.y += fLandHeight;
					i->vEndPos.y    = fLandHeight;
					i->vStartPos    = i->vStartPos + m_Height * m_vDir;
					i->vPos         = i->vStartPos;
				}

				break;
		}
	}
}

void CDropEffect::UpdateBack(DWORD Time)
{
	///--Time은 g_nowTime이다.	
	std::list	<DROP_EFFECT_DATA>::iterator i = m_List.begin();
	float	Range;

	D3DXVECTOR3  vERad, vSRad;
	
	for( ; i != m_List.end() ; )
	{
		switch( i->State )
		{
			// 대기 
			case DROPST_DELAY:
				
				///-- 대기시간이 끝나면 드롭상태로 변한다.
				if( ( Time - i->OldTime ) >= i->WaitTime )
				{
					i->State = DROPST_DROP;
					i->vPos = i->vStartPos;
				}
				
				++i;

				break;

			// 떨어질때
			case DROPST_DROP:
				{
					///-- 낙하상태에서 위치를 선형보간으로 계산 낙하가 끝나면
					///-- DROPST_END로 전환 
					Range = ( float ) ( Time - ( i->WaitTime + i->OldTime ) ) / ( float ) i->DropTime;
			
					if( Range >= 1.0f )
					{
						i->vPos = i->vEndPos;
						
						i->State = DROPST_END;
					}
					else
						D3DXVec3Lerp( &i->vPos , &i->vStartPos , &i->vEndPos , Range );
					
					++i;
				}

				break;

			// 끝
			case DROPST_END:
								
				i->State = DROPST_DESTROY;

				++i;

				break;
			
			// 죽을때
			case DROPST_DESTROY:

				///-- 반복형태가 아니면 지운다.
				if( !m_ISLoop )
				{
					m_List.erase( i++ );
				}
				else
				{
					D3DXMATRIX matRot;
					
					///-- 반복시에는 반경위치와 낙하시작위치를 다시구한다.
					///-- 딜레이 상태로 전환
					float fR = Random( 0.0f , m_Radius );
					vERad = D3DXVECTOR3( fR , 0.0f , 0.0f );
					fR = fR * ( m_fSRadius / m_Radius );
					vSRad = D3DXVECTOR3( fR , 0.0f , 0.0f );		
					D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );

					///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
					D3DXVec3TransformCoord( &vERad , &vERad , &matRot );
					D3DXVec3TransformCoord( &vSRad , &vSRad , &matRot );

					float fRndHeight = Random( m_fMinHei , m_fMaxHei );
					vSRad.y = vERad.y = fRndHeight;

					i->State = DROPST_DELAY;
					i->OldTime = Time;
					
					i->vStartPos    = vSRad + m_vStart;
					i->vEndPos      = vERad + m_vEnd;

					D3DXVECTOR3 vDistance = i->vEndPos - i->vStartPos;
					vDistance.y = 0.0f;
					D3DXVec3Normalize(&vDistance, &vDistance);
					float fY = D3DXVec3Dot( &vDistance , &D3DXVECTOR3(0.0f, 0.0f, -1.0f) );
					fY = acos((double)fY);
					if(vDistance.x > 0.0f)
					{
						fY = (2.0f * D3DX_PI) - fY;
					}
					i->fYadian = fY;								

					i->vPos         = i->vStartPos;

					DROP_EFFECT_DATA p = (*i);
				}

				break;
		}
	}
}

void CDropEffect::StartPosition(void)
{
	m_ISStart = TRUE;
}

DROP_EFFECT_DATA *CDropEffect::GetDropData(void)
{
	static DROP_EFFECT_DATA Data;
	
	if( m_ISStart == TRUE )
	{
		m_pPointer = m_List.begin();

		m_ISStart = FALSE;
	}

	if( m_pPointer != m_List.end() )
	{
		Data.DropTime  = m_pPointer->DropTime;
		Data.OldTime   = m_pPointer->OldTime;
		Data.State     = m_pPointer->State;
		Data.vEndPos   = m_pPointer->vEndPos;
		Data.vPos      = m_pPointer->vPos;
		Data.vStartPos = m_pPointer->vStartPos;
		Data.WaitTime  = m_pPointer->WaitTime;
		Data.fYadian   = m_pPointer->fYadian;
		
		m_pPointer++;
		
		return &Data;
	}

	return NULL;
}
