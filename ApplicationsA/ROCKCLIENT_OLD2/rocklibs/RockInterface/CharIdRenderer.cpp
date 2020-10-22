//#include "Pc.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\StringManager.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "CharIdRenderer.h"
#include "EmblemTexManager.h"
#include "StringManager.h"

CCharIdRenderer::CCharIdRenderer()
{
	m_pPcInfo = NULL;
	m_pFontg = NULL;

	Init();
}

CCharIdRenderer::~CCharIdRenderer()
{

}

void CCharIdRenderer::Init(SPcInfo* pcinfo, CFontg* pFontg)
{
	m_pPcInfo = pcinfo;
	m_pFontg = pFontg;
}

void CCharIdRenderer::Init()
{
	memset(m_strTemp, 0 , sizeof(TCHAR)*256);
	memset(m_strCharName, 0 , sizeof(TCHAR)*256);
	memset(m_strPartyName, 0 , sizeof(TCHAR)*256);
	memset(m_strFrontierName, 0 , sizeof(TCHAR)*256);
	memset(m_strCallName, 0 , sizeof(TCHAR)*256);
	m_uFrontierId = 0;

	m_Name_Color =  D3DCOLOR_XRGB( 0, 0, 0 );
	m_Party_Color =  D3DCOLOR_XRGB( 0, 0, 0 );
	m_Frontier_Color = D3DCOLOR_XRGB( 164, 255, 164 );
	m_CallName_Color = D3DCOLOR_XRGB( 164, 255, 255 );

	m_nIndex = -1;

	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_IsRenderEmblen = FALSE;			
	m_IsRenderCallName = FALSE;		
	m_IsRenderCharName = FALSE;		
	m_IsRenderFtierName = FALSE;	
	m_IsRenderPartyName = FALSE;
	
	m_IsRenderCaoMark = FALSE;
}

void CCharIdRenderer::SetCharName(TCHAR *tStr)
{
	SAFE_STR_CPY( m_strCharName, tStr, 256 );
}

void CCharIdRenderer::SetPartyName(TCHAR *tStr)
{
	SAFE_STR_CPY( m_strPartyName, tStr, 256 );
}

void CCharIdRenderer::SetFrontierName(TCHAR *tStr)
{
	SAFE_STR_CPY( m_strFrontierName, tStr, 256 );
}

void CCharIdRenderer::SetCallName(TCHAR *tStr)
{
	SAFE_STR_CPY( m_strCallName, tStr, 256 );
}

void CCharIdRenderer::SetFrontierStatus(BYTE Status)
{
	m_Status = Status;
}

void CCharIdRenderer::SetNameColor(D3DCOLOR Color)
{
	m_Name_Color = Color;
}

void CCharIdRenderer::SetFrontierId(DWORD uId)
{
	m_uFrontierId = uId;
}

void CCharIdRenderer::SetPartyColor(D3DCOLOR Color)
{
	m_Party_Color = Color;
}

void CCharIdRenderer::SetFrontierColor(D3DCOLOR Color)
{
	m_Frontier_Color = Color;
}

void CCharIdRenderer::SetCallNameColor(D3DCOLOR Color)
{
	m_CallName_Color = Color;
}

void CCharIdRenderer::SetPcInfo( SPcInfo* pcinfo )
{
	m_pPcInfo = pcinfo;
}

BOOL CCharIdRenderer::SetIndex( int nIndex )
{
	Init();
	
	m_nIndex = nIndex;
	
	if( NULL ==  m_pPcInfo || NULL == m_pFontg )
		return FALSE;	
	
	Character *pPlayer = (Character *)(m_pPcInfo[m_nIndex].pCharacter);
	
	if(pPlayer == NULL)
		return FALSE;

	//파이트 존에서 관람자 모드이면 자신의 아이디를 보여주지 않는다.
	if( !g_Pc.GetPlayer()->IsMyPlayerRender && pPlayer->m_nTableIndex == MAX_USER_INTABLE )
	{
		return FALSE;		
	}
	
	//은신, 월영 사용시에는 아이디를 찍지않는다.
	if(pPlayer->m_CharAlpha.State == CHAR_NONE_DISPLAY)
		return FALSE;
	
	if(!pPlayer->m_IsAllLTexture)
		return FALSE;
	
	m_vPos.x = m_pPcInfo[m_nIndex].wpos.x;
	m_vPos.y = m_pPcInfo[m_nIndex].wpos.y;
	m_vPos.z = m_pPcInfo[m_nIndex].wpos.z;	
	
	m_vPos.y += (pPlayer->m_fMaxHeight);	

	D3DXMATRIX mat;
	D3DXMatrixMultiply(&mat, &(g_Camera.m_matView), &(g_Camera.m_matProj));			
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &mat);
	
	
	return TRUE;
}

