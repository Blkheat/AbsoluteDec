
#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "PartyInforWnd.h"

#include "..\\..\\GTimer.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"
#include "..\\..\\global.h"

#include "MiniMapWndProc.h"
#include "LocalEntireMap.h"

#include "..\\..\\CRenderManager.h"
#include "..\\..\\MapDataManager.h"

CLocalEntireMap::CLocalEntireMap( SDesktop* DT ) : CWnd( DT )
{
	m_pPcInfo = NULL;
	m_pFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal ); 

	m_pRefMiniMap = NULL;
	MapClientImage = TID_None;
	m_uiMapAlpha = 255;

	m_FrameWidth = 2;

	m_tilesize = 16;

	m_fGlowFadingTime = 0.65f;		 //글로우 페이딩타임	
	m_fCumulativeTime = 0.0f;		 //누적시간
	
	m_bFindFocusName = FALSE;

	m_MapState = LEntState_Nomal;
	
	m_fEffCumTime = 0.0f;
	m_fEffectTime = 0.25f;
	m_uiMapAlpha = 120;

}

CLocalEntireMap::~CLocalEntireMap()
{

}

void CLocalEntireMap::SetPcInfo( SPcInfo* pcinfo )
{
	m_pPcInfo = pcinfo;
}

void CLocalEntireMap::SetMapAlpha(UINT uiAlpha)
{
	m_uiMapAlpha = uiAlpha;
}

void CLocalEntireMap::RenderProc()
{
	switch(m_MapState)
	{
		case LEntState_Nomal:     NormalRender();
								  break;
		case LEntState_Viewing:   ViewingRender();
								  break;
		case LentState_Closing:	  ClosingRender();
								  break;	
	}
}

void CLocalEntireMap::NormalRender()
{
	Render->ScretchDraw( MapClientImage, m_WndRect.x, m_WndRect.y, m_WndRect.w, m_WndRect.h, 
	D3DCOLOR_ARGB( m_uiMapAlpha, 255, 255, 255 ) );

	RenderFrame();

	float fElapsedtime = g_GTimer.GetFrameElapsedSecTime();	
	m_fCumulativeTime += fElapsedtime;	

	if( m_fCumulativeTime > m_fGlowFadingTime )
	{
		int temp = m_fCumulativeTime/m_fGlowFadingTime;			
		m_fCumulativeTime = m_fCumulativeTime - (m_fGlowFadingTime * temp);		
	}

	m_bFindFocusName = FALSE;

	RenderSimbolMarkWholeRange();
	RenderUnitMarkInViewRange();
	RenderUnitMarkOutViewRange();
	RenderMyUnitMark();

	RenderLocalMapInfo();

	if(m_bFindFocusName)
	{
       RenderFocusName();
	}
}

