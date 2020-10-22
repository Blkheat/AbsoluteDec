#ifndef __BZONEIN2DEFFECT_H__
#define __BZONEIN2DEFFECT_H__
//-----------------------------------------------------------------------------

#include "Define.h"

enum N2DEffectType
{
	N2DEffect_None = 0,
	N2DEffect_Trans,
	N2DEffect_View,
	N2DEffect_Hide,
};

class CBZoneIn2DEffect
{
protected:
	// 레디 연출부 필요 변수
	UINT			m_uReadyState;				//현재 레디연출상태
	UINT			m_uRedayImg;				//현재 레디 렌더 이미지
	SRect			m_sReadyMoveRect;			//레디 렌더 이동 영역
	SRect			m_sReadyNomalRect;			//레디 렌더 일반 영역
	
	SRect			m_sReadyRenderRect;			//실제 레디 렌더 영역
	float			m_fMovetime;				//레디 이동 타임
	float			m_fMCumtime;				//레디 이동 누적타임
	int				m_nReadyAlpha;				//레디 알파값

	float			m_fReadyFadetime;			//레디 페이딩 적용타임
	float			m_fRedayFCumtime;			//레디 페이딩 적용누적타임


	//스타트 연출부 필요 변수
	UINT			m_uStartState;				//현재 스타트연출상태
	UINT			m_uStartImg;				//현재 스타트 렌더 이미지
	SRect			m_sStartMiniRect;			//스타트 렌더 최소 영역
	SRect			m_sStartNormalRect;			//스타트 렌더 일반 영역
	
	SRect			m_sStartRenderRect;			//실제 스타트 렌더 영역
	SPoint2			m_sStartCenPoint;			//실제 스타트 렌더 중심점
	float			m_fSizetime;				//스타트 축소 타임
	float			m_fSCumtime;				//스타트 축소 누적타임
	int				m_nStartAlpha;				//스타트 알파값
	
	float			m_fStartViewtime;			//스타트 뷰 적용타임
	float			m_fStartVCumtime;			//스타트 뷰 적용누적타임
	
	float			m_fStartFadetime;			//스타트 페이딩 적용타임	
	float			m_fStartFCumtime;			//스타트 페이딩 적용누적타임

	bool		    m_bIsGlow;					//번쩍이는 효과 적용여부
	float			m_fGlowtime;				//글로우 적용 타임
	float			m_fGCumtime;				//글로우 적용 누적타임

public:
	CBZoneIn2DEffect();
	virtual ~CBZoneIn2DEffect();

	void InitEffect();
	void SetReadyEffect(UINT uEffectType);
	void SetStartEffect(UINT uEffectType);	
		
	void UpdateProc();
	void RenderProc();

protected:
	void InitReadyEffect();
	void InitStartEffect();
	
	void UpdateReadyProc();
	void UpdateReadyTransProc();
	void UpdateReadyViewProc();
	void UpdateReadyHideProc();
	
	void UpdateStartProc();
	void UpdateStartTransProc();
	void UpdateStartViewProc();
	void UpdateStartHideProc();

	void RenderReadyProc();
	void RenderStartProc();

};

#endif	__BZONEIN2DEFFECT_H__