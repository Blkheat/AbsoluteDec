 #include "RockPCH.h"
#include "..\\Obj\\Character.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH
#include "quadlist.h"
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include <Obj\\ObjBase.h>
#include <Map\\Field.h>
#include "RockClient.h"
#include "Map.h"
#include "Pc.h"
#include "Npc.h"
#include <Obj\\TextureManager.H>
#include "..\\..\\GTimer.h"
#include "..\\..\\rocklibs\\RockInterface\\Render.h"
#include "..\\..\\rocklibs\\RockInterface\\PetMainWndProc.h"

#include "..\\..\\CRenderManager.h"

#define CHARIMAGE_TARGET_SIZE	128

float s_CharImageData::m_fReNewTime = 0.5f;

CCharImage::CCharImage()
{
	m_MaskTex[ 0 ] = m_MaskTex[ 1 ] = NULL;
	m_CharMask = NULL;
	m_PartyCharMask = NULL;
}

CCharImage::~CCharImage()
{
	InvalidateDeviceObject();	
}

HRESULT	CCharImage::InvalidateDeviceObject()
{
	if( m_pRenderToSurface != NULL )
	{
		m_pRenderToSurface->Release();
		m_pRenderToSurface = NULL;
	}
	
	if( NULL != m_MaskTex[ 0 ] )
	{
		m_MaskTex[ 0 ]->Release();
		m_MaskTex[ 0 ] = NULL;
	}
	
	if( NULL != m_MaskTex[ 1 ] )
	{
		m_MaskTex[ 1 ]->Release();
		m_MaskTex[ 1 ] = NULL;
	}

	if( NULL != m_CharMask )
	{
		m_CharMask->Release();
		m_CharMask = NULL;
	}

	if( NULL != m_PartyCharMask )
	{
		m_PartyCharMask->Release();
		m_PartyCharMask = NULL;
	}

	DeleteAll();
	
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CCharImage::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT CCharImage::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	Init( pDevice );
	
	return S_OK;
}

void CCharImage::DeleteAll(void)
{
	list<LPCHAR_IMAGE_DATA>::iterator i = m_List.begin();

	for( ; i != m_List.end() ; ++i )
	{
		LPCHAR_IMAGE_DATA pData = (*i);
		
		if( pData->race == c_NpcRace_Mob )
		{
			SAFE_DELETE( pData->m_pCharacter ) ;
		}	
		
		SAFE_DELETE( pData );
	    (*i) = NULL;
	}

	m_List.clear();
}

int CCharImage::Delete(int unique,int race, bool isBackImg)
{
	list<LPCHAR_IMAGE_DATA>::iterator i = m_List.begin();

	for( ; i != m_List.end() ; ++i )
	{
		LPCHAR_IMAGE_DATA pData = (*i);	
		
		if( pData->unique == unique &&
			pData->race   == race  &&
			pData->isBackImg == isBackImg )
		{
			if( pData->race ==  c_NpcRace_Mob )
			{
				SAFE_DELETE( pData->m_pCharacter ) ;
			}
			
			SAFE_DELETE( pData );
			(*i) = NULL;

			m_List.erase( i );

			return TRUE;
		}
	}

	return FALSE;
}

LPCHAR_IMAGE_DATA *CCharImage::Find(int unique,int race, bool isBackImg)
{
	list<LPCHAR_IMAGE_DATA>::iterator i = m_List.begin();

	for( ; i != m_List.end() ; ++i )
	{
		if( (*i)->unique == unique &&
			(*i)->race   == race &&
			(*i)->isBackImg == isBackImg )
		{
			return &(*i);
		}
	}

	return FALSE;
}

