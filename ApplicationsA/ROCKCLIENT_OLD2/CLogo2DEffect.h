#ifndef __CLOGO2DEFFECT_H__
#define __CLOGO2DEFFECT_H__
//-----------------------------------------------------------------------------

#include "rocklibs\\RockInterface\\Define.h"
#include <deque>

using namespace std;

#pragma pack(push,1)
struct SLogoAImage
{
	int		nEffAlpha;			///-- ���� ����
	SFRect	sEffRect;			///-- ���� ����

	SLogoAImage()
	{
		memset(this, 0, sizeof(SLogoAImage) );
	}
};
#pragma pack(pop)

enum nLogoEffStep
{
	n_LogoEffNone = 0,		///-- �ƹ� ���۵� ���� ����
	n_LogoEffStart,			///-- �ΰ� ���� ù�ܰ� - ����̹����� ����...
							///-- �ణ�� �ð��� ������ n_LogoEffStep1���� �̵�
	n_LogoEffStep1,			///-- ���� Deco ǥ�ð� �������� �ٰ��´�.
							///-- �ڽ��� ��ġ�� ������ ��� n_LogoEffStep2�� �̵��Ѵ�.
	n_LogoEffStep2,			///-- �ణ�� �ð��� ������ n_LogoEffStep3�� �̵��Ѵ�.
	n_LogoEffStep3,			///-- ���� Fro ǥ�ð� �������� �ٰ��´�.
							///-- �ڽ��� ��ġ�� ������ ��� n_LogoEffStep4�� �̵��Ѵ�.
	n_LogoEffStep4,			///-- �ణ�� �ð��� ������ n_LogoEffStep5�� �̵��Ѵ�.
	n_LogoEffStep5,			///-- �߾��� �Ҳɸ�ũ�� ������ ��ź����.
							///-- �ҁ븶ũ�� �� ��Ÿ����� n_LogoEffStep6�� �̵��Ѵ�.
	n_LogoEffStep6,			///-- �ణ�� �ð��� ������... n_LogoEffEnd�� �̵��Ѵ�.
	n_LogoEffEnd,
};

class CLogo2DEffect
{

public:	
	CLogo2DEffect();
	virtual ~CLogo2DEffect();

	UINT	m_uNowEffStep;			///-- ���� ����ܰ�

	DWORD   m_dGapTime;
	DWORD   m_dPrevTime;
	
	SFRect	m_sEff1Rect;			///-- ���� 1 ����
	SFRect	m_sEff2Rect;			///-- ���� 2 ����
	SFRect	m_sEff3Rect;			///-- ���� 3 ����
	
	SFRect	m_sEffAddRect;			///-- ���� 4 ����
	SFRect	m_sLoaingRect;			///-- �ε� ���� ����

	SFRect	m_sEff1RectUV;			///-- ���� 1 UV����
	SFRect	m_sEff2RectUV;			///-- ���� 2 UV����
	SFRect	m_sEff3RectUV;			///-- ���� 3 UV����

	SFRect	m_sLoaingRectUV;		///-- �ε� ���� UV����

	int		m_nEff1Alpha;			///-- ���� 1 ����
	int		m_nEff2Alpha;			///-- ���� 1 ����
	int		m_nEff3Alpha;			///-- ���� 1 ����

	int		m_nEffAddAlpha;			///-- ���� 1 ����

	int		m_nLoadingAlpah;		///-- �ε� ���� ����

	BOOL	m_bIsFlashEff;			///-- ���� ���⿩��
	DWORD   m_dFlashPrevTime;		///-- ���� ���� Ÿ��
	
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