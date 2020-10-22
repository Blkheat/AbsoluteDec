#include "RockPCH.h"
#include "VFileSystem.h"

#include "CEFWing0.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include <SOUND\Sound.H>
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include "..\\RockClient.h"
#include "..\\quadlist.h"
#include <Map\\Field.h>
#include "..\\Map.H"
#include "..\\PC.H"
#include "..\\Effect.H"
#include "..\\Camera.H"
#include "..\\RockClient.h"
#include "CEFEldaWeaponItem.h"

#include <vector>

float fMove = 0.0f;

int SortDTimeListElda( const void *arg1, const void *arg2 )
{
	DWORD * a1 = ( DWORD * ) arg1;
	DWORD * a2 = ( DWORD * ) arg2;

	if( *a1 == *a2 )
	{
		return 0;
	}
	else if( *a1 > *a2 )
	{
		return 1;
	}
	else
	{
		return -1;
	}

	return 0;
}

CEFEldaWeaponItem::CEFEldaWeaponItem()
{
	m_dInitTime = g_nowTime;
	m_dPrevTime = g_nowTime;

	m_IsInit = FALSE;
	m_IsOneRender = FALSE;

	m_IsAfterSortRender = TRUE;

	m_fScaleSizeStart = 0.0f; 
	m_fScaleSizeEnd = 0.0f ;
	tx2 = ty2 = tz2 = tx = ty = tz = 0.0f;
	

}

CEFEldaWeaponItem::~CEFEldaWeaponItem()
{
	if( g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		m_pSrcChr->m_State.nWeaponLightType = nWeaponLightNone;
	}
}

void CEFEldaWeaponItem::SetDestroy(BOOL ISDamageAct, BOOL ISBasicAttack)
{
	CEffectBase::SetDestroy( ISDamageAct, ISBasicAttack );
}

void CEFEldaWeaponItem::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
		return;
	}

	if( ( m_pData[0] = g_Particle.m_ParticleData.GetKeyDataPointer( 24671 ) ) == NULL )
	{
		SetDestroy();
		return;
	}

	m_dInitTime = g_nowTime;
	m_dPrevTime = g_nowTime;
	
	switch(m_Level)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		SetDestroy();
		return;
	}

	//모델링 만들기 .. 
	
	
	switch(m_ID)
	{	
	case EF_BASE_ITEM_EFFECT_PROMINAS:
		{
			//m_fScaleSize = 1.3f;
			//m_fScaleSizeEnd	  = 1.7f 
			m_fScaleSizeStart = 1.3f;
			m_fScaleSizeEnd	  = 1.7f ;
		}
		break; 	
	case EF_BASE_ITEM_EFFECT_PSYCHE:
		{
			m_fScaleSizeStart = 1.5f;
			m_fScaleSizeEnd = 2.1f;
		}
		break;
	case EF_BASE_ITEM_EFFECT_MERCENARY:	
	case EF_BASE_ITEM_EFFECT_KNIGHT:
	case EF_BASE_ITEM_EFFECT_SNIPER:
	case EF_BASE_ITEM_EFFECT_SLAYER:
	case EF_BASE_ITEM_EFFECT_PRIEST:
	case EF_BASE_ITEM_EFFECT_HOLY:
		{
			m_fScaleSizeStart = 1.3f;
			m_fScaleSizeEnd = 1.5f;
		}
		break; 
		
	}

	m_IsInit = FALSE;
}