#ifdef DIRECT_VERSION_9_MJH
int	CCharImage::Init(LPDIRECT3DDEVICE9 pDevice)
#else
int	CCharImage::Init(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	D3DSURFACE_DESC Desc;
	HRESULT			hr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( FAILED( g_RenderManager.CreateTextureFromFile( "\\Etc\\CharImgAlpha32.tga", &m_MaskTex[ 0 ] ) ) )
	{
		return FALSE;
	}

	if( FAILED( g_RenderManager.CreateTextureFromFile( "\\Etc\\CharImgAlpha64.tga", &m_MaskTex[ 1 ] ) ) )
	{
		return FALSE;
	}

	if( FAILED( g_RenderManager.CreateTextureFromFile( "\\Etc\\cha_mask.tga", &m_CharMask ) ) )
	{
		return FALSE;
	}

	if( FAILED( g_RenderManager.CreateTextureFromFile( "\\Etc\\party_mask.tga", &m_PartyCharMask ) ) )
	{
		return FALSE;
	}	
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFEtc.OpenFile( "CharImgAlpha32.tga" );	
	if( pFH )
	{
		HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_MaskTex[ 0 ] );
		
		g_VFEtc.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}		

	pFH = g_VFEtc.OpenFile( "CharImgAlpha64.tga" );	
	if( pFH )
	{
		HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_MaskTex[ 1 ] );
		
		g_VFEtc.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}
	
	pFH = g_VFEtc.OpenFile( "cha_mask.tga" );	
	if( pFH )
	{
		HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_CharMask );
		
		g_VFEtc.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}

	pFH = g_VFEtc.OpenFile( "party_mask.tga" );	
	if( pFH )
	{
		HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																	0, NULL, NULL, &m_PartyCharMask );
		
		g_VFEtc.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9		pBackBuffer;
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0, 0,D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ) )
#else
	LPDIRECT3DSURFACE8		pBackBuffer;
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}
	
	pBackBuffer->GetDesc( &Desc );
	SAFE_RELEASE( pBackBuffer );	

	if( FAILED( hr = g_RenderManager.CreateRenderToSurface( CHARIMAGE_TARGET_SIZE, CHARIMAGE_TARGET_SIZE, 
		D3DFMT_A8R8G8B8, TRUE, D3DFMT_D16, &m_pRenderToSurface ) ) )
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int	CCharImage::Create(int unique,int race,LPDIRECT3DDEVICE9 pDevice, bool isBackImg)
#else
int	CCharImage::Create(int unique,int race,LPDIRECT3DDEVICE8 pDevice, bool isBackImg)
#endif // DIRECT_VERSION_9_MJH
{
	LPCHAR_IMAGE_DATA *pData = Find(unique,race,isBackImg);
	
	if( NULL != pData )
	{
		 return TRUE;
	}

	D3DSURFACE_DESC Desc;
	HRESULT			hr;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9		pBackBuffer;
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0, 0,D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ) )
#else
	LPDIRECT3DSURFACE8		pBackBuffer;
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}
	pBackBuffer->GetDesc( &Desc );
	SAFE_RELEASE( pBackBuffer );

	LPCHAR_IMAGE_DATA TempData = NULL;	

	TempData = SAFE_NEW( CHAR_IMAGE_DATA );		

	if( TempData == NULL )
		return FALSE;

	if( FAILED( hr = g_RenderManager.CreateTexture( CHARIMAGE_TARGET_SIZE , CHARIMAGE_TARGET_SIZE , 1 , 
		D3DUSAGE_RENDERTARGET , D3DFMT_A8R8G8B8 , D3DPOOL_DEFAULT, &TempData->pCharImageTex ) ) )
	{
		SAFE_DELETE(TempData);

		return FALSE;
	}		

	if( FAILED( hr = g_RenderManager.CreateTexture( CHARIMAGE_TARGET_SIZE , CHARIMAGE_TARGET_SIZE , 1 ,
		0, D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED, &TempData->pCharImageTexMem ) ) )
	{
		SAFE_DELETE(TempData);
		
		return FALSE;
	}	

	if( FAILED( hr = TempData->pCharImageTex->GetSurfaceLevel( 0, &TempData->pCharImageSurface ) ) )
	{
		SAFE_DELETE(TempData);
		
		return FALSE;
	}

	if( FAILED( hr = TempData->pCharImageTexMem->GetSurfaceLevel( 0, &TempData->pCharImageSurfaceMem ) ) )
	{
		SAFE_DELETE(TempData);
		
		return FALSE;
	}

	TempData->unique = unique;
	TempData->race   = race;
	TempData->isBackImg = isBackImg;
	
	if( race ==  c_NpcRace_Mob )
	{
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( unique ); 
		if(!npc_Info)
		{
			return FALSE;
		}

		if( npc_Info->theRace != NPC_RACE_REALHUMAN )
		{
			bool bFindNpc = false;		
		
			// NPC 에 타입에 없는 것을 생성하라고 하면 시스템 다운 
			bFindNpc = g_Pc_Manager.IsFindFileData( (DWORD)unique );

			if( !bFindNpc )
			{
				if( !g_Pc_Manager.AddNpcCode( unique ) )
				{
					return FALSE;
				}
			}		
		}			
		
		NonPlayer *pNonPlayer = SAFE_NEW( NonPlayer );		
		pNonPlayer->m_TempIndex = npc_Info->theFileCode;		
		pNonPlayer->m_bPC = false;		

		if( npc_Info->theRace == NPC_RACE_REALHUMAN )
		{
			SPlayerNPCData PNPCData = g_Pc_Manager.m_PlayerNPCDataManager.FindPlayerNPCData( unique );
			
			pNonPlayer->m_nNpcCode = unique;
			pNonPlayer->m_State.nCharacterType = nCharacterTypePlayerNPC;
			pNonPlayer->m_TempIndex = g_Pc.GetAttrIndex( PNPCData.Citizen, PNPCData.Gender );
		}	

		pNonPlayer->CreateNonPlayer();
		TempData->m_pCharacter = (void *)pNonPlayer;
	}

	m_List.insert( m_List.end() , TempData );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int	CCharImage::Change(int unique,int race,LPDIRECT3DDEVICE9 pDevice, bool isBackImg)
