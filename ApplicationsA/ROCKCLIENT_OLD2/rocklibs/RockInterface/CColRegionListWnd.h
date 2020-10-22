#ifndef __CCOLREGIONLISTWND_H__
#define __CCOLREGIONLISTWND_H__
//-----------------------------------------------------------------------------

#include "Define.h"
#include "WndProc.h"
#include "ScrollBar.h"

struct SColAreaField
{
	SColAreaData*	pColAreaData;	
	CTextBox	*	pColRName;
	
	SColAreaField()
	{
		pColAreaData = NULL;
		pColRName = NULL;
	}
};

///-- 콜로니 지역 안내리스트 창

class CColRegionListWnd : public CWndProc
{
public:
	int				m_nLinkColonyCount;	
	SColAreaData	m_sCAreaData[RP_MAX_LINKED_COLONY];	
	
	CFrameWnd		* m_pColRListFrame;
	SColAreaField	m_pColRName[14];
	CButton			* m_pColRSelBtn[14];

	int				m_nStartListNum;		//시작 리스트 넘버
	int	 			m_nSelectedBtn;		    //선택버튼
	CScrollBar *	m_pScrollBar;			//스크롤바
	CButton	   *	m_pCloseBtn;			//클로즈 버튼

public:
	CColRegionListWnd();
	~CColRegionListWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	void ClearColAreaDataList();
	void Open();
	void Close();	
	void LogOff();

	void OpenColInfoWnd( SColAreaData *pColAreaData );

	void  SetListScroll( int _linenum );
	void  FieldBntSelected( int _num_btn );
	void  SetColAreaField();
};

extern CColRegionListWnd g_ColRegionListWnd;

#endif