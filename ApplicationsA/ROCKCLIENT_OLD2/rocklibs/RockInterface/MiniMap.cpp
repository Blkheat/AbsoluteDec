///////////////////////////////////////////////////////////////////////////////
///
///		File		: MiniMap.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Rui.h"
#include "PartyInforWnd.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"

#include "..\\..\\Camera.h"
#include "..\\..\\Map.h"
#include "..\\..\\bravolibs\\obj\\Character_Item.H"
#include "..\\..\\bravolibs\\obj\\Player_Manager.H"
#include "..\\..\\bravolibs\\obj\\Line_Vertex.H"
#include "..\\..\\Effect.h"

#include "..\\..\\GTimer.h"

#include "UiUtil.h"
#include "MiniMapWndProc.h"
#include "MiniMap.h"

#include "..\\..\\MapDataManager.h"
#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

//-----------------------------------------------------------------------------
CMiniMap::CMiniMap( SDesktop* DT ) : CWnd( DT )
{
	thePcInfo = NULL;
	
	Real_Map_Size	= 0;
	Tex_Map_Size	= 0;
	Screen_Map_Size	= 0;
	
	MapClientImage	= TID_None;
	memset( &MapClientRect, 0, sizeof( SRect ) );
	
	Border = 0;
	MapWndImage	= TID_CLIENT;
	memset( &MapWndRect, 0, sizeof( SRect ) );
	
	Map_CWHalf_Size = 0;
	Map_CHHalf_Size = 0;
	
	m_bIsPress = FALSE;
	m_bIsMapChange = FALSE;

	m_iTempCx = m_iTempCy = m_iTempCw = m_iTempCh =0;

	//by simwoosung
	m_vPlayerPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
    m_vOldPlayerPos = m_vPlayerPos;
	memset(m_strFocusName , NULL , MAX_SIZE_512 );
	
	m_iFocusIndex = 0;
	
	m_fGlowFadingTime = 0.65f;		 //�۷ο� ���̵�Ÿ��	
	m_fCumulativeTime = 0.0f;		 //�����ð�

	m_fMapViewRatio = 0.0f;
}

//-----------------------------------------------------------------------------
CMiniMap::~CMiniMap()
{
}

SEventMessage* CMiniMap::EventProc( SEventMessage* EMsg )
{
	static int prev_mx = 0;
	static int prev_my = 0;	
	
	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;
	
	int iDelMx = Mx-prev_mx;
	int iDelMy = My-prev_my;
	
	if(EMsg->Notify == NM_OUTRANGE)
	{
		iDelMx = iDelMy = 0;
	}
	
	int x = 0;	
	
	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		{	
			OnMouseFocus( Mx, My );
		}
		break;
	case MA_LDOWN:
		{
			OnMouseLButtonDown( iDelMx, iDelMx );
			m_bIsPress = TRUE;				
		}
		break;
	case MA_LPRESS:
		{
			OnMouseLButtonPress( iDelMx, iDelMy );
			m_bIsPress = TRUE;
		}
		break;
	case MA_LUP:
		{
			OnMouseLButtonUp( Mx, My );		
			m_bIsPress = FALSE;	
		}
		break;
	case MA_LDRAG:
		{
			OnMouseLDrag( iDelMx, iDelMy );
			m_bIsPress = TRUE;			    
		}
		break;
	case MA_LDROP:
		{
			m_bIsPress = FALSE;		        	
		}
		break;
	case MA_RUP:
		{
			if( GetMapWndSize() == 128 )
			{				
				MiniMapWnd.SetMapInfoText( 256 );
			}
			else
			{				
				MiniMapWnd.SetMapInfoText( 128 );
			}
		}
		break;		
	}	
	
	prev_mx = Mx;
	prev_my = My;
	
	return( EMsg );
}

BOOL CMiniMap::IsPress()
{
	return m_bIsPress;
}

//-----------------------------------------------------------------------------
void CMiniMap::OnMouseFocus( int Mx, int My )
{
	
}

//-----------------------------------------------------------------------------
void CMiniMap::OnMouseLButtonDown( int Mx, int My )
{
	
}

//-----------------------------------------------------------------------------
void CMiniMap::OnMouseLButtonPress( int Mx, int My )
{
    CalcImgRangeInPressMove(Mx, My);
}

//-----------------------------------------------------------------------------
void CMiniMap::OnMouseLButtonUp( int Mx, int My )
{
	
	
}

void CMiniMap::OnMouseLDrag( int Mx, int My )
{
	CalcImgRangeInPressMove(Mx, My);
}

