#include "RockPCH.h"
#include "VFileSystem.h"
#include "CCombo2DEffect.h"
#include <stdio.h>
#include	<NETWORK\Protocol.h>
#include	"..\\quadlist.h"
#include	<BASE\\D3DBASE.H>
#include	"..\\RockClient.h"
#include	"..\\PC.h"
#include	"..\\Camera.h"
#include	"..\\Map.h"
#include	"ObjBase.H"
#include	"..\\bravolibs\\obj\\Character_Item.H"
#include	"..\\bravolibs\\obj\\Player_Manager.H"
#include	 "..\\bravolibs\\obj\\Line_Vertex.H"
#include	"CDamageEffect.h"
#include	"..\\Effect.h"
#include	<stdio.h>
#include	<assert.H>
#include	<SOUND\Sound.H>
#include    "..\\..\\CRenderManager.h"

CCombo2DEffect::CCombo2DEffect()
{
	int i = 0;
	for( i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 9 ; ++j )
		{
			for( int n = 0 ; n < 3 ; ++n )
			{
				m_Texture[ i ][ j ].Texture[ n ] = NULL;
			}
		}
	}

	for( i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			m_SpotTexture[ i ][ j ] = NULL;
		}
	}

	for( i = 0 ; i < 3 ; ++i )
	{
		m_CompleTexture[ i ] = NULL;
	}

	m_State = COMBO_NONE;	

	m_Start.x     = 448.0f;
	m_Start.y     = 200.0f;

	m_LeftSide.x  = -30;
	m_LeftSide.y  = 200.0f;

	m_RightSide.x = -22;
	m_RightSide.y = 200.0f;

	m_Center.x    = -25;
	m_Center.y    = 200.0f;

	m_IsComplelete = FALSE;

	m_Comple_State = COMPLE_COMBO_NONE;

	m_ComStart.x     = 448.0f + 90.0f;
	m_ComStart.y     = 200.0f + 45.0f;

	m_ComLeftSide.x  = -30 + 90.0f;
	m_ComLeftSide.y  = 200.0f + 45.0f;

	m_ComRightSide.x = -22 + 90.0f;
	m_ComRightSide.y = 200.0f + 45.0f;

	m_ComCenter.x    = -25 + 90.0f;
	m_ComCenter.y    = 200.0f + 45.0f;
}


