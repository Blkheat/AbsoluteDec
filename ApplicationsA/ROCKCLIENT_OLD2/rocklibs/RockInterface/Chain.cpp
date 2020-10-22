#include "Chain.h"
#include "FrameWnd.h"
#include "TextBox.h"
#include "ChatMainProc.h"
#include "Rui.h"

#include "CUserStoreTitleWnd.h"
#include "CUserStoreBuyWnd.h"
#include "CUserStoreSellWnd.h"
#include "CUserStoreSetBuyWnd.h"
#include "NpcChatWnd.h"
#include "MiniMapWndProc.h"

//-----------------------------------------------------------------------------
CChain::CChain()
{
	Head = NULL;
	Tail = NULL;

	NormalWndFocus = NULL;
	pCount	 =0 ;
}

//-----------------------------------------------------------------------------
CChain::~CChain()
{
	Release();
}

//-----------------------------------------------------------------------------
stChainNode* CChain::Add( CWnd* Wnd )
{
	stChainNode*	Node;
	stChainNode*	Temp;
	NDepthOrder		Zorder = n_zoNone;
	
	pCount++;
	Node = SAFE_NEW( stChainNode );	
	Node->Wnd = Wnd;
	Node->Prev = NULL;
	Node->Next = NULL;

	Temp = Head;
	if( Temp == NULL )
	{
		Head = Node;
		Tail = Node;

		if(0xffffffff == (DWORD)Head)
		{
			int a = 0;
		}
	}
	else
	{
		//오더 순서에 맞춰서 리스팅
		Zorder = Node->Wnd->GetZorder();
		if( Zorder == n_zoTop )	InsertNodeLast( Node );
		if( Zorder == n_zoNormal )	InsertNodeAfter( Node );
		if( Zorder == n_zoBottom )	InsertNodeFirst( Node );
	}

	return Node;
}

//-----------------------------------------------------------------------------
void CChain::InsertNodeFirst( stChainNode* Node )
{
	stChainNode* TempH = GetHead();

	Node->Prev = NULL;
	Node->Next = TempH;
	TempH->Prev = Node;
	Head = Node;

	if(0xffffffff == (DWORD)Head)
	{
		int a = 0;
	}
}

//-----------------------------------------------------------------------------
void CChain::InsertNodeLast( stChainNode* Node )
{
	stChainNode* TempT = GetTail();

	Node->Next = NULL;
	Node->Prev = TempT;
	TempT->Next = Node;
	Tail = Node;
}

//-----------------------------------------------------------------------------
void CChain::InsertNodeBefore( stChainNode* Node )
{
	Node;
}

//-----------------------------------------------------------------------------
void CChain::InsertNodeAfter( stChainNode* Node )
{
	stChainNode*	Temp	= NULL;
	stChainNode*	Tnode	= GetTail();
	NDepthOrder		Zorder	= n_zoNone;


	while( Tnode != NULL )
	{
		Zorder = Tnode->Wnd->GetZorder();
		if( ( Zorder == n_zoNormal ) || ( Zorder == n_zoBottom ) )
		{
			if( Tnode->Next == NULL )//Tail
			{
				InsertNodeLast( Node );
				return;
			}
			
			Temp = Tnode->Next;
			Node->Prev = Tnode;
			Node->Next = Temp;

			Tnode->Next = Node;
			Temp->Prev = Node;
			return;
		}
		Tnode = GetPrev( Tnode );
	}

	//Head.... (Tnode == NULL)
	InsertNodeFirst( Node );
	return;
}