//-----------------------------------------------------------------------------
void CMiniMap::RenderProc()
{
	int i = 0;
	static int		tilesize = 16;		 //�ͷ����� �����ϴ� �� Ÿ��ũ���ε�..;;
										 //�������� Real_Map_Size�� Ÿ�ϻ���� ���ؾ��ҵ�;;
	static SPoint2	tex_pchar_pos;		 //�ʻ����� �󿡼� �ڽ������� ��ġ		
	
	int indexMyUnit = -1;

	m_fCumulativeTime += g_GTimer.GetFrameElapsedSecTime();

	if( m_fCumulativeTime > m_fGlowFadingTime )
	{
		int temp = m_fCumulativeTime/m_fGlowFadingTime;			
		m_fCumulativeTime = m_fCumulativeTime - (m_fGlowFadingTime * temp);
	}
	
	//�ڽ��� ĳ������ ��ġ�� �ش���ġ��  �߽��� �Ǵ� ���� ������ ���Ѵ�.
	//���� ��ȯ�Ǿ��ų� || �ʵ巡�� ��ũ���� �������ʰ� �������� �̵����̶��....
	if( m_bIsMapChange || (!m_bIsPress && IsMyUnitMove()) )
	{
		if(m_bIsMapChange)
        {
			m_bIsMapChange = FALSE;
        }
			
		m_pCenterPos.x = tex_pchar_pos.x;
		m_pCenterPos.y = tex_pchar_pos.y;
		CalcImgRangeInMyUnitCenter( indexMyUnit, tilesize, tex_pchar_pos, m_tex_vtx_pos );
		//�� ������ �߽����� �̴ϸ� 4�ڳ��� uv��ǥ�� ���Ѵ�.
	}
	else
	{	
		CalcPosMyUnit(indexMyUnit, tilesize, tex_pchar_pos );
		//�̴ϸ� 4�ڳʴ� �̹� ����
		//�� ������ �ؽ��� ��ǥ�� ���Ѵ�.
	}	
	
	///--JAPAN_BUG_MODIFY
	if( g_RockClient.m_wPixelFormat == 32 )
	{	
		g_RenderManager.SetStencilEnable( TRUE );
		g_RenderManager.SetStencilFunc( D3DCMP_ALWAYS );
		g_RenderManager.SetStencilZFail( D3DSTENCILOP_KEEP );
		g_RenderManager.SetStencilFail( D3DSTENCILOP_KEEP );
		g_RenderManager.SetStencilRef( 0x1 );
		g_RenderManager.SetStencilMask( 0xffffffff );
		g_RenderManager.SetStencilWriteMask( 0xffffffff );
		g_RenderManager.SetStencilPass( D3DSTENCILOP_REPLACE );
	}

	Render->DrawMiniMap( MapClientImage, &MapClientRect, &m_tex_vtx_pos[0], Tex_Map_Size, 
						 D3DCOLOR_XRGB( 255, 255, 255 ) );

	///--JAPAN_BUG_MODIFY
	if( g_RockClient.m_wPixelFormat == 32 )
	{
		g_RenderManager.SetStencilEnable( FALSE );
	}

	m_bFindFocusName = FALSE;	
	
	RenderSimbolMarkWholeRange( tilesize, m_tex_vtx_pos );
	RenderUnitMarkInViewRange( tilesize, m_tex_vtx_pos );	
	RenderUnitMarkOutViewRange( tilesize, m_tex_vtx_pos );
	RenderMyUnitMark(indexMyUnit, tex_pchar_pos, m_tex_vtx_pos);	
	
	DWORD SrcBlend, DestBlend;
	
	g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );
	
	g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	
	///--JAPAN_BUG_MODIFY
	if( g_RockClient.m_wPixelFormat == 32 )
	{
		g_RenderManager.SetStencilEnable( TRUE );
		g_RenderManager.SetStencilFunc( D3DCMP_NEVER );
		g_RenderManager.SetStencilFail( D3DSTENCILOP_ZERO );
	}

	Render->DrawMiniMap( MapClientImage, &MapClientRect, &m_tex_vtx_pos[0], 
						Tex_Map_Size, D3DCOLOR_XRGB(255, 255, 255 ) );	
	
	g_RenderManager.SetSrcBlend( SrcBlend );
	g_RenderManager.SetDestBlend( DestBlend );

	///--JAPAN_BUG_MODIFY
	if( g_RockClient.m_wPixelFormat == 32 )
	{
		g_RenderManager.SetStencilEnable( FALSE );
	}	
	
	//MiniMap Wnd Image
	Render->Draw( MapWndImage, MapWndRect.x, MapWndRect.y, MapWndRect.w, MapWndRect.h, D3DCOLOR_XRGB( 255, 255, 255 ) );

	if(m_bFindFocusName)
	{
       RenderFocusName();
	}	
	
	UpdateHandler();

}

