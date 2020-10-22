#ifndef __MESSENGERMEMOWND_H__
#define __MESSENGERMEMOWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "MLineEditBox.h"

enum MMemoState
{
	MMSendState = 0,
    MMRecvState,
};

class CMessengerMemoWnd : public CWndProc
{
protected:
	CFrameWnd *			m_pMessenMemoBgWnd;
	CButton *			m_pMMCloseBtn;
	CImageBox *			m_pMMTitleImgBox;
	CTextBox *			m_pMMTransStateTxt;	
	CTextBox *			m_pMMInterDestTxt;
	CMLineEditBox *		m_pMMInMLineEdit;
	CTextBox *		    m_pMMOutputTxt;
	CImageBox *			m_pMMScrollHideImgBox;
	CButton *			m_pMMConfirmBtn;

	UINT				m_MMState;
	
	TCHAR				m_strInterName[512];
	TCHAR				m_strTransContent[512];

public:
	CMessengerMemoWnd();
	~CMessengerMemoWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void SetMMemoState(UINT uState);
	void SetInterName(TCHAR *pStrName);
	void SetTransContent(TCHAR *pTransContent);

};

extern CMessengerMemoWnd g_MessengerMemoWnd;

//-----------------------------------------------------------------------------
#endif	__MESSENGERMEMOWND_H__
