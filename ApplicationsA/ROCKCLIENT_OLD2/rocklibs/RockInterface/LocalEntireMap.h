#ifndef __LOCALENTIREMAP_H__
#define __LOCALENTIREMAP_H__
//-----------------------------------------------------------------------------

#include "MiniMap.h"
#include "Wnd.h"

enum LEntMapState
{
	LEntState_Nomal = 0,		//일반모드
	LEntState_Viewing,			//지역전체맵 뷰
	LentState_Closing,			//지역전체맵 클로즈	
};

class CLocalEntireMap : public CWnd
{
protected:
	 SPcInfo *		m_pPcInfo;
	 CFontg  *		m_pFontg;
	
	 CMiniMap *		m_pRefMiniMap;				//참조 미니맵 포인터
	 TEXID			MapClientImage;
	 SRect			m_WndRect;
	 UINT			m_uiMapAlpha;					//맵 알파값	

	 SRect			m_NorthFrm;
	 SRect			m_Norm_astFrm;
	 SRect			m_EastFrm;
	 SRect			m_Soum_astFrm;
	 SRect			m_SouthFrm;
	 SRect			m_SouthWestFrm;
	 SRect			m_WestFrm;
	 SRect			m_NorthWestFrm;

	 int			m_FrameWidth;

	 int		    m_tilesize;				//터레인을 구성하는 한 타일크기인듯..;;
										    //실질적인 Real_Map_Size는 타일사이즈를 곱해야할듯;;
	 
	 float			m_fGlowFadingTime;		//글로우 페이딩타임	
	 float			m_fCumulativeTime;		//누적시간
	 	 
	 BOOL			m_bFindFocusName;		//커서가 이름표시가 필요한 영역에 잡혀있는지 여부
	 TCHAR		    m_strFocusName[512];
	 SRect		    m_FocusRect;

	 float			m_fEffectTime;			//연출시간
	 float			m_fEffCumTime;			//연출 누적시간	

public:
	 UINT			m_MapState;				//전체맵 진행상태	 
	
	 void  CalcFrameSize();
	 
	 void  NormalRender();					//일반렌더링
	 void  ViewingRender();					//보여지는 연출
	 void  ClosingRender();					//닫히는 연출

public:
	 CLocalEntireMap() {}
	 CLocalEntireMap( SDesktop* DT );	 
	 virtual ~CLocalEntireMap();

	 virtual void RenderProc();
	 void RenderFrame();
	 void RenderSimbolMarkWholeRange();		//상징적인 유닛 표시
	 void RenderUnitMarkInViewRange();		//가시영역 안의 유닛을 표시
	 void RenderUnitMarkOutViewRange();		//가시영역 바깥쪽의 유닛을 표시
	 void RenderMyUnitMark();				//자신의 유닛을 표시
	 void RenderLocalMapInfo();				//로컬 맵 정보 표시
	 
	 void CheckFocusPosition(SPoint2 Src_Cen_Pos, int width, int height, TCHAR *strName );
	 void RenderFocusName();
	 void RenderTextureMark(UINT Tid, SPoint2 CenPos, int width, int height, int iExtraGlowMark );	 

public:
	void SetPcInfo( SPcInfo* pcinfo );
	void SetRefMiniMap(CMiniMap * pRefMiniMap)
	{
		m_pRefMiniMap = pRefMiniMap;
	}
	void SetActiveMap(bool bFlag);
	void SetMapAlpha(UINT uiAlpha);

	void ResetActiveMap();
};

//-----------------------------------------------------------------------------
#endif	__LOCALENTIREMAP_H__