BOOL CMiniMap::IsMyUnitMove()
{
	Player*	user_pc = g_Pc.GetPlayer();
	
	D3DXMATRIX		matchar;
	
	matchar = user_pc->GetPosTM();
    m_vPlayerPos = D3DXVECTOR3(matchar._41, matchar._42, matchar._43);	
	
	//ĳ���Ͱ� �̵��ϸ� �����̹ٸ� ���ְ� �ʱ�ȭ
	float fDel = D3DXVec3Length(&(m_vOldPlayerPos - m_vPlayerPos));
	
	m_vOldPlayerPos = m_vPlayerPos;
	
	if(fDel > 0.0001f)
		return TRUE;	
	
	return FALSE;
}

void CMiniMap::CalcImgRangeInPressMove(int Mx, int My )
{
    m_pCenterPos.x -= Mx;
	m_pCenterPos.y -= My;
	
	GetTexVtxPositionX( &m_pCenterPos, &m_tex_vtx_pos[0] );
	GetTexVtxPositionY( &m_pCenterPos, &m_tex_vtx_pos[0] );
	
	m_pCenterPos.x = (m_tex_vtx_pos[0].x + m_tex_vtx_pos[2].x) / 2;
	m_pCenterPos.y = (m_tex_vtx_pos[0].y + m_tex_vtx_pos[2].y) / 2;
}

void CMiniMap::CalcImgRangeInMyUnitCenter(int & indexMyUnit,int tilesize, 
										  SPoint2 & tex_pchar_pos ,SPoint2 * tex_vtx_pos )
{
	
	SPoint2	grid_pos;
	
	for(int i = 0; i < MAX_CHARACTER; ++i )
	{
		//�ڽ��� ������ ���
		if(	( thePcInfo[i].existence == true ) && (	thePcInfo[i].ctype == enPLAYER ) )
		{
			indexMyUnit = i;
			
			grid_pos.x = (thePcInfo[i].wpos.x / tilesize);
			grid_pos.y = (thePcInfo[i].wpos.z / tilesize);
			grid_pos.y = Real_Map_Size - grid_pos.y;//��ǥ ������.... 3D������ǥ -> 2D��ũ����ǥ��..
			
			tex_pchar_pos.x = ( Tex_Map_Size * grid_pos.x ) / Real_Map_Size;
			tex_pchar_pos.y = ( Tex_Map_Size * grid_pos.y ) / Real_Map_Size;
			
			if( Tex_Map_Size == Real_Map_Size )
			{
				tex_vtx_pos[0].x = 0;
				tex_vtx_pos[3].x = 0;
				tex_vtx_pos[1].x = Tex_Map_Size;
				tex_vtx_pos[2].x = Tex_Map_Size;
				tex_vtx_pos[0].y = 0;
				tex_vtx_pos[1].y = 0;
				tex_vtx_pos[2].y = Tex_Map_Size;
				tex_vtx_pos[3].y = Tex_Map_Size;
			}
			else
			{
				GetTexVtxPositionX( &tex_pchar_pos, &tex_vtx_pos[0] );
				GetTexVtxPositionY( &tex_pchar_pos, &tex_vtx_pos[0] );
			}
			break;
		}
	}	
}

void CMiniMap::CalcPosMyUnit(int & indexMyUnit, int tilesize, SPoint2 & tex_pchar_pos )
{
	SPoint2	grid_pos;
	
	for(int i = 0; i < MAX_CHARACTER; ++i )
	{
		//�ڽ��� ������ ���
		if(	( thePcInfo[i].existence == true ) && (	thePcInfo[i].ctype == enPLAYER ) )
		{
			indexMyUnit = i;
			
			grid_pos.x = (thePcInfo[i].wpos.x / tilesize);
			grid_pos.y = (thePcInfo[i].wpos.z / tilesize);
			grid_pos.y = Real_Map_Size - grid_pos.y;//��ǥ ������.... 3D������ǥ -> 2D��ũ����ǥ��..
			
			tex_pchar_pos.x = ( Tex_Map_Size * grid_pos.x ) / Real_Map_Size;
			tex_pchar_pos.y = ( Tex_Map_Size * grid_pos.y ) / Real_Map_Size;
			
			CalcImgRangeInPressMove( 0, 0 );
			
			break;
		}	
	}
}

