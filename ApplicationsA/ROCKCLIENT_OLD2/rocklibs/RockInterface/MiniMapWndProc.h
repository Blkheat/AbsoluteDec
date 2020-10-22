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
	void  AllClearNPCInfo();								//�ʿ��� NPC������ ���� �����Ѵ�.
	void  InsertNPCInfo(SMinimapNPCInfo *pNPCInfo);			//�ʿ��� NPC������ �߰��Ѵ�.
	void  SetNPCQuestInfo(SMinimapNPCInfo *pNPCInfo);		//NPC�� ������ ����Ʈ������ �����Ѵ�.
	SMinimapNPCInfo * DeleteQuestCode( WORD quest_code,int index );
	//NPC�κ��� ����Ʈ������ �����Ѵ�.
	SMinimapNPCInfo * FindQuestNPC( int quest_code, int index );
	//����Ʈ ���� NPC�� �˻� - �ش�����Ʈ�� �����������������
	//����Ʈ�� �߰���Ų��.

protected:
	bool				IsRenLEntMap;   					//���� ��ü�� ������ ����
	CLocalEntireMap		* LocalEntMap;						//���� ��ü��	
	int			iMapType; 
public:
	bool IsRenderLEntMap()	{ return IsRenLEntMap; }
	void SetRenderLEntMap(bool bFlag);
	BOOL IsMapChange(bool bMiniMize = false);
};



extern	CMiniMapWnd	MiniMapWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_MINIMAP_WNDPROC_H__