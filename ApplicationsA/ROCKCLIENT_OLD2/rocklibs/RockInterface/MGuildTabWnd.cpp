#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"
#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "MCommunityWnd.h"
#include "MGuildTabWnd.h"
#include "stringmanager.h"

CMGuildTabWnd::CMGuildTabWnd()
{
	m_pMGuildTabWnd = NULL;
	m_pMGTextBox1 = NULL;	
}

CMGuildTabWnd::~CMGuildTabWnd()
{

}

void CMGuildTabWnd::Init()
{
	CMFriendTabWnd::Init();
}

void CMGuildTabWnd::Composition()
{
	CMFriendTabWnd::Composition();
}

void CMGuildTabWnd::Update()
{
	CMFriendTabWnd::Update();
}

void CMGuildTabWnd::Proc( SEventMessage* EMsg )
{
	for(int i = 0 ; i < 6 ; i++)
	 {
		if(FriendSelBtn[i]->GetID() == EMsg->FocusWnd)
		{			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				FieldBntSelected( i );				
			}	

			if( EMsg->Notify == NM_BTN_RCLICK )
			{
				SFriendData * pFriendData = FriendField[i].pFriendData;
				if( pFriendData && (pFriendData->uFriendState != n_MsngrStateOffline) )
				{
					FieldBntSelected( i );
					
					POINT		ptCursor;
					GetCursorPos( &ptCursor );
					::ScreenToClient( g_RockClient.GetApp(), &ptCursor );

					ptCursor.x -= 5;
					ptCursor.y -= 5;				
					g_MCommunityWnd.m_MPopupMenuWnd.GetPopUpWnd()->SetAbsolutePosition(ptCursor.x, ptCursor.y);
					g_MCommunityWnd.m_MPopupMenuWnd.SetParam(pFriendData->strFName);

					//여기에 팝업메뉴 비활성화 기능부여
					if(g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
					{
						g_MCommunityWnd.m_MPopupMenuWnd.SetActiveMenu(true, false, true, false);
					}
					else
					{
						g_MCommunityWnd.m_MPopupMenuWnd.SetActiveMenu(true, true, false, false);
					}				
					
					nRui->SendEvent( WID_MPopupWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
				}		
			}
		}
	 }

	if(m_pNameFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscNameSort = !m_bAscNameSort;
				
			if(m_bAscNameSort)
			{
				m_uSortPolicy = SORT_ASCNAME;						
			}
			else
			{
				m_uSortPolicy = SORT_DESCNAME;		
			}

			SortFriendData();
			SetFriendField();
		}	
	}

	if(m_pAreaFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscAreaSort = !m_bAscAreaSort;
			if(m_bAscAreaSort)
			{
				m_uSortPolicy = SORT_ASCAREA;	
			}
			else
			{
				m_uSortPolicy = SORT_DESCAREA;
			}
			
			SortFriendData();
			SetFriendField();	
		}
	}

	if(m_pChanelFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscChanelSort = !m_bAscChanelSort;
			if(m_bAscChanelSort)
			{
				m_uSortPolicy = SORT_ASCCHANEL;
			}
			else
			{
				m_uSortPolicy = SORT_DESCCHANEL;
			}
			
			SortFriendData();
			SetFriendField();		
		}
	}

	if(m_pLevelFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscLevelSort = !m_bAscLevelSort;
			if(m_bAscLevelSort)
			{
				m_uSortPolicy = SORT_ASCLEVEL;
			}
			else
			{
				m_uSortPolicy = SORT_DESCLEVEL;	
			}
			
			SortFriendData();
			SetFriendField();	
		}
	}

	if(m_pJobFieldBtn->GetID() == EMsg->FocusWnd)
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bAscJobSort = !m_bAscJobSort;
			if(m_bAscJobSort)
			{
				m_uSortPolicy = SORT_ASCJOB;
			}
			else
			{
				m_uSortPolicy = SORT_DESCJOB;
			}
			
			SortFriendData();
			SetFriendField();	
		}
	}
}