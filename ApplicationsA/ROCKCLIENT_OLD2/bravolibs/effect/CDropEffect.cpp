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


///-- Init(�߽� X��ǥ, �߽� Z��ǥ, ����, Z��ȸ������, �ּҳ��Ͻð�, �ִ볫�Ͻð�,
///-- ���ϰ���, ȿ������, ����ð�, ����Ÿ��(����Ÿ���� ���ϱ����� �뵵), ���Ϲݺ�����
int	CDropEffect::Init(float CenterX,float CenterZ,float Height,float Angle,
					  DWORD DropMinTime,DWORD DropMaxTime,int Count,float Radius,DWORD nowTime,
					  DWORD RunningTime,BOOL Loop)
{
	D3DXMATRIX   matRot;
	D3DXVECTOR3  vRad;
	float		 fLandHeight;
	
	///--  �������� ����Ÿ�� ���ϱ�... 
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
		///-- ȿ�� ���� �ݰ��� �����ϸ�....
		D3DXMATRIX matRot;

		D3DXMatrixRotationZ( &matRot , Angle ); 

		///-- Up ���͸� Z������ Angle��ŭ ȸ���� ������ ���Ѵ�.
		D3DXVec3TransformCoord( &m_vDir , &m_vDir , &matRot );
	}

	for( int i = 0 ; i < Count ; ++i )
	{
		DROP_EFFECT_DATA TempData;

		///-- DROP_EFFECT_DATA�� �ʱ�ȭ�� �ð�
		TempData.OldTime = nowTime;
			
		float fR = Random( -Radius , Radius );		
		vRad = D3DXVECTOR3( fR , 0.0f , 0.0f );
		D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
		///-- �ݰ�Radius �̳��� 360 ȸ���� ��ġ�� ���Ѵ�.	
		D3DXVec3TransformCoord( &vRad , &vRad , &matRot );

		TempData.State        = DROPST_DELAY;
		///-- �����ݰ� ��ġ�� �����߽ɰ��� ���Ѵ�.
		TempData.vStartPos    = vRad + D3DXVECTOR3( CenterX , 0.0f , CenterZ );
		///-- ������ġ�� ���Ѵ�.
		TempData.vEndPos      = TempData.vStartPos;
		///-- ������ġ�� Ÿ�ϳ��̰��� ���Ѵ�.
		fLandHeight           = g_Map.PHF_GetHeight( TempData.vStartPos.x , TempData.vStartPos.z );
		TempData.vStartPos.y += fLandHeight;
		TempData.vEndPos.y	  = fLandHeight;
		//TempData.WaitTime	  = ( DWORD )Random( 0 , ( int ) WaitTime );
		
		///-- DROPST_DROP���·� ��ȯ�ϱ� ���� ���ð�...
		TempData.WaitTime     = i * RangeTime;
		///-- �ּҳ���Ÿ�Ӱ� �ִ볫��Ÿ�ӻ����� ����Ÿ���� ����Ÿ������ ��´�.
		TempData.DropTime     = ( DWORD )Random( ( int ) DropMinTime , ( int ) DropMaxTime );

		///-- ���Ͻ�����ġ�� ���Ѵ�. �ݰ���ġ + (����)*����
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
	
	///--  �������� ����Ÿ�� ���ϱ�... 
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

		///-- DROP_EFFECT_DATA�� �ʱ�ȭ�� �ð�
		TempData.OldTime = nowTime;
			
		float fR = Random( 0.0f , m_Radius );		
		vERad = D3DXVECTOR3( fR , 0.0f , 0.0f );
		fR = fR * ( m_fSRadius / m_Radius );
		vSRad = D3DXVECTOR3( fR , 0.0f , 0.0f );		
		D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );

		///-- �ݰ�Radius �̳��� 360 ȸ���� ��ġ�� ���Ѵ�.	
		D3DXVec3TransformCoord( &vERad , &vERad , &matRot );
		D3DXVec3TransformCoord( &vSRad , &vSRad , &matRot );

		float fRndHeight = Random( m_fMinHei , m_fMaxHei );
		vSRad.y = vERad.y = fRndHeight;

		TempData.State        = DROPST_DELAY;
		///-- �����ݰ� ��ġ�� �����߽ɰ��� ���Ѵ�.
		TempData.vStartPos    = vSRad + m_vStart;
		///-- ������ġ�� ���Ѵ�.
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
		
		///-- DROPST_DROP���·� ��ȯ�ϱ� ���� ���ð�...
		TempData.WaitTime     = i * RangeTime;
		///-- �ּҳ���Ÿ�Ӱ� �ִ볫��Ÿ�ӻ����� ����Ÿ���� ����Ÿ������ ��´�.
		TempData.DropTime     = ( DWORD )Random( ( int ) dMinTime , ( int ) dMaxTime );

		TempData.vPos         = TempData.vStartPos;		

		m_List.push_back( TempData );
	}

	m_pPointer = m_List.begin();

	return TRUE;
}

