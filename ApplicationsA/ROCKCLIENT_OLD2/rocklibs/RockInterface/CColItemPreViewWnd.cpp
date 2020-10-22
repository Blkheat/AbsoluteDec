#include "RockPCH.h"
#include "Rui.h"
#include "CColItemPreViewWnd.h"
#include "UiUtil.h"
#include "..\\..\\bravolibs\\obj\\player_manager.h"
#include "..\\..\\StringManager.h"

CColItemPreViewWnd g_ColItemPreViewWnd;

///-- 콜로니 아이템 미리보기창
CColItemPreViewWnd::CColItemPreViewWnd()
{
	m_SlotWndID = 0;
	m_PcItem = NULL;
	
	m_pDT = NULL;

	m_ClientRect.x		= 0;
	m_ClientRect.y		= 0;
	m_ClientRect.w		= MAX_ITEMTIP_WIDTH;
	m_ClientRect.h		= 0;

	m_FrameWidth = 2;

	m_DetailImgRect.x = 0;
	m_DetailImgRect.y = 0;
	m_DetailImgRect.w = 200;
	m_DetailImgRect.h = 200;

	memset( &m_StrInfo, 0, sizeof( SStringInfo ) );

	m_TColor = D3DCOLOR_XRGB( 221, 225, 237 );
	InitInfo();

	m_DescLineNum = 0;

	m_ItemType = n_COLONY_NONE;
}

CColItemPreViewWnd::~CColItemPreViewWnd()
{
	
}

void CColItemPreViewWnd::Initialize(SDesktop* DT)
{
	m_pDT = DT;
	InitInfo();
}

void CColItemPreViewWnd::Set( WNDID wndid, SRect* wc_size, SPcItem* pcitem )
{
	//id와 code가 같다면..같은 툴팁
	if( ( m_SlotWndID == wndid ) && ( m_PcItem == pcitem ) && ( m_PcItem->Code == pcitem->Code ) )
	{
		//그래도 위치 값은 계산....
		CalcClientSize(wc_size);
		CalcFrameSize();
		return;
	}

	m_SlotWndID = wndid;
	m_PcItem = pcitem;

	if( !m_PcItem || !(m_PcItem->ItemTable) )
	{
		return;
	}

	InitInfo();

	m_DetailImgID = (TEXID)(m_PcItem->ItemTable->theReMP);	
	WORD _CallType = m_PcItem->ItemTable->theReHPSpeed;

	switch( _CallType )
	{
		case n_CoPosition:
			{
				SetItemPosition();
			}
			break;
		case n_CoAttackArams:
			{
				SetItemAttackArams();
			}
			break;
		case n_CoBuilding:
			{
				SetItemBuilding();
			}
			break;
		case n_CoGuardTower:
			{
				SetItemGuardTower();
			}
			break;
	}		

	CalcClientSize( wc_size );
	CalcFrameSize();
}

//-----------------------------------------------------------------------------
void CColItemPreViewWnd::CalcClientSize( SRect* wc_size )
{
	CWnd* Fwnd	= NULL;
	
	int sc_x = 0;
	int sc_y = 0;
	
	if( ( nRui->GetFocusWnd() != WID_None ) && ( nRui->GetFocusWnd() != WID_DeskTop ) )
	{
		Fwnd = Container->Get( nRui->GetFocusWnd() );
		WNDID Pid = Fwnd->GetParentID();
		
		if(Pid == WID_InventoryWnd)
		{
			Fwnd = 	Container->Get( WID_InventoryWnd );
			SRect sRect;
			Fwnd->GetClientSize(&sRect);
			
			sc_x = sRect.x + (sRect.w / 2);   
			sc_y = nRui->Desktop.Height / 2;
			
		}
		else
		{
			sc_x = nRui->Desktop.Width / 2;
			sc_y = nRui->Desktop.Height / 2;
		}
	}
	
	int wc_x = wc_size->x;
	int wc_y = wc_size->y;
	int wc_w = wc_size->x + wc_size->w;
	int wc_h = wc_size->y + wc_size->h;
	
	//wndclient_center
	int wc_cnt = ( wc_size->w / 2 ) +  wc_size->x;
	
	if( wc_cnt <= sc_x )
	{
		//왼쪽
		if( ( wc_y - m_ClientRect.h ) > 0 )
		{
			m_ClientRect.x = wc_w;
			m_ClientRect.y = wc_y - m_ClientRect.h;
		}
		else
		{
			m_ClientRect.x = wc_w;
			m_ClientRect.y = wc_h;
		}
		
	}
	else if( wc_cnt > sc_x )
	{
		//오른쪽
		if( ( wc_y - m_ClientRect.h ) > 0 )
		{
			m_ClientRect.x = wc_x - m_ClientRect.w;
			m_ClientRect.y = wc_y - m_ClientRect.h;
		}
		else
		{
			m_ClientRect.x = wc_x - m_ClientRect.w;
			m_ClientRect.y = wc_h;
		}
	}
}

