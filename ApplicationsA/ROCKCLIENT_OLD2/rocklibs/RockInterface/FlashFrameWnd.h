#ifndef __FLASHFRAMEWND_H__
#define __FLASHFRAMEWND_H__
//-----------------------------------------------------------------------------

#include "FrameWnd.h"

class CFlashFrameWnd : public CFrameWnd
{
public:
	bool		m_bSwitchFlag;
	float		m_fCumulativeTime;			//���� Ÿ��	
	bool		m_bNoRender;

protected:
	bool		m_bIsActiveFlash;
	float		m_fIntervalTime;			//���� ���͹�Ÿ��
	
	TEXID		m_FlashImg;					//���� �̹���
	UINT		m_FlashColorOp;				//�������� Į��ɼ�
	D3DCOLOR	m_FlashColor;				//�������� Į��
	DWORD		m_SrcBlend;					//�ҽ�   ���尪
	DWORD		m_DestBlend;				//����Ʈ ���尪
	
	int			m_nRposX;
	int			m_nRposY;	
	int			m_nFImgWidth;				//���α���
	int			m_nFImgHeight;				//���α���

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