void CMiniMap::RenderSimbolMarkWholeRange(int tilesize, SPoint2 * tex_vtx_pos )
{
	SPoint2	screen_char_pos;
	SPoint2	tex_nchar_pos;
	SPoint2	grid_pos;

	if( g_Map.m_uMapType != MAP_TYPE_PVPNEUTFIELD )
	{
		return;
	}

	PvPFieldInfoListPos aPos = g_Map.m_PvPFieldInfoList.begin();

	for( ; aPos != g_Map.m_PvPFieldInfoList.end(); ++aPos )
	{
		SPvPFieldInfo * pPvPFieldInfo = (SPvPFieldInfo *)(*aPos);

		DWORD dCell = pPvPFieldInfo->theNPCCell;
		
		int nPosX = dCell % g_Pc.m_cSize;
		int nPosY = dCell / g_Pc.m_cSize;
	
		float xfPos = ( nPosX * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );
		float zfPos = ( nPosY * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );
		
		grid_pos.x = ( xfPos / tilesize );
		grid_pos.y = ( zfPos / tilesize );
		grid_pos.y = Real_Map_Size - grid_pos.y;//��ǥ ������....
				
		tex_nchar_pos.x = ( Tex_Map_Size * grid_pos.x ) / Real_Map_Size;
		tex_nchar_pos.y = ( Tex_Map_Size * grid_pos.y ) / Real_Map_Size;

		if( !IsInnerRange(tex_nchar_pos, tex_vtx_pos, 64, 64) )
			continue;

		screen_char_pos.x = tex_nchar_pos.x - tex_vtx_pos[0].x;
		screen_char_pos.y = tex_nchar_pos.y - tex_vtx_pos[0].y;
		screen_char_pos.x += MapClientRect.x;
		screen_char_pos.y += MapClientRect.y;	

		///-- �߸�, �з���, ���������� ���� �ٸ� �̹��� �����ֱ�
		TEXID nTexID = TID_None;
		BYTE aOccupationNation = pPvPFieldInfo->theOccupationNation;
		
		switch( aOccupationNation )
		{
			case RAIN:
				{
					nTexID = TID_totem_icen_l;
				}
				break;
			case MILLENA:
				{
					nTexID = TID_totem_icen_m;
				}
				break;
			case NEUTRAL_STATE:
				{
					nTexID = TID_totem_icen_n;
				}
				break;
		}

		RenderTextureMark(nTexID, screen_char_pos, 64, 64, -1);
		
		/*SNPCBaseInfo* npc_Info = NULL;				
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pPvPFieldInfo->theNPCCode );
		if( npc_Info ) 
		{
			CheckFocusPosition(screen_char_pos, 64, 64, npc_Info->theName );
		}*/
	}
}

void CMiniMap::RenderUnitMarkInViewRange(int tilesize, SPoint2 * tex_vtx_pos )
{
	SPoint2	screen_char_pos;
	SPoint2	tex_nchar_pos;
	SPoint2	grid_pos;
	
	for(int i = 0; i < MAX_CHARACTER; ++i )
	{
		if(	thePcInfo[i].existence == true )
		{
			grid_pos.x = (thePcInfo[i].wpos.x / tilesize);
			grid_pos.y = (thePcInfo[i].wpos.z / tilesize);
			grid_pos.y = Real_Map_Size - grid_pos.y;//��ǥ ������....
			
			tex_nchar_pos.x = ( Tex_Map_Size * grid_pos.x ) / Real_Map_Size;
			tex_nchar_pos.y = ( Tex_Map_Size * grid_pos.y ) / Real_Map_Size;
			
			if(IsInnerRange(tex_nchar_pos, tex_vtx_pos, 12, 12))
			{
				screen_char_pos.x = tex_nchar_pos.x - tex_vtx_pos[0].x;
				screen_char_pos.y = tex_nchar_pos.y - tex_vtx_pos[0].y;
				screen_char_pos.x += MapClientRect.x;
				screen_char_pos.y += MapClientRect.y;
				
				if( thePcInfo[i].ctype == enNPC )
				{
					int iFindId = -1;
					//��Ƽ ������� ã�´�. ��Ƽ����� Ǫ�������� ǥ��
					SPARTYMEMBER_INFO * pMemberInfo = NULL;
					pMemberInfo = g_PartyInforWnd.FindPartyMember(thePcInfo[i].id);
					if(pMemberInfo)
					{
						iFindId = thePcInfo[i].id;		
					}
                    
					if(iFindId == -1)
					{
						//�Ϲ��������� �̴ϸʻ� ǥ������ �ʴ´�.
						//Render->DrawPoint( screen_char_pos.x, screen_char_pos.y, D3DCOLOR_XRGB( 255, 255, 0 ), 4.0f ); 
					}
					else
					{
				         RenderTextureMark(TID_mini_party, screen_char_pos, 12, 12, -1);
					     CheckFocusPosition(screen_char_pos, 12, 12, thePcInfo[i].name);										
					}
				}			
			}
		}
	}
}