#else
int	CCharImage::Change(int unique,int race,LPDIRECT3DDEVICE8 pDevice, bool isBackImg)
#endif // DIRECT_VERSION_9_MJH
{
	LPCHAR_IMAGE_DATA *pData = Find(unique,race,isBackImg);

	if( NULL != pData )
	{
		(*pData)->m_IsRender = false;
		(*pData)->m_IsOneTimeRender = false;
		(*pData)->m_fCumulativeTime = 0.0f;
		//MakeImage( pData , pDevice, true);

		return TRUE;
	}

	return FALSE;
}

#ifdef DIRECT_VERSION_9_MJH
int CCharImage::MakeImage(LPCHAR_IMAGE_DATA *ppData,LPDIRECT3DDEVICE9 pDevice,  bool bFlag)
#else
int CCharImage::MakeImage(LPCHAR_IMAGE_DATA *ppData,LPDIRECT3DDEVICE8 pDevice,  bool bFlag)
#endif // DIRECT_VERSION_9_MJH
{
	int					ani_number;					// 현재 애니메이션 번호
	int					pre_ani_number;				// 이전 애니메이션 번호
	int					ani_index;					// 현재 애니메이션 인덱스	
	int					ani_frame;					// 애니 프레임
	unsigned int		end_frame;					// 현재 종료 프레임			
	unsigned int		pre_end_frame;				// 이전 종표 프레임			
	
	int					blend_frame;				// 모션 블렌딩 프레임 
	
	int					move_frame;					// 이동 프레임 
	float				move_middle_time;			// 이동 보간 프레임
	
	float				move_percent;				// 이동거리 퍼센트 
	float				ani_percent;				// 애니메이션 퍼센트 
	
	float				step_time;					// 보폭타임
	float				ani_time;					// 애니 타임	
	float				middle_time;				// 1프레임 ~ 2프레임 사이의 중간값 저장 ( 0.0 ~ 1.0 사이 )	
	long				PreTime;					// 이전 렌더링한 시간	
	long				SimTime;					// 심심한 시간	
	D3DXMATRIX			matChr;
	
	Character			*pChar = NULL;
	int					 stand_ani_index = 0;

	//.........................................................................................................
	// 유저일때
	//.........................................................................................................
	if( (*ppData)->race == c_PcRace_User )
	{
		if( g_Pc.GetPlayer()->GetUnique() == (*ppData)->unique )
		{
			pChar = ( Character *)g_Pc.GetPlayer();
		}
		else
		{
			pChar = ( Character *)g_Pc.GetUser( (long)(*ppData)->unique );			
		}

		if( NULL == pChar )
		{
			return FALSE;
		}
	}
	else
	//.........................................................................................................
	// 몹일때
	//.........................................................................................................
	if( (*ppData)->race == c_NpcRace_Mob )
	{
		pChar = (Character *)(*ppData)->m_pCharacter;

		if( pChar->m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			stand_ani_index = 0;
		}
		else
		{
			stand_ani_index = n_NPC_Stand0;
		}
	}

	ani_number		 = pChar->m_ani_number;		
	pre_ani_number   = pChar->m_pre_ani_number;	
	ani_index	 	 = pChar->m_ani_index;		
	ani_frame		 = pChar->m_ani_frame;		
	end_frame		 = pChar->m_end_frame;		
	pre_end_frame	 = pChar->m_pre_end_frame;	
	
	blend_frame		 = pChar->m_blend_frame;	
	
	move_frame		 = pChar->m_move_frame;		
	move_middle_time = pChar->m_move_middle_time;
	
	move_percent	 = pChar->m_move_percent;	
	ani_percent		 = pChar->m_ani_percent;	
	
	step_time		 = pChar->m_step_time;		
	ani_time		 = pChar->m_ani_time;		
	middle_time		 = pChar->m_middle_time;	
	PreTime			 = pChar->m_PreTime;		
	SimTime			 = pChar->m_SimTime;		
	matChr           = pChar->GetPosTM();
	
	pChar->AnimationPlayNoneBlend( stand_ani_index );
	pChar->m_IsRenderImg = TRUE;
	pChar->AniRender();
	pChar->m_IsRenderImg = FALSE;
	
	HRESULT hr;
	
	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}	
	
	g_RenderManager.GetTransform( D3DTS_VIEW , &m_matView  );
	g_RenderManager.GetTransform( D3DTS_WORLD, &m_matWorld );
	
	D3DXMATRIX  matView;
	D3DXMATRIX  matHead;
	
	float fheight = ( pChar->GetBoundMaxVec().y - pChar->GetBoundMinVec().y );
	float fDepth  = ( pChar->GetBoundMaxPos().z - pChar->GetBoundMinPos().z );
	float fWidth  = ( pChar->GetBoundMaxVec().x - pChar->GetBoundMinVec().x );
	
	fWidth = 0.0f;
	
	if( (*ppData)->race == c_PcRace_User )
	{
		fDepth = 6.0f;
		fheight = 18.0f;
	}
	else
	{
		//fheight *= pChar->m_vScale.y;
		fheight -= 2.0f;
		fDepth += 3.0f;
	}

	D3DXMATRIX  matRotX;
	D3DXVECTOR3 vUp(0.0f,1.0f,0.0f);  

	D3DXMatrixRotationX( &matRotX , D3DXToRadian( 25 ) );

	D3DXVec3TransformCoord( &vUp , &vUp , &matRotX );

	D3DXVECTOR3 vFromPt, vLookatPt, vUpVec;

	if( (*ppData)->race == c_PcRace_User )
	{
		vFromPt   = D3DXVECTOR3( -2.0f  , fheight * 0.95f , -fDepth );
		vLookatPt = D3DXVECTOR3( fWidth , fheight * 0.9f ,  0.0f );
		vUpVec    = vUp;
	}
	else
	{
		float fBack = 0.0f;
		float fHei = 0.0f;
		
		SNPCBaseInfo*	npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( (*ppData)->unique ); 
		
		if( npc_Info )
		{
			if( npc_Info->theRace == NPC_RACE_REALHUMAN )
			{
				fWidth = 0.0f;
				fDepth = 6.0f;
				fheight = 18.0f;				
				
				vFromPt   = D3DXVECTOR3( -2.0f  , fheight * 0.95f , -fDepth );
				vLookatPt = D3DXVECTOR3( fWidth , fheight * 0.9f ,  0.0f );
				vUpVec    = vUp;			
			}
			else
			{
				fHei = (float)(npc_Info->theEld);
				fBack = (float)(npc_Info->theEldRate / 100.0f);
#ifdef PET_TIME_TEST
				if( npc_Info->theCode == 6191 || npc_Info->theCode == 6193 || npc_Info->theCode == 6195 )
				{
					vFromPt   = D3DXVECTOR3( -1.0f  , fHei-4 * 0.95f , -( fBack ) );
					vLookatPt = D3DXVECTOR3( 0.0f , fHei-4 * 0.9f ,  0.0f );
				}
				else if( npc_Info->theCode == 6192 || npc_Info->theCode == 6194 || npc_Info->theCode == 6196 )
				{
					vFromPt   = D3DXVECTOR3( -1.0f  , fHei-1 * 0.95f , -( fBack ) );
					vLookatPt = D3DXVECTOR3( 0.0f , fHei-1 * 0.9f ,  0.0f );
				}
				else
				{
					vFromPt   = D3DXVECTOR3( -1.0f  , fHei * 0.95f , -( fBack ) );
					vLookatPt = D3DXVECTOR3( 0.0f , fHei * 0.9f ,  0.0f );
				}
#else 
				vFromPt   = D3DXVECTOR3( -1.0f  , fHei * 0.95f , -( fBack ) );
				vLookatPt = D3DXVECTOR3( 0.0f , fHei * 0.9f ,  0.0f );
#endif // PET_TIME_TEST
				
				vUpVec    = vUp;			
			}
		}		
	}	
	
	D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	g_RenderManager.SetTransform( D3DTS_VIEW, &matView );

	
	D3DXMATRIX matProj;	

	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/5.25f, 1.0f, 1.0f, 1000.0f );	//조금더 확대

	g_RenderManager.SetTransform( D3DTS_PROJECTION, &matProj );
		
	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	

	g_RenderManager.SetFogEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dAlphaTestEnable = g_RenderManager.GetAlphaTestEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend(); 
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );			

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTOP_DISABLE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTOP_DISABLE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );

	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	
	if( SUCCEEDED( ( hr = m_pRenderToSurface->BeginScene( (*ppData)->pCharImageSurface , NULL ) ) ) )
	{		
		if((*ppData)->isBackImg)
		{
			g_RenderManager.Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0L );
		}
		else
		{
			g_RenderManager.Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0 , 1.0f, 0L );
		}		
	
		if(bFlag)
		{
			
			int nRace = (*ppData)->race;

			switch( nRace )
			{
				case c_NpcRace_Mob:
					if( !pChar->ViewModelNPC() )
					{
						(*ppData)->m_IsRender = false;
					}
					break;
				case c_PcRace_User:
					if( !pChar->ViewModelPC() )
					{
						(*ppData)->m_IsRender = false;
					}					
					break;				
			}
		}
