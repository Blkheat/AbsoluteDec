#ifndef __CLOGO2DEFFECT_H__
#define __CLOGO2DEFFECT_H__
//-----------------------------------------------------------------------------

#include "rocklibs\\RockInterface\\Define.h"
#include <deque>

using namespace std;

#pragma pack(push,1)
struct SLogoAImage
{
	int		nEffAlpha;			///-- 연출 알파
	SFRect	sEffRect;			///-- 연출 영역

	SLogoAImage()
	{
		memset(this, 0, sizeof(SLogoAImage) );
	}
};
#pragma pack(pop)

enum nLogoEffStep
{
	n_LogoEffNone = 0,		///-- 아무 동작도 하지 않음
	n_LogoEffStart,			///-- 로고 연출 첫단계 - 배경이미지만 나옴...
							///-- 약간의 시간이 지나면 n_LogoEffStep1으로 이동
	n_LogoEffStep1,			///-- 좌측 Deco 표시가 좌측에서 다가온다.
							///-- 자신의 위치에 도달할 경우 n_LogoEffStep2로 이동한다.
	n_LogoEffStep2,			///-- 약간의 시간이 지나면 n_LogoEffStep3로 이동한다.
	n_LogoEffStep3,			///-- 우측 Fro 표시가 우측에서 다가온다.
							///-- 자신의 위치에 도달할 경우 n_LogoEffStep4로 이동한다.
	n_LogoEffStep4,			///-- 약간의 시간이 지나면 n_LogoEffStep5로 이동한다.
	n_LogoEffStep5,			///-- 중앙의 불꽃마크가 서서히 나탄나다.
							///-- 불곷마크가 다 나타날경우 n_LogoEffStep6로 이동한다.
	n_LogoEffStep6,			///-- 약간의 시간이 지나면... n_LogoEffEnd로 이동한다.
	n_LogoEffEnd,
};

class CLogo2DEffect
{

public:	
	CLogo2DEffect();
	virtual ~CLogo2DEffect();

	UINT	m_uNowEffStep;			///-- 현재 연출단계

	DWORD   m_dGapTime;
	DWORD   m_dPrevTime;
	
	SFRect	m_sEff1Rect;			///-- 연출 1 영역
	SFRect	m_sEff2Rect;			///-- 연출 2 영역
	SFRect	m_sEff3Rect;			///-- 연출 3 영역
	
	SFRect	m_sEffAddRect;			///-- 연출 4 영역
	SFRect	m_sLoaingRect;			///-- 로딩 글자 영역

	SFRect	m_sEff1RectUV;			///-- 연출 1 UV영역
	SFRect	m_sEff2RectUV;			///-- 연출 2 UV영역
	SFRect	m_sEff3RectUV;			///-- 연출 3 UV영역

	SFRect	m_sLoaingRectUV;		///-- 로딩 글자 UV영역

	int		m_nEff1Alpha;			///-- 연출 1 알파
	int		m_nEff2Alpha;			///-- 연출 1 알파
	int		m_nEff3Alpha;			///-- 연출 1 알파

	int		m_nEffAddAlpha;			///-- 연출 1 알파

	int		m_nLoadingAlpah;		///-- 로딩 글자 알파

	BOOL	m_bIsFlashEff;			///-- 점멸 연출여부
	DWORD   m_dFlashPrevTime;		///-- 점멸 시작 타임
	
	void	Update2DEffect();
	void	Render2DEffect(int nAlpha);

	void	Render2DEff1(int nAlpha);
	void	Render2DEff2(int nAlpha);
	void	Render2DEff3(int nAlpha);

	void	Render2DEffAdd(int nAlpha);

	void	Render2DEffLoading(int nAlpha);

	deque < SLogoAImage > m_AImageList;
	deque < SLogoAImage > m_AImageList2;
};

#endif