int CCombo2DEffect::SetCombo(int race,int level, BOOL IsComplete)
{
	if( ( level - 1 ) < 0 )
	{
		Clear();
		
		return TRUE;	
	}

	m_ComboNum  = ( level - 1 );

	m_IsComplelete = IsComplete;

	// 급하니 하드코딩 하장 T.T
	switch( race )	
	{
		case 0:
			if( ( level - 1 ) > 7 )
			{
				Clear();	
		
				return TRUE;
			}
			break;

		case 1:
			if( ( level - 1 ) > 5 )
			{
				Clear();	
		
				return TRUE;
			}
			break;

		case 2:
			if( ( level - 1 ) > 6 )
			{
				m_ComboNum = 6;
			}
			break;
	}
	
	///-- 콤보가 성공했을 시 카메라를 흔들어 보아요 
	/*if( m_IsComplelete )
	{
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	}*/
	
	m_State     = SLIDE_LEFT;

	m_StartPos  = m_Start;
	m_Pos       = m_Start;
	m_EndPos    = m_LeftSide;
	
	m_RangeTime = C2DCOMBO_LEFT_SLIDE_TIME;

	m_ImageNum  = COMBO_SLIDE_IMAGE;

	m_OldTimer  = timeGetTime();

	m_Race      = race;

	m_Alpha     = 0;
	
	m_SpotTimer = timeGetTime();
	m_ISSpot	= TRUE;

	m_SpotNum   = 0;

	// 밀레나 일때만 표시
	if( m_Race == 0 )
	{
		switch( m_ComboNum )
		{
			case 0:
				g_Particle.SetEffSetCon( 0.0f , EF_MILLENA_COMBO_SPOT0 , 0 , 2.0f , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				break;

			case 1:
				g_Particle.SetEffSetCon( 0.0f , EF_MILLENA_COMBO_SPOT1 , 0 , 2.0f , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				break;

			case 2:
				g_Particle.SetEffSetCon( 0.0f , EF_MILLENA_COMBO_SPOT2 , 0 , 2.0f , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				break;

			case 3:
				g_Particle.SetEffSetCon( 0.0f , EF_MILLENA_COMBO_SPOT3 , 0 , 2.0f , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				break;

			default:
				g_Particle.SetEffSetCon( 0.0f , EF_MILLENA_COMBO_SPOT4 , 0 , 2.0f , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() );
				break;
		}
	}
	
	//g_Sound.Play( 97 );
/*
	switch( m_Race )
	{
		case 0:
			g_Sound.Play( 97 );
			break;
		case 2:
		case 1:
			g_Sound.Play( 96 );
			break;
	}
*/
	return TRUE;
}

int CCombo2DEffect::Clear(void)
{
	m_State = COMBO_NONE;

	m_Comple_State = COMPLE_COMBO_NONE;

	return TRUE;
}

int CCombo2DEffect::Update(void)
{
	if( m_ISSpot )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_SpotTimer ) , >= , 30 ) )
		{
			m_SpotNum++;

			m_SpotTimer = timeGetTime();
			
			if( m_SpotNum == 3 )
				m_ISSpot = FALSE;
		}
	}

	if( m_State == COMBO_NONE  )
		return TRUE;

	DWORD CapTimer = 0;

	CapTimer = SAFE_TIME_SUB( timeGetTime() , m_OldTimer );

	if( SAFE_TIME_COMPARE( CapTimer , >= , m_RangeTime ) )
	{
		switch( m_State )
		{
			case SLIDE_LEFT:

				m_State     = SLIDE_RIGHT;
				
				m_StartPos  = m_LeftSide;
				m_Pos       = m_LeftSide;
				m_EndPos    = m_RightSide;
				m_RangeTime = C2DCOMBO_RIGHT_SLIDE_TIME;
				
				m_ImageNum  = COMBO_NORMAL_IMAGE;

				m_Alpha     = 255;
				break;

			case SLIDE_RIGHT:

				m_State     = SLIDE_CENTER;
				
				m_StartPos  = m_RightSide;
				m_Pos       = m_RightSide;
				m_EndPos    = m_Center;
				m_RangeTime = C2DCOMBO_CENTER_SLIDE_TIME;
				
				m_ImageNum  = COMBO_NORMAL_IMAGE;

				break;

			case SLIDE_CENTER:

				m_State     = COMBO_TWINKLE;
		
				m_Pos       = m_Center;

				m_RangeTime = C2DCOMBO_TWINKLE_SLIDE_TIME;
				
				m_ImageNum  = COMBO_SPOT_IMAGE;

				break;

			case COMBO_TWINKLE:

				m_State     = COMBO_NORMAL;		
				
				m_Pos       = m_Center;

				m_ImageNum  = COMBO_NORMAL_IMAGE;
				
				m_RangeTime = 1000;

				if( m_IsComplelete )
				{
					m_Comple_State = COMPLE_SLIDE_LEFT;
					
					m_ComStartPos  = m_ComStart;
					m_ComPos       = m_ComStart;
					m_ComEndPos    = m_ComLeftSide;

					m_ComRangeTime = 50;

					m_ComImageNum  = COMBO_SLIDE_IMAGE;

					m_ComOldTimer  = timeGetTime();

					m_ComAlpha     = 0;					
				}				

				return TRUE;

			case COMBO_NORMAL:

				m_State     = COMBO_NONE;

				if( m_IsComplelete )
				{
					goto COM_COMBO_LINE;
				}
		
				return TRUE;
		}

		m_OldTimer  = timeGetTime();
	}
	else
	{
		if( m_State == COMBO_NORMAL )
		{
			if( m_IsComplelete )
			{
				goto COM_COMBO_LINE;
			}
			
			return TRUE;
		}
		
		if( CapTimer <= 0 )
		{
			m_Pos = m_StartPos;
		}
		else
		{
			m_Pos = m_StartPos + ( ( m_EndPos - m_StartPos ) * ( float ) ( ( float ) CapTimer / ( float ) m_RangeTime ) );
			
			if( SLIDE_LEFT == m_State )
				m_Alpha = ( BYTE ) ( 255.0f * ( float ) ( ( float ) CapTimer / ( float ) m_RangeTime ) );
		}
	}


COM_COMBO_LINE:

	if( m_Comple_State == COMPLE_COMBO_NONE )
		return TRUE;

	CapTimer = 0;

	CapTimer = SAFE_TIME_SUB( timeGetTime() , m_ComOldTimer );

	if( SAFE_TIME_COMPARE( CapTimer , >= , m_ComRangeTime ) )
	{
		switch( m_Comple_State )
		{
			case COMPLE_SLIDE_LEFT:

				m_Comple_State     = COMPLE_SLIDE_RIGHT;
				
				m_ComStartPos  = m_ComLeftSide;
				m_ComPos       = m_ComLeftSide;
				m_ComEndPos    = m_ComRightSide;
				m_ComRangeTime = C2DCOMBO_RIGHT_SLIDE_TIME;
				
				m_ComImageNum  = COMBO_NORMAL_IMAGE;

				m_ComAlpha     = 255;
				break;

			case COMPLE_SLIDE_RIGHT:

				m_Comple_State     = COMPLE_SLIDE_CENTER;
				
				m_ComStartPos  = m_ComRightSide;
				m_ComPos       = m_ComRightSide;
				m_ComEndPos    = m_ComCenter;
				m_ComRangeTime = C2DCOMBO_CENTER_SLIDE_TIME;
				
				m_ComImageNum  = COMBO_NORMAL_IMAGE;

				break;

			case COMPLE_SLIDE_CENTER:

				m_Comple_State     = COMPLE_COMBO_TWINKLE;
		
				m_ComPos       = m_ComCenter;

				m_ComRangeTime = C2DCOMBO_TWINKLE_SLIDE_TIME;
				
				m_ComImageNum  = COMBO_SPOT_IMAGE;

				break;

			case COMPLE_COMBO_TWINKLE:

				m_Comple_State     = COMPLE_COMBO_NORMAL;		
				
				m_ComPos       = m_ComCenter;

				m_ComImageNum  = COMBO_NORMAL_IMAGE;
				
				m_ComRangeTime = 1000;				

				return TRUE;

			case COMPLE_COMBO_NORMAL:

				m_Comple_State     = COMPLE_COMBO_NONE;
		
				return TRUE;
		}

		m_ComOldTimer  = timeGetTime();
	}
	else
	{
		if( m_Comple_State == COMPLE_COMBO_NORMAL )
			return TRUE;
		
		if( CapTimer <= 0 )
		{
			m_ComPos = m_ComStartPos;
		}
		else
		{
			m_ComPos = m_ComStartPos + ( ( m_ComEndPos - m_ComStartPos ) * 
						( float ) ( ( float ) CapTimer / ( float ) m_ComRangeTime ) );
			
			if( SLIDE_LEFT == m_Comple_State )
				m_ComAlpha = ( BYTE ) ( 255.0f * ( float ) ( ( float ) CapTimer / ( float ) m_ComRangeTime ) );
		}
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CCombo2DEffect::Draw(LPDIRECT3DDEVICE9 pDevice)
#else
int CCombo2DEffect::Draw(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( m_State == COMBO_NONE  )
		return TRUE;

	g_RenderManager.SetZEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetFogEnable( FALSE );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	g_RenderManager.SetTextureAlphaOP  ( 0 ,D3DTOP_MODULATE  );
	g_RenderManager.SetTextureAlphaArg1( 0 ,D3DTA_DIFFUSE  );
	g_RenderManager.SetTextureAlphaArg2( 0 ,D3DTA_TEXTURE );

	// 투명색 적용을 위한 알파테스트. (알파값이 0이면 투명색처리)
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );


	g_RenderManager.SetTexture( 0 , m_Texture[ m_Race ][ m_ComboNum ].Texture[ m_ImageNum ] );
	
	g_RenderManager.SetFVF( D3DFVF_COMBO2DVERTEXT );
	
	DrawTexture( m_Pos.x , m_Pos.y , 256.0f , 64.0f , 0.0f , 0.0f , 1.0f , 1.0f , D3DCOLOR_ARGB( m_Alpha , 255 , 255 , 255 ) , pDevice );

	if( ( m_Comple_State != COMPLE_COMBO_NONE ) && m_IsComplelete )
	{
		g_RenderManager.SetTexture( 0 , m_CompleTexture[ m_ComImageNum ] );
		DrawTexture( m_ComPos.x , m_ComPos.y, 256.0f , 64.0f , 0.0f , 0.0f , 1.0f , 1.0f , D3DCOLOR_ARGB( m_ComAlpha , 255 , 255 , 255 ) , pDevice );
	}
	
	
	if( m_ISSpot )
	{
		g_RenderManager.SetTexture( 0 , m_SpotTexture[ m_Race ][ m_SpotNum ] );
		DrawTexture( 0 , 0 , 1024.0f , 768.0f , 0.0f , 0.0f , 1.0f , 1.0f , D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) , pDevice );
	}

	g_RenderManager.SetTexture( 0 , NULL );

	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_POINT );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_NONE );

	return TRUE;
}