#ifdef DIRECT_VERSION_9_MJH
		m_pRenderToSurface->EndScene( D3DX_FILTER_LINEAR );
#else
		m_pRenderToSurface->EndScene();
#endif // DIRECT_VERSION_9_MJH				
	}
	
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetAlphaTestEnable( dAlphaTestEnable );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );

	D3DSURFACE_DESC d3dsd;
	D3DLOCKED_RECT  d3dlr;

	(*ppData)->pCharImageTexMem->GetLevelDesc( 0, &d3dsd );

	//by dongs 흑백 이미지 .. 추가 
	if(SUCCEEDED (hr = D3DXLoadSurfaceFromSurface(  (*ppData)->pCharImageSurfaceMem , NULL , NULL ,(*ppData)->pCharImageSurface,NULL,NULL,D3DX_DEFAULT , 0)))
	{
		
		if( SUCCEEDED(hr = (*ppData)->pCharImageTexMem->LockRect( 0, &d3dlr, 0, 0 )))
		{
			DWORD color = 0;
		
			for( int u=0; u<d3dsd.Height; ++u )
			{
				for( int v=0; v<d3dsd.Width; ++v )
				{
					if(d3dsd.Format == D3DFMT_A8R8G8B8 )
					{
						color = ((DWORD*)d3dlr.pBits)[d3dsd.Width*u+v];
						color = ( GetRValue(color) + GetGValue(color) + GetBValue(color)  ) / 3;
						
						if(color)
						{
							((DWORD*)d3dlr.pBits)[d3dsd.Width*u+v] = D3DCOLOR_XRGB(color,color,color);
						}
						else 
						{
							((DWORD*)d3dlr.pBits)[d3dsd.Width*u+v] = 0;
						}

					}
					else 
					{
						break;
					}
				}
			}
			
			// Unlock the texture and return OK.
			(*ppData)->pCharImageTexMem->UnlockRect(0);
			
		}
	}

	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetFVF( RUI_VERTEX_FORMAT );
	
	g_Camera.ReloadProjTM();	
	
	pChar->m_ani_number		  = ani_number;		
	pChar->m_pre_ani_number   = pre_ani_number;	
	pChar->m_ani_index	 	  = ani_index;		
	pChar->m_ani_frame		  = ani_frame;		
	pChar->m_end_frame		  = end_frame;		
	pChar->m_pre_end_frame	  = pre_end_frame;	
	
	pChar->m_blend_frame	  =	blend_frame;	
	
	pChar->m_move_frame		  = move_frame;		
	pChar->m_move_middle_time = move_middle_time;
	
	pChar->m_move_percent	  = move_percent;	
	pChar->m_ani_percent	  =	ani_percent;	
	
	pChar->m_step_time		  = step_time;		
	pChar->m_ani_time		  = ani_time;		
	pChar->m_middle_time	  =	middle_time;	
	pChar->m_PreTime		  =	PreTime;		
	pChar->m_SimTime		  = SimTime;		
	
	pChar->SetPosTM2( matChr );
	
	g_RenderManager.SetTransform( D3DTS_VIEW, &m_matView );
	g_RenderManager.SetTransform( D3DTS_WORLD, &m_matWorld );

	if( bIsBeginScene )
	{
		g_RockClient.BeginScene();
	}

	if(!pChar->m_IsAllLTexture)
	{
		return FALSE;
	}

	return TRUE;
}

