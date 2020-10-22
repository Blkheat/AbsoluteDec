#ifndef __FRAMETEXTBOX_H__
#define __FRAMETEXTBOX_H__
//-----------------------------------------------------------------------------

#include "TextBox.h"

class CFrameTextBox : public CTextBox
{
protected:
	 SRect			m_NorthFrm;
	 SRect			m_Norm_astFrm;
	 SRect			m_EastFrm;
	 SRect			m_Soum_astFrm;
	 SRect			m_SouthFrm;
	 SRect			m_SouthWestFrm;
	 SRect			m_WestFrm;
	 SRect			m_NorthWestFrm;

	 SRect			m_TexRect;

	 int			m_FrameWidth;

	 D3DCOLOR		m_FrameColor;

	 void CalcFrameSize();
	 void RenderFrame();
public:
	 CFrameTextBox() {}
	 CFrameTextBox( SDesktop* DT );	 
	 virtual ~CFrameTextBox();

	 virtual void RenderProc();
	 virtual void SetText( void* pStr );

	 void SetFrameColor(D3DCOLOR color);
};


//-----------------------------------------------------------------------------
#endif	__FRAMETEXTBOX_H__