#ifndef __RITEXTBOX_H__
#define __RITEXTBOX_H__
//-----------------------------------------------------------------------------

#include "TextBox.h"

class CRITextBox : public CTextBox
{
protected:

public:
	CRITextBox() 
	{
		m_IsMarkUp = FALSE;
	}; 

	CRITextBox( SDesktop* DT );
	virtual ~CRITextBox();

	virtual void SetText( void* Str );
	
	//-----------------------------------------------------------------------
	virtual void RenderProc();
	//-----------------------------------------------------------------------
	
	CFontg * GetFong() { return theFontg; };
	
	BOOL m_IsMarkUp;
};

//-----------------------------------------------------------------------------
#endif	__RITEXTBOX_H__