int	CEFEldaWeaponItem::SortRender(float dtime)
{
	m_pSrcChr->m_State.nWeaponLightType = (UINT)m_ID;
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
		return TRUE;
	}	

	if( m_pSrcChr->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY || !m_pSrcChr->m_bIsVisible )
	{
		return TRUE;
	}
	
	if( !m_pSrcChr->m_IsAllLTexture )
	{
		return TRUE;
	}

	if( m_pSrcChr == g_Pc.GetPlayer() && !( g_Pc.GetPlayer()->IsMyPlayerRender ) )
	{
		return TRUE;		
	}
	
	if( g_Particle.m_EffectManager.ISOverLength( m_pSrcChr, 240.0f ) )
	{
		return TRUE;
	}
	//아이템에서 밝아지는 것 처리 .. 
	if( !m_IsInit )
	{
		if( !m_IsOneRender )
		{
			m_dInitTime = g_nowTime;
			m_dPrevTime = g_nowTime;
			
			m_IsOneRender = TRUE;
			
			return TRUE;
		}		

		int nGapTime = 333;
		
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dPrevTime ) , < , nGapTime ) )
		{
			return TRUE;		
		}		
		
		m_dInitTime = g_nowTime;
		m_dPrevTime = g_nowTime;
		
		m_IsInit = TRUE;
		
		return TRUE;
	}

	int nNode = 0;

	if( !m_pSrcChr->DrawWeaponCheckStatus() )
	{
		return TRUE;
	}

	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dLighting = g_RenderManager.GetLighting();
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dFVF = g_RenderManager.GetFVF();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend(); 
	DWORD dZFunc = g_RenderManager.GetZFunc();	
	DWORD dAlphaTestEnable = g_RenderManager.GetAlphaTestEnable();

	g_RenderManager.SetZWriteEnable( FALSE );
	//g_RenderManager.SetZWriteEnable( TRUE );
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
	g_RenderManager.LightEnable( 0 , TRUE );
	g_RenderManager.LightEnable( 1 , TRUE );
	g_RenderManager.SetLighting( TRUE );

	g_RenderManager.SetAlphaTestEnable(FALSE);
	
	D3DXMATRIX matWorld;
	
	D3DXMATRIX matTrans, matTemp;
	D3DXMatrixIdentity( &matTrans );							
	D3DXMATRIX matScale;
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 preMaterial;
#else
	D3DMATERIAL8 preMaterial;
#endif // DIRECT_VERSION_9_MJH	
	g_RenderManager.GetMaterial( &preMaterial );
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 TempMaterial = m_pSrcChr->m_pMaterial[0];
#else
	D3DMATERIAL8 TempMaterial = m_pSrcChr->m_pMaterial[0];
#endif // DIRECT_VERSION_9_MJH

	vector < DWORD > aTimeList;

	DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );

	DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
	DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
	DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9 tem_Light1, tem_Light2;
#else
	D3DLIGHT8 tem_Light1, tem_Light2;
