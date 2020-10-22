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
#ifndef __NONPLAYERINFOMINIBAR_H__
#define __NONPLAYERINFOMINIBAR_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#include "Define.h"
//#include "Fontman.h"
#include "Render.h"



class CNonPlayerInfoMiniBar
{
public:
	LONG			thePickedIndex;
	bool			m_isNpcHpView;

	
public:
	CNonPlayerInfoMiniBar();
	CNonPlayerInfoMiniBar( SDesktop* DT );
	~CNonPlayerInfoMiniBar();
	
	void Initialize( SDesktop* DT );
	void RenderProc();	
		
	//모든 NPC 에너지 보여주기
	void RenderGameUIAll(int index);
	bool CalcMiniBarAll(int index);

protected:
	SDesktop*		theDT;

	SRect			theHP_Bar;
	SRect			theHP_Back;
	SRect			theSMP_Bar;
	SRect			theSMP_Back;

	
	bool CalcMiniBar();
	void RenderGameBillUI();
    void RenderGameUI();

};



//-----------------------------------------------------------------------------
#endif	__NONPLAYERINFOMINIBAR_H__