int	CCharImage::Render(int x,int y,int unique,int race,NCHAR_IMAGE_TYPE type,
#ifdef DIRECT_VERSION_9_MJH
					   LPDIRECT3DDEVICE9 pDevice, bool isBackImg)
#else
					   LPDIRECT3DDEVICE8 pDevice, bool isBackImg)
#endif // DIRECT_VERSION_9_MJH
				
{
	static int size[ 2 ] = { 32 , 64 };
	
	LPCHAR_IMAGE_DATA *pData = Find(unique,race,isBackImg);
	
	if( NULL == pData )
		return FALSE;

	//by simwoosung
	if(!((*pData)->m_IsRender))		//실제 렌더링이 되어있지 않으면
	{		
		if( !((*pData)->m_IsOneTimeRender) )
		{
			(*pData)->m_IsOneTimeRender = true;
		}		
		else
		{
			(*pData)->m_fCumulativeTime += g_GTimer.GetFrameElapsedSecTime();
			if( (*pData)->m_fCumulativeTime >= s_CharImageData::m_fReNewTime )
			{
				(*pData)->m_fCumulativeTime = 0.0f;
				(*pData)->m_IsRender = true;		//일정시간 누적후 렌더링
				
				///--JAPAN_BUG_MODIFY
				if( !MakeImage( pData , pDevice, true) )
				{
					(*pData)->m_IsOneTimeRender = false;
					(*pData)->m_fCumulativeTime = 0.0f;
					(*pData)->m_IsRender = false;
				}
			}
			else
			{
				if((*pData)->race == c_NpcRace_Mob )
				{
					((Character *)((*pData)->m_pCharacter))->AniRender();
					
					bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
					if( bIsBeginScene )
					{
						g_RockClient.EndScene();
					}			
					
					((Character *)((*pData)->m_pCharacter))->ViewModelNPC();
					
					if( bIsBeginScene )
					{
						g_RockClient.BeginScene();
					}
				}
			}
		}

		nRui->SetState();
		return TRUE;
	}
	
	nRui->SetState();

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	DWORD		TexCoordIndex_0, ColorOP_0, ColorARG1_0, AlphaOP_0, AlphaARG1_0;
	DWORD		TexCoordIndex_1, ColorOP_1, ColorARG1_1, AlphaOP_1, AlphaARG1_1;
	
	g_RenderManager.GetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, &TexCoordIndex_0 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP		, &ColorOP_0 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1	, &ColorARG1_0   );	
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAOP		, &AlphaOP_0 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG1	, &AlphaARG1_0 );

	g_RenderManager.GetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, &TexCoordIndex_1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLOROP		, &ColorOP_1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG1	, &ColorARG1_1 );	
	g_RenderManager.GetTextureStageState( 1, D3DTSS_ALPHAOP		, &AlphaOP_1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_ALPHAARG1	, &AlphaARG1_1   );
	
	g_RenderManager.SetTextureCoordIndex ( 0 , 0 );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureCoordIndex ( 1 , 0 );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT   );

	g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 1 , D3DTA_TEXTURE );

	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

	g_RenderManager.SetTexture( 0, (*pData)->pCharImageTex );

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 pTexture = m_MaskTex[ (int) type ];
#else
	LPDIRECT3DTEXTURE8 pTexture = m_MaskTex[ (int) type ];