#endif // DIRECT_VERSION_9_MJH

	g_RenderManager.GetLight( 0, &tem_Light1 );
				
	switch( m_ID )
	{
		case EF_BASE_ITEM_EFFECT_PROMINAS:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
				tem_Light2.Ambient.r = tem_Light2.Diffuse.r = 0.3f;
				tem_Light2.Ambient.g = tem_Light2.Diffuse.g = 0.3f; 
				tem_Light2.Ambient.b = tem_Light2.Diffuse.b = 1.0f;
			}
			break;
		case EF_BASE_ITEM_EFFECT_PSYCHE:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
			}
			break;
		case EF_BASE_ITEM_EFFECT_MERCENARY:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
				tem_Light2.Ambient.r = tem_Light2.Diffuse.r = 1.0f;
				tem_Light2.Ambient.g = tem_Light2.Diffuse.g = 0.5f; 
				tem_Light2.Ambient.b = tem_Light2.Diffuse.b = 0.5f;		
			}
			break;
		case EF_BASE_ITEM_EFFECT_KNIGHT:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
				tem_Light2.Ambient.r = tem_Light2.Diffuse.r = 0.3f;
				tem_Light2.Ambient.g = tem_Light2.Diffuse.g = 0.3f; 
				tem_Light2.Ambient.b = tem_Light2.Diffuse.b = 1.0f;
			}
			break;
		case EF_BASE_ITEM_EFFECT_SNIPER:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
				tem_Light2.Ambient.r = tem_Light2.Diffuse.r = 1.0f;
				tem_Light2.Ambient.g = tem_Light2.Diffuse.g = 0.3f; 
				tem_Light2.Ambient.b = tem_Light2.Diffuse.b = 0.3f;
			}
			break;
		case EF_BASE_ITEM_EFFECT_SLAYER:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
				tem_Light2.Ambient.r = tem_Light2.Diffuse.r = 1.0f;
				tem_Light2.Ambient.g = tem_Light2.Diffuse.g = 0.3f; 
				tem_Light2.Ambient.b = tem_Light2.Diffuse.b = 0.3f;
			}
			break;
		case EF_BASE_ITEM_EFFECT_PRIEST:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
				tem_Light2.Ambient.r = tem_Light2.Diffuse.r = 0.3f;
				tem_Light2.Ambient.g = tem_Light2.Diffuse.g = 0.3f; 
				tem_Light2.Ambient.b = tem_Light2.Diffuse.b = 1.0f;
			}
			break;
		case EF_BASE_ITEM_EFFECT_HOLY:
			{
				tem_Light2 = g_Pc_Manager.theCharacter_Light;
				tem_Light2.Ambient.r = tem_Light2.Diffuse.r = 0.3f;
				tem_Light2.Ambient.g = tem_Light2.Diffuse.g = 0.3f; 
				tem_Light2.Ambient.b = tem_Light2.Diffuse.b = 1.0f;
			}
			break;
	}

	g_RenderManager.SetLight( 0, &tem_Light2 );
	g_RenderManager.LightEnable( 0 , TRUE );	

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );	

	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetLighting( TRUE );

	DWORD dAlphaRef = g_RenderManager.GetAlphaRef();
	DWORD dAlphaFunc = g_RenderManager.GetAlphaFunc();	

	for( int i = 0; i < LINK_ITEM_COUNT; ++i )
	{
		// 화살통이 있는지 검사하기			
		int itm_index = m_pSrcChr->m_pUserInfo->ArmorInfo.index[i];
		if( itm_index != -1 )
		{
			// 모델, 텍스쳐 인덱스				
			int mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
			int tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;
			
			if( m_pSrcChr->m_curt_event.type == SM_ACT)
			{
				if( !m_pSrcChr->m_curt_event.weaponView[i] )
				{
					continue;
				}
			}

			// 월드메트릭스설정
			switch( i )
			{				
				case 0:	// 오른손
					matWorld = m_pSrcChr->GetRHandTM();
					break;				

				case 1:	// 왼손
					matWorld = m_pSrcChr->GetLHandTM();
					break;					
			}

			///-- 무기류는 기본적으로 알파테스팅을 뺀다.
			g_RenderManager.SetAlphaTestEnable(FALSE);
			
			BOOL IsCrystalBall = FALSE;
			
			///-- 예외적으로 수정구는 알파 테스팅을 적용한다.
			if( i == 0 )
			{
				int RHand_index = m_pSrcChr->m_pUserInfo->ArmorInfo.index[0];
				if( RHand_index != -1 ) 	
				{
					if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 )  // 수정구
					{
						IsCrystalBall = TRUE;	
					}			
				}
			}

			switch( m_ID )
			{
				case EF_BASE_ITEM_EFFECT_PROMINAS:
				case EF_BASE_ITEM_EFFECT_KNIGHT:
				case EF_BASE_ITEM_EFFECT_SNIPER:
					{
						///-- 마법서, 방패 - 왼손장착 아이템이 아니면..
						if( i != 1 )
						{
							continue;	
						}
				
					}
					break;
				case EF_BASE_ITEM_EFFECT_PSYCHE:
				case EF_BASE_ITEM_EFFECT_MERCENARY:
				case EF_BASE_ITEM_EFFECT_SLAYER:
				case EF_BASE_ITEM_EFFECT_PRIEST:
				case EF_BASE_ITEM_EFFECT_HOLY:
					{
						///-- 수정구, 한손검 - 오른손 장착 아이템이 아니면..
						if( i != 0 )
						{
							continue;	
						}
					}
					break;		
			}	
			
			//-----------------------------------------------------------------------------
			// 텍스쳐 있는지 검사
			//-----------------------------------------------------------------------------
			if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( 1, tex_index ) == true )
			{					
				for( int j = 0; j < m_AImageList.size() ; j++ )
				{
					DWORD dNowTime = ( ( g_nowTime - m_dInitTime ) + m_AImageList[j].dSetTime ) % 1000;
					aTimeList.push_back( dNowTime );
				}
	
				qsort( (void *) &aTimeList[0], (size_t) aTimeList.size(), sizeof( DWORD ), SortDTimeListElda );				
				
				for(int j = 0; j < aTimeList.size() ; j++ )
				{					
					DWORD dNowTime = aTimeList[j];

					float fScale = 1.0f;
					float fMaterialA = 0.0f;

					if( dNowTime < 500 )
					{						
						switch( m_ID )
						{
							case EF_BASE_ITEM_EFFECT_PROMINAS:
								{
									fScale = m_fScaleSizeStart + (float)( 0.4f * ( dNowTime / 500.0f ) );
									fMaterialA = (float)( 0.3f * ( dNowTime / 500.0f ) );
								}
								break;
							case EF_BASE_ITEM_EFFECT_PSYCHE:
								{
									fScale = m_fScaleSizeStart + (float)( 0.6f * ( dNowTime / 500.0f ) );
									fMaterialA = (float)( 0.2f * ( dNowTime / 500.0f ) );
								}
								break;
							case EF_BASE_ITEM_EFFECT_MERCENARY:	
							case EF_BASE_ITEM_EFFECT_KNIGHT:
							case EF_BASE_ITEM_EFFECT_SNIPER:
							case EF_BASE_ITEM_EFFECT_SLAYER:
							case EF_BASE_ITEM_EFFECT_PRIEST:
							case EF_BASE_ITEM_EFFECT_HOLY:
								{
									fScale = m_fScaleSizeStart + (float)( 0.2f * ( dNowTime / 500.0f ) );
									fMaterialA = (float)( 0.3f * ( dNowTime / 500.0f ) );
								}
								break;
						}			
					}	
					else
					{
						dNowTime -= 500;
						
						switch( m_ID )
						{
							case EF_BASE_ITEM_EFFECT_PROMINAS:
								{
									fScale =m_fScaleSizeEnd+ (float)( 0.4f * ( dNowTime / 500.0f ) );
									fMaterialA = 0.3f - (float)( 0.3f * ( dNowTime / 500.0f ) );
								}
								break;
							case EF_BASE_ITEM_EFFECT_PSYCHE:
								{
									fScale = m_fScaleSizeEnd + (float)( 0.6f * ( dNowTime / 500.0f ) );
									fMaterialA = 0.2f - (float)( 0.2f * ( dNowTime / 500.0f ) );
								}
								break;
							case EF_BASE_ITEM_EFFECT_MERCENARY:	
							case EF_BASE_ITEM_EFFECT_KNIGHT:
							case EF_BASE_ITEM_EFFECT_SNIPER:
							case EF_BASE_ITEM_EFFECT_SLAYER:
							case EF_BASE_ITEM_EFFECT_PRIEST:
							case EF_BASE_ITEM_EFFECT_HOLY:
								{
									fScale = m_fScaleSizeEnd + (float)( 0.2f * ( dNowTime / 500.0f ) );
									fMaterialA = 0.3f - (float)( 0.3f * ( dNowTime / 500.0f ) );
								}
								break;
						}				
					}
					
					g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[itm_index]->m_pNormalVB, sizeof(OBJECTVERTEX) );
					g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );

					D3DXMATRIX matTempWorld;					
					D3DXMatrixIdentity( &matTempWorld );
					D3DXMatrixIdentity( &matScale );
					D3DXMatrixIdentity( &matTrans );					

					switch( m_ID )
					{
						case EF_BASE_ITEM_EFFECT_PROMINAS:
							{
								D3DXMatrixScaling( &matScale, fScale, fScale, fScale );					
								
								TempMaterial.Diffuse.r = 0.3f;
								TempMaterial.Diffuse.g = 0.3f;
								TempMaterial.Diffuse.b = 1.0f;
								TempMaterial.Ambient.r = 0.3f;
								TempMaterial.Ambient.g = 0.3f;
								TempMaterial.Ambient.b = 1.0f;
								TempMaterial.Diffuse.a = fMaterialA;
								TempMaterial.Ambient.a = fMaterialA;
								D3DXMatrixTranslation( &matTrans, -0.1f, -0.2f, 1.5f );
								D3DXMatrixMultiply( &matScale, &matTrans, &matScale );
								D3DXMatrixTranslation( &matTrans, 0.1f, 0.2f, -1.5f );
							}
							break;
						case EF_BASE_ITEM_EFFECT_PSYCHE:
							{
								D3DXMatrixScaling( &matScale, fScale, fScale, fScale );
								
								TempMaterial.Diffuse.a = fMaterialA;
								D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 0.0f );
							}
							break;
						case EF_BASE_ITEM_EFFECT_MERCENARY:	
							{
								D3DXMatrixScaling( &matScale, fScale + 0.5f , fScale + 0.5f , fScale );
								
								TempMaterial.Diffuse.r = 1.0f;
								TempMaterial.Diffuse.g = 0.5f;
								TempMaterial.Diffuse.b = 0.5f;
								TempMaterial.Ambient.r = 1.0f;
								TempMaterial.Ambient.g = 0.5f;
								TempMaterial.Ambient.b = 0.5f;
								TempMaterial.Diffuse.a = fMaterialA;
								TempMaterial.Ambient.a = fMaterialA;
								//D3DXMatrixTranslation( &matTrans, 0.3f, 0.5f, 0.5f );
						//		D3DXMatrixTranslation( &matTrans, 0.2f, 0.2f, 0.0f );
						//		D3DXMatrixMultiply( &matScale, &matScale, &matTrans  );
						//		D3DXMatrixTranslation( &matTrans, -0.2f, 0.0f, 0.0f );

								D3DXMatrixMultiply( &matScale, &matTrans, &matScale  );
								//D3DXMatrixTranslation( &matTrans,0.3f, 0.5f, 0.0f );
								D3DXMatrixTranslation( &matTrans,0.1f,0.3f,tz2);
							}
							break;
						case EF_BASE_ITEM_EFFECT_KNIGHT:
							{
								D3DXMatrixScaling( &matScale, fScale + 0.5f , fScale + 0.5f , fScale + 0.5f );
								
								TempMaterial.Diffuse.r = 0.3f;
								TempMaterial.Diffuse.g = 0.3f;
								TempMaterial.Diffuse.b = 1.0f;
								TempMaterial.Ambient.r = 0.3f;
								TempMaterial.Ambient.g = 0.3f;
								TempMaterial.Ambient.b = 1.0f;
								TempMaterial.Diffuse.a = fMaterialA;
								TempMaterial.Ambient.a = fMaterialA;
								D3DXMatrixTranslation( &matTrans, -1.2f, 1.5f, 0.0f );
								D3DXMatrixMultiply( &matScale, &matTrans, &matScale );
								D3DXMatrixTranslation( &matTrans, 1.2f, -1.5f, 0.0f );
							}
							break;	
						case EF_BASE_ITEM_EFFECT_SNIPER:
							{
								D3DXMatrixScaling( &matScale, fScale + 0.3f , fScale + 0.3f , fScale + 0.3f );
								
								TempMaterial.Diffuse.r = 1.0f;
								TempMaterial.Diffuse.g = 0.3f;
								TempMaterial.Diffuse.b = 0.3f;
								TempMaterial.Ambient.r = 1.0f;
								TempMaterial.Ambient.g = 0.3f;
								TempMaterial.Ambient.b = 0.3f;
								TempMaterial.Diffuse.a = fMaterialA;
								TempMaterial.Ambient.a = fMaterialA;
								//D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, tz );
								D3DXMatrixMultiply( &matScale, &matScale, &matTrans  );
								D3DXMatrixTranslation( &matTrans, -0.9f, 1.0f, 0.0f );
						
							}
							break;
						case EF_BASE_ITEM_EFFECT_SLAYER:
							{
								D3DXMatrixScaling( &matScale, fScale + 1.5f , fScale + 1.5f , fScale );
								
								TempMaterial.Diffuse.r = 1.0f;
								TempMaterial.Diffuse.g = 0.3f;
								TempMaterial.Diffuse.b = 0.3f;
								TempMaterial.Ambient.r = 1.0f;
								TempMaterial.Ambient.g = 0.3f;
								TempMaterial.Ambient.b = 0.3f;
								TempMaterial.Diffuse.a = fMaterialA;
								TempMaterial.Ambient.a = fMaterialA;

								//tx2 = ty2 = tz2 = tx = ty = tz
								
								//D3DXMatrixTranslation( &matTrans,0.0f, ty, tz );	
								//D3DXMatrixTranslation( &matTrans, 0.2f, 0.2f, 0.0f );
								D3DXMatrixMultiply( &matScale, &matTrans, &matScale  );
								//D3DXMatrixTranslation( &matTrans,0.3f, 0.5f, 0.0f );
								D3DXMatrixTranslation( &matTrans,0.15f,0.5f, 0.0f);
						
								
							}
							break;
						case EF_BASE_ITEM_EFFECT_PRIEST:
							{
								D3DXMatrixScaling( &matScale, fScale + 0.5f , fScale + 0.5f , fScale );
								
								TempMaterial.Diffuse.r = 0.3f;
								TempMaterial.Diffuse.g = 0.3f;
								TempMaterial.Diffuse.b = 1.0f;
								TempMaterial.Ambient.r = 0.3f;
								TempMaterial.Ambient.g = 0.3f;
								TempMaterial.Ambient.b = 1.0f;
								TempMaterial.Diffuse.a = fMaterialA;
								TempMaterial.Ambient.a = fMaterialA;
								D3DXMatrixTranslation( &matTrans, -0.2f, 0.1f, fMove );
								D3DXMatrixMultiply( &matScale, &matTrans, &matScale );
								D3DXMatrixTranslation( &matTrans, 0.2f, -0.1f, -fMove );
							}
							break;
						case EF_BASE_ITEM_EFFECT_HOLY:
							{
								D3DXMatrixScaling( &matScale, fScale + 0.5f , fScale + 0.5f , fScale );
								
								TempMaterial.Diffuse.r = 0.3f;
								TempMaterial.Diffuse.g = 0.3f;
								TempMaterial.Diffuse.b = 1.0f;
								TempMaterial.Ambient.r = 0.3f;
								TempMaterial.Ambient.g = 0.3f;
								TempMaterial.Ambient.b = 1.0f;
								TempMaterial.Diffuse.a = fMaterialA;
								TempMaterial.Ambient.a = fMaterialA;
								D3DXMatrixTranslation( &matTrans, -0.2f, 0.1f, fMove );
								D3DXMatrixMultiply( &matScale, &matTrans, &matScale );
								D3DXMatrixTranslation( &matTrans, 0.2f, -0.1f, -fMove );
							}
							break;
					}			
					
					D3DXMatrixMultiply( &matTemp, &matScale, &matTrans );
					D3DXMatrixMultiply( &matTempWorld, &matTemp, &matWorld );
					g_RenderManager.SetTransform( D3DTS_WORLD, &matTempWorld );
													
					g_RenderManager.SetMaterial( &TempMaterial );
					
					switch( m_ID )
					{
						case EF_BASE_ITEM_EFFECT_PROMINAS:
						case EF_BASE_ITEM_EFFECT_PSYCHE:
						case EF_BASE_ITEM_EFFECT_MERCENARY:
						case EF_BASE_ITEM_EFFECT_KNIGHT:
						case EF_BASE_ITEM_EFFECT_SNIPER:
						case EF_BASE_ITEM_EFFECT_SLAYER:
						case EF_BASE_ITEM_EFFECT_PRIEST:
						case EF_BASE_ITEM_EFFECT_HOLY:		
							{
								g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
								g_RenderManager.SetDestBlend( D3DBLEND_ONE );
							}
							break;
					}

					DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
						g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				}
			}		
		}
	}
	
	g_RenderManager.SetLight( 0, &tem_Light1 );	
	g_RenderManager.SetMaterial( &preMaterial );

	g_RenderManager.LightEnable( 0 , FALSE );
	g_RenderManager.LightEnable( 1 , FALSE );
	
	g_RenderManager.SetFVF( dFVF );
	g_RenderManager.SetCullMode( dCullMode );
	g_RenderManager.SetLighting( dLighting );
	g_RenderManager.SetZWriteEnable( dZWriteEnable );
	g_RenderManager.SetZEnable( dZEnable );	
	g_RenderManager.SetZFunc( dZFunc );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetAlphaTestEnable( dAlphaTestEnable );

	g_RenderManager.SetTextureAlphaOP  ( 0 , dAlpOp );
	g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );

	g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
	g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
	g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );

	return TRUE;
}