void CColItemPreViewWnd::CalcFrameSize()
{
	//위
	m_NorthFrm.x = m_ClientRect.x;
	m_NorthFrm.y = m_ClientRect.y - m_FrameWidth;
	m_NorthFrm.w = m_ClientRect.w;
	m_NorthFrm.h = m_FrameWidth;
	//
	m_NorthEastFrm.x = m_ClientRect.x + m_ClientRect.w;
	m_NorthEastFrm.y = m_ClientRect.y - m_FrameWidth;
	m_NorthEastFrm.w = m_FrameWidth;
	m_NorthEastFrm.h = m_FrameWidth;
	//오른쪽
	m_EastFrm.x = m_ClientRect.x + m_ClientRect.w;
	m_EastFrm.y = m_ClientRect.y;
	m_EastFrm.w = m_FrameWidth;
	m_EastFrm.h = m_ClientRect.h;
	//
	m_SouthEastFrm.x = m_ClientRect.x + m_ClientRect.w;
	m_SouthEastFrm.y = m_ClientRect.y + m_ClientRect.h;
	m_SouthEastFrm.w = m_FrameWidth;
	m_SouthEastFrm.h = m_FrameWidth;
	//아래
	m_SouthFrm.x = m_ClientRect.x;
	m_SouthFrm.y = m_ClientRect.y + m_ClientRect.h;
	m_SouthFrm.w = m_ClientRect.w;
	m_SouthFrm.h = m_FrameWidth;
	//
	m_SouthWestFrm.x = m_ClientRect.x - m_FrameWidth;
	m_SouthWestFrm.y = m_ClientRect.y + m_ClientRect.h;
	m_SouthWestFrm.w = m_FrameWidth;
	m_SouthWestFrm.h = m_FrameWidth;
	//왼쪽
	m_WestFrm.x = m_ClientRect.x - m_FrameWidth;
	m_WestFrm.y = m_ClientRect.y;
	m_WestFrm.w = m_FrameWidth;
	m_WestFrm.h = m_ClientRect.h;
	//
	m_NorthWestFrm.x = m_ClientRect.x - m_FrameWidth;
	m_NorthWestFrm.y = m_ClientRect.y - m_FrameWidth;
	m_NorthWestFrm.w = m_FrameWidth;
	m_NorthWestFrm.h = m_FrameWidth;
}

void CColItemPreViewWnd::InitInfo()
{
	m_DescLineNum = 0;
	m_DetailImgID = 0;
	m_ItemType = n_COLONY_NONE;
	
	memset( &m_ItemName, 0, sizeof( STipString ) );

	memset( &m_CallNpcList, 0, sizeof( STipString ) );

	memset( &m_CallNPCName1, 0, sizeof( STipString ) );
	memset( &m_MAXHP1, 0, sizeof( STipString ) );
	memset( &m_AttackAndMagic1, 0, sizeof( STipString ) );
	memset( &m_DefenceAndMagicRes1, 0, sizeof( STipString ) );

	memset( &m_CallNPCName2, 0, sizeof( STipString ) );
	memset( &m_MAXHP2, 0, sizeof( STipString ) );
	memset( &m_AttackAndMagic2, 0, sizeof( STipString ) );
	memset( &m_DefenceAndMagicRes2, 0, sizeof( STipString ) );

	memset( &m_CallNPCName3, 0, sizeof( STipString ) );
	memset( &m_MAXHP3, 0, sizeof( STipString ) );
	memset( &m_AttackAndMagic3, 0, sizeof( STipString ) );
	memset( &m_DefenceAndMagicRes3, 0, sizeof( STipString ) );
	
	memset( m_ItemDesc, '\0', sizeof( STipString ) * 10 );

	m_CallNpcList.color = D3DCOLOR_XRGB( 255, 255, 100 );

    m_CallNPCName1.color = D3DCOLOR_XRGB( 255, 255, 255 );
	m_MAXHP1.color = D3DCOLOR_XRGB( 100, 255, 100 );
	m_AttackAndMagic1.color = D3DCOLOR_XRGB( 255, 150, 150 );
	m_DefenceAndMagicRes1.color = D3DCOLOR_XRGB( 150, 150, 255 );
	
	m_CallNPCName2.color = D3DCOLOR_XRGB( 255, 255, 255 );
	m_MAXHP2.color = D3DCOLOR_XRGB( 100, 255, 100 );
	m_AttackAndMagic2.color = D3DCOLOR_XRGB( 255, 150, 150 );
	m_DefenceAndMagicRes2.color = D3DCOLOR_XRGB( 150, 150, 255 );

	m_CallNPCName3.color = D3DCOLOR_XRGB( 255, 255, 255 );
	m_MAXHP3.color = D3DCOLOR_XRGB( 100, 255, 100 );
	m_AttackAndMagic3.color = D3DCOLOR_XRGB( 255, 150, 150 );
	m_DefenceAndMagicRes3.color = D3DCOLOR_XRGB( 150, 150, 255 );	
	
	for( int i = 0; i < 10; ++i )
	{
		m_ItemDesc[i].color = m_TColor;
	}
}

