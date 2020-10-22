///////////////////////////////////////////////////////////////////////////////
///
///		File		: ImageBox.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __IMAGEANI_H__
#define __IMAGEANI_H__
//-----------------------------------------------------------------------------
#include "TextBox.h"

///---------------------------------------------------------------------------
///-- ImageAni
///---------------------------------------------------------------------------
/// �⺻������ Play �Ǹ� �׶����� Ÿ�� �� �ð� ���ư��� . 
//////////////////////////////////////////////////////////////////////////




class CImageAni : public CTextBox
{
public:
	CImageAni(){}
	CImageAni( SDesktop* DT );
	virtual ~CImageAni();
	
	virtual void RenderProc();//OR
	void Clear();
	enum _EFFMODE{	DEFAULT_BASE , 
					SIZE_UP, // ���� ������ ��������� �Ѵ�. 
					REPEAT_ANI , //�ݴ� ���� 
					SIZE_DOWN,
	};
		
	//�⺻ ������ ���� ũ�� �ϰų� ��� ��ų��.. 
	void SetSize(int MaxSize , float Scale);
	
	//�⺻ ���� ����
	//////////////////////////////////////////////////////////////////////////
	// _EFFMODE Mode �⺻���� ȿ���� ��Ÿ�� ��带 ���� �� . ���� �߰� ���� 
	// m_MaxImgNum ���̹��� ����
	// LimitTime -1 �� ���� �ݺ� , Ÿ�������� �ű������
	// Interval �ִϸ��̼� ���� ���� �̴�.. 
	//
	void SetInfoSprite( _EFFMODE Mode , int MaxImgNum, DWORD LimitTime , DWORD Interval , int MaxImgCount , bool bStopImg =false );
	void SetPlay(bool play);
	void SetNextImageAni(CImageAni *_NextImageAni){ m_pNextImageAni = _NextImageAni;}
	void SetNextLink(bool _bNextLink){ m_bNextLink = _bNextLink;}
	bool GetEndSprite(){return m_EndSprite;}
	int	 GetImgCount(){return m_ImgCount;}
	bool GetPlay(){return m_Play;}
	
private:
	
	bool	m_bStopImg;
	_EFFMODE m_effMode; 
	int		m_MaxImgNum;	//�Ѱ��� 
	DWORD	m_LimitTime; 	//�ִϸ��̼� �� �÷��̽ð�	-1���Ѵ�
	DWORD	m_dwLimitOldTime;
	DWORD	m_IntervalTime;	//�ִϸ��̼� ���� �׸� ���޽ð�
	DWORD	m_dwOldTimer;
	bool	m_Play;		//
	int		m_NextImg;
	int		m_Tid;
	bool	m_BackPlay;	//���������� �ٽ� ó������
	float	m_fSize;
	float	m_fScale;
	int		m_iMaxSize;
	int		m_iMaxImgCount;
	int		m_ImgCount;

	int		m_nAnimTime;
	
	
	
	
	bool	m_bRepeat;
	bool m_bNextLink;
	bool m_EndSprite;
	
	CImageAni *m_pNextImageAni;


};


//-----------------------------------------------------------------------------
#endif  __IMAGEANI_H__