void CMiniMap::RenderUnitMarkOutViewRange(int tilesize, SPoint2 * tex_vtx_pos )
{
	SPoint2	screen_char_pos;
	SPoint2	tex_nchar_pos;
	SPoint2	grid_pos;

	//�ʿ��� NPC��ġ ���
	list< SMinimapNPCInfo * >::iterator ni = m_NPCInfoList.begin();
	list< SMinimapNPCInfo * >::iterator nend = m_NPCInfoList.end();

	for( ; ni != nend ; ni++ )
	{
		SMinimapNPCInfo * pNPCInfo = (*ni);
		float xfPos = ( pNPCInfo->nCellPosX * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );
		float zfPos = ( pNPCInfo->nCellPosY * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );

		grid_pos.x = ( xfPos / tilesize );
		grid_pos.y = ( zfPos / tilesize );
		grid_pos.y = Real_Map_Size - grid_pos.y;//��ǥ ������....
				
		tex_nchar_pos.x = ( Tex_Map_Size * grid_pos.x ) / Real_Map_Size;
		tex_nchar_pos.y = ( Tex_Map_Size * grid_pos.y ) / Real_Map_Size;

		if( !IsInnerRange(tex_nchar_pos, tex_vtx_pos, 12, 12) )
			continue;
			
			screen_char_pos.x = tex_nchar_pos.x - tex_vtx_pos[0].x;
			screen_char_pos.y = tex_nchar_pos.y - tex_vtx_pos[0].y;
			screen_char_pos.x += MapClientRect.x;
			screen_char_pos.y += MapClientRect.y;			
			
			if(pNPCInfo->PreQuestCodeList.size() != 0)
			{
				if( pNPCInfo->nQuestMarkType == START_QUEST_MARK )
				{
					RenderTextureMark(TID_mini_npc, screen_char_pos, 12, 12, START_QUEST_MARK);
					CheckFocusPosition(screen_char_pos, 12, 12, pNPCInfo->strNpcName);
				}
				else if(pNPCInfo->nQuestMarkType == REPORT_QUEST_MARK )
				{
					RenderTextureMark(TID_mini_npc, screen_char_pos, 12, 12, REPORT_QUEST_MARK);	
					CheckFocusPosition(screen_char_pos, 12, 12, pNPCInfo->strNpcName);
				}		
			}
			else
			{
				RenderTextureMark(TID_mini_npc, screen_char_pos, 12, 12, -1);
				CheckFocusPosition(screen_char_pos, 12, 12, pNPCInfo->strNpcName);
			}
	}
	
	if(!g_PartyInforWnd.IsMemberParty())
		return;
	
	//��Ƽ���� ��ġ���
	list< SPARTYMEMBER_INFO * >::iterator i = g_PartyInforWnd.GetBeginMember();
	list< SPARTYMEMBER_INFO * >::iterator end = g_PartyInforWnd.GetEndMember();
	
	for(  ; i != end ; i++)
	{
		SPARTYMEMBER_INFO *pMemberInfo = (*i);	
		
		if( g_Pc.GetMainPC().lCurMap == pMemberInfo->m_wMap ) 
		{
			if( !(pMemberInfo->m_bIsLogOut) && (!pMemberInfo->m_bIsViewRange) )
			{
				int		nPosX, nPosY;
				nPosX = (pMemberInfo->m_dCell) % g_Pc.m_cSize;
				nPosY = (pMemberInfo->m_dCell) / g_Pc.m_cSize;
				
				float xfPos = ( nPosX * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );
				float zfPos = ( nPosY * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );
				
				grid_pos.x = ( xfPos / tilesize );
				grid_pos.y = ( zfPos / tilesize );
				grid_pos.y = Real_Map_Size - grid_pos.y;//��ǥ ������....
				
				tex_nchar_pos.x = ( Tex_Map_Size * grid_pos.x ) / Real_Map_Size;
				tex_nchar_pos.y = ( Tex_Map_Size * grid_pos.y ) / Real_Map_Size;
				
				if(IsInnerRange(tex_nchar_pos, tex_vtx_pos, 12, 12))
				{
					screen_char_pos.x = tex_nchar_pos.x - tex_vtx_pos[0].x;
					screen_char_pos.y = tex_nchar_pos.y - tex_vtx_pos[0].y;
					screen_char_pos.x += MapClientRect.x;
					screen_char_pos.y += MapClientRect.y;					
					
					RenderTextureMark(TID_mini_party, screen_char_pos, 12, 12, -1);		 
					CheckFocusPosition(screen_char_pos, 12, 12, (pMemberInfo->m_strPCName) );				
				}
			}
		}
	}
}

void CMiniMap::RenderMyUnitMark(int indexMyUnit, SPoint2 & tex_pchar_pos, SPoint2 * tex_vtx_pos)
{
	SPoint2	screen_char_pos;
	// ���� ĳ���͸� ���߿� ��� ���ؼ�....
	int direction = 0;
	char str[40] = "";
	
	if(indexMyUnit == -1)
		return;
	
		//���� ǥ�� ��ƾ....
	if(IsInnerRange(tex_pchar_pos, tex_vtx_pos, 16, 16))
	{
		screen_char_pos.x = tex_pchar_pos.x - tex_vtx_pos[0].x;
		screen_char_pos.y = tex_pchar_pos.y - tex_vtx_pos[0].y;
		screen_char_pos.x += MapClientRect.x;
		screen_char_pos.y += MapClientRect.y;		
		
		direction = GetAngleToDirection( thePcInfo[indexMyUnit].angle ) + TID_Direction0;//Direction TID�� 472���� ����....
		RenderTextureMark(direction, screen_char_pos, 16, 16, -1);
	}
}