void CDropEffect::Update(DWORD Time)
{
	///--Time�� g_nowTime�̴�.	
	std::list	<DROP_EFFECT_DATA>::iterator i = m_List.begin();
	float	Range;
	
	for( ; i != m_List.end() ; )
	{
		switch( i->State )
		{
			// ��� 
			case DROPST_DELAY:
				
				///-- ���ð��� ������ ��ӻ��·� ���Ѵ�.
				if( ( Time - i->OldTime ) >= i->WaitTime )
				{
					i->State = DROPST_DROP;
					i->vPos = i->vStartPos;
				}
				
				++i;

				break;

			// ��������
			case DROPST_DROP:
				{
					///-- ���ϻ��¿��� ��ġ�� ������������ ��� ���ϰ� ������
					///-- DROPST_END�� ��ȯ 
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

			// ��
			case DROPST_END:
								
				i->State = DROPST_DESTROY;

				++i;

				break;
			
			// ������
			case DROPST_DESTROY:

				///-- �ݺ����°� �ƴϸ� �����.
				if( !m_ISLoop )
				{
					m_List.erase( i++ );
				}
				else
				{
					///-- �ݺ��ÿ��� �ݰ���ġ�� ���Ͻ�����ġ�� �ٽñ��Ѵ�.
					///-- ������ ���·� ��ȯ
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
	///--Time�� g_nowTime�̴�.	
	std::list	<DROP_EFFECT_DATA>::iterator i = m_List.begin();
	float	Range;

	D3DXVECTOR3  vERad, vSRad;
	
	for( ; i != m_List.end() ; )
	{
		switch( i->State )
		{
			// ��� 
			case DROPST_DELAY:
				
				///-- ���ð��� ������ ��ӻ��·� ���Ѵ�.
				if( ( Time - i->OldTime ) >= i->WaitTime )
				{
					i->State = DROPST_DROP;
					i->vPos = i->vStartPos;
				}
				
				++i;

				break;

			// ��������
			case DROPST_DROP:
				{
					///-- ���ϻ��¿��� ��ġ�� ������������ ��� ���ϰ� ������
					///-- DROPST_END�� ��ȯ 
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

			// ��
			case DROPST_END:
								
				i->State = DROPST_DESTROY;

				++i;

				break;
			
			// ������
			case DROPST_DESTROY:

				///-- �ݺ����°� �ƴϸ� �����.
				if( !m_ISLoop )
				{
					m_List.erase( i++ );
				}
				else
				{
					D3DXMATRIX matRot;
					
					///-- �ݺ��ÿ��� �ݰ���ġ�� ���Ͻ�����ġ�� �ٽñ��Ѵ�.
					///-- ������ ���·� ��ȯ
					float fR = Random( 0.0f , m_Radius );
					vERad = D3DXVECTOR3( fR , 0.0f , 0.0f );
					fR = fR * ( m_fSRadius / m_Radius );
					vSRad = D3DXVECTOR3( fR , 0.0f , 0.0f );		
					D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );

					///-- �ݰ�Radius �̳��� 360 ȸ���� ��ġ�� ���Ѵ�.	
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
