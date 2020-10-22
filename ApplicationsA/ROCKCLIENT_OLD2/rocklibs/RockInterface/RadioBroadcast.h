// RadioBroadcast.h: interface for the CRadioBroadcast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADIOBROADCAST_H__B9970B93_A512_4B8E_856B_4AC20CF06F53__INCLUDED_)
#define AFX_RADIOBROADCAST_H__B9970B93_A512_4B8E_856B_4AC20CF06F53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndProc.h"

#define RADIO_BROADCAST_BTN_NUM 2

struct WNDCHECK
{
	bool			m_IsLive;
	bool			m_IsExplorer;
	DWORD			m_dwDelayTime;
	char			TileName[256];
	char			AddressName[1024];
};


class CRadioBroadcast : public CWndProc
{

	
	CFrameWnd*		m_FrameWnd;
	CButton*		m_BtnList[RADIO_BROADCAST_BTN_NUM];		
	CImageBox*		m_BtnSelect_img;
	CButton*		m_CancelBtn;
	CButton*		m_pCloseBtn;

	int				m_iSelect;
	
	WNDCHECK m_Wndcheck[2];
	
	
public:
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void ExploreRun(int Num);

	void Open();
	void Close();
	void SelectBtn(int Num);

	CRadioBroadcast();
	virtual ~CRadioBroadcast();

};

extern CRadioBroadcast *g_pRadioBroadcast;

#endif // !defined(AFX_RADIOBROADCAST_H__B9970B93_A512_4B8E_856B_4AC20CF06F53__INCLUDED_)