CCombo2DEffect::~CCombo2DEffect()
{
	DeleteAllMemory();
}

void CCombo2DEffect::DeleteAllMemory(void)
{
	int i = 0;
	for( i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 9 ; ++j )
		{
			for( int n = 0 ; n < 3 ; ++n )
			{
				if( m_Texture[ i ][ j ].Texture[ n ] != NULL )
				{
					m_Texture[ i ][ j ].Texture[ n ]->Release();
					m_Texture[ i ][ j ].Texture[ n ] = NULL;
				}
			}
		}
	}

	for( i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			if( m_SpotTexture[ i ][ j ] != NULL )
			{
				m_SpotTexture[ i ][ j ]->Release();

				m_SpotTexture[ i ][ j ] = NULL;
			}
		}
	}

	for( i = 0 ; i < 3 ; ++i )
	{
		if( m_CompleTexture[ i ] != NULL )
		{
			m_CompleTexture[ i ]->Release();
			m_CompleTexture[ i ] = NULL;
		}
	}
}


#ifdef DIRECT_VERSION_9_MJH
int	 CCombo2DEffect::LoadTexture(LPDIRECT3DDEVICE9 pDevice)
#else
int	 CCombo2DEffect::LoadTexture(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	char TempStr[ 1024 ] = "";
	
	for( int i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 9 ; ++j )
		{
			for( int n = 0 ; n < 3 ; ++n )
			{
				m_Texture[ i ][ j ].Texture[ n ] = NULL;
				
				// 급하니 하드코딩 하장 T.T
				switch( i )	
				{
				case 0:
					if( j > 8 )
					{
						continue;
					}
					break;
					
				case 1:
					if( j > 5 )
					{
						continue;
					}
					break;
					
				case 2:
					if( j > 6 )
					{
						continue;
					}
					break;
				}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
				switch( i )
				{
					case 0:
						wsprintf( TempStr , "etc\\Combo%d_%d.tga" , j , n + 1 );
						break;

					case 1:
						wsprintf( TempStr , "etc\\chant%d_%d.tga" , j , n + 1 );
						break;

					case 2:
						wsprintf( TempStr , "etc\\shot%d_%d.tga" , j , n + 1 );
						break;
				}

				if( FAILED( g_RenderManager.CreateTextureFromFileEx( TempStr , D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
																	D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0 , NULL, NULL, &m_Texture[ i ][ j ].Texture[ n ] ) ) )
				{
					RLG1( "CCombo2DEffect::LoadTexture Fail %s" , TempStr );
				}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
				switch( i )
				{
					case 0:
						wsprintf( TempStr , "Combo%d_%d.tga" , j , n + 1 );
						break;

					case 1:
						wsprintf( TempStr , "chant%d_%d.tga" , j , n + 1 );
						break;

					case 2:
						wsprintf( TempStr , "shot%d_%d.tga" , j , n + 1 );
						break;
				}

				VFileHandle* pFH = g_VFEtc.OpenFile( TempStr );	/// OpenFile(...)에서 이미 메모리에 올라와 있음 
				

				if( pFH )
				{
					HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																				D3DX_DEFAULT, D3DX_DEFAULT, 
																				1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																				D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																				D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																				0, NULL, NULL, &m_Texture[ i ][ j ].Texture[ n ] );
			
					g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
				}		
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
			}
		}
	}

	for(int i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
			switch( i )
			{
				case 0:
					wsprintf( TempStr , "etc\\Combospot%d.tga" , j );
					break;
				
				case 1:
					wsprintf( TempStr , "etc\\Chantspot%d.tga" , j );
					break;

				case 2:
					wsprintf( TempStr , "etc\\Combospot%d.tga" , j );
					break;
			}


		if( FAILED( g_RenderManager.CreateTextureFromFileEx( TempStr , D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
			0 , NULL, NULL, &m_SpotTexture[ i ][ j ] ) ) )
		{
		}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
		switch( i )
		{
		case 0:
			wsprintf( TempStr , "Combospot%d.tga" , j );
			break;			
		case 1:
			wsprintf( TempStr , "Chantspot%d.tga" , j );
			break;
		case 2:
			wsprintf( TempStr , "Combospot%d.tga" , j );
			break;
		}
		

		VFileHandle* pFH = g_VFEtc.OpenFile( TempStr );	/// OpenFile(...)에서 이미 메모리에 올라와 있음 
		
		if( pFH )
		{
			HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_SpotTexture[ i ][ j ] );
			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
		}

	}
	
	for(int i = 0 ; i < 3 ; ++i )
	{
		wsprintf( TempStr , "complete_%d.tga" , i+1 );

		VFileHandle* pFH = g_VFEtc.OpenFile( TempStr );	/// OpenFile(...)에서 이미 메모리에 올라와 있음

		if( pFH )
		{
			HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_CompleTexture[ i ] );
			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		
	}	
	

	return TRUE;
}


void CCombo2DEffect::DrawTexture( float x, float y, float w, float h, 
								  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
								  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice )
#else
								  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice )
#endif // DIRECT_VERSION_9_MJH
{
	static COMBO2DVERTEX	v[4];
	
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	
	v[0].pos.x = (float)x - 0.5f;
	v[0].pos.y = (float)y - 0.5f;
	v[1].pos.x = v[0].pos.x + w;
	v[1].pos.y = v[0].pos.y;
	v[2].pos.x = v[0].pos.x + w;
	v[2].pos.y = v[0].pos.y + h;
	v[3].pos.x = v[0].pos.x;
	v[3].pos.y = v[0].pos.y + h;

	v[0].tu0 = (float)tx;
	v[0].tv0 = (float)ty;
	v[1].tu0 = v[0].tu0 + tw;
	v[1].tv0 = v[0].tv0;
	v[2].tu0 = v[0].tu0 + tw;
	v[2].tv0 = v[0].tv0 + th;
	v[3].tu0 = v[0].tu0;
	v[3].tv0 = v[0].tv0 + th;

	g_RenderManager.DrawPrimitiveUP(D3DPT_TRIANGLEFAN , 2, v , sizeof( COMBO2DVERTEX ) );
}
