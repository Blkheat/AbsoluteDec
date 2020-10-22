#ifndef __PLAYERINFOADDWND_H__
#define __PLAYERINFOADDWND_H__
//-----------------------------------------------------------------------------


#include "WndProc.h"

class CPlayerInfoAddWnd : public CWndProc  
{
protected:
	CFrameWnd *		 m_pFrameWnd;
	CButton *		 m_pMyShopBtn;
	CButton *		 m_pSoActBtn;
	CButton *		 m_pPVPBtn;
	CButton *		 m_pPetInfoBtn;

	///-- 중국 작업 PK 모드 버튼
	CButton *		 m_pPKModeBtn;

	bool			 m_IsMySellShop;

public:
	CPlayerInfoAddWnd();
	virtual ~CPlayerInfoAddWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	UINT	GetFrameWndID();
	
	void	Open();
	void    Close();
};

extern CPlayerInfoAddWnd PlayerInfoAddWnd;

//-----------------------------------------------------------------------------
#endif	__PLAYERINFOADDWND_H__