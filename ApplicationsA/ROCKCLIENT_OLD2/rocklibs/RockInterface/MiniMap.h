///////////////////////////////////////////////////////////////////////////////
///
///		File		: MiniMap.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __MINIMAP_H__
#define __MINIMAP_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"

#define NM_OUTRANGE 800

class CTexRender
{
public:
  


};

struct SMinimapNPCInfo
{
	int		 nNpcIndex;					//NPC Index
	TCHAR	 strNpcName[512];			//NPC Name
	int		 nCellPosX;					//NPC Cell X��ġ	
	int		 nCellPosY;					//NPC Cell Y��ġ
	int		 nQuestMarkType;			//NPC ����Ʈ ��ũŸ��
	std::vector<int> PreQuestCodeList;	//����Ʈ �ڵ� ����Ʈ
};

class CMiniMap : public CWnd
{
public:
	CMiniMap(){ }
	CMiniMap( SDesktop* DT );
	~CMiniMap();

	virtual SEventMessage* EventProc( SEventMessage* EMsg );
	virtual void RenderProc();//OR
	void CalcImgRangeInMyUnitCenter(int & indexMyUnit, int tilesize, 
							  SPoint2 & tex_pchar_pos ,SPoint2 * tex_vtx_pos );
	void CalcPosMyUnit(int & indexMyUnit, int tilesize, SPoint2 & tex_pchar_pos );
	void CalcImgRangeInPressMove(int Mx, int My );

	void RenderSimbolMarkWholeRange(int tilesize, SPoint2 * tex_vtx_pos );
	void RenderUnitMarkInViewRange(int tilesize, SPoint2 * tex_vtx_pos );
	void RenderUnitMarkOutViewRange(int tilesize, SPoint2 * tex_vtx_pos );
	void RenderMyUnitMark(int indexMyUnit, SPoint2 & tex_pchar_pos, SPoint2 * tex_vtx_pos);
	void RenderTextureMark(UINT Tid, SPoint2 CenPos, int width, int height, int iExtraGlowMark );

	BOOL IsMyUnitMove();
	BOOL IsPress();
	BOOL IsInnerRange(SPoint2 & cenPoint, SPoint2 * tex_vtx_pos, int width, int height);
	BOOL IsInnerRange2(SPoint2 & SrcCenPoint, SPoint2 * tex_vtx_pos, int width, int height);

	void CheckFocusPosition(SPoint2 Src_Cen_Pos, int width, int height, TCHAR *strName );
	void RenderFocusName();

	void SetPcInfo( SPcInfo* pcinfo );
	
	void SetMiniMap( int size, TEXID image );
	void SetMapWnd( int size, TEXID image );

	int GetMapWndSize();

	void GetTexVtxPositionX( SPoint2* _tex_char_pos, SPoint2* _tex_vtx_pos );
	void GetTexVtxPositionY( SPoint2* _tex_char_pos, SPoint2* _tex_vtx_pos );
	
	int GetAngleToDirection( int _angle );

	//-------------------------------------------------------------------------
	virtual void OnMouseFocus( int Mx, int My );//OR
	virtual void OnMouseLButtonDown( int Mx, int My );//OR
	virtual void OnMouseLButtonPress( int Mx, int My );//OR
	virtual void OnMouseLButtonUp( int Mx, int My );//OR
	virtual void OnMouseLDrag( int Mx, int My );
	//-------------------------------------------------------------------------
	
private:
	SPcInfo* thePcInfo;

	int		Real_Map_Size;
	int		Tex_Map_Size;
	int		Screen_Map_Size;

	int		Map_Wnd_Size;
	int		Map_WHalf_Size;
	int		Map_CWHalf_Size;
	int		Map_CHHalf_Size;

	TEXID	MapClientImage;
	SRect	MapClientRect;
	
	int		Border;
	TEXID	MapWndImage;
	SRect	MapWndRect;

	SPoint2	m_tex_vtx_pos[4];		 //�ʻ�����󿡼� ���� �׷����� 4������ ��ġ

	int		m_iTempCx;
	int		m_iTempCy;
	int		m_iTempCw;
	int		m_iTempCh;

	SPoint2	m_pCenterPos;
	D3DXVECTOR3    m_vPlayerPos;
	D3DXVECTOR3    m_vOldPlayerPos;
	TCHAR		   m_strFocusName[512];
	SRect		   m_FocusRect;
	int			   m_iFocusIndex;

	float		   m_fMapViewRatio;

public:
	list < SMinimapNPCInfo * >		  m_NPCInfoList;	 //���� �� ���ݿ� �����ϴ� NPC����Ʈ

public:	
	BOOL	m_bIsPress;				 //���� ���� �������ִ��� ����
	BOOL	m_bIsMapChange;			 //�� ü���� ����
	BOOL	m_bFindFocusName;		 //Ŀ���� �̸�ǥ�ð� �ʿ��� ������ �����ִ��� ����
	
protected:
	float	m_fGlowFadingTime;		 //�۷ο� ���̵�Ÿ��	
	float   m_fCumulativeTime;		 //�����ð�

public:
	TEXID	GetMapClientImage()		 { return MapClientImage; }
	int		GetRealMapSize()		 { return Real_Map_Size; }
	int		GetTexMapSize()			 { return Tex_Map_Size; }
	float   GetMapViewRatio()		 { return m_fMapViewRatio; }
	SRect * GetMapClientRect()		 { return &MapClientRect; }		
};



//-----------------------------------------------------------------------------
#endif	__MINIMAP_H__