void CMiniMap::RenderTextureMark(UINT Tid, SPoint2 CenPos, int width, int height, int iExtraGlowMark )
{
   SPoint2 sPoint; 
   sPoint.x = CenPos.x - (width/2);
   sPoint.y = CenPos.y - (height/2);
   
   Render->Draw( Tid, sPoint.x, sPoint.y, width, height, D3DCOLOR_XRGB( 255, 255, 255 ) );

   if(iExtraGlowMark == -1)
	   return;

   float fRatio = m_fCumulativeTime / m_fGlowFadingTime;
   
   float afRatio = 0.0f;
   if( m_fCumulativeTime < (m_fGlowFadingTime/2.0f) )
   {
	   afRatio = m_fCumulativeTime / (m_fGlowFadingTime/2.0f);
   }
   else
   {
	   float fCulmuTime = m_fCumulativeTime - (m_fGlowFadingTime/2.0f);
	   afRatio = (1 - fCulmuTime / (m_fGlowFadingTime/2.0f));
   }

   D3DCOLOR Color = D3DCOLOR_ARGB((int)(afRatio * 255), 1, 1, 1 );

   int iGWidth = 14;
   int iGHeight = 14;

   iGWidth += (int)(fRatio * (43 - 14));
   iGHeight += (int)(fRatio * (43 - 14));

   if((iGWidth % 2) != 0)
     iGWidth +=1;
   if((iGHeight % 2) != 0)
     iGHeight +=1;

   sPoint.x = CenPos.x - (iGWidth/2);
   sPoint.y = CenPos.y - (iGHeight/2);

   DWORD ColorOP, ColorARG1, ColorARG2; 

	g_RenderManager.GetTextureStageState( 0, D3DTSS_COLOROP		, &ColorOP );
    g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG1	, &ColorARG1  );
    g_RenderManager.GetTextureStageState( 0, D3DTSS_COLORARG2	, &ColorARG2  );   
   
    g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_ADD );
    g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
    g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

	///--JAPAN_BUG_MODIFY
	if( g_RockClient.m_wPixelFormat == 32 )
	{
		g_RenderManager.SetStencilEnable( TRUE );
		g_RenderManager.SetStencilRef( 0x0 );
		g_RenderManager.SetStencilFunc( D3DCMP_NOTEQUAL ); 
		g_RenderManager.SetStencilPass( D3DSTENCILOP_KEEP );
	}

   switch(iExtraGlowMark)
   {
	  case 	START_QUEST_MARK:
		  {
			  Render->ScretchDraw( TID_mini_qstartglow, sPoint.x, sPoint.y, iGWidth, iGHeight, Color );	
		  }
		  break;
	  case  REPORT_QUEST_MARK:
		  {
			  Render->ScretchDraw( TID_mini_qreportglow, sPoint.x, sPoint.y, iGWidth, iGHeight, Color );
		  }
		  break;  
   }

	///--JAPAN_BUG_MODIFY	
    if( g_RockClient.m_wPixelFormat == 32 )
	{	
		g_RenderManager.SetStencilEnable( FALSE );
	}

	g_RenderManager.SetTextureColorOP  ( 0 , ColorOP );
	g_RenderManager.SetTextureColorArg1( 0 , ColorARG1 );
	g_RenderManager.SetTextureColorArg2( 0 , ColorARG2 );
}

void CMiniMap::CheckFocusPosition(SPoint2 Src_Cen_Pos, int width, int height, TCHAR *strName )
{
	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	SPoint2 UpLeft, DownRight;
	UpLeft.x = Src_Cen_Pos.x - (width/2);
    UpLeft.y = Src_Cen_Pos.y - (height/2);

	DownRight.x = Src_Cen_Pos.x + (width/2);
	DownRight.y = Src_Cen_Pos.y + (height/2);

	if( ( ptCursor.x > UpLeft.x ) && ( ptCursor.y > UpLeft.y ) && 
		( ptCursor.x < DownRight.x ) && ( ptCursor.y < DownRight.y ) )
	{
       m_bFindFocusName = TRUE;	   
	   Rstrcpy(RWCHAR(m_strFocusName), RWCHAR(strName));
	   m_FocusRect.x = UpLeft.x;
	   m_FocusRect.y = UpLeft.y;
	   m_FocusRect.w = width;
	   m_FocusRect.h = height;
	}
}

void CMiniMap::RenderFocusName()
{
    nRui->GetHelpTip()->SetString( &m_FocusRect,  m_strFocusName, 
		                             D3DCOLOR_XRGB( 255, 255, 255 ));
	nRui->GetHelpTip()->RenderProc();
}

