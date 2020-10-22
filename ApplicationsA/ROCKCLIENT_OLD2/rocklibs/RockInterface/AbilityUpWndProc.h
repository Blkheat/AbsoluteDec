#if !defined( __RUI_ABILITYUP_WNDPROC_H__ )
#define __RUI_ABILITYUP_WNDPROC_H__
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
#include "Spin.h"
#include "Wnd.h"

#include "WndProc.h"



class CAbilityUpWnd : public CWndProc
{
public:
	CAbilityUpWnd()
	{
		ZeroMemory(TmpStr , 40 );

		Power_spin		= NULL;	
		Vital_spin		= NULL;	
		Stamina_spin	= NULL;	
		Dex_spin		= NULL;	
		Sympathy_spin	= NULL;	
		Int_spin		= NULL;	
		Point_Txt		= NULL;	

		memset( &Power, 0, sizeof( SValue ) );//파워
		memset( &Vital, 0, sizeof( SValue ) );//방어
		memset( &Stamina, 0, sizeof( SValue ) );//집중
		memset( &Dex, 0, sizeof( SValue ) );//반응
		memset( &Sympathy, 0, sizeof( SValue ) );//교감
		memset( &Intel, 0, sizeof( SValue ) );//저항

		LvPoint		= 0;
		cLvPoint	= 0;
	}
	~CAbilityUpWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );


	void InitPointUpProc();
	void PointUpProc( SValue* attribute, CSpin* spin, int value );
	void PointDownProc( SValue* attribute, CSpin* spin, int value );
	void Select_Race( const WORD al_race);
	
	TCHAR		TmpStr[40];
	
	CSpin*		Power_spin;	
	CSpin*		Vital_spin;	
	CSpin*		Stamina_spin;	
	CSpin*		Dex_spin;	
	CSpin*		Sympathy_spin;	
	CSpin*		Int_spin;	
	CTextBox*	Point_Txt;	
	CTextBox*	Static_Text[5];	

	SValue		Power;
	SValue		Vital;
	SValue		Stamina;
	SValue		Dex;
	SValue		Sympathy;
	SValue		Intel;

	int			LvPoint;

	int			cLvPoint;
	int			cLvMin;
	int			cLvMax;

	//변화량
	int			ValPower;
	int			ValVital;
	int			ValStamina;
	int			ValDex;
	int			ValSympathy;
	int			ValInt;
};


extern	CAbilityUpWnd	AbilityUpWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_ABILITYUP_WNDPROC_H__