int CEFEldaWeaponItem::Render(float dtime)
{
	return TRUE;
}	
						
int CEFEldaWeaponItem::Update(float dtime)
{
	if( !m_IsInit )
	{
		return TRUE;
	}	
	
	int nGapTime = 333;
	int nAfterNum = 2;
/*

	switch( m_ID )
	{
		case EF_BASE_ITEM_EFFECT_PROMINAS:	
		case EF_BASE_ITEM_EFFECT_PSYCHE:	
		case EF_BASE_ITEM_EFFECT_MERCENARY:
		case EF_BASE_ITEM_EFFECT_KNIGHT:
		case EF_BASE_ITEM_EFFECT_SNIPER:
		case EF_BASE_ITEM_EFFECT_SLAYER:
		case EF_BASE_ITEM_EFFECT_PRIEST:
		case EF_BASE_ITEM_EFFECT_HOLY:		
			{
				nGapTime = 2000;
				nAfterNum = 2;	
			}
			break;
	}
*/	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dPrevTime ) , > , nGapTime ) )
	{
		if( m_AImageList.size() > nAfterNum )
		{
			return TRUE;
		}		
		
		SSpeItmAimage sPeItemAimg;
		sPeItemAimg.dSetTime = g_nowTime - m_dInitTime;
		
		m_AImageList.push_front( sPeItemAimg );
		m_dPrevTime = g_nowTime;
	}
	
	return TRUE;
}