#endif // DIRECT_VERSION_9_MJH
	if(!((*pData)->isBackImg))
	{
		pTexture = m_CharMask; 
	}
	
	g_RenderManager.SetTexture( 1, pTexture);

	DrawTexture( x , y , size[ (int) type ] , size[ (int) type ]  , 
							  0.0f , 0.0f , 1.0f , 1.0f ,
							  D3DCOLOR_XRGB( 255 , 255 , 255 ) , pDevice );

	g_RenderManager.SetTexture( 0 , NULL );
	g_RenderManager.SetTexture( 1 , NULL );

	g_RenderManager.SetTextureCoordIndex ( 0 , TexCoordIndex_0 );

	g_RenderManager.SetTextureColorOP  ( 0 , ColorOP_0 );
	g_RenderManager.SetTextureColorArg1( 0 , ColorARG1_0 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , AlphaOP_0 );
	g_RenderManager.SetTextureAlphaArg1( 0 , AlphaARG1_0 );

	g_RenderManager.SetTextureCoordIndex ( 1 , TexCoordIndex_1 );

	g_RenderManager.SetTextureColorOP  ( 1 , ColorOP_1 );
	g_RenderManager.SetTextureColorArg1( 1 , ColorARG1_1 );
	
	g_RenderManager.SetTextureAlphaOP  ( 1 , AlphaOP_1 );
	g_RenderManager.SetTextureAlphaArg1( 1 , AlphaARG1_1 );

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	nRui->SetState();

	return TRUE;
}

