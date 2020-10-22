#ifndef __CHARIMGBUTTON_H__
#define __CHARIMGBUTTON_H__
//-----------------------------------------------------------------------------
#include "ImageBox.h"
#include "Button.h"

enum NCButtonType
{
	CBTYPE_NONE = 0,
	CBTYPE_PARTY,
};

class CCharImgBox : public CImageBox
{
public:
	CCharImgBox() {}
	CCharImgBox( SDesktop* DT );	 
	virtual ~CCharImgBox();

	virtual void RenderProc();

	void SetRenderCharImgProperty(int unique,
								  int race, int type, bool isBackImg, bool alive);
public:
	bool m_IsMask;
protected:
	int m_unique;
	int m_race;
	int m_type;	
	bool m_isBackImg;	 
	bool m_alive;

};


class CCharImgButton : public CButton
{
public:
	 CCharImgButton() {}
	 CCharImgButton( SDesktop* DT );	 
	 virtual ~CCharImgButton();

	 virtual void RenderProc();

	 void RenderCBNoneProc();
	 void RenderCBPartyProc();

public:
	 void SetIsCharImgRender(bool flag);
	 void SetRenderCharImgProperty(int iXpos, int iYpos, int unique,
								   int race, int type);

	 int GetXpos();
	 int GetYpos();

	 void SetCButtonType(UINT nCButtonType); 
	 

protected:
	 UINT m_nCButtonType;	
	 
	 int m_iXpos;
	 int m_iYpos;
	 int m_unique;
	 int m_race;
	 int m_type;	
	 bool IsCharImgRender;	 
};

#endif	__CHARIMGBUTTON_H__