BOOL CMiniMap::IsInnerRange(SPoint2 & cenPoint, SPoint2 * tex_vtx_pos, int width, int height)
{
	if( ( (cenPoint.x - (width/2)) > tex_vtx_pos[0].x ) && ( (cenPoint.x + (width/2)) < tex_vtx_pos[2].x ) &&
	( (cenPoint.y - (height/2)) > tex_vtx_pos[0].y ) && ( (cenPoint.y + (height/2)) < tex_vtx_pos[2].y ) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CMiniMap::IsInnerRange2(SPoint2 & SrcCenPoint, SPoint2 * tex_vtx_pos, int width, int height)
{
    SPoint2 cenPoint;

	cenPoint.x = SrcCenPoint.x + tex_vtx_pos[0].x - MapClientRect.x;
	cenPoint.y = SrcCenPoint.y + tex_vtx_pos[0].y - MapClientRect.y;

	if( ( (cenPoint.x - (width/2)) > tex_vtx_pos[0].x ) && ( (cenPoint.x + (width/2)) < tex_vtx_pos[2].x ) &&
	( (cenPoint.y - (height/2)) > tex_vtx_pos[0].y ) && ( (cenPoint.y + (height/2)) < tex_vtx_pos[2].y ) )
	{
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
void CMiniMap::SetPcInfo( SPcInfo* pcinfo )
{
	thePcInfo = pcinfo;
}

//-----------------------------------------------------------------------------
void CMiniMap::SetMiniMap( int size, TEXID image )
{
	Real_Map_Size	= size;	
	Tex_Map_Size = max( Real_Map_Size * 2, 256 );
	
	m_fMapViewRatio =  (float)( Real_Map_Size * 2 ) / (float)Tex_Map_Size;	

	MapClientImage	= image;	
}

//-----------------------------------------------------------------------------
void CMiniMap::SetMapWnd( int size, TEXID image )
{
	if(!isInit)
	{
		m_iTempCx = Cx;
		m_iTempCy = Cy;
		m_iTempCw = Cw;
		m_iTempCh = Ch;
		
		isInit = true;
	}
	else
	{
		Cx = m_iTempCx;
		Cy = m_iTempCy;
		Cw = m_iTempCw;
		Ch = m_iTempCh;
	}
	
	Map_Wnd_Size	= size;
	Map_WHalf_Size	= Map_Wnd_Size / 2;
	MapWndImage		= image;
	Border			= 21;
	
	if( Map_Wnd_Size == 128 )
	{
		MapWndRect.x	= Cx + 128;
		MapWndRect.y	= Cy;
		MapWndRect.w	= Cw + 128;
		MapWndRect.h	= 161; //Ch + 256;//�̴ϸ� ���ҽ��� �ٲ�鼭 �޶�����.
		
		MapClientRect.x = MapWndRect.x + 1;//  Border; 
		MapClientRect.y = MapWndRect.y + 21;//Border;
		MapClientRect.w = MapWndRect.w - 2;//( Border * 2 );// 14*2 -- 14: 1286map�� �׵θ� �β�
		MapClientRect.h = 161 - 42;//( Ch + 128 ) - ( Border * 2 );//MapWndRect.h - ( Border * 2 );
		
		Map_CWHalf_Size	= MapClientRect.w / 2;
		Map_CHHalf_Size	= MapClientRect.h / 2;		
		
		Screen_Map_Size	= size - ( Border * 2 );
	}
	else if( Map_Wnd_Size == 256 )
	{
		MapWndRect.x	= Cx;
		MapWndRect.y	= Cy;
		MapWndRect.w	= Cw + 256;
		MapWndRect.h	= 258;//Ch + 512;//�̴ϸ� ���ҽ��� �ٲ�鼭 �޶�����.
		
		MapClientRect.x = MapWndRect.x + 1;//Border;
		MapClientRect.y = MapWndRect.y + 21;//Border;
		MapClientRect.w = MapWndRect.w - 2;//( Border * 2 );// 27*2 -- 27: 256map�� �׵θ� �β�
		MapClientRect.h = 258 - 42;//( Ch + 256 ) - ( Border * 2 );//MapWndRect.h - ( Border * 2 );
		
		Map_CWHalf_Size	= MapClientRect.w / 2;
		Map_CHHalf_Size	= MapClientRect.h / 2;

		Screen_Map_Size	= size - ( Border * 2 );
	}
	
	Cx = MapClientRect.x;
	Cy = MapClientRect.y;
	Cw = MapClientRect.w;
	Ch = MapClientRect.h;

	CWnd *pWnd;

	pWnd = Container->Get( Pid );
	pWnd->ResetSize(&MapClientRect);
   
}

//-----------------------------------------------------------------------------
int CMiniMap::GetMapWndSize()
{
	return( Map_Wnd_Size );
}

//-----------------------------------------------------------------------------
void CMiniMap::GetTexVtxPositionX( SPoint2* _tex_char_pos, SPoint2* _tex_vtx_pos )
{
	static int tmp_pos_l( 0 );
	static int tmp_pos_r( 0 );
	
	// X
	tmp_pos_l = _tex_char_pos->x - Map_CWHalf_Size;
	tmp_pos_r = _tex_char_pos->x + Map_CWHalf_Size;
	
	
	if( tmp_pos_l <= 0 )
	{
		_tex_vtx_pos[0].x = 0;
		_tex_vtx_pos[3].x = 0;
		_tex_vtx_pos[1].x = MapClientRect.w;
		_tex_vtx_pos[2].x = MapClientRect.w;
		return;
	}
	else
	{
		_tex_vtx_pos[0].x = tmp_pos_l;
		_tex_vtx_pos[3].x = tmp_pos_l;
	}
	
	
	if( tmp_pos_r >= Tex_Map_Size )
	{
		_tex_vtx_pos[0].x = Tex_Map_Size - MapClientRect.w;
		_tex_vtx_pos[3].x = Tex_Map_Size - MapClientRect.w;
		_tex_vtx_pos[1].x = Tex_Map_Size;
		_tex_vtx_pos[2].x = Tex_Map_Size;
		return;
	}
	else
	{
		_tex_vtx_pos[1].x = tmp_pos_r;
		_tex_vtx_pos[2].x = tmp_pos_r;
	}
	
	
}

//-----------------------------------------------------------------------------
void CMiniMap::GetTexVtxPositionY( SPoint2* _tex_char_pos, SPoint2* _tex_vtx_pos )
{
	static int tmp_pos_t( 0 );
	static int tmp_pos_b( 0 );
	
	
	// Y
	tmp_pos_t = _tex_char_pos->y - Map_CHHalf_Size;//-
	tmp_pos_b = _tex_char_pos->y + Map_CHHalf_Size;//+
	
	
	if( tmp_pos_t <= 0 )
	{
		_tex_vtx_pos[0].y = 0;
		_tex_vtx_pos[1].y = 0;
		_tex_vtx_pos[2].y = MapClientRect.h;
		_tex_vtx_pos[3].y = MapClientRect.h;
		return;
	}
	else
	{
		_tex_vtx_pos[0].y = tmp_pos_t;
		_tex_vtx_pos[1].y = tmp_pos_t;
	}
	
	
	if( tmp_pos_b >= Tex_Map_Size )
	{
		_tex_vtx_pos[0].y = Tex_Map_Size - MapClientRect.h;
		_tex_vtx_pos[1].y = Tex_Map_Size - MapClientRect.h;
		_tex_vtx_pos[2].y = Tex_Map_Size;
		_tex_vtx_pos[3].y = Tex_Map_Size;
		return;
	}
	else
	{
		_tex_vtx_pos[2].y = tmp_pos_b;
		_tex_vtx_pos[3].y = tmp_pos_b;
	}
}

//-----------------------------------------------------------------------------
int CMiniMap::GetAngleToDirection( int _angle )
{
	//-------------------------------------------------------------------------
	// ���߿� ȸ���������� �ٲ���....(������ �ǰ���~~~...)
	//-------------------------------------------------------------------------
	if( _angle < 0 )
	{
		_angle = 360 + _angle;
	}
	
	if( _angle >= 0 )
	{
		if( (_angle >= 0) && (_angle <= 10) )		return 0;
		
		if( (_angle > 10) && (_angle <= 40) )		return 1;
		if( (_angle > 40) && (_angle <= 60) )		return 2;
		if( (_angle > 60) && (_angle <= 80) )		return 3;
		
		if( (_angle > 80) && (_angle <= 100) )		return 4;
		
		if( (_angle > 100) && (_angle <= 130) )		return 5;
		if( (_angle > 130) && (_angle <= 150) )		return 6;
		if( (_angle > 150) && (_angle <= 170) )		return 7;
		
		if( (_angle > 170) && (_angle <= 190) )		return 8;
		
		if( (_angle > 190) && (_angle <= 213) )		return 9;
		if( (_angle > 213) && (_angle <= 236) )		return 10;
		if( (_angle > 236) && (_angle <= 260) )		return 11;
		
		if( (_angle > 260) && (_angle <= 280) )		return 12;
		
		if( (_angle > 280) && (_angle <= 303) )		return 13;
		if( (_angle > 303) && (_angle <= 326) )		return 14;
		if( (_angle > 326) && (_angle <= 350) )		return 15;
		
		if( (_angle > 350) && (_angle <= 359) )		return 0;
	}
	
	return 0xff;
}

//-----------------------------------------------------------------------------