void CColItemPreViewWnd::SetItemNoaml()
{
	int ClWidth = 0;
	int nMaxStrWidthNum = 28;

	m_ClientRect.h = BV;

	///-- 아이템 명	
	Rsprintf( RWCHAR(m_ItemName.str), _RT("%s"), RWCHAR(m_PcItem->ItemTable->theName) );
	if(m_Fontg)
		m_Fontg->GetStringInfo( m_ItemName.str, &m_StrInfo );	
	if( ClWidth < m_StrInfo.Width )
        ClWidth = m_StrInfo.Width;
	if( nMaxStrWidthNum < m_StrInfo.nCLen )
		nMaxStrWidthNum = m_StrInfo.nCLen;
	m_ItemName.x = 5;
	m_ItemName.y = m_ClientRect.h;
	m_ClientRect.h += RV;
	m_ClientRect.h += BV;

	///-- 상세 이미지
	if(m_DetailImgID)
	{
		m_DetailImgRect.x = 5;
		m_DetailImgRect.y = m_ClientRect.h;
		m_ClientRect.h += m_DetailImgRect.h;
		if( ClWidth < m_DetailImgRect.w )
			ClWidth = m_DetailImgRect.w;	
		m_ClientRect.h += BV;
	}

	///-- 소환대상
	Rsprintf( RWCHAR(m_CallNpcList.str), _RT("%s"), _RT("소환대상" ));		///-- HAN_MSG
	if(m_Fontg)
		m_Fontg->GetStringInfo( m_CallNpcList.str, &m_StrInfo );
	if( ClWidth < m_StrInfo.Width )
        ClWidth = m_StrInfo.Width;
	if( nMaxStrWidthNum < m_StrInfo.nCLen )
		nMaxStrWidthNum = m_StrInfo.nCLen;
	m_CallNpcList.x = 5;
	m_CallNpcList.y = m_ClientRect.h;
	m_ClientRect.h += BV;
	m_ClientRect.h += BV;

	DWORD dNPCCode = (DWORD)m_PcItem->ItemTable->theReSP;
	SNPCBaseInfo* npc_info = NULL;		
	npc_info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

	TCHAR strAttack[256] = {0,};
	TCHAR strMagic[256] = {0,};
	TCHAR strDefense[256] = {0,};
	TCHAR strResistant[256] = {0,};
	
	if(npc_info)
	{
		///-- 이름		
		Rsprintf( RWCHAR(m_CallNPCName1.str), _RT("%s: %s"), G_STRING_CHAR( IDS_CHARSTATE_NAME ), RWCHAR(npc_info->theName) );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_CallNPCName1.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_CallNPCName1.x = 5;
		m_CallNPCName1.y = m_ClientRect.h;
		m_ClientRect.h += RV;
		m_ClientRect.h += RV;

		///-- 최대 HP
		Rsprintf( RWCHAR(m_MAXHP1.str), _RT("%s: %u"), _RT("최대 HP"), npc_info->theMaxHP );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_MAXHP1.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_MAXHP1.x = 5;
		m_MAXHP1.y = m_ClientRect.h;
		m_ClientRect.h += RV;

		///-- 물리공격력 마법공격력
		if( npc_info->theAttack || npc_info->theMagic )
		{
			if(npc_info->theAttack)
			{
				Rsprintf( RWCHAR(strAttack), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), npc_info->theAttack );	///-- HAN_MSG
			}

			if(npc_info->theMagic)
			{
				Rsprintf( RWCHAR(strMagic), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), npc_info->theMagic );	///-- HAN_MSG
			}

			Rstrcat( RWCHAR(m_AttackAndMagic1.str), RWCHAR(strAttack) );
			if( Rstrcmp(RWCHAR(m_AttackAndMagic1.str), _RT("") ) != 0 )
			{
				Rstrcat( RWCHAR(m_AttackAndMagic1.str), _RT("  ") );
			}
			Rstrcat( RWCHAR(m_AttackAndMagic1.str), RWCHAR(strMagic) );
			if(m_Fontg)
			m_Fontg->GetStringInfo( m_AttackAndMagic1.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_AttackAndMagic1.x = 5;
			m_AttackAndMagic1.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
		
		///-- 물리방어력 마법방어력
		if( npc_info->theDefense || npc_info->theResistant )
		{
			if(npc_info->theDefense)
			{
				Rsprintf( RWCHAR(strDefense) , _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), npc_info->theDefense );	
			}

			if(npc_info->theResistant)
			{
				Rsprintf( RWCHAR(strResistant), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), npc_info->theResistant );	
			}

			Rstrcat(RWCHAR(m_DefenceAndMagicRes1.str), RWCHAR(strDefense) );
			if( Rstrcmp(RWCHAR(m_DefenceAndMagicRes1.str), _RT("") ) != 0 )
			{
				Rstrcat(RWCHAR(m_DefenceAndMagicRes1.str), _RT("  ") );
			}
			Rstrcat(RWCHAR(m_DefenceAndMagicRes1.str), RWCHAR(strResistant) );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_DefenceAndMagicRes1.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_DefenceAndMagicRes1.x = 5;
			m_DefenceAndMagicRes1.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
	
		m_ClientRect.h += BV;
		m_ClientRect.h += BV;
	}

	bool IsDesc = false;
	m_DescLineNum = 0;
	RTCHAR *pStrIn = (RTCHAR *)m_PcItem->ItemTable->theDesc;

	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