//-----------------------------------------------------------------------------
stChainNode* CChain::ExtractNode( stChainNode* Node )
{
	stChainNode* TempP;
	stChainNode* TempN;
	stChainNode* Temp;

	if( Node == Head )
	{
		Temp = Head->Next;
		Temp->Prev = NULL;
		Head = Temp;
		return Node;
	}
	else if( Node == Tail )
	{
		Temp = Tail->Prev;
		Temp->Next = NULL;
		Tail = Temp;
		return Node;
	}
	else
	{
		TempP = Node->Prev;
		TempN = Node->Next;
		TempP->Next = TempN;
		TempN->Prev = TempP;
		return Node;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
void CChain::Ordering( CWnd* Wnd )
{
	stChainNode* Node = GetHead(); 
	NDepthOrder Zorder = n_zoNone;

	if( Head == Tail )
	{
		return;
	}

	while( Node != NULL )
	{
		if( Node->Wnd == Wnd )
		{
			Zorder = Node->Wnd->GetZorder();
			if( Zorder == n_zoNormal )
			{
				CFrameWnd* FocusFWnd = NULL;

				///--JAPAN_BUG_MODIFY
				if(  NormalWndFocus && 
					( ( NormalWndFocus->GetWindowType() == n_wtFrameWnd ) ||
					  ( NormalWndFocus->GetWindowType() == n_wtFlashFrameWnd ) ) )
				{
					FocusFWnd = (CFrameWnd*)NormalWndFocus;
					FocusFWnd->SetFocusWndColor( false );				
				}

				NormalWndFocus = Wnd;

				// Ordering 된 윈도우의 Parents가 FrameWnd이기 때문에...포커스 표시 해주기 
				///--JAPAN_BUG_MODIFY
				if(  NormalWndFocus && 
					( ( NormalWndFocus->GetWindowType() == n_wtFrameWnd ) ||
					  ( NormalWndFocus->GetWindowType() == n_wtFlashFrameWnd ) ) )
				{				
					FocusFWnd = (CFrameWnd*)NormalWndFocus;
					FocusFWnd->SetFocusWndColor( true );
				}
			}
			
			switch( Zorder )
			{
			case n_zoTop:
				InsertNodeLast( ExtractNode( Node ) );
				return;
			case n_zoNormal:
				InsertNodeAfter( ExtractNode( Node ) );
				return;
			case n_zoBottom:
				InsertNodeFirst( ExtractNode( Node ) );
				return;
			}
		}

		Node = GetNext( Node );
	}
}

//-----------------------------------------------------------------------------
CWnd* CChain::GetNormalWndFocusHandle()
{
	return( NormalWndFocus );
}

//-----------------------------------------------------------------------------
void CChain::Release()
{
	stChainNode* Temp = NULL;
	stChainNode* Node = GetHead();
	int Count =0 ; 

	while( Node != NULL )
	{
		Count++;
		Temp = GetNext( Node );
		delete( Node );
	
		Node = Temp;
	}
	if( Count != pCount)
	{
		int a =0 ;
	}
}

//-----------------------------------------------------------------------------
stChainNode* CChain::GetNext( stChainNode* Node )
{
	return Node->Next;
}

//-----------------------------------------------------------------------------
stChainNode* CChain::GetPrev( stChainNode* Node )
{
	return Node->Prev;
}

//-----------------------------------------------------------------------------
stChainNode* CChain::GetHead()
{
	return Head;
}

//-----------------------------------------------------------------------------
stChainNode* CChain::GetTail()
{
	return Tail;
}

///-----------------------------------------------------------------------------
///-- Chain 으로 연결된 Wnd 중 최 상위 Wnd 를 날린다.
///-----------------------------------------------------------------------------
///-- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///-- 2005.03.14 / Lyul
///-- Node 가 가지는 Wnd 는 CWnd 이다 하지만 개념적으로는 CFrameWnd 가 맞을듯
///-- 일단 Cast 로 해결

bool CChain::CloseVisibleWnd()
{
	stChainNode* Node = GetTail(); 
	NDepthOrder Zorder = n_zoNone;

	while( Node != NULL )
	{
		Zorder = Node->Wnd->GetZorder();
			
		if( Zorder == n_zoNormal )
		{
			///-- Visble 설정은 Wnd 와 WndProc 가 동기되어야 한다.
			if( Node->Wnd->GetWindowType() == n_wtFrameWnd && Node->Wnd->IsVisible() )
			{
				if( Node->Wnd->GetID() == WID_BZ_ROOMLIST_WND ||
					Node->Wnd->GetID() == WID_BZ_CREATEROOM_WND || 
					Node->Wnd->GetID() == WID_BZ_WAITINGROOM_WND ||
					Node->Wnd->GetID() == g_UserStoreSellWnd.m_pFrame->GetID() 
				  )
				{
					return true;
				}
				else if( Node->Wnd->GetID() == g_UserStoreBuyWnd.m_pFrame->GetID() )
				{
					g_UserStoreBuyWnd.Close();
					g_UserStoreSetBuyWnd.Close();

					return true;
				}
				else if( Node->Wnd->GetID() == g_UserStoreTitleWnd.m_pFrame->GetID() )
				{
					g_UserStoreTitleWnd.Close();

					return true;
				}
				else if( Node->Wnd->GetID() == WID_NpcChatWnd )
				{
					if( Node->Wnd->GetWindowType() == n_wtFrameWnd && Node->Wnd->IsVisible() )
					{
						//					nRui->SendEvent( Node->Wnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
						NpcChatWnd.CloseNpcChatWnd();
						return true;
					}			
				}			
			
				nRui->SendEvent(Node->Wnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0);
				
				return true;
			}
		}
		else if( Zorder == n_zoTop )
		{	
			if( Node->Wnd->GetID() == WID_ChatActFrame )
			{
				if( Node->Wnd->GetWindowType() == n_wtFrameWnd && Node->Wnd->IsVisible() )
				{
					g_ChatMainProc.SetOutState(COutState_NonAct);
					return true;
				}			
			}
		}
		else if( Zorder == n_zoBottom)
		{

			if( Node->Wnd->GetID() == WID_MiniMapWnd ) 
			{
				if(MiniMapWnd.IsMapChange(true))
				{
					return true; 
				}
			}

		}
		

		Node = GetPrev( Node );
	}
	return false;
}
//------------------------------------------------------------------------------