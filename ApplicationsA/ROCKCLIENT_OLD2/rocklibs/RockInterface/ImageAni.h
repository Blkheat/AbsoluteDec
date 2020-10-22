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
/// 기본조작은 Play 되면 그때부터 타임 과 맡게 돌아간다 . 
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
					SIZE_UP, // 상자 영역을 설정해줘야 한다. 
					REPEAT_ANI , //반대 방향 
					SIZE_DOWN,
	};
		
	//기본 사이즈 보다 크게 하거나 축소 시킬때.. 
	void SetSize(int MaxSize , float Scale);
	
	//기본 정보 셋팅
	//////////////////////////////////////////////////////////////////////////
	// _EFFMODE Mode 기본적이 효과를 나타낼 모드를 선택 함 . 지속 추가 가능 
	// m_MaxImgNum 총이미지 갯수
	// LimitTime -1 은 무한 반복 , 타임지정시 거기까지만
	// Interval 애니메이션 실행 간격 이다.. 
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
	int		m_MaxImgNum;	//총갯수 
	DWORD	m_LimitTime; 	//애니메이션 총 플레이시간	-1무한대
	DWORD	m_dwLimitOldTime;
	DWORD	m_IntervalTime;	//애니메이션 다음 그림 도달시간
	DWORD	m_dwOldTimer;
	bool	m_Play;		//
	int		m_NextImg;
	int		m_Tid;
	bool	m_BackPlay;	//끝에서부터 다시 처음으로
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