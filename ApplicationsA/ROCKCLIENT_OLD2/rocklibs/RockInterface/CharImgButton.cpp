#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"
#include "..\\..\\global.h"

#include "PartyInforWnd.h"
#include "CharImgButton.h"

#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CCharImgBox::CCharImgBox( SDesktop* DT ) : CImageBox( DT )
{
	m_unique = 0;
	m_race = 0;
	m_type = 0;	
	m_isBackImg = false;	
	
	m_IsMask = false;
	m_alive = true; 

}

CCharImgBox::~CCharImgBox()
{

}

void CCharImgBox::RenderProc()
{
	DWORD SrcBlend, DestBlend;

    g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
    g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );

	if(m_IsMask)
	{
		g_Pc_Manager.m_CharImage.Render( Cx , Cy , m_unique ,
			m_race , (n_CharImageType)m_type , g_lpDevice, m_isBackImg );
	}
	else
	{ 
		g_Pc_Manager.m_CharImage.Render( Cx , Cy , m_unique ,
			m_race , (n_CharImageType)m_type , NULL, g_lpDevice, m_isBackImg , m_alive);
	}

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( SrcBlend );
	g_RenderManager.SetDestBlend( DestBlend );
}

void CCharImgBox::SetRenderCharImgProperty(int unique,
	        		  int race, int type, bool isBackImg , bool alive)
{
	m_unique = unique;
	m_race = race;
	m_type = type;
	isBackImg = isBackImg;
	m_alive = alive;
}


CCharImgButton::CCharImgButton( SDesktop* DT ) : CButton( DT )
{
   m_nCButtonType = CBTYPE_NONE;
	
   m_iXpos = 0;
   m_iYpos = 0;
   m_unique = 0;
   m_race = 0;
   m_type = 0;	
   IsCharImgRender = false;
}

CCharImgButton::~CCharImgButton()
{

}

void CCharImgButton::SetCButtonType(UINT nCButtonType)
{
	m_nCButtonType = nCButtonType;
}

void CCharImgButton::RenderProc()
{
    if(IsCharImgRender)
	{
		switch(m_nCButtonType)
		{
			case CBTYPE_NONE:
				{
					RenderCBNoneProc();
				}
				break;
			case CBTYPE_PARTY:
				{
					RenderCBPartyProc();
				}
				break;
		}
	}	
	
	CButton::RenderProc();
}

void CCharImgButton::RenderCBNoneProc()
{
	DWORD SrcBlend, DestBlend;

    g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
    g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );
			
	bool bFlag = g_Pc_Manager.m_CharImage.Render( m_iXpos , m_iYpos , m_unique ,
		m_race , (n_CharImageType)m_type , NULL, g_lpDevice );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( SrcBlend );
	g_RenderManager.SetDestBlend( DestBlend );

}

void CCharImgButton::RenderCBPartyProc()
{
	SPARTYMEMBER_INFO *pPartyMemberInfo = g_PartyInforWnd.FindPartyMember((UINT)m_unique);
	
	if(pPartyMemberInfo && pPartyMemberInfo->m_bIsViewRange)
	{
		DWORD SrcBlend, DestBlend;

		g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
		g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );
#ifdef DIRECT_VERSION_9_MJH
		LPDIRECT3DTEXTURE9 pPartyMask = g_Pc_Manager.m_CharImage.GetPartyCharMask();
#else
		LPDIRECT3DTEXTURE8 pPartyMask = g_Pc_Manager.m_CharImage.GetPartyCharMask();
#endif // DIRECT_VERSION_9_MJH
		
		bool bFlag = g_Pc_Manager.m_CharImage.Render( m_iXpos , m_iYpos , m_unique ,
			m_race , (n_CharImageType)m_type , pPartyMask, g_lpDevice );
		
		if(!bFlag)	//렌더링 실패하면....
		{
			g_Pc_Manager.m_CharImage.Delete(m_unique, c_PcRace_User); //지우고
			
			SPARTYMEMBER_INFO *pPartyMemberInfo = g_PartyInforWnd.FindPartyMember((UINT)m_unique);
			
			if(pPartyMemberInfo)
			{
				pPartyMemberInfo->m_bIsSee = FALSE;
				pPartyMemberInfo->m_bIsViewRange = FALSE;
				
				if( g_PartyInforWnd.MakePCharImage( m_unique ) )	//다시만든다.
				{
					pPartyMemberInfo->m_bIsSee = TRUE;
					pPartyMemberInfo->m_bIsViewRange = TRUE;
				}
			}
		}

		g_RenderManager.SetAlphaBlendEnable( TRUE );
		g_RenderManager.SetSrcBlend( SrcBlend );
		g_RenderManager.SetDestBlend( DestBlend );
	}
	else
	{
		Cimg = TID_PARTY_MEMBER_NOSEE;
	}
}

void CCharImgButton::SetIsCharImgRender(bool flag)
{
	IsCharImgRender = flag;
}

void CCharImgButton::SetRenderCharImgProperty(int iXpos, int iYpos, int unique,
								   int race, int type)
{
   m_iXpos = iXpos;
   m_iYpos = iYpos;
   m_unique = unique;
   m_race = race;
   m_type = type;	
}


int CCharImgButton::GetXpos()
{	
	return m_iXpos;
}

int CCharImgButton::GetYpos()
{
	return m_iYpos;
}