void CLocalEntireMap::ViewingRender()
{
	float fElapsTime = g_GTimer.GetFrameElapsedSecTime();
	m_fEffCumTime += fElapsTime;

	float fRatio = m_fEffCumTime / m_fEffectTime;
	
	if( m_fEffCumTime > m_fEffectTime )
	{
		m_MapState = LEntState_Nomal;
		m_fEffCumTime = 0.0f;		
		nRui->SendEvent( WID_MiniMapWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		return;
	}

	SRect sRect, eRect, renRect;
	sRect = *(m_pRefMiniMap->GetMapClientRect());
	eRect = m_WndRect;
	
	renRect.x = sRect.x + ( fRatio * (eRect.x - sRect.x) );
	renRect.y = sRect.y + ( fRatio * (eRect.y - sRect.y) );
	renRect.w = sRect.w + ( fRatio * (eRect.w - sRect.w) );
	renRect.h = sRect.h + ( fRatio * (eRect.h - sRect.h) );

	Render->Draw( TID_CLIENT, renRect.x, renRect.y, renRect.w, renRect.h, D3DCOLOR_ARGB( 40, 192, 174, 90 ) );
}

void CLocalEntireMap::ClosingRender()
{
	float fElapsTime = g_GTimer.GetFrameElapsedSecTime();
	m_fEffCumTime += fElapsTime;
	
	float fRatio = m_fEffCumTime / m_fEffectTime;
	
	if( m_fEffCumTime > m_fEffectTime )
	{
		m_MapState = LEntState_Nomal;
		m_fEffCumTime = 0.0f;		
		nRui->SendEvent( WID_MiniMapWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

		SetVisible(false);
		return;
	}

	SRect sRect, eRect, renRect;

	sRect = m_WndRect;
	eRect = *(m_pRefMiniMap->GetMapClientRect());	
	
	renRect.x = sRect.x + ( fRatio * (eRect.x - sRect.x) );
	renRect.y = sRect.y + ( fRatio * (eRect.y - sRect.y) );
	renRect.w = sRect.w + ( fRatio * (eRect.w - sRect.w) );
	renRect.h = sRect.h + ( fRatio * (eRect.h - sRect.h) );

	Render->Draw( TID_CLIENT, renRect.x, renRect.y, renRect.w, renRect.h, D3DCOLOR_ARGB( 40, 192, 174, 90 ) );
}

void CLocalEntireMap::RenderFrame()
{
	Render->Draw( TID_CLIENT, m_NorthFrm.x, m_NorthFrm.y, m_NorthFrm.w, m_NorthFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
	Render->Draw( TID_CLIENT, m_Norm_astFrm.x, m_Norm_astFrm.y, m_Norm_astFrm.w, m_Norm_astFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
	Render->Draw( TID_CLIENT, m_EastFrm.x, m_EastFrm.y, m_EastFrm.w, m_EastFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
	Render->Draw( TID_CLIENT, m_Soum_astFrm.x, m_Soum_astFrm.y, m_Soum_astFrm.w, m_Soum_astFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
	Render->Draw( TID_CLIENT, m_SouthFrm.x, m_SouthFrm.y, m_SouthFrm.w, m_SouthFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
	Render->Draw( TID_CLIENT, m_SouthWestFrm.x, m_SouthWestFrm.y, m_SouthWestFrm.w, m_SouthWestFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
	Render->Draw( TID_CLIENT, m_WestFrm.x, m_WestFrm.y, m_WestFrm.w, m_WestFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
	Render->Draw( TID_CLIENT, m_NorthWestFrm.x, m_NorthWestFrm.y, m_NorthWestFrm.w, m_NorthWestFrm.h, D3DCOLOR_ARGB( m_uiMapAlpha, 192, 174, 90 ) );
}

void CLocalEntireMap::SetActiveMap(bool bFlag)
{
	if(bFlag)
	{
		m_MapState = LEntState_Viewing;
	}
	else
	{
		m_MapState = LentState_Closing;
	}
	
	
	TEXID texID = m_pRefMiniMap->GetMapClientImage();
	STexture*	Tex = Resource->GetTexture( texID );

	if( (Tex != NULL) && bFlag )
	{
		STexture	tmpTex;
		memcpy( &tmpTex, Tex, sizeof( STexture ) );

		SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );		

		int center_x	= nRui->Desktop.Width / 2;
		int center_y	= nRui->Desktop.Height / 2;
		
		m_WndRect.w = m_pRefMiniMap->GetTexMapSize(); //Img->Width;
		m_WndRect.h = m_pRefMiniMap->GetTexMapSize(); //Img->Height;
		m_WndRect.x = center_x - (m_WndRect.w / 2);
		m_WndRect.y = center_y - (m_WndRect.h / 2);

		ResetSize(&m_WndRect);
		MapClientImage = texID;
		
		CalcFrameSize();
	}	
	
	if(bFlag)
	{
		SetVisible(bFlag);
	}
}

void CLocalEntireMap::ResetActiveMap()
{
	TEXID texID = m_pRefMiniMap->GetMapClientImage();
	STexture*	Tex = Resource->GetTexture( texID );

	if( Tex != NULL )
	{
		STexture	tmpTex;
		memcpy( &tmpTex, Tex, sizeof( STexture ) );

		SImage* Img = Resource->GetImageInfo( tmpTex.ImgId );		

		int center_x	= nRui->Desktop.Width / 2;
		int center_y	= nRui->Desktop.Height / 2;
		
		m_WndRect.w = tmpTex.w;
		m_WndRect.h = tmpTex.h;
		m_WndRect.x = center_x - (m_WndRect.w / 2);
		m_WndRect.y = center_y - (m_WndRect.h / 2);

		ResetSize(&m_WndRect);
		MapClientImage = texID;
		
		CalcFrameSize();
	}
}

void  CLocalEntireMap::CalcFrameSize()
{
	//위
	m_NorthFrm.x = m_WndRect.x;
	m_NorthFrm.y = m_WndRect.y - m_FrameWidth;
	m_NorthFrm.w = m_WndRect.w;
	m_NorthFrm.h = m_FrameWidth;
	//
	m_Norm_astFrm.x = m_WndRect.x + m_WndRect.w;
	m_Norm_astFrm.y = m_WndRect.y - m_FrameWidth;
	m_Norm_astFrm.w = m_FrameWidth;
	m_Norm_astFrm.h = m_FrameWidth;
	//오른쪽
	m_EastFrm.x = m_WndRect.x + m_WndRect.w;
	m_EastFrm.y = m_WndRect.y;
	m_EastFrm.w = m_FrameWidth;
	m_EastFrm.h = m_WndRect.h;
	//
	m_Soum_astFrm.x = m_WndRect.x + m_WndRect.w;
	m_Soum_astFrm.y = m_WndRect.y + m_WndRect.h;
	m_Soum_astFrm.w = m_FrameWidth;
	m_Soum_astFrm.h = m_FrameWidth;
	//아래
	m_SouthFrm.x = m_WndRect.x;
	m_SouthFrm.y = m_WndRect.y + m_WndRect.h;
	m_SouthFrm.w = m_WndRect.w;
	m_SouthFrm.h = m_FrameWidth;
	//
	m_SouthWestFrm.x = m_WndRect.x - m_FrameWidth;
	m_SouthWestFrm.y = m_WndRect.y + m_WndRect.h;
	m_SouthWestFrm.w = m_FrameWidth;
	m_SouthWestFrm.h = m_FrameWidth;
	//왼쪽
	m_WestFrm.x = m_WndRect.x - m_FrameWidth;
	m_WestFrm.y = m_WndRect.y;
	m_WestFrm.w = m_FrameWidth;
	m_WestFrm.h = m_WndRect.h;
	//
	m_NorthWestFrm.x = m_WndRect.x - m_FrameWidth;
	m_NorthWestFrm.y = m_WndRect.y - m_FrameWidth;
	m_NorthWestFrm.w = m_FrameWidth;
	m_NorthWestFrm.h = m_FrameWidth;
}

void CLocalEntireMap::RenderSimbolMarkWholeRange()
{
	SPoint2	screen_char_pos;
	SPoint2	tex_nchar_pos;
	SPoint2	grid_pos;

	int iRealMapSize = m_pRefMiniMap->GetRealMapSize();
	int iTexMapSize = m_pRefMiniMap->GetTexMapSize();	

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
		
		grid_pos.x = ( xfPos / m_tilesize );
		grid_pos.y = ( zfPos / m_tilesize );
		grid_pos.y = iRealMapSize - grid_pos.y;//좌표 뒤집기....
				
		tex_nchar_pos.x = ( iTexMapSize * grid_pos.x ) / iRealMapSize;
		tex_nchar_pos.y = ( iTexMapSize * grid_pos.y ) / iRealMapSize;	

		screen_char_pos.x = tex_nchar_pos.x;
		screen_char_pos.y = tex_nchar_pos.y;
		screen_char_pos.x += m_WndRect.x;
		screen_char_pos.y += m_WndRect.y;	

		///-- 중립, 밀레나, 레인진영에 따라 다른 이미지 보여주기
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

void CLocalEntireMap::RenderUnitMarkInViewRange()
{
	SPoint2	screen_char_pos;
	SPoint2	tex_nchar_pos;
	SPoint2	grid_pos;

	int iRealMapSize = m_pRefMiniMap->GetRealMapSize();
	int iTexMapSize = m_pRefMiniMap->GetTexMapSize();
	
	for(int i = 0; i < MAX_CHARACTER; ++i )
	{
		if(	m_pPcInfo[i].existence == true )
		{
			grid_pos.x = (m_pPcInfo[i].wpos.x / m_tilesize);
			grid_pos.y = (m_pPcInfo[i].wpos.z / m_tilesize);
			grid_pos.y = iRealMapSize - grid_pos.y;//좌표 뒤집기....
			
			tex_nchar_pos.x = ( iTexMapSize * grid_pos.x ) / iRealMapSize;
			tex_nchar_pos.y = ( iTexMapSize * grid_pos.y ) / iRealMapSize;			
	
				screen_char_pos.x = tex_nchar_pos.x;
				screen_char_pos.y = tex_nchar_pos.y;
				screen_char_pos.x += m_WndRect.x;
				screen_char_pos.y += m_WndRect.y;
				
				if( m_pPcInfo[i].ctype == enNPC )
				{
					int iFindId = -1;
					//파티 멤버인지 찾는다. 파티멤버는 푸른색으로 표시
					SPARTYMEMBER_INFO * pMemberInfo = NULL;
					pMemberInfo = g_PartyInforWnd.FindPartyMember(m_pPcInfo[i].id);
					if(pMemberInfo)
					{
						iFindId = m_pPcInfo[i].id;		
					}
                    
					if(iFindId != -1)			
					{
				         RenderTextureMark(TID_mini_party, screen_char_pos, 12, 12, -1);
					     CheckFocusPosition(screen_char_pos, 12, 12, m_pPcInfo[i].name);										
					}
				}		
		}
	}
}

void CLocalEntireMap::RenderUnitMarkOutViewRange()
{
	SPoint2	screen_char_pos;
	SPoint2	tex_nchar_pos;
	SPoint2	grid_pos;

	int iRealMapSize = m_pRefMiniMap->GetRealMapSize();
	int iTexMapSize = m_pRefMiniMap->GetTexMapSize();

	//맵연관 NPC위치 찍기
	list< SMinimapNPCInfo * >::iterator ni = m_pRefMiniMap->m_NPCInfoList.begin();
	list< SMinimapNPCInfo * >::iterator nend = m_pRefMiniMap->m_NPCInfoList.end();

	for( ; ni != nend ; ni++ )
	{
		SMinimapNPCInfo * pNPCInfo = (*ni);

		if( (pNPCInfo->nCellPosX == 0) && (pNPCInfo->nCellPosY == 0) )
			continue;

		float xfPos = ( pNPCInfo->nCellPosX * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );
		float zfPos = ( pNPCInfo->nCellPosY * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );

		grid_pos.x = ( xfPos / m_tilesize );
		grid_pos.y = ( zfPos / m_tilesize );
		grid_pos.y = iRealMapSize - grid_pos.y; //좌표 뒤집기....
				
		tex_nchar_pos.x = ( iTexMapSize * grid_pos.x ) / iRealMapSize;
		tex_nchar_pos.y = ( iTexMapSize * grid_pos.y ) / iRealMapSize;

	    screen_char_pos.x = tex_nchar_pos.x;
		screen_char_pos.y = tex_nchar_pos.y;
		screen_char_pos.x += m_WndRect.x;
		screen_char_pos.y += m_WndRect.y;			
			
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

	//파티관련 위치찍기
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
				
				grid_pos.x = ( xfPos / m_tilesize );
				grid_pos.y = ( zfPos / m_tilesize );
				grid_pos.y = iRealMapSize - grid_pos.y;//좌표 뒤집기....
				
				tex_nchar_pos.x = ( iTexMapSize * grid_pos.x ) / iRealMapSize;
				tex_nchar_pos.y = ( iTexMapSize * grid_pos.y ) / iRealMapSize;				
			
				screen_char_pos.x = tex_nchar_pos.x;
				screen_char_pos.y = tex_nchar_pos.y;
				screen_char_pos.x += m_WndRect.x;
				screen_char_pos.y += m_WndRect.y;					
				
				RenderTextureMark(TID_mini_party, screen_char_pos, 12, 12, -1);		 
				CheckFocusPosition(screen_char_pos, 12, 12, (pMemberInfo->m_strPCName) );				
			}
		}
	}
}

void CLocalEntireMap::RenderMyUnitMark()
{
	SPoint2	screen_char_pos;
	SPoint2	tex_nchar_pos;
	SPoint2	grid_pos;

	int iRealMapSize = m_pRefMiniMap->GetRealMapSize();
	int iTexMapSize = m_pRefMiniMap->GetTexMapSize();
	
	Player*	user_pc = g_Pc.GetPlayer();	
	
	D3DXMATRIX		matchar;	
	matchar = user_pc->GetPosTM();

	grid_pos.x = (matchar._41 / m_tilesize);
	grid_pos.y = (matchar._43 / m_tilesize);
	grid_pos.y = iRealMapSize - grid_pos.y;//좌표 뒤집기.... 3D공간좌표 -> 2D스크린좌표로..

	tex_nchar_pos.x = ( iTexMapSize * grid_pos.x ) / iRealMapSize;
	tex_nchar_pos.y = ( iTexMapSize * grid_pos.y ) / iRealMapSize;				

	screen_char_pos.x = tex_nchar_pos.x;
	screen_char_pos.y = tex_nchar_pos.y;
	screen_char_pos.x += m_WndRect.x;
	screen_char_pos.y += m_WndRect.y;

	int angle = (int)D3DXToDegree( user_pc->m_radian ) % 360;

	int direction = m_pRefMiniMap->GetAngleToDirection( angle ) + TID_Direction0; //Direction TID가 472부터 시작....
	RenderTextureMark(direction, screen_char_pos, 16, 16, -1);
}

void CLocalEntireMap::RenderLocalMapInfo()
{
	static TCHAR TempBuf[50] = _T("");
	MiniMapWnd.InfoText256[0]->GetText(TempBuf, 50);
	int iXpos, iYpos;
	iXpos = m_WndRect.x + 3;
	iYpos = m_WndRect.y + 3;

	Render->DrawText(m_pFontg, TempBuf, iXpos, iYpos, D3DCOLOR_ARGB(m_uiMapAlpha, 255, 255, 255 ));

	MiniMapWnd.InfoText256[1]->GetText(TempBuf, 50);

	SStringInfo StrInfo;
	m_pFontg->GetStringInfo( TempBuf, &StrInfo );
	iXpos = (m_WndRect.x + m_WndRect.w) - (3 + StrInfo.Width);
	iYpos = (m_WndRect.y + m_WndRect.h) - (3 + StrInfo.Height);

	Render->DrawText(m_pFontg, TempBuf, iXpos, iYpos, D3DCOLOR_ARGB(m_uiMapAlpha, 255, 255, 255 ));
}

void CLocalEntireMap::CheckFocusPosition(SPoint2 Src_Cen_Pos, int width, int height, TCHAR *strName )
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

void CLocalEntireMap::RenderFocusName()
{
    nRui->GetHelpTip()->SetString( &m_FocusRect,  m_strFocusName, 
		                             D3DCOLOR_XRGB( 255, 255, 255 ));
	nRui->GetHelpTip()->RenderProc(0.47f, D3DCOLOR_XRGB( 255, 255, 255 ));
}

void CLocalEntireMap::RenderTextureMark(UINT Tid, SPoint2 CenPos, int width, int height, int iExtraGlowMark )
{
   SPoint2 sPoint; 
   sPoint.x = CenPos.x - (width/2);
   sPoint.y = CenPos.y - (height/2);
   
   Render->Draw( Tid, sPoint.x, sPoint.y, width, height, D3DCOLOR_ARGB(m_uiMapAlpha, 255, 255, 255 ) );

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

   D3DCOLOR Color = D3DCOLOR_ARGB((int)(afRatio * 255), 255, 255, 255 );

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
}

