#ifndef __ANNOUNCEMENT_H__
#define __ANNOUNCEMENT_H__
//-----------------------------------------------------------------------------

#include "Define.h"
#include "Fontman.h"

enum ANNOUNCETYPE
{
	AIN = 0,
	AOUT,
};

enum ANNOUNCESTATE
{
	ANONE = 0,
	AMOVE,
	AVIEW,
};

class CAnnouncement
{
private:		
		  CFontg*				m_pFontg;		  
		  SDesktop*				m_pDesktop;		
		  STransTxtData			m_ATxtData[2];
		  float					m_fACumTime[2];
		  
		  SStringInfo			m_StrInfo;
		  
		  float					m_fTransTime;
		  float					m_fRndertime;		//텍스트 렌더타임		 
		  
public:
		  CAnnouncement();
		  virtual   ~CAnnouncement();
		  void		Init(SDesktop *pDesktop);
		  void		UpdateProc();
		  void		RenderProc();

		  void		InsertAnnouncement(TCHAR *pStr ,bool bFlag = TRUE);
		  void		LogOff();
		  
		  bool		IsRenderProc();
};

extern CAnnouncement	g_Announcement;

//-----------------------------------------------------------------------------
#endif	__ANNOUNCEMENT_H__