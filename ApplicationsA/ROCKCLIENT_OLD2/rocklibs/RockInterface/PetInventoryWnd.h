// PetInventoryWnd.h: interface for the CPetInventoryWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__RUI_PETINVENTORY_WND_H__)
#define __RUI_PETINVENTORY_WND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/********************************************************************
	펫인벤창에서는 단순의 현재의 포커스 정보 만을 받아서 처리해야한다 .. 
	전역이 아닌 지역적으로 처리한다 .
	int m_CurSlotIndex;
*********************************************************************/


#include "WndProc.h"



class CPetInventoryWnd   : public CWndProc  
{
private:
	// 소모품 아이템 개수 입력시 필요 
	CSlot*				m_SrcSlot;			
	CSlot*				m_TargetSlot;

public:

	
	int c_MaxPetInvenNum;
	
	CFrameWnd* FrameWnd;
	CButton*		CencelBtn;
	CSlot*			PetInvenSlot[4];

	
	SPcDataInven*	PcInven;


	
	void SetSrcSlot(CSlot*		Src);
	void SetTargetSlot(CSlot*	Target);

	int m_CurSlotIndex;
	

	//==============================================================================
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	//==============================================================================


	void Open();
	void Close();

	
	
	void UpdatePetPcItem();
	

	void PetInvenSlotFocus( SEventMessage* EMsg );
	void PetInvenSlotClick( SEventMessage* EMsg );
	void PetInvenSlotDBClick( SEventMessage* EMsg );
	void PetInvenSlotDrag( SEventMessage* EMsg );
	void PetInvenSlotDrop( SEventMessage* EMsg );

	
	void CloseNumberInputWnd( int aNumItem ); 


	CPetInventoryWnd(){ Init();}
	virtual ~CPetInventoryWnd(){}

};


extern CPetInventoryWnd g_PetInventoryWnd;


#endif __RUI_PETINVENTORY_WND_H__
