
#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "PartyInforWnd.h"

#include "..\\..\\GTimer.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"
#include "..\\..\\global.h"

#include "FrameTextBox.h"

CFrameTextBox::CFrameTextBox( SDesktop* DT ) : CTextBox(DT)
{
	m_FrameWidth = 1;
	m_FrameColor = D3DCOLOR_ARGB( 255, 255, 255, 255 ); 
}

CFrameTextBox::~CFrameTextBox()
{

}

void CFrameTextBox::RenderProc()
{
	CTextBox::RenderProc();

	RenderFrame();
}

void CFrameTextBox::SetFrameColor(D3DCOLOR color)
{
	m_FrameColor = color;
}

void CFrameTextBox::SetText( void* pStr )
{
	CTextBox::SetText(pStr);

	m_TexRect.x = Cx;
	m_TexRect.y = Cy;
	m_TexRect.w = Cw;
	m_TexRect.h = Ch;

	CalcFrameSize();

}

void CFrameTextBox::CalcFrameSize()
{
		//위
	m_NorthFrm.x = m_TexRect.x;
	m_NorthFrm.y = m_TexRect.y - m_FrameWidth;
	m_NorthFrm.w = m_TexRect.w;
	m_NorthFrm.h = m_FrameWidth;
	//
	m_Norm_astFrm.x = m_TexRect.x + m_TexRect.w;
	m_Norm_astFrm.y = m_TexRect.y - m_FrameWidth;
	m_Norm_astFrm.w = m_FrameWidth;
	m_Norm_astFrm.h = m_FrameWidth;
	//오른쪽
	m_EastFrm.x = m_TexRect.x + m_TexRect.w;
	m_EastFrm.y = m_TexRect.y;
	m_EastFrm.w = m_FrameWidth;
	m_EastFrm.h = m_TexRect.h;
	//
	m_Soum_astFrm.x = m_TexRect.x + m_TexRect.w;
	m_Soum_astFrm.y = m_TexRect.y + m_TexRect.h;
	m_Soum_astFrm.w = m_FrameWidth;
	m_Soum_astFrm.h = m_FrameWidth;
	//아래
	m_SouthFrm.x = m_TexRect.x;
	m_SouthFrm.y = m_TexRect.y + m_TexRect.h;
	m_SouthFrm.w = m_TexRect.w;
	m_SouthFrm.h = m_FrameWidth;
	//
	m_SouthWestFrm.x = m_TexRect.x - m_FrameWidth;
	m_SouthWestFrm.y = m_TexRect.y + m_TexRect.h;
	m_SouthWestFrm.w = m_FrameWidth;
	m_SouthWestFrm.h = m_FrameWidth;
	//왼쪽
	m_WestFrm.x = m_TexRect.x - m_FrameWidth;
	m_WestFrm.y = m_TexRect.y;
	m_WestFrm.w = m_FrameWidth;
	m_WestFrm.h = m_TexRect.h;
	//
	m_NorthWestFrm.x = m_TexRect.x - m_FrameWidth;
	m_NorthWestFrm.y = m_TexRect.y - m_FrameWidth;
	m_NorthWestFrm.w = m_FrameWidth;
	m_NorthWestFrm.h = m_FrameWidth;

}

void CFrameTextBox::RenderFrame()
{
	Render->Draw( TID_CLIENT, m_NorthFrm.x, m_NorthFrm.y, m_NorthFrm.w, m_NorthFrm.h, m_FrameColor );
	Render->Draw( TID_CLIENT, m_Norm_astFrm.x, m_Norm_astFrm.y, m_Norm_astFrm.w, m_Norm_astFrm.h, m_FrameColor );
	Render->Draw( TID_CLIENT, m_EastFrm.x, m_EastFrm.y, m_EastFrm.w, m_EastFrm.h, m_FrameColor );
	Render->Draw( TID_CLIENT, m_Soum_astFrm.x, m_Soum_astFrm.y, m_Soum_astFrm.w, m_Soum_astFrm.h, m_FrameColor );
	Render->Draw( TID_CLIENT, m_SouthFrm.x, m_SouthFrm.y, m_SouthFrm.w, m_SouthFrm.h, m_FrameColor );
	Render->Draw( TID_CLIENT, m_SouthWestFrm.x, m_SouthWestFrm.y, m_SouthWestFrm.w, m_SouthWestFrm.h, m_FrameColor );
	Render->Draw( TID_CLIENT, m_WestFrm.x, m_WestFrm.y, m_WestFrm.w, m_WestFrm.h, m_FrameColor );
	Render->Draw( TID_CLIENT, m_NorthWestFrm.x, m_NorthWestFrm.y, m_NorthWestFrm.w, m_NorthWestFrm.h, m_FrameColor );
}