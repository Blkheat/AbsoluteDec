#ifndef __FLASHFRAMEWND_H__
#define __FLASHFRAMEWND_H__
//-----------------------------------------------------------------------------

#include "FrameWnd.h"

class CFlashFrameWnd : public CFrameWnd
{
public:
	bool		m_bSwitchFlag;
	float		m_fCumulativeTime;			//누적 타임	
	bool		m_bNoRender;

protected:
	bool		m_bIsActiveFlash;
	float		m_fIntervalTime;			//점멸 인터벌타임
	
	TEXID		m_FlashImg;					//점멸 이미지
	UINT		m_FlashColorOp;				//점멸적용 칼라옵션
	D3DCOLOR	m_FlashColor;				//점멸적용 칼라값
	DWORD		m_SrcBlend;					//소스   블랜드값
	DWORD		m_DestBlend;				//데스트 블랜드값
	
	int			m_nRposX;
	int			m_nRposY;	
	int			m_nFImgWidth;				//가로길이
	int			m_nFImgHeight;				//세로길이

public:	
	CFlashFrameWnd() {}
	CFlashFrameWnd( SDesktop* DT );	 
	virtual ~CFlashFrameWnd();

	virtual void RenderProc();

public:
	void SetActiveFlash(bool bflag);
	void SetIntervalTime(float fTime);
	void SetFlashImage(UINT fImg);
	void SetFlashColor(D3DCOLOR color);
	void SetFlashColorOption(UINT ColorOp);
	void SetFlashBlendOption(DWORD SrcBlend, DWORD DestBlend);
	void SetFlashImgSize(int nRposX, int nRposY, int width, int height);
};

//-----------------------------------------------------------------------------
#endif	__FLASHFRAMEWND_H__