void CCharIdRenderer::RecallLayout()
{
	SStringInfo			StrInfo;
	SStringInfo			StrInfo2;
	TCHAR				tstrTitle[256];
	TCHAR				tstrTemp[256];
	
	//이쪽에서 외곽 배경의 사이즈와 각 엠블렌과 이름부의 
	//위치를 계산한다.	

	//호칭부와 이름부의 영역을 계산한다.	
	memset(&StrInfo, 0 , sizeof(SStringInfo) );
	memset(&StrInfo2, 0 , sizeof(SStringInfo) );
	memset(tstrTitle, 0 , sizeof(TCHAR)*256);
	memset(tstrTemp, 0 , sizeof(TCHAR)*256);

	Character *pPlayer = (Character *)(m_pPcInfo[m_nIndex].pCharacter);
	pPlayer->m_nAddNum = 0;
	
	m_IsRenderCharName = TRUE;
	
	if( Rstrcmp( m_strCallName, _RT("") ) == 0 )		//호칭이 존재하지 않으면...
	{
		Rsprintf(RWCHAR(tstrTitle), _RT("%s"), RWCHAR(m_strCharName));
		m_pFontg->GetStringInfo( tstrTitle, &StrInfo );
		
		float fXpos = m_pPcInfo[m_nIndex].spos.x - ( StrInfo.Width / 2 );
		float fYpos = m_pPcInfo[m_nIndex].spos.y - ( StrInfo.Height );	
		
		m_IsRenderCallName = FALSE;
		m_CallNameRect = SRect(0.0f, 0.0f, 0.0f, 0.0f);		
		m_CharNameRect = SRect( fXpos, fYpos, StrInfo.Width ,StrInfo.Height );	
	}
	else
	{
		Rsprintf(RWCHAR(tstrTitle), _RT("[%s] %s"), RWCHAR(m_strCallName), RWCHAR(m_strCharName) );
		Rsprintf(RWCHAR(tstrTemp), _RT("[%s] "),RWCHAR(m_strCallName));
		m_pFontg->GetStringInfo( tstrTitle, &StrInfo );
		m_pFontg->GetStringInfo( tstrTemp, &StrInfo2 );

		float fXpos = m_pPcInfo[m_nIndex].spos.x - ( StrInfo.Width / 2 );
		float fYpos = m_pPcInfo[m_nIndex].spos.y - ( StrInfo.Height );
		
		m_IsRenderCallName = TRUE;
		m_CallNameRect = SRect( fXpos, fYpos, 0 ,0 );
		m_CharNameRect = SRect( fXpos + StrInfo2.Width, fYpos, 0 ,StrInfo.Height );		
	}

	float fWBlank = 8.0f;
	float fHBlank = 4.0f;
    float fWidth = ( StrInfo.Width  +  fWBlank );
	float fHeight = ( StrInfo.Height + fHBlank );

	m_FrameRect.x = m_pPcInfo[m_nIndex].spos.x - ( fWidth / 2 );
	m_FrameRect.y = m_pPcInfo[m_nIndex].spos.y - ( fHeight - fHBlank/2 );
	m_FrameRect.w = ( StrInfo.Width  + fWBlank );
	m_FrameRect.h = ( StrInfo.Height + fHBlank );	

	if( Rstrcmp( m_strFrontierName, _RT("") ) != 0 )		//프론티어 네임이 존재하면...
	{
		if(n_Preliminary_Frontier == m_Status)
		{
			Rsprintf(RWCHAR(tstrTitle), _RT("(%s) %s"), G_STRING_CHAR( IDS_WND_PRE ) , RWCHAR(m_strFrontierName));
		}
		else
		{
			Rsprintf(RWCHAR(tstrTitle), _RT("%s"), RWCHAR(m_strFrontierName));
		}	

		m_pFontg->GetStringInfo( tstrTitle, &StrInfo );
		
		float fXpos = m_pPcInfo[m_nIndex].spos.x - ( StrInfo.Width / 2 );
		float fYpos = m_pPcInfo[m_nIndex].spos.y - ( StrInfo.Height ) - ( m_FrameRect.h - 2 );
				
		m_IsRenderFtierName = TRUE;		
		
		m_FtierNameRect = SRect( fXpos , fYpos, 0 ,StrInfo.Height );	
		
		float fWidth = ( StrInfo.Width  +  fWBlank );
		if( m_FrameRect.x > m_pPcInfo[m_nIndex].spos.x - ( fWidth / 2 ) )
		{
			m_FrameRect.x = m_pPcInfo[m_nIndex].spos.x - ( fWidth / 2 );
			m_FrameRect.w = ( StrInfo.Width  + fWBlank );
		}
		
		m_FrameRect.y -= ( m_CharNameRect.h + 3 );
		m_FrameRect.h += (StrInfo.Height + 3);

		pPlayer->m_nAddNum += (StrInfo.Height + 3);
	}	

	if( Rstrcmp( m_strPartyName, _RT("") ) != 0 )	  //파티나 리그네임이 존재하면
	{
		Rsprintf(RWCHAR(tstrTitle), _RT("-%s-"), RWCHAR(m_strPartyName));
		m_pFontg->GetStringInfo( tstrTitle, &StrInfo );
		
		float fXpos = m_pPcInfo[m_nIndex].spos.x - ( StrInfo.Width / 2 );
		float fYpos = m_pPcInfo[m_nIndex].spos.y - ( StrInfo.Height ) - ( m_FrameRect.h - 2 );
				
		m_IsRenderPartyName = TRUE;		
		
		m_PartyNameRect = SRect( fXpos , fYpos, 0 ,0 );	

		float fWidth = ( StrInfo.Width  +  fWBlank );
		if( m_FrameRect.x > m_pPcInfo[m_nIndex].spos.x - ( fWidth / 2 ) )
		{
			m_FrameRect.x = m_pPcInfo[m_nIndex].spos.x - ( fWidth / 2 );
			m_FrameRect.w = ( StrInfo.Width  + fWBlank );
		}
		
		m_FrameRect.y -= ( m_CharNameRect.h + 3 );
		m_FrameRect.h += (StrInfo.Height + 3);

		pPlayer->m_nAddNum += (StrInfo.Height + 3);
	}
	
	if(m_IsRenderFtierName && m_uFrontierId)
	{
		m_EmblenRect.x = m_FrameRect.x - 24;
		m_EmblenRect.y = m_FtierNameRect.y;
		
		m_IsRenderEmblen = TRUE;
	}

#ifdef CPK_SYSTEM	

	if( m_IsRenderCaoMark )
	{
		m_CaoMarkRect.x = m_FrameRect.x + m_FrameRect.w;
		if(m_IsRenderEmblen)
		{	
			m_CaoMarkRect.y = m_FtierNameRect.y;
		}
		else
		{
			m_CaoMarkRect.y = m_FrameRect.y + m_FrameRect.h - 24;
		}
	}

#endif

}

