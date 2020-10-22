///////////////////////////////////////////////////////////////////////////////
///
///		File		: NonPlayerInfoMiniBar.h
///		Desc		:
///
///		Author		: NiceguY
///		Team		: Program - Client Team
///		Date		: 2004-12-02
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Rui.h"

#include "NonPlayerInfoMiniBar.h"
#include "NonPlayerInfoWnd.h"
#include "CBillBoardUIRender.h"

#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"


const WORD c_NonPlayerInfoMiniBar_Width( 64 );
const WORD c_NonPlayerInfoMiniBar_Height( 6 );


//-----------------------------------------------------------------------------
CNonPlayerInfoMiniBar::CNonPlayerInfoMiniBar()
{
	thePickedIndex = -1;
	m_isNpcHpView = false ; 
	
}

//-----------------------------------------------------------------------------
CNonPlayerInfoMiniBar::CNonPlayerInfoMiniBar( SDesktop* DT )
{
	Initialize( DT );
}

//-----------------------------------------------------------------------------
CNonPlayerInfoMiniBar::~CNonPlayerInfoMiniBar()
{
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoMiniBar::Initialize( SDesktop* DT )
{
	theDT		= DT;
	
	thePickedIndex = -1;
	m_isNpcHpView = false ; 
	
	return;
}

//-----------------------------------------------------------------------------
bool CNonPlayerInfoMiniBar::CalcMiniBar()
{
	Character* pChar = ( Character* )( g_RockClient.m_PcInfo[thePickedIndex].pCharacter );
	if( pChar == NULL )
		return false;
	
	float percent = 100.0f;
	//	percent = NonPlayerInfoWnd.theHP->GetPercentage();
	
	if( pChar->m_bPC )
	{
		if( pChar->m_nTableIndex == MAX_USER_INTABLE )
		{
			percent = ((float)nRui->thePcParam.Stat1.theHP / (float)nRui->thePcParam.MaxHP) * 100;
		}
		else
		{
			if( pChar->m_CharAlpha.State == CHAR_NONE_DISPLAY )
				return false;
			
			if(!pChar->m_IsAllLTexture)
				return false;
			
			percent = ((float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theHP / (float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theMaxHP) * 100;
		}
	}
	else
	{
		if( g_Map.m_NpcTable[pChar->m_nTableIndex].race == c_NpcRace_Item )			// 아이템은 체력바 그려줄 필요 없잖아~ 
			return false;
		
		percent = ((float)g_Map.m_NpcTable[pChar->m_nTableIndex].theHP / (float)g_Map.m_NpcTable[pChar->m_nTableIndex].theMaxHP) * 100;
	}
	
	if( percent <= 0 ){	percent = 0;	}
	if( percent >= 100 ){	percent = 100;	}	
	
	// HP Bar ForeGround
	theHP_Bar.x = nRui->thePcInfo[thePickedIndex].spos.x - ( c_NonPlayerInfoMiniBar_Width / 2 );
	theHP_Bar.y = nRui->thePcInfo[thePickedIndex].spos.y - c_NonPlayerInfoMiniBar_Height + 10;
	//theHP_Bar.y = nRui->thePcInfo[thePickedIndex].spos.y + pChar->m_fMaxHeight - c_NonPlayerInfoMiniBar_Height;
	theHP_Bar.w = c_NonPlayerInfoMiniBar_Width * ( percent * 0.01f );
	theHP_Bar.h = c_NonPlayerInfoMiniBar_Height;
	
	// HP Bar BackGround
	theHP_Back.x = theHP_Bar.x;
	theHP_Back.y = theHP_Bar.y;
	theHP_Back.w = c_NonPlayerInfoMiniBar_Width;
	theHP_Back.h = theHP_Bar.h;
	
	// SMP Bar ForeGround
	theSMP_Bar.x = theHP_Bar.x;
	theSMP_Bar.y = theHP_Bar.y + 9;
	theSMP_Bar.w = theHP_Bar.w;
	theSMP_Bar.h = theHP_Bar.h;
	
	// SMP Bar BackGround
	theSMP_Back.x = theSMP_Bar.x;
	theSMP_Back.y = theSMP_Bar.y;
	theSMP_Back.w = c_NonPlayerInfoMiniBar_Width;
	theSMP_Back.h = theSMP_Bar.h;
	
	return true;
}

bool CNonPlayerInfoMiniBar::CalcMiniBarAll(int index)
{
	
	Character* pChar = ( Character* )( g_RockClient.m_PcInfo[index].pCharacter );
	if( pChar == NULL )
		return false;
	
	float percent = 100.0f;
	float percentMp = 100.0f;

	
	//	percent = NonPlayerInfoWnd.theHP->GetPercentage();
	
	if( pChar->m_bPC )
	{
		if( pChar->m_nTableIndex == MAX_USER_INTABLE )
		{
			percent = ((float)nRui->thePcParam.Stat1.theHP / (float)nRui->thePcParam.MaxHP) * 100;
			if( nRui->thePcParam.Citizen == MILLENA)
			{
				percentMp = ((float)nRui->thePcParam.Stat1.theSP / (float)nRui->thePcParam.MaxSP) * 100;
			}
			else 
			{
				percentMp = ((float)nRui->thePcParam.Stat1.theMP / (float)nRui->thePcParam.MaxMP) * 100;
			}
		}
		else
		{
			if( pChar->m_CharAlpha.State == CHAR_NONE_DISPLAY )
				return false;
			
			if(!pChar->m_IsAllLTexture)
				return false;

			percent = ((float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theHP / (float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theMaxHP) * 100;
						
		
			{
							
			//	if( g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theCitizen == MILLENA)
				{
					percentMp = ((float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theSMP / (float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theMaxSMP) * 100;
				}
//				else 
//				{
//					percentMp = ((float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theMP / (float)g_Pc.m_PcTable[pChar->m_nTableIndex].char_info.theMaxMP) * 100;
//				}
//	
			}
	
		}
	}
	else
	{
		if( g_Map.m_NpcTable[pChar->m_nTableIndex].race == c_NpcRace_Item )			// 아이템은 체력바 그려줄 필요 없잖아~ 
			return false;
		
		percent = ((float)g_Map.m_NpcTable[pChar->m_nTableIndex].theHP / (float)g_Map.m_NpcTable[pChar->m_nTableIndex].theMaxHP) * 100;
	}
	
	if( percent <= 0 ){	percent = 0;	}
	if( percent >= 100 ){	percent = 100;	}	
	
	if( percentMp <= 0 ){	percentMp = 0;	}
	if( percentMp >= 100 ){	percentMp = 100;	}	
	
	

	// HP Bar ForeGround
	theHP_Bar.x = nRui->thePcInfo[index].spos.x - ( c_NonPlayerInfoMiniBar_Width / 2 );
	theHP_Bar.y = nRui->thePcInfo[index].spos.y - c_NonPlayerInfoMiniBar_Height + 10;
	//theHP_Bar.y = nRui->thePcInfo[thePickedIndex].spos.y + pChar->m_fMaxHeight - c_NonPlayerInfoMiniBar_Height;
	theHP_Bar.w = c_NonPlayerInfoMiniBar_Width * ( percent * 0.01f );
	theHP_Bar.h = c_NonPlayerInfoMiniBar_Height;
	
	// HP Bar BackGround
	theHP_Back.x = theHP_Bar.x;
	theHP_Back.y = theHP_Bar.y;
	theHP_Back.w = c_NonPlayerInfoMiniBar_Width;
	theHP_Back.h = theHP_Bar.h;
	
	// SMP Bar ForeGround
	theSMP_Bar.x = theHP_Bar.x;
	theSMP_Bar.y = theHP_Bar.y + 9;
	theSMP_Bar.w = c_NonPlayerInfoMiniBar_Width * ( percentMp * 0.01f ); 
	theSMP_Bar.h = theHP_Bar.h;
	
	// SMP Bar BackGround
	theSMP_Back.x = theSMP_Bar.x;
	theSMP_Back.y = theSMP_Bar.y;
	theSMP_Back.w = c_NonPlayerInfoMiniBar_Width;
	theSMP_Back.h = theSMP_Bar.h;
	
	
	return true;
}

//----------------------------------------------------------------------------
void CNonPlayerInfoMiniBar::RenderProc()
{
	//	return;
	
	if(m_isNpcHpView)
	{
		for( int i = 0; i < nRui->thePcinfoCount ; ++i)
		{
			if(nRui->thePcInfo[i].pCharacter != NULL)
			{
				if(CalcMiniBarAll(i))
				{
					RenderGameUIAll(i);
				}
			
			}
		}		
	}
	
	
	if( thePickedIndex == -1 )
	{
		return;
	}
	
	if( CalcMiniBar() == false )
	{
		return;
	}
	
    //by simwoosung
	
	RenderGameUI();
	//RenderGameBillUI();    
	
	//	Render->Draw( TID_NonPlayerInfo_Gauge_Back, theHP_Back.x, theHP_Back.y, theHP_Back.w, theHP_Back.h, D3DCOLOR_ARGB( 255, 100, 100, 100 ) );
	//	Render->Draw( TID_NonPlayerInfo_Gauge_Back, theSMP_Back.x, theSMP_Back.y, theSMP_Back.w, theSMP_Back.h, D3DCOLOR_ARGB( 255, 100, 100, 100 ) );
	//	Render->Draw( TID_NonPlayerInfo_HP_Gauge, theHP_Bar.x, theHP_Bar.y, theHP_Bar.w, theHP_Bar.h, D3DCOLOR_ARGB( 255, 255, 152, 136 ) );
	//	Render->Draw( TID_NonPlayerInfo_MP_Gauge, theSMP_Bar.x, theSMP_Bar.y, theSMP_Bar.w, theSMP_Bar.h, D3DCOLOR_ARGB( 255, 52, 227, 255 ) );
	
	//			theFontg2->GetStringInfo( thePcInfo[thePickPcIdx].name, &StrInfo );
	//			Render->DrawText( theFontg2, thePcInfo[thePickPcIdx].name,
	//							thePcInfo[thePickPcIdx].spos.x - ( StrInfo.Width / 2 ),
	//							thePcInfo[thePickPcIdx].spos.y - ( StrInfo.Height ),
	//							D3DCOLOR_XRGB( 162, 147, 85 ) );
	
	return;
}

void CNonPlayerInfoMiniBar::RenderGameBillUI()
{
	
}

void CNonPlayerInfoMiniBar::RenderGameUI()
{
	D3DXVECTOR3 vPos;
	vPos.x = nRui->thePcInfo[thePickedIndex].wpos.x;
	vPos.y = nRui->thePcInfo[thePickedIndex].wpos.y;
	vPos.z = nRui->thePcInfo[thePickedIndex].wpos.z;
	
	Character *pPlayer = (Character *)(nRui->thePcInfo[thePickedIndex].pCharacter);
	vPos.y += (pPlayer->m_fMaxHeight);
	
	D3DXMATRIX mat;
	D3DXMatrixMultiply(&mat, &(g_Camera.m_matView), &(g_Camera.m_matProj));			
	D3DXVec3TransformCoord(&vPos, &vPos, &mat);	
	
	D3DCOLOR minibar_color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Render->Draw( TID_NonPlayerInfoMiniBar_Gauge_Back, theHP_Back.x, theHP_Back.y, theHP_Back.w, theHP_Back.h, minibar_color, vPos.z);
	Render->Draw( TID_NonPlayerInfoMiniBar_HP_Gauge, theHP_Bar.x, theHP_Bar.y, theHP_Bar.w, theHP_Bar.h, minibar_color, vPos.z );
}

void CNonPlayerInfoMiniBar::RenderGameUIAll(int index)
{
	
	D3DXVECTOR3 vPos;
	vPos.x = nRui->thePcInfo[index].wpos.x;
	vPos.y = nRui->thePcInfo[index].wpos.y;
	vPos.z = nRui->thePcInfo[index].wpos.z;
	
	Character *pPlayer = (Character *)(nRui->thePcInfo[index].pCharacter);
	vPos.y += (pPlayer->m_fMaxHeight);
	
	D3DXMATRIX mat;
	D3DXMatrixMultiply(&mat, &(g_Camera.m_matView), &(g_Camera.m_matProj));			
	D3DXVec3TransformCoord(&vPos, &vPos, &mat);	
	
	D3DCOLOR minibar_color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	D3DCOLOR minibar_colorMp = D3DCOLOR_ARGB( 255, 0, 0, 255 );
	// 	if(g_RockClient.m_PcInfo[index].ctype == enNPC)


	if(pPlayer->m_bPC)
	{
		Render->Draw( TID_NonPlayerInfoMiniBar_Gauge_Back, theHP_Back.x, theHP_Back.y, theHP_Back.w, theHP_Back.h, minibar_color, vPos.z);
		Render->Draw( TID_NonPlayerInfoMiniBar_HP_Gauge, theHP_Bar.x, theHP_Bar.y, theHP_Bar.w, theHP_Bar.h, minibar_color, vPos.z );
			
		Render->Draw( TID_NonPlayerInfoMiniBar_Gauge_Back, theSMP_Back.x, theSMP_Back.y, theSMP_Back.w, theSMP_Back.h, minibar_color, vPos.z);
		Render->Draw( TID_NonPlayerInfoMiniBar_HP_Gauge, theSMP_Bar.x, theSMP_Bar.y, theSMP_Bar.w, theSMP_Bar.h, minibar_colorMp, vPos.z );
	
	}
	else
	{
		Render->Draw( TID_NonPlayerInfoMiniBar_Gauge_Back, theHP_Back.x, theHP_Back.y, theHP_Back.w, theHP_Back.h, minibar_color, vPos.z);
		Render->Draw( TID_NonPlayerInfoMiniBar_HP_Gauge, theHP_Bar.x, theHP_Bar.y, theHP_Bar.w, theHP_Bar.h, minibar_color, vPos.z );
	}
	
}




//-----------------------------------------------------------------------------