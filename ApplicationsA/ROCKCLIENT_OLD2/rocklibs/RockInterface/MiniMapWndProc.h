#if !defined( __RUI_MINIMAP_WNDPROC_H__ )
#define __RUI_MINIMAP_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "MiniMap.h"
#include "Wnd.h"

#include "LocalEntireMap.h"

#include "WndProc.h"

///---------------------------------------------------------------------------
///-- CMiniMapButton 
///---------------------------------------------------------------------------
class CMiniMapButton : public CWndProc
{
public:
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
	virtual void Proc( SEventMessage* EMsg );
	void LogOff();	
	
	CFrameWnd*		fw;
	CButton*	map_btn[3];
	SRect map_menuwnd;
};
///---------------------------------------------------------------------------
///-- CMiniMapWnd 
///---------------------------------------------------------------------------
class CMiniMapWnd : public CWndProc
{
public:
	CMiniMapWnd();
	~CMiniMapWnd();

	virtual void Init();
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void LogOff();	
	
	void SetMiniMap( int size, TEXID image );

	void SetPcMapCoord( int x, int y );
	void SetMapIndex( int map_id );
	void SetMapInfoText( int size );

	CMiniMap*	Map;
	CTextBox*	InfoText128[3];
	CTextBox*	InfoText256[3];
	SPcInfo*	PcInfo;
	virtual void SetVisible( bool Visible );
	
	//by simwoosung
public:
	void  AllClearNPCInfo();								//맵연관 NPC정보를 전부 삭제한다.
	void  InsertNPCInfo(SMinimapNPCInfo *pNPCInfo);			//맵연관 NPC정보를 추가한다.
	void  SetNPCQuestInfo(SMinimapNPCInfo *pNPCInfo);		//NPC와 연관된 퀘스트정보를 설정한다.
	SMinimapNPCInfo * DeleteQuestCode( WORD quest_code,int index );
	//NPC로부터 퀘스트정보를 삭제한다.
	SMinimapNPCInfo * FindQuestNPC( int quest_code, int index );
	//퀘스트 연관 NPC를 검색 - 해당퀘스트를 가지고있지않을경우
	//퀘스트를 추가시킨다.

protected:
	bool				IsRenLEntMap;   					//지역 전체맵 렌더링 여부
	CLocalEntireMap		* LocalEntMap;						//지역 전체맵	
	int			iMapType; 
public:
	bool IsRenderLEntMap()	{ return IsRenLEntMap; }
	void SetRenderLEntMap(bool bFlag);
	BOOL IsMapChange(bool bMiniMize = false);
};



extern	CMiniMapWnd	MiniMapWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_MINIMAP_WNDPROC_H__