void CCharIdRenderer::RenderProc()
{
	SVertex vt[4];
	
	vt[0].diffuse	= D3DCOLOR_ARGB( 100 , 0 , 0 , 0  );
    vt[3].diffuse = vt[2].diffuse = vt[1].diffuse = vt[0].diffuse;
	
	vt[0].x = m_FrameRect.x - 0.5f;
    vt[0].y = m_FrameRect.y - 0.5f;		
    vt[1].x = vt[0].x + m_FrameRect.w;
    vt[1].y = vt[0].y;
    vt[2].x = vt[0].x + m_FrameRect.w;
    vt[2].y = vt[0].y + m_FrameRect.h;
    vt[3].x = vt[0].x;
    vt[3].y = vt[0].y + m_FrameRect.h;

	vt[0].z = vt[1].z = vt[2].z = vt[3].z = m_vPos.z;
	
    g_RenderManager.SetTexture( 0, NULL );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, vt, sizeof( SVertex ) );

	g_RenderManager.SetZBias( 2 );

	if(m_IsRenderPartyName)
	{
		Rsprintf(RWCHAR(m_strTemp), _RT("-%s-"),RWCHAR(m_strPartyName));	   
		Render->DrawText( m_pFontg, m_strTemp , m_PartyNameRect.x, m_PartyNameRect.y,
						  m_Party_Color, m_vPos.z );
	}

	if(m_IsRenderFtierName)
	{		
		if(n_Preliminary_Frontier == m_Status)
		{
			Rsprintf(RWCHAR(m_strTemp), _RT("(%s) %s"), G_STRING_CHAR( IDS_WND_PRE ) , RWCHAR(m_strFrontierName));
		}
		else
		{
			Rsprintf(RWCHAR(m_strTemp), _RT("%s"), RWCHAR(m_strFrontierName));
		}	
  
		Render->DrawText( m_pFontg, m_strTemp , m_FtierNameRect.x, m_FtierNameRect.y,
						  m_Frontier_Color, m_vPos.z );
	}

	if(m_IsRenderFtierName && m_IsRenderEmblen)
	{
		VESIONTEX *pVesionTex = g_EmblemTexManager.FindForntierVersionTex(m_uFrontierId);
		if(pVesionTex)
		{
			g_EmblemTexManager.RenderFrontierImg(pVesionTex, m_EmblenRect.x, m_EmblenRect.y, 24, 24, m_vPos.z);			
		}		
	}
	
	if(m_IsRenderCallName)
	{
		Rsprintf(RWCHAR(m_strTemp), _RT("[%s]"),RWCHAR(m_strCallName));	   
		Render->DrawText( m_pFontg, m_strTemp , m_CallNameRect.x, m_CallNameRect.y,
						  m_CallName_Color, m_vPos.z );
	}

	Render->DrawText( m_pFontg, m_strCharName ,	m_CharNameRect.x, m_CharNameRect.y, 
						  m_Name_Color, m_vPos.z );
	
#ifdef CPK_SYSTEM
	
	if( m_IsRenderCaoMark )
	{
		D3DCOLOR color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
		Render->ScretchDraw( TID_icon_Chaos, m_CaoMarkRect.x, m_CaoMarkRect.y, 
					  24, 24, color , m_vPos.z);
	}

#endif

	g_RenderManager.SetZBias( 0 );
	
}