#ifdef UNICODE_ROCK
		pStrIn = GetStringReturnInStringW('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
#else
		pStrIn = GetStringReturnInString('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
#endif
	
		if( Rstrcmp(RWCHAR(TempBuf), _RT("") ) != 0 )
		{
			SAFE_STR_CPY(m_ItemDesc[m_DescLineNum].str, TempBuf, 256 );
			if(m_Fontg)
	
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			m_ItemDesc[m_DescLineNum].x = 5;			
			m_ItemDesc[m_DescLineNum].y = m_ClientRect.h;
			m_ClientRect.h += RV;
			
			m_DescLineNum++;
			IsDesc = true;
		}
	}
	
	if(IsDesc)
	{
		m_ClientRect.h += BV;
	}

	if( (ClWidth + 10) >= MAX_ITEMTIP_WIDTH )
		ClWidth = MAX_ITEMTIP_WIDTH;
	else
		ClWidth += 10;
    
	m_ClientRect.w = ClWidth;

}

void CColItemPreViewWnd::SetItemBuilding()
{
	m_ItemType = n_COLONY_BULDING;
	
		int ClWidth = 0;
	int nMaxStrWidthNum = 28;

	m_ClientRect.h = BV;

	///-- 아이템 명	
	Rsprintf( RWCHAR(m_ItemName.str), _RT("%s"), RWCHAR(m_PcItem->ItemTable->theName));
	if(m_Fontg)
		m_Fontg->GetStringInfo( m_ItemName.str, &m_StrInfo );	
	if( ClWidth < m_StrInfo.Width )
        ClWidth = m_StrInfo.Width;
	if( nMaxStrWidthNum < m_StrInfo.nCLen )
		nMaxStrWidthNum = m_StrInfo.nCLen;
	m_ItemName.x = 5;
	m_ItemName.y = m_ClientRect.h;
	m_ClientRect.h += RV;
	m_ClientRect.h += BV;

	///-- 상세 이미지
	if(m_DetailImgID)
	{
		m_DetailImgRect.x = 5;
		m_DetailImgRect.y = m_ClientRect.h;
		m_ClientRect.h += m_DetailImgRect.h;
		if( ClWidth < m_DetailImgRect.w )
			ClWidth = m_DetailImgRect.w;	
		m_ClientRect.h += BV;
	}

	///-- 소환대상
	Rsprintf( RWCHAR(m_CallNpcList.str), _RT("%s"), _RT("소환대상"));		///-- HAN_MSG
	if(m_Fontg)
		m_Fontg->GetStringInfo( m_CallNpcList.str, &m_StrInfo );
	if( ClWidth < m_StrInfo.Width )
        ClWidth = m_StrInfo.Width;
	if( nMaxStrWidthNum < m_StrInfo.nCLen )
		nMaxStrWidthNum = m_StrInfo.nCLen;
	m_CallNpcList.x = 5;
	m_CallNpcList.y = m_ClientRect.h;
	m_ClientRect.h += BV;
	m_ClientRect.h += BV;

	DWORD dNPCCode = (DWORD)m_PcItem->ItemTable->theReSP;
	SNPCBaseInfo* npc_info = NULL;		
	npc_info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

	TCHAR strAttack[256] = {0,};
	TCHAR strMagic[256] = {0,};
	TCHAR strDefense[256] = {0,};
	TCHAR strResistant[256] = {0,};
	
	if(npc_info)
	{
		///-- 이름		
		Rsprintf( RWCHAR(m_CallNPCName1.str), _RT("%s: %s"), G_STRING_CHAR( IDS_CHARSTATE_NAME ), RWCHAR(npc_info->theName) );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_CallNPCName1.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_CallNPCName1.x = 5;
		m_CallNPCName1.y = m_ClientRect.h;
		m_ClientRect.h += RV;
		m_ClientRect.h += RV;

		///-- 최대 HP
		Rsprintf( RWCHAR(m_MAXHP1.str), _RT("%s: %u"), _RT("최대 HP"), npc_info->theMaxHP );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_MAXHP1.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_MAXHP1.x = 5;
		m_MAXHP1.y = m_ClientRect.h;
		m_ClientRect.h += RV;		

		///-- 물리공격력 마법공격력
		if( npc_info->theAttack || npc_info->theMagic )
		{
			if(npc_info->theAttack)
			{
				Rsprintf( RWCHAR(strAttack), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), npc_info->theAttack );	///-- HAN_MSG
			}

			if(npc_info->theMagic)
			{
				Rsprintf( RWCHAR(strMagic), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), npc_info->theMagic );	///-- HAN_MSG
			}

			Rstrcat( RWCHAR(m_AttackAndMagic1.str), RWCHAR(strAttack) );
			if( Rstrcmp(RWCHAR(m_AttackAndMagic1.str), _RT("") ) != 0 )
			{
				Rstrcat( RWCHAR(m_AttackAndMagic1.str), _RT("  ") );
			}
			Rstrcat( RWCHAR(m_AttackAndMagic1.str), RWCHAR(strMagic) );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_AttackAndMagic1.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_AttackAndMagic1.x = 5;
			m_AttackAndMagic1.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
		
		///-- 물리방어력 마법방어력
		if( npc_info->theDefense || npc_info->theResistant )
		{
			if(npc_info->theDefense)
			{
				Rsprintf( RWCHAR(strDefense), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), npc_info->theDefense );	
			}

			if(npc_info->theResistant)
			{
				Rsprintf( RWCHAR(strResistant), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), npc_info->theResistant );	
			}

			Rstrcat(m_DefenceAndMagicRes1.str, strDefense );
			if( Rstrcmp(RWCHAR(m_DefenceAndMagicRes1.str), _RT("") ) != 0 )
			{
				Rstrcat(RWCHAR(m_DefenceAndMagicRes1.str), _RT("  ") );
			}
			Rstrcat(RWCHAR(m_DefenceAndMagicRes1.str), RWCHAR(strResistant) );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_DefenceAndMagicRes1.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_DefenceAndMagicRes1.x = 5;
			m_DefenceAndMagicRes1.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
	
		m_ClientRect.h += BV;
		m_ClientRect.h += BV;
	}

	dNPCCode++;			
	npc_info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );
	
	if(npc_info)
	{
		///-- 이름		
		Rsprintf( RWCHAR(m_CallNPCName2.str), _RT("%s: %s"), G_STRING_CHAR( IDS_CHARSTATE_NAME ), RWCHAR(npc_info->theName) );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_CallNPCName2.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_CallNPCName2.x = 5;
		m_CallNPCName2.y = m_ClientRect.h;
		m_ClientRect.h += RV;
		m_ClientRect.h += RV;

		///-- 최대 HP
		Rsprintf( RWCHAR(m_MAXHP2.str), _RT("%s: %u"), _RT("최대 HP"), npc_info->theMaxHP );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_MAXHP2.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_MAXHP2.x = 5;
		m_MAXHP2.y = m_ClientRect.h;
		m_ClientRect.h += RV;

		///-- 물리공격력 마법공격력
		if( npc_info->theAttack || npc_info->theMagic )
		{
			if(npc_info->theAttack)
			{
				Rsprintf( RWCHAR(strAttack), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), npc_info->theAttack );	///-- HAN_MSG
			}

			if(npc_info->theMagic)
			{
				Rsprintf( RWCHAR(strMagic), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), npc_info->theMagic );	///-- HAN_MSG
			}

			Rstrcat( RWCHAR(m_AttackAndMagic2.str), RWCHAR(strAttack) );
			if( Rstrcmp(RWCHAR(m_AttackAndMagic2.str), _RT("") ) != 0 )
			{
				Rstrcat( RWCHAR(m_AttackAndMagic2.str), _RT("  ") );
			}
			Rstrcat( RWCHAR(m_AttackAndMagic2.str), RWCHAR(strMagic) );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_AttackAndMagic2.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_AttackAndMagic2.x = 5;
			m_AttackAndMagic2.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
		
		///-- 물리방어력 마법방어력
		if( npc_info->theDefense || npc_info->theResistant )
		{
			if(npc_info->theDefense)
			{
				Rsprintf( RWCHAR(strDefense), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), npc_info->theDefense );	
			}

			if(npc_info->theResistant)
			{
				Rsprintf( RWCHAR(strResistant), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), npc_info->theResistant );	
			}

			Rstrcat( RWCHAR(m_DefenceAndMagicRes2.str), RWCHAR(strDefense) );
			if( Rstrcmp(RWCHAR(m_DefenceAndMagicRes2.str), _RT("") ) != 0 )
			{
				Rstrcat( RWCHAR(m_DefenceAndMagicRes2.str), _RT("  ") );
			}
			Rstrcat( RWCHAR(m_DefenceAndMagicRes2.str), strResistant );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_DefenceAndMagicRes2.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_DefenceAndMagicRes2.x = 5;
			m_DefenceAndMagicRes2.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
	
		m_ClientRect.h += BV;
		m_ClientRect.h += BV;
	}

	dNPCCode++;			
	npc_info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );
	
	if(npc_info)
	{
		///-- 이름		
		Rsprintf( RWCHAR(m_CallNPCName3.str), _RT("%s: %s"), G_STRING_CHAR( IDS_CHARSTATE_NAME ), RWCHAR(npc_info->theName) );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_CallNPCName2.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_CallNPCName3.x = 5;
		m_CallNPCName3.y = m_ClientRect.h;
		m_ClientRect.h += RV;
		m_ClientRect.h += RV;

		///-- 최대 HP
		Rsprintf( RWCHAR(m_MAXHP2.str), _RT("%s: %u"), _RT("최대 HP"), npc_info->theMaxHP );		///-- HAN_MSG
		if(m_Fontg)
			m_Fontg->GetStringInfo( m_MAXHP3.str, &m_StrInfo );
		if( ClWidth < m_StrInfo.Width )
			ClWidth = m_StrInfo.Width;
		if( nMaxStrWidthNum < m_StrInfo.nCLen )
			nMaxStrWidthNum = m_StrInfo.nCLen;
		m_MAXHP3.x = 5;
		m_MAXHP3.y = m_ClientRect.h;
		m_ClientRect.h += RV;

		///-- 물리공격력 마법공격력
		if( npc_info->theAttack || npc_info->theMagic )
		{
			if(npc_info->theAttack)
			{
				Rsprintf( RWCHAR(strAttack), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ), npc_info->theAttack );	///-- HAN_MSG
			}

			if(npc_info->theMagic)
			{
				Rsprintf( RWCHAR(strMagic), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), npc_info->theMagic );	///-- HAN_MSG
			}

			Rstrcat( RWCHAR(m_AttackAndMagic3.str), RWCHAR(strAttack) );
			if( Rstrcmp(RWCHAR(m_AttackAndMagic3.str), _RT("") ) != 0 )
			{
				Rstrcat( RWCHAR(m_AttackAndMagic3.str), _RT("  ") );
			}
			Rstrcat( RWCHAR(m_AttackAndMagic3.str), RWCHAR(strMagic) );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_AttackAndMagic3.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_AttackAndMagic3.x = 5;
			m_AttackAndMagic3.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
		
		///-- 물리방어력 마법방어력
		if( npc_info->theDefense || npc_info->theResistant )
		{
			if(npc_info->theDefense)
			{
				Rsprintf( RWCHAR(strDefense), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), npc_info->theDefense );	
			}

			if(npc_info->theResistant)
			{
				Rsprintf( RWCHAR(strResistant), _RT("%s: %u"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), npc_info->theResistant );	
			}

			Rstrcat(RWCHAR( m_DefenceAndMagicRes3.str), RWCHAR(strDefense) );
			if( Rstrcmp(RWCHAR(m_DefenceAndMagicRes3.str), _RT("") ) != 0 )
			{
				Rstrcat(RWCHAR( m_DefenceAndMagicRes3.str), _RT("  ") );
			}
			Rstrcat(RWCHAR( m_DefenceAndMagicRes3.str), RWCHAR(strResistant) );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_DefenceAndMagicRes3.str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			if( nMaxStrWidthNum < m_StrInfo.nCLen )
				nMaxStrWidthNum = m_StrInfo.nCLen;
			m_DefenceAndMagicRes3.x = 5;
			m_DefenceAndMagicRes3.y = m_ClientRect.h;
			m_ClientRect.h += RV;
		}
	
		m_ClientRect.h += BV;
	}

	bool IsDesc = false;
	m_DescLineNum = 0;
	RTCHAR *pStrIn = (RTCHAR *)m_PcItem->ItemTable->theDesc;

	while(pStrIn)
	{
		TCHAR TempBuf[256];
		memset(TempBuf, NULL, sizeof(TCHAR) * 256 );
#ifdef UNICODE_ROCK
		pStrIn = GetStringReturnInStringW('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
#else
		pStrIn = GetStringReturnInString('\\', true, nMaxStrWidthNum, pStrIn, TempBuf);
#endif
	
		if( Rstrcmp(TempBuf, _RT("") ) != 0 )
		{
			SAFE_STR_CPY(m_ItemDesc[m_DescLineNum].str, TempBuf, 256 );
			if(m_Fontg)
				m_Fontg->GetStringInfo( m_ItemDesc[m_DescLineNum].str, &m_StrInfo );
			if( ClWidth < m_StrInfo.Width )
				ClWidth = m_StrInfo.Width;
			m_ItemDesc[m_DescLineNum].x = 5;			
			m_ItemDesc[m_DescLineNum].y = m_ClientRect.h;
			m_ClientRect.h += RV;
			
			m_DescLineNum++;
			IsDesc = true;
		}
	}
	
	if(IsDesc)
	{
		m_ClientRect.h += BV;
	}

	if( (ClWidth + 10) >= MAX_ITEMTIP_WIDTH )
		ClWidth = MAX_ITEMTIP_WIDTH;
	else
		ClWidth += 10;
    
	m_ClientRect.w = ClWidth;

}

