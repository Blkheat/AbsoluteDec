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

///-- �ݷδ� ���� �ȳ�����Ʈ â

class CColRegionListWnd : public CWndProc
{
public:
	int				m_nLinkColonyCount;	
	SColAreaData	m_sCAreaData[RP_MAX_LINKED_COLONY];	
	
	CFrameWnd		* m_pColRListFrame;
	SColAreaField	m_pColRName[14];
	CButton			* m_pColRSelBtn[14];

	int				m_nStartListNum;		//���� ����Ʈ �ѹ�
	int	 			m_nSelectedBtn;		    //���ù�ư
	CScrollBar *	m_pScrollBar;			//��ũ�ѹ�
	CButton	   *	m_pCloseBtn;			//Ŭ���� ��ư

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