int	CCharImage::Render(int x,int y,int unique,int race,NCHAR_IMAGE_TYPE type, 
#ifdef DIRECT_VERSION_9_MJH
					   LPDIRECT3DTEXTURE9 texmask,LPDIRECT3DDEVICE9 pDevice, bool isBackImg , bool alive)
#else
					   LPDIRECT3DTEXTURE8 texmask,LPDIRECT3DDEVICE8 pDevice, bool isBackImg , bool alive)
#endif // DIRECT_VERSION_9_MJH
{
	static int size[ 2 ] = { 32 , 64 };
	
	LPCHAR_IMAGE_DATA *pData = Find( unique, race, isBackImg );

	if( NULL == pData )
		return FALSE;

	//by simwoosung
	if(!((*pData)->m_IsRender))		//실제 렌더링이 되어있지 않으면
	{		
		if( !((*pData)->m_IsOneTimeRender) )
		{
			(*pData)->m_IsOneTimeRender = true;
		}		
		else
		{
			(*pData)->m_fCumulativeTime += g_GTimer.GetFrameElapsedSecTime();
			
			if( (*pData)->m_fCumulativeTime >= s_CharImageData::m_fReNewTime )
			{
				(*pData)->m_fCumulativeTime = 0.0f;
				((*pData)->m_IsRender) = true;		//일정시간 누적후 렌더링
				
				///--JAPAN_BUG_MODIFY
				if( !MakeImage( pData , pDevice, true) )
				{
					(*pData)->m_IsOneTimeRender = false;
					(*pData)->m_fCumulativeTime = 0.0f;
					(*pData)->m_IsRender = false;
				}				
			}
			else
			{
				if((*pData)->race == c_NpcRace_Mob )
				{
					((Character *)((*pData)->m_pCharacter))->AniRender();
					
					bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
					if( bIsBeginScene )
					{
						g_RockClient.EndScene();
					}					
					
					((Character *)((*pData)->m_pCharacter))->ViewModelNPC();
					
					if( bIsBeginScene )
					{
						g_RockClient.BeginScene();
					}			
				}
			}
		}

		nRui->SetState();
		return TRUE;
	}

	nRui->SetState();
	
	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	DWORD		TexCoordIndex_0, ColorOP_0, ColorARG1_0, ColorARG2_0,AlphaOP_0, AlphaARG1_0;
	DWORD		TexCoordIndex_1, ColorOP_1, ColorARG1_1, AlphaOP_1, AlphaARG1_1;

	g_RenderManager.GetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, &TexCoordIndex_0 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP		, &ColorOP_0 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1	, &ColorARG1_0   );	
	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2	, &ColorARG2_0   );	
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAOP		, &AlphaOP_0 );
	g_RenderManager.GetTextureStageState( 0, D3DTSS_ALPHAARG1	, &AlphaARG1_0 );

	g_RenderManager.GetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, &TexCoordIndex_1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLOROP		, &ColorOP_1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_COLORARG1	, &ColorARG1_1 );	
	g_RenderManager.GetTextureStageState( 1, D3DTSS_ALPHAOP		, &AlphaOP_1 );
	g_RenderManager.GetTextureStageState( 1, D3DTSS_ALPHAARG1	, &AlphaARG1_1   );
	
	g_RenderManager.SetTextureCoordIndex ( 0 , 0 );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureCoordIndex ( 1 , 0 );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT   );

	g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 1 , D3DTA_TEXTURE );


	if(alive)
	{
		g_RenderManager.SetTexture( 0 , (*pData)->pCharImageTex);
		
	}
	else 
	{
		g_RenderManager.SetTexture( 0, (*pData)->pCharImageTexMem );
	
	}

	g_RenderManager.SetTexture( 1, texmask );	

	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

	DrawTexture( x , y , size[ (int) type ] , size[ (int) type ]  , 
							  0.0f , 0.0f , 1.0f , 1.0f ,
							  D3DCOLOR_XRGB( 255 , 255 , 255 ), pDevice );	

	g_RenderManager.SetTexture( 0 , NULL );
	g_RenderManager.SetTexture( 1 , NULL );

	g_RenderManager.SetTextureCoordIndex ( 0 , TexCoordIndex_0 );

	g_RenderManager.SetTextureColorOP  ( 0 , ColorOP_0 );
	g_RenderManager.SetTextureColorArg1( 0 , ColorARG1_0 );
	g_RenderManager.SetTextureColorArg2( 0 , ColorARG2_0 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , AlphaOP_0 );
	g_RenderManager.SetTextureAlphaArg1( 0 , AlphaARG1_0 );

	g_RenderManager.SetTextureCoordIndex ( 1 , TexCoordIndex_1 );

	g_RenderManager.SetTextureColorOP  ( 1 , ColorOP_1 );
	g_RenderManager.SetTextureColorArg1( 1 , ColorARG1_1 );
	
	g_RenderManager.SetTextureAlphaOP  ( 1 , AlphaOP_1 );
	g_RenderManager.SetTextureAlphaArg1( 1 , AlphaARG1_1 );

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_LINEAR );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_LINEAR );

	nRui->SetState();

	return TRUE;
}


