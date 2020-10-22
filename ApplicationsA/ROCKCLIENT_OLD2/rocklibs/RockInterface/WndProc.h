///////////////////////////////////////////////////////////////////////////////
///
///		File		: WndProc.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __WNDPROC_H__
#define __WNDPROC_H__
//-----------------------------------------------------------------------------
#include "define.h"


class CWndProc
{
public:
	CWndProc()
	{
		IsVisible	= false;
	}
	virtual ~CWndProc(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	HRESULT InitDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();
	
	virtual void CloseNumberInputWnd( SPcItem* _pc_item, int _num_item, BYTE _item_tab ){}
	virtual void SetWheelValue( short _wheel ){}
	  

	bool IsVisible;
};

//-----------------------------------------------------------------------------
#endif	__WNDPROC_H__