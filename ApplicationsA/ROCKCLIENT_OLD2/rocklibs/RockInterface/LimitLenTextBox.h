#ifndef __LIMITLENTEXBOX_H__
#define __LIMITLENTEXBOX_H__
//-----------------------------------------------------------------------------

#include "Button.h"

class CLimitLenTextBox : public CButton
{
protected:
	 int		m_nLimitLength;
	 TCHAR		m_LimitTxt[512];
	 bool		m_bEqualString;	 

public:
	 CLimitLenTextBox() {}
	 CLimitLenTextBox( SDesktop* DT );	 
	 virtual ~CLimitLenTextBox();

	 virtual SEventMessage* EventProc( SEventMessage* EMsg );
	 virtual void SetText( void* Str );
	 virtual void RenderProc();

public: 
	 void SetLimitLength(int nLength);	 
	 bool IsEqualString()	{ return m_bEqualString; }	
};

//-----------------------------------------------------------------------------
#endif	__LIMITLENTEXBOX_H__