void CColItemPreViewWnd::SetItemPosition()
{
	m_ItemType = n_COLONY_POSITION;
	SetItemNoaml();
}

void CColItemPreViewWnd::SetItemAttackArams()
{
	m_ItemType = n_COLONY_ATTCKARMS;
	SetItemNoaml();
}

void CColItemPreViewWnd::SetItemGuardTower()
{
	m_ItemType = n_COLONY_GTOWER;
	SetItemNoaml();
}	

void CColItemPreViewWnd::RenderProc()
{
	Render->Draw( TID_CLIENT, m_ClientRect.x, m_ClientRect.y, m_ClientRect.w, m_ClientRect.h, D3DCOLOR_ARGB( 225, 57, 61, 72 ) );	
	
	Render->Draw( TID_Tooltip2_Img, m_NorthFrm.x, m_NorthFrm.y, m_NorthFrm.w, m_NorthFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, m_NorthEastFrm.x, m_NorthEastFrm.y, m_NorthEastFrm.w, m_NorthEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, m_EastFrm.x, m_EastFrm.y, m_EastFrm.w, m_EastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, m_SouthEastFrm.x, m_SouthEastFrm.y, m_SouthEastFrm.w, m_SouthEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, m_SouthFrm.x, m_SouthFrm.y, m_SouthFrm.w, m_SouthFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, m_SouthWestFrm.x, m_SouthWestFrm.y, m_SouthWestFrm.w, m_SouthWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, m_WestFrm.x, m_WestFrm.y, m_WestFrm.w, m_WestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, m_NorthWestFrm.x, m_NorthWestFrm.y, m_NorthWestFrm.w, m_NorthWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
		
    switch( m_ItemType )
	{
		case n_COLONY_POSITION:		DrawItemPosition();		break;
		case n_COLONY_ATTCKARMS:	DrawItemAttackArams();	break;
		case n_COLONY_BULDING:		DrawItemBuilding();		break;
		case n_COLONY_GTOWER:		DrawItemGuardTower();	break;
	}
}

void CColItemPreViewWnd::DrawNormal()
{
	if(m_Fontg)
	{
		m_Fontg->GetStringInfo( m_ItemName.str, &m_StrInfo );
		Render->Draw(TID_Tooltip2_Img, (m_ItemName.x + m_ClientRect.x - 5), (m_ItemName.y + m_ClientRect.y - 5), 
			m_ClientRect.w , (m_StrInfo.Height + 10), D3DCOLOR_ARGB( 180, 220, 220, 255 ));
		
		Render->DrawText( m_Fontg, m_ItemName.str, (m_ItemName.x + m_ClientRect.x), 
						 (m_ItemName.y + m_ClientRect.y), D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		
		Render->ScretchDraw( m_DetailImgID, (m_DetailImgRect.x + m_ClientRect.x), 
							 (m_DetailImgRect.y + m_ClientRect.y) , 
							 m_DetailImgRect.w , m_DetailImgRect.h, D3DCOLOR_XRGB( 255, 255, 255 ) );		
		
		Render->DrawText( m_Fontg, m_CallNpcList.str, (m_CallNpcList.x + m_ClientRect.x), 
						  (m_CallNpcList.y + m_ClientRect.y),  m_CallNpcList.color );
		
		Render->DrawText( m_Fontg, m_CallNPCName1.str, (m_CallNPCName1.x + m_ClientRect.x), 
						  (m_CallNPCName1.y + m_ClientRect.y),  m_CallNPCName1.color );
		Render->DrawText( m_Fontg, m_MAXHP1.str, (m_MAXHP1.x + m_ClientRect.x), 
						  (m_MAXHP1.y + m_ClientRect.y),  m_MAXHP1.color );
		Render->DrawText( m_Fontg, m_AttackAndMagic1.str, (m_AttackAndMagic1.x + m_ClientRect.x), 
						  (m_AttackAndMagic1.y + m_ClientRect.y),  m_AttackAndMagic1.color );
		Render->DrawText( m_Fontg, m_DefenceAndMagicRes1.str, (m_DefenceAndMagicRes1.x + m_ClientRect.x), 
						  (m_DefenceAndMagicRes1.y + m_ClientRect.y),  m_DefenceAndMagicRes1.color );

		for( int i = 0; i < m_DescLineNum; ++i )
		{
			Render->DrawText( m_Fontg, m_ItemDesc[i].str, (m_ItemDesc[i].x + m_ClientRect.x), 
							  (m_ItemDesc[i].y + m_ClientRect.y), m_ItemDesc[i].color );
		}	
	}	
}

void CColItemPreViewWnd::DrawItemBuilding()
{
	if(m_Fontg)
	{
		m_Fontg->GetStringInfo( m_ItemName.str, &m_StrInfo );
		Render->Draw(TID_Tooltip2_Img, (m_ItemName.x + m_ClientRect.x - 5), (m_ItemName.y + m_ClientRect.y - 5), 
			m_ClientRect.w , (m_StrInfo.Height + 10), D3DCOLOR_ARGB( 180, 220, 220, 255 ));
		
		Render->DrawText( m_Fontg, m_ItemName.str, (m_ItemName.x + m_ClientRect.x), 
						 (m_ItemName.y + m_ClientRect.y), D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		
		Render->ScretchDraw( m_DetailImgID, (m_DetailImgRect.x + m_ClientRect.x), 
							 (m_DetailImgRect.y + m_ClientRect.y) , 
							 m_DetailImgRect.w , m_DetailImgRect.h, D3DCOLOR_XRGB( 255, 255, 255 ) );		
		
		Render->DrawText( m_Fontg, m_CallNpcList.str, (m_CallNpcList.x + m_ClientRect.x), 
						  (m_CallNpcList.y + m_ClientRect.y),  m_CallNpcList.color );
		
		Render->DrawText( m_Fontg, m_CallNPCName1.str, (m_CallNPCName1.x + m_ClientRect.x), 
						  (m_CallNPCName1.y + m_ClientRect.y),  m_CallNPCName1.color );
		Render->DrawText( m_Fontg, m_MAXHP1.str, (m_MAXHP1.x + m_ClientRect.x), 
						  (m_MAXHP1.y + m_ClientRect.y),  m_MAXHP1.color );
		Render->DrawText( m_Fontg, m_AttackAndMagic1.str, (m_AttackAndMagic1.x + m_ClientRect.x), 
						  (m_AttackAndMagic1.y + m_ClientRect.y),  m_AttackAndMagic1.color );
		Render->DrawText( m_Fontg, m_DefenceAndMagicRes1.str, (m_DefenceAndMagicRes1.x + m_ClientRect.x), 
						  (m_DefenceAndMagicRes1.y + m_ClientRect.y),  m_DefenceAndMagicRes1.color );

		Render->DrawText( m_Fontg, m_CallNPCName2.str, (m_CallNPCName2.x + m_ClientRect.x), 
						  (m_CallNPCName2.y + m_ClientRect.y),  m_CallNPCName2.color );
		Render->DrawText( m_Fontg, m_MAXHP2.str, (m_MAXHP2.x + m_ClientRect.x), 
						  (m_MAXHP2.y + m_ClientRect.y),  m_MAXHP2.color );
		Render->DrawText( m_Fontg, m_AttackAndMagic2.str, (m_AttackAndMagic2.x + m_ClientRect.x), 
						  (m_AttackAndMagic2.y + m_ClientRect.y),  m_AttackAndMagic2.color );
		Render->DrawText( m_Fontg, m_DefenceAndMagicRes2.str, (m_DefenceAndMagicRes2.x + m_ClientRect.x), 
						  (m_DefenceAndMagicRes2.y + m_ClientRect.y),  m_DefenceAndMagicRes2.color );

		Render->DrawText( m_Fontg, m_CallNPCName3.str, (m_CallNPCName3.x + m_ClientRect.x), 
						  (m_CallNPCName3.y + m_ClientRect.y),  m_CallNPCName3.color );
		Render->DrawText( m_Fontg, m_MAXHP3.str, (m_MAXHP3.x + m_ClientRect.x), 
						  (m_MAXHP3.y + m_ClientRect.y),  m_MAXHP3.color );
		Render->DrawText( m_Fontg, m_AttackAndMagic3.str, (m_AttackAndMagic3.x + m_ClientRect.x), 
						  (m_AttackAndMagic3.y + m_ClientRect.y),  m_AttackAndMagic3.color );
		Render->DrawText( m_Fontg, m_DefenceAndMagicRes3.str, (m_DefenceAndMagicRes3.x + m_ClientRect.x), 
						  (m_DefenceAndMagicRes3.y + m_ClientRect.y),  m_DefenceAndMagicRes3.color );

		for( int i = 0; i < m_DescLineNum; ++i )
		{
			Render->DrawText( m_Fontg, m_ItemDesc[i].str, (m_ItemDesc[i].x + m_ClientRect.x), 
							  (m_ItemDesc[i].y + m_ClientRect.y), m_ItemDesc[i].color );
		}	
	}
}

void CColItemPreViewWnd::DrawItemPosition()
{
	DrawNormal();
}
void CColItemPreViewWnd::DrawItemAttackArams()
{
	DrawNormal();
}

void CColItemPreViewWnd::DrawItemGuardTower()
{
	DrawNormal();
}

//-----------------------------------------------------------------------------
D3DCOLOR CColItemPreViewWnd::GetRareColor( BYTE rare )//Rare( 희귀도 )에 따른 컬러값....
{
	if( rare == 0 ){	return( D3DCOLOR_XRGB( 120, 110, 75 ) );	}//초급 아이템 - 회색
	else if( ( rare >= 1 ) && ( rare <= 30 ) )		{	return( D3DCOLOR_XRGB( 64, 64, 64 ) );	}		//일반 아이템	- 검정색
	else if( ( rare >= 31 ) && ( rare <= 70 ) )		{	return( D3DCOLOR_XRGB( 180, 35, 255 ) );	}	//중급 아이템	- 보라색
	else if( ( rare >= 71 ) && ( rare <= 150 ) )	{	return( D3DCOLOR_XRGB( 90, 85, 250 ) );	}	//고급 아이템	- 파란색
	else if( ( rare >= 151 ) && ( rare <= 200 ) )	{	return( D3DCOLOR_XRGB( 255, 175, 0 ) );	}	//귀중품		- 노란색
	else if( ( rare >= 201 ) && ( rare <= 230 ) )	{	return( D3DCOLOR_XRGB( 255, 30, 200 ) );	}	//보물			- 분홍색
	else if( ( rare >= 231 ) && ( rare <= 255 ) )	{	return( D3DCOLOR_XRGB( 250, 52, 52 ) );	}	//레전드리		- 빨간색
	
	//예외상황일때..흰색 리턴
	return( D3DCOLOR_XRGB( 0, 0, 0 ) );
}