void CCharImage::DrawTexture( float x, float y, float w, float h, 
							  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
							  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice )
#else
							  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice )
#endif // DIRECT_VERSION_9_MJH
{
	static VERTEX_2D_CHARIMG	v[4];
	
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

	g_RenderManager.SetFVF( D3DFVF_VERTEX_2D_CHARIMG );
	g_RenderManager.DrawPrimitiveUP(D3DPT_TRIANGLEFAN , 2, v , sizeof( VERTEX_2D_CHARIMG ) );
}

void CCharImage::ReFreshPetImage()
{
	vector < list<LPCHAR_IMAGE_DATA>::iterator > templist;

	list<LPCHAR_IMAGE_DATA>::iterator in = m_List.begin();
   
	for( ; in != m_List.end() ; ++in )
	{
		LPCHAR_IMAGE_DATA pData = (*in);
		
		if( pData->race == c_NpcRace_Mob )
		{
			SAFE_DELETE(pData->m_pCharacter);
			SAFE_DELETE(pData);			
			(*in) = NULL;			
			templist.push_back(in);
		}
	}
	int i = 0;
	for( i = 0 ; i < templist.size() ; i++)
	{
		m_List.erase(templist[i]);
	}
	
	for( i = 0 ; i < MAX_PET_SLOT_NUM ; ++i )
	{
		if( g_RockClient.m_PcInven->thePetData[i] != NULL)
		{
			Create( g_RockClient.m_PcInven->thePetData[i]->NpcCode, 
					 c_NpcRace_Mob , g_lpDevice, false );			
		}
	}	
}