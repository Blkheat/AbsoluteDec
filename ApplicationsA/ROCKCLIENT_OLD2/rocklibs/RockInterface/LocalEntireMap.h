#ifndef __LOCALENTIREMAP_H__
#define __LOCALENTIREMAP_H__
//-----------------------------------------------------------------------------

#include "MiniMap.h"
#include "Wnd.h"

enum LEntMapState
{
	LEntState_Nomal = 0,		//�Ϲݸ��
	LEntState_Viewing,			//������ü�� ��
	LentState_Closing,			//������ü�� Ŭ����	
};

class CLocalEntireMap : public CWnd
{
protected:
	 SPcInfo *		m_pPcInfo;
	 CFontg  *		m_pFontg;
	
	 CMiniMap *		m_pRefMiniMap;				//���� �̴ϸ� ������
	 TEXID			MapClientImage;
	 SRect			m_WndRect;
	 UINT			m_uiMapAlpha;					//�� ���İ�	

	 SRect			m_NorthFrm;
	 SRect			m_Norm_astFrm;
	 SRect			m_EastFrm;
	 SRect			m_Soum_astFrm;
	 SRect			m_SouthFrm;
	 SRect			m_SouthWestFrm;
	 SRect			m_WestFrm;
	 SRect			m_NorthWestFrm;

	 int			m_FrameWidth;

	 int		    m_tilesize;				//�ͷ����� �����ϴ� �� Ÿ��ũ���ε�..;;
										    //�������� Real_Map_Size�� Ÿ�ϻ���� ���ؾ��ҵ�;;
	 
	 float			m_fGlowFadingTime;		//�۷ο� ���̵�Ÿ��	
	 float			m_fCumulativeTime;		//�����ð�
	 	 
	 BOOL			m_bFindFocusName;		//Ŀ���� �̸�ǥ�ð� �ʿ��� ������ �����ִ��� ����
	 TCHAR		    m_strFocusName[512];
	 SRect		    m_FocusRect;

	 float			m_fEffectTime;			//����ð�
	 float			m_fEffCumTime;			//���� �����ð�	

public:
	 UINT			m_MapState;				//��ü�� �������	 
	
	 void  CalcFrameSize();
	 
	 void  NormalRender();					//�Ϲݷ�����
	 void  ViewingRender();					//�������� ����
	 void  ClosingRender();					//������ ����

public:
	 CLocalEntireMap() {}
	 CLocalEntireMap( SDesktop* DT );	 
	 virtual ~CLocalEntireMap();

	 virtual void RenderProc();
	 void RenderFrame();
	 void RenderSimbolMarkWholeRange();		//��¡���� ���� ǥ��
	 void RenderUnitMarkInViewRange();		//���ÿ��� ���� ������ ǥ��
	 void RenderUnitMarkOutViewRange();		//���ÿ��� �ٱ����� ������ ǥ��
	 void RenderMyUnitMark();				//�ڽ��� ������ ǥ��
	 void RenderLocalMapInfo();				//���� �� ���� ǥ��
	 
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