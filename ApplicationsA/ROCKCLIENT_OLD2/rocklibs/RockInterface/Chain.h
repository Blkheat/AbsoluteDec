///////////////////////////////////////////////////////////////////////////////
///
///		File		: Chain.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __CHAIN_H__
#define __CHAIN_H__
//-----------------------------------------------------------------------------

#include "Define.h"
#include "Wnd.h"


struct stChainNode
{
	CWnd* Wnd;

	stChainNode* Prev;
	stChainNode* Next;
};



class CChain
{
public:
	CChain();
	~CChain();

	stChainNode* Add( CWnd* Wnd );
	void Ordering( CWnd* Wnd );
	CWnd* GetNormalWndFocusHandle();
	void Release();

	stChainNode* GetNext( stChainNode* Node );
	stChainNode* GetPrev( stChainNode* Node );
	stChainNode* GetHead();
	stChainNode* GetTail();

private:
	CWnd*			NormalWndFocus;

	stChainNode*	Head;
	stChainNode*	Tail;
	int pCount;
	stChainNode* ExtractNode( stChainNode* Node );
	void InsertNodeFirst( stChainNode* Node );
	void InsertNodeLast( stChainNode* Node );
	void InsertNodeBefore( stChainNode* Node );
	void InsertNodeAfter( stChainNode* Node );
	
public:
	bool			CloseVisibleWnd();

};

//-----------------------------------------------------------------------